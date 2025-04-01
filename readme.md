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
5. [��������߳�](#��������߳�)  
6. [����ʾ��](#����ʾ��)  
7. [ע������](#ע������)  

---

## ����
�ÿ�ΪWindowsƽ̨�ṩ���/����ģ�⡢�ȼ�ע�ᡢ����������ȹ��ܣ��������Զ����ű��������������԰�����
- **���߳̿���**��֧���ڶ����߳������м�������  
- **��ʽ����**��������ƶ���������������¼�  
- **���ӳ�**��ֱ�ӵ���Windows APIʵ�ֿ�����Ӧ  

---

## ���ٿ�ʼ
### 1. ����ͷ�ļ�
```cpp
#include "winKeyPressH.hpp"
using namespace std;
```

### 2. ����ָ��
```bash
g++ main.cpp -o app -lUser32 -lOle32
```

---

## ������

### ��ʼ��
```cpp
MouseEvent mouse; // Ĭ�ϳ�ʼ��
```

### �������

#### 1. `GetScreenSIze` - ��ȡ��Ļ�ֱ���
| ����    | ���� | ˵��                      |
|---------|------|-------------------------|
| output  | bool | �Ƿ�����ֱ��ʵ�����̨��Ĭ��true�� |

**����ֵ**:  
`ScreenSize` �ṹ�壬���� `x`����ȣ���`y`���߶ȣ�����  

```cpp
ScreenSize screen = mouse.GetScreenSIze();
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
keyboard.WriteStr("admin@123"); // ���ַ�����
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
    // ������
    MouseEvent mouse;
    mouse.MouseMoveTo(500, 300);
    mouse.MouseClick(500, 300, 2); // ˫�����

    // ���̲���
    KeyBoardEvent keyboard;
    keyboard.AddHotKey("ctrl shift p", [](){
        cout << "�ȼ�����: ��������" << endl;
    });

    // �����ȼ������߳�
    thread hotkeyThread([&](){
        keyboard.WaitHotKey();
    });
    hotkeyThread.detach();

    // ���߳������߼�
    while(true) {
        this_thread::sleep_for(chrono::seconds(1));
    }
    return 0;
}
```

---

## ע������
1. **����ԱȨ��**�����ֲ������Թ���Ա������г���  
2. **������ͻ**�������ظ�ע��ϵͳ���ȼ����� `alt+F4`��  
3. **�ӳ�����**�����������佨����Ӷ������ߣ�
   ```cpp
   this_thread::sleep_for(chrono::milliseconds(50));
   ```
4. **��������**��`WriteStr` ��֧��ASCII�ַ����������ü����巽ʽ  
5. **���Խ���**��������������ǰ�����־���ȷ���߳�״̬  

[����Ŀ¼](#Ŀ¼)