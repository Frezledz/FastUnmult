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

# include STB image to your environment using include_directories