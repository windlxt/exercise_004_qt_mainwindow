## # 清理旧构建目录，清除损坏的ninja缓存
rm -rf build
cmake -S . -B build -G Ninja
cmake --build build


## Qt Widget 控件前缀
```表格
控件类	             前缀	        示例
QPushButton	        btn	    btnOk、btnCancel
QLabel	            lbl	    lblTitle、lblTipMsg
QLineEdit	        edt / le	edtUsername、lePassword
QTextEdit	        txt	    txtLog
QPlainTextEdit	    pte	    pteCode
QComboBox	        cbo	    cboLanguage
QCheckBox	        chk	    chkAutoSave
QRadioButton	    rdo	    rdoMale
QGroupBox	        grp	    grpConfig
QFrame	            fra	    fraCard
QWidget（自定义容器）  wgt	   wgtSideBar
QDialog	            dlg	    dlgSetting
QMainWindow	        win	    winMain
QSpinBox	        spn	    spnCount
QDoubleSpinBox	    dsp	    dspScale
QSlider	            sld	    sldVolume
QProgressBar	    prg	    prgLoad
QTableWidget	    tbl	    tblDeviceList
QTreeWidget	        tre	    treFileTree
QListWidget	        lst	    lstHistory
QListView / QTableView / QTreeView	view	viewData
QScrollArea	        scr	    scrContent
QTabWidget	        tab	    tabMainPages
QSplitter	        spl	    splLeftRight
QMenu	            mnu	    mnuRightClick
QAction	            act	    actSave
QToolBar	        tbr	    tbrTool
QStatusBar	        stb	    stbStatus
QDockWidget	        dock	dockLogPanel
```
## 一、下载更新clang相关软件
```bash
sudo apt install -y clang lld lldb clangd clang-tidy clang-format libc++-dev libc++abi-dev build-essential
clang++-21 --version	//查看clang++
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-21 100
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-21 100
sudo update-alternatives --install /usr/bin/cc cc /usr/bin/clang 100
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 100
```

## 二、VS‑Code配置文件
- `.vscode/tasks.json`：编译任务
- `.vscode/launch.json`：lldb调试
- `.vscode/c_cpp_properties.json`：c++编译器设置为 clang 相关

```tasks.json
{
    "version": "2.0.0",
    "options": {
        "cwd": "${workspaceFolder}/build"   // 要在工程目录下建 build 文件夹
    },
    "tasks": [
        {
            "type": "shell",
            "label": "cmake",
            "command": "cmake",
            "args": [
                "-G",
                "Ninja",                
                ".."
            ]
        },
        {
            "label": "ninja",
            "group": {
                "kind": "build",
                "isDefault": false
            },
            "command": "ninja",
            "args": []
        },
        {
            "label": "CMake ninja: build",
            "dependsOrder": "sequence",
            "dependsOn": [
                "cmake",
                "ninja"
            ]
        }
    ]
}
```

```launch.json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "clang++ debug",
            "type": "lldb",
            "request": "launch",
            "program": "${workspaceFolder}/build/demo",
            "args": [],
            "cwd": "${workspaceFolder}",
            "preLaunchTask": "CMake ninja: build"            
        }
    ]
}
```

```c_cpp_properties.json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": ["${workspaceFolder}/**"],
            "defines": [],
            "compilerPath": "/usr/bin/clang++",
            "cStandard": "c17",
            "cppStandard": "c++23",
            "intelliSenseMode": "linux-clang-x64",
            "compilerArgs": ["-Wall", "-Wextra", "-stdlib=libc++"]
        }
    ],
    "version": 4
}
```
- `├── include/            # 头文件
- `├── src/            # 源代码
- `├── build/          # CMake构建产物
- `├── .vscode/        # VS配置文件
- `├── CMakeLists.txt
- `├── compile_commands.json

