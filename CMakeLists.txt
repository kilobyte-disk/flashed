cmake_minimum_required(VERSION 3.16)
project(flashed LANGUAGES C VERSION 0.0.1)

#set(CXX_STANDARD 17)

# sudo pacman -S raylib
# need raylib installed as a package, and CMake will find it, and add it to compile.
find_package(raylib 3.0 REQUIRED)

# get libxml2
find_package(LibXml2 REQUIRED)

add_executable(flashed 
	# Main
	src/main.c

	# Engine2D

	# Core
	src/core/Helium.c

	# States
	src/states/engine/HlErrorState.c
	src/states/engine/HlInitState.c

	src/states/TemplateState.c
	src/states/MenuState.c
	src/states/FlashState.c

	# Instance
	src/instance/HlSprite.c
	src/instance/HlAnimation.c
	src/instance/HlDialog.c

	# Util
	src/util/HlEase.c
	src/util/HlData.c
	src/util/HlUtils.c
	src/util/Hl2DUtil.c
	src/util/HlList.c
)

# Link raylib against the program.
target_link_libraries(flashed raylib)
# Link libxml2 against the program.
target_link_libraries(flashed ${LIBXML2_LIBRARIES})
# Link math library
target_link_libraries(flashed m)

target_include_directories(flashed PUBLIC
	${LIBXML2_INCLUDE_DIR}

	${CMAKE_CURRENT_SOURCE_DIR}/src/engine2d
	${CMAKE_CURRENT_SOURCE_DIR}/src/core
	${CMAKE_CURRENT_SOURCE_DIR}/src/states
	${CMAKE_CURRENT_SOURCE_DIR}/src/states/engine
	${CMAKE_CURRENT_SOURCE_DIR}/src/instance
	${CMAKE_CURRENT_SOURCE_DIR}/src/util
)
