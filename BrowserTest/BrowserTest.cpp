// BrowserTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <tchar.h>
#include <Windows.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")


BOOL GetRequest(WCHAR *site, WCHAR *uri)
{
	BOOL	r = FALSE;
	HANDLE session{}, connection{}, request{};

	session = WinHttpOpen(NULL, 0, NULL, NULL, 0);
	if (session)
		connection = WinHttpConnect(session, site, INTERNET_DEFAULT_HTTP_PORT, 0);

	if (connection)
		request = WinHttpOpenRequest(connection, NULL, uri, NULL, NULL, NULL, 0);

	if (request)
		r = WinHttpSendRequest(request, NULL, 0, NULL, 0, 0, NULL);

	if (r)
		r = WinHttpReceiveResponse(request, 0);

	DWORD numberOfBytes = 0;
	BYTE *data = NULL;
	DWORD ret;
	while (r) {
		r = WinHttpQueryDataAvailable(request, &numberOfBytes);
		
		if (r) {
			if (numberOfBytes == 0)
				break;
			data = new BYTE[numberOfBytes + 1];
			r = WinHttpReadData(request, data, numberOfBytes, &ret);
			data[numberOfBytes] = 0;
			if (r) {
				printf("%s", data);
			}
			delete data;
		}
	}
	
	if (request)
		WinHttpCloseHandle(request);

	if (connection)
		WinHttpCloseHandle(connection);

	if (session)
		WinHttpCloseHandle(session);

	return r;
}

int main()
{
	GetRequest(L"127.0.0.1", L"/");
    return 0;
}

