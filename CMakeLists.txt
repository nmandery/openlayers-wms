PROJECT(OpenlayersWMS)

# Stop cmake 2.6 from whining
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

# If building for release, don't include QDebug stuff
IF("x${CMAKE_BUILD_TYPE}" STREQUAL "x")
  SET(CMAKE_BUILD_TYPE "Release")
ENDIF()
IF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  ADD_DEFINITIONS("-DQT_NO_DEBUG_OUTPUT")
ELSE()
  IF(${CMAKE_COMPILER_IS_GNUCXX})
    ADD_DEFINITIONS("-Wall")
  ENDIF()
ENDIF()

# add our own cmake modules
set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/" )

# source directories
ADD_SUBDIRECTORY( src )
