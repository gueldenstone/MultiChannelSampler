cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(TARGET_NAME MultiChannelSampler)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(${TARGET_NAME} VERSION 0.0.1)
set(COMPANY_NAME gueldi.dev)

# ALSA for Linux (needs to be found up here)
if(UNIX AND NOT APPLE)
  find_package(ALSA REQUIRED)
endif()

# protobuf
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS protobuf/schema.proto)

# include JUCE framework
set(DONT_SET_USING_JUCE_NAMESPACE ON)
add_subdirectory(lib/juce)
juce_add_console_app(${TARGET_NAME}
  PRODUCT_NAME "Multi Channel Sampler"
)

juce_generate_juce_header(${TARGET_NAME})

# doxygen
find_package(Doxygen)

if(BUILD_DOC)
  if(DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(doc_doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_FILE}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM)
  else(DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
  endif(DOXYGEN_FOUND)
endif(BUILD_DOC)

target_sources(${TARGET_NAME}
  PRIVATE
  src/main.cpp
  src/processor.cpp
  src/app.cpp
  ${PROTO_SRCS}
  ${PROTO_HDRS}
)

target_include_directories(${TARGET_NAME}
  PRIVATE
  src
)

target_compile_definitions(${TARGET_NAME}
  PRIVATE
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
  DONT_SET_USING_JUCE_NAMESPACE=0
)

# link juce
target_link_libraries(${TARGET_NAME}
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

# link protobuf
target_link_libraries(${TARGET_NAME}
  PRIVATE
  ${Protobuf_LIBRARIES}
)

if(UNIX AND NOT APPLE)
  target_link_libraries(${TARGET_NAME}
    PRIVATE
    ${ALSA_LIBRARIES}
  )
endif()
