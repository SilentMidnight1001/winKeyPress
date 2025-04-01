#include <iostream>
#include <windows.h>
#include <cstdio>
#include <ctime>
#include <unistd.h> // For sleep()
#include <csignal> // For signal()
#include <regex>  // ������ͷ�ļ�
#include <vector>  // �Զ������ڴ�
#include <thread>
#include <unistd.h>
#include <functional>
#include <cctype>
#include "winKeyPressH.hpp"
using namespace std;

/*********** �������ձ� Virtual key code mapping table **************/
string alphabet_list[] = {"a", "b", "c", "d", "e", "f", "g",
                          "h", "i", "j", "k", "l", "m",
                          "n", "o", "p", "q", "r", "s",
                          "t", "u", "v", "w", "x", "y", "z"  // 26��ĸ
        ,"0", "1", "2", "3", "4", "5", "6"
        , "7", "8", "9"};  // ������  // ��ĸ�б�


string function_keys_list [] = {"ctrl", "alt", "shift", "F1", "F2", "F3", "F4", "F5",
                                "F6", "F7", "F8", "F9", "F10", "F11", "F12", "esc",
                                "space", "delete", "tab", "enter", "caps", "clear",
                                "backspace", "win", "Pause", "page up", "page down", "left arrow",
                                "right arrow", "down arrow", "up arrow", "insert"};

/****************************������б�********************************/
int alphabet_code[] = {65, 66, 67, 68, 69, 70,
                       71, 72, 73, 74, 75, 76,
                       77, 78, 79, 80, 81, 82,
                       83, 84, 85, 86, 87, 88,
                       89, 90, 48, 49, 50, 51,  // 48 �Ժ�������
                       52, 53, 54, 55, 56, 57};  // �������

int function_code[] = {VK_CONTROL, VK_MENU, VK_SHIFT, VK_F1, VK_F2, VK_F3,
                       VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10,
                       VK_F11, VK_F12, VK_ESCAPE, VK_SPACE, VK_DELETE, VK_TAB,
                       VK_RETURN, VK_CAPITAL, VK_CLEAR, VK_BACK, VK_LWIN, VK_PAUSE,
                       VK_PRIOR, VK_NEXT, VK_LEFT, VK_RIGHT, VK_DOWN, VK_UP, VK_INSERT
};


/**************************** ����������б� ********************************/
string symbol_list_not_shift_press[] = {"`", "[", "]", "\\", ";", "''", ",", ".", "/",
                                        "-", "=", "��", "��", "��", "��"};

string symbol_list_need_shift_press[] = {"~", "{", "}", "|", ":", "\"\"", "<", ">", "?",
                                         "_", "+","!", "@", "#", "$", "%", "^",
                                         "&", "*", "(",")"};

string chinese_symbol_shift_press[] = {"��", "����", "��", "��", "��",
                                       "����", "��" , "��", "����","��","��"};

//������б�//
int symbol_not_shift_code[] = {VK_OEM_3, VK_OEM_4, VK_OEM_6, VK_OEM_5, VK_OEM_1, VK_OEM_7, 188,
                               190, VK_OEM_2, VK_OEM_MINUS, VK_OEM_PLUS, 188, 190, VK_OEM_7,
                               VK_OEM_7};

int symbol_need_shift_code[] = {VK_OEM_3, VK_OEM_4, VK_OEM_6, VK_OEM_5, VK_OEM_1, VK_OEM_7, 188,
                                190, VK_OEM_2, VK_OEM_MINUS, VK_OEM_PLUS, 49,
                                50, 51,52, 53, 54, 55, 56, 57, 48 };

int chinese_symbol_shift_code[] = {VK_OEM_1, VK_OEM_7, 188,190, VK_OEM_2,
                                   VK_OEM_MINUS,  49,52, 53, 57, 48 };


/************** ��Ļ�¼� **************/
ScreenSize GetScreenSIze(bool out_put=True)
{
    ScreenSize screen_size;
    int x_len, y_len;
    x_len=GetSystemMetrics(SM_CXSCREEN);  // ��ȡx����
    y_len=GetSystemMetrics(SM_CYSCREEN);  // ��ȡy����
    screen_size.x = x_len;  // �ṹ�崢���С(��Ļ)
    screen_size.y = y_len;  // �ṹ�崢���С(��Ļ)
    if (out_put)
    {
        printf("Display size x:%d, y:%d\n", screen_size.x, screen_size.y);
    }
    return screen_size;
//    If you want to get coordinates you need to instantiate the struct function:
//    ScreenSizeGet screen_size = GetScreenSIze; screen_size.x, screen_size.y
}


/************************��ȡ�������*******************************/
MousePosition GetMousePosition(bool out_put=True)  // ��ȡ���Ŀǰ���ڵ�����
{
    POINT cursorPos;  // ʵ�������ṹ�壬��ȡλ��
    MousePosition mouse_position;  // ʵ���ṹ��ȡ���б���
    if (GetCursorPos(&cursorPos))
    {
        if (out_put)
        {
            printf("The current position of the cursor is x:%ld, y:%ld\n", cursorPos.x, cursorPos.y);
        }
        mouse_position.x = cursorPos.x;
        mouse_position.y = cursorPos.y;
        return mouse_position;  // �������ʵ���������꣬�����Ҫ�õ�������Ҫʵ����
        // ��ȡ������Ҫʵ�����ṹ����ͽ�������ֵ�����ڽṹ��ʵ���ı����е������������ṹ���ٻ�ȡ
//        If you want to get coordinates you need to instantiate the struct function:
//        MousePositionGet mouse_position = GetMousePosition(); mouse_position.x,mouse_position.y
    }
    else
    {
        printf("Sorry, cursor coordinates not found\n");
        return mouse_position;
    }

}

/***************** ����¼����� *******************/

void MouseEvent::MouseMoveTo(int x, int y)  // MouseMoveTo��void����
{
    SetCursorPos(x, y);  // �ƶ���굽ĳ��(���ù��λ��)
}

void MouseEvent::MouseClick(int x, int y, int clicks=1, const string& button="left")
{
    /* ��clickΪ1ʱ����button��ΪNoneʱִ�е���һ�ε�ָ�� */
    SetCursorPos(x, y);  // �ƶ���굽ĳ��(���ù��λ��)
    if (clicks!=0)
    {
        int click_time = 0;  // ��ʼ���������
        while (click_time <clicks && button== "left")  // ����ʼ�������С��Ŀ����������
        {
            // ���û������Ĭ�����
            INPUT input = {0};  // ����INPUT�ṹ�������ʾ�����¼���Ϣ
            input.type = INPUT_MOUSE;  // ָ�������¼�����Ϊ����¼�
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;  // �趨����¼�Ϊ�������
            SendInput(1, &input, sizeof(INPUT));  // ʹ��SendInput������������¼�

            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;  // �趨����¼�Ϊ����ͷ�
            SendInput(1, &input, sizeof(INPUT));  // �����ͷ��������¼�
            click_time++;
        }

        while (click_time <clicks && button == "right")  // ����ʼ�������С��Ŀ������������
        {
            INPUT input = {0};  // ����INPUT�ṹ�������ʾ�����¼���Ϣ
            input.type = INPUT_MOUSE;  // ָ�������¼�����Ϊ����¼�
            input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;  // �趨����¼�Ϊ�������
            SendInput(1, &input, sizeof(INPUT));  // ʹ��SendInput������������¼�


            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;  // �趨����¼�Ϊ�Ҽ��ͷ�
            SendInput(1, &input, sizeof(INPUT));  // �����ͷ�����Ҽ��¼�
            click_time++;  // ��ʼֵ����
        }
    }
}

void MouseEvent::MouseDown(const string& button="left")
{
    if (button == "left")
    {
        // ���û������Ĭ�����
        INPUT input = {0};  // ����INPUT�ṹ�������ʾ�����¼���Ϣ
        input.type = INPUT_MOUSE;  // ָ�������¼�����Ϊ����¼�
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;  // �趨����¼�Ϊ�������
        SendInput(1, &input, sizeof(INPUT));  // ʹ��SendInput������������¼�
        // ������걻����
        mouse_flag.mouse_left_down = True;  // ��־Ϊtrue
    }
    else if (button == "right")
    {
        // ���û������Ĭ�����
        INPUT input = {0};  // ����INPUT�ṹ�������ʾ�����¼���Ϣ
        input.type = INPUT_MOUSE;  // ָ�������¼�����Ϊ����¼�
        input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;  // �趨����¼�Ϊ�������
        SendInput(1, &input, sizeof(INPUT));  // ʹ��SendInput������������¼�
        // ������걻����
        mouse_flag.mouse_right_down = True;
    }
}

void MouseEvent::MouseUp(const string& button="left")
{
    if  (button == "left")
    {
        INPUT input = {0};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP; // �����������������Ҫ����
        SendInput(1, &input, sizeof(INPUT));
        // �ɿ���־ΪFalse
        mouse_flag.mouse_left_down = False;
    }
    else if (button == "right")
    {
        INPUT input = {0};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_RIGHTUP; // �����������������Ҫ����
        SendInput(1, &input, sizeof(INPUT));
        // �ɿ���־ΪFalse
        mouse_flag.mouse_right_down = False;
    }
}

void MouseEvent::MouseRoll(int move)
{
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dx = 0;       // x����
    input.mi.dy = 0;       // y����
    input.mi.mouseData = move; // ��������;���
    input.mi.dwFlags = MOUSEEVENTF_WHEEL; // ���͹����¼�
    input.mi.time = 0;     // ��ǰϵͳʱ��
    input.mi.dwExtraInfo = 0; // ������Ϣ

    SendInput(1, &input, sizeof(INPUT));
}

//              ����¼�����             //

// ����ȫ�ֱ��������ڴ洢��깳�Ӿ��
HHOOK hMouseHook = NULL;

