cmake_minimum_required(VERSION 3.16)
project(betasim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC 默认按系统代码页(中文 Windows=GBK)读源码，会误解 UTF-8 中文注释 → 加 /utf-8
if(MSVC)
    add_compile_options(/utf-8)
endif()

# —— Geant4 —— #
# 需先 source E:\geant4\install\bin\geant4.(bat/ps1)，或给 -DGeant4_DIR=...
find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})

# —— 主引擎 betasim —— #
file(GLOB SIM_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)
add_executable(betasim ${SIM_SOURCES})
target_include_directories(betasim PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(betasim ${Geant4_LIBRARIES})

# —— 纯 C++ 单元测试（不依赖 Geant4）—— #
add_executable(test_beta ${PROJECT_SOURCE_DIR}/test/test_beta.cc)
target_include_directories(test_beta PRIVATE ${PROJECT_SOURCE_DIR}/src)
enable_testing()
add_test(NAME beta_spectrum COMMAND test_beta)

# 把 macros 复制到构建/运行目录，便于 betasim 找到默认宏
add_custom_command(TARGET betasim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/macros $<TARGET_FILE_DIR:betasim>/macros)
