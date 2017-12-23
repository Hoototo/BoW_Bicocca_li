cmake_minimum_required(VERSION 2.8)
project(DBoW2)

option(BUILD_DBoW2   "Build DBoW2"            ON)
option(BUILD_Demo64    "Build demo64 application" ON)
option(BUILD_Demo128    "Build demo128 application" ON)
option(BUILD_Demolift    "Build demolift application" ON)

set(HDRS
  include/DBoW2/BowVector.h           include/DBoW2/FBrief.h              include/DBoW2/FSurf64.h
  include/DBoW2/QueryResults.h        include/DBoW2/TemplatedDatabase.h   include/DBoW2/FORB.h
  include/DBoW2/DBoW2.h               include/DBoW2/FClass.h              include/DBoW2/FeatureVector.h
  include/DBoW2/ScoringObject.h       include/DBoW2/TemplatedVocabulary.h include/DBoW2/FSurf128.h
  include/DBoW2/FLift128.h)
set(SRCS 
  src/BowVector.cpp     src/FBrief.cpp        src/FSurf64.cpp       src/FORB.cpp
  src/FeatureVector.cpp src/QueryResults.cpp  src/ScoringObject.cpp src/FSurf128.cpp
  src/FLift128.cpp)

set(OpenCV_DIR /usr/local/opencv2.4.13/share/OpenCV)

find_package(OpenCV REQUIRED)
find_package(DLib REQUIRED)

if(BUILD_DBoW2)
  include_directories(include/DBoW2/ ${OpenCV_INCLUDE_DIRS} ${DLib_INCLUDE_DIRS})
  add_library(${PROJECT_NAME} SHARED ${SRCS})
  target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${DLib_LIBS})
endif(BUILD_DBoW2)

if(BUILD_Demo64)
  add_executable(demo64 demo/demo64.cpp)
  target_link_libraries(demo64 ${PROJECT_NAME} ${OpenCV_LIBS} ${DLib_LIBS})
  file(COPY demo/images DESTINATION ${CMAKE_BINARY_DIR}/)
endif(BUILD_Demo64)

if(BUILD_Demo128)
  add_executable(demo128 demo/demo128.cpp)
  target_link_libraries(demo128 ${PROJECT_NAME} ${OpenCV_LIBS} ${DLib_LIBS})
  file(COPY demo/images DESTINATION ${CMAKE_BINARY_DIR}/)
endif(BUILD_Demo128)

if(BUILD_Demolift)
  add_executable(demolift demo/demolift.cpp)
  target_link_libraries(demolift ${PROJECT_NAME} ${OpenCV_LIBS} ${DLib_LIBS})
  file(COPY demo/images DESTINATION ${CMAKE_BINARY_DIR}/)
endif(BUILD_Demolift)


configure_file(src/DBoW2.cmake.in
  "${PROJECT_BINARY_DIR}/DBoW2Config.cmake" @ONLY)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
if(BUILD_DBoW2)
  install(DIRECTORY include/DBoW2 DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
endif()
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/DBoW2Config.cmake"
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
install(FILES "${PROJECT_BINARY_DIR}/DBoW2Config.cmake" 
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/DBoW2/)
