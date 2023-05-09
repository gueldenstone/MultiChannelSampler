#include <juce_audio_devices/juce_audio_devices.h>

using namespace juce;

void playTestSound() {
  AudioDeviceManager deviceManager;
  deviceManager.initialise(0, 2, nullptr, true);
  deviceManager.playTestSound();
  Thread::sleep(5000);
}

//==============================================================================
int main(int argc, char *argv[]) {
  ScopedJuceInitialiser_GUI plattform;
  while (true) {
    playTestSound();
  }
  return 0;
}
