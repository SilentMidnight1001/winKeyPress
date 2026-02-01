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
7. [弹窗与窗口控制](#弹窗与窗口控制)
8. [进程与系统管理](#进程与系统管理)
9. [清理与多线程](#清理与多线程)
10. [完整示例](#完整示例)
11. [注意事项](#注意事项)

---

## 概述
该库为Windows平台提供鼠标/键盘模拟、热键注册、剪贴板操作、图像识别等功能，适用于自动化脚本开发。核心特性包括：
- **跨线程控制**：支持在独立线程中运行监听函数
- **链式操作**：可组合移动、点击、按键等事件
- **低延迟**：直接调用Windows API实现快速响应
- **图像识别**：支持通过模板匹配定位屏幕中的图像位置
- **编码转换**：提供UTF-8与GBK编码互转功能
- **窗口控制**：支持窗口置顶、消息弹窗等
- **进程管理**：支持进程终止、开机自启等功能
- **字典数据结构**：提供类似Python的Dict容器，保持插入顺序
- **格式化输出**：支持类似Python的print和format功能
- **正则表达式**：提供findall、search、sub等正则操作
- **颜色输出**：支持控制台彩色文本输出
- **随机数生成**：提供多种随机数生成功能
- **文件操作**：完整的文件读写、目录操作功能

---

## 快速开始
### 1. 引入头文件
```cpp  
#include "winKeyPressH.hpp"  
using namespace std;  
```  

### 2. 编译指令
```bash  
g++ main.cpp -o app -lUser32 -lOle32 -lgdiplus -lopencv_core -lopencv_highgui -lopencv_imgproc  
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
支持监听鼠标侧边按钮（前进/后退）
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
    case WM_FORWARDOWN:
        cout << "前进按钮按下" << endl;
        break;
    case WM_BACKSPACEDOWN:
        cout << "后退按钮按下" << endl;
        break;
}
```  

#### 8. `UninstallMouseEvent` - 注销鼠标监听事件
```cpp  
MouseEvent::UninstallMouseEvent(); // 注销所有鼠标事件，但不退出程序
```  

#### 9. `ExitMouseEvent` - 退出鼠标监听
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

#### 9. `GetOneKeyCode` - 获取按键码
```cpp  
long keyCode = keyboard.GetOneKeyCode();  
if (keyCode == KEY_A_DOWN) {  
    cout << "A键被按下" << endl;  
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

#### `GetSpecifiedAreaImagePosition` - 在指定区域内获取图像坐标
| 参数       | 类型   | 说明                   |  
|------------|--------|----------------------|  
| x1, y1     | int    | 区域左上角坐标           |  
| x2, y2     | int    | 区域右下角坐标           |  
| filePath   | string | 模板图像路径            |  
| threshold  | double | 匹配阈值（0.0~1.0）      |  
| notException | bool | 是否抛出异常（默认false） |  

**返回值**:  
`ImagePosition` 结构体

```cpp  
ImageEvent imgevent;
// 在屏幕指定区域(100,100)-(500,500)内查找图像
ImagePosition img = imgevent.GetSpecifiedAreaImagePosition(100, 100, 500, 500, "img.png", 0.85);  
```  

### 二、修改图像格式
#### `SetImageFormat` - 更改图像格式
| 参数       | 类型     | 说明             |  
|------------|--------|----------------|  
| imgPath   | string | 原图像格式路径(源图文件名) |  
| targetImageSavePath  | string | 目标格式路径 |  

```cpp  
ImageEvent imgevent;
imgevent.SetImageFormat("img.png", "img.jpg");
```  

### 三、修改图像尺寸
#### `SetImageSize` - 更改图像尺寸
| 参数      | 类型     | 说明             |  
|---------|--------|----------------|  
| imgPath | string | 原图像格式路径(源图文件名) |  
| width   | int    | 图片宽度           |  
| height  | int    | 图片高度           |  
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
| targetImageSavePath  | string | 保存路径(可选) |

```cpp  
int main()
{
    ImageEvent imge;
    imge.CaptureScreen("test", "../");
    return 0;
}
```  

### 五、区域截屏
#### `CaptureAreaScreen` - 区域截图
| 参数       | 类型     | 说明          |
|----------|--------|-------------|
| saveName | string | 截图保存的文件名与路径 |
| X1       | int    | 左上矩形x坐标     |
| Y1       | int    | 左上矩形y坐标     |
| X2       | int    | 左下矩形x坐标     |
| Y2       | int    | 左下矩形y坐标     |

```cpp  
int main()
{
    ImageEvent img;
    img.CaptureAreaScreen("区域截图", 654, 142, 2498, 973);
    return 0;
}
```  

---

## 实用工具

### 一、字符串编码类
#### `StringCode` - 字符串编码转换
| 函数              | 参数             | 类型                | 说明                                          |
|-----------------|----------------|-------------------|---------------------------------------------|
| GBKToUTF8       | gbkStr         | string            | 将GBK编码字符串转为UTF-8                      |
| Utf8ToGBK       | utf8Str        | string            | 将UTF-8编码字符串转为GBK                      |
| StrToWStr       | str            | string            | 将字符串转为宽字符                            |
| StrToWStr(重载) | str, isUtf8    | string, bool      | 根据布尔值转换编码(True为utf8转宽字符，False为gbk) |
| WStrToStr       | wstr           | wstring           | 将宽字符转为普通字符串                        |

```cpp  
StringCode scd;
string utf8Str = scd.GBKToUTF8("中文文本");  
string gbkStr = scd.Utf8ToGBK(utf8Str);  
wstring wstr = scd.StrToWStr("文本", true);
```  

### 二、文件操作类
#### `FileEvent` - 文件操作类
| 函数名        | 参数                                      | 返回值 | 说明                    |  
|------------|-----------------------------------------|------|-----------------------|
| RemoveFile | const std::string& fileName             | bool | 删除文件                  |
| CreateDir  | const std::string& fileName             | bool | 创建目录                  |
| RemoveDir  | const std::string& dirPath              | bool | 删除目录(文件夹)             |
| WriteLog   | const std::string& str, const std::string& fileName, bool utf8=true | bool | 写日志(a追加模式)           |
| CopyDir    | const fs::path& sourcePath, const fs::path& targetPath | bool | 复制文件夹到目标路径           |

```cpp  
int main()
{
    FileEvent f;
    bool a = f.CreateDir("../Test");
    bool b = f.RemoveFile("../Test/Test.txt");
    bool c = f.RemoveDir("D:/Test/test");
    bool d = f.WriteLog("日志...", "test.log");
    bool e = f.CopyDir("./", "D:/Test");
    return 0;
}
```  

### 三、文件IO操作
#### `wkp::Open` - 文件读写类
| 函数          | 参数                                      | 返回值                  | 说明                            |
|-------------|-----------------------------------------|-----------------------|-------------------------------|
| 构造函数       | const string &file_path, const string &file_mode, bool isUtf8=true, bool auto_close=true | - | 打开文件,"r", "w", "a", "rb", "wb", "ab" |
| open        | const string &file_path, const string &file_mode, bool isUtf8=true, bool auto_close=true | void | 打开文件                        |
| isOpen      | -                                       | bool                  | 返回文件是否打开                    |
| read        | -                                       | string                | 读取整个文件                      |
| readLine    | -                                       | vector<string>        | 逐行读取文件内容                    |
| readBinary  | -                                       | vector<unsigned char> | 读取整个二进制文件                   |
| readJson    | -                                       | Dict<string,string>   | 读取JSON文件                     |
| readBinaryToString | -                                   | string                | 二进制文件转字符串                  |
| write       | const string &str                       | void                  | 写入文件                         |
| writeBinary | const vector<unsigned char>& data       | void                  | 写入二进制文件                     |
| writeJson   | Dict<K, V> dicts                        | void                  | 写入JSON文件                     |
| writeStringToBinary | const string &str                  | void                  | 字符串转二进制写入文件                |
| close       | -                                       | void                  | 关闭文件                         |

```cpp  
int main()
{
    // 写入文件
    wkp::Open opt;
    opt.open("测试.txt", "w", true);
    opt.write("测试写入");
    opt.close();

    // 读取文件
    wkp::Open ipt;
    ipt.open("测试.txt", "r", true);
    string content = ipt.read();
    ipt.close();
    
    cout << content << endl;

    return 0;
}
```  

### 四、字典数据结构
#### `wkp::Dict` - 保持插入顺序的字典
| 函数/操作符 | 说明 | 示例 |
|------------|------|------|
| operator[] | 访问或插入键值对 | `dict["key"] = value` |
| insert | 插入键值对 | `dict.insert("key", value)` |
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

// 遍历字典（保持插入顺序）
for (auto& [key, value] : dict.items()) {
    wkp::println("{}: {}", key, value);
}

// 获取键和值列表
auto keys = dict.keys();
auto values = dict.values();

// 删除键值对
dict.pop("banana");

// 支持print和format输出
wkp::println("字典内容: {}", dict);
string formatted = wkp::format("水果数量: {}", dict);
```  

### 五、字符串转字典
#### `wkp::stringToJson` - JSON字符串转字典
```cpp  
string jsonStr = "{\"name\": \"John\", \"age\": 30}";
auto dict = wkp::stringToJson(jsonStr);
wkp::println("姓名: {}, 年龄: {}", dict["name"], dict["age"]);
```  

### 六、管理员权限
#### `wkp::isAdmin` - 判断是否为管理员权限
| 参数   | 类型   | 说明      |
|------|------|---------|
| void | bool | 结果返回布尔值 |

```cpp  
bool admin = wkp::isAdmin();
```  

#### `wkp::openAdmin` - 以管理员身份打开可执行文件
| 参数   | 类型   | 说明      |
|------|------|---------|
| void | bool | 结果返回布尔值 |

```cpp  
bool admin = wkp::openAdmin();
```  

### 七、环境变量与编码设置
#### `wkp::addTempPath` - 添加临时环境变量
| 参数   | 类型                                             | 说明 |
|------|------------------------------------------------|----|
| path | const std::string& path                        | 要添加的路径 |
| path | const std::wstring& newPath                    | 要添加的宽字符路径 |

```cpp  
// 普通字符串变量 //
std::string pythonPath = "D:\\Python嵌入式\\python嵌入式";
// 添加到PATH
wkp::addTempPath(pythonPath);

// 宽字符串变量 //
std::wstring widePath = L"D:\\QtImageTools\\python嵌入式";
// 添加到PATH
wkp::addTempPath(widePath);
```  

#### `wkp::useConsoleUtf8` - 设置控制台编码为UTF-8
```cpp  
wkp::useConsoleUtf8();
```  

#### `wkp::getConsoleEncoding` - 获取控制台编码
```cpp  
string encoding = wkp::getConsoleEncoding();
cout << "控制台编码: " << encoding << endl;
```  

### 八、文件夹与文件操作
#### `wkp::listDir` - 获取文件夹内文件列表（C++17 filesystem）
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
| path | const std::string& pat | 要打开的文件夹路径 |
| utf8 | bool            | 是否使用UTF-8编码（默认true） |

```cpp  
wkp::openDir("C:/MyFolder");
```  

### 九、排序与时间函数
#### `wkp::sort` - 冒泡排序
| 函数   | 参数                                                          | 返回值         | 说明                   |
|------|-------------------------------------------------------------|-------------|----------------------|
| sort | const int *numList, int numListSize, const string &mode     | vector<int> | 简单的冒泡排序，支持升序和降序 |
| sort | const std::vector<int> &numList, const string &model        | vector<int> | 向量版本的冒泡排序         |

```cpp  
int numList[] = {5, 2, 8, 1, 9};
// 降序
auto a = wkp::sort(numList, 5, "down");
for(auto i:a) {
    cout << i << " ";
}

// 升序
auto b = wkp::sort(numList, 5, "up");
for(auto i:b) {
    cout << i << " ";
}
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

#### `wkp::getNowTime` - 获取当前时间
| 函数   | 参数 | 返回值   | 说明                   |
|------|----|--------|----------------------|
| getNowTime | 无  | string | 返回当前时间字符串 |

```cpp  
string time = wkp::getNowTime();
cout << "当前时间: " << time << endl;
```  

#### `wkp::getRunTime` - 获取运行时间
| 函数   | 参数 | 返回值   | 说明                   |
|------|----|--------|----------------------|
| getRunTime | 无  | double | 类似Python的time.time() |

```cpp  
double start = wkp::getRunTime();
// 执行一些操作
double end = wkp::getRunTime();
cout << "耗时: " << (end - start) << "秒" << endl;
```  

### 十、路径获取
#### `wkp::getExePath` - 获取当前exe所在路径
| 函数   | 参数   | 返回值   | 说明                   |
|------|------|--------|----------------------|
| getExePath | bool | string | 获取当前exe所在路径 |

```cpp  
string path = wkp::getExePath(false);
cout << "当前路径: " << path << endl;
```  

### 十一、延迟函数
#### `wkp::sleep` - 延迟函数
| 函数   | 参数   | 返回值 | 说明                   |
|------|------|------|----------------------|
| sleep | double delay | void | 延迟指定秒数 |

```cpp  
wkp::sleep(1.5); // 延迟1.5秒
```  

### 十二、类型识别
#### `wkp::typeName` - 获取变量类型名称
| 函数   | 参数     | 返回值   | 说明                   |
|------|--------|--------|----------------------|
| typeName | Types type | string | 获取变量类型名称 |

```cpp  
auto type = wkp::typeName(123);
cout << "类型: " << type << endl; // 输出: int
```  

### 十三、字符串格式化
#### `wkp::string_format` - 字符串格式化（C风格）
| 函数   | 参数     | 返回值   | 说明                   |
|------|--------|--------|----------------------|
| string_format | const char* format, ... | string | 安全的字符串格式化函数 |

```cpp  
string formatted = wkp::string_format("姓名: %s, 年龄: %d", "张三", 25);
cout << formatted << endl;
```  

#### `wkp::format` - 智能映射（C++风格）
| 函数   | 参数     | 返回值   | 说明                   |
|------|--------|-------|----------------------|
| format | const std::string& fmt, Args&&... args | 映射字符串 |

```cpp  
vector<int> a = {1, 2, 3};
string str = "hello {}";
wkp::print("输出: {}", a);

// 更多测试
vector<vector<int>> b = {{1, 2}, {3, 4}};
cout << wkp::format("嵌套数组: {}", b) << endl;  // 输出: 嵌套数组: [[1, 2], [3, 4]]

int arr[] = {5, 6, 7};
cout << wkp::format("C数组: {}", arr) << endl;  // 输出: C数组: [5, 6, 7]

// 支持map和Dict类型
std::map<string, int> mp = {{"a", 1}, {"b", 2}};
cout << wkp::format("map: {}", mp) << endl;  // 输出: map: {"a": 1, "b": 2}
```  

### 十四、输出函数
#### `wkp::print` / `wkp::println` - 智能输出
| 函数   | 参数     | 返回值 | 说明                   |
|------|--------|------|----------------------|
| print | const T& first, const Args&... rest | void | 智能判断输出模式，不换行 |
| println | const T& first, const Args&... rest | void | 智能判断输出模式，换行 |

```cpp  
wkp::print("Hello"); // 不换行输出
wkp::println("World"); // 换行输出
vector<int> a = {1, 2, 3}
wkp::println("姓名: {}, 年龄: {}, 数组/vector: {}", "李四", 30, a); // 格式化输出
```  

#### 颜色输出
```cpp  
#include "winKeyPressH.hpp"

// 使用颜色输出
wkp::println(color::printColor("红色文本", Color("red")));
wkp::println(color::printColor("绿色文本", Color("green")));
wkp::println(color::printColor("蓝色文本", Color("blue")));
wkp::println(color::printColor("黄色文本", Color("yellow")));

// 嵌套颜色使用
wkp::print("正常文本");
Print::pushColor(Color("red"));
wkp::print(color::printColor("红色文本", Color("red")));
Print::pushColor(Color("green"));
wkp::print(color::printColor("绿色文本", Color("green")));
Print::popColor(); // 返回到红色
wkp::print(color::printColor("又是红色文本", Color("red")));
Print::popColor(); // 返回到默认颜色
wkp::println("正常文本");
```  

#### `Color` 类 - 颜色管理
支持多种颜色格式：
- RGB值：`Color(255, 0, 0)` 红色
- 颜色名称：`Color("red")` 红色
- 十六进制：`Color("#FF0000")` 红色

```cpp  
Color red(255, 0, 0);
Color green("green");
Color blue("#0000FF");
```  

### 十五、随机数功能
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
```  

### 十六、正则表达式功能
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

---

## 弹窗与窗口控制

### 一、消息弹窗
#### `WMessageBox` - 消息框类
| 函数        | 参数                                      | 返回值 | 说明                |
|-----------|-----------------------------------------|------|-------------------|
| showinfo  | const string title, const string content, bool utf8=true | void | 显示信息提示框          |
| showwarning | const string title, const string content, bool utf8=true | void | 显示警告提示框          |
| showerror | const string title, const string content, bool utf8=true | void | 显示错误提示框          |
| askyesno  | const string title, const string content, const string model="info", bool utf8=true | bool | 显示是/否选择框，返回选择结果 |

```cpp  
WMessageBox msg;
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

### 二、窗口置顶
#### `WindowTop` - 窗口置顶控制
| 函数        | 参数                                      | 返回值 | 说明                |
|-----------|-----------------------------------------|------|-------------------|
| 构造函数     | const string& hwndName, int pxc=0, int pyc=0, int wc=0, int hc=0 | - | 通过窗口标题创建 |
| 构造函数     | HWND hwnd, int pxc=0, int pyc=0, int wc=0, int hc=0 | - | 通过窗口句柄创建 |
| setWindowsTopOn | - | void | 开启窗口置顶 |
| setWindowsTopOff | - | void | 关闭窗口置顶 |

```cpp  
// 通过窗口标题设置置顶
WindowTop top("计算器", 0, 0, 400, 300);
top.setWindowsTopOn(); // 开启置顶

// 一段时间后取消置顶
this_thread::sleep_for(chrono::seconds(10));
top.setWindowsTopOff(); // 关闭置顶
```  

---

## 进程与系统管理

### 一、进程管理
#### `wkp::KillProcess` - 终止进程
| 函数   | 参数     | 返回值   | 说明                   |
|------|--------|--------|----------------------|
| KillProcess | const fs::path &processName | bool | 根据进程名终止进程 |

```cpp  
bool success = wkp::KillProcess("notepad.exe");
if (success) {
    cout << "进程终止成功" << endl;
}
```  

#### `wkp::KillProcess` - 增强版进程终止
| 函数   | 参数     | 返回值   | 说明                   |
|------|--------|--------|----------------------|
| KillProcess | const fs::path& processName, bool killTree, bool partialMatch | bool | 增强版进程终止 |
| KillProcess | DWORD pid, bool killTree = false | bool | 通过PID终止进程 |

```cpp  
// 终止进程树
bool success = wkp::KillProcess("chrome.exe", true, false);
// 通过PID终止
bool success2 = wkp::KillProcess(1234, true);
```  

#### `wkp::startOtherApp` - 启动其他应用程序
| 函数   | 参数     | 返回值   | 说明                   |
|------|--------|--------|----------------------|
| startOtherApp | const string& appPath | bool | 创建新进程打开其他软件 |

```cpp  
bool success = wkp::startOtherApp("D:\\Programs\\myapp.exe");
if (success) {
    cout << "程序启动成功" << endl;
}
```  

### 二、开机自启管理
#### `wkp::setAutoStartUp` - 设置开机自启
| 函数   | 参数     | 返回值   | 说明                   |
|------|--------|--------|----------------------|
| setAutoStartUp | const std::string &appName, bool isUtf8=true | bool | 将当前可执行文件列入开机自启注册表 |
| setAutoStartUp | const std::string &exePath, const std::string &appName, bool isUtf8=true | bool | 将指定文件路径的可执行文件列入开机自启注册表 |

```cpp  
bool a = wkp::setAutoStartUp("myapp"); // 当前程序开机自启
bool b = wkp::setAutoStartUp("C:\\Programs\\myapp.exe", "myapp2"); // 指定程序开机自启
```  

#### `wkp::disableAutoStart` - 禁用开机自启
| 函数   | 参数     | 返回值   | 说明                   |
|------|--------|--------|----------------------|
| disableAutoStart | const string& appName, bool isUtf8=true | bool | 禁止并删除开机自启 |

```cpp  
bool success = wkp::disableAutoStart("myapp");
if (success) {
    cout << "开机自启已禁用" << endl;
}
```  

### 三、注册表操作
#### `wkp::getRegistryPath` - 获取注册表键值
| 函数   | 参数     | 返回值   | 说明                   |
|------|--------|--------|----------------------|
| getRegistryPath | const std::string &keyPath, const std::string &valueName | string | 获取注册表键值的路径 |

```cpp  
string path = wkp::getRegistryPath("Software\\Microsoft\\Windows\\CurrentVersion\\Run", "myapp");
cout << "注册表路径: " << path << endl;
```  

### 四、PowerShell调用
#### `wkp::callPowerShell` - 执行PowerShell命令
| 函数   | 参数     | 返回值   | 说明                   |
|------|--------|--------|----------------------|
| callPowerShell | const string& cmd | bool | 执行PowerShell命令 |

```cpp  
bool success = wkp::callPowerShell("Get-Process");
if (success) {
    cout << "PowerShell命令执行成功" << endl;
}
```  

### 五、DPI设置
#### `wkp::setAutoDpi` - 自动调整DPI
```cpp  
wkp::setAutoDpi(); // 设置进程DPI感知，确保在高DPI屏幕上正确获取坐标
```  

### 六、窗口锁定
#### `wkp::lockWindows(string/int)` - 自动调整DPI
```cpp  
    auto a = wkp::lockWindows("元宝");
    auto n = wkp::lockWindows(1001);
```  

---

## 清理与多线程
### 自动清理
- 析构时自动释放未松开的按键
- 手动清理方法：
  ```cpp  
  KeyBoardEvent::ClearHotKey(); // 清除所有注册热键  
  MouseEvent::ExitMouseEvent(); // 停止鼠标监听  
  KeyBoardEvent::ExitOneKeyEvent(); // 停止单按键监听
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
ExitCheckWork(); // 注册退出时的清理函数
```  

---

## 完整示例
```cpp  
#include <thread>  
#include "winKeyPressH.hpp"  

int main() {  
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
    
    // 文件操作示例
    wkp::Open file;
    file.open("data.json", "w");
    file.writeJson(fruitDict);
    file.close();
    
    // 字符串编码转换示例  
    StringCode converter;  
    string utf8Text = converter.GBKToUTF8("自动化脚本");  

    // 键盘热键示例  
    KeyBoardEvent keyboard;  
    keyboard.AddHotKey("ctrl+alt+l", [](){  
        wkp::println("热键触发：锁定系统");  
    });  

    // 消息弹窗示例
    WMessageBox msg;
    msg.showinfo("提示", "自动化脚本启动成功!");

    // 窗口置顶示例
    WindowTop top("记事本");
    top.setWindowsTopOn();

    // 启动监听线程  
    thread hotkeyThread([&](){  
        keyboard.WaitHotKey();  
    });  
    hotkeyThread.detach();  

    // 注册退出清理
    ExitCheckWork();

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
6. **DPI适配**：在高DPI屏幕上使用前调用 `wkp::setAutoDpi()`
7. **线程安全**：监听函数需要在独立线程中运行
8. **字典顺序**：`wkp::Dict` 保持插入顺序，适合需要顺序遍历的场景
9. **颜色输出**：控制台颜色输出需要支持ANSI转义序列的控制台
10. **正则表达式**：使用正则表达式功能需要包含 `<regex>` 头文件
11. **随机数**：随机数生成器需要包含 `<random>` 头文件
12. **文件系统**：文件操作功能需要C++17支持（`<filesystem>`）
13. **Windows版本**：部分API可能需要特定Windows版本支持
14. **图像识别范围**：`GetSpecifiedAreaImagePosition` 支持在指定区域内进行图像匹配

### 更新日志
*2025/4/27更新*
1. 新增单按键监听功能 `ListenOneKeyEvent`
2. 优化 `PressHotKey` 支持任意数量组合键

*2025/12/9更新*
1. 新增颜色输出功能，支持控制台彩色文本
2. 完善颜色管理类 `Color` 和 `ColoredText`
3. 添加颜色堆栈管理功能
4. 优化格式化输出对颜色对象的支持
5. 增强正则表达式功能，支持回调函数替换
6. 完善文档说明和示例代码

*2026/1/3更新*
1. 新增字符串编码类 `StringCode`
2. 新增文件操作类 `FileEvent`
3. 新增弹窗类 `WMessageBox`
4. 新增窗口置顶类 `WindowTop`
5. 新增类型识别功能 `GetValueType` 和 `wkp::typeName`
6. 新增颜色类 `Color` 支持RGB、颜色名称和十六进制
7. 新增随机数命名空间 `random`，提供多种随机数生成功能
8. 新增 `wkp` 命名空间下的多个实用函数
9. 新增 `wkp::Dict` 字典数据结构，保持插入顺序
10. 新增 `wkp::Open` 文件读写类，支持文本、二进制和JSON
11. 新增正则表达式命名空间 `re`
12. 新增字符串转字典功能 `wkp::stringToJson`
13. 全面完善文档说明和示例代码

*2026/1/28更新*
1. 新增区域图像识别功能 `GetSpecifiedAreaImagePosition`
2. 增强进程管理功能，新增增强版 `KillProcess` 支持进程树终止和PID终止
3. 优化文档结构和示例代码
4. 修复部分已知问题和文档错误
5. 2026/2/1更新 新增函数 `lockWindows` 锁定窗口