#include "stdafx.h"
#include <Windows.h>
#include <atlstr.h> 

int main()
{
	SetConsoleTitle(L"Razor IMU");
	
	HANDLE hSerial;
	int cNum = 3;

	CString sPortName;
	sPortName.Format(_T("COM%d"), cNum);

	hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("%s", "serial port does not exist.\r\n");
		}
		printf("%s", "some other error occurred.\r\n");
	}

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams))
	{
		printf("%s", "getting state error\r\n");
	}
	dcbSerialParams.BaudRate = CBR_115200;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	dcbSerialParams.EvtChar = '\n';

	if (!SetCommState(hSerial, &dcbSerialParams))
	{
		printf("%s", "error setting serial port state\r\n");
	}

	DWORD bytes_read;
	float buffer[3]; //yaw, pitch, roll
	memset(&buffer, 0, sizeof(buffer));

	while (true) {
		if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0 || (GetAsyncKeyState(VK_RETURN) & 0x8000)) break;
		ReadFile(hSerial, &buffer, sizeof(buffer), &bytes_read, 0); 
		if (bytes_read > 0)
			printf("%7.2f \t %7.2f \t %7.2f\r\n", buffer[0], buffer[1], buffer[2]);

	}
	
	unsigned long ulCommErr = 0;
	ClearCommBreak(hSerial);
	ClearCommError(hSerial, &ulCommErr, NULL);

	CloseHandle(hSerial);
	system("pause");
    return 0;
}