// ���ӻص����������ڴ�������¼� //
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    // ��ȡ����¼�����ϸ��Ϣ
    MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;

    // ������Ӵ�����Ч��nCode >= 0������������¼�
    if (nCode >= 0)
    {
        // ����wParam�ж�����¼�����
        switch (wParam)
        {
            case WM_LBUTTONDOWN: // �������
                std::cout << "Mouse left button down at (" << pMouseStruct->pt.x << ", " << pMouseStruct->pt.y << ")" << std::endl;

                // �������ʱ��������������� True��̧����Ϊ False //
                mouseEventFlag.MouseLeftDown = True;
                mouseEventFlag.MouseLeftUp = False;
                cout<<"Press!\n";
                break;
            case WM_LBUTTONUP: // ����ͷ�
                std::cout << "Mouse left button up at (" << pMouseStruct->pt.x << ", " << pMouseStruct->pt.y << ")" << std::endl;

                // ����ͷ�ʱ�������������Ϊ False��̧����Ϊ True //
                mouseEventFlag.MouseLeftDown = False;
                mouseEventFlag.MouseLeftUp = True;
                break;
            case WM_RBUTTONDOWN: // �Ҽ�����
//                std::cout << "Mouse right button down at (" << pMouseStruct->pt.x << ", " << pMouseStruct->pt.y << ")" << std::endl;

                // �Ҽ�����ʱ�����Ҽ��������� True��̧����Ϊ False //
                mouseEventFlag.MouseRightDown = True;
                mouseEventFlag.MouseLeftUp = False;
                break;
            case WM_RBUTTONUP: // �Ҽ��ͷ�
//                std::cout << "Mouse right button up at (" << pMouseStruct->pt.x << ", " << pMouseStruct->pt.y << ")" << std::endl;

                // �Ҽ��ͷ�ʱ�����Ҽ��������� False��̧����Ϊ True //
                mouseEventFlag.MouseRightDown = False;
                mouseEventFlag.MouseLeftUp = True;

            break;
            case WM_MBUTTONDOWN: // �м�����
//                std::cout << "Mouse middle button down at (" << pMouseStruct->pt.x << ", " << pMouseStruct->pt.y << ")" << std::endl;

                // �м�����ʱ�����м��������� True��̧����Ϊ False //
                mouseEventFlag.MouseRollDown = True;
                mouseEventFlag.MouseLeftUp = False;
                break;
            case WM_MBUTTONUP: // �м��ͷ�
//                std::cout << "Mouse middle button up at (" << pMouseStruct->pt.x << ", " << pMouseStruct->pt.y << ")" << std::endl;

                // �м��ͷ�ʱ�����м��������� False��̧����Ϊ True //
                mouseEventFlag.MouseRollDown = False;
                mouseEventFlag.MouseLeftUp = True;
                break;
        }
    }

    // ������һ�����ӣ�ȷ����������Ҳ��������������¼�
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// �˳�����������������Դ���˳�����
void MouseEvent:: ExitMouseEvent()
{
    // ���ȫ�ֱ���hMouseHook�Ƿ���Ч
    if (hMouseHook != NULL)
    {
        // �Ƴ���깳��
        UnhookWindowsHookEx(hMouseHook);
        hMouseHook = NULL; // ��ȫ�ֱ�����Ϊ��
    }

    // ���ͳ��������Ϣ //
    PostQuitMessage(0);

//    exit(0); // �˳�����
}

// ������꺯�� //
void MouseEvent::ListenMouseEvent()
{
    // ��װ��깳�ӣ���������洢��ȫ�ֱ�����
    hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, GetModuleHandle(NULL), 0);

    // ��ʼ������¼���־ //
    mouseEventFlag.MouseLeftDown = False;
    mouseEventFlag.MouseRightDown = False;

    mouseEventFlag.MouseLeftUp = False;
    mouseEventFlag.MouseRightUp = False;

    mouseEventFlag.MouseRollDown = False;
    mouseEventFlag.MouseRollUp = False;

    // ��鰲װ�����Ƿ�ɹ� //
    if (hMouseHook == NULL)
    {
        std::cerr << "Failed to install mouse hook." << std::endl;
        return ; // �����װ����ʧ�ܣ��˳�����
    }

    // �� PostQuitMessage(0) ������ʱ��GetMessage�᷵�� 0, �Ӷ��˳�ѭ���� //
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) // ʹ��GetMessage����ֵ�ж��Ƿ��յ��˳���Ϣ
    {
        TranslateMessage(&msg); // ������Ϣ
        DispatchMessage(&msg); // �ɷ���Ϣ
    }
}


/************************* �����¼� ****************************/
// ���̰��¼�¼������ڲ�ѯ�Ƿ��ͷ� //
void key_down_record(int key_code)  // ��¼���̰��� (�����ظ�)
{
    int len = sizeof(free_keys.key_code) / sizeof(free_keys.key_code[0]);  // ��鰴�������С
    bool found = false;  // ����Ƿ��ҵ�������

    // �������飬��鰴�����Ƿ��Ѿ�����
    for (int i = 0; i < len; i++)
    {
        if (free_keys.key_code[i] == key_code)
        {
            found = true;  // �ҵ������룬���ñ��Ϊtrue
            break;
        }
    }

    // ���û���ҵ������룬��ӵ�����ĵ�һ����λ��
    if (!found)  // ���û�ҵ� �����±������� Ѱ��ֵΪ0��λ�� ������븳ֵ������
    {
        for (int i = 0; i < len; i++)
        {
            if (free_keys.key_code[i] == 0)  // �ҵ�����ĵ�һ����λ�� (�ҵ�����Ϊ0��λ�ã�������븳ֵ������)
            {
                free_keys.key_code[i] = key_code;
                break;
            }
        }
    }
}

// ��¼�����ɿ�����հ��� //
void key_up_record(int key_code)
{
    int len = sizeof(free_keys.key_code) / sizeof(free_keys.key_code[0]);  // ����ͷż��̵������С
    for (int i = 0; i < len; i++)
    {
        if (free_keys.key_code[i] == key_code)
        {
            free_keys.key_code[i] = 0;  // ��հ�����
            break;
        }
    }
}

// ���̰��� //
void KeyBoardEvent::KeyDown(const string& key)
{
    /****************�б���************************/
    int alphabet_list_len = sizeof (alphabet_list)/sizeof (alphabet_list[0]);  // ��ĸ���ֳ���
    int function_keys_list_len = sizeof (function_keys_list)/sizeof (function_keys_list[0]);  // ���ܼ����鳤��


    for (int i = 0; i<alphabet_list_len; i++)
    {
        // ���������ַ��Ƿ��봫��Ĳ���һ��(�ж���ĸ) �����û������һ��ѭ��
        if (key == alphabet_list[i])
        {
            INPUT input; // ����һ�� INPUT �ṹ����� input���������������¼�

            input.type = INPUT_KEYBOARD; // ָ�� input ������Ϊ��������
            input.ki.wScan = 0; // ɨ������Ϊ 0��ͨ������Ҫʹ��
            input.ki.time = 0; // ʱ�����Ϊ 0
            input.ki.dwExtraInfo = 0; // ������Ϣ��Ϊ 0

            // ģ�ⰴ��Ŀ���
            input.ki.wVk = alphabet_code[i]; // ָ��ģ�ⰴ�µļ�Ϊ Ŀ�� ��
            input.ki.dwFlags = 0; // ָ�����̰����¼���dwFlags Ϊ 0
            SendInput(1, &input, sizeof(INPUT)); // ���Ͱ����¼���ϵͳ

            key_down_record(alphabet_code[i]);
            break;
        }
    }

    for (int i=0; i<function_keys_list_len; i++)
    {
        // �Ƿ��봫��Ĳ���һ�� (�жϹ��ܼ�)
        if (key == function_keys_list[i])
        {
            INPUT input; // ����һ�� INPUT �ṹ����� input���������������¼�

            input.type = INPUT_KEYBOARD; // ָ�� input ������Ϊ��������
            input.ki.wScan = 0; // ɨ������Ϊ 0��ͨ������Ҫʹ��
            input.ki.time = 0; // ʱ�����Ϊ 0
            input.ki.dwExtraInfo = 0; // ������Ϣ��Ϊ 0

            // ģ�ⰴ��Ŀ���
            input.ki.wVk = function_code[i]; // ָ��ģ�ⰴ�µļ�Ϊ Ŀ�� ��
            input.ki.dwFlags = 0; // ָ�����̰����¼���dwFlags Ϊ 0
            SendInput(1, &input, sizeof(INPUT)); // ���Ͱ����¼���ϵͳ

            key_down_record(function_code[i]);
            break;
        }
    }
}

// �����ͷ� //
void KeyBoardEvent::KeyUp(const string& key)
{
    int len = sizeof (free_keys.key_code)/sizeof (free_keys.key_code[0]);
/****************�б���************************/
    int alphabet_list_len = sizeof (alphabet_list)/sizeof (alphabet_list[0]);  // ��ĸ���ֳ���
    int function_keys_list_len = sizeof (function_keys_list)/sizeof (function_keys_list[0]);  // ���ܼ����鳤��

    for (int i = 0; i<alphabet_list_len; i++) {
        // ���������ַ��Ƿ��봫��Ĳ���һ��(�ж���ĸ) �����û������һ��ѭ��
        if (key == alphabet_list[i]) {
            INPUT input = {0};
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = alphabet_code[i];  // ��λ����
            // ���֮ǰ�����������ˣ�������Ҫ�ͷ�
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &input, sizeof(INPUT));

            key_up_record(alphabet_code[i]);
            break;
        }
    }


    for (int i=0; i<function_keys_list_len; i++)
    {
        // �Ƿ��봫��Ĳ���һ�� (�жϹ��ܼ�)
        if (key == function_keys_list[i])
        {
            INPUT input = {0};
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = function_code[i];  // ��λ����
            // ���֮ǰ�����������ˣ�������Ҫ�ͷ�
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &input, sizeof(INPUT));

            key_up_record(function_code[i]);
            break;
        }
    }

//    INPUT input = {0};
//    input.type = INPUT_KEYBOARD;
//    input.ki.wVk = vkCode;  // ��λ����
//    // ���֮ǰ�����������ˣ�������Ҫ�ͷ�
//    input.ki.dwFlags = KEYEVENTF_KEYUP;
//    SendInput(1, &input, sizeof(INPUT));
}

