cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(beginning)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(b b.cc ${sources} ${headers})
target_include_directories(b PRIVATE include)
target_link_libraries(b PRIVATE ${Geant4_LIBRARIES})

set(B_SCRIPTS
#    b.in
#    b.out
    init_vis.mac
    run.mac
    vis.mac
    )
foreach(_script ${B_SCRIPTS})
    configure_file(
        ${PROJECT_SOURCE_DIR}/${_script}
        ${PROJECT_BINARY_DIR}/${_script}
        COPYONLY
        )
endforeach()
