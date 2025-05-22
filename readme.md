[file name]: readme.md
[file content begin]
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
5. [图像识别](#图像识别)
6. [实用工具](#实用工具)
7. [清理与多线程](#清理与多线程)
8. [完整示例](#完整示例)
9. [注意事项](#注意事项)

---  

## 概述
该库为Windows平台提供鼠标/键盘模拟、热键注册、剪贴板操作、图像识别等功能，适用于自动化脚本开发。核心特性包括：
- **跨线程控制**：支持在独立线程中运行监听函数
- **链式操作**：可组合移动、点击、按键等事件
- **低延迟**：直接调用Windows API实现快速响应
- **图像识别**：支持通过模板匹配定位屏幕中的图像位置
- **编码转换**：提供UTF-8与GBK编码互转功能

---  

## 快速开始
### 1. 引入头文件
```cpp  
#include "winKeyPressH.hpp"  
using namespace std;  
```  

### 2. 编译指令
```bash  
g++ main.cpp -o app -lUser32 -lOle32 -lopencv_core -lopencv_highgui -lopencv_imgproc  
```  
*注：使用图像识别功能需链接OpenCV库。*

---  

## 鼠标操作

### 初始化
```cpp  
MouseEvent mouse; // 默认初始化  
```  

### 方法详解

#### 1. `GetScreenSize` - 获取屏幕分辨率
| 参数    | 类型 | 说明                      |  
|---------|------|-------------------------|  
| output  | bool | 是否输出分辨率到控制台（默认true） |  

**返回值**:  
`ScreenSize` 结构体，包含 `x`（宽度）、`y`（高度）属性

```cpp  
ScreenSize screen = mouse.GetScreenSize();  
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
// 需在独立线程运行  
thread mouseThread([&](){  
    mouse.ListenMouseEvent();  
});  
mouseThread.detach();  
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
keyboard.WriteStr("admin@123"); // 逐字符输入（仅支持ASCII）  
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
hotkeyThread.join();  
```  

#### 8. `ListenOneKeyEvent` - 单按键监听
```cpp  
// 在独立线程中调用监听函数  
KeyBoardEvent k;  
thread keyThread([&](){  
    k.ListenOneKeyEvent();  
});  
keyThread.detach();  

// 主循环中获取按键码  
while (true) {  
    long keyCode = k.GetOneKeyCode();  
    if (keyCode == KEY_ESC_DOWN) {  
        KeyBoardEvent::ExitOneKeyEvent(); // 退出监听  
        break;  
    }  
}  
```  

---  

## 图像类

### 一、图像识别
#### `GetImagePosition` - 获取图像在屏幕中的位置
| 参数       | 类型   | 说明                   |  
|------------|--------|----------------------|  
| filePath   | string | 模板图像路径（PNG格式以及其他格式）  |  
| threshold  | double | 匹配阈值（0.0~1.0，推荐0.85） |  

**返回值**:  
`ImagePosition` 结构体，包含 `x`（中心X坐标）、`y`（中心Y坐标）、`result`（是否匹配成功）属性

```cpp  
ImageEvent imgevent;
ImagePosition img = imgevent.GetImagePosition("img.png", 0.85);  
if (img.result) {  
    MouseEvent m;  
    m.MouseClick(img.x, img.y, 1, "left"); // 点击图像中心  
}  
```  

### 二、修改图像格式
#### `SetImageFormat` - 更改图像格式
| 参数       | 类型     | 说明                    |  
|------------|--------|-----------------------|  
| imgPath   | string | 原图像格式路径(源图文件名)        |  
| targetImageFormat  | string | 需要更改的目标格式名称（修改格式的文件名） |  

```cpp  
ImageEvent imgevent;
ImagePosition img = imgevent.GetImagePosition("img.png", 0.85);  
img.SetImageFormat("img.png", "ing.jpg");
```  

### 三、截屏功能
#### `CaptureScreen` - 截取整个屏幕
| 参数       | 类型     | 说明                    |
|------------|--------|-----------------------|
| imageName  | string | 截图保存的文件名 |

```cpp  
ImageEvent imgevent;
ImagePosition img = imgevent.GetImagePosition("img.png", 0.85);  
img.CaptureScreen("img");
```  

## 管理员判断
#### `wkp::isAdmin` - 判断是否为管理员权限
| 参数   | 类型   | 说明      |
|------|------|---------|
| void | bool | 结果返回布尔值 |

```cpp  
bool admin = wkp::isAdmin();
```  

## 以管理员身份打开可执行文件
#### `wkp::openAdmin` - 判断是否为管理员权限
| 参数   | 类型   | 说明      |
|------|------|---------|
| void | bool | 结果返回布尔值 |
```cpp  
bool admin = wkp::openAdmin();
```  

---  

## 实用工具

### 1. 字符串编码转换（StringCode类）
#### 方法：
- **`GBKToUTF8`**: 将GBK编码字符串转为UTF-8
- **`Utf8ToGBK`**: 将UTF-8编码字符串转为GBK

```cpp  
StringCode converter;  
string utf8Str = converter.GBKToUTF8("中文文本");  
string gbkStr = converter.Utf8ToGBK(utf8Str);  
```  

### 2. 获取文件夹内文件列表
#### 方法：
`GetListDir` - 返回指定目录下的所有文件和子目录名称

| 参数           | 类型   | 说明           |  
|----------------|--------|--------------|  
| directoryPath  | string | 目标文件夹路径    |  

**返回值**:  
`vector<string>` 包含文件/目录名的容器

```cpp  
vector<string> files = GetListDir("C:/MyFolder");  
for (auto& file : files) {  
    cout << file << endl;  
}  
```  

---  

## 清理与多线程
### 自动清理
- 析构时自动释放未松开的按键
- 手动清理方法：
  ```cpp  
  KeyBoardEvent::ClearHotKey(); // 清除所有注册热键  
  MouseEvent::ExitMouseEvent(); // 停止鼠标监听  
  ```  

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
    // 图像识别示例  
    ImagePosition img = GetImagePosition("button.png", 0.9);  
    if (img.result) {  
        MouseEvent mouse;  
        mouse.MouseClick(img.x, img.y); // 点击识别到的按钮  
    }  

    // 字符串编码转换示例  
    StringCode converter;  
    string utf8Text = converter.GBKToUTF8("自动化脚本");  

    // 键盘热键示例  
    KeyBoardEvent keyboard;  
    keyboard.AddHotKey("ctrl+alt+l", [](){  
        cout << "热键触发：锁定系统" << endl;  
    });  

    // 启动监听线程  
    thread hotkeyThread([&](){  
        keyboard.WaitHotKey();  
    });  
    hotkeyThread.detach();  

    while (true) {  
        this_thread::sleep_for(chrono::seconds(1));  
    }  
    return 0;  
}  
```  

---  

## 注意事项
1. **管理员权限**：部分操作需以管理员身份运行程序
2. **按键冲突**：避免重复注册系统级热键（如 `alt+F4`）
3. **图像识别依赖**：需正确安装OpenCV库并配置编译环境
4. **编码兼容性**：剪贴板操作和文件路径建议使用GBK编码
5. **调试建议**：监听函数启动前添加日志输出确认线程状态

### 更新日志
*2025/4/27更新*
1. 新增单按键监听功能 `ListenOneKeyEvent`
2. 优化 `PressHotKey` 支持任意数量组合键

*2025/5/5更新*
1. 新增图像识别功能 `GetImagePosition`

*2025/5/7更新*
1. 新增字符串编码转换类 `StringCode`
2. 新增文件夹文件列表获取函数 `GetListDir`

*2025/5/18更新*
1. 新增图像处理类 `ImageEvent`
2. MouseEvent新增功能，默认点击1次，按钮为左键 `MouseClick(int x, int y)`
3. GetImagePosition移动至图像处理类 `ImageEvent`
4. 新增命名空间 `wkp`，存在两个功能:`isAdmin`、`openAdmin`用于判断是否为管理员权限，与以管理员权限打开可执行文件
[返回目录](#目录)  