// ���̵�� //
void KeyBoardEvent::PressKey(const string& key)
{
/****************�б���************************/
    int alphabet_list_len = sizeof (alphabet_list)/sizeof (alphabet_list[0]);  // ��ĸ���ֳ���
    int symbol_not_shift_len = sizeof (symbol_list_not_shift_press)/sizeof(symbol_list_not_shift_press[0]); // �ַ�����
    int symbol_need_shift_len = sizeof (symbol_list_need_shift_press)/sizeof (symbol_list_need_shift_press[0]); // �ַ����鳤��
    int function_keys_list_len = sizeof (function_keys_list)/sizeof (function_keys_list[0]);  // ���ܼ����鳤��
    int chinese_symbol_shift_press_len = sizeof (chinese_symbol_shift_press)/sizeof (chinese_symbol_shift_press[0]);  // �����ַ������б���


    for (int i = 0; i<alphabet_list_len; i++)
    {
        // ���������ַ��Ƿ��봫��Ĳ���һ��(�ж���ĸ) �����û������һ��ѭ��
        if (key == alphabet_list[i])
        {
            INPUT input; // ����һ�� INPUT �ṹ����� input���������������¼�

            input.type = INPUT_KEYBOARD; // ָ�� input ������Ϊ��������
            input.ki.wScan = 0; // ɨ������Ϊ 0��ͨ������Ҫʹ��
            input.ki.time = 0; // ʱ�����Ϊ 0
            input.ki.dwExtraInfo = 0; // ������Ϣ��Ϊ 0

            // ģ�ⰴ��Ŀ���
            input.ki.wVk = alphabet_code[i]; // ָ��ģ�ⰴ�µļ�Ϊ Ŀ�� ��
            input.ki.dwFlags = 0; // ָ�����̰����¼���dwFlags Ϊ 0
            SendInput(1, &input, sizeof(INPUT)); // ���Ͱ����¼���ϵͳ

            // ģ���ͷ�Ŀ���
            input.ki.wVk = alphabet_code[i]; // ָ��ģ���ͷŵļ���Ȼ�� Ŀ�� ��
            input.ki.dwFlags = KEYEVENTF_KEYUP; // ָ�������ͷ��¼���dwFlags ����Ϊ KEYEVENTF_KEYUP
            SendInput(1, &input, sizeof(INPUT)); // �ٴη��Ͱ����¼���ϵͳ
            break;
        }
    }

    for (int i=0; i<function_keys_list_len; i++)
    {
        // �Ƿ��봫��Ĳ���һ�� (�жϹ��ܼ�)
        if (key == function_keys_list[i])
        {
            INPUT input; // ����һ�� INPUT �ṹ����� input���������������¼�

            input.type = INPUT_KEYBOARD; // ָ�� input ������Ϊ��������
            input.ki.wScan = 0; // ɨ������Ϊ 0��ͨ������Ҫʹ��
            input.ki.time = 0; // ʱ�����Ϊ 0
            input.ki.dwExtraInfo = 0; // ������Ϣ��Ϊ 0

            // ģ�ⰴ��Ŀ���
            input.ki.wVk = function_code[i]; // ָ��ģ�ⰴ�µļ�Ϊ Ŀ�� ��
            input.ki.dwFlags = 0; // ָ�����̰����¼���dwFlags Ϊ 0
            SendInput(1, &input, sizeof(INPUT)); // ���Ͱ����¼���ϵͳ

            input.ki.wVk = function_code[i]; // ָ��ģ���ͷŵļ���Ȼ�� Ŀ�� ��
            input.ki.dwFlags = KEYEVENTF_KEYUP; // ָ�������ͷ��¼���dwFlags ����Ϊ KEYEVENTF_KEYUP
            SendInput(1, &input, sizeof(INPUT)); // �ٴη��Ͱ����¼���ϵͳ
            break;
        }
    }

    for (int i=0; i< symbol_not_shift_len; i++)
    {
        // �Ƿ��봫��Ĳ���һ�� (�ж����谴shift�����ķ���)
        if (key == symbol_list_not_shift_press[i])
        {
            INPUT input; // ����һ�� INPUT �ṹ����� input���������������¼�

            input.type = INPUT_KEYBOARD; // ָ�� input ������Ϊ��������
            input.ki.wScan = 0; // ɨ������Ϊ 0��ͨ������Ҫʹ��
            input.ki.time = 0; // ʱ�����Ϊ 0
            input.ki.dwExtraInfo = 0; // ������Ϣ��Ϊ 0

            // ģ�ⰴ��Ŀ���
            input.ki.wVk = symbol_not_shift_code[i]; // ָ��ģ�ⰴ�µļ�Ϊ Ŀ�� ��
            input.ki.dwFlags = 0; // ָ�����̰����¼���dwFlags Ϊ 0
            SendInput(1, &input, sizeof(INPUT)); // ���Ͱ����¼���ϵͳ

            input.ki.wVk = symbol_not_shift_code[i]; // ָ��ģ���ͷŵļ���Ȼ�� Ŀ�� ��
            input.ki.dwFlags = KEYEVENTF_KEYUP; // ָ�������ͷ��¼���dwFlags ����Ϊ KEYEVENTF_KEYUP
            SendInput(1, &input, sizeof(INPUT)); // �ٴη��Ͱ����¼���ϵͳ
            break;
        }
    }

    for (int i=0; i<symbol_need_shift_len; i++)
    {
        // ��Ҫ��shift�ķ���
        if (key == symbol_list_need_shift_press[i])
        {
            // ������������ṹ�岢��ʼ��
            INPUT inputs[2] = {0};

            // ��һ������ 'shift' ������ṹ��
            inputs[0].type = INPUT_KEYBOARD;
            inputs[0].ki.wVk = VK_SHIFT; // 'shift'�����������
            inputs[0].ki.dwFlags = 0; // ��ʾ���°���

            // �ڶ������� 'Ŀ�����' ������ṹ��
            inputs[1].type = INPUT_KEYBOARD;
            inputs[1].ki.wVk = symbol_need_shift_code[i]; // 'Ŀ�����'�����������
            inputs[1].ki.dwFlags = 0; // ��ʾ���°���
            SendInput(2, inputs, sizeof (INPUT));  // ����ָ���shift���������

            inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
            inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
            SendInput(2, inputs, sizeof(INPUT));  // ͬʱ�ͷ�
            break;
        }
    }

    for (int i=0; i<chinese_symbol_shift_press_len; i++)
    {
        // �����ַ��ж�
        if (key == chinese_symbol_shift_press[i])
        {
            // ������������ṹ�岢��ʼ��
            INPUT inputs[2] = {0};

            // ��һ������ 'shift' ������ṹ��
            inputs[0].type = INPUT_KEYBOARD;
            inputs[0].ki.wVk = VK_SHIFT; // 'shift'�����������
            inputs[0].ki.dwFlags = 0; // ��ʾ���°���

            // �ڶ������� 'Ŀ�����' ������ṹ��
            inputs[1].type = INPUT_KEYBOARD;
            inputs[1].ki.wVk = chinese_symbol_shift_code[i]; // 'Ŀ�����'�����������
            inputs[1].ki.dwFlags = 0; // ��ʾ���°���
            SendInput(2, inputs, sizeof (INPUT));  // ����ָ���shift���������

            inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
            inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
            SendInput(2, inputs, sizeof(INPUT));  // ͬʱ�ͷ�
            break;
        }
    }
}

