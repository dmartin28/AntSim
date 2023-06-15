cmake_minimum_required(VERSION 3.23)

project(Ants LANGUAGES CXX VERSION 0.1.0)

configure_file(AntsConfig.h.in AntsConfig.h)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

add_library(AntSim STATIC)
target_sources(AntSim
    PRIVATE
      ant.cpp
      world.cpp
      colony.cpp
      pheromoneGrid.cpp
      food.cpp
      obstaclegrid.cpp
      vector3D.cpp

    PUBLIC FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}
    FILES
        ant.hpp
        world.hpp
        colony.hpp
        pheromoneGrid.hpp
        food.hpp
        obstaclegrid.hpp
        vector3D.hpp
)

install(TARGETS AntSim
    EXPORT AntSimTargets
    FILE_SET HEADERS
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include)

install(EXPORT AntSimTargets
    FILE AntSimTargets.cmake
    NAMESPACE AntSim::
    DESTINATION lib/cmake/AntSim)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "AntSimConfigVersion.cmake"
    VERSION ${AntSim_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(
    FILES
        "AntSimConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/AntSimConfigVersion.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(AntSimUnittests)
target_sources(AntSimUnittests
    PRIVATE unittests.cpp
    )

target_link_libraries(AntSimUnittests PRIVATE
          ${GTEST_LIBRARIES}
     ${GTEST_MAIN_LIBRARIES}
    AntSim
    )

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets Concurrent  REQUIRED)

add_executable(Ants)
target_sources(Ants PRIVATE
  main.cpp
  mainwindow.cpp
  mainwindow.hpp
  mainwindowform.ui
  renderarea.hpp
  renderarea.cpp
  icons/icons.qrc
  )

target_link_libraries(Ants PRIVATE
  Qt6::Widgets
  Qt6::Concurrent
  Qt6::Gui
  AntSim
  )
