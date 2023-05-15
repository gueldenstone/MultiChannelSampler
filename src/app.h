#pragma once
#include <JuceHeader.h>

#include "processor.h"

using AudioGraphIOProcessor = juce::AudioProcessorGraph::AudioGraphIOProcessor;
using Node = juce::AudioProcessorGraph::Node;
using NodeID = juce::AudioProcessorGraph::NodeID;
using Connection = juce::AudioProcessorGraph::Connection;

using namespace std;

class MainApp : public ConsoleApplication {
 public:
  MainApp();
  ~MainApp();

 private:
  void setupDeviceManager(string const &deviceName);

  /* -------------------------------- commands -------------------------------
   */
 private:
  void listDevices() const;
  void playSound(File const &file, int channel);

  /* --------------------------------- members --------------------------------
   */
 private:
  unique_ptr<MessageManager> m_messageManager;
  shared_ptr<AudioDeviceManager> m_deviceManager;
};

class MultiChannelSampler {
 public:
  MultiChannelSampler(shared_ptr<AudioDeviceManager> devMngr, string const &deviceName,
                      int outputs);
  MultiChannelSampler(shared_ptr<AudioDeviceManager> devMngr) :
    MultiChannelSampler(devMngr, "SQ - Audio", 4) {}

  ~MultiChannelSampler();

 public:
  void initialize();
  void playSound(File const &file, int channel);

 private:
  void initializeDeviceManager();
  void initializeEngine();

 private:
  shared_ptr<AudioDeviceManager> m_deviceManager;
  unique_ptr<AudioProcessorGraph> m_mainProcessor;
  unique_ptr<AudioProcessorPlayer> m_player;
  int m_outputs;
  string m_deviceName;
  Node::Ptr audioOutputNode;
};