void KeyBoardEvent::PressHotKey(const string& key1="NULL", const string& key2="MULL", const string& key3="NULL")
{
    // �����ʹ��������ݼ�����None
    // �����ǲ鿴�������飬������������������ƥ������������ð��������Լ�������


    string button_list[] = {"a", "b", "c", "d", "e", "f", "g",
                            "h", "i", "j", "k", "l", "m",
                            "n", "o", "p", "q", "r", "s",
                            "t", "u", "v", "w", "x", "y", "z"  // 26��ĸ
            ,"0", "1", "2", "3", "4", "5", "6"
            , "7", "8", "9"};  // ������


    int button_code_list[] = {65, 66, 67, 68, 69, 70,
                              71, 72, 73, 74, 75, 76,
                              77, 78, 79, 80, 81, 82,
                              83, 84, 85, 86, 87, 88,
                              89, 90, 48, 49, 50, 51,
                              52, 53, 54, 55, 56, 57};  // �������

    int len = sizeof (button_list)/sizeof (button_list[0]);  // �����С


    if (key1 != "null" && key2 != "null")  // ��������ַ����Ƿ�ΪNone
    {
        if (key1 == "ctrl") // �����ݼ���ctrl��ͷ
        {
            for (int i=0; i <len; i++)  // ƥ�䰴���б����봫�������ͬ�İ�������ȡ����������
            {
                // ����ڶ�������������
                if (button_list[i] == key2)
                {
                    // ����һ������ṹ�岢��ʼ��
                    INPUT inputs[2] = {0};  // [2]��ʾ����Ԫ��ִ���������������{0}��ʼ������
                    // ��һ������ 'ctrl' ������ṹ��
                    inputs[0].type = INPUT_KEYBOARD;
                    inputs[0].ki.wVk = VK_CONTROL; // 'ctrl'�����������
                    inputs[0].ki.dwFlags = 0; // ��ʾ���°���

                    // �ڶ������� '������ַ�' ������ṹ��
                    inputs[1].type = INPUT_KEYBOARD;
                    inputs[1].ki.wVk = button_code_list[i]; // '��������'�����������
                    inputs[1].ki.dwFlags = 0; // ��ʾ���°���
                    // �������룬ͬʱ����'ctrl'��'ָ������'��
                    SendInput(2, inputs, sizeof(INPUT));


                    // ģ�ⰴ���ͷ�
                    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                    SendInput(2, inputs, sizeof(INPUT));
                    break;  // break��ֹѭ���ж�ε����ݼ�
                }

                    // ����ڶ�������alt���� ��ctrl��ͷ
                else if (key2 == "alt")
                {
                    if (key3!="NULL" && button_list[i] == key3)
                    {
                        // ����һ������ṹ�岢��ʼ��
                        INPUT inputs[3] = {0};  // [2]��ʾ����Ԫ��ִ���������������{0}��ʼ������
                        inputs[0].type = INPUT_KEYBOARD;
                        inputs[0].ki.wVk = VK_CONTROL; // 'ctrl'�����������
                        inputs[0].ki.dwFlags = 0; // ��ʾ���°���

                        inputs[1].type = INPUT_KEYBOARD;
                        inputs[1].ki.wVk = VK_MENU; // 'alt'�����������
                        inputs[1].ki.dwFlags = 0; // ��ʾ���°���

                        // �ڶ������� '������ַ�' ������ṹ��
                        inputs[2].type = INPUT_KEYBOARD;
                        inputs[2].ki.wVk = button_code_list[i]; // '��������'�����������
                        inputs[2].ki.dwFlags = 0; // ��ʾ���°���
                        // �������룬ͬʱ����'ctrl��"alt',��'ָ������'��
                        SendInput(3, inputs, sizeof(INPUT));  // ����

                        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        SendInput(3, inputs, sizeof(INPUT));
                        break;
                    }

                }

                    // ����ڶ�������Ϊshift�� ����ctrl��ͷ
                else if (key2 == "shift")
                {
                    if (key3 !="NULL" && button_list[i] == key3)
                    {
                        // ����һ������ṹ�岢��ʼ��
                        INPUT inputs[3] = {0};  // [2]��ʾ����Ԫ��ִ���������������{0}��ʼ������
                        inputs[0].type = INPUT_KEYBOARD;
                        inputs[0].ki.wVk = VK_CONTROL; // 'ctrl'�����������
                        inputs[0].ki.dwFlags = 0; // ��ʾ���°���

                        inputs[1].type = INPUT_KEYBOARD;
                        inputs[1].ki.wVk = VK_SHIFT; // 'shift'�����������
                        inputs[1].ki.dwFlags = 0; // ��ʾ���°���

                        // �ڶ������� '������ַ�' ������ṹ��
                        inputs[2].type = INPUT_KEYBOARD;
                        inputs[2].ki.wVk = button_code_list[i]; // '��������'�����������
                        inputs[2].ki.dwFlags = 0; // ��ʾ���°���
                        // �������룬ͬʱ����'ctrl��"shift',��'ָ������'��
                        SendInput(3, inputs, sizeof(INPUT));  // ����

                        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        SendInput(3, inputs, sizeof(INPUT));
                        break;
                    }

                }
            }
        }


            // �ж�alt��ͷ
        else if (key1 == "alt") // �����ݼ���alt��ͷ
        {
            for (int i=0; i <len; i++)  // ƥ�䰴���б����봫�������ͬ�İ�������ȡ����������
            {
                if (button_list[i] == key2)
                {
                    // ����һ������ṹ�岢��ʼ��
                    INPUT inputs[2] = {0};  // [2]��ʾ����Ԫ��ִ���������������{0}��ʼ������
                    // ��һ������ 'alt' ������ṹ��
                    inputs[0].type = INPUT_KEYBOARD;
                    inputs[0].ki.wVk = VK_MENU; // 'alt'�����������
                    inputs[0].ki.dwFlags = 0; // ��ʾ���°���

                    // �ڶ������� '������ַ�' ������ṹ��
                    inputs[1].type = INPUT_KEYBOARD;
                    inputs[1].ki.wVk = button_code_list[i]; // '�������'�����������
                    inputs[1].ki.dwFlags = 0; // ��ʾ���°���
                    // �������룬ͬʱ����'ctrl'��'ָ������'��
                    SendInput(2, inputs, sizeof(INPUT));


                    // ģ�ⰴ���ͷ�
                    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                    SendInput(2, inputs, sizeof(INPUT));

                    break;
                }

                    // ����ڶ�������Ϊctrl���� ��alt��ͷ
                else if(key2 == "ctrl")
                {
                    if (key3 !="NULL" && button_list[i] == key3)
                    {
                        // ����һ������ṹ�岢��ʼ��
                        INPUT inputs[3] = {0};  // [2]��ʾ����Ԫ��ִ���������������{0}��ʼ������
                        inputs[0].type = INPUT_KEYBOARD;
                        inputs[0].ki.wVk = VK_CONTROL; // 'ctrl'�����������
                        inputs[0].ki.dwFlags = 0; // ��ʾ���°���

                        inputs[1].type = INPUT_KEYBOARD;
                        inputs[1].ki.wVk = VK_MENU; // 'alt'�����������
                        inputs[1].ki.dwFlags = 0; // ��ʾ���°���

                        // �ڶ������� '������ַ�' ������ṹ��
                        inputs[2].type = INPUT_KEYBOARD;
                        inputs[2].ki.wVk = button_code_list[i]; // '��������'�����������
                        inputs[2].ki.dwFlags = 0; // ��ʾ���°���
                        // �������룬ͬʱ����'ctrl��"alt',��'ָ������'��
                        SendInput(3, inputs, sizeof(INPUT));  // ����

                        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        SendInput(3, inputs, sizeof(INPUT));
                        break;
                    }

                }

                    // ����ڶ���������shift,,��alt��ͷ
                else if (key2 == "shift")
                {
                    if (key3 !="NULL" && button_list[i] == key3)
                    {
                        // ����һ������ṹ�岢��ʼ��
                        INPUT inputs[3] = {0};  // [2]��ʾ����Ԫ��ִ���������������{0}��ʼ������
                        inputs[0].type = INPUT_KEYBOARD;
                        inputs[0].ki.wVk = VK_MENU; // 'alt'�����������
                        inputs[0].ki.dwFlags = 0; // ��ʾ���°���

                        inputs[1].type = INPUT_KEYBOARD;
                        inputs[1].ki.wVk = VK_SHIFT; // 'shift'�����������
                        inputs[1].ki.dwFlags = 0; // ��ʾ���°���

                        // �ڶ������� '������ַ�' ������ṹ��
                        inputs[2].type = INPUT_KEYBOARD;
                        inputs[2].ki.wVk = button_code_list[i]; // '��������'�����������
                        inputs[2].ki.dwFlags = 0; // ��ʾ���°���
                        // �������룬ͬʱ����'alt'��"shift',��'ָ������'��
                        SendInput(3, inputs, sizeof(INPUT));  // ����

                        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        SendInput(3, inputs, sizeof(INPUT));
                        break;
                    }
                }
            }
        }


            // �ж�shift��ͷ
        else if (key1 == "shift") // �����ݼ���shift��ͷ
        {
            for (int i=0; i <len; i++)  // ƥ�䰴���б����봫�������ͬ�İ�������ȡ����������
            {
                if (button_list[i] == key2)
                {
                    // ����һ������ṹ�岢��ʼ��
                    INPUT inputs[2] = {0};  // [2]��ʾ����Ԫ��ִ���������������{0}��ʼ������
                    // ��һ������ 'shift' ������ṹ��
                    inputs[0].type = INPUT_KEYBOARD;
                    inputs[0].ki.wVk = VK_SHIFT; // 'shift'�����������
                    inputs[0].ki.dwFlags = 0; // ��ʾ���°���

                    // �ڶ������� '������ַ�' ������ṹ��
                    inputs[1].type = INPUT_KEYBOARD;
                    inputs[1].ki.wVk = button_code_list[i]; // 'B'�����������
                    inputs[1].ki.dwFlags = 0; // ��ʾ���°���

                    // �������룬ͬʱ����'shift'��'ָ������'��
                    SendInput(2, inputs, sizeof(INPUT));

                    // ģ�ⰴ���ͷ�
                    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                    SendInput(2, inputs, sizeof(INPUT));
                    break;
                }

                    // ����ڶ�������Ϊctrl.������shift��ͷ
                else if (key2 == "ctrl")
                {
                    if (key3 !="NULL" && button_list[i] == key3)
                    {
                        // ����һ������ṹ�岢��ʼ��
                        INPUT inputs[3] = {0};  // [2]��ʾ����Ԫ��ִ���������������{0}��ʼ������
                        inputs[0].type = INPUT_KEYBOARD;
                        inputs[0].ki.wVk = VK_CONTROL; // 'ctrl'�����������
                        inputs[0].ki.dwFlags = 0; // ��ʾ���°���

                        inputs[1].type = INPUT_KEYBOARD;
                        inputs[1].ki.wVk = VK_SHIFT; // 'shift'�����������
                        inputs[1].ki.dwFlags = 0; // ��ʾ���°���

                        // �ڶ������� '������ַ�' ������ṹ��
                        inputs[2].type = INPUT_KEYBOARD;
                        inputs[2].ki.wVk = button_code_list[i]; // '��������'�����������
                        inputs[2].ki.dwFlags = 0; // ��ʾ���°���
                        // �������룬ͬʱ����'ctrl��"alt',��'ָ������'��
                        SendInput(3, inputs, sizeof(INPUT));  // ����
                        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        SendInput(3, inputs, sizeof(INPUT));
                        break;
                    }
                }

                    // �жϵڶ��������Ƿ���alt,,,��shift��ͷ
                else if(key2 == "alt")
                {
                    if (key3 !="NULL" && button_list[i] == key3)
                    {
                        // ����һ������ṹ�岢��ʼ��
                        INPUT inputs[3] = {0};  // [2]��ʾ����Ԫ��ִ���������������{0}��ʼ������
                        inputs[0].type = INPUT_KEYBOARD;
                        inputs[0].ki.wVk = VK_MENU; // 'alt'�����������
                        inputs[0].ki.dwFlags = 0; // ��ʾ���°���

                        inputs[1].type = INPUT_KEYBOARD;
                        inputs[1].ki.wVk = VK_SHIFT; // 'shift'�����������
                        inputs[1].ki.dwFlags = 0; // ��ʾ���°���

                        // �ڶ������� '������ַ�' ������ṹ��
                        inputs[2].type = INPUT_KEYBOARD;
                        inputs[2].ki.wVk = button_code_list[i]; // '��������'�����������
                        inputs[2].ki.dwFlags = 0; // ��ʾ���°���
                        // �������룬ͬʱ����'alt'��"shift',��'ָ������'��
                        SendInput(3, inputs, sizeof(INPUT));  // ����

                        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                        SendInput(3, inputs, sizeof(INPUT));
                        break;
                    }
                }
            }
        }

            // �����win��
        else if(key1 == "win")
        {
            for (int i=0; i<len; i++)
            {
                // ����ڶ�������������
                if (button_list[i] == key2)
                {
                    // ����һ������ṹ�岢��ʼ��
                    INPUT inputs[2] = {0};  // [2]��ʾ����Ԫ��ִ���������������{0}��ʼ������
                    // ��һ������ 'win' ������ṹ��
                    inputs[0].type = INPUT_KEYBOARD;
                    inputs[0].ki.wVk = VK_LWIN; // 'win'�����������
                    inputs[0].ki.dwFlags = 0; // ��ʾ���°���

                    // �ڶ������� '������ַ�' ������ṹ��
                    inputs[1].type = INPUT_KEYBOARD;
                    inputs[1].ki.wVk = button_code_list[i]; // '��������'�����������
                    inputs[1].ki.dwFlags = 0; // ��ʾ���°���

                    // �������룬ͬʱ����'win'��'ָ������'��
                    SendInput(2, inputs, sizeof(INPUT));
                    // ģ�ⰴ���ͷ�
                    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                    SendInput(2, inputs, sizeof(INPUT));
                    break;
                }
            }
        }
    }
}


/**
 *                    _ooOoo_
 *                   o8888888o
 *                   88" . "88
 *                   (| -_- |)
 *                    O\ = /O
 *                ____/`---'\____
 *              .   ' \\| |// `.
 *               / \\||| : |||// \
 *             / _||||| -:- |||||- \
 *               | | \\\ - /// | |
 *             | \_| ''\---/'' | |
 *              \ .-\__ `-` ___/-. /
 *           ___`. .' /--.--\ `. . __
 *        ."" '< `.___\_<|>_/___.' >'"".
 *       | | : `- \`.;`\ _ /`;.`/ - ` : | |
 *         \ \ `-. \_ __\ /__ _/ .-` / /
 * ======`-.____`-.___\_____/___.-`____.-'======
 *                    `=---='
 *
 * .............................................
 *          ���汣��             ����BUG
 **/

