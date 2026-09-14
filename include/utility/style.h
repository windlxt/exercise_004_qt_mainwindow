#pragma once

#include <iterator>     // IWYU pragma: keep
#include <QString>

/* QSS应用位置优先级（从高到低）

1. 控件自身 setStyleSheet（最高，覆盖一切父级、全局）
2. 父容器控件 setStyleSheet
3. 祖父容器
4. qApp->setStyleSheet() 全局样式（最低）Qt文档

QSS 默认不会自动继承颜色、字体！如果想让子控件继承：`#parent, #parent * {color:red;}` 同时写父和全部后代。
*/

inline const QString StyleActivityBar = QStringLiteral(R"(         
        #activityBar QPushButton{
            border: 0px solid transparent;
            border-left:3px solid transparent; /* 预留左边3px占位，未激活透明 */
            min-width: 48px;
            min-height: 48px;
            color:#858585;
            background:transparent;
            font-size:14px;
            text-align:center;
            margin: 2px 0;
            padding:0px;            
        }
        #activityBar QPushButton:hover{color:#cccccc;background:#3c3c3c;}
        #activityBar QPushButton:checked{
            color:#ffffff;
            background:#2c2c2c;
            border-left:3px solid #007acc;
        }
    )"
);

// 个别的样式 ===================================================
inline const QString StyleTextEdit = QStringLiteral(R"(
        #textDisplayArea {
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
            margin: 5px 5px; /* 上下，左右 */

        }
        /* 焦点状态：点击选中时高亮边框 */
        #textDisplayArea:focus {
            border: 3px solid #2196F3;            
        }
        /* 只读状态样式 */
        #textDisplayArea [readOnly="true"] {
            background-color: #f6f6f6;
            color: #666666;
        }
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


// 声明应用主题调色板全局函数
void applyTheme(const QString& theme);