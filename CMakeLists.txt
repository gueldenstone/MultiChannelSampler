cmake_minimum_required(VERSION 3.22)

project(MultiChannelSampler VERSION 0.0.1)

if(UNIX AND NOT APPLE)
  find_package(ALSA REQUIRED)
endif()

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
  juce::juce_dsp
  PUBLIC
  juce::juce_recommended_config_flags
  juce::juce_recommended_warning_flags
)

if(UNIX AND NOT APPLE)
  target_link_libraries(MultiChannelSampler
    PRIVATE
    ${ALSA_LIBRARIES}
  )
endif()
