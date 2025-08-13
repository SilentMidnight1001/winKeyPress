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
| 参数    | 类型    | 说明        |  
|-------|-------|-----------|  
| x     | int   | 目标X坐标（像素） |  
| y     | int   | 目标Y坐标（像素） |  
| delay | float | 移动延迟      |  

```cpp  
mouse.MouseMoveTo(0, 0, 1); // 移动到屏幕左上角  
```  

#### 4. `MouseClick` - 点击操作
| 参数     | 类型     | 可选值             | 默认值    |  
|--------|--------|-----------------|--------|  
| x, y   | int    | -               | 无      |  
| clicks | int    | ≥1              | 1      |  
| button | string | "left", "right" | "left" |  
| delay  | float  | int->float      | 0      |  

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
| 参数       | 类型     | 说明             |  
|------------|--------|----------------|  
| imgPath   | string | 原图像格式路径(源图文件名) |  
| width  | string | 图像宽度           |  
| height  | string | 图像高度           |  
| targetImageSavePath  | string | 存放路径(可以不需要)    |  

```cpp  
ImageEvent imgevent;
ImagePosition img = imgevent.GetImagePosition("img.png", 0.85);  
img.SetImageFormat("img.png", "ing.jpg");
```  

### 三、修改图像尺寸
#### `setImageSize` - 更改图像格式
| 参数      | 类型     | 说明             |  
|---------|--------|----------------|  
| imgPath | string | 原图像格式路径(源图文件名) |  
| width   | int    | 图片大小           |  
| height  | int    | 图片大小           |  
| targetImageSavePath   | string | 目标存放路径(可选)     |
| outPutName   | string | 输出图片名称(可选)     |

```cpp  
int main()
{
    ImageEvent imge;
    // 原名称仅修改大小存放到其他目录 //
    imge.SetImageSize("../test.png", 150, 400, "D:/WinKeyPress/cmake-build-release/Testing", "");
    return 0;
}
```  

### 四、截屏功能
#### `CaptureScreen` - 截取整个屏幕
| 参数       | 类型     | 说明         |
|------------|--------|------------|
| imageName  | string | 截图保存的文件名   |
| targetImageSavePath  | string | 保存路径(可以不要) |

```cpp  
int main()
{
    ImageEvent imge;
    imge.CaptureScreen("test", "../");
    return 0;
}
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

## 字符串编码类
### `StringCode` - 给字符串进行编码
#### string GBKToUTF8(const std::string& gbkStr);
#### string Utf8ToGBK(const std::string& utf8Str);
#### wstring StrToWStr(const std::string& str);
#### wstring StrToWStr(const std::string& str, bool isUtf8);
WStrToStr(const std::wstring& wstr);


| 函数              | 参数             | 类型                | 说明                                          |
|-----------------|----------------|-------------------|---------------------------------------------|
| GBKToUTF8       | gbkStr         | string            | 返回改变编码后的字符串                                 |
| Utf8ToGBK       | utf8Str        | string            | 返回改变编码后的字符串                                 |
| StrToWStr       | str            | string            | 返回改变编码后的字符串                                 |
| StrToWStr(重载函数) | str <br/> isUtf8 | string <br/> bool | 返回改变编码后的字符串(根据布尔值True为utf8编码转宽字符，<br/>False为gbk) |
```cpp  
StringCode scd;

```  

## 添加临时环境变量
#### `wkp::addTempPath` - 添加临时环境变量(重载函数)
| 参数   | 类型   | 说明 |
|------|------|----|
| void | const std::string& path | 无  |
| void | const std::wstring& newPath | 无  |
```cpp  
 // 普通字符串变量 //
 std::string pythonPath = "D:\\Python嵌入式\\python嵌入式";
 // 添加到PATH
 wkp::addTempPath(pythonPath);

/******************* 宽字符串变量 *****************/
std::string Path = "D:\\QtImageTools\\python嵌入式";
// 添加到PATH
wkp::addTempPath(Path);
// 检查是否添加环境变量
system("where python");
```  

## 设置控制台输入输出编码
#### `wkp::useConsoleUtf8` - 设置控制台编码
| 函数              | 参数 | 类型 | 说明        |
|-----------------|----|----|-----------|
| useConsoleUtf8       | 无  | 无  | 设置终端控制台编码 |
```cpp  
wkp::useConsoleUtf8();
```  

## 后台打开文件夹
#### `wkp::openDir` - 后台打开文件夹
| 函数              | 参数              | 类型   | 说明             |
|-----------------|-----------------|------|----------------|
| openDir    | const std::string& pat | string | 用于后台打开文件夹显示给用户 |
```cpp  
wkp::useConsoleUtf8();
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

### 3. 文件处理事件
#### 类：
`FileEvent` - 返回指定目录下的所有文件和子目录名称

| 函数名        | 类型   | 说明         |  
|------------|------|------------|  
| RemoveFile | bool | 删除文件       |  
| CreateDir  | bool | 创建目录       |  
| RemoveDir  | bool | 删除目录(文件夹)  |  
| WriteLog  | bool | 写日志(a追加模式) |  
| CopyDir  | bool | 复制文件夹到目标路径 |  

**返回值**:  
`vector<string>` 包含文件/目录名的容器

```cpp  
int main()
{
    FileEvent f;
    bool a = f.CreateDir("../Test");
    bool b = f.RemoveFile("../Test/Test.txt");
    bool c = f.RemoveDir("D:/Test/test")
    bool d = f.WriteLog("日志...", "test.log")
    bool e = f.CopyDir("./", "D:/Test")
    return 0;
}
```  

## 冒泡排序
#### `wkp::sort` - 简单排序
| 函数   | 参数                                                          | 函数类型            | 说明                   |
|------|-------------------------------------------------------------|-----------------|----------------------|
| sort | const int *numList , int numListSize ,  const string &mode) | vector<int> | 简单的冒泡排序 |
```cpp  
    // 降序 //
    auto a = wkp::sort(numList, 17, "down");
    for(auto i:a)
    {
        cout << i << " ";
    }
    
    // 升序 //
    auto a = wkp::sort(numList, 17, "up");
    for(auto i:a)
    {
        cout << i << " ";
    }
```  

## 获取当前时间
#### `wkp::getNowTime` - 
| 函数   | 参数 | 函数类型   | 说明                   |
|------|----|--------|----------------------|
| getNowTime | 无  | string | 简单的冒泡排序 |
```cpp  
string time = wkp::getNowTime();
std::cout<<time
```
## 获取当前exe所在路径
#### `wkp::getExePath` -
| 函数   | 函数类型   | 说明                   |
|------|--------|----------------------|
| getExePath | string | 获取当前exe所在路径 |
```cpp
string path = wkp::getExePath();
std::cout<<path;
```

## 实现读写文件io操作
#### `wkp::open 类` - 
| 函数          | 函数类型                  | 说明                            |
|-------------|-----------------------|-------------------------------|
| open        | string                | 打开文件,"r", "w", "a", "ab", "wb" |
| isOpen      | bool                  | 返回文件是否打开                      |
| read        | string                | 读取整个文件                        |
| readLine    | vector<string>        | 逐行读取文件内容                      |
| readBinary  | vector<unsigned char> | 读取整个二进制文件                     |
| write       | void                  | 写入文件                          |
| writeBinary | void                  | 写入二进制文件(重载函数)                 |
```cpp
int main()
{
    wkp::Open opt;
    opt.open("测试.txt", "w");
    opt.write("测试写入");
    opt.close();

    wkp::Open opt2;
    opt2.open("测试.txt", "w");
    opt2.write("测试追加写入");
    opt2.close();

    wkp::Open ipt;
    ipt.open("测试.txt", "r");
    string read1 = ipt.read();
    ipt.close();

    wkp::Open ipt2("测试.txt", "r");
    string read2 = ipt2.read();
    ipt2.close();

    cout<<read1<<endl;
    cout<<read2<<endl;

    return 0;
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
####  至此版本起，opencv使用4.10版本，编译器为mingw14，版本转为C++14标准(winKeyPress4.10)
1. 新增图像处理类 `ImageEvent`
2. MouseEvent新增功能，默认点击1次，按钮为左键 `MouseClick(int x, int y)`
3. GetImagePosition移动至图像处理类 `ImageEvent`
4. 新增命名空间 `wkp`，存在两个功能:`isAdmin`、`openAdmin`用于判断是否为管理员权限，与以管理员权限打开可执行文件
5. 新增文件事件类 `FileEvent`存在两个功能: `RemoveFile`、 `CreateDir`
6. RemoveFile已被弃用，现在它与创建文件夹函数一起放在FileEvent类中，后续这个类将退出新功能
7. 更新函数kwp::getRunTime()，类似python的time.time()

*2025/7/6更新*
#### 至此版本起，opencv使用4.11版本，编译器为mingw15，版本转为C++17标准(winKeyPress4.2版本)
1. 增加宽字符转为普通字符，WStrToStr
2. StrToWStr函数更新，增加了gbk与utf8的选项转化
3. 新增函数addTempPath在命名空间内，支持添加临时环境变量
4. FileEvent类增加功能: `RemoveDir`，支持删除指定文件夹

*2025/7/24更新*
1. 新增函数，useConsoleUtf8()，允许控制台输入输出utf-8编码，适用于mingw
2. FileEvent类增加功能: `WriteLog`，用于写入日志
3. FileEvent类增加功能: `CopyDir`，用于复制文件夹
4. wkp命名空间新增功能 `openDir` ，函数用于打开指定目录
5. MouseEvent类增加功能: 可以监听鼠标侧边按钮

*2025/8/11更新*
1. 增加函数 `sort` 实现简单的冒泡排序，可以升序与降序
2. 新增函数 `getNowTime` 返回当前时间的字符串
3. 新增函数 `getExePath` 返回当前exe所在路径
4. wkp命名空间新增类 `open` 实现c/c++的读写操作,但限于filesystem仅在utf8编码才不会报错
5. 新增函数 `wkp::getConsoleEncoding` 获取控制台编码
6. MouseMoveTo以及MouseClick函数增加参数，可以设置延迟抵达目标坐标，支持float和int


[返回目录](#目录)  