## 三、CMakeLists.txt配置
```CMakeLists.txt
cmake_minimum_required(VERSION 4.2)
project(Demo VERSION 1.0.0.1)
# 直接包含头文件，消除红色波浪线
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #关键配置，在build中导出compile_commands.json

set(CMAKE_BUILD_TYPE Debug)

# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     add_compile_options(-g -O0)
# endif()

# 1. 设置C++标准 C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# 2. 使用Ninja构建（模块编译更快，推荐）
set(CMAKE_GENERATOR "Ninja")


# 3. 生成可执行文件
add_executable(demo)

# 4. 设置头文件和源文件目录  使用include 或 import 注意4与5替换
file(GLOB SRC_LISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
# file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/include/*.hpp)

target_sources(demo PRIVATE ${SRC_LISTS} ${HEADER_FILES})
# target_include_directories(demo PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 5.关键：把cppm放到CXX_MODULES文件集
target_sources(demo
    PRIVATE
        FILE_SET CXX_MODULES FILES
            src/math.cppm
)

# ==========新增代码：构建结束后复制 compile_commands.json 到项目根目录==========
add_custom_command(TARGET demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMENT "Copy compile_commands.json to project root"
)
```
## 四、为了消除红色波浪线，把自动生成的compile_commands.json自动拷贝到工程项目目录下
```compile_commands.json
[
{
  "directory": "/home/lxt/C++VsCodeProject/Clang_import_make_task_launch/build",
  "command": "/usr/bin/c++ -g -std=c++23 @CMakeFiles/demo.dir/src/main.cpp.o.modmap -o CMakeFiles/demo.dir/src/main.cpp.o -c /home/lxt/C++VsCodeProject/Clang_import_make_task_launch/src/main.cpp",
  "file": "/home/lxt/C++VsCodeProject/Clang_import_make_task_launch/src/main.cpp",
  "output": "/home/lxt/C++VsCodeProject/Clang_import_make_task_launch/build/CMakeFiles/demo.dir/src/main.cpp.o"
},
{
  "directory": "/home/lxt/C++VsCodeProject/Clang_import_make_task_launch/build",
  "command": "/usr/bin/c++ -g -std=c++23 @CMakeFiles/demo.dir/src/math.cppm.o.modmap -o CMakeFiles/demo.dir/src/math.cppm.o -c /home/lxt/C++VsCodeProject/Clang_import_make_task_launch/src/math.cppm",
  "file": "/home/lxt/C++VsCodeProject/Clang_import_make_task_launch/src/math.cppm",
  "output": "/home/lxt/C++VsCodeProject/Clang_import_make_task_launch/build/CMakeFiles/demo.dir/src/math.cppm.o"
}
]
```

## 五、安装必须的插件
核心必装插件（适配 Ubuntu‑26.04 + Clang‑18 + C++20‑Modules + Ninja‑CMake）
关键原则：** 用 clangd(LLVM 官方) 做代码解析，微软 C/C++ 插件禁用 IntelliSense，否则.cppm模块疯狂报错（比如你遇到的Expected template）。
1. 强制必备插件（只装这 3 个就能完成整套工作流）
① clangd（llvm‑vs‑code‑extensions.vscode‑clangd）【最重要】
作用：调用系统clang++/clangd做语法检查、代码补全、跳转定义、识别.cppm模块、读取compile_commands.json，解决你之前 Expected template报错的核心插件。
注意：
安装后它可以自动下载配套 clangd；优先使用 Ubuntu 系统自带 /usr/bin/clangd‑18；
.clangd文件指定compile_commands.json路径，和 CMake 编译参数完全一致；
命令重启 clangd：Ctrl+Shift+P → Restart clangd。
② CMake‑Tools（ms‑vscode.cmake‑tools，微软官方）
作用：配合 CMake 生成compile_commands.json编译数据库、选择编译器 (指定 clang++)、配置 Ninja、构建项目；
在CMakeLists.txt开启：set(CMAKE_EXPORT_COMPILE_COMMANDS ON)，build 目录产出compile_commands.json给 clangd 读取编译参数；
如果你坚持手写tasks.json构建，可以不用 CMake‑Tools，只用 tasks.json 命令行编译。
③ CodeLLDB（vadimcn.vscode‑lldb）【调试必备】
专门适配 Clang 编译产物，调用系统lldb‑18调试；launch.json里type:lldb依赖这个插件；
微软原版 C/C++ 插件的调试器对 Clang+libc++ 兼容性不如 CodeLLDB。
2. 可选配套插件（推荐安装）
twxs.cmake：CMakeLists 语法高亮、补全；
Better‑C++‑Syntax：cpp、cppm 文件语法着色优化；


注意极简安装：完全避免冲突，只靠clangd + CodeLLDB；.cppm模块环境强烈推荐这个选择。
❌ 禁忌：微软 IntelliSense 和 clangd 同时开启，一定会报波浪线错误、头文件识别异常、C++20 Modules 解析失败。


