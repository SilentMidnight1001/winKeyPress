[file name]: readme.md
[file content begin]
# WinKeyPress ���ĵ�

## Ŀ¼
1. [����](#����)
2. [���ٿ�ʼ](#���ٿ�ʼ)
3. [������](#������)
   - [��ʼ��](#��ʼ��)
   - [�������](#�������)
4. [���̲���](#���̲���)
   - [��ʼ��](#��ʼ��-1)
   - [�������](#�������-1)
5. [ͼ��ʶ��](#ͼ��ʶ��)
6. [ʵ�ù���](#ʵ�ù���)
7. [��������߳�](#��������߳�)
8. [����ʾ��](#����ʾ��)
9. [ע������](#ע������)

---  

## ����
�ÿ�ΪWindowsƽ̨�ṩ���/����ģ�⡢�ȼ�ע�ᡢ�����������ͼ��ʶ��ȹ��ܣ��������Զ����ű��������������԰�����
- **���߳̿���**��֧���ڶ����߳������м�������
- **��ʽ����**��������ƶ���������������¼�
- **���ӳ�**��ֱ�ӵ���Windows APIʵ�ֿ�����Ӧ
- **ͼ��ʶ��**��֧��ͨ��ģ��ƥ�䶨λ��Ļ�е�ͼ��λ��
- **����ת��**���ṩUTF-8��GBK���뻥ת����

---  

## ���ٿ�ʼ
### 1. ����ͷ�ļ�
```cpp  
#include "winKeyPressH.hpp"  
using namespace std;  
```  

### 2. ����ָ��
```bash  
g++ main.cpp -o app -lUser32 -lOle32 -lopencv_core -lopencv_highgui -lopencv_imgproc  
```  
*ע��ʹ��ͼ��ʶ����������OpenCV�⡣*

---  

## ������

### ��ʼ��
```cpp  
MouseEvent mouse; // Ĭ�ϳ�ʼ��  
```  

### �������

#### 1. `GetScreenSize` - ��ȡ��Ļ�ֱ���
| ����    | ���� | ˵��                      |  
|---------|------|-------------------------|  
| output  | bool | �Ƿ�����ֱ��ʵ�����̨��Ĭ��true�� |  

**����ֵ**:  
`ScreenSize` �ṹ�壬���� `x`����ȣ���`y`���߶ȣ�����

```cpp  
ScreenSize screen = mouse.GetScreenSize();  
cout << "��ǰ�ֱ���: " << screen.x << "x" << screen.y;  
```  

#### 2. `GetMousePosition` - ��ȡ�������
**����ֵ**:  
`MousePosition` �ṹ�壬���� `x`��`y` ����

```cpp  
MousePosition pos = mouse.GetMousePosition();  
cout << "��ǰ����: (" << pos.x << ", " << pos.y << ")";  
```  

#### 3. `MouseMoveTo` - �ƶ����
| ���� | ���� | ˵��         |  
|------|------|------------|  
| x    | int  | Ŀ��X���꣨���أ� |  
| y    | int  | Ŀ��Y���꣨���أ� |  

```cpp  
mouse.MouseMoveTo(0, 0); // �ƶ�����Ļ���Ͻ�  
```  

#### 4. `MouseClick` - �������
| ����     | ����     | ��ѡֵ               | Ĭ��ֵ   |  
|----------|----------|---------------------|--------|  
| x, y     | int      | -                   | ��      |  
| clicks   | int      | ��1                  | 1       |  
| button   | string   | "left", "right"     | "left" |  

```cpp  
// ��(100,200)���Ҽ�˫��  
mouse.MouseClick(100, 200, 2, "right");  
```  

#### 5. `MouseDown`/`MouseUp` - ����/�ͷ�
```cpp  
mouse.MouseDown();    // �������  
this_thread::sleep_for(chrono::seconds(1));  
mouse.MouseUp();      // �ͷ����  
```  

#### 6. `MouseRoll` - ���ֿ���
| ����  | ���� | ˵��           |  
|-------|------|--------------|  
| move  | int  | �������ϣ��������� |  

```cpp  
mouse.MouseRoll(120);  // ���Ϲ���һҳ  
```  

#### 7. `ListenMouseEvent` - �¼�����
```cpp  
// ���ڶ����߳�����  
thread mouseThread([&](){  
    mouse.ListenMouseEvent();  
});  
mouseThread.detach();  
```  

---  

## ���̲���

### ��ʼ��
```cpp  
KeyBoardEvent keyboard;  
```  

### �������

#### 1. `KeyDown`/`KeyUp` - ��������
| ���� | ����   | ʾ��ֵ                |  
|------|--------|----------------------|  
| key  | string | "a", "ctrl", "F1"�� |  

```cpp  
keyboard.KeyDown("alt");  
keyboard.KeyDown("tab");  
keyboard.KeyUp("tab");  
keyboard.KeyUp("alt"); // �л�����  
```  

#### 2. `PressKey` - ���²��ͷ�
```cpp  
keyboard.PressKey("enter"); // ģ��س���  
```  

#### 3. `PressHotKey` - ��ϼ�
| ����    | ����   | ˵��                   |  
|---------|--------|----------------------|  
| key1-3  | string | ���μ������3����         |  

```cpp  
keyboard.PressHotKey("win", "r"); // �����д���  
```  

#### 4. `WriteStr` - �ַ�������
```cpp  
keyboard.WriteStr("admin@123"); // ���ַ����루��֧��ASCII��  
```  

#### 5. `CopyStr` - ���������
```cpp  
keyboard.CopyStr("�����Ƶ��ı�");  
keyboard.PressHotKey("ctrl", "v"); // ճ��  
```  

#### 6. `AddHotKey` - ע���ȼ�
| ����            | ����               | ˵��               |  
|-----------------|--------------------|------------------|  
| key             | string             | ��ʽ�� "ctrl shift k" |  
| targetVoidFunc  | std::function<void()> | ����ʱִ�еĺ���       |  

```cpp  
keyboard.AddHotKey("ctrl+alt+t", [](){  
    system("start cmd.exe"); // ��������  
});  
```  

#### 7. `WaitHotKey` - �����ȼ�
```cpp  
thread hotkeyThread([&](){  
    keyboard.WaitHotKey(); // ��������  
});  
hotkeyThread.join();  
```  

#### 8. `ListenOneKeyEvent` - ����������
```cpp  
// �ڶ����߳��е��ü�������  
KeyBoardEvent k;  
thread keyThread([&](){  
    k.ListenOneKeyEvent();  
});  
keyThread.detach();  

// ��ѭ���л�ȡ������  
while (true) {  
    long keyCode = k.GetOneKeyCode();  
    if (keyCode == KEY_ESC_DOWN) {  
        KeyBoardEvent::ExitOneKeyEvent(); // �˳�����  
        break;  
    }  
}  
```  

---  

## ͼ����

### һ��ͼ��ʶ��
#### `GetImagePosition` - ��ȡͼ������Ļ�е�λ��
| ����       | ����   | ˵��                   |  
|------------|--------|----------------------|  
| filePath   | string | ģ��ͼ��·����PNG��ʽ�Լ�������ʽ��  |  
| threshold  | double | ƥ����ֵ��0.0~1.0���Ƽ�0.85�� |  

**����ֵ**:  
`ImagePosition` �ṹ�壬���� `x`������X���꣩��`y`������Y���꣩��`result`���Ƿ�ƥ��ɹ�������

```cpp  
ImageEvent imgevent;
ImagePosition img = imgevent.GetImagePosition("img.png", 0.85);  
if (img.result) {  
    MouseEvent m;  
    m.MouseClick(img.x, img.y, 1, "left"); // ���ͼ������  
}  
```  

### �����޸�ͼ���ʽ
#### `SetImageFormat` - ����ͼ���ʽ
| ����       | ����     | ˵��                    |  
|------------|--------|-----------------------|  
| imgPath   | string | ԭͼ���ʽ·��(Դͼ�ļ���)        |  
| targetImageFormat  | string | ��Ҫ���ĵ�Ŀ���ʽ���ƣ��޸ĸ�ʽ���ļ����� |  

```cpp  
ImageEvent imgevent;
ImagePosition img = imgevent.GetImagePosition("img.png", 0.85);  
img.SetImageFormat("img.png", "ing.jpg");
```  

### ������������
#### `CaptureScreen` - ��ȡ������Ļ
| ����       | ����     | ˵��                    |
|------------|--------|-----------------------|
| imageName  | string | ��ͼ������ļ��� |

```cpp  
ImageEvent imgevent;
ImagePosition img = imgevent.GetImagePosition("img.png", 0.85);  
img.CaptureScreen("img");
```  

## ����Ա�ж�
#### `wkp::isAdmin` - �ж��Ƿ�Ϊ����ԱȨ��
| ����   | ����   | ˵��      |
|------|------|---------|
| void | bool | ������ز���ֵ |

```cpp  
bool admin = wkp::isAdmin();
```  

## �Թ���Ա��ݴ򿪿�ִ���ļ�
#### `wkp::openAdmin` - �ж��Ƿ�Ϊ����ԱȨ��
| ����   | ����   | ˵��      |
|------|------|---------|
| void | bool | ������ز���ֵ |
```cpp  
bool admin = wkp::openAdmin();
```  

---  

## ʵ�ù���

### 1. �ַ�������ת����StringCode�ࣩ
#### ������
- **`GBKToUTF8`**: ��GBK�����ַ���תΪUTF-8
- **`Utf8ToGBK`**: ��UTF-8�����ַ���תΪGBK

```cpp  
StringCode converter;  
string utf8Str = converter.GBKToUTF8("�����ı�");  
string gbkStr = converter.Utf8ToGBK(utf8Str);  
```  

### 2. ��ȡ�ļ������ļ��б�
#### ������
`GetListDir` - ����ָ��Ŀ¼�µ������ļ�����Ŀ¼����

| ����           | ����   | ˵��           |  
|----------------|--------|--------------|  
| directoryPath  | string | Ŀ���ļ���·��    |  

**����ֵ**:  
`vector<string>` �����ļ�/Ŀ¼��������

```cpp  
vector<string> files = GetListDir("C:/MyFolder");  
for (auto& file : files) {  
    cout << file << endl;  
}  
```  

---  

## ��������߳�
### �Զ�����
- ����ʱ�Զ��ͷ�δ�ɿ��İ���
- �ֶ���������
  ```cpp  
  KeyBoardEvent::ClearHotKey(); // �������ע���ȼ�  
  MouseEvent::ExitMouseEvent(); // ֹͣ������  
  ```  

### �̹߳���
- **����** �� `ListenMouseEvent` �� `WaitHotKey` ���ڶ����߳�
- �Ƽ�ʹ�� `detach` �������߳�������
  ```cpp  
  thread t([&](){ keyboard.WaitHotKey(); });  
  t.detach();  
  ```  

---  

## ����ʾ��
```cpp  
#include <thread>  
#include "winKeyPressH.hpp"  

int main() {  
    // ͼ��ʶ��ʾ��  
    ImagePosition img = GetImagePosition("button.png", 0.9);  
    if (img.result) {  
        MouseEvent mouse;  
        mouse.MouseClick(img.x, img.y); // ���ʶ�𵽵İ�ť  
    }  

    // �ַ�������ת��ʾ��  
    StringCode converter;  
    string utf8Text = converter.GBKToUTF8("�Զ����ű�");  

    // �����ȼ�ʾ��  
    KeyBoardEvent keyboard;  
    keyboard.AddHotKey("ctrl+alt+l", [](){  
        cout << "�ȼ�����������ϵͳ" << endl;  
    });  

    // ���������߳�  
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

## ע������
1. **����ԱȨ��**�����ֲ������Թ���Ա������г���
2. **������ͻ**�������ظ�ע��ϵͳ���ȼ����� `alt+F4`��
3. **ͼ��ʶ������**������ȷ��װOpenCV�Ⲣ���ñ��뻷��
4. **���������**��������������ļ�·������ʹ��GBK����
5. **���Խ���**��������������ǰ�����־���ȷ���߳�״̬

### ������־
*2025/4/27����*
1. ������������������ `ListenOneKeyEvent`
2. �Ż� `PressHotKey` ֧������������ϼ�

*2025/5/5����*
1. ����ͼ��ʶ���� `GetImagePosition`

*2025/5/7����*
1. �����ַ�������ת���� `StringCode`
2. �����ļ����ļ��б��ȡ���� `GetListDir`

*2025/5/18����*
1. ����ͼ������ `ImageEvent`
2. MouseEvent�������ܣ�Ĭ�ϵ��1�Σ���ťΪ��� `MouseClick(int x, int y)`
3. GetImagePosition�ƶ���ͼ������ `ImageEvent`
4. ���������ռ� `wkp`��������������:`isAdmin`��`openAdmin`�����ж��Ƿ�Ϊ����ԱȨ�ޣ����Թ���ԱȨ�޴򿪿�ִ���ļ�
[����Ŀ¼](#Ŀ¼)  
