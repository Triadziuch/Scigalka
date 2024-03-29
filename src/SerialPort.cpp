#include "SerialPort.h"

// Constructors / Destructors
SerialPort::SerialPort(char* portName)
{
	errors = 0;
	status = { 0 };
	connected = false;

	handleToCOM = CreateFileA(static_cast<LPCSTR>(portName), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD errMsg = GetLastError();

	if (errMsg == 2)
		printf("Urzadzenie odlaczone.\n");
	else if (errMsg == 5)
		printf("Urzadzenie jest uzywane przez inny proces.\n");
	else if (errMsg == 0) {
		DCB dcbSerialParameters = { 0 };

		if (!GetCommState(handleToCOM, &dcbSerialParameters))
			printf("Nie udalo sie uzyskac ustawien polaczenia.");
		else {
			dcbSerialParameters.BaudRate = CBR_115200;
			dcbSerialParameters.ByteSize = 8;
			dcbSerialParameters.StopBits = ONESTOPBIT;
			dcbSerialParameters.Parity = NOPARITY;
			dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(handleToCOM, &dcbSerialParameters))
				printf("Nie udalo sie zmienic ustawien polaczenia.\n");
			else {
				connected = true;
				PurgeComm(handleToCOM, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(2000);
			}
		}
	}
}

SerialPort::SerialPort(const SerialPort& _obj)
{
	this->handleToCOM	 = _obj.handleToCOM;
	this->connected		 = _obj.connected;
	this->status		 = _obj.status;
	this->errors		 = _obj.errors;
}

SerialPort::~SerialPort()
{
	if (connected) {
		connected = false;
		CloseHandle(handleToCOM);
	}
}

// Public functions
int SerialPort::ReadSerialPort(char* buffer, unsigned int buf_size)
{
	DWORD bytesRead;
	unsigned int toRead = 0;

	ClearCommError(handleToCOM, &errors, &status);

	if (status.cbInQue > 0) {
		if (status.cbInQue > buf_size)
			toRead = buf_size;
		else
			toRead = status.cbInQue;
	}

	if (toRead != 0 && ReadFile(handleToCOM, buffer, toRead, &bytesRead, NULL))
		return bytesRead;
	return 0;
}

int SerialPort::WriteSerialPort(const char* buffer, unsigned int buf_size)
{
	DWORD bytesWritten;

	ClearCommError(handleToCOM, &errors, &status);

	if (WriteFile(handleToCOM, buffer, buf_size, &bytesWritten, NULL))
		return bytesWritten;
	return 0;
}