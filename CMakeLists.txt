set (CMAKE_VERBOSE_MAKEFILE "on")
cmake_minimum_required (VERSION 2.6.0)
#Possible values are empty, Debug, Release, RelWithDebInfo and MinSizeRel

if (WIN32)
  if (MSVC)
    set (CMAKE_BUILD_TYPE "Release")
  else (MSVC)
    set (CMAKE_BUILD_TYPE "Debug")
  endif(MSVC)
else (WIN32)
  set (CMAKE_BUILD_TYPE "Debug")  
endif(WIN32)

set (CLEAN_DIRECT_OUTPUT true)
set (ADDITIONAL_MAKE_CLEAN_FILES 
  
)

add_subdirectory (src)
