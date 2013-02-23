/**
*	@file	LogViewWindow.h
*	@brief	ログ表示クラス
*/
/**
	this file is part of Proxydomo
	Copyright (C) amate 2013-

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either
	version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#pragma once

#include "resource.h"
#include <vector>
#include <memory>
#include <atlcrack.h>
#include <atlctrls.h>
#include <atlframe.h>
#include <atlsync.h>
#include <atlddx.h>
#include "Log.h"

class CLogViewWindow : 
	public CDialogImpl<CLogViewWindow>, 
	public CDialogResize<CLogViewWindow>,
	public CWinDataExchange<CLogViewWindow>,
	public ILogTrace
{
public:
	enum { IDD = IDD_LOGVIEW };

	CLogViewWindow();
	~CLogViewWindow();

	void	ShowWindow();

	// ILogTrace
	virtual void ProxyEvent(LogProxyEvent Event, const IPv4Address& addr) override;
	virtual void HttpEvent(LogHttpEvent Event, const IPv4Address& addr, int RequestNumber, const std::string& text) override;

	BEGIN_DLGRESIZE_MAP( CLogViewWindow )
		DLGRESIZE_CONTROL( IDC_RICHEDIT_LOG, DLSZ_SIZE_X | DLSZ_SIZE_Y )
		DLGRESIZE_CONTROL( IDC_CHECKBOX_STOPLOG, DLSZ_MOVE_Y )
		DLGRESIZE_CONTROL( IDC_BUTTON_SHOWACTIVEREQUESTLOG, DLSZ_MOVE_Y )
		DLGRESIZE_CONTROL( IDC_BUTTON_CLEAR, DLSZ_MOVE_Y )

		DLGRESIZE_CONTROL( IDC_CHECKBOX_BROWSERTOPROXY, DLSZ_MOVE_Y )
		DLGRESIZE_CONTROL( IDC_CHECKBOX_PROXYTOWEB, DLSZ_MOVE_Y )
		DLGRESIZE_CONTROL( IDC_CHECKBOX_PROXYFROMWEB, DLSZ_MOVE_Y )
		DLGRESIZE_CONTROL( IDC_CHECKBOX_BROWSERFROMPROXY, DLSZ_MOVE_Y )
		DLGRESIZE_CONTROL( IDC_GROUP_HTTPEVENT, DLSZ_MOVE_Y )

		DLGRESIZE_CONTROL( IDC_CHECKBOX_PROXYEVENT, DLSZ_MOVE_Y )
		DLGRESIZE_CONTROL( IDC_CHECKBOX_FILTEREVENT, DLSZ_MOVE_Y )
		DLGRESIZE_CONTROL( IDC_CHECKBOX_WEBFILTERDEBUG, DLSZ_MOVE_Y )
	END_DLGRESIZE_MAP()

	BEGIN_DDX_MAP( CLogViewWindow )
		DDX_CHECK(IDC_CHECKBOX_STOPLOG		,	m_bStopLog)

		DDX_CHECK(IDC_CHECKBOX_BROWSERTOPROXY	,	m_bBrowserToProxy)
		DDX_CHECK(IDC_CHECKBOX_PROXYTOWEB		,	m_bProxyToWeb)
		DDX_CHECK(IDC_CHECKBOX_PROXYFROMWEB		,	m_bProxyFromWeb)
		DDX_CHECK(IDC_CHECKBOX_BROWSERFROMPROXY	,	m_bBrowserFromProxy)

		DDX_CHECK(IDC_CHECKBOX_PROXYEVENT	,	m_bProxyEvent)
		DDX_CHECK(IDC_CHECKBOX_FILTEREVENT	,	m_bFilterEvent)
		DDX_CHECK(IDC_CHECKBOX_WEBFILTERDEBUG,	m_bWebFilterDebug)

	END_DDX_MAP()

	BEGIN_MSG_MAP_EX( CLogViewWindow )
		MSG_WM_INITDIALOG( OnInitDialog )
		MSG_WM_DESTROY( OnDestroy )
		COMMAND_ID_HANDLER_EX( IDCANCEL, OnCancel )
		COMMAND_ID_HANDLER_EX( IDC_BUTTON_CLEAR, OnClear )
		COMMAND_ID_HANDLER_EX( IDC_BUTTON_SHOWACTIVEREQUESTLOG, OnShowActiveRequestLog )

		COMMAND_ID_HANDLER_EX( IDC_CHECKBOX_STOPLOG			, OnCheckBoxChanged )
		COMMAND_ID_HANDLER_EX( IDC_CHECKBOX_BROWSERTOPROXY	, OnCheckBoxChanged )
		COMMAND_ID_HANDLER_EX( IDC_CHECKBOX_PROXYTOWEB		, OnCheckBoxChanged )	
		COMMAND_ID_HANDLER_EX( IDC_CHECKBOX_PROXYFROMWEB	, OnCheckBoxChanged )	
		COMMAND_ID_HANDLER_EX( IDC_CHECKBOX_BROWSERFROMPROXY, OnCheckBoxChanged )	
		COMMAND_ID_HANDLER_EX( IDC_CHECKBOX_PROXYEVENT		, OnCheckBoxChanged )	
		COMMAND_ID_HANDLER_EX( IDC_CHECKBOX_FILTEREVENT		, OnCheckBoxChanged )	
		COMMAND_ID_HANDLER_EX( IDC_CHECKBOX_WEBFILTERDEBUG	, OnCheckBoxChanged )	
		CHAIN_MSG_MAP( CDialogResize<CLogViewWindow> )
	END_MSG_MAP()

	// void OnCommandIDHandlerEX(UINT uNotifyCode, int nID, CWindow wndCtl)

	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	void OnDestroy();
	void OnCancel(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnClear(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnShowActiveRequestLog(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnCheckBoxChanged(UINT uNotifyCode, int nID, CWindow wndCtl);

private:
	void	_AppendText(const CString& text, COLORREF textColor);
	void	_RefreshTitle();

	// Data members
	CRichEditCtrl	m_editLog;
	CCriticalSection	m_csLog;
	struct EventLog {
		uint16_t	port;
		CString		text;
		COLORREF	textColor;

		EventLog(uint16_t p, const CString& t, COLORREF tc) : port(p), text(t), textColor(tc) { }
	};
	std::vector<std::unique_ptr<EventLog>>	m_vecActiveRequestLog;
	CCriticalSection	m_csActiveRequestLog;

	bool	m_bStopLog;

	bool	m_bBrowserToProxy;
	bool	m_bProxyToWeb;
	bool	m_bProxyFromWeb;
	bool	m_bBrowserFromProxy;

	bool	m_bProxyEvent;
	bool	m_bFilterEvent;
	bool	m_bWebFilterDebug;
};

