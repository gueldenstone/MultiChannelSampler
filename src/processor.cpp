#include "processor.h"

ProcessorBase::ProcessorBase(int inputs, int outputs) :
  AudioProcessor(BusesProperties()
                     .withInput("Input", AudioChannelSet::discreteChannels(inputs))
                     .withOutput("Output", AudioChannelSet::discreteChannels(outputs))) {}

MonoFilePlayerProcessor::MonoFilePlayerProcessor(File file) :
  ProcessorBase(1, 1), m_inputFile(file) {
  m_formatManager.registerBasicFormats();

  m_fmtRdrSrc = new AudioFormatReaderSource(m_formatManager.createReaderFor(file), true);
  m_source.setSource(m_fmtRdrSrc);
}

MonoFilePlayerProcessor::~MonoFilePlayerProcessor() { delete m_fmtRdrSrc; }

void MonoFilePlayerProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  m_source.prepareToPlay(static_cast<int>(sampleRate), samplesPerBlock);
}

void MonoFilePlayerProcessor::processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &) {
  m_source.getNextAudioBlock(AudioSourceChannelInfo(buffer));
}
