# CMakeList.txt : 最上位の CMake プロジェクト ファイル。グローバル構成を行います
# また、サブプロジェクトをここに含めます。
#
cmake_minimum_required (VERSION 3.8)

project ("FastUnmult")
add_executable(FastUnmult FastUnmult/FastUnmult.cpp)
# サブプロジェクトを含めます。

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(FastUnmult PUBLIC OpenMP::OpenMP_CXX)
endif()



#[[
define external libraries' path by making file named "dependencies.cmake".
variables you need to define(copy and paste, change values) : 
set(STB_PATH PATH_TO_STB_IMAGE_FOLDER)
]]

include(dependencies.cmake)
include_directories(${STB_PATH})