// ģ���������� //
void KeyBoardEvent::WriteStr(string output)
{
    // ����Ƿ�û�а���shift�� //
    bool shift_up = false;
    /****************�б���************************/
    int alphabet_list_len = sizeof (alphabet_list)/sizeof (alphabet_list[0]);  // ��ĸ���ֳ���
    int symbol_not_shift_len = sizeof (symbol_list_not_shift_press)/sizeof(symbol_list_not_shift_press[0]); // �ַ�����
    int symbol_need_shift_len = sizeof (symbol_list_need_shift_press)/sizeof (symbol_list_need_shift_press[0]); // �ַ����鳤��

    /****************************** ִ�в��� ************************************/
    // ѭ�����������ַ����е�ÿһ���ַ�
    for (auto i:output)
    {
        char current_char = i;  // ���ַ��洢���ַ�����

        // ������ĸ
        for (int alp = 0; alp < alphabet_list_len; alp++)
        {
            // ����Ƿ�ΪСд��ĸ
            if (tolower(current_char) == alphabet_list[alp][0])
            {
                // ����Ƿ�Ϊ��д��ĸ
                bool is_uppercase = isupper(current_char); // ����Ǵ�д

                if (is_uppercase)  // ����д�д��ĸ
                {
                    INPUT inputs[2] = {0};  // ������������
                    // ����Ǵ�д��ĸ��������ģ�ⰴ�� Shift ��
                    inputs[0].type = INPUT_KEYBOARD;
                    inputs[0].ki.wVk = VK_SHIFT; // 'shift'�����������
                    inputs[0].ki.dwFlags = 0; // ��ʾ���°���

                    // ģ�ⰴ��Ŀ���
                    inputs[1].type = INPUT_KEYBOARD;
                    inputs[1].ki.wVk = alphabet_code[alp]; // Ŀ������������
                    inputs[1].ki.dwFlags = 0; // ��ʾ���°���
                    SendInput(2, inputs, sizeof(INPUT));  // ���Ͱ����¼���ϵͳ
                    shift_up = true;  // shift������

                    // �ͷ� Shift ����Ŀ���
                    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                    SendInput(2, inputs, sizeof(INPUT));  // �ٴη��Ͱ����¼���ϵͳ
                    shift_up = false;  // shift���ͷ�
                }
                else
                {
                    // �����Сд��ĸ����ֱ��ģ�ⰴ��Ŀ���
                    INPUT input; // ����һ�� INPUT �ṹ����� input���������������¼�

                    input.type = INPUT_KEYBOARD;
                    input.ki.wScan = 0; // ɨ������Ϊ 0��ͨ������Ҫʹ��
                    input.ki.time = 0; // ʱ�����Ϊ 0
                    input.ki.dwExtraInfo = 0; // ������Ϣ��Ϊ 0

                    // ģ�ⰴ��Ŀ���
                    input.ki.wVk = alphabet_code[alp]; // ָ��ģ�ⰴ�µļ�Ϊ Ŀ�� ��
                    input.ki.dwFlags = 0; // ָ�����̰����¼���dwFlags Ϊ 0
                    SendInput(1, &input, sizeof(INPUT)); // ���Ͱ����¼���ϵͳ
                    shift_up = true;  // shift������

                    // ģ���ͷ�Ŀ���
                    input.ki.dwFlags = KEYEVENTF_KEYUP; // ָ�������ͷ��¼���dwFlags ����Ϊ KEYEVENTF_KEYUP
                    SendInput(1, &input, sizeof(INPUT)); // �ٴη��Ͱ����¼���ϵͳ
                    shift_up = false;  // shift���ͷ�
                }
                break;
            }
        }


        for (int syb_no_shift=0; syb_no_shift< symbol_not_shift_len; syb_no_shift++)
        {
            // �Ƿ��봫��Ĳ���һ�� (�ж����谴shift�����ķ���)
            if (current_char == symbol_list_not_shift_press[syb_no_shift][0])  // ȡ����ǰ�ַ����е��ַ������������ַ����бȽ�
            {
                INPUT input; // ����һ�� INPUT �ṹ����� input���������������¼�

                input.type = INPUT_KEYBOARD; // ָ�� input ������Ϊ��������
                input.ki.wScan = 0; // ɨ������Ϊ 0��ͨ������Ҫʹ��
                input.ki.time = 0; // ʱ�����Ϊ 0
                input.ki.dwExtraInfo = 0; // ������Ϣ��Ϊ 0

                // ģ�ⰴ��Ŀ���
                input.ki.wVk = symbol_not_shift_code[syb_no_shift]; // ָ��ģ�ⰴ�µļ�Ϊ Ŀ�� ��
                input.ki.dwFlags = 0; // ָ�����̰����¼���dwFlags Ϊ 0
                SendInput(1, &input, sizeof(INPUT)); // ���Ͱ����¼���ϵͳ
                shift_up = true;  // shift������

                input.ki.wVk = symbol_not_shift_code[syb_no_shift]; // ָ��ģ���ͷŵļ���Ȼ�� Ŀ�� ��
                input.ki.dwFlags = KEYEVENTF_KEYUP; // ָ�������ͷ��¼���dwFlags ����Ϊ KEYEVENTF_KEYUP
                SendInput(1, &input, sizeof(INPUT)); // �ٴη��Ͱ����¼���ϵͳ
                shift_up = false;  // shift���ͷ�
                break;
            }
            // ����ո�
            if (current_char == ' ')
            {
                // ģ�ⰴ�¿ո��
                INPUT input; // ����һ�� INPUT �ṹ����� input���������������¼�

                input.type = INPUT_KEYBOARD;
                input.ki.wScan = 0; // ɨ������Ϊ 0��ͨ������Ҫʹ��
                input.ki.time = 0; // ʱ�����Ϊ 0
                input.ki.dwExtraInfo = 0; // ������Ϣ��Ϊ 0


                input.ki.wVk = VK_SPACE; // ָ��ģ�ⰴ�µļ�Ϊ �ո� ��
                input.ki.dwFlags = 0; // ָ�����̰����¼���dwFlags Ϊ 0
                SendInput(1, &input, sizeof(INPUT)); // ���Ͱ����¼���ϵͳ
                shift_up = true;  // shift������

                // ģ���ͷſո��
                input.ki.dwFlags = KEYEVENTF_KEYUP; // ָ�������ͷ��¼���dwFlags ����Ϊ KEYEVENTF_KEYUP
                SendInput(1, &input, sizeof(INPUT)); // �ٴη��Ͱ����¼���ϵͳ
                shift_up = false;  // shift���ͷ�
                break;
            }
        }

        for (int syb_need_shift=0; syb_need_shift< symbol_need_shift_len; syb_need_shift++)
        {
            // �Ƿ��봫��Ĳ���һ�� (�ж��谴shift�����ķ��Ź���)
            if (current_char == symbol_list_need_shift_press[syb_need_shift][0])  // ȡ����ǰ�ַ����е��ַ������������ַ����бȽ�
            {
                // ������������ṹ�岢��ʼ��
                INPUT inputs[2] = {0};

                // ��һ������ 'shift' ������ṹ��
                inputs[0].type = INPUT_KEYBOARD;
                inputs[0].ki.wVk = VK_SHIFT; // 'shift'�����������
                inputs[0].ki.dwFlags = 0; // ��ʾ���°���

                // �ڶ������� 'Ŀ�����' ������ṹ��
                inputs[1].type = INPUT_KEYBOARD;
                inputs[1].ki.wVk = symbol_need_shift_code[syb_need_shift]; // 'Ŀ�����'�����������
                inputs[1].ki.dwFlags = 0; // ��ʾ���°���
                SendInput(2, inputs, sizeof (INPUT));  // ����ָ���shift���������
                shift_up = true;  // shift������

                inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // ��ʾ�����ͷ�
                SendInput(2, inputs, sizeof(INPUT));  // ͬʱ�ͷ�
                shift_up = false;  // shift���ͷ�
                break;
            }
        }
    }
    if (!shift_up)
    {
        // ����INPUT�ṹ��
        INPUT input_shift = {0};
        input_shift.type = INPUT_KEYBOARD;  // ָ����������Ϊ����

        // ģ���ͷ�Shift��
        input_shift.ki.wVk = VK_SHIFT;       // �����������ΪShift
        input_shift.ki.dwFlags = KEYEVENTF_KEYUP;  // ָ�������ͷ��¼�

        // ���ͼ����ͷ��¼�
        SendInput(1, &input_shift, sizeof(INPUT));
    }
}
/*********** ���а�******************/
// �ַ���������������˹ //
void copy_str_in(copy_str_structs self)  // ���Ƶ����а�ĺ���������������CopyStr������
{
    if (OpenClipboard(NULL))
    {
        // ��ռ��а�����
        EmptyClipboard();

        // �����ڴ沢���ı����ݸ��Ƶ�ȫ���ڴ��
        HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, strlen(self.textToCopy) + 1); // +1��Ϊ�˰����ַ����Ľ�β null �ַ�
        LPVOID lpCopy = NULL;
        if (hglbCopy!= NULL)
        {
            lpCopy = GlobalLock(hglbCopy);
            strcpy((char*)lpCopy, self.textToCopy);
            GlobalUnlock(hglbCopy);

            // ��ȫ���ڴ������Ϊ���а�����
            SetClipboardData(CF_TEXT, hglbCopy);
        }
        else
        {
            // ���hglbCopy����ʧ�ܣ����lpCopy�Ƿ��Ѿ�����ȡ���ͷ�
            if (lpCopy!= NULL)
            {
                GlobalFree(lpCopy);
            }
        }

        // �رռ��а�
        CloseClipboard();
    }
}

// ��ֵ�����а� //
void KeyBoardEvent::CopyStr(const char *str)
{
    // �����ַ��ܵ����а�
    copy_str_structs self = {str, copy_str_in};
    self.copy_str_in(self);
}

/*************** ע���ݼ����� ******************/
// ����ȫ�ֱ��������湳�Ӿ�� //
HHOOK hHook = nullptr;  // HHOOK �������ڱ��湳�Ӿ��;

// ����һ���������̰�����̧���Ӿ�� //
HHOOK hHookDownOrUp = nullptr;  // ���Ӿ��

// ��ȡ������������벢���� //
int getOneKeyCode(string key_name)
{
    // ��ʼ������ֵΪ��һ�� //
    int index = 0;

    // ������ĸ���б���������� //
    for (auto i:alphabet_list)
    {
        // ����봫����ַ�����ȣ���ȡ����ֵ�������벢���أ���֮�������� //
        if (key_name == i)
        {
//            cout<<"index in :alphabet_list"<<index<<endl;
            return alphabet_code[index];
        }
        index++;
    }

    // �����һ��û���ҵ����������������ڹ��������Ҷ�Ӧ����� //
    index = 0;

    // ������ĸ���б���������� //
    for (auto i:function_keys_list)
    {
        // ����봫����ַ�����ȣ���ȡ����ֵ�������벢���أ���֮�������� //
        if (key_name == i)
        {
//            cout<<"index int function_keys_list:"<<index<<endl;
            return function_code[index];
        }
        index++;
    }

    // ������Ҳ���˵����ݼ�ע�᲻�Ϸ�������-1 //
    cout<<"The shortcut name does not exist. Make sure to register a valid shortcut\n";
    return  -1;
}

// ���̹��ӵĻص�����
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    for (auto hotKeys:key_code)
    {
        // ��鹳�Ӳ����Ƿ�ɹ� //
        if (nCode >= 0)
        {
            // ��ȡ���̹��ӽṹ�� //
            PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

//            // �ȼ���Ƿ��е�����ݼ������ⰴ��ע�� //
//            if (hotKeys.FuncID == 1 && hotKeys.key1 != 0)
//            {
////                printf("����Ϊ: %d\n ctrl�����: %d\n", hotKeys.key1, VK_CONTROL);
//                // ����Ƿ�Ϊ���̰�����Ϣ����ͬʱ����ע��Ŀ�ݼ�
//                if ((GetAsyncKeyState(hotKeys.key1) & 0x8000))               // ���ڶ������Ƿ���
//                {
//                    // �ص�ע��ĺ���
//                    oneKeyFunc[hotKeys.FuncIndex]();
//                    return CallNextHookEx(nullptr, nCode, wParam, lParam);
//                }
//            }

            // ��ע�ᵥ����ݼ�.����ȷ����ݼ��������Ч��IDȷ����ݼ�����һ�������絥��ݼ�[a]��˫��ݼ��ȴ�[ctrl+a] //
            if (hotKeys.FuncID==1 &&hotKeys.key1 !=0)
            {
                // �������̰����¼��������ȼ��ṹ���б��key1
                if (wParam == WM_KEYDOWN &&pKey->vkCode == hotKeys.key1)
                {
                    // �ص�ע��ĺ���  //
                    oneKeyFunc[hotKeys.FuncIndex]();
//                    cout<<"ע�ᵥ����ݼ�:"<<hotKeys.key1<<" ��������Ϊ:"<<hotKeys.FuncIndex<<"ID:"<<hotKeys.FuncID<<endl;
                    return CallNextHookEx(nullptr, nCode, wParam, lParam);
                }
            }

                // ˫��ݼ�ע��,��ѯ˫��ݼ��ṹ���������Ƿ񶼲�Ϊ0 //
            else if (hotKeys.FuncID == 2 && hotKeys.key1 != 0 && hotKeys.key2 != 0)
            {
                // ����Ƿ�Ϊ���̰�����Ϣ����ͬʱ����ע��Ŀ�ݼ�
                if ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) &&  // ������������,(wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ���ȼ��ж�
                    (GetAsyncKeyState(hotKeys.key1) & 0x8000) &&  // ������μ�����Ctrl��Alt���Ƿ���
                    (pKey->vkCode == hotKeys.key2))               // ���ڶ������Ƿ���
                {
                    // �ص�ע��ĺ���
                    twoKeyFunc[hotKeys.FuncIndex]();
                    return CallNextHookEx(nullptr, nCode, wParam, lParam);
                }
            }

                // ����ݼ�ע��,��ѯ����ݼ��ṹ���������Ƿ񶼲�Ϊ0 //
            else if (hotKeys.FuncID == 3 && hotKeys.key1 != 0 && hotKeys.key2 != 0 && hotKeys.key3 != 0)
            {
                // ��ȡ���̹��ӽṹ��
                PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

                // ����Ƿ�Ϊ���̰�����Ϣ����ͬʱ����ע��Ŀ�ݼ�
                if ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) &&  // ������������
                    (GetAsyncKeyState(hotKeys.key1) & 0x8000) &&  // ������μ�1����Ctrl���Ƿ���
                    (GetAsyncKeyState(hotKeys.key2) & 0x8000) &&  // ������μ�2����Alt���Ƿ���
                    (pKey->vkCode == hotKeys.key3))               // �����������Ƿ���
                {
                    // �ص�ע��ĺ���
                    threeKeyFunc[hotKeys.FuncIndex]();
                    return CallNextHookEx(nullptr, nCode, wParam, lParam);
                }
            }

                // �Ŀ�ݼ�ע��,��ѯ�Ŀ�ݼ��ṹ���������Ƿ񶼲�Ϊ0 //
            else if (hotKeys.FuncID == 4 && hotKeys.key1 != 0 && hotKeys.key2 != 0 && hotKeys.key3 != 0 && hotKeys.key4 != 0)
            {
//                printf("k1:%d key2:%d key3:%d key4:%d\n", hotKeys.key1, hotKeys.key2, hotKeys.key3, hotKeys.key4);
                // ��ȡ���̹��ӽṹ��
                PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

                // ����Ƿ�Ϊ���̰�����Ϣ����ͬʱ����ע��Ŀ�ݼ�
                if ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) &&  // ������������
                    (GetAsyncKeyState(hotKeys.key1) & 0x8000) &&  // ������μ�1����Ctrl���Ƿ���
                    (GetAsyncKeyState(hotKeys.key2) & 0x8000) &&  // ������μ�2����Alt���Ƿ���
                    (GetAsyncKeyState(hotKeys.key3) & 0x8000) &&  // ������μ�3����Shift���Ƿ���
                    (pKey->vkCode == hotKeys.key4))               // �����ĸ����Ƿ���
                {
                    // �ص�ע��ĺ���
                    fourKeyFunc[hotKeys.FuncIndex]();
                    return CallNextHookEx(nullptr, nCode, wParam, lParam);
                }
            }
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}


// ע��һ�����̵ͼ�����,����ע�ắ������ѭ�� //
//void KeyBoardEvent::AddHotKey(const string &key, void (*targetVoidFunc)())
//{
//    // ��ȡע��Ŀ�ݼ����� //
//    std::regex wordRegex(R"((\w+)+)");
//    // ����һ��������ʽ��������������text�ַ���������ƥ��wordRegexģʽ���Ӵ���
//    // text.begin()��text.end()�����������ķ�ΧѰ�ҿ�ݼ����ơ�
//    auto wordBegin = std::sregex_iterator(key.begin(), key.end(), wordRegex);
//
//    // ����һ����������������������������ʱ��Ϊ�������ıȽ�Ŀ�ꡣ
//    auto wordEnd = std::sregex_iterator();
//
//    // ��ʱ������������飬ÿ��ѭ����ʼ�����¶��� //
//    vector<int> keyCodeTemp;
//
//    // ʹ��forѭ����������ƥ����Ӵ���
//    for (std::sregex_iterator j = wordBegin; j != wordEnd; ++j)
//    {
//        // ��ȡ��ǰ������ָ���ƥ������
//        std::smatch match = *j;
//
//        // ���ƥ�䵽����ĸ���У�ʹ��match.str()��ȡƥ����ַ�����
////        std::cout << "ƥ�䵽����ĸ���У�" << match.str() << std::endl;
//
//        // ��ȡ����뺯�����������͵���ʱ������ //
//        keyCodeTemp.push_back(getOneKeyCode(match.str()));
//    }
//    // ��ȡ���鳤�ȣ����п�ݼ����鳤���жϣ�����ֵ����� //
//    int arrSize = keyCodeTemp.size();
//
//    // ��ʱ�������//
//    int Ctrl = 0;
//    int Alt = 0;
//    int Shift = 0;
//    int Other = 0;
//
//    //  ���ݳ��Ƚ��и�ֵ����룬����Ϊ1��case1���ȵ� //
//    switch (arrSize)
//    {
//        // ��������븳ֵ����룬�޵ĸ�ֵ0 //
//        case 1:KeyCode oneKey;
//            oneKey.key1 = keyCodeTemp[0];
//            oneKey.key2 = 0;
//            oneKey.key3 = 0;
//            oneKey.key4 = 0;
//
//            oneKey.FuncIndex = oneKeyIndex;  // ��ֵ����������ֵ+1������ע�ắ��������ע���˳��
//            oneKey.FuncID = 1;  // ����ֻ��һ������ע��
//            key_code.push_back(oneKey);  // ��ӿ�ݼ��ṹ�嵽����
//            oneKeyFunc.push_back(targetVoidFunc);  // ����ݼ��������ӿ�ݼ�����
//            oneKeyIndex++;  // ID����
//            break;
//
//            // ˫��ݼ� //
//        case 2:KeyCode  twoKey;
//            // ����ڶ�����ݼ�Ϊctrl,alt,shift�;���˳�� //
//            if (keyCodeTemp[1] ==VK_CONTROL || keyCodeTemp[1] ==VK_SHIFT || keyCodeTemp[1] ==VK_MENU)
//            {
//                // �����һ������ctrl�Ȱ�����ô������ĸ�ֵ��ǰ�� //
//                twoKey.key1 = keyCodeTemp[1];  // ȷ����һ������ctrl�Ȱ���
//                twoKey.key2 = keyCodeTemp[0];
//                twoKey.key3 = 0;
//                twoKey.key4 = 0;
//
//                twoKey.FuncIndex = twoKeyIndex;  // ��ֵ����������ֵ+1������ע�ắ��������ע���˳��
//                twoKey.FuncID = 2;  // ��������������ע��
//                key_code.push_back(twoKey);  // ��ӿ�ݼ��ṹ�嵽����
//                twoKeyFunc.push_back(targetVoidFunc);  // ˫��ݼ��������ӿ�ݼ�����
//                twoKeyIndex++;  // ID����
//                break;
//            }
//            twoKey.key1 = keyCodeTemp[0];
//            twoKey.key2 = keyCodeTemp[1];
//            twoKey.key3 = 0;
//            twoKey.key4 = 0;
//
//            twoKey.FuncIndex = twoKeyIndex;  // ��ֵ����������ֵ+1������ע�ắ��������ע���˳��
//            twoKey.FuncID = 2;  // ��������������ע��
//            key_code.push_back(twoKey);  // ��ӿ�ݼ��ṹ�嵽����
//            twoKeyFunc.push_back(targetVoidFunc);  // ˫��ݼ��������ӿ�ݼ�����
//            twoKeyIndex++;  // ID����
//            break;
//
//            // ����ݼ� //
//        case 3:KeyCode threeKey;
//            // ����������б����˳�� //
//            for (const auto &i:keyCodeTemp)
//            {
//                switch (i)
//                {
//                    case VK_CONTROL:
//                        Ctrl = i;
//                        break;
//                    case VK_SHIFT:
//                        Shift = i;
//                        break;
//                    case VK_MENU:
//                        Alt = i;
//                        break;
//                    default:
//                        Other = i;
//                        break;
//                }
//            }
//            if (Ctrl !=0 && Alt !=0 && Shift ==0)
//            {
//                // ���Ctrl��Alt��ע�ᣬAltû�б�ע�ᣬ����˳���� //
//                threeKey.key1 = Ctrl;
//                threeKey.key2 = Alt;
//                threeKey.key3 = Other;
//                threeKey.key4 = 0;
//            }
//            else if (Ctrl !=0 && Shift !=0 && Alt ==0)
//            {
//                // ���Ctrl��Shift��ע�ᣬAltû�б�ע�ᣬ����˳���� //
//                threeKey.key1 = Ctrl;
//                threeKey.key2 = Shift;
//                threeKey.key3 = Other;
//                threeKey.key4 = 0;
//            }
//            else if (Alt !=0 && Shift !=0 && Ctrl ==0)
//            {
//                // ���Alt��Shift��ע��,Ctrlû�б�ע�ᣬ����˳���� //
//                threeKey.key1 = Alt;
//                threeKey.key2 = Shift;
//                threeKey.key3 = Other;
//                threeKey.key4 = 0;
//            }
//            threeKey.FuncIndex = threeKeyIndex;  // ��ֵ����������ֵ+1������ע�ắ��������ע���˳��
//            threeKey.FuncID = 3;  // ��������������ע��
//            key_code.push_back(threeKey);  // ��ӿ�ݼ��ṹ�嵽����
//            threeKeyFunc.push_back(targetVoidFunc);  // ����ݼ��������ӿ�ݼ�����
//            threeKeyIndex++;  // ID����
//            break;
//
//            // �Ŀ�ݼ� //
//        case 4:KeyCode fourKey;
//            // ����������б����˳�� //
//            for (const auto &i:keyCodeTemp)
//            {
//                switch (i)
//                {
//                    case VK_CONTROL:
//                        Ctrl = i;
//                        cout<<"ctrl:"<<Ctrl<<endl;
//                        break;
//                    case VK_SHIFT:
//                        Shift = i;
//                        cout<<"shift:"<<Shift<<endl;
//                        break;
//                    case VK_MENU:
//                        Alt = i;
//                        cout<<"alt:"<<Alt<<endl;
//                        break;
//                    default:
//                        Other = i;
//                        cout<<"other:"<<Other<<endl;
//                        break;
//                }
//            }
//            printf("\n");
//
//            if (Ctrl !=0 && Alt !=0 && Shift !=0)
//            {
//                // ���Ctrl,Alt,Shift����ע�� //
//                fourKey.key1 = Ctrl;
//                fourKey.key2 = Alt;
//                fourKey.key3 = Shift;
//                fourKey.key4 = Other;
//            }
//
//            fourKey.FuncIndex = fourKeyIndex;  // ��ֵ����������ֵ+1������ע�ắ��������ע���˳��
//            fourKey.FuncID = 4; // �������ĸ�����ע��
//            key_code.push_back(fourKey);  // ��ӿ�ݼ��ṹ�嵽����
//            fourKeyFunc.push_back(targetVoidFunc);  // �Ŀ�ݼ��������ӿ�ݼ�����
//            fourKeyIndex++;  // ID����
//            break;
//
//    }
//}


