cmake_minimum_required(VERSION 3.10)

project(VisualPresentationAlgorithm)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)


set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/bin)


add_subdirectory(third_party/glew)
add_subdirectory(third_party/glfw)
add_subdirectory(third_party/glm)

include_directories(src/include third_party/CLI11)

add_executable(c_test tests/c_test.c)
target_link_libraries(c_test PRIVATE libglew_static glfw)

add_executable(cpp_test tests/cpp_test.cc)
target_link_libraries(cpp_test PRIVATE libglew_static glfw glm)

#add_executable(cc_graph graph.cc)
#target_link_libraries(cc_graph PRIVATE libglew_static glfw)

add_executable(cc_sort src/sort.cc)
target_link_libraries(cc_sort PRIVATE libglew_static glfw glm)

add_executable(cc_table src/table.cc)
target_link_libraries(cc_table PRIVATE libglew_static glfw glm)