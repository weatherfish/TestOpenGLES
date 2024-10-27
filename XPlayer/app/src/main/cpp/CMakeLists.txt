cmake_minimum_required(VERSION 3.22.1)

project("xplayer")

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libavcodec.so)

add_library(avdevice SHARED IMPORTED)
set_target_properties(avdevice PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libavdevice.so)

add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libavfilter.so)

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libavformat.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libavutil.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libswresample.so)

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libswscale.so)

add_library(${CMAKE_PROJECT_NAME} SHARED
        demux/IDemux.cpp
        demux/FFDemux.cpp
        XData.cpp
        XLog.cpp
        XThread.cpp
        IObserver.cpp
        decode/IDecode.cpp
        decode/FFDecode.cpp
        XParameter.cpp
        view/IVideoView.cpp
        view/GLVideoView.cpp
        view/XTexture.cpp
        view/XShader.cpp
        view/XEGL.cpp
        native-lib.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        avutil
        avcodec
        avdevice
        avfilter
        avformat
        swresample
        swscale
        EGL
        GLESv2
        android
        log)