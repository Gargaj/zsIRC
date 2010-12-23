// zsIRC_Settings.h: interface for the zsIRC_Settings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZSIRC_SETTINGS_H__0D0C8207_E25E_4390_BC7A_9524E6905067__INCLUDED_)
#define AFX_ZSIRC_SETTINGS_H__0D0C8207_E25E_4390_BC7A_9524E6905067__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum {
  ZSIRC_ST_INT = 0,
  ZSIRC_ST_STRING,
  ZSIRC_ST_COLOR,
  ZSIRC_ST_CODEPAGE,
  ZSIRC_ST_BOOL,
} ZSIRC_SETTING_TYPE;

typedef struct {
  TCHAR * szName;
  void * pValue;
  ZSIRC_SETTING_TYPE nType;
} ZSIRC_SETTING;

typedef struct {
  int nCodepage;
  TCHAR * szName;
} ZSIRC_SETTING_CODEPAGE;

extern ZSIRC_SETTING g_pZsircSettings[];
extern ZSIRC_SETTING_CODEPAGE g_pZsircSettingsCodepages[];

//#define ZSIRC_NUMSETTINGS (sizeof(g_pZsircSettings)/sizeof(g_pZsircSettings[0]))
//#define ZSIRC_NUMCODEPAGES (sizeof(g_pZsircSettingsCodepages)/sizeof(g_pZsircSettingsCodepages[0]))

typedef enum {
  ZSIRC_SSLBEHAVIOUR_PROMPTUSER = 0,
  ZSIRC_SSLBEHAVIOUR_ALWAYSACCEPT,
  ZSIRC_SSLBEHAVIOUR_NEVERACCEPT,
  ZSIRC_SSLBEHAVIOUR_FORCEDWORD = 0x7fffffff,
} ZSIRC_SSLCERTIFICATE_BEHAVIOUR;

class zsIRC_Settings  
{
public:
	zsIRC_Settings();
	virtual ~zsIRC_Settings();

  void Load();
  void Save();

  //////////////////////////////////////////////////////////////////////////
  
  int nChannelBarHeight;
  int nNicklistWidth;

  ZSIRC_SSLCERTIFICATE_BEHAVIOUR nServerSSLBehaviour;

  TCHAR szButtonFont[1024];
  int nButtonFontSize;

  TCHAR szConversationFont[1024];
  int nConversationFontSize;

  TCHAR szNicklistFont[1024];
  int nNicklistFontSize;

  COLORREF cBackgroundColor;
  COLORREF cForegroundColor;

  int nShowHosts;
  int nDoTimestamp;
  int nDoLogTimestamp;
  int nDoLogging;
  int nShowDebugWindow;
  int nPopupNotify;
  int nPassCommandsRaw;
  int nStripColorCodes;
  int nCodepage;
  int nOffscreenBuffering;
  int nSortButtons;
  int nCheckForUpdates;

  TCHAR szServerName[1024];
  int nServerPort;
  TCHAR szServerPass[1024];
  int nServerSecure;

  TCHAR szLogDirectory[1024];

  TCHAR szDefaultNick[9][1024];
  TCHAR szIdent[1024];
  TCHAR szRealname[1024];
  TCHAR szQuitMessage[1024];
  TCHAR szNotifyList[1024];

  COLORREF cJoin;
  COLORREF cPart;
  COLORREF cMode;
  COLORREF cAction;
  COLORREF cNotice;

  int nAutoConnect;
  int nRejoinOnConnect;
};

extern zsIRC_Settings SETUP;

#endif // !defined(AFX_ZSIRC_SETTINGS_H__0D0C8207_E25E_4390_BC7A_9524E6905067__INCLUDED_)
