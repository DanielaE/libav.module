cmake_minimum_required(VERSION 3.26)

if (CMAKE_CXX_STANDARD LESS 20)
  message(FATAL_ERROR "At least C++20 required but have ${CMAKE_CXX_STANDARD}")
endif()

project(libav
        DESCRIPTION "Modularized libav for C++20"
        HOMEPAGE_URL "https://github.com/DanielaE/libav.module/tree/module"
        LANGUAGES CXX
)

if (CMAKE_VERSION VERSION_LESS 3.28)
  if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.27)
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
  else ()
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
  endif()
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
else()
  cmake_policy(VERSION 3.28)
endif()
set(CMAKE_CXX_EXTENSIONS OFF)

set(libav-sources module/libav.ixx)

set(ffmpeg-lib-paths
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
  ${CMAKE_SOURCE_DIR}/ffmpeg/lib
  ${CMAKE_SOURCE_DIR}/ffmpeg/lib
)
#set(CMAKE_FIND_DEBUG_MODE TRUE)
find_library(ffmpeg-codec  NAME avcodec.dll.a avcodec PATHS ${ffmpeg-lib-paths})
find_library(ffmpeg-filter NAME avfilter.dll.a avfilter PATHS ${ffmpeg-lib-paths})
find_library(ffmpeg-format NAME avformat.dll.a avformat PATHS ${ffmpeg-lib-paths})
find_library(ffmpeg-util   NAME avutil.dll.a avutil PATHS ${ffmpeg-lib-paths})
#set(CMAKE_FIND_DEBUG_MODE FALSE)
message(STATUS ${ffmpeg-codec})
message(STATUS ${ffmpeg-filter})
message(STATUS ${ffmpeg-format})
message(STATUS ${ffmpeg-util})

add_library(libav STATIC)
add_library(libav::libav ALIAS libav)
target_sources(libav
  PUBLIC
    FILE_SET modules TYPE CXX_MODULES
      BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/module
      FILES ${libav-sources}
)
target_compile_features(libav PUBLIC cxx_std_20)
if (MSVC)
  target_include_directories(libav BEFORE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()
target_link_libraries(libav INTERFACE ${ffmpeg-codec} ${ffmpeg-filter} ${ffmpeg-format} ${ffmpeg-util})

if (MSVC)
# right, but please don't complain about #includes in the module purview
  target_compile_options(libav PRIVATE /wd5244)
# I do mean C++23
  target_compile_options(libav PUBLIC /utf-8 /Zc:__cplusplus /Zc:throwingNew /Zc:inline /Zc:externConstexpr /Zc:templateScope /Zc:checkGwOdr /Zc:enumTypes)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
# I do mean C++20
  target_compile_options(libav PUBLIC -fsized-deallocation -faligned-allocation)
endif()
