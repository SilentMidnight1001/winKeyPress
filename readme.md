根据头文件 `winKeyPressH.hpp` 和现有 README 的对比，我发现以下功能在文档中缺失或描述不完整。以下是补充后的完整 README.md：

---

# WinKeyPress 库文档

## 目录
1. [概述](#概述)
2. [快速开始](#快速开始)
3. [鼠标操作](#鼠标操作)
4. [键盘操作](#键盘操作)
5. [图像识别](#图像识别)
6. [视频与音频操作](#视频与音频操作)
7. [字符串编码](#字符串编码)
8. [文件操作](#文件操作)
9. [弹窗与窗口控制](#弹窗与窗口控制)
10. [进程与系统管理](#进程与系统管理)
11. [实用工具](#实用工具)
12. [清理与多线程](#清理与多线程)
13. [动态库调用（DLL加载）](#动态库调用dll加载)
14. [文件夹压缩与解压](#文件夹压缩与解压)
15. [加密与解密](#加密与解密)
16. [输入交互](#输入交互)
17. [控制台控制](#控制台控制)
18. [完整示例](#完整示例)
19. [注意事项](#注意事项)
20. [更新日志](#更新日志)

---

## 概述
该库为Windows平台提供鼠标/键盘模拟、热键注册、剪贴板操作、图像识别等功能，适用于自动化脚本开发。核心特性包括：
- **跨线程控制**：支持在独立线程中运行监听函数
- **链式操作**：可组合移动、点击、按键等事件
- **低延迟**：直接调用Windows API实现快速响应
- **图像识别**：支持通过模板匹配定位屏幕中的图像位置（需OpenCV）
- **编码转换**：提供UTF-8与GBK编码互转功能，支持自动编码检测与转换
- **窗口控制**：支持窗口置顶、消息弹窗等
- **进程管理**：支持进程终止、开机自启等功能
- **字典数据结构**：提供类似Python的Dict容器，保持插入顺序
- **格式化输出**：支持类似Python的print和format功能
- **正则表达式**：提供findall、search、sub等正则操作
- **颜色输出**：支持控制台彩色文本输出
- **随机数生成**：提供多种随机数生成功能
- **文件操作**：完整的文件读写、目录操作功能，支持JSON、二进制读写
- **驱动级模拟**：支持驱动级别的键鼠模拟操作
- **视频处理**：支持视频剪切、合并、封面设置、时长获取等
- **音频处理**：支持音频合并、从视频分离音频等
- **动态库调用**：支持加载和调用Windows DLL中的函数
- **文件夹压缩**：支持文件夹压缩为二进制文件和解压
- **加密解密**：提供字符串的简单XOR加密解密功能
- **控制台控制**：支持隐藏/显示控制台、光标移动、行清除等

---

## 快速开始
### 1. 引入头文件
```cpp  
#include "winKeyPressH.hpp"  
using namespace std;  
```  

### 2. 编译指令
```bash  
# 基础编译（不使用图像识别）
g++ main.cpp -o app -lUser32 -lOle32 -lgdiplus -lwinmm

# 使用图像识别功能（需安装OpenCV）
g++ main.cpp -o app -lUser32 -lOle32 -lgdiplus -lwinmm -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs
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
`ScreenSize` 结构体，包含 `x`（宽度）、`y`（高度）、`screenZoom`（缩放比例）属性

```cpp  
ScreenSize screen = GetScreenSize();  
cout << "当前分辨率: " << screen.x << "x" << screen.y;  
cout << "缩放比例: " << screen.screenZoom * 100 << "%";
```  

#### 2. `GetMousePosition` - 获取鼠标坐标
**返回值**:  
`MousePosition` 结构体，包含 `x`、`y` 属性

```cpp  
MousePosition pos = GetMousePosition();  
cout << "当前坐标: (" << pos.x << ", " << pos.y << ")";  
```  

#### 3. `MouseMoveTo` - 移动鼠标
| 参数    | 类型    | 说明        |  
|-------|-------|-----------|  
| x     | int   | 目标X坐标（像素） |  
| y     | int   | 目标Y坐标（像素） |  
| delay | float | 移动延迟（秒）   |  

```cpp  
mouse.MouseMoveTo(0, 0, 1); // 移动到屏幕左上角，耗时1秒
```  

**驱动级版本**：
```cpp  
mouse.MouseMoveTo(100, 200, true); // 驱动级移动
mouse.MouseMoveTo(300, 400, 0.5, true); // 驱动级移动带延迟
```  

#### 4. `MouseClick` - 点击操作
| 参数     | 类型     | 可选值             | 默认值    |  
|--------|--------|-----------------|--------|  
| x, y   | int    | -               | 无      |  
| clicks | int    | ≥1              | 1      |  
| button | string | "left", "right" | "left" |  
| delay  | float  | 任意正数          | 0      |  

```cpp  
// 在(100,200)处右键双击  
mouse.MouseClick(100, 200, 2, "right", 0.5);  

// 使用结构体
MousePosition pos = {100, 200};
mouse.MouseClick(pos, 2, "left", 0.1);
```  

**驱动级版本**：
```cpp  
mouse.MouseClickDriving(100, 200, 2, "left", 0.1);
mouse.MouseClickDriving(pos, 2, "left", 0.1);
```  

#### 5. `MouseDown`/`MouseUp` - 按下/释放
```cpp  
mouse.MouseDown("left");    // 按下左键  
this_thread::sleep_for(chrono::seconds(1));  
mouse.MouseUp("left");      // 释放左键  
```  

**驱动级版本**：
```cpp  
mouse.MouseDown("left", true);
mouse.MouseUp("left", true);
```  

#### 6. `MouseRoll` - 滚轮控制
| 参数  | 类型 | 说明           |  
|-------|------|--------------|  
| move  | int  | 正数向上，负数向下 |  

```cpp  
mouse.MouseRoll(120);  // 向上滚动一页  
mouse.MouseRoll(-120); // 向下滚动一页  
```  

**驱动级版本**：
```cpp  
mouse.MouseRoll(120, true);
```  

#### 7. `MouseSideKeyPress` - 侧键操作
| 参数     | 类型     | 可选值                 | 说明 |
|--------|--------|---------------------|------|
| button | string | "forward", "backspace" | 侧键类型 |
| clicks | int    | ≥1                  | 点击次数 |
| delay  | double | 任意正数               | 延迟时间 |

```cpp  
mouse.MouseSideKeyPress("forward", 1, 0.1);  // 按下前进键  
mouse.MouseSideKeyPress("backspace", 1, 0.1); // 按下后退键  

// 独立操作
mouse.MouseSideKeyDown("forward", 1, 0.1);
mouse.MouseSideKeyUp("forward", 1, 0.1);

// 驱动级版本
mouse.MouseSideKeyPressDriving("forward", 1, 0.1);
mouse.MouseSideKeyDownDriving("forward", 1, 0.1);
mouse.MouseSideKeyUpDriving("forward", 1, 0.1);
```  

#### 8. `ListenMouseEvent` - 事件监听
支持监听鼠标侧边按钮（前进/后退）和常规按钮
```cpp  
// 需在独立线程运行  
thread mouseThread([&](){  
    mouse.ListenMouseEvent();  
});  
mouseThread.detach();  

// 获取鼠标事件码
int mouseCode = mouse.GetMouseCode();
switch(mouseCode) {
    case WM_LEFTDOWN:
        cout << "左键按下" << endl;
        break;
    case WM_LEFTUP:
        cout << "左键释放" << endl;
        break;
    case WM_FORWARDOWN:
        cout << "前进按钮按下" << endl;
        break;
    case WM_BACKSPACEDOWN:
        cout << "后退按钮按下" << endl;
        break;
}
```  

#### 9. `UninstallMouseEvent` - 注销鼠标监听事件
```cpp  
MouseEvent::UninstallMouseEvent(); // 注销所有鼠标事件，但不退出程序
```  

#### 10. `ExitMouseEvent` - 退出鼠标监听
```cpp  
MouseEvent::ExitMouseEvent(); // 停止鼠标监听并清理资源
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

**驱动级版本**：
```cpp  
keyboard.KeyDown("ctrl", true);
keyboard.KeyUp("ctrl", true);
```  

#### 2. `PressKey` - 按下并释放
```cpp  
keyboard.PressKey("enter"); // 模拟回车键  
keyboard.PressKey("space", true); // 驱动级
```  

#### 3. `PressHotKey` - 组合键
支持任意数量的组合键，自动逆序释放
```cpp  
keyboard.PressHotKey("win", "r"); // 打开运行窗口  
keyboard.PressHotKey("ctrl", "shift", "esc"); // 打开任务管理器  
```  

**驱动级版本**：
```cpp  
keyboard.PressHotKeyDriving("ctrl", "alt", "delete"); // 驱动级组合键
```  

#### 4. `WriteStr` - 字符串输入
支持UTF-8编码的中文字符
```cpp  
keyboard.WriteStr("admin@123");     // 英文输入  
keyboard.WriteStr("你好，世界！");    // 中文输入  
keyboard.WriteStr("带延迟的输入", 0.5); // 每个字符间隔0.5秒
```  

#### 5. `CopyStr` / `GetCopyStr` - 剪贴板操作
```cpp  
keyboard.CopyStr("待复制的文本");  
string text = keyboard.GetCopyStr(); // 获取剪贴板内容  
keyboard.PressHotKey("ctrl", "v");   // 粘贴  
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

// 支持任意数量组合键
keyboard.AddHotKey("ctrl+shift+alt+x", [](){  
    cout << "复杂热键触发" << endl;  
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
| 参数       | 类型 | 说明 |
|-----------|------|------|
| real_time | bool | true:实时模式，false:非实时模式（默认true） |

```cpp  
// 在独立线程中调用监听函数  
thread keyThread([&](){  
    keyboard.ListenOneKeyEvent(); // 默认实时模式  
    // 或 keyboard.ListenOneKeyEvent(false); // 非实时模式  
});  
keyThread.detach();  

// 主循环中获取按键码  
while (true) {  
    long keyCode = keyboard.GetOneKeyCode();  
    if (keyCode == KEY_ESC_DOWN) {  
        KeyBoardEvent::ExitOneKeyEvent(); // 退出监听  
        break;  
    }  
}  
```  

#### 9. `GetOneKeyCode` - 获取按键码
```cpp  
long keyCode = keyboard.GetOneKeyCode();  
if (keyCode == KEY_A_DOWN) {  
    cout << "A键被按下" << endl;  
} else if (keyCode == KEY_A_UP) {  
    cout << "A键释放" << endl;  
} else if (keyCode == KEY_CTRL_DOWN) {
    cout << "Ctrl键按下" << endl;
}
```  

#### 10. `ClearHotKey` - 清除所有注册热键
```cpp  
KeyBoardEvent::ClearHotKey();  
```  

#### 11. `UninstallHotKeyEvent` - 注销快捷键事件
```cpp  
KeyBoardEvent::UninstallHotKeyEvent(); // 注销所有快捷键，但不退出程序
```  

#### 12. `UninstallOneKeyEvent` - 注销单按键监听
```cpp  
KeyBoardEvent::UninstallOneKeyEvent(); // 注销单个按键监听，但不退出程序
```  

#### 13. `ExitOneKeyEvent` - 退出单按键监听
```cpp  
KeyBoardEvent::ExitOneKeyEvent(); // 停止单个按键监听并清理资源
```  

---

## 图像识别

### 一、图像识别
#### `GetImagePosition` - 获取图像在屏幕中的位置
| 参数       | 类型   | 说明                   |  
|------------|--------|----------------------|  
| filePath   | string | 模板图像路径（PNG格式以及其他格式）  |  
| threshold  | double | 匹配阈值（0.0~1.0，推荐0.85） |  
| useGbk     | bool   | 是否使用GBK编码（默认VS:false, MinGW:true） |  
| notException | bool | 是否抛出异常（默认false） |  

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

#### `GetImagePositionVec` - 获取图像在屏幕中的所有匹配位置
| 参数       | 类型   | 说明                   |  
|------------|--------|----------------------|  
| filePath   | string | 模板图像路径            |  
| threshold  | double | 匹配阈值（0.0~1.0）      |  
| useGbk     | bool   | 是否使用GBK编码         |  
| notException | bool | 是否抛出异常（默认false） |  
| maxMatches | int    | 最大匹配数量（默认返回所有） |  

**返回值**:  
`std::vector<ImagePosition>` 包含所有匹配位置的结构体数组

```cpp  
ImageEvent imgevent;
vector<ImagePosition> positions = imgevent.GetImagePositionVec("button.png", 0.85);
for (auto& pos : positions) {
    cout << "找到按钮位置: (" << pos.x << ", " << pos.y << ")" << endl;
}
```  

#### `GetSpecifiedAreaImagePosition` - 在指定区域内获取图像坐标
| 参数       | 类型   | 说明                   |  
|------------|--------|----------------------|  
| x1, y1     | int    | 区域左上角坐标           |  
| x2, y2     | int    | 区域右下角坐标           |  
| filePath   | string | 模板图像路径            |  
| threshold  | double | 匹配阈值（0.0~1.0）      |  
| useGbk     | bool   | 是否使用GBK编码         |  
| notException | bool | 是否抛出异常（默认false） |  

**返回值**:  
`ImagePosition` 结构体

```cpp  
ImageEvent imgevent;
// 在屏幕指定区域(100,100)-(500,500)内查找图像
ImagePosition img = imgevent.GetSpecifiedAreaImagePosition(100, 100, 500, 500, "img.png", 0.85);  
```  

#### `FindSimilarImage` - 判断图像是否存在
| 参数       | 类型   | 说明                   |  
|------------|--------|----------------------|  
| filePath   | string | 模板图像路径            |  
| threshold  | double | 匹配阈值（0.0~1.0）      |  
| useGbk     | bool   | 是否使用GBK编码         |  

**返回值**:  
`bool` - 是否找到匹配图像

```cpp  
ImageEvent imgevent;
if (imgevent.FindSimilarImage("button.png", 0.85)) {
    cout << "按钮已出现在屏幕上" << endl;
}
```  

**重载版本**：比较两个图像文件
```cpp  
if (imgevent.FindSimilarImage("template.png", "compare.png", 0.9)) {
    cout << "两个图像相似" << endl;
}
```  

#### `FindSpecifiedSimilarImage` - 在指定区域内判断图像是否存在
| 参数       | 类型   | 说明                   |  
|------------|--------|----------------------|  
| x1, y1     | int    | 区域左上角坐标           |  
| x2, y2     | int    | 区域右下角坐标           |  
| filePath   | string | 模板图像路径            |  
| threshold  | double | 匹配阈值（0.0~1.0）      |  
| useGbk     | bool   | 是否使用GBK编码         |  

**返回值**:  
`bool` - 是否找到匹配图像

```cpp  
ImageEvent imgevent;
if (imgevent.FindSpecifiedSimilarImage(100, 100, 500, 500, "button.png", 0.85)) {
    cout << "按钮在指定区域内" << endl;
}
```  

### 二、获取图像尺寸
#### `GetImageSize` - 获取图片分辨率
| 参数       | 类型   | 说明                   |  
|------------|--------|----------------------|  
| filePath   | string | 图像路径               |  
| outPutFlag | bool   | 是否输出到控制台（默认true） |  

**返回值**:  
`ImageSize` 结构体，包含 `x`（宽度）、`y`（高度）、`channels`（通道数）

```cpp  
ImageEvent imgevent;
ImageSize size = imgevent.GetImageSize("img.png");
cout << "图片尺寸: " << size.x << "x" << size.y << ", 通道数: " << size.channels << endl;
```  

### 三、修改图像格式
#### `SetImageFormat` - 更改图像格式
| 参数       | 类型     | 说明             |  
|------------|--------|----------------|  
| imgPath   | string | 原图像路径 |  
| targetImageFormat  | string | 目标格式（如"jpg"、"png"） |  
| useGbk    | bool   | 是否使用GBK编码 |  

```cpp  
ImageEvent imgevent;
imgevent.SetImageFormat("img.png", "img.jpg");
```  

### 四、修改图像尺寸
#### `SetImageSize` - 更改图像尺寸
| 参数      | 类型     | 说明             |  
|---------|--------|----------------|  
| imgPath | string | 原图像路径 |  
| width   | float  | 图片宽度（支持浮点数） |  
| height  | float  | 图片高度（支持浮点数） |  
| targetImageSavePath | string | 目标存放路径(可选) |  
| outPutName | string | 输出图片名称(可选) |  

```cpp  
ImageEvent imge;
// 仅修改大小存放到其他目录
imge.SetImageSize("../test.png", 150, 400, "D:/WinKeyPress/Output", "resized.png");

// 直接修改原图
imge.SetImageSize("test.png", 800, 600);
```  

### 五、截屏功能
#### `CaptureScreen` - 截取整个屏幕
| 参数       | 类型     | 说明         |
|------------|--------|------------|
| imageName  | string | 截图保存的文件名   |
| targetImageSavePath | string | 保存路径(可选) |

```cpp  
ImageEvent imge;
imge.CaptureScreen("screenshot"); // 保存到当前目录
imge.CaptureScreen("screenshot", "../images/"); // 保存到指定目录
```  

### 六、区域截屏
#### `CaptureAreaScreen` - 区域截图
| 参数       | 类型     | 说明          |
|----------|--------|-------------|
| saveName | string | 截图保存的文件名 |
| X1       | int    | 左上角X坐标     |
| Y1       | int    | 左上角Y坐标     |
| X2       | int    | 右下角X坐标     |
| Y2       | int    | 右下角Y坐标     |

```cpp  
ImageEvent img;
img.CaptureAreaScreen("区域截图", 654, 142, 2498, 973);
```  

### 七、异常处理
```cpp  
try {
    ImageEvent imgevent;
    ImagePosition img = imgevent.GetImagePosition("notexist.png", 0.85, true, true);
} catch (ImageNotFoundException& e) {
    cerr << "图像未找到: " << e.what() << endl;
}
```  

---

## 视频与音频操作

### 一、视频操作类 `VideoEvent`

#### `VideoArrivalTargetStop` - 视频剪切
| 参数 | 类型 | 说明 |
|------|------|------|
| input_path | string | 输入视频路径 |
| output_path | string | 输出视频路径 |
| start_sec | double | 开始时间（秒） |
| stop_sec | double | 结束时间（秒） |
| useFFMPEG | bool | 是否使用FFMPEG（推荐） |
| useGbk | bool | 是否使用GBK编码 |
| useGpu | bool | 是否使用GPU加速 |
| show_errors | bool | 是否显示错误信息 |

```cpp  
VideoEvent ve;
bool success = ve.VideoArrivalTargetStop("input.mp4", "output.mp4", 10.0, 30.0, true, true, false);
if (success) {
    cout << "视频剪切成功" << endl;
}
```  

#### `CompositeVideo` - 合并视频
| 参数 | 类型 | 说明 |
|------|------|------|
| inputPaths | vector<string> | 输入视频路径列表 |
| outputPath | string | 输出视频路径 |
| fps | double | 输出帧率（0表示保持原帧率） |
| useGpu | bool | 是否使用GPU加速 |

```cpp  
VideoEvent ve;
vector<string> videos = {"video1.mp4", "video2.mp4", "video3.mp4"};
bool success = ve.CompositeVideo(videos, "merged.mp4", 30.0, false);
if (success) {
    cout << "视频合并成功" << endl;
}
```  

#### `SetVideoCover` - 设置视频封面
| 参数 | 类型 | 说明 |
|------|------|------|
| videoPath | string | 视频文件路径 |
| imagePath | string | 封面图片路径 |
| outputPath | string | 输出视频路径 |
| useGpu | bool | 是否使用GPU加速 |

```cpp  
VideoEvent ve;
bool success = ve.SetVideoCover("video.mp4", "cover.jpg", "video_with_cover.mp4");
if (success) {
    cout << "视频封面设置成功" << endl;
}
```  

#### `GetVideoDuration` - 获取视频时长
| 参数 | 类型 | 说明 |
|------|------|------|
| videoPath | string | 视频文件路径 |
| useGbk | bool | 是否使用GBK编码 |

**返回值**: long long - 视频时长（微秒）

```cpp  
VideoEvent ve;
long long duration = ve.GetVideoDuration("video.mp4");
cout << "视频时长: " << duration / 1000000 << " 秒" << endl;
```  

#### `GetVideoCodec` - 获取视频编码格式
| 参数 | 类型 | 说明 |
|------|------|------|
| videoPath | string | 视频文件路径 |

**返回值**: string - 视频编码格式

```cpp  
VideoEvent ve;
string codec = ve.GetVideoCodec("video.mp4");
cout << "视频编码: " << codec << endl;
```  

#### `RemoveVideoAudioTrack` - 移除视频音轨
| 参数 | 类型 | 说明 |
|------|------|------|
| inputPath | string | 输入视频路径 |
| outputPath | string | 输出视频路径 |
| useGpu | bool | 是否使用GPU加速 |

```cpp  
VideoEvent ve;
bool success = ve.RemoveVideoAudioTrack("video_with_audio.mp4", "video_no_audio.mp4");
```  

#### `AddAudioToVideo` - 添加音频到视频
| 参数 | 类型 | 说明 |
|------|------|------|
| inputPath | string | 视频文件路径 |
| audioPath | string | 音频文件路径 |
| outputPath | string | 输出视频路径 |
| useGpu | bool | 是否使用GPU加速 |
| silent | bool | 是否静默模式 |

```cpp  
VideoEvent ve;
bool success = ve.AddAudioToVideo("video.mp4", "audio.mp3", "video_with_audio.mp4");
```  

### 二、音频操作类 `MusicEvent`

#### `CompositeMusic` - 合并音频
| 参数 | 类型 | 说明 |
|------|------|------|
| inputPaths | vector<string> | 输入音频路径列表 |
| outputPath | string | 输出音频路径 |
| useGpu | bool | 是否使用GPU加速 |

```cpp  
MusicEvent me;
vector<string> audios = {"audio1.mp3", "audio2.mp3", "audio3.mp3"};
bool success = me.CompositeMusic(audios, "merged.mp3");
```  

#### `SplitAudioTracks` - 从视频分离音频
| 参数 | 类型 | 说明 |
|------|------|------|
| videoPath | string | 视频文件路径 |
| audioSavePath | string | 音频保存路径 |
| useGpu | bool | 是否使用GPU加速 |

```cpp  
MusicEvent me;
bool success = me.SplitAudioTracks("video.mp4", "extracted_audio.mp3");
```  

---

## 字符串编码

### `wkp::StringCode` - 字符串编码转换类（位于wkp命名空间）

该类提供强大的字符串编码检测与转换功能，支持多种编码格式之间的相互转换。

**支持的编码检测类型：**
| 编码类型 | 说明 |
|---------|------|
| utf8bom | UTF-8 with BOM (EF BB BF) |
| utf16le | UTF-16 Little Endian (FF FE) |
| utf16be | UTF-16 Big Endian (FE FF) |
| utf32le | UTF-32 Little Endian (FF FE 00 00) |
| utf32be | UTF-32 Big Endian (00 00 FE FF) |
| utf8 | UTF-8 without BOM |
| ascii | ASCII / US-ASCII (0x00-0x7F) |
| gbk | GBK编码 (中文扩展编码) |
| gb2312 | GB2312编码 (GBK子集，简体中文) |
| big5 | Big5编码 (繁体中文) |
| latin1 | Latin-1 / ISO-8859-1 (西欧语言) |

**支持的转换目标编码：**
| 编码类型 | 说明 |
|---------|------|
| utf8 | UTF-8 without BOM (通用推荐编码) |
| utf8bom | UTF-8 with BOM (Windows常用) |
| utf16le | UTF-16 Little Endian (Windows内部编码) |
| utf16be | UTF-16 Big Endian (网络传输/其他系统) |
| utf16 | UTF-16 with BOM (自动选择字节序) |
| gbk | GBK编码 (Windows中文环境默认) |
| big5 | Big5编码 (繁体中文环境) |
| latin1 | Latin-1 / ISO-8859-1 (西欧语言) |
| ascii | ASCII编码 (仅支持0x00-0x7F字符) |

**编码名称别名支持：**
- UTF-8: utf-8, utf8
- UTF-8 with BOM: utf-8bom, utf8bom, utf-8 with bom
- UTF-16 LE: utf-16le, utf16le, utf-16 le
- UTF-16 BE: utf-16be, utf16be, utf-16 be
- UTF-16: utf-16, utf16 (Windows默认LE, 其他默认BE)
- 中文编码: gb2312 → gbk, gb18030 → gbk (统一处理)
- Latin-1: iso-8859-1 → latin1
- ASCII: us-ascii → ascii

| 函数              | 参数             | 类型                | 说明                                          |
|-----------------|----------------|-------------------|---------------------------------------------|
| GBKToUTF8       | gbkStr         | string            | 将GBK编码字符串转为UTF-8                      |
| Utf8ToGBK       | utf8Str        | string            | 将UTF-8编码字符串转为GBK                      |
| StrToWStr       | str            | string            | 将字符串转为宽字符（默认UTF-8）                 |
| StrToWStr(重载) | str, useGBK    | string, bool      | 根据布尔值转换编码(True为UTF-8转宽字符，False为GBK) |
| WStrToStr       | wstr           | wstring           | 将宽字符转为普通字符串（UTF-8）                 |
| StringChangeEncoding | str, targetEncoding | string, string | 自动检测源编码并转换为目标编码 |
| CheckStringCode | str            | string            | 检测字符串的编码类型 |

```cpp  
wkp::StringCode scd;

// 基础转换
string utf8Str = scd.GBKToUTF8("中文文本");  
string gbkStr = scd.Utf8ToGBK(utf8Str);  
wstring wstr = scd.StrToWStr("文本", true);  // UTF-8转宽字符
wstring wstr2 = scd.StrToWStr("文本", false); // GBK转宽字符
string str = scd.WStrToStr(wstr);

// 自动编码检测
string encoding = scd.CheckStringCode(utf8Str);  // 返回 "utf8"

// 智能编码转换（自动检测源编码）
string result = scd.StringChangeEncoding("中文文本", "gbk");  // UTF-8自动转GBK
string result2 = scd.StringChangeEncoding(gbkStr, "utf8");    // GBK自动转UTF-8
```  

---

## 文件操作

### 一、文件操作类 `FileEvent`
| 函数名        | 参数                                      | 返回值 | 说明                    |  
|------------|-----------------------------------------|------|-----------------------|
| RemoveFile | const std::string& fileName             | bool | 删除文件                  |
| CreateDir  | const std::string& fileName, bool useGbk | bool | 创建目录                  |
| RemoveDir  | const std::string& dirPath              | bool | 删除目录(文件夹)             |
| WriteLog   | const std::string& str, const std::string& fileName, bool utf8 | bool | 写日志（追加模式）           |
| CopyDir    | const fs::path& sourcePath, const fs::path& targetPath | bool | 复制文件夹到目标路径           |

```cpp  
FileEvent f;
bool a = f.CreateDir("../Test");
bool b = f.RemoveFile("../Test/Test.txt");
bool c = f.RemoveDir("D:/Test/test");
bool d = f.WriteLog("日志内容...", "test.log");
bool e = f.CopyDir("./source", "D:/Test/target");
```  

### 二、文件IO操作类 `wkp::Open`

该类支持文本、二进制和JSON文件的读写操作，并提供自动编码转换功能。

**构造函数/打开文件参数：**
| 参数 | 类型 | 说明 |
|------|------|------|
| file_path | string | 文件路径 |
| file_mode | string | 文件模式："r", "w", "a", "rb", "wb", "ab" |
| encoding | string | 编码设置（默认"auto"自动检测） |
| useGBK | bool | 是否使用GBK编码（已废弃，建议使用encoding参数） |
| auto_close | bool | 是否自动关闭文件（默认true） |

| 函数          | 参数                                      | 返回值                  | 说明                            |
|-------------|-----------------------------------------|-----------------------|-------------------------------|
| 构造函数       | const string &file_path, const string &file_mode, const string &encoding="auto", bool useGBK=true, bool auto_close=true | - | 打开文件 |
| open        | const string &file_path, const string &file_mode, const string &encoding="auto", bool useGBK=true, bool auto_close=true | void | 打开文件                        |
| isOpen      | -                                       | bool                  | 返回文件是否打开                    |
| read        | -                                       | string                | 读取整个文件（自动处理编码）            |
| readLine    | -                                       | vector<string>        | 逐行读取文件内容                    |
| readBinary  | -                                       | vector<unsigned char> | 读取整个二进制文件                   |
| readJson    | -                                       | Dict<string,string>   | 读取JSON文件                     |
| readBinaryToString | -                                   | string                | 二进制文件转字符串                  |
| write       | const string &str                       | void                  | 写入文件（自动处理编码）              |
| writeBinary | const vector<unsigned char>& data       | void                  | 写入二进制文件                     |
| writeJson   | Dict<K, V> dicts                        | void                  | 写入JSON文件                     |
| writeStringToBinary | const string &str                  | void                  | 字符串转二进制写入文件                |
| close       | -                                       | void                  | 关闭文件                         |

```cpp  
// 写入文件（自动编码）
wkp::Open opt;
opt.open("测试.txt", "w", "auto");
opt.write("测试写入");
opt.close();

// 读取文件（自动编码检测）
wkp::Open ipt;
ipt.open("测试.txt", "r", "auto");
string content = ipt.read();
ipt.close();
cout << content << endl;

// 逐行读取
vector<string> lines = ipt.readLine();

// 读取JSON
wkp::Open jsonFile;
jsonFile.open("data.json", "r", "auto");
auto dict = jsonFile.readJson();
cout << "name: " << dict["name"] << endl;
```  

---

## 弹窗与窗口控制

### 一、消息弹窗类 `wkp::WMessageBox`（位于wkp命名空间）
| 函数        | 参数                                      | 返回值 | 说明                |
|-----------|-----------------------------------------|------|-------------------|
| showinfo  | const fs::path title, const fs::path content | void | 显示信息提示框          |
| showwarning | const fs::path title, const fs::path content | void | 显示警告提示框          |
| showerror | const fs::path title, const fs::path content | void | 显示错误提示框          |
| askyesno  | const fs::path title, const fs::path content, const string model="info" | bool | 显示是/否选择框，返回选择结果 |

```cpp  
wkp::WMessageBox msg;
msg.showinfo("提示", "操作成功完成!");
msg.showwarning("警告", "磁盘空间不足!");
msg.showerror("错误", "文件打开失败!");

bool result = msg.askyesno("确认", "确定要删除吗?", "warning");
if (result) {
    // 用户选择"是"
} else {
    // 用户选择"否"
}
```  

### 二、窗口置顶类 `wkp::WindowTop`（位于wkp命名空间）
| 函数        | 参数                                      | 返回值 | 说明                |
|-----------|-----------------------------------------|------|-------------------|
| 构造函数     | const string& hwndName, int pxc=0, int pyc=0, int wc=0, int hc=0 | - | 通过窗口标题创建 |
| 构造函数     | HWND hwnd, int pxc=0, int pyc=0, int wc=0, int hc=0 | - | 通过窗口句柄创建 |
| setWindowsTopOn | - | void | 开启窗口置顶 |
| setWindowsTopOff | - | void | 关闭窗口置顶 |

```cpp  
// 通过窗口标题设置置顶
wkp::WindowTop top("计算器", 0, 0, 400, 300);
top.setWindowsTopOn(); // 开启置顶

// 一段时间后取消置顶
this_thread::sleep_for(chrono::seconds(10));
top.setWindowsTopOff(); // 关闭置顶

// 通过窗口句柄设置
HWND hwnd = FindWindow(nullptr, L"记事本");
if (hwnd) {
    wkp::WindowTop top2(hwnd);
    top2.setWindowsTopOn();
}
```  

### 三、窗口锁定
#### `wkp::lockWindows` - 锁定窗口为焦点
| 参数   | 类型 | 说明 |
|------|------|------|
| windowName | string | 窗口标题 |
| pid | int | 进程ID |

**返回值**: bool - 是否成功锁定

```cpp  
// 通过窗口标题锁定
bool a = wkp::lockWindows("记事本");

// 通过进程ID锁定
bool b = wkp::lockWindows(1001);
```  

---

## 进程与系统管理

### 一、进程管理
#### `wkp::KillProcess` - 终止进程（基础版）
| 函数   | 参数     | 返回值   | 说明 |
|------|--------|--------|------|
| KillProcess | const fs::path &processName | bool | 根据进程名终止进程 |

```cpp  
bool success = wkp::KillProcess("notepad.exe");
if (success) {
    cout << "进程终止成功" << endl;
}
```  

#### `wkp::KillProcess` - 增强版进程终止
| 函数   | 参数     | 返回值   | 说明 |
|------|--------|--------|------|
| KillProcess | const fs::path& processName, bool killTree, bool partialMatch | bool | 增强版进程终止 |
| KillProcess | DWORD pid, bool killTree = false | bool | 通过PID终止进程 |

```cpp  
// 终止进程树（包含子进程）
bool success = wkp::KillProcess("chrome.exe", true, false);

// 部分匹配（终止所有包含"chrome"的进程）
bool success2 = wkp::KillProcess("chrome", true, true);

// 通过PID终止
bool success3 = wkp::KillProcess(1234, true);
```  

#### `wkp::getProcessPID` - 获取进程PID
| 函数   | 参数     | 返回值   | 说明 |
|------|--------|--------|------|
| getProcessPID | const fs::path& processPath | unsigned long | 获取匹配的进程PID |
| getProcessPID | const fs::path& processPath, bool getAllPidList | vector<DWORD> | 获取所有匹配的进程PID列表 |

```cpp  
// 获取单个PID
DWORD pid = wkp::getProcessPID("notepad.exe");

// 获取所有匹配的PID
vector<DWORD> pids = wkp::getProcessPID("chrome.exe", true);
for (auto pid : pids) {
    cout << "PID: " << pid << endl;
}
```  

#### `wkp::startOtherApp` - 启动其他应用程序
| 函数   | 参数     | 返回值   | 说明 |
|------|--------|--------|------|
| startOtherApp | const string& appPath | bool | 创建新进程打开其他软件 |
| startOtherApp | const string& appPath, bool console | bool | 指定是否显示控制台 |

```cpp  
bool success = wkp::startOtherApp("D:\\Programs\\myapp.exe");
if (success) {
    cout << "程序启动成功" << endl;
}

// 隐藏控制台窗口
bool success2 = wkp::startOtherApp("cmd.exe /c dir", false);
```  

#### `wkp::notRepeatOpenExe` - 检查进程是否已运行
```cpp  
if (!wkp::notRepeatOpenExe("myapp.exe")) {
    cout << "程序已在运行" << endl;
    return 0;
}
```  

### 二、开机自启管理
#### `wkp::setAutoStartUp` - 设置开机自启
| 函数   | 参数     | 返回值   | 说明 |
|------|--------|--------|------|
| setAutoStartUp | const fs::path &appName | bool | 将当前可执行文件列入开机自启注册表 |
| setAutoStartUp | const fs::path &appName, const fs::path& exePath | bool | 将指定文件路径的可执行文件列入开机自启注册表 |
| setAutoStartUpCmd | const fs::path& appName, const fs::path& exePath | bool | 使用cmd命令方式启动（解决路径空格问题） |

```cpp  
// 当前程序开机自启
bool a = wkp::setAutoStartUp("myapp");

// 指定程序开机自启
bool b = wkp::setAutoStartUp("myapp", "C:\\Programs\\myapp.exe");

// 使用cmd方式启动（推荐用于带空格路径）
bool c = wkp::setAutoStartUpCmd("myapp", "C:\\Program Files\\myapp.exe");
```  

#### `wkp::getAutoStartExePath` - 获取开机自启程序路径
| 函数   | 参数     | 返回值   | 说明 |
|------|--------|--------|------|
| getAutoStartExePath | const fs::path& appName | string | 获取开机自启注册表中程序的存放路径 |

```cpp  
string path = wkp::getAutoStartExePath("myapp");
cout << "开机自启程序路径: " << path << endl;
```  

#### `wkp::disableAutoStart` - 禁用开机自启
| 函数   | 参数     | 返回值   | 说明 |
|------|--------|--------|------|
| disableAutoStart | const fs::path& appName | bool | 禁止并删除开机自启 |

```cpp  
bool success = wkp::disableAutoStart("myapp");
if (success) {
    cout << "开机自启已禁用" << endl;
}
```  

### 三、注册表操作
#### `wkp::getRegistryPath` - 获取注册表键值
| 函数   | 参数     | 返回值   | 说明 |
|------|--------|--------|------|
| getRegistryPath | const std::string &keyPath, const std::string &valueName | string | 获取注册表键值的路径 |

```cpp  
string path = wkp::getRegistryPath("Software\\Microsoft\\Windows\\CurrentVersion\\Run", "myapp");
cout << "注册表路径: " << path << endl;
```  

### 四、PowerShell调用
#### `wkp::callPowerShell` - 执行PowerShell命令
| 函数   | 参数     | 返回值   | 说明 |
|------|--------|--------|------|
| callPowerShell | const string& cmd | bool | 执行PowerShell命令 |

```cpp  
bool success = wkp::callPowerShell("Get-Process | Where-Object {$_.CPU -gt 10}");
if (success) {
    cout << "PowerShell命令执行成功" << endl;
}
```  

#### `wkp::callCmd` - 执行CMD命令
| 函数   | 参数     | 返回值   | 说明 |
|------|--------|--------|------|
| callCmd | const string& cmd | bool | 执行CMD命令并返回执行状态 |

```cpp  
bool success = wkp::callCmd("dir C:\\");
if (success) {
    cout << "CMD命令执行成功" << endl;
}
```  

### 五、DPI设置
#### `wkp::setAutoDpi` - 自动调整DPI
```cpp  
wkp::setAutoDpi(); // 设置进程DPI感知，确保在高DPI屏幕上正确获取坐标
```  

#### `wkp::getScreenZoom` - 获取屏幕缩放比例
```cpp  
float zoom = wkp::getScreenZoom();
cout << "屏幕缩放: " << zoom * 100 << "%" << endl;
```  

### 六、管理员权限
#### `wkp::isAdmin` - 判断是否为管理员权限
**返回值**: bool

```cpp  
if (wkp::isAdmin()) {
    cout << "当前是管理员权限" << endl;
} else {
    cout << "当前不是管理员权限" << endl;
}
```  

#### `wkp::openAdmin` - 以管理员身份重新打开当前程序
**返回值**: bool

```cpp  
if (!wkp::isAdmin()) {
    wkp::openAdmin(); // 以管理员身份重新启动
    return 0;
}
```  

### 七、环境变量
#### `wkp::addTempPath` - 添加临时环境变量
| 参数   | 类型                                             | 说明 |
|------|------------------------------------------------|----|
| path | const std::string& path, bool useGbk           | 要添加的路径 |
| path | const std::wstring& newPath                     | 宽字符版本 |

```cpp  
std::string pythonPath = "D:\\Python\\python3";
wkp::addTempPath(pythonPath); // 添加到PATH
```  

### 八、其他系统函数
#### `wkp::findExeInPath` - 在PATH环境变量中查找可执行文件
```cpp  
string path = wkp::findExeInPath("notepad.exe");
cout << "notepad.exe 路径: " << path << endl;

// 忽略.exe后缀查找
string path2 = wkp::findExeInPath("notepad", true);
```  

#### `wkp::getWinRcData` - 获取资源文件数据
```cpp  
vector<unsigned char> data = wkp::getWinRcData(101); // 获取ID为101的资源
```  

#### `wkp::setExeIcon` - 设置可执行文件图标
| 参数   | 类型     | 说明 |
|------|--------|------|
| exePath | const std::filesystem::path& | 可执行文件路径 |
| iconPath | const std::filesystem::path& | 图标文件路径（.ico） |
| saveFullPath | const fs::path& | 保存路径（可选，默认覆盖原文件） |

**返回值**: bool - 是否成功设置

```cpp  
bool success = wkp::setExeIcon("myapp.exe", "icon.ico");
if (success) {
    cout << "图标设置成功" << endl;
}
```  

#### `wkp::setSystemVolumeModern` - 设置系统音量
| 参数   | 类型 | 说明 |
|------|------|------|
| volumePercent | int | 音量百分比（0-100） |

**返回值**: bool - 是否成功设置

```cpp  
bool success = wkp::setSystemVolumeModern(50); // 设置音量为50%
if (success) {
    cout << "音量设置成功" << endl;
}
```  

#### `wkp::loadToMemory` - DLL注入到进程内存
| 参数   | 类型 | 说明 |
|------|------|------|
| processId | DWORD | 目标进程ID |
| dll_path | const fs::path& | DLL文件路径 |

**返回值**: bool - 是否成功注入

```cpp  
DWORD pid = 1234;
bool success = wkp::loadToMemory(pid, "inject.dll");
if (success) {
    cout << "DLL注入成功" << endl;
}
```  

#### `wkp::hideConsole` / `wkp::showConsole` - 控制台隐藏/显示
```cpp  
wkp::hideConsole();  // 隐藏控制台窗口
// ... 程序继续在后台运行 ...
wkp::showConsole();  // 重新显示控制台窗口
```  

---

## 实用工具

### 一、字典数据结构 `wkp::Dict`
保持插入顺序的字典容器

| 函数/操作符 | 说明 | 示例 |
|------------|------|------|
| operator[] | 访问或插入键值对 | `dict["key"] = value` |
| insert | 插入键值对 | `dict.insert("key", value)` |
| get | 安全获取值（带默认值） | `dict.get("key", defaultValue)` |
| pop | 删除键值对 | `dict.pop("key")` |
| items | 返回键值对视图 | `for(auto& [k,v]: dict.items())` |
| keys | 返回键列表 | `auto keys = dict.keys()` |
| values | 返回值列表 | `auto values = dict.values()` |
| size | 大小 | `dict.size()` |
| empty | 是否为空 | `dict.empty()` |
| clear | 清空 | `dict.clear()` |
| dictToString | 转为字符串 | `dict.dictToString()` |

```cpp  
// 创建字典
wkp::Dict<string, int> dict;

// 插入键值对
dict["apple"] = 10;
dict.insert("banana", 20);
dict["orange"] = 15;

// 初始化列表构造
wkp::Dict<string, string> person = {
    {"name", "张三"},
    {"age", "25"},
    {"city", "北京"}
};

// 遍历字典（保持插入顺序）
for (auto& [key, value] : dict.items()) {
    wkp::println("{}: {}", key, value);
}

// 获取键和值列表
auto keys = dict.keys();
auto values = dict.values();

// 安全获取值
int val = dict.get("grape", 0); // 如果不存在返回0

// 删除键值对
dict.pop("banana");

// 支持print和format输出
wkp::println("字典内容: {}", dict);
string formatted = wkp::format("水果数量: {}", dict);
```  

### 二、字符串转字典
#### `wkp::stringToJson` - JSON字符串转字典
```cpp  
string jsonStr = "{\"name\": \"John\", \"age\": 30}";
auto dict = wkp::stringToJson(jsonStr);
wkp::println("姓名: {}, 年龄: {}", dict["name"], dict["age"]);
```  

### 三、控制台编码设置
#### `wkp::useConsoleUtf8` - 设置控制台编码为UTF-8
```cpp  
wkp::useConsoleUtf8();
```  

#### `wkp::getConsoleEncoding` - 获取控制台编码
```cpp  
string encoding = wkp::getConsoleEncoding();
cout << "控制台编码: " << encoding << endl; // 输出: utf8, gbk, big5等
```  

### 四、文件夹操作
#### `wkp::listDir` - 获取文件夹内文件列表
| 参数           | 类型   | 说明           |  
|----------------|--------|--------------|  
| path           | string | 目标文件夹路径    |  

**返回值**:  
`vector<string>` 包含文件/目录名的容器

```cpp  
vector<string> files = wkp::listDir("C:/MyFolder");  
for (auto& file : files) {  
    cout << file << endl;  
}  
```  

#### `wkp::openDir` - 后台打开文件夹
| 参数   | 类型              | 说明             |
|------|-----------------|----------------|
| path | const std::string& | 要打开的文件夹路径 |
| utf8 | bool            | 是否使用UTF-8编码（默认true） |

```cpp  
wkp::openDir("C:/MyFolder"); // 在资源管理器中打开
```  

### 五、排序与求和
#### `wkp::sort` - 冒泡排序
| 函数   | 参数                                                          | 返回值         | 说明                   |
|------|-------------------------------------------------------------|-------------|----------------------|
| sort | const int *numList, int numListSize, const string &mode     | vector<int> | 简单的冒泡排序，支持升序和降序 |
| sort | const std::vector<int> &numList, const string &model        | vector<int> | 向量版本的冒泡排序         |

```cpp  
int numList[] = {5, 2, 8, 1, 9};
// 降序
auto a = wkp::sort(numList, 5, "down");
// 升序
auto b = wkp::sort(numList, 5, "up");

vector<int> nums = {3, 1, 4, 1, 5};
auto sorted = wkp::sort(nums, "up");
```  

#### `wkp::sum` - 列表求和
| 函数   | 参数                        | 返回值   | 说明                   |
|------|---------------------------|--------|----------------------|
| sum  | std::vector<int> &numList | int    | 整数向量求和              |
| sum  | std::vector<long> &numList | long   | 长整数向量求和            |
| sum  | std::vector<double> &numList | double | 双精度浮点数向量求和        |
| sum  | std::vector<float> &numList | float  | 单精度浮点数向量求和        |

```cpp  
vector<int> nums = {1, 2, 3, 4, 5};
int total = wkp::sum(nums);
cout << "总和: " << total << endl;
```  

### 六、时间函数
#### `wkp::getNowTime` - 获取当前时间
**返回值**: string - 格式如 "2026-04-23 15:30:45"

```cpp  
string time = wkp::getNowTime();
cout << "当前时间: " << time << endl;
```  

#### `wkp::getRunTime` - 获取运行时间
**返回值**: double - 程序启动至今的秒数

```cpp  
double start = wkp::getRunTime();
// 执行一些操作
Sleep(1000);
double end = wkp::getRunTime();
cout << "耗时: " << (end - start) << "秒" << endl;
```  

### 七、路径获取
#### `wkp::getExePath` - 获取当前exe所在路径
| 参数   | 类型 | 说明 |
|------|------|------|
| useWindows | bool | 是否使用Windows路径格式（默认false） |

```cpp  
string path = wkp::getExePath(false);
cout << "当前路径: " << path << endl;

string winPath = wkp::getExePath(true);
cout << "Windows格式路径: " << winPath << endl;
```  

#### `wkp::getNowExeName` - 获取当前exe文件名
**返回值**: string - 当前可执行文件的文件名（含.exe后缀）

```cpp  
string exeName = wkp::getNowExeName();
cout << "当前程序名称: " << exeName << endl;
```  

### 八、延迟函数
#### `wkp::sleep` - 延迟函数
| 参数   | 类型   | 说明 |
|------|------|------|
| delay | double | 延迟秒数 |

```cpp  
wkp::sleep(1.5); // 延迟1.5秒
```  

### 九、类型识别
#### `wkp::typeName` - 获取变量类型名称
| 参数   | 类型     | 返回值   | 说明 |
|------|--------|--------|------|
| type | Types  | string | 获取变量类型名称 |

```cpp  
auto type = wkp::typeName(123);
cout << "类型: " << type << endl; // 输出: int

vector<string> vec;
cout << wkp::typeName(vec) << endl; // 输出: vector_string

map<string, int> mp;
cout << wkp::typeName(mp) << endl; // 输出: map

struct Test {} t;
cout << wkp::typeName(t) << endl; // 输出: struct
```  

### 十、字符串格式化
#### `wkp::string_format` - 字符串格式化（C风格）
| 参数   | 类型     | 返回值   | 说明 |
|------|--------|------|------|
| format | const char* format, ... | string | 安全的字符串格式化函数 |

```cpp  
string formatted = wkp::string_format("姓名: %s, 年龄: %d", "张三", 25);
cout << formatted << endl;
```  

#### `wkp::format` - 智能映射（C++风格）
| 参数   | 类型     | 返回值   | 说明 |
|------|--------|-------|------|
| format | const std::string& fmt, Args&&... args | string | 映射字符串 |

```cpp  
vector<int> a = {1, 2, 3};
string str = "hello {}";
cout << wkp::format("输出: {}", a) << endl;  // 输出: 输出: [1, 2, 3]

// 支持位置参数 {0}, {1}
cout << wkp::format("{1}, {0}", "world", "hello") << endl; // 输出: hello, world

// 嵌套数组
vector<vector<int>> b = {{1, 2}, {3, 4}};
cout << wkp::format("嵌套数组: {}", b) << endl;  // 输出: 嵌套数组: [[1, 2], [3, 4]]

// C数组
int arr[] = {5, 6, 7};
cout << wkp::format("C数组: {}", arr) << endl;  // 输出: C数组: [5, 6, 7]

// map和Dict类型
std::map<string, int> mp = {{"a", 1}, {"b", 2}};
cout << wkp::format("map: {}", mp) << endl;  // 输出: map: {"a": 1, "b": 2}

wkp::Dict<string, int> dict;
dict["apple"] = 10;
dict["banana"] = 20;
cout << wkp::format("dict: {}", dict) << endl; // 输出: dict: {"apple": 10, "banana": 20}
```  

### 十一、输出函数
#### `wkp::print` / `wkp::println` - 智能输出
| 函数   | 参数     | 返回值 | 说明 |
|------|--------|------|------|
| print | const T& first, const Args&... rest | void | 智能判断输出模式，不换行 |
| println | const T& first, const Args&... rest | void | 智能判断输出模式，换行 |

```cpp  
wkp::print("Hello"); // 不换行输出
wkp::println("World"); // 换行输出

vector<int> a = {1, 2, 3};
wkp::println("姓名: {}, 年龄: {}, 数组: {}", "李四", 30, a); // 格式化输出

// 带颜色的输出
wkp::println(color::printColor("成功提示", Color("green")), ": 操作完成!");

// 普通输出
wkp::print("当前时间: ");
wkp::println(wkp::getNowTime());
```  

#### `wkp::printUpdateLine` - 控制台行刷新输出
在控制台同一行位置刷新输出内容

```cpp  
for (int i = 0; i <= 100; i++) {
    wkp::printUpdateLine("进度: {}%", i);
    wkp::sleep(0.1);
}
```  

### 十二、颜色输出
#### `Color` 类 - 颜色管理
支持多种颜色格式：
- RGB值：`Color(255, 0, 0)` 红色
- 颜色名称：`Color("red")` 红色
- 十六进制：`Color("#FF0000")` 红色

支持的颜色名称：`red`, `green`, `blue`, `yellow`, `cyan`, `magenta`, `white`, `black`, `gray` 等

```cpp  
// 基本颜色输出
wkp::println(color::printColor("红色文本", Color("red")));
wkp::println(color::printColor("绿色文本", Color("green")));
wkp::println(color::printColor("蓝色文本", Color("blue")));
wkp::println(color::printColor("黄色文本", Color("yellow")));

// RGB颜色
Color custom(255, 128, 0); // 橙色
wkp::println(color::printColor("自定义颜色", custom));

// 十六进制颜色
Color hexColor("#FF00FF"); // 紫色
wkp::println(color::printColor("十六进制颜色", hexColor));

// 嵌套颜色使用
Print::pushColor(Color("red"));
wkp::print(color::printColor("红色文本", Color("red")));
Print::pushColor(Color("green"));
wkp::print(color::printColor("绿色文本", Color("green")));
Print::popColor(); // 返回到红色
wkp::print(color::printColor("又是红色文本", Color("red")));
Print::popColor(); // 返回到默认颜色
wkp::println("正常文本");
```  

### 十三、随机数功能
#### `random` 命名空间 - 随机数生成
| 函数   | 参数 | 返回值 | 说明 |
|------|------|------|------|
| randint | int min, int max | int | 生成指定范围内的随机整数 |
| randint | long min, long max | long | 生成长整数随机数 |
| randint | long long min, long long max | long long | 生成长长整数随机数 |
| uniform | float min, float max | float | 生成指定范围内的随机浮点数 |
| uniform | double min, double max | double | 生成双精度随机浮点数 |
| choice | vector<T> vec | T | 从容器中随机选择一个元素 |
| choices | const std::vector<T>& population, const std::vector<double>& weights, int k = 1 | vector<T> | 带权重的随机选择多个元素 |
| choices | const std::vector<T>& population, int k = 1 | vector<T> | 均匀随机选择多个元素 |

```cpp  
// 生成随机整数
int num = random::randint(1, 100);

// 生成随机浮点数
double fnum = random::uniform(0.0, 1.0);

// 从容器中随机选择
vector<string> fruits = {"apple", "banana", "orange", "grape"};
string chosen = random::choice(fruits);

// 带权重的选择
vector<double> weights = {0.1, 0.3, 0.4, 0.2};
vector<string> chosenFruits = random::choices(fruits, weights, 2);

// 均匀选择多个
vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
vector<int> selected = random::choices(numbers, 3);
```  

### 十四、正则表达式功能
#### `re` 命名空间 - 正则表达式操作
| 函数   | 参数 | 返回值 | 说明 |
|------|------|------|------|
| findall | const string &pattern, const string &text, int flags=NOMODEL | vector<string> | 查找所有匹配项 |
| search | const string &pattern, const string &text, int flags=NOMODEL | string | 搜索第一个匹配项 |
| sub | const string &pattern, const string &repl, const string &text, int flags=NOMODEL | string | 替换匹配项 |
| sub | const string &pattern, std::function<string(const std::smatch&)> callback, const string &text, int flags=NOMODEL | string | 使用回调函数替换 |

**标志枚举**:
- `re::IGNORECASE` 或 `re::I`: 忽略大小写
- `re::MULTILINE` 或 `re::M`: 多行模式
- `re::NOMODEL`: 无特殊模式（默认）

```cpp  
// 查找所有数字
auto numbers = re::findall(R"(\d+)", "abc123def456ghi");
// 结果: ["123", "456"]

// 查找邮箱
auto emails = re::findall(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})", 
                          "Contact: test@example.com, admin@test.org");

// 替换文本
string result = re::sub(R"(\d+)", "NUM", "abc123def456");
// 结果: "abcNUMdefNUM"

// 使用回调函数替换
string callbackResult = re::sub(R"(\d+)", 
    [](const std::smatch& m){
        return "[" + m.str() + "]";
    }, 
    "abc123def456");
// 结果: "abc[123]def[456]"

// 忽略大小写搜索
string found = re::search("hello", "HELLO World", re::IGNORECASE);
// 结果: "HELLO"
```  

### 十五、字符串处理函数
#### `wkp::stringToVector` - 字符串转容器（按行分割）
```cpp  
string text = "line1\nline2\nline3";
vector<string> lines = wkp::stringToVector(text);
for (auto& line : lines) {
    cout << line << endl;
}
```  

#### `wkp::findStringWhereLine` - 查找字符串所在行号
| 参数   | 类型 | 说明 |
|------|------|------|
| str | const string& | 源字符串 |
| targetPattern | const string& | 要查找的模式 |

**返回值**: long - 匹配的行号（从0开始），未找到返回-1

```cpp  
string text = "hello world\nhello cpp\nhello win";
long lineNum = wkp::findStringWhereLine(text, "cpp");
cout << "找到位置在第 " << lineNum << " 行" << endl;
```  

#### `wkp::findStringWhereLineVec` - 查找所有匹配的行号
```cpp  
string text = "hello world\nhello cpp\nhello win\nhello cpp again";
vector<long> lineNums = wkp::findStringWhereLineVec(text, "cpp");
for (auto line : lineNums) {
    cout << "匹配行: " << line << endl;
}
```  

#### `wkp::input` - 获取用户输入
| 参数   | 类型 | 说明 |
|------|------|------|
| inform_text | const string& | 提示文本 |
| color | const string& | 提示文本颜色（可选） |

**返回值**: string - 用户输入的字符串

```cpp  
string name = wkp::input("请输入姓名: ");
string age = wkp::input("请输入年龄: ", "green");
```  

---

## 加密与解密

### `wkp::addStringKey` / `wkp::decryptStringKey` - 字符串加密解密
使用XOR算法对字符串进行加密和解密，返回16进制字符串。

| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| addStringKey | const string& text, const string& key | string | 加密字符串，返回16进制字符串 |
| decryptStringKey | const string& hex_text, const string& key | string | 解密16进制字符串 |

```cpp  
string key = "mySecretKey123";
string original = "这是敏感信息";

// 加密
string encrypted = wkp::addStringKey(original, key);
cout << "加密结果: " << encrypted << endl;

// 解密
string decrypted = wkp::decryptStringKey(encrypted, key);
cout << "解密结果: " << decrypted << endl;
```  

---

## 动态库调用（DLL加载）

### `wkp::CallWinDll` - Windows DLL动态调用类

该类支持加载Windows DLL并调用其中的函数，支持多种调用约定。

**构造函数参数：**
| 参数 | 类型 | 说明 |
|------|------|------|
| winDllPath | const fs::path& | DLL文件路径 |
| convention | const std::string& | 调用约定（"stdcall"或"cdecl"，默认"stdcall"） |

**方法：**
| 函数 | 说明 |
|------|------|
| isOpen() | 检查DLL是否成功加载 |
| setCallingConvention() | 设置调用约定 |
| getCallingConvention() | 获取当前调用约定 |
| callWinDll<ReturnType>() | 调用DLL中的函数 |
| help() | 打印帮助信息 |

```cpp  
// 加载User32.dll
wkp::CallWinDll user32("user32.dll", "stdcall");

if (user32.isOpen()) {
    // 调用MessageBoxA函数
    int result = user32.callWinDll<int>("MessageBoxA", 
                                        (HWND)nullptr, 
                                        "Hello from DLL!", 
                                        "Title", 
                                        MB_OK);
    cout << "MessageBox returned: " << result << endl;
}

// 调用Kernel32.dll中的函数
wkp::CallWinDll kernel32("kernel32.dll");
if (kernel32.isOpen()) {
    DWORD pid = kernel32.callWinDll<DWORD>("GetCurrentProcessId");
    cout << "Process ID: " << pid << endl;
}
```  

---

## 文件夹压缩与解压

### `wkp::ZipDirToBin` - 文件夹压缩/解压类

该类支持将整个文件夹压缩为二进制文件，以及从二进制文件解压恢复文件夹结构。

**方法：**
| 函数 | 参数 | 返回值 | 说明 |
|------|------|--------|------|
| CompressFolder | const fs::path& folderPath, const fs::path& outputFile | bool | 压缩文件夹为二进制文件 |
| DecompressFolder | const fs::path& compressedFile, const fs::path& outputFolder | bool | 解压二进制文件到目标文件夹 |

**特性：**
- 保留文件夹结构
- 保留文件属性（修改时间等）
- 使用zlib压缩算法
- 支持中文路径（UTF-8）

```cpp  
// 压缩文件夹
wkp::ZipDirToBin zipper;
bool success = zipper.CompressFolder("D:/MyFolder", "D:/MyFolder.bin");
if (success) {
    cout << "文件夹压缩成功" << endl;
}

// 解压文件夹
bool decompressSuccess = zipper.DecompressFolder("D:/MyFolder.bin", "D:/ExtractedFolder");
if (decompressSuccess) {
    cout << "解压成功" << endl;
}
```  

---

## 清理与多线程
### 自动清理
- 析构时自动释放未松开的按键
- 程序退出时自动清理监听线程

### 手动清理方法：
```cpp  
KeyBoardEvent::ClearHotKey();       // 清除所有注册热键  
KeyBoardEvent::ExitOneKeyEvent();   // 停止单按键监听  
MouseEvent::ExitMouseEvent();       // 停止鼠标监听  
KeyBoardEvent::UninstallHotKeyEvent(); // 注销热键但不退出
KeyBoardEvent::UninstallOneKeyEvent(); // 注销单按键监听
MouseEvent::UninstallMouseEvent();     // 注销鼠标监听
```  

### 线程管理
- **必须** 将 `ListenMouseEvent` 和 `WaitHotKey` 放在独立线程
- 推荐使用 `detach` 避免主线程阻塞：
  ```cpp  
  thread t([&](){ keyboard.WaitHotKey(); });  
  t.detach();  
  ```  

### 退出检查
```cpp  
ExitCheckWork(); // 注册退出时的清理函数（通常在main开头调用一次）
```  

---

## 完整示例
```cpp  
#include <thread>  
#include "winKeyPressH.hpp"  

int main() {  
    // 设置DPI和编码
    wkp::setAutoDpi();
    wkp::useConsoleUtf8();
    
    // 注册退出清理
    ExitCheckWork();

    // 图像识别示例
    ImageEvent imgEvent;
    ImagePosition img = imgEvent.GetImagePosition("button.png", 0.9);  
    if (img.result) {  
        MouseEvent mouse;  
        mouse.MouseClick(img.x, img.y); // 点击识别到的按钮  
    }  

    // 字典使用示例
    wkp::Dict<string, int> fruitDict;
    fruitDict["apple"] = 10;
    fruitDict["banana"] = 20;
    fruitDict["orange"] = 15;
    
    wkp::println("水果字典: {}", fruitDict);
    
    // 正则表达式示例
    string text = "价格: $123.45 数量: 100";
    auto numbers = re::findall(R"(\d+\.?\d*)", text);
    wkp::println("找到的数字: {}", numbers);
    
    // 颜色输出示例
    wkp::println(color::printColor("成功提示", Color("green")), ": 操作完成!");
    wkp::println(color::printColor("警告提示", Color("yellow")), ": 请注意磁盘空间!");
    wkp::println(color::printColor("错误提示", Color("red")), ": 文件打开失败!");
    
    // 随机数示例
    int randomNum = random::randint(1, 100);
    wkp::println("随机数: {}", randomNum);
    
    // DLL调用示例
    wkp::CallWinDll user32("user32.dll");
    if (user32.isOpen()) {
        user32.callWinDll<int>("MessageBoxA", nullptr, "Hello!", "Title", MB_OK);
    }
    
    // 文件夹压缩示例
    wkp::ZipDirToBin zipper;
    zipper.CompressFolder("./data", "data.bin");
    
    // 文件操作示例
    wkp::Open file;
    file.open("data.json", "w", "auto");
    file.writeJson(fruitDict);
    file.close();
    
    // 字符串编码转换示例  
    wkp::StringCode converter;  
    string utf8Text = converter.GBKToUTF8("自动化脚本");
    string encoding = converter.CheckStringCode(utf8Text);
    cout << "编码类型: " << encoding << endl;

    // 键盘热键示例  
    KeyBoardEvent keyboard;  
    keyboard.AddHotKey("ctrl+alt+l", [](){  
        wkp::println("热键触发：锁定系统");  
    });  

    // 消息弹窗示例
    wkp::WMessageBox msg;
    msg.showinfo("提示", "自动化脚本启动成功!");

    // 窗口置顶示例
    HWND hwnd = FindWindow(nullptr, L"记事本");
    if (hwnd) {
        wkp::WindowTop top(hwnd);
        top.setWindowsTopOn();
    }

    // 启动监听线程  
    thread hotkeyThread([&](){  
        keyboard.WaitHotKey();  
    });  
    hotkeyThread.detach();  

    // 视频处理示例
    VideoEvent ve;
    long long duration = ve.GetVideoDuration("video.mp4");
    wkp::println("视频时长: {} 秒", duration / 1000000);
    
    // 加密解密示例
    string encrypted = wkp::addStringKey("敏感信息", "mykey");
    string decrypted = wkp::decryptStringKey(encrypted, "mykey");
    wkp::println("加密: {}, 解密: {}", encrypted, decrypted);
    
    // 用户输入示例
    string name = wkp::input("请输入姓名: ");
    wkp::println("你好, {}", name);

    // 主循环
    while (true) {  
        wkp::sleep(1);  
        // 检查是否有按键
        long keyCode = keyboard.GetOneKeyCode();
        if (keyCode == KEY_ESC_DOWN) {
            wkp::println("ESC键按下，退出程序");
            break;
        }
    }  
    
    // 清理资源
    KeyBoardEvent::ClearHotKey();
    KeyBoardEvent::ExitOneKeyEvent();
    MouseEvent::ExitMouseEvent();
    
    return 0;  
}  
```  

---

## 注意事项

1. **管理员权限**：部分操作（如进程终止、开机自启、驱动级模拟、设置图标、DLL注入）需以管理员身份运行程序
2. **按键冲突**：避免重复注册系统级热键（如 `alt+F4`）
3. **图像识别依赖**：需正确安装OpenCV库并配置编译环境
4. **编码兼容性**：文件路径建议使用UTF-8，调用 `wkp::useConsoleUtf8()` 设置控制台编码
5. **调试建议**：监听函数启动前添加日志输出确认线程状态
6. **DPI适配**：在高DPI屏幕上使用前调用 `wkp::setAutoDpi()`
7. **线程安全**：监听函数需要在独立线程中运行
8. **字典顺序**：`wkp::Dict` 保持插入顺序，适合需要顺序遍历的场景
9. **颜色输出**：控制台颜色输出需要支持ANSI转义序列的控制台
10. **正则表达式**：使用正则表达式功能需要包含 `<regex>` 头文件（已包含）
11. **随机数**：随机数生成器需要包含 `<random>` 头文件（已包含）
12. **文件系统**：文件操作功能需要C++17支持（`<filesystem>`）
13. **Windows版本**：部分API可能需要特定Windows版本支持（Windows 7及以上）
14. **图像识别范围**：`GetSpecifiedAreaImagePosition` 支持在指定区域内进行图像匹配
15. **进程终止**：使用 `killTree=true` 时要谨慎，会终止所有子进程
16. **资源清理**：程序退出时会自动清理，但建议显式调用清理函数
17. **驱动级模拟**：驱动级操作可能被某些安全软件拦截，需要管理员权限
18. **剪贴板操作**：频繁操作剪贴板可能影响其他应用程序
19. **图标设置**：`setExeIcon` 功能需要管理员权限，且只能用于未运行的可执行文件
20. **视频处理**：使用FFMPEG功能需要系统安装FFMPEG并配置环境变量
21. **编码转换**：`wkp::StringCode` 的自动编码检测功能处于测试阶段，结果可能不完全准确，建议对重要数据手动指定编码
22. **文件编码**：`wkp::Open` 类的encoding参数支持"auto"自动检测，但在处理大型文件时可能影响性能
23. **DLL调用**：调用DLL函数时需确保函数名称和参数类型完全匹配，错误的调用约定可能导致程序崩溃
24. **文件夹压缩**：压缩大文件夹时可能需要较长时间和内存，建议在后台线程中执行
25. **加密安全**：XOR加密为简单加密算法，不适合存储高度敏感信息
26. **控制台控制**：隐藏控制台后无法通过常规方式重新显示，建议仅在需要时使用

---

## 更新日志

### 2026/4/23 更新
1. **新增功能**：添加 `wkp::input` 函数，支持获取用户输入
2. **新增功能**：添加 `wkp::hideConsole` 和 `wkp::showConsole` 函数，支持控制台隐藏/显示
3. **新增功能**：添加 `wkp::addStringKey` 和 `wkp::decryptStringKey` 加密解密函数
4. **新增功能**：添加 `Print::pushColor` 和 `Print::popColor` 方法，支持颜色嵌套
5. **新增功能**：添加 `Print::setColor` 和 `Print::resetColor` 静态方法
6. **新增功能**：添加 `Print::gotoXY` 和 `Print::clearLine` 方法，支持光标控制
7. **完善文档**：补充 `useGbk` 参数在所有图像识别函数中的说明
8. **完善文档**：补充 `VideoEvent` 构造函数和析构函数的说明
9. **完善文档**：补充 `ImageEvent` 析构函数的说明
10. **修复**：修正部分函数参数类型不匹配的问题

### 2026/4/6 更新
1. 新增 `GetImagePositionVec` 函数，支持获取图像在屏幕中的所有匹配位置
2. 新增 `FindSimilarImage` 函数，支持判断图像是否存在于屏幕中
3. 新增 `FindSpecifiedSimilarImage` 函数，支持在指定区域内判断图像是否存在
4. 新增 `setAutoStartUpCmd` 函数，支持使用cmd命令方式设置开机自启（解决路径空格问题）
5. 新增 `getAutoStartExePath` 函数，获取开机自启注册表中程序的存放路径
6. 新增 `callCmd` 函数，支持执行CMD命令
7. 新增 `setSystemVolumeModern` 函数，支持设置系统音量
8. 新增 `getNowExeName` 函数，获取当前可执行文件名
9. 完善 `format` 函数，增加 `{0}`, `{1}` 等位置参数的支持
10. 新增视频操作类 `VideoEvent`，支持视频剪切、合并、封面设置、时长获取等
11. 新增音频操作类 `MusicEvent`，支持音频合并、从视频分离音频等
12. **重要变更**：`StringCode` 类已移动到 `wkp` 命名空间下（使用方式：`wkp::StringCode`）
13. **重要变更**：`WMessageBox` 和 `WindowTop` 类已移动到 `wkp` 命名空间下
14. **新增功能**：`wkp::StringCode` 类新增自动编码检测功能 `CheckStringCode()`
15. **新增功能**：`wkp::StringCode` 类新增智能编码转换功能 `StringChangeEncoding()`
16. **新增功能**：`wkp::Open` 类新增 `encoding` 参数，支持自动检测文件编码
17. 新增 `GetVideoCodec` 函数，获取视频编码格式
18. 新增 `wkp::CallWinDll` 类，支持动态加载和调用Windows DLL
19. 新增 `wkp::ZipDirToBin` 类，支持文件夹压缩和解压
20. 新增 `wkp::loadToMemory` 函数，支持DLL注入到进程内存
21. 新增 `wkp::getProcessPID` 函数重载，支持获取所有匹配的进程PID
22. 新增 `wkp::stringToVector` 函数，字符串按行转容器
23. 新增 `wkp::findStringWhereLine` 和 `wkp::findStringWhereLineVec` 函数
24. 新增 `wkp::printUpdateLine` 函数，支持控制台行刷新输出
25. 修复已知问题和文档错误

### 2026/3/12 更新
1. 优化了 `println` 与 `format` 的bug，增加了 `{0}`, `{1}` 的格式化输入支持

### 2026/3/6 更新
1. 新增 `wkp::setExeIcon` 函数，支持为可执行文件设置图标
2. 修正驱动级组合键函数名为 `PressHotKeyDriving`（原为 `PressHotKey` 驱动级重载）
3. 完善 `PressHotKeyDriving` 的文档说明和示例
4. 优化字典 `dictToString` 方法的嵌套字典处理能力
5. 增强颜色输出功能对嵌套对象的支持
6. 更新示例代码中的时间格式为当前日期
7. 修复文档中部分函数参数说明不一致的问题
8. 完善注意事项中关于管理员权限的说明

### 2026/3/2 更新
1. 完善文档结构和内容，补充缺失的函数说明
2. 优化 `GetScreenSize` 返回值说明，增加屏幕缩放比例属性
3. 完善驱动级键鼠模拟功能的文档说明
4. 新增 `notRepeatOpenExe` 函数说明，用于检查进程是否已运行
5. 更新 `getNowTime` 示例时间为当前日期
6. 优化颜色输出和格式化输出的示例代码
7. 增强正则表达式功能的示例说明

### 2026/2/6 更新
1. 类成员函数 `WriteStr` 接口更改，用法不变，支持输出utf8编码的字符，不再需要copy完成
2. 新增函数 `GetImageSize` 获取图片分辨率
3. 更改 `SetImageSize`，支持浮点数设置图片大小
4. 优化字符串输入功能，支持中文输入

### 2026/2/1 更新
1. 新增函数 `lockWindows` 锁定窗口为焦点
2. 优化窗口控制功能

### 2026/1/28 更新
1. 新增区域图像识别功能 `GetSpecifiedAreaImagePosition`
2. 增强进程管理功能，新增增强版 `KillProcess` 支持进程树终止和PID终止
3. 优化文档结构和示例代码
4. 修复部分已知问题和文档错误

### 2026/1/3 更新
1. 新增字符串编码类 `StringCode`（现已移至 `wkp` 命名空间）
2. 新增文件操作类 `FileEvent`
3. 新增弹窗类 `WMessageBox`（现已移至 `wkp` 命名空间）
4. 新增窗口置顶类 `WindowTop`（现已移至 `wkp` 命名空间）
5. 新增类型识别功能 `GetValueType` 和 `wkp::typeName`
6. 新增颜色类 `Color` 支持RGB、颜色名称和十六进制
7. 新增随机数命名空间 `random`，提供多种随机数生成功能
8. 新增 `wkp` 命名空间下的多个实用函数
9. 新增 `wkp::Dict` 字典数据结构，保持插入顺序
10. 新增 `wkp::Open` 文件读写类，支持文本、二进制和JSON
11. 新增正则表达式命名空间 `re`
12. 新增字符串转字典功能 `wkp::stringToJson`
13. 全面完善文档说明和示例代码