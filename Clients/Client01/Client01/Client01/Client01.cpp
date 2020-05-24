// Client01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Client01.h"
#include "afxsock.h" // Lib dung trong Socket

#include<string>
#include<iostream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: code your application's behavior here.

            // Khai bao su dung winsock
            AfxSocketInit(NULL);

            //Socket client
            CSocket client;

            // Khởi tạo socket_client và connect tới server tại PORT 9909
            client.Create(); // Client tự lấy PORT trống và  conect tới server

            client.Connect(_T("localhost"), 9909); // Connect tới địa chỉ localhost và PORT 9909 hoặc có thể sử dụng địa chỉ loopback 127.0.0.1 để gọi đến chính máy đang chạy (Vì client và server đang cùng 1 máy)

            // Các biến dùng để chat
            char* msgReci;
            char msgSend[101];
            int len = 0;

            // Nếu server accept connect thì bắt đầu giao tiếp
            while (1) {
                // Client first
                cout << "Client: ";
                gets_s(msgSend);
                len = strlen(msgSend);

                // Send tới server
                client.Send(&len, sizeof(int), 0); // Gửi độ dài qua trước để bên nhận khỏi nhận thiếu
                client.Send(msgSend, len, 0); // Gửi thông điệp

                // Get msg từ server
                client.Receive(&len, sizeof(int), 0);
                msgReci = new char[len + 1];
                client.Receive(msgReci, len, 0); // Chổ này cần while loop
                msgReci[len] = '\0';

                // Xuất msg từ server
                cout << "Server: " << msgReci << endl;
            }

            // Đóng socket
            client.Close();
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
