cmake_minimum_required(VERSION 3.26)

project(MultiChannelSampler VERSION 0.0.1)

# include JUCE framework
add_subdirectory(lib/juce)
juce_add_console_app(MultiChannelSampler
  PRODUCT_NAME "Multi Channel Sampler"
)

juce_generate_juce_header(MultiChannelSampler)

set_property(TARGET MultiChannelSampler PROPERTY CXX_STANDARD 17)

target_sources(MultiChannelSampler
  PRIVATE
  src/main.cpp
  src/processor.cpp
  src/app.cpp
)

target_compile_definitions(MultiChannelSampler
  PRIVATE
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
)

# link juce
target_link_libraries(MultiChannelSampler
  PRIVATE
  juce::juce_core
  juce::juce_audio_devices
  juce::juce_audio_formats
  juce::juce_audio_basics
  juce::juce_audio_utils
  PUBLIC
  juce::juce_recommended_config_flags
  juce::juce_recommended_warning_flags
)
