// zsIRC_Socket.h: interface for the zsIRC_Socket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZSIRC_SOCKET_H__9F11AA4B_265C_4B17_8605_D40FF0096EE1__INCLUDED_)
#define AFX_ZSIRC_SOCKET_H__9F11AA4B_265C_4B17_8605_D40FF0096EE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include <sslsock.h>

typedef void (WINAPI * ZSIRC_LOGGER)(void * u, TCHAR *);

class zsIRC_Socket  
{
public:
	int isConnected();
	zsIRC_Socket();
	virtual ~zsIRC_Socket();

	void Log(ZSIRC_LOGGER log, void * u, TCHAR * szFormat, ...);
	int Connect(char * sServerName,int port,ZSIRC_LOGGER log = NULL, void * u = NULL, bool bSecure = false);
  int InitializeGPRS(ZSIRC_LOGGER log, void * u);
	int Disconnect();
	int Read(void * pData, unsigned int iLen);
	int Write(void * pData, unsigned int iLen);
	int hasData();

  SOCKET sock;
};

#endif // !defined(AFX_ZSIRC_SOCKET_H__9F11AA4B_265C_4B17_8605_D40FF0096EE1__INCLUDED_)
