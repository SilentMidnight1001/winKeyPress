#include <iostream>
#include "winKeyPressH.hpp"
#include <thread>
#include <unistd.h>
using namespace std;
bool run = true;
int a = 0;

// ��ȡ���λ�� //
void mousePositon()
{
    // Ĭ�����������̨ //
    MousePosition mousePosition = GetMousePosition(true);

    // ������洢������ //
    int x, y;
    x = mousePosition.x;
    y = mousePosition.y;
}

// ��ȡ��Ļ��С //
void screenSize()
{
    // Ĭ�����������̨ //
    ScreenSize screenSize = GetScreenSIze(true);

    // ������洢������ //
    int x, y;
    x = screenSize.x;
    y = screenSize.y;
}

/****** ����MouseEvent���ʹ�÷��� ******/
// ����ƶ�������������������MouseEvent�� //
void mouseMove()
{
    // �ƶ���ָ������
    MouseEvent m;
    m.MouseMoveTo(100, 100);
}

// ��갴�£����̧��һ����ʹ��MouseUp()����
void mouseDown()
{
    MouseEvent m;
    m.MouseDown("left");
}

// �������ƶ��������������¸��� //
void mouseRoll()
{
    MouseEvent m;
    m.MouseRoll(-1000);  // ���¹���
}


// ������(Ŀǰ���ڰ��Ʒδ�������) //
void ListenMouseEvent()
{
    MouseEvent m;
    m.ListenMouseEvent();

}

/****** ����KeyBoardEvent���ʹ�÷��� ******/
// ����ĳ��������̧������KeyUp�÷���ͬ��ע�ⰴ����Ҫ�ֶ�����KeyUp��������Ҳ�Ƿ��򲻻�̧�� //
void keyDown()
{
    KeyBoardEvent k;
    k.KeyDown("a");
}

// ���������������̧�𣬲���Ҫ�������ǵ���̧���������°���һֱ���� //
void pressKey()
{
    KeyBoardEvent k;
    k.PressKey("a");
}


// ��ݼ����£����֧��������ݼ� //
void pressHotKey()
{
    KeyBoardEvent k;
    k.PressHotKey("ctrl", "alt", "c");
    k.PressHotKey("ctrl", "v", "NULL");
}

// ģ��������룬��֧��Ӣ�ģ������Ҫ���Ŀ���ʹ��CopyStr�������� //
void writeStr()
{
    KeyBoardEvent k;
    k.WriteStr("Hello World");
}

// �����ַ��� //
void copyStr()
{
    KeyBoardEvent k;
    k.CopyStr("Hello World");
}

class Test
{
public:
    void print()
    {
        cout<<"your press ctrl+d";
    }
};

void print()
{
    cout<<"your press ctrl+d";
    KeyBoardEvent::ClearHotKey();
}

// ע���ݼ�,������������֧�����ⰴ��:ctrl,alt,shift //
void addHotKey()
{
    KeyBoardEvent k;
    Test t;
    // ������ͨ���� //
    k.AddHotKey("esc", print);

    // �������Ա���� //
    k.AddHotKey("ctrl+d", [&t](){t.print(); KeyBoardEvent::ClearHotKey();});
    k.WaitHotKey();
}

int main()
{
    return 0;
}