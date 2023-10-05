
# set the minimum cmake to 3.24 to be able to use FIND_PACKAGE_ARGS
cmake_minimum_required(VERSION 3.24)

# set the CMake project properties
project(nftMonitorQml VERSION 1.0 DESCRIPTION "NFT Monitor QML module" LANGUAGES CXX)


# set cmake variables
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)

include(FetchContent)
# Get the library that check if a NFT exist on an address using the REST API of the nodes 
# This will check if the library is installed first, if not download it from GitHub and compile it
FetchContent_Declare(
	qclient
	GIT_REPOSITORY https://github.com/EddyTheCo/Qclient-IOTA.git
	GIT_TAG v0.2.3
	FIND_PACKAGE_ARGS 0.2 CONFIG
	)
FetchContent_MakeAvailable(qclient)

# Get the library that notifies when a new nft is transferred to an address using the event API of the nodes 
# https://wiki.iota.org/shimmer/tips/tips/TIP-0028/
FetchContent_Declare(
	qclientMQTT
	GIT_REPOSITORY https://github.com/EddyTheCo/QclientMqtt-IOTA.git
        GIT_TAG v0.2.3
	FIND_PACKAGE_ARGS 0.2 CONFIG
	)
FetchContent_MakeAvailable(qclientMQTT)
# Get the library that takes care of encoding and decoding bech32 addresses
FetchContent_Declare(
	qbech32
	GIT_REPOSITORY https://github.com/EddyTheCo/Qbech32.git
	GIT_TAG v0.0.2
	FIND_PACKAGE_ARGS 0.0 CONFIG
	)
FetchContent_MakeAvailable(qbech32)



# Find Core and Gui Quick Qt6 components
# Gui and Quick components are need it because we want to create a qml module
find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick)

# https://doc.qt.io/qt-6/qt-standard-project-setup.html
qt_standard_project_setup()

# Creates a CMake target (a library in this case) that expose a custom QML type using C++(defined in the SOURCES) 
# and also expose a custom QML type defined in the QML_FILES 
qt6_add_qml_module(nftMonitor
	URI  nftMonitor
	VERSION 1.0
	SOURCES
	src/nftmonitor.cpp include/nftmonitor.hpp
	OUTPUT_TARGETS out_targets_var
	QML_FILES
	qml/NFTMonitor.qml
	RESOURCE_PREFIX
	"/esterVtech.com/imports"
	IMPORT_PATH ${QML_IMPORT_PATH}
	OUTPUT_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}/nftMonitor
	)

# When compiling shared library for Windows 
# https://doc.qt.io/qt-6/sharedlibrary.html
target_compile_definitions(nftMonitor PRIVATE WINDOWS_OMONI)

# link to our library target the different Qt libraries
target_link_libraries(nftMonitor PUBLIC Qt6::Gui Qt6::Quick)



# link to our library target the different Shimmer++ libraries
# Use Private because no symbol from the Shimmer++ libraries will be exposed in this library
target_link_libraries(nftMonitor PRIVATE qbech32 qclient qclientMQTT)

# Include the headers with the exposed symbols of the library 
target_include_directories(nftMonitor PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/nftMonitor>")


# Install the library target and targets(out_targets_var) created by qt6_add_qml_module
# Associate those targets to a CMake Export
install(TARGETS nftMonitor ${out_targets_var}
	EXPORT nftMonitorTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)

# Install public headers 
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nftMonitor
	)

# Generates and installs a CMake file containing code to import the targets associated with the nftMonitorTargets EXPORT
# from the installation tree into another project
install(EXPORT nftMonitorTargets
	FILE nftMonitorTargets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nftMonitor
	)

# Create CMake Package and install it
# This will allow to use CMake find_package to link to our library
include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/nftMonitorConfig.cmake"
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nftMonitor
	)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/nftMonitorConfigVersion.cmake"
	VERSION ${VERSION}
	COMPATIBILITY SameMajorVersion
	)
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/nftMonitorConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/nftMonitorConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nftMonitor
	)
export(EXPORT nftMonitorTargets
	FILE "${CMAKE_CURRENT_BINARY_DIR}/nftMonitorTargets.cmake"
	)


# Set some Cpack configuration variables
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CPACK_PACKAGE_CONTACT "estervtech")
	set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${SEMVER}-${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_VERSION}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}")
	if(USE_THREADS)
		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-MThread")
	endif(USE_THREADS)
	include(CPack)
endif()

