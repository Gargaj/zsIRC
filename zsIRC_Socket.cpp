// zsIRC_Socket.cpp: implementation of the zsIRC_Socket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zsIRC.h"
#include "zsIRC_Socket.h"
#include "zsIRC_Settings.h"
#include <objbase.h>
#include <initguid.h>
#if UNDER_CE >= 420
#include <connmgr.h>
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

zsIRC_Socket::zsIRC_Socket()
{
  sock = NULL;
}

zsIRC_Socket::~zsIRC_Socket()
{

}

void zsIRC_Socket::Log(ZSIRC_LOGGER log, void * u, TCHAR * szFormat, ...) {
  if (!log) return;

  TCHAR msg[2000];

  va_list ap;
  va_start(ap, szFormat);
  _vsntprintf(msg, 2000, szFormat, ap);
  va_end(ap);
  
  log(u,msg);
}

int SSLValidateCertHook(
  DWORD dwType, // in
  LPVOID pvArg, // in
  DWORD dwChainLen, // in
  LPBLOB pCertChain, // in
  DWORD dwFlags) // in
{
  if (dwFlags & SSL_CERT_FLAG_ISSUER_UNKNOWN) {
    switch (SETUP.nServerSSLBehaviour) {
      case ZSIRC_SSLBEHAVIOUR_ALWAYSACCEPT:
        {
          return SSL_ERR_OKAY;
        } break;
      case ZSIRC_SSLBEHAVIOUR_NEVERACCEPT:
        {
          return SSL_ERR_CERT_UNKNOWN;
        } break;
      case ZSIRC_SSLBEHAVIOUR_PROMPTUSER:
      default:
        {
          if (MessageBox(GetForegroundWindow(),
            _T("The certificate issuer for this server is unknown. Still accept the certificate?"),
            _T("zsIRC SSL"),MB_YESNO | MB_ICONQUESTION)==IDNO) {
            return SSL_ERR_CERT_UNKNOWN;
          }
          return SSL_ERR_OKAY;
        } break;
    }
  }
  return SSL_ERR_OKAY;
}

