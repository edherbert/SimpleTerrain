cmake_minimum_required(VERSION 2.8.9)
project (terrain)

set(OGRE_ROOT_DIR /home/edward/Documents/ogre)
set(OGRE_MAIN_INCLUDE ${OGRE_ROOT_DIR}/OgreMain/include)
set(OGRE_BUILD_SETTINGS_INCLUDE ${OGRE_ROOT_DIR}/build/include)
set(OGRE_PBS_INCLUDE ${OGRE_ROOT_DIR}/Components/Hlms/Pbs/include)
set(OGRE_HLMS_COMMON_INCLUDE ${OGRE_ROOT_DIR}/Components/Hlms/Common/include)

set(OGRE_LIBS ${OGRE_ROOT_DIR}/build/lib)


include_directories(${OGRE_MAIN_INCLUDE})
include_directories(${OGRE_BUILD_SETTINGS_INCLUDE})
include_directories(${OGRE_PBS_INCLUDE})
include_directories(${OGRE_HLMS_COMMON_INCLUDE})

include_directories("/usr/include/SDL2")

find_library(OGRE_MAIN
	NAMES OgreMain
	HINTS "${OGRE_LIBS}"
)

find_library(HLMS_PBS
	NAMES libOgreHlmsPbs.so
	HINTS "${OGRE_LIBS}"
)

add_executable(terrain
src/Main.cpp
src/Game.cpp
src/Terrain.cpp
src/Window.cpp
)

target_link_libraries(terrain ${OGRE_MAIN} ${HLMS_PBS} boost_system SDL2)
