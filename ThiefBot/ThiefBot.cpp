// ThiefBot.cpp: definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <sstream>
#include <string>
#include <windows.h>
#include <time.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
int main()
{
	SYSTEM_POWER_STATUS status;

	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;
	
	//Request URL Params
	std::string s = "<your_bot_token>";

	std::string fullAddress = "/bot";
	fullAddress.append(s);
	fullAddress.append("/sendMessage");

	std::wstring stemp = std::wstring(fullAddress.begin(), fullAddress.end());
	LPCWSTR sw = stemp.c_str();
	//Request body
	LPSTR postdata = const_cast<char *>("{\"text\":\"prova\", \"chat_id\":\"<your_chat_id>\"}");

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"WinHTTP Example/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, L"api.telegram.org",
			INTERNET_DEFAULT_HTTPS_PORT, 0);

	// Create an HTTP request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"GET", sw,
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			WINHTTP_FLAG_SECURE);

	// Add a request header.
	if (hRequest)
		bResults = WinHttpAddRequestHeaders(hRequest,
			L"content-type:application/json",
			(ULONG)-1L,
			WINHTTP_ADDREQ_FLAG_ADD);

	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			-1L,
			postdata, strlen(postdata),
			strlen(postdata), 0);


	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		do
		{
			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				printf("Error %u in WinHttpQueryDataAvailable.\n",
					GetLastError());

			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize = 0;
			}
			else
			{
				// Read the data.
				ZeroMemory(pszOutBuffer, dwSize + 1);

				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
					dwSize, &dwDownloaded))
					printf("Error %u in WinHttpReadData.\n", GetLastError());
				else
					printf("%s", pszOutBuffer);

				// Free the memory allocated to the buffer.
				delete[] pszOutBuffer;
			}
		} while (dwSize > 0);
	}


	// Report any errors.
	if (!bResults)
		printf("Error %d has occurred.\n", GetLastError());

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	do {
		GetSystemPowerStatus(&status);
		printf("ACLine: %d\n", status.ACLineStatus);
		printf("Battery: %d\%\n", status.BatteryLifePercent);
		printf("Battery: flag %d\n\n", status.BatteryFlag);
		Sleep(1000);
	} while (true);

	return status.ACLineStatus;
}

