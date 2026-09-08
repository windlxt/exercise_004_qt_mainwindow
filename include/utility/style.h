#pragma once

#include <iterator>     // IWYU pragma: keep
#include <QString>

inline const QString StyleMainWindow = QStringLiteral(R"( 
        font-family: "Noto Sans CJK SC";
        font-size: 20px;
        font-weight: bold;
        font-style: normal;
        color: #ef6d78;
    )"
);

inline const QString StyleTextEdit = QStringLiteral(R"(
        QTextEdit {
            /* 背景底色 */
            background-color: #63b7ad;
            /* 文字颜色 */
            color: #e65151;
            /* 边框样式：线条类型 粗细 颜色 */
            border: 2px solid #e1db17;
            /* 圆角 */
            border-radius: 4px;
            /* 内边距（文字离边框距离） */
            padding: 5px;
            /* 字体 */
            font-family: "Noto Sans CJK SC";
            font-size: 20px;
        }
        /* 焦点状态：点击选中时高亮边框 */
        QTextEdit:focus {
            border: 3px solid #2196F3;
        }
        /* 只读状态样式 */
        QTextEdit[readOnly="true"] {
            background-color: #f6f6f6;
            color: #666666;
        }
    )"
);

inline const QString StyleActivityBar = QStringLiteral(R"( 
        ActivityBar{background-color:#333333;}
        QPushButton{
            border:none;
            width:48px;height:48px;
            color:#858585;
            font-size:20px;
            text-align:center;
            background:transparent;
            padding:0px;
        }
        QPushButton:hover{color:#cccccc;background:#3c3c3c;}
        QPushButton:checked{
            color:#ffffff;
            background:#2c2c2c;
            border-left:3px solid #007acc;
        }
    )"
);

inline const QString StyleSplitter = QStringLiteral(R"(
        QSplitter::handle{background:#444;width:1px;}        
        /* 水平QSplitter，handle是竖线，设置绘制宽度 */
        QSplitter::handle:horizontal {
            width: 2px;
        }
        /* 垂直QSplitter，handle是横线，设置绘制高度 */
        QSplitter::handle:vertical {
            height: 2px;
        }
        /* hover悬停样式 */
        QSplitter::handle:horizontal:hover {
            background-color: #007acc;            
        }
    )"
);

inline const QString StyleLeftPanel001 = QStringLiteral(R"(        
        QPushButton{            
            width:48px;height:48px;
            color:#007acc;
            font-size:20px;
            text-align:center;            
            padding:5px;
            margin:10px;
        }
        QPushButton:hover{color:#cccccc;background:#3c3c3c;}
    )"
);

/*
一、Qt 的 QString 不是字面类型，**不能做到编译期 consteval**。就算加 inline，QString 对象依然运行时构造。
二、如果你只是存静态样式文本：也可以用`QStringLiteral`减少开销：
`QStringLiteral`会在编译期把字符串存入 Qt 的静态字符缓冲区，**避免运行时从 char * 拷贝到 QString**，更适合 QSS 静态字符串。
*/
/*
**`#pragma once` 和 `inline` 解决完全不同层面的问题，二者不能互相替代。**
`#pragma once`：**预处理阶段，防止同一个头文件在同一个 cpp 内被重复 include**
`inline变量(C++17)`：**链接阶段，允许多个不同 cpp 都定义这个全局变量，链接合并成一份**
*/