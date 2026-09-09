cmake_minimum_required(VERSION 4.2)

project(Demo VERSION 1.0.0.1)
# 直接包含头文件，消除红色波浪线
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #关键配置，在build中导出compile_commands.json

set(CMAKE_BUILD_TYPE Debug)

# 1. 设置C++标准 C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
        -fmodules
        -fbuiltin-module-map
        -fimplicit-modules
        -stdlib=libc++
    )
    add_link_options(
        -stdlib=libc++
        -lc++abi
    )
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# 2. 使用Ninja构建（模块编译更快，推荐）
set(CMAKE_GENERATOR "Ninja")

# 查找Qt6，按需添加组件：Widgets / Quick / Core / Network等
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup() # 自动开启AUTOMOC/AUTOUIC/AUTORCC，不用手动写moc

# 开启资源编译
set(CMAKE_AUTORCC ON)

# 3. 生成可执行文件
add_executable(demo)

# 设置Qt链接库
target_link_libraries(demo PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# 4. 设置头文件和源文件目录
file(GLOB_RECURSE SRC_LISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/include/*.hpp)

target_sources(demo PRIVATE ${SRC_LISTS} ${HEADER_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/res.qrc)
target_include_directories(demo PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/left_panel
    ${CMAKE_SOURCE_DIR}/include/right_panel
    ${CMAKE_SOURCE_DIR}/include/utility
    ${CMAKE_SOURCE_DIR}/include/algorithm
)

# 5.关键：把cppm放到CXX_MODULES文件集
# target_sources(demo
#     PRIVATE
#         FILE_SET CXX_MODULES FILES
#             src/*.cppm
# )


# ==========新增代码：构建结束后复制 compile_commands.json 到项目根目录==========
add_custom_command(TARGET demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMENT "Copy compile_commands.json to project root"
)




