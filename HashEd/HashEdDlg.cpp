// HashEdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HashEd.h"
#include "HashEdDlg.h"
#include ".\hasheddlg.h"

#include <shlobj.h>
#include <tchar.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHashEdDlg dialog

CHashEdDlg::CHashEdDlg()
{
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = ::LoadIcon( (HINSTANCE)GetModuleHandle(NULL), (LPCTSTR)IDR_MAINFRAME);
}

BOOL CHashEdDlg::wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	CHashEdDlg* self;
	if( msg == WM_INITDIALOG )
	{
		::SetWindowLongPtr(hwnd, DWL_USER, lp);
		self = (CHashEdDlg*)lp;
		self->m_hWnd = hwnd;
		return self->OnInitDialog();
	}
	self = (CHashEdDlg*)::GetWindowLongPtr(hwnd, DWL_USER);
	if( !self )
		return FALSE;
	return self->wndProc( msg, wp, lp );
}

LRESULT CHashEdDlg::wndProc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch( msg )
	{
		case WM_COMMAND:
		{
			UINT id = LOWORD(wp);
			switch( HIWORD(wp) )
			{
			case EN_UPDATE:
				if( id == IDC_NEW_PHRASE_EDIT )
					OnUpdateNewPhraseEdit();
				break;
			case EN_CHANGE:
				if( id == IDC_NEW_PHRASE_EDIT )
					OnChangeNewPhraseEdit();
				break;
			case EN_KILLFOCUS:
				if( id == IDC_NEW_PHRASE_EDIT )
					OnKillfocusNewPhraseEdit();
				break;
			default:
				onCommand( id );
				break;
			}
		}
		case WM_TIMER:
			OnTimer( wp );
			break;
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CHashEdDlg message handlers

BOOL CHashEdDlg::OnInitDialog()
{
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)m_hIcon);			// Set big icon
	SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)m_hIcon);			// Set big icon

	m_btnAbout = GetDlgItem( m_hWnd, IDC_ABOUT );
	m_banner = GetDlgItem(  m_hWnd, IDC_BANNER );
	m_btnSaveAs = GetDlgItem(  m_hWnd, IDC_SAVE_AS );
	m_btnFindPhrase = GetDlgItem(  m_hWnd, IDC_FIND_PHRASE );
	m_listing = GetDlgItem(  m_hWnd, IDC_PHRASE_LIST );
	m_btnAddPhrase = GetDlgItem(  m_hWnd, IDC_ADD_PHRASE );
	m_btnDelPhrase = GetDlgItem(  m_hWnd, IDC_DEL_PHRASE );
	m_Import = GetDlgItem( m_hWnd, IDC_IMPORT );
	m_edtPhrase = GetDlgItem( m_hWnd, IDC_NEW_PHRASE_EDIT );
	m_btnSave = GetDlgItem( m_hWnd, IDC_SAVE );

	// TODO: Add extra initialization here
    _enable_buttons(FALSE);
    SetTimer(m_hWnd, 553, 10, NULL);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHashEdDlg::ReloadListCtrl()
{
	ShowWindow( m_listing, SW_HIDE );
    HASH_ITEM *pItem;
    int lop, count = m_context.get_phrase_count();

	ListView_DeleteAllItems( m_listing );
    for ( lop=0; lop<count; ++lop )
    {
        pItem = m_context.get_phrase_by_id(lop);
		LVITEM item = {0};
		item.iItem = lop;
		item.pszText = pItem->data.wordSeq;
		item.lParam = (LPARAM)pItem;
		item.mask = LVIF_TEXT|LVIF_PARAM;

		int i = ListView_InsertItem(m_listing, &item);
		i = i;
    }
	ShowWindow( m_listing, SW_SHOW );
}

void CHashEdDlg::OnUpdateNewPhraseEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
}

void CHashEdDlg::OnChangeNewPhraseEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

}

int CHashEdDlg::_isMatch(char *string, int id)
{
	LVITEM lv_item;
	lv_item.iItem = id;
	lv_item.mask = LVIF_PARAM;
	ListView_GetItem(m_listing, &lv_item);

	HASH_ITEM *pItem = (HASH_ITEM*) lv_item.lParam;
    return  strcmp(string, pItem->data.wordSeq);
}

int CHashEdDlg::find(char *tok, BOOL &bExactMatch, int hi, int lo)
{
    int p, cmp;
    
    p = ListView_GetItemCount(m_listing)-1;
    if ( hi==-1 )
        hi = p;
    if ( lo==-1 )
        lo=0;

    if ( lo<0 ) lo = 0;
    if ( hi>p ) hi = p;

    bExactMatch = FALSE;
    while ( hi>=lo )
    {
        p = (hi+lo)/2;
        cmp = _isMatch(tok, p);
        
        if ( cmp<0 )
            hi = p-1;
        else if ( cmp>0 )
            lo = p+1;
        else
        {
            bExactMatch = TRUE;
            return   p;
        }
    };

    return p;
}

void CHashEdDlg::SelItem(int idx) 
{
	ListView_SetItemState( m_listing, idx, LVIS_FOCUSED|LVIS_SELECTED, 
										   LVIS_FOCUSED|LVIS_SELECTED);
	ListView_EnsureVisible( m_listing, idx, FALSE);
    ListView_SetSelectionMark( m_listing, idx);
}

void CHashEdDlg::OnAddPhrase() 
{
	// TODO: Add your control notification handler code here
    HASH_ITEM *pItem;
	int	tt;
    BOOL bMatch;

    if ( strlen(m_string)==0 )    return;
	if ( CHashContext::_isDbcsString(m_string)==FALSE )
	{
		MessageBox(m_hWnd, GetStringFromTab(IDS_ALLOW_CHI_STRING),
                   NULL, MB_OK );
		return;
	}

	if ( (int)strlen(m_string)/2!=m_NumPhoneSeq )
	{
		MessageBox(m_hWnd, GetStringFromTab(IDS_INPUT_RULE),
                   NULL, MB_OK );
		return;
	}

    tt = find(m_string, bMatch);
    if ( bMatch==TRUE )
	{
        char strtemp[128];
        SelItem(tt);
        sprintf( strtemp, GetStringFromTab(IDS_PHRASE_EXIST), m_string);
		MessageBox(m_hWnd, strtemp, NULL, MB_OK);
		return;
	}

	pItem = m_context.append_phrase(m_string, m_PhoneSeq);

	LVITEM lv_item;
	lv_item.iItem = ListView_GetItemCount(m_listing);
	lv_item.pszText = m_string;
	lv_item.lParam = (LPARAM)pItem;
	lv_item.mask = LVIF_TEXT|LVIF_PARAM;

    tt = ListView_InsertItem( m_listing, &lv_item );
    ListView_SetItemState( m_listing, tt, LVIS_FOCUSED|LVIS_SELECTED,
										  LVIS_FOCUSED|LVIS_SELECTED);
	tt = ListView_EnsureVisible( m_listing, tt, FALSE);
    ListView_RedrawItems(m_listing, tt, tt);

    strcpy(m_string, "");
    m_NumPhoneSeq = 0;
    m_PhoneSeq[0] = 0;
    SetWindowText(m_edtPhrase, NULL);
    UpdateBanner();
    UpdateWindow( m_listing );
}

void CHashEdDlg::OnKillfocusNewPhraseEdit() 
{
	// TODO: Add your control notification handler code here
    GetWindowText(m_edtPhrase, m_string, sizeof(m_string));
    m_string[sizeof(m_string)-1] = '\0';
	m_NumPhoneSeq = m_context._get_phone_seq_from_server(m_PhoneSeq);
}



void CHashEdDlg::OnFindPhrase() 
{
	// TODO: Add your control notification handler code here
    char    tstring[(MAX_PHONE_SEQ_LEN+1)*2];
    int     idx, beep;
    BOOL    bMatch;

    GetWindowText(m_edtPhrase, tstring, sizeof(tstring));
    tstring[sizeof(tstring)-1] = '\0';
    if ( strlen(tstring)==0 )   return;
	if ( CHashContext::_isDbcsString(tstring)==FALSE )
	{
		MessageBox(m_hWnd, GetStringFromTab(IDS_ALLOW_CHI_STRING),
                   NULL, MB_OK);
		return;
	}
    
    idx = find(tstring, bMatch);

    beep = MB_ICONQUESTION;
    if ( bMatch==TRUE )
    {
        beep = MB_OK;
    }
    MessageBeep(beep);

    SelItem(idx);
    ListView_RedrawItems(m_listing, idx, idx);
    UpdateWindow( m_listing );
}

void CHashEdDlg::OnImport() 
{
	TCHAR file_name[MAX_PATH + 1] = "hash.dat";
	TCHAR file_title[MAX_PATH + 1] = "hash";
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.Flags = OFN_ENABLESIZING|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST;
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = (HINSTANCE)GetModuleHandle( NULL );
	ofn.lpstrFilter = "Chewing hash data file (*.dat)\0*.dat\0All files (*.*)\0*.*\0\0";
	ofn.lpstrDefExt = "dat";
	ofn.lpstrFile = file_name;
	ofn.nMaxFile = sizeof(file_name)/sizeof(TCHAR);
	ofn.lpstrFileTitle = file_title;
	ofn.nMaxFileTitle = sizeof(file_title)/sizeof(TCHAR);

    if ( !GetOpenFileName( &ofn ) ) 
    {
		DWORD err = CommDlgExtendedError ();
        return;
    }

    _enable_buttons(FALSE);
    Reload( file_name, false);

    _enable_buttons(TRUE);
    UpdateBanner();
}

void CHashEdDlg::Reload(char* hashfile, bool bClearContext)
{
    char strtemp[MAX_PATH + 100];
    int iret, iWarning = 0;

    sprintf( strtemp, GetStringFromTab(IDS_LOADING), hashfile);
    UpdateBanner( strtemp);

    if ( m_context.load_hash(hashfile, bClearContext)==-1 )
        iWarning = 1;

    m_context.sort_phrase();

    if ( m_context.arrange_phrase()==false )
        iWarning = 1;

    ReloadListCtrl();

    if ( iWarning!=0 )
        MessageBox(m_hWnd, GetStringFromTab(IDS_SUGGEST_DO_SAVE), 
		           NULL, MB_OK );

    UpdateBanner();
    UpdateWindow( m_listing );
}

void CHashEdDlg::GetHashLocation()
{
    LPITEMIDLIST pidl;
    strcpy( m_strHashFolder, "c:\\" );
	if( S_OK == SHGetSpecialFolderLocation( NULL, CSIDL_APPDATA, &pidl ) )
	{
		SHGetPathFromIDList(pidl, m_strHashFolder);
		_tcscat(m_strHashFolder, _T("\\Chewing") );
    }
}

void CHashEdDlg::OnSave() 
{
	// TODO: Add your control notification handler code here
     TCHAR strTemp[100];
	 TCHAR strpath[MAX_PATH+1];

    if ( MessageBox(m_hWnd, GetStringFromTab(IDS_SAVE_CONFIRM), 
		            NULL, MB_YESNO ) !=IDYES )
    {
        return;
    }
    //  ui...
    _enable_buttons(FALSE);
    GetWindowText( m_btnSave, strTemp, sizeof(strTemp) );
    SetWindowText( m_btnSave, GetStringFromTab(IDS_SAVE_BTN_FACE));

    //
    sprintf( strpath, "%s\\hash.dat", m_strHashFolder );
    _save( strpath , FALSE);
    
    Reload( strpath, true);

    //  ui...
    _enable_buttons(TRUE);
    SetWindowText( m_btnSave, strTemp );
}

void CHashEdDlg::OnDelPhrase() 
{
	// TODO: Add your control notification handler code here
    HASH_ITEM *pItem;
    int selItem;

	selItem = ListView_GetSelectionMark(m_listing);

	if ( selItem==-1 )  return;

	LVITEM lv_item;
	lv_item.iItem = selItem;
	lv_item.mask = LVIF_PARAM;
	ListView_GetItem( m_listing, &lv_item );
	pItem = (HASH_ITEM*) lv_item.lParam;

    ListView_DeleteItem(m_listing, selItem);
    m_context.del_phrase_by_id(pItem->item_index);

    UpdateWindow( m_listing );
    UpdateBanner();
}

void CHashEdDlg::_save(const char *pathfile, BOOL bSaveNoSwap)
{
    SYSTEMTIME syst;
    TCHAR strHashOld[MAX_PATH+1];
	TCHAR strTempfile[MAX_PATH+1];
	TCHAR strHashFile[MAX_PATH+1];
	TCHAR strTemp[MAX_PATH+1];

    sprintf( strTemp, "%s\\hash.dat", m_strHashFolder );
    GetLocalTime(&syst);
    //  locate hash.dat
    strcpy( strHashFile, pathfile );
    sprintf( strTempfile, "%s.new", pathfile );

    sprintf(strHashOld, "%s.%04d%02d%02d.%02d%02d%02d.bak",
        pathfile, syst.wYear, syst.wMonth, syst.wDay,
        syst.wHour, syst.wMinute, syst.wSecond);
    DeleteFile( strTempfile );

    //  prepare Hash_new.dat file
    m_context.sort_phrase();
    m_context.arrange_phrase();

    if ( bSaveNoSwap==TRUE &&
         strcmpi( strTemp, strHashFile ) != 0 )
    {
        m_context.save_hash( strHashFile );
        return;
    }

    m_context.save_hash( strTempfile );
    
    //  shutdown server
    m_context.shutdown_server();
    
    //  swap new & current file.
	MoveFile( strHashFile, strHashOld);
    MoveFile( strTempfile, strHashFile);
    
    //  launch server
    m_context._connect_server();
}

void CHashEdDlg::UpdateBanner(const char *message) 
{
    if ( message==NULL )
    {
        char banner[64];

        sprintf(banner, GetStringFromTab(IDS_NUM_OF_PHRASES), 
                m_context.get_phrase_count());

        SetWindowText(m_banner, banner);
    }
    else
        SetWindowText(m_banner, message);
}

void CHashEdDlg::OnSaveAs() 
{
	TCHAR file_name[MAX_PATH + 1] = "hash.dat";
	TCHAR file_title[MAX_PATH + 1] = "hash";
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.Flags = OFN_ENABLESIZING|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST;
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = (HINSTANCE)GetModuleHandle( NULL );
	ofn.lpstrFilter = "Chewing hash data file (*.dat)\0*.dat\0All files (*.*)\0*.*\0\0";
	ofn.lpstrDefExt = "dat";
	ofn.lpstrFile = file_name;
	ofn.nMaxFile = sizeof(file_name)/sizeof(TCHAR);
	ofn.lpstrFileTitle = file_title;
	ofn.nMaxFileTitle = sizeof(file_title)/sizeof(TCHAR);

    if ( !GetSaveFileName( &ofn ) ) 
    {
		DWORD err = CommDlgExtendedError ();
        return;
    }

    TCHAR strTemp[64];
    _enable_buttons(FALSE);
    GetWindowText( m_btnSaveAs, strTemp, sizeof(strTemp) );
    SetWindowText( m_btnSaveAs, "Saving..." );

    //
    _save( file_name, TRUE);
    
    //  ui...
    _enable_buttons(TRUE);
    SetWindowText( m_btnSaveAs, strTemp );
}

void CHashEdDlg::_enable_buttons(BOOL bEnable)
{
	EnableWindow(m_btnSaveAs, bEnable);
	EnableWindow(m_btnFindPhrase, bEnable);
	EnableWindow(m_btnAddPhrase, bEnable);
	EnableWindow(m_btnDelPhrase, bEnable);
	EnableWindow(m_Import, bEnable);
	EnableWindow(m_btnSave, bEnable);
    EnableWindow(m_btnAbout, bEnable);
}

static BOOL AboutDlgProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if( msg == WM_COMMAND )
	{
		EndDialog( hwnd, LOWORD(wp) );
		return TRUE;
	}
	return FALSE;
}


void CHashEdDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	DialogBox( (HINSTANCE)GetModuleHandle(NULL), LPCTSTR(IDD_ABOUTBOX), m_hWnd, (DLGPROC)AboutDlgProc );
}

void CHashEdDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    if ( nIDEvent==553 )
    {
        TCHAR strFile[MAX_PATH + 1];

        KillTimer(m_hWnd, 553);
        GetHashLocation();
        _stprintf( strFile, "%s\\hash.dat", m_strHashFolder );
        
        m_context._connect_server();
		Reload( strFile, true );

        _enable_buttons(TRUE);
    }
}

void CHashEdDlg::onCommand(UINT cmd)
{
	switch( cmd )
	{
	case IDC_ADD_PHRASE:
		OnAddPhrase();
		break;
	case IDC_FIND_PHRASE:
		OnFindPhrase();
		break;
	case IDC_IMPORT:
		OnImport();
		break;
	case IDC_SAVE:
		OnSave();
		break;
	case IDC_DEL_PHRASE:
		OnDelPhrase();
		break;
	case IDC_SAVE_AS:
		OnSaveAs();
		break;
	case IDC_ABOUT:
		OnAbout();
		break;
	case IDOK:
	case IDCANCEL:
		EndDialog( m_hWnd, cmd );
		break;
	}
}

char* CHashEdDlg::GetStringFromTab(int id)
{
    if ( LoadString(GetModuleHandle(NULL),
                    id, m_msgStr, sizeof(m_msgStr))==0 )
    {
        m_msgStr[0] = '\0';
    }
    return  m_msgStr;
}
