#include "app.h"

MainApp::MainApp() :
  m_messageManager(MessageManager::getInstance()), m_deviceManager(new AudioDeviceManager()) {
  // default device manager config
  m_deviceManager->initialiseWithDefaultDevices(2, 2);

  // declare commands to be used
  addHelpCommand("--help|-h", "Usage:", true);
  addVersionCommand("--version|-v", "Multi channel sampler 0.0.1");
  addCommand({
      "list-devices",
      "list-devices",
      "Lists available devices",
      "This command lists all available devices on your computer",
      [this]([[maybe_unused]] ArgumentList const &args) { listDevices(); },
  });
  addDefaultCommand({
      "play",
      "play",
      "Plays an audio file on a specified channel",
      "",
      [this](ArgumentList const &args) {
        try {
          auto file = args.getExistingFileForOption("--file");
          auto channel = args.getValueForOption("--channel").getIntValue();
          MultiChannelSampler sampler(m_deviceManager);
          try {
            sampler.initialize();
          } catch (std::string str) {
            fail(str);
          }
          sampler.playSound(file, channel);
        } catch (exception ex) {
          fail(ex.what());
        }
      },
  });
}

MainApp::~MainApp() { m_deviceManager->removeAllChangeListeners(); }

/* -------------------------------- commands -------------------------------- */
void MainApp::listDevices() const {
  OwnedArray<AudioIODeviceType> devTypes;
  m_deviceManager->createAudioDeviceTypes(devTypes);
  for (const auto &type : devTypes) {
    cout << "[[ " << type->getTypeName() << " ]]" << endl;
    type->scanForDevices();
    for (const auto &dev : type->getDeviceNames()) {
      cout << "  - " << dev << endl;
    }
  }
}

/* --------------------------- MutliChannelSampler -------------------------- */

/**
 * @brief Construct a new Multi Channel Sampler:: Multi Channel Sampler object
 *
 * @param devMngr pointer to the device manager
 * @param deviceName name of the device we want to use
 * @param outputs number of outputs
 */
MultiChannelSampler::MultiChannelSampler(shared_ptr<AudioDeviceManager> devMngr,
                                         string const &deviceName, int outputs) :
  m_deviceManager(devMngr),
  m_mainProcessor(new AudioProcessorGraph()),
  m_player(new AudioProcessorPlayer()),
  m_outputs(outputs),
  m_deviceName(deviceName) {}

MultiChannelSampler::~MultiChannelSampler() {
  m_deviceManager->removeAudioCallback(m_player.get());
  m_mainProcessor->releaseResources();
}

/**
 * @brief Initializes the audio engine which is a AudioGraph.
 *
 */
void MultiChannelSampler::initializeEngine() {
  auto device = m_deviceManager->getCurrentAudioDevice();
  auto sampleRate = device->getCurrentSampleRate();
  auto samplesPerBlock = device->getCurrentBufferSizeSamples();

  m_mainProcessor->enableAllBuses();
  m_mainProcessor->setPlayConfigDetails(0, m_outputs, sampleRate, samplesPerBlock);
  m_mainProcessor->prepareToPlay(sampleRate, samplesPerBlock);

  m_mainProcessor->clear();
  audioOutputNode = m_mainProcessor->addNode(
      make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));
  m_player->setProcessor(m_mainProcessor.get());
  m_deviceManager->addAudioCallback(m_player.get());
}

/**
 * @brief Initializes the device manager with number of outputs and sample rate.
 *
 */
void MultiChannelSampler::initializeDeviceManager() {
  auto setup = m_deviceManager->getAudioDeviceSetup();
  setup.outputDeviceName = m_deviceName;
  setup.inputDeviceName = m_deviceName;
  setup.outputChannels = m_outputs;
  setup.inputChannels = 0;
  setup.sampleRate = 48000.0;

  auto err = m_deviceManager->initialise(0, m_outputs, nullptr, true, "", &setup);
  if (err.isNotEmpty()) {
    std::cout << err << std::endl;
  }
}

/**
 * @brief Initializes the device manager and the audio engine
 *
 */
void MultiChannelSampler::initialize() {
  initializeDeviceManager();
  initializeEngine();
}

/**
 * @brief Plays back a single file
 *
 * @param file
 * @param channel
 */
void MultiChannelSampler::playSound(juce::File const &file, int channel) {
  cout << "Playing " << file.getFullPathName() << " on channel " << channel << endl;
  auto node = m_mainProcessor->addNode(make_unique<MonoFilePlayerProcessor>(file));
  auto conn = Connection{{node->nodeID, 0}, {audioOutputNode->nodeID, channel - 1}};
  m_mainProcessor->addConnection(conn);
  if (auto proc = dynamic_cast<MonoFilePlayerProcessor *>(node->getProcessor())) {
    proc->start();
    while (proc->isPlaying()) {
      Thread::sleep(20);
    }
  };
}