// ������ݼ�ʹ��wait hot key �ص�KeyboardProc���� //
void KeyBoardEvent::AddHotKey(const string &key, std::function<void()> targetVoidFunc)
{
    // ��ȡע��Ŀ�ݼ����� //
    std::regex wordRegex(R"((\w+)+)");
    // ����һ��������ʽ��������������text�ַ���������ƥ��wordRegexģʽ���Ӵ���
    // text.begin()��text.end()�����������ķ�ΧѰ�ҿ�ݼ����ơ�
    auto wordBegin = std::sregex_iterator(key.begin(), key.end(), wordRegex);

    // ����һ����������������������������ʱ��Ϊ�������ıȽ�Ŀ�ꡣ
    auto wordEnd = std::sregex_iterator();

    // ��ʱ������������飬ÿ��ѭ����ʼ�����¶��� //
    vector<int> keyCodeTemp;

    // ʹ��forѭ����������ƥ����Ӵ���
    for (std::sregex_iterator j = wordBegin; j != wordEnd; ++j)
    {
        // ��ȡ��ǰ������ָ���ƥ������
        std::smatch match = *j;

        // ���ƥ�䵽����ĸ���У�ʹ��match.str()��ȡƥ����ַ�����
//        std::cout << "ƥ�䵽����ĸ���У�" << match.str() << std::endl;

        // ��ȡ����뺯�����������͵���ʱ������ //
        keyCodeTemp.push_back(getOneKeyCode(match.str()));
    }
    // ��ȡ���鳤�ȣ����п�ݼ����鳤���жϣ�����ֵ����� //
    int arrSize = keyCodeTemp.size();

    // ��ʱ�������//
    int Ctrl = 0;
    int Alt = 0;
    int Shift = 0;
    int Other = 0;

    //  ���ݳ��Ƚ��и�ֵ����룬����Ϊ1��case1���ȵ� //
    switch (arrSize)
    {
        // ��������븳ֵ����룬�޵ĸ�ֵ0 //
        case 1:KeyCode oneKey;
            oneKey.key1 = keyCodeTemp[0];
            oneKey.key2 = 0;
            oneKey.key3 = 0;
            oneKey.key4 = 0;

            oneKey.FuncIndex = oneKeyIndex;  // ��ֵ����������ֵ+1������ע�ắ��������ע���˳��
            oneKey.FuncID = 1;  // ����ֻ��һ������ע��
            key_code.push_back(oneKey);  // ��ӿ�ݼ��ṹ�嵽����
            oneKeyFunc.push_back(targetVoidFunc);  // ����ݼ��������ӿ�ݼ�����
            oneKeyIndex++;  // ID����
            if (keyCodeTemp[0] == VK_CONTROL)
            {
                printf("ctrl %d\n", keyCodeTemp[0]);
            }
            if (keyCodeTemp[0] == VK_SHIFT)
            {
                printf("shift %d\n", keyCodeTemp[0]);
            }
            if (keyCodeTemp[0] == VK_MENU)
            {
                printf("alt %d\n", keyCodeTemp[0]);
            }
            break;

            // ˫��ݼ� //
        case 2:KeyCode  twoKey;
            // ����ڶ�����ݼ�Ϊctrl,alt,shift�;���˳�� //
            if (keyCodeTemp[1] ==VK_CONTROL || keyCodeTemp[1] ==VK_SHIFT || keyCodeTemp[1] ==VK_MENU)
            {
                // �����һ������ctrl�Ȱ�����ô������ĸ�ֵ��ǰ�� //
                twoKey.key1 = keyCodeTemp[1];  // ȷ����һ������ctrl�Ȱ���
                twoKey.key2 = keyCodeTemp[0];
                twoKey.key3 = 0;
                twoKey.key4 = 0;

                twoKey.FuncIndex = twoKeyIndex;  // ��ֵ����������ֵ+1������ע�ắ��������ע���˳��
                twoKey.FuncID = 2;  // ��������������ע��
                key_code.push_back(twoKey);  // ��ӿ�ݼ��ṹ�嵽����
                twoKeyFunc.push_back(targetVoidFunc);  // ˫��ݼ��������ӿ�ݼ�����
                twoKeyIndex++;  // ID����
                break;
            }
            twoKey.key1 = keyCodeTemp[0];
            twoKey.key2 = keyCodeTemp[1];
            twoKey.key3 = 0;
            twoKey.key4 = 0;

            twoKey.FuncIndex = twoKeyIndex;  // ��ֵ����������ֵ+1������ע�ắ��������ע���˳��
            twoKey.FuncID = 2;  // ��������������ע��
            key_code.push_back(twoKey);  // ��ӿ�ݼ��ṹ�嵽����
            twoKeyFunc.push_back(targetVoidFunc);  // ˫��ݼ��������ӿ�ݼ�����
            twoKeyIndex++;  // ID����
            break;

            // ����ݼ� //
        case 3:KeyCode threeKey;
            // ����������б����˳�� //
            for (const auto &i:keyCodeTemp)
            {
                switch (i)
                {
                    case VK_CONTROL:
                        Ctrl = i;
                        break;
                    case VK_SHIFT:
                        Shift = i;
                        break;
                    case VK_MENU:
                        Alt = i;
                        break;
                    default:
                        Other = i;
                        break;
                }
            }
            if (Ctrl !=0 && Alt !=0 && Shift ==0)
            {
                // ���Ctrl��Alt��ע�ᣬShiftû�б�ע�ᣬ����˳���� //
                threeKey.key1 = Ctrl;
                threeKey.key2 = Alt;
                threeKey.key3 = Other;
                threeKey.key4 = 0;
            }
            else if (Ctrl !=0 && Shift !=0 && Alt ==0)
            {
                // ���Ctrl��Shift��ע�ᣬAltû�б�ע�ᣬ����˳���� //
                threeKey.key1 = Ctrl;
                threeKey.key2 = Shift;
                threeKey.key3 = Other;
                threeKey.key4 = 0;
            }
            else if (Alt !=0 && Shift !=0 && Ctrl ==0)
            {
                // ���Alt��Shift��ע��,Ctrlû�б�ע�ᣬ����˳���� //
                threeKey.key1 = Alt;
                threeKey.key2 = Shift;
                threeKey.key3 = Other;
                threeKey.key4 = 0;
            }
            threeKey.FuncIndex = threeKeyIndex;  // ��ֵ����������ֵ+1������ע�ắ��������ע���˳��
            threeKey.FuncID = 3;  // ��������������ע��
            key_code.push_back(threeKey);  // ��ӿ�ݼ��ṹ�嵽����
            threeKeyFunc.push_back(targetVoidFunc);  // ����ݼ��������ӿ�ݼ�����
            threeKeyIndex++;  // ID����
            break;

            // �Ŀ�ݼ� //
        case 4:KeyCode fourKey;
            // ����������б����˳�� //
            for (const auto &i:keyCodeTemp)
            {
                switch (i)
                {
                    case VK_CONTROL:
                        Ctrl = i;
                        cout<<"ctrl:"<<Ctrl<<endl;
                        break;
                    case VK_SHIFT:
                        Shift = i;
                        cout<<"shift:"<<Shift<<endl;
                        break;
                    case VK_MENU:
                        Alt = i;
                        cout<<"alt:"<<Alt<<endl;
                        break;
                    default:
                        Other = i;
                        cout<<"other:"<<Other<<endl;
                        break;
                }
            }
            printf("\n");

            if (Ctrl !=0 && Alt !=0 && Shift !=0)
            {
                // ���Ctrl,Alt,Shift����ע�� //
                fourKey.key1 = Ctrl;
                fourKey.key2 = Alt;
                fourKey.key3 = Shift;
                fourKey.key4 = Other;
            }

            fourKey.FuncIndex = fourKeyIndex;  // ��ֵ����������ֵ+1������ע�ắ��������ע���˳��
            fourKey.FuncID = 4; // �������ĸ�����ע��
            key_code.push_back(fourKey);  // ��ӿ�ݼ��ṹ�嵽����
            fourKeyFunc.push_back(targetVoidFunc);  // �Ŀ�ݼ��������ӿ�ݼ�����
            fourKeyIndex++;  // ID����
            break;

    }
}



// �ȴ���ݼ� //
void KeyBoardEvent::WaitHotKey()
{
    // ������ݼ�ѭ������
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);  // ��װ�ͼ����̹���

    MSG msg;          // MSG �ṹ�����ڴ洢����Ϣ�����л�ȡ����Ϣ


    // ������Ϣѭ��
    while (GetMessage(&msg, NULL, 0, 0))  // ����Ϣ�����л�ȡ��Ϣ
    {
        TranslateMessage(&msg);           // ���������ת�����ַ�
        DispatchMessage(&msg);            // �ַ���Ϣ�����ڹ���
    }
}


// ����Ϊ static ClearHotKey ���Ա��Ա�������������ݼ����� //
void KeyBoardEvent::ClearHotKey()
{
    if (hHook != NULL || hHook!= nullptr)
    {
        // ȷ�����ӱ�ж��
        UnhookWindowsHookEx(hHook);       // ж�ع���
        // ���ͳ��������Ϣ
        PostQuitMessage(0);
        hHook = nullptr;  // ���ù��Ӿ��
//        cout<<"������ж��\n";
    }
}



/**************** ������ ******************/
void cleanup_check()
{
//    cout<<"��ʼ������\n";
    // ����δ�ͷŵ���� //
    MouseEvent mouse;
    // �ͷ���꣬�����갴�� //
    if (mouse_flag.mouse_left_down)
    {
        // �ͷ����
        mouse.MouseUp("left");
        mouse_flag.mouse_left_down = False;  // ���±�־
//        cout<<"������ͷ�\n";
    }
    else if(mouse_flag.mouse_right_down)
    {
        // �ͷ��Ҽ�
        mouse.MouseUp("right");
        mouse_flag.mouse_right_down = False;  // ���±�־
//        cout<<"�Ҽ����ͷ�\n";
    }

    // ����δ�ͷŵļ��� //
    // �ͷ� Ctrl��Alt��Shift �� Win ��
    ////// �ͷż��� //////
    int len = sizeof (free_keys.key_code)/sizeof (free_keys.key_code[0]);
    for (int f=0; f<len; f++)
    {
        if (free_keys.key_code[f] !=0)
        {
            INPUT input = {0};
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = free_keys.key_code[f];  // ��λ����
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &input, sizeof(INPUT));
//            printf("�ͷ������ͷŵİ���: %d \n", free_keys.key_code[f]);
        }
    }

    // �������¼������Ƿ�ע�� //
    if (hMouseHook != NULL || hMouseHook!= nullptr)
    {
        // ���ȫ�ֱ���hMouseHook�Ƿ���Ч
        if (hMouseHook != NULL)
        {
            // �Ƴ���깳��
            UnhookWindowsHookEx(hMouseHook);
            hMouseHook = NULL; // ��ȫ�ֱ�����Ϊ��
            // ���ͳ��������Ϣ
        }
//    exit(0); // �˳�����
    }

    // ���¼����̿�ݼ�hHook�Ƿ�ע�� //
    if (hHook != NULL || hHook!= nullptr)
    {
        // ȷ�����ӱ�ж��
        UnhookWindowsHookEx(hHook);       // ж�ع���
        // ���ͳ��������Ϣ
        PostQuitMessage(0);
        hHook = nullptr;  // ���ù��Ӿ��
    }

}

// ���������� //
void ExitCheckWork()
{
    atexit(cleanup_check);  // �˳���ִ�е�����
}

void winKeyPressHelp()
{
    string user_help = R"(
# WinKeyPress ���ĵ�

## ����
�ÿ��ṩ��Windowsϵͳ���ͼ����¼���ģ�������֧����Ļ��Ϣ��ȡ�������ơ���ݼ�ע��ȹ��ܡ��������Զ����ű����ȼ���������ģ�ⳡ����

---

## ͷ�ļ�����
```cpp
#include "winKeyPressH.hpp"
using namespace std;
```

---

## ������ (MouseEvent)

### ��ʼ��
```cpp
MouseEvent mouse;
```

### �����б�

#### 1. ��ȡ��Ļ�ֱ���
```cpp
ScreenSize GetScreenSIze(bool output = true);
```
- **����**��`output` �Ƿ�����ֱ�����Ϣ������̨
- **����**��`ScreenSize`�ṹ�� (���� `x`, `y` ����)
```cpp
ScreenSize screen = GetScreenSIze();
cout << "Width: " << screen.x << ", Height: " << screen.y;
```

#### 2. ��ȡ���λ��
```cpp
MousePosition GetMousePosition(bool output = true);
```
- **����**��`MousePosition`�ṹ�� (���� `x`, `y` ����)
```cpp
MousePosition pos = GetMousePosition();
mouse.MouseMoveTo(pos.x + 100, pos.y); // ����100����
```

#### 3. �ƶ����
```cpp
void MouseMoveTo(int x, int y);
```
```cpp
mouse.MouseMoveTo(500, 300); // �ƶ�������(500,300)
```

#### 4. �����
```cpp
void MouseClick(int x, int y, int clicks = 1, const string& button = "left");
```
- **����**��
  - `x`, `y`: Ŀ������
  - `clicks`: �������
  - `button`: "left"(Ĭ��)/"right"
```cpp
mouse.MouseClick(100, 200, 2); // ���˫��
mouse.MouseClick(300, 400, 1, "right"); // �Ҽ�����
```

#### 5. ����/�ͷ�����
```cpp
void MouseDown(const string& button = "left");
void MouseUp(const string& button = "left");
```
```cpp
mouse.MouseDown(); // �������
sleep(1);
mouse.MouseUp();   // �ͷ����
```

#### 6. ���ֹ���
```cpp
void MouseRoll(int move);
```
- **����**���������Ϲ�������������
```cpp
mouse.MouseRoll(120);  // ���Ϲ���
mouse.MouseRoll(-120); // ���¹���
```

#### 7. ��������¼�
```cpp
void ListenMouseEvent();
```
- **����ʽ����**�����ڶ����߳�����
```cpp
thread t([&](){ mouse.ListenMouseEvent(); });
t.detach();
```

---

## ���̲��� (KeyBoardEvent)

### ��ʼ��
```cpp
KeyBoardEvent keyboard;
```

### �����б�

#### 1. ��������/�ͷ�
```cpp
void KeyDown(const string& key);
void KeyUp(const string& key);
```
- **֧�ְ�����**����ĸ�����֡�"ctrl", "alt", "shift", "F1"-"F12", "esc", "space"��
```cpp
keyboard.KeyDown("ctrl");
keyboard.KeyDown("c");
keyboard.KeyUp("c");
keyboard.KeyUp("ctrl"); // ģ��Ctrl+C
```

#### 2. ���ΰ���
```cpp
void PressKey(const string& key);
```
```cpp
keyboard.PressKey("enter"); // ���²��ͷŻس�
```

#### 3. ��ϼ�
```cpp
void PressHotKey(const string& key1, const string& key2, const string& key3 = "NULL");
```
```cpp
keyboard.PressHotKey("ctrl", "shift", "esc"); // Ctrl+Shift+Esc
```

#### 4. �����ַ���
```cpp
void WriteStr(string output);
```
```cpp
keyboard.WriteStr("Hello World!"); // ģ������
```

#### 5. ���������
```cpp
void CopyStr(const char *str);
```
```cpp
keyboard.CopyStr("Copied text"); // ���Ƶ�������
```

#### 6. ע���ݼ�
```cpp
void AddHotKey(const string &key, std::function<void()> targetVoidFunc);
void WaitHotKey();
```
- **��ݼ���ʽ**���ÿո�ָ����μ����������� `"ctrl alt s"`
```cpp
keyboard.AddHotKey("ctrl k", [](){
    cout << "��ݼ�������" << endl;
});
keyboard.WaitHotKey(); // �������������ڶ����߳�����
```

---

## ������ע������

### �Զ�����
- ���캯���Զ�ע���˳����������ͷ�δ�ɿ��İ�����
- �ֶ���������
  ```cpp
  KeyBoardEvent::ClearHotKey();
  MouseEvent::ExitMouseEvent();
  ```

### ���߳̽���
- �������� (`ListenMouseEvent`, `WaitHotKey`) ���ڶ����߳������Ա�������������

---

## ʾ������

```cpp
#include <thread>
#include "winKeyPressH.hpp"

int main() {
    // ���ʾ��
    MouseEvent mouse;
    mouse.MouseClick(500, 300); // �����Ļ����

    // ����ʾ��
    KeyBoardEvent keyboard;
    keyboard.WriteStr("�Զ��������");

    // ��ݼ�ע��
    keyboard.AddHotKey("ctrl shift q", [](){
        cout << "�˳�����" << endl;
        exit(0);
    });

    thread hotkeyThread([&](){ keyboard.WaitHotKey(); });
    hotkeyThread.join();

    return 0;
}
```
)";
    printf("%s\n", user_help.c_str());
}