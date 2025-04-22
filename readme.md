# WinKeyPress 库文档

## 目录
1. [概述](#概述)
2. [快速开始](#快速开始)
3. [鼠标操作](#鼠标操作)
    - [初始化](#初始化)
    - [方法详解](#方法详解)
4. [键盘操作](#键盘操作)
    - [初始化](#初始化-1)
    - [方法详解](#方法详解-1)
5. [清理与多线程](#清理与多线程)
6. [完整示例](#完整示例)
7. [注意事项](#注意事项)
8. [更新日志](#更新日志)

---

## 概述
该库为Windows平台提供鼠标/键盘模拟、热键注册、剪贴板操作等功能，适用于自动化脚本开发。核心特性包括：
- **跨线程控制**：支持在独立线程中运行监听函数
- **链式操作**：可组合移动、点击、按键等事件
- **低延迟**：直接调用Windows API实现快速响应

---

## 快速开始
### 1. 引入头文件
```cpp
#include "winKeyPressH.hpp"
using namespace std;
```

### 2. 编译指令
```bash
g++ main.cpp -o app -lUser32 -lOle32
```

---

## 鼠标操作

### 初始化
```cpp
MouseEvent mouse; // 默认初始化
```

### 方法详解

#### 1. `GetScreenSIze` - 获取屏幕分辨率
| 参数    | 类型 | 说明                      |
|---------|------|-------------------------|
| output  | bool | 是否输出分辨率到控制台（默认true） |

**返回值**:  
`ScreenSize` 结构体，包含 `x`（宽度）、`y`（高度）属性

```cpp
ScreenSize screen = mouse.GetScreenSIze();
cout << "当前分辨率: " << screen.x << "x" << screen.y;
```

#### 2. `GetMousePosition` - 获取鼠标坐标
**返回值**:  
`MousePosition` 结构体，包含 `x`、`y` 属性

```cpp
MousePosition pos = mouse.GetMousePosition();
cout << "当前坐标: (" << pos.x << ", " << pos.y << ")";
```

#### 3. `MouseMoveTo` - 移动鼠标
| 参数 | 类型 | 说明         |
|------|------|------------|
| x    | int  | 目标X坐标（像素） |
| y    | int  | 目标Y坐标（像素） |

```cpp
mouse.MouseMoveTo(0, 0); // 移动到屏幕左上角
```

#### 4. `MouseClick` - 点击操作
| 参数     | 类型     | 可选值               | 默认值   |
|----------|----------|---------------------|--------|
| x, y     | int      | -                   | 无      |
| clicks   | int      | ≥1                  | 1       |
| button   | string   | "left", "right"     | "left" |

```cpp
// 在(100,200)处右键双击
mouse.MouseClick(100, 200, 2, "right");
```

#### 5. `MouseDown`/`MouseUp` - 按下/释放
```cpp
mouse.MouseDown();    // 按下左键
this_thread::sleep_for(chrono::seconds(1));
mouse.MouseUp();      // 释放左键
```

#### 6. `MouseRoll` - 滚轮控制
| 参数  | 类型 | 说明           |
|-------|------|--------------|
| move  | int  | 正数向上，负数向下 |

```cpp
mouse.MouseRoll(120);  // 向上滚动一页
```

#### 7. `ListenMouseEvent` - 事件监听
```cpp
/************************/

void T_getMouseCode()
{
MouseEvent m;
m.ListenMouseEvent();

}

int main()
{
thread t1(T_getMouseCode);
t1.detach();

MouseEvent m;
while (run)
{
int code = m.GetMouseCode();
if(code != 0)
{
switch (code)
{
case WM_LEFTDOWN:
cout << "left down" << endl;
break;
case WM_LEFTUP:
cout << "left up" << endl;
aa++;
break;
case WM_RIGHTDOWN:
cout << "right down" << endl;
bb++;
break;
case WM_ROLLUP:
run = false;
break;
}
}
Sleep(10);  // 添加适当延迟减少CPU占用
}


printf("left up: %d\n right down: %d\n", aa, bb);
return 0;
}
```

#### 8. `UninstallMouseEvent` - 注销鼠标监听
**说明**:  
停止鼠标事件监听线程，但不退出程序

```cpp
mouse.UninstallMouseEvent(); // 停止监听后仍可重新启动
```

---

## 键盘操作

### 初始化
```cpp
KeyBoardEvent keyboard;
```

### 方法详解

#### 1. `KeyDown`/`KeyUp` - 单键操作
| 参数 | 类型   | 示例值                |
|------|--------|----------------------|
| key  | string | "a", "ctrl", "F1"等 |

```cpp
keyboard.KeyDown("alt");
keyboard.KeyDown("tab");
keyboard.KeyUp("tab");
keyboard.KeyUp("alt"); // 切换窗口
```

#### 2. `PressKey` - 按下并释放
```cpp
keyboard.PressKey("enter"); // 模拟回车键
```

#### 3. `PressHotKey` - 组合键
| 参数    | 类型   | 说明                   |
|---------|--------|----------------------|
| key1-3  | string | 修饰键（最多3个）         |

```cpp
keyboard.PressHotKey("win", "r"); // 打开运行窗口
```

#### 4. `WriteStr` - 字符串输入
```cpp
keyboard.WriteStr("admin@123"); // 逐字符输入
```

#### 5. `CopyStr` - 剪贴板操作
```cpp
keyboard.CopyStr("待复制的文本"); 
keyboard.PressHotKey("ctrl", "v"); // 粘贴
```

#### 6. `AddHotKey` - 注册热键
| 参数            | 类型               | 说明               |
|-----------------|--------------------|------------------|
| key             | string             | 格式如 "ctrl shift k" |
| targetVoidFunc  | std::function<void()> | 触发时执行的函数       |

```cpp
keyboard.AddHotKey("ctrl+alt+t", [](){
    system("start cmd.exe"); // 打开命令行
});
```

#### 7. `WaitHotKey` - 监听热键
```cpp
thread hotkeyThread([&](){
    keyboard.WaitHotKey(); // 阻塞监听
});
hotkeyThread.detach();
```

#### 8. `UninstallKeyEvent` - 清理快捷键注册
**说明**:  
清除所有已注册的热键绑定，但不影响其他键盘操作

```cpp
keyboard.UninstallKeyEvent(); // 清理后可以重新注册新热键
```

---

## 清理与多线程
### 手动清理方法
| 方法                        | 说明                          |
|----------------------------|-----------------------------|
| `KeyBoardEvent::ClearHotKey()` | 清除所有注册热键（已弃用，建议使用UninstallKeyEvent） |
| `MouseEvent::ExitMouseEvent()` | 完全停止鼠标监听（会退出相关线程）          |
| `UninstallMouseEvent()`       | 停止鼠标监听但保持程序运行              |
| `UninstallKeyEvent()`         | 清除热键注册但保留键盘操作功能            |

### 自动清理
- 析构时自动释放未松开的按键
- 程序退出时自动执行`ExitMouseEvent()`

### 线程管理
- **必须** 将 `ListenMouseEvent` 和 `WaitHotKey` 放在独立线程
- 推荐使用 `detach` 避免主线程阻塞：
  ```cpp
  thread t([&](){ keyboard.WaitHotKey(); });
  t.detach();
  ```

---

## 完整示例
```cpp
#include <thread>
#include "winKeyPressH.hpp"

int main() {
    // 鼠标操作
    MouseEvent mouse;
    mouse.MouseMoveTo(500, 300);
    mouse.MouseClick(500, 300, 2); // 双击左键

    // 键盘操作
    KeyBoardEvent keyboard;
    keyboard.AddHotKey("ctrl shift p", [](){
        cout << "热键触发: 保存配置" << endl;
    });

    // 启动热键监听线程
    thread hotkeyThread([&](){
        keyboard.WaitHotKey();
    });
    hotkeyThread.detach();

    // 主线程其他逻辑
    while(true) {
        this_thread::sleep_for(chrono::seconds(1));
    }

    // 程序结束前清理
    mouse.UninstallMouseEvent();
    keyboard.UninstallKeyEvent();
    return 0;
}
```

---

## 注意事项
1. **管理员权限**：部分操作需以管理员身份运行程序
2. **按键冲突**：避免重复注册系统级热键（如 `alt+F4`）
3. **延迟问题**：连续操作间建议添加短暂休眠：
   ```cpp
   this_thread::sleep_for(chrono::milliseconds(50));
   ```
4. **编码问题**：`WriteStr` 仅支持ASCII字符，中文需用剪贴板方式
5. **调试建议**：监听函数启动前添加日志输出确认线程状态
6. **资源释放**：长时间运行程序时，应定期调用`Uninstall`系列方法释放系统资源

---

## 更新日志
### v1.1.0 - 2023-08-20
**新增功能**:
- 增加`MouseEvent::UninstallMouseEvent()`方法：可停止鼠标监听而不退出程序
- 增加`KeyBoardEvent::UninstallKeyEvent()`方法：清理注册热键但保留其他键盘功能

### v1.0.0 - 2023-07-15
**初始版本功能**:
- 基础鼠标/键盘模拟操作
- 热键注册与监听功能
- 剪贴板操作支持

[返回目录](#目录)