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
8. [������־](#������־)

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
Sleep(10);  // ����ʵ��ӳټ���CPUռ��
}


printf("left up: %d\n right down: %d\n", aa, bb);
return 0;
}
```

#### 8. `UninstallMouseEvent` - ע��������
**˵��**:  
ֹͣ����¼������̣߳������˳�����

```cpp
mouse.UninstallMouseEvent(); // ֹͣ�������Կ���������
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
hotkeyThread.detach();
```

#### 8. `UninstallKeyEvent` - �����ݼ�ע��
**˵��**:  
���������ע����ȼ��󶨣�����Ӱ���������̲���

```cpp
keyboard.UninstallKeyEvent(); // ������������ע�����ȼ�
```

---

## ��������߳�
### �ֶ�������
| ����                        | ˵��                          |
|----------------------------|-----------------------------|
| `KeyBoardEvent::ClearHotKey()` | �������ע���ȼ��������ã�����ʹ��UninstallKeyEvent�� |
| `MouseEvent::ExitMouseEvent()` | ��ȫֹͣ�����������˳�����̣߳�          |
| `UninstallMouseEvent()`       | ֹͣ�����������ֳ�������              |
| `UninstallKeyEvent()`         | ����ȼ�ע�ᵫ�������̲�������            |

### �Զ�����
- ����ʱ�Զ��ͷ�δ�ɿ��İ���
- �����˳�ʱ�Զ�ִ��`ExitMouseEvent()`

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

    // �������ǰ����
    mouse.UninstallMouseEvent();
    keyboard.UninstallKeyEvent();
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
6. **��Դ�ͷ�**����ʱ�����г���ʱ��Ӧ���ڵ���`Uninstall`ϵ�з����ͷ�ϵͳ��Դ

---

## ������־
### v1.1.0 - 2023-08-20
**��������**:
- ����`MouseEvent::UninstallMouseEvent()`��������ֹͣ�����������˳�����
- ����`KeyBoardEvent::UninstallKeyEvent()`����������ע���ȼ��������������̹���

### v1.0.0 - 2023-07-15
**��ʼ�汾����**:
- �������/����ģ�����
- �ȼ�ע�����������
- ���������֧��

[����Ŀ¼](#Ŀ¼)