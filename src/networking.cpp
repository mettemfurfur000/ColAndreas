#include "include/networking.h"

#ifdef __cplusplus
extern "C"
{
#endif

	void ErrorExit(LPCTSTR lpszFunction)
	{
		// Retrieve the system error message for the last-error code

		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		// Display the error message and exit the process

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
										  (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
		StringCchPrintf((LPTSTR)lpDisplayBuf,
						LocalSize(lpDisplayBuf) / sizeof(TCHAR),
						TEXT("%s failed with error %d: %s"),
						lpszFunction, dw, lpMsgBuf);
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
		ExitProcess(dw);
	}

	// returns sockaddr, pointing to server machine (used only on client machine)
	struct sockaddr_in make_sockaddr(int port_number, uint32_t inaddr)
	{
		struct sockaddr_in sa_in;

		memset(&sa_in, 0, sizeof(sa_in));
		sa_in.sin_family = AF_INET;
		sa_in.sin_addr.s_addr = htonl(inaddr);
		sa_in.sin_port = htons(port_number);

		return sa_in;
	}

	// acts as recv, just adds null terminator at the end
	int recvstr(int socket, char *dest, const size_t maxlen, int flags)
	{
		int len = recv(socket, dest, maxlen, flags);
		dest[len] = '\0';
		return len;
	}

	int winsockinit()
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;

		wVersionRequested = MAKEWORD(2, 2);

		err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			printf("WSAStartup failed with error: %d\n", err);
			return -1;
		}
		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		{
			printf("Could not find a usable version of Winsock.dll\n");
			WSACleanup();
			return -1;
		}
		// printf("The Winsock 2.2 dll was found okay\n");

		return 0;
	}

	int init_client(int *socket_dest, struct sockaddr_in *servaddr_dest)
	{
		if (winsockinit() == -1)
			return -1;

		*socket_dest = socket(AF_INET, SOCK_STREAM, 0);
		*servaddr_dest = make_sockaddr(PORTNUM, INADDR_LOOPBACK);

		if (connect(*socket_dest, (struct sockaddr *)servaddr_dest, sizeof(struct sockaddr_in)))
			return -1;
		return 0;
	}

	int init_server(int *socket_dest, struct sockaddr_in *servaddr_dest)
	{
		if (winsockinit() == -1)
			return -1;

		*socket_dest = socket(AF_INET, SOCK_STREAM, 0);
		*servaddr_dest = make_sockaddr(PORTNUM, INADDR_LOOPBACK);

		if (bind(*socket_dest, (struct sockaddr *)servaddr_dest, sizeof(struct sockaddr_in)))
			return -1;
		return 0;
	}

	void finish_networking()
	{
		WSACleanup();
	}

#ifdef __cplusplus
}
#endif