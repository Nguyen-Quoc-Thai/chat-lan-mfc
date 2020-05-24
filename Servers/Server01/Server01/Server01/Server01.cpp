// Server01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Server01.h"
#include "afxsock.h" // Lib dung trong Socket

#include<string>
#include <cstdio>
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


            /* Bên phía server phải khai báo nhiều hơn client 1 socket là vì
               phải có 1 server root để quản lý chung và một socket để lắng nghe và
               giao tiếp với client.

                             ________ server01  ---------- client01
                            /
               root_server /_________ server02  ---------- client02
                           \
                            \________ server03  ---------- client03


            */

            //Socket
            CSocket server;
            CSocket client;

            // Khởi tạo socket_server là lắng nghe kết nối từ client với PORT tuỳ ý
            // Do đây là server nên ta cần truyền PORT để cố định. Còn về client thì không cần vì nó tự tạo PORT và connect tới server
            server.Create(9909);

            server.Listen();
            cout << "Waiting for connect from clients !!!" << endl;

            // Nếu có clients connect tới thì Accept
            if (server.Accept(client)) {
                cout << "Client connected !" << endl;
            }

            // Các biến dùng để chat
            int len = 0;
            char msgSend[101];
            char *msgReci;

            // Giao tiếp giữa server và client
            while (1) {
                // Get msg từ client
                client.Receive(&len, sizeof(int), 0);
                msgReci = new char[len + 1];
                client.Receive(msgReci, len, 0); // Chổ này cần while loop
                msgReci[len] = '\0';

                // Xuất msg từ client
                cout << "Client: " << msgReci << endl;

                // Free msgReci
                delete msgReci;

                // Gửi lại msg gì đó cho client
                cout << "Server: ";
                gets_s(msgSend);
                len = strlen(msgSend);

                // Send qua
                client.Send(&len, sizeof(int), 0); // Gửi độ dài qua trước để bên nhận khỏi nhận thiếu
                client.Send(msgSend, len, 0); // Gửi thông điệp
            }

            // Đóng socket
            client.Close();
            server.Close();
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
