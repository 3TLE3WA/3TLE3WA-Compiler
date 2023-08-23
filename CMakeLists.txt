cmake_minimum_required(VERSION 3.16)

project(compiler)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE CXXSRC
  ${CMAKE_SOURCE_DIR}/3tle3wa/src/*.cpp
  ${CMAKE_SOURCE_DIR}/antlr/*.cpp
)

# 链接 antlr4 cpp runtime
# 设置寻找路径 official path

# 以下方法适用于高版本的 antlr4
# 如何使用如下命令
# 在完成 antlr4 编译或者安装后直接做 find
# 比如在 archlinux 下安装后就是 find /usr -name antlr4*
# 找到类似这样的文件名 /usr/lib/cmake/antlr4-runtime/antlr4-runtime-config.cmake
# 这就是我们所需要的 cmake 文件，他会帮助我们找到 antlr4-runtime
find_package(antlr4-runtime REQUIRED)

# 以下方法适用于低版本的 antlr4
# link_libraries(  ... 你的 lib 路径  )
# include_directories( ... 你的头文件 )

# 设置编译参数
# 官方参数 m 在后面链接
add_compile_options(-std=c++17)
add_compile_options(-O2)

# 用于调试
# add_compile_options(-g)
add_compile_options(-Wall -Wextra)

# 用于内存检测
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

# 包含头文件
include_directories(
  3tle3wa/include
  antlr

  # 来自 antlr4-runtime-config.cmake
  # 低版本请手动设置
  ${ANTLR4_INCLUDE_DIR}
)

# 编译程序
add_executable(${PROJECT_NAME} ${CXXSRC})

target_link_libraries(${PROJECT_NAME} m antlr4-runtime)