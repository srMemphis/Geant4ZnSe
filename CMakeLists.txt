cmake_minimum_required(VERSION 3.8...3.18)

# Project name
project(Proj)

# Find Geant4 package, activating all available UI and Vis drivers
find_package(Geant4 REQUIRED ui_all vis_all)

# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})

# Setup include directory for this project
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate sources and headers for this project
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Add the executable, and link it to the Geant4 libraries
add_executable(Proj Proj.cc ${sources} ${headers})
target_link_libraries(Proj ${Geant4_LIBRARIES})

# Copy all scripts to the build directory
set(PROJ_SCRIPTS
	vis.mac
	run.mac
)

foreach(_script ${PROJ_SCRIPTS})
	configure_file(
		${PROJECT_SOURCE_DIR}/scripts/${_script}
		${PROJECT_BINARY_DIR}/${_script}
		COPYONLY
	)
endforeach()
