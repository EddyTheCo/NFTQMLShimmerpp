# Example linking the nftMonitor library using add_subdirectory()
# set the minimum cmake to 3.24 to be able to use FIND_PACKAGE_ARGS
cmake_minimum_required(VERSION 3.11)

# set the CMake project properties
project(exampleApp VERSION 1.0 DESCRIPTION "Example application using the QML module" LANGUAGES CXX)


# set cmake variables
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


# Find Core and Gui Quick Qt6 components
# Gui and Quick components are need it because we want to create a qml module
find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick)

# https://doc.qt.io/qt-6/qt-standard-project-setup.html
qt_standard_project_setup()

# Configure an application target to test the library
qt_add_executable(example main.cpp )
qt6_add_qml_module(example
	URI  App
	VERSION 1.0
	QML_FILES
	qml/window.qml
	RESOURCE_PREFIX
	"/esterVtech.com/imports"
	IMPORT_PATH ${CMAKE_BINARY_DIR}
	)

# Link the application target with the libraries
target_link_libraries(example PRIVATE Qt::Gui Qt::Qml Qt::Quick nftMonitor )

set_target_properties(example PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
	)

# If building for browser copy files to wasm folder
if(EMSCRIPTEN)
	#The STATIC QML modules also generate the static QML plugins if NO_PLUGIN is not specified. Targets that import such STATIC QML modules also need to explicitly link to corresponding QML plugins.
	target_link_libraries(example PRIVATE nftMonitorplugin )
	add_custom_command(
		TARGET example
		POST_BUILD
		COMMAND ${CMAKE_COMMAND}
		ARGS -E copy "${CMAKE_CURRENT_BINARY_DIR}/example.js" "${CMAKE_CURRENT_BINARY_DIR}/example.wasm" "${CMAKE_CURRENT_BINARY_DIR}/qtloader.js" "${CMAKE_SOURCE_DIR}/wasm"
		)
endif()