int zsIRC_Socket::Connect(char * sServerName, int port, ZSIRC_LOGGER log, void * u, bool bSecure)
{
  WSADATA WSData;
  if(WSAStartup(MAKEWORD(1, 1), &WSData)) {
    DWORD e = WSAGetLastError();
    Log(log,u,_T("WSAStartup failed: %d"),e);
    return 0;
  }

  unsigned int addr = inet_addr(sServerName);
  if (addr == INADDR_NONE) {
    hostent * hostEntry = gethostbyname(sServerName);

    if (!hostEntry) {
      DWORD e = WSAGetLastError();
      Log(log,u,_T("Unable to resolve host: %d"),e);
      return 0;
    }

    LPIN_ADDR pa = (LPIN_ADDR)(hostEntry->h_addr);
    Log(log,u,_T("Resolved to %d.%d.%d.%d..."),
        pa->S_un.S_un_b.s_b1,
        pa->S_un.S_un_b.s_b2,
        pa->S_un.S_un_b.s_b3,
        pa->S_un.S_un_b.s_b4);

    addr = pa->S_un.S_addr;
    if (addr == INADDR_NONE) {
      Log(log,u,_T("Failed to create sensible address!"));
      return 0;
    }
  } else {
    Log(log,u,_T("Address is an IP, skipping resolve..."));
  }

  sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

  if (sock==INVALID_SOCKET) {
    DWORD e = WSAGetLastError();
    Log(log,u,_T("Unable to create socket: %d"),e);
    return 0;
  }

  if (bSecure) {
    SSLVALIDATECERTHOOK hook;
    DWORD dwVal = SO_SEC_SSL;

    if(setsockopt(sock, SOL_SOCKET, SO_SECURE, (LPSTR)&dwVal, sizeof(DWORD)) != 0) {
      Log(log,u,_T("Unable to set SockOpt: %d"));
      return FALSE;
    }

    hook.HookFunc = SSLValidateCertHook;
    hook.pvArg = (PVOID) sock;

    if(WSAIoctl(sock,
                SO_SSL_SET_VALIDATE_CERT_HOOK,
                &hook, sizeof(SSLVALIDATECERTHOOK),
                NULL, 0, NULL, NULL, NULL))
    {
      Log(log,u,_T("WSAIoctl SO_SSL_SET_VALIDATE_CERT_HOOK failed: %d"),WSAGetLastError());
      return 0;
    }

/*
    dwFlags = SSL_FLAG_DEFER_HANDSHAKE;
    if(WSAIoctl(sock, SO_SSL_SET_FLAGS, &dwFlags, sizeof(DWORD), NULL, 0, NULL, NULL, NULL))
    {
      Log(log,u,_T("WSAIoctl SO_SSL_SET_FLAGS failed: %d"),WSAGetLastError());
      return 0;
    }
*/
  }
  SOCKADDR_IN serverInfo;
  ZeroMemory(&serverInfo,sizeof(SOCKADDR_IN));
  serverInfo.sin_family = AF_INET;
  serverInfo.sin_addr.S_un.S_addr = addr;
  serverInfo.sin_port = htons(port);

  if (connect(sock,(LPSOCKADDR)&serverInfo,sizeof(sockaddr))==SOCKET_ERROR) {
    DWORD e = WSAGetLastError();
    Log(log,u,_T("Unable to connect: %d"),e);
    return 0;
  }

  if (bSecure) {
    DWORD cbReturned;
    SSLCONNECTIONINFO ConnectionInfo;

    /*
    char szName[2];
        szName[0] = (char) GetCurrentThreadId();
        szName[1] = '\0';
        if(WSAIoctl(sock, SO_SSL_PERFORM_HANDSHAKE, szName, 2, NULL, 0, NULL, NULL, NULL))
        {
          Log(log,u,_T("WSAIoctl SO_SSL_PERFORM_HANDSHAKE failed: %d"),WSAGetLastError());
          return 0;
        }
    */
    
    if (WSAIoctl(sock, SO_SSL_GET_CONNECTION_INFO, NULL, 0, &ConnectionInfo, sizeof(ConnectionInfo), &cbReturned, NULL,NULL))
    {
      Log(log,u,_T("WSAIoctl SO_SSL_GET_CONNECTION_INFO failed: %d"),WSAGetLastError());
      return 0;
    }
#ifdef _DEBUG
    Log(log,u,_T("[ssl] Protocol: 0x%08x"),ConnectionInfo.dwProtocol);
    Log(log,u,_T("[ssl] Cipher: 0x%08x"), ConnectionInfo.aiCipher);
    Log(log,u,_T("[ssl] Cipher strength: %d"),ConnectionInfo.dwCipherStrength);
    Log(log,u,_T("[ssl] Hash: 0x%08x"), ConnectionInfo.aiHash);
    Log(log,u,_T("[ssl] Hash strength: %d"), ConnectionInfo.dwHashStrength);
    Log(log,u,_T("[ssl] Key exchange: 0x%08x"),ConnectionInfo.aiExch);
    Log(log,u,_T("[ssl] Key exchange strength: %d"),ConnectionInfo.dwExchStrength); 
#endif
  }
   
  return 1;
}

int zsIRC_Socket::Read( void * pData, unsigned int iLen)
{
  return recv(sock, (char*)pData, iLen, 0);
}

int zsIRC_Socket::Write( void * pData, unsigned int iLen)
{
  return send(sock, (char*)pData, iLen, 0);
}

int zsIRC_Socket::Disconnect() {
  if (sock) {
    closesocket(sock);
    WSACleanup();
  }
  sock=NULL;
  return 1;
}

int zsIRC_Socket::hasData()
{
  timeval timeout;
  timeout.tv_sec=0;
  timeout.tv_usec=0;
  
  fd_set conn;
  FD_ZERO(&conn);
  FD_SET(sock, &conn);

  return !(select(sock+1, &conn, NULL, NULL, &timeout)<=0);
}

int zsIRC_Socket::isConnected()
{
  return (sock!=NULL);
}

#if UNDER_CE >= 420
#pragma comment(lib,"cellcore.lib")
#endif

int zsIRC_Socket::InitializeGPRS(ZSIRC_LOGGER log, void * u) {
#if UNDER_CE >= 420
  CONNMGR_CONNECTIONINFO connInfo;
  
  ZeroMemory(&connInfo, sizeof(CONNMGR_CONNECTIONINFO));
  connInfo.cbSize      = sizeof(CONNMGR_CONNECTIONINFO);
  connInfo.dwParams    = CONNMGR_PARAM_GUIDDESTNET;
  connInfo.dwPriority  = CONNMGR_PRIORITY_HIPRIBKGND;
  connInfo.guidDestNet = IID_DestNetInternet;
  HANDLE hConnection  = NULL;
  
  DWORD dwStatus = 0;
  DWORD dwTimeout = 5000;

  if (FAILED(ConnMgrEstablishConnectionSync(&connInfo, &hConnection, dwTimeout, &dwStatus)))
  {
    DWORD e = WSAGetLastError();
    Log(log,u,_T("Unable to initialize GRPS connection: %d"), e);
    return 0;
  }
#endif

  return 1;
}