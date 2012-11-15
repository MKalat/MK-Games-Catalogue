// MK Games Catalogue.cpp : Defines the entry point for the application.
//

/*
 This software is licensed under terms of GNU GPL v.2 License.
 Author : Marcin Ka³at
 email	: support@mkalat.pl
 www	: http://mkalat.pl


*/


#include "stdafx.h"
#include "MK Games Catalogue.h"
#include "db_def_all.h"
#include <vector>
#include <time.h>
#include <process.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <errno.h>
#include <shlobj.h>


#define MAX_LOADSTRING 100
#define MAX_MODS_REC 20
#define MAX_EXP_REC 20
#define SETT_FN TEXT("settings.cfg")
#define MAIN_FN TEXT("MKGC.mkgc")
#define MODS_FN TEXT("MKGC_M.mkgc")
#define EXP_FN TEXT("MKGC_E.mkgc")
#define LANG_PL TEXT("PL.ln")
#define LANG_EN TEXT("EN.ln")

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING] = TEXT("MK Games Catalogue");					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING] = TEXT("MKGAMES_DUMMY");			// the main window class name
unsigned long curPos = 0;
int search_sel_id = 0; // okreœla wyszukany rekord w wyszukiwarce, który ma zostaæ wyœwietlony w g³ownym oknie
MAIN_REC main_db;
MODS_REC mods_db;
EXPANSIONS_REC exp_db;
std::vector<MAIN_REC> main_wysz;
std::vector <MODS_REC> mods_db_arr;
std::vector <EXPANSIONS_REC> exp_db_arr;
SETTINGS_REC sett_db;
LANG_STRS_REC lang_db;
ATOM Dlg_tmp;
HWND MAIN_hWnd;
//HWND Tab_hwnd;
HWND TAB_KINFO;
HWND TAB_KODY;
HWND TAB_MODY;
HWND TAB_DODATKI;
HWND DLG_WYSZ;
bool new_mods_rec = false;
bool new_exp_rec = false;
bool new_main_rec = false;
wchar_t cur_db_path[1024];
wchar_t db_path[550] = TEXT("\\db\\");
wchar_t db_covers[550] = TEXT("covers\\");
wchar_t MAIN_FN_PATH[1000];
wchar_t EXP_FN_PATH[1000];
wchar_t MODS_FN_PATH[1000];
wchar_t COVERS_DIR[1000];
int edit_id = -1;

 

// Forward declarations of functions included in this code module:
ATOM					MyRegisterClass(HINSTANCE hInstance);
BOOL					InitInstance(HINSTANCE, int);
LRESULT CALLBACK		WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK		About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK		Settings(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK		Dlg_InfoKontWndProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK		Dlg_KodyWndProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK		Dlg_ModyWndProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK		Dlg_DodatkiWndProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK		EditMody_WndProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK		EditDodatki_WndProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK		MAIN_WND_PROC(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK		Dlg_Search_WndProc(HWND,UINT,WPARAM,LPARAM);


void				New_rec(); // podstawowe funkvj zwi¹zane z rekordami
void				Save_rec();
void				Del_rec();
int					Export_to_text_file(); // export i import db do plikó tekstowych
int					Import_from_text_file();
void				FirstRec(); // nawigacja po rekordach
void				PrevRec();
void				NextRec();
void				LastRec();
void				Load_pic(); // laduje obrazek
void				Del_pic(); // kasuje obrazek
int					SaveSettings(); // zapisuej ustawienia
int					ReadSettings(); // odczytuj ustawienia
int					SaveToFile(long); // niskopoziomowa funkcja do zapisu plików bazy danych
int					ReadFromFile(long); //niskopoziomowa funkcja do odczytu plików bazy danych
void				ReadRec(long); // odczytuje rekord w podanej posyzji w pliku
int					ApplySettings(HWND); // aplikuje ustawienia przed uruchomieniem okna w tym jêzykowe
void				UpdateRCTRLS(); // przepisuje zawartoœæ struktur i tablic struktur do kontrolek po odczytaniu pliku
void				UpdateWCTRLS(); // przepisuje zawartoœæ kontrolek do struktur i tablic struktur przed zapisem do pliku
long				SearchLastPos();
int					Find_Last_id();
void				Add_Mody();
void				Edit_Mody();
void				Prep_Edit_Mody();
void				Add_Dod();
void				Edit_Dod();
void				Prep_Edit_Dod(); 
int					GetLastIDMody(); // zwraca najwy¿sze id rekordu MODS_DB w pliku MODS_FN
void				InitializeCOMBOS(); // przygotowuje pola combo przed uruchomieniem aplikacji wype³niaj¹c je predefiniowanymi wartoœciami
void				ClearCTRLS(); // czyœci kontrolki przed np. dodaniem nowego rekordu
void				ClearARRS();	// czyœci tablice przed dodaniem nowego rekordu
void				ClearMDB();	// czyœci instancje struktury MAIN_REC, main_db, przed dodaniem nowego rekordu
void				Refresh_Mods();
void				Refresh_Dod();
void				Start_AU();
int					CheckDBExists(); // sprawdza czy istnieje baza danych
void				CreateDBFN(); // tworzy nowa pust¹ bazê danych
void				InitDBPpath(); // inicjuje œcie¿ke do bazy danych programu lokalna œciezk¹
void				LoadDBPATH(bool cust, wchar_t *cust_path); // ³aduje œcie¿kê do aktualnej bazy danych;
void				OpenDB();
void				Search_BTSZUKAJ();
int					GetLastIDDod(); // zwraca najwy¿sze id rekordu EXP_DB w pliku EXP_FN
void				Del_rec_ModsDB(int item);
void				Del_rec_ExpDB(int item);
void				ClearListCTRLS();
void				Refresh_SZUKAJ_LV();
int					BrowseMAINForID(int id);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	
	MSG msg;
	HACCEL hAccelTable;
	LoadLibrary(TEXT("Riched20.dll"));

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	Dlg_tmp = MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON8));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MKGAMESCATALOGUE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON8));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   
   DWORD le;
	HWND hWnd;
   hInst = hInstance; // Store instance handle in our global variable

  /* hWnd = CreateWindowW(szWindowClass,szTitle, WS_OVERLAPPEDWINDOW,0, 0, 495, 438, NULL, NULL, hInst, NULL);*/

   hWnd = CreateWindowW(szWindowClass,szTitle,WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,0,0,750,750,NULL,NULL,hInst,NULL);

	le = GetLastError();
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, i;
	/*PAINTSTRUCT ps;*/
	/*HBRUSH bt_brush;
	HFONT bts_font, std_font;
	LRESULT l_res;
	LPDRAWITEMSTRUCT pdis;*/
	//MAIN_hWnd = hWnd;
	
	RECT rcTab;
	
	switch (message)
	{
	case WM_CREATE:
		
		//Tab_hwnd = CreateWindow(WC_TABCONTROL,TEXT(""),WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,420,240,317,407,hWnd,NULL,hInst,NULL);
		MAIN_hWnd = CreateDialog(hInst,MAKEINTRESOURCE(100),hWnd,MAIN_WND_PROC);
		SetWindowPos(MAIN_hWnd,NULL,0,0,1000,900,SWP_SHOWWINDOW);
		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

				
       /* bt_brush = CreateSolidBrush(RGB(251,255,30));
		bts_font = CreateFont(0,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS
			,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,TEXT("MS Sans Serif"));*/

		/*ReadSettings();
		ApplySettings(hWnd);*/
		/*ReadRec(0); */// wczytaj pierwszy rekord
		//Start_AU()
		return 0;
		
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmEvent)
		{
		case 0:
			switch (wmId)
			{
			case ID_PLIK_USTAWIENIA:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_SETTINGS),hWnd, Settings);
				break;
			case ID_PLIK_NOWY:
				New_rec();
				break;
			case ID_PLIK_ZAPISZ:
				Save_rec();
				break;
			case ID_PLIK_USU32773:
				Del_rec();
				break;
			case ID_EKSPORTUJ_PLIKTEKSTOWY:
				Export_to_text_file();
				break;
			case ID_IMOPRTUJ_PLIKTEKSTOWY:
				Import_from_text_file();
				break;
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			case ID_PLIK_ZARZ32782:
				OpenDB();
				break;
			case ID_PLIK_WY:
				DialogBox(hInst,MAKEINTRESOURCE(IDD_SZUKAJ),hWnd,Dlg_Search_WndProc);
				break;
			default:
				break;
			}
		}
	break;

	case WM_DESTROY:
		/*DeleteObject(bt_brush);
		DeleteObject(bts_font);*/
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Settings(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	wchar_t buff[10];

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		//TODO : Ustawiæ nieedytowalne combosy days i lang

		ReadSettings();
		if (sett_db.enabled == 1)
		{
			Button_SetCheck(GetDlgItem(hDlg,10700),BST_CHECKED);
		}
		else if (sett_db.enabled == 0)
		{
			Button_SetCheck(GetDlgItem(hDlg,10700),BST_UNCHECKED);
		}

		if (sett_db.autosave == 1)
		{
			Button_SetCheck(GetDlgItem(hDlg,10703),BST_CHECKED);
		}
		else if (sett_db.autosave == 0)
		{
			Button_SetCheck(GetDlgItem(hDlg,10703),BST_UNCHECKED);
		}

		if (sett_db.conf_exit == 1)
		{
			Button_SetCheck(GetDlgItem(hDlg,10704),BST_CHECKED);
		}
		else if (sett_db.conf_exit == 0)
		{
			Button_SetCheck(GetDlgItem(hDlg,10704),BST_UNCHECKED);
		}
		_itow(sett_db.days,buff,10);
		SetWindowText(GetDlgItem(hDlg,10701),buff);
		SetWindowText(GetDlgItem(hDlg,10702),sett_db.language);

		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == 10705)
		{
			if (Button_GetCheck(GetDlgItem(hDlg,10700)) == BST_CHECKED)
			{
				sett_db.enabled = 1;
			}
			else if (Button_GetCheck(GetDlgItem(hDlg,10700)) == BST_UNCHECKED)
			{
				sett_db.enabled = 0;
			}
			
			GetWindowText(GetDlgItem(hDlg,10701),buff,10);
			sett_db.days = _wtoi(buff);
			GetWindowText(GetDlgItem(hDlg,10702),sett_db.language,10);
			
			
			if (Button_GetCheck(GetDlgItem(hDlg,10703)) == BST_CHECKED)
			{
				sett_db.autosave = 1;
			}
			else if (Button_GetCheck(GetDlgItem(hDlg,10703)) == BST_UNCHECKED)
			{
				sett_db.autosave = 0;
			}

			if (Button_GetCheck(GetDlgItem(hDlg,10704)) == BST_CHECKED)
			{
				sett_db.conf_exit = 1;
			}
			else if (Button_GetCheck(GetDlgItem(hDlg,10704)) == BST_UNCHECKED)
			{
				sett_db.conf_exit = 0;
			}

			SaveSettings();
			EndDialog(hDlg,LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == 10706)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return(INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int ReadSettings()
{
	FILE *cfg_fs;
	if ((cfg_fs = _tfopen(SETT_FN,TEXT("rb"))) != NULL)
	{
		fread(&sett_db, sizeof(sett_db),1,cfg_fs);
		fclose(cfg_fs);
		return 1;
	}
	else
	{
		cfg_fs = _tfopen(SETT_FN,TEXT("wb"));
		sett_db.autosave = 1;
		sett_db.conf_exit = 0;
		sett_db.days = 0;
		sett_db.enabled = 1;
		_tcscpy(sett_db.language,TEXT("PL"));

		fwrite(&sett_db,sizeof(sett_db),1,cfg_fs);
		fclose(cfg_fs);
		return 1;
	}
}

int SaveSettings()
{
	FILE *cfg_fs;
	int err = 0;
	if ((cfg_fs = _tfopen(SETT_FN,TEXT("wb"))) != NULL)
	{
		err = fwrite(&sett_db,sizeof(sett_db),1,cfg_fs);
		fclose(cfg_fs);
		if (err < 1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		cfg_fs = _tfopen(SETT_FN,TEXT("wb"));
		sett_db.autosave = 1;
		sett_db.conf_exit = 0;
		sett_db.days = 0;
		sett_db.enabled = 1;
		_tcscpy(sett_db.language,TEXT("PL"));

		err = fwrite(&sett_db,sizeof(sett_db),1,cfg_fs);
		fclose(cfg_fs);
		if (err < 1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

int ApplySettings(HWND hWnd)
{
	FILE *lang_fs;
	int err = 0;
	if (sett_db.language == TEXT("PL"))
	{
		if ((lang_fs = _tfopen(LANG_PL,TEXT("rb"))) != NULL)
		{
			// odczytaj i zmieñ zawartoœæ opisów
			fread(&lang_db,sizeof(lang_db),1,lang_fs);
			fclose(lang_fs);
			//EnumChildWindows(hWnd,CtlTranslate,NULL);

		}
		else
		{
			MessageBox(hWnd,_T("Nie znaleziono pliku jêzykowego. Language file doesn't exist."),_T("B³¹d / Error "),MB_OK);
			DestroyWindow(hWnd);
		}

	}
	else if (sett_db.language == TEXT("EN"))
	{
		if ((lang_fs = _tfopen(LANG_EN,TEXT("rb"))) != NULL)
		{
			fread(&lang_db, sizeof(lang_db),1,lang_fs);
			fclose(lang_fs);

		}
		else
		{
			MessageBox(hWnd,_T("Nie znaleziono pliku jêzykowego. Language file doesn't exist."),_T("B³¹d / Error "),MB_OK);
			DestroyWindow(hWnd);
		}

	}
	return 0;
}


void New_rec()
{
	new_main_rec = true;
	ClearMDB();
	ClearARRS();
	ClearCTRLS();
	main_db.ID = Find_Last_id() + 1;
	//Save_rec();
	


}
void Save_rec()
{
	if (CheckDBExists() == 0)
	{
		
		UpdateWCTRLS();
		
		if (SaveToFile(curPos) == 0)
		{
			ReadRec(curPos);
		}
		else
		{
			MessageBox(MAIN_hWnd,TEXT("Wystapi³ b³¹d podczas zapisu bazy danych !. Ecounetred error during DB write operation !."),TEXT("MK Games Catalogue"),MB_OK);
		}
	}


}
void Del_rec()
{
	main_db.ID = 0;
 	FILE *plik;
	plik = _tfopen(MAIN_FN_PATH,TEXT("r+b"));
	fseek(plik,curPos,SEEK_SET); 
	fwrite(&main_db,sizeof(struct MAIN_REC),1,plik);
	fclose(plik);
	
	wchar_t *buff = new wchar_t;
	_tcscpy(buff,cur_db_path);
	_tcscat(buff,TEXT("MKGC.gc0"));
	_trename(MAIN_FN_PATH,buff); 

	plik = _tfopen(MAIN_FN_PATH,TEXT("wb"));
	fclose(plik);
	FILE *src_file;
	src_file = _tfopen(buff,TEXT("rb"));
	plik = _tfopen(MAIN_FN_PATH,TEXT("a+b"));
	struct MAIN_REC main_buff;
	LONGLONG i;
	struct _stat status;
	LONGLONG stop = 0;
	if (_tstat(buff,&status))
	{
		stop = status.st_size;
	}
	for (i = 0; i < stop ; )
	{
		fseek(src_file,i,SEEK_SET);
		fread(&main_buff,sizeof(struct MAIN_REC),1,src_file);
		if (main_buff.ID == 0)
		{
			i = i + sizeof(struct MAIN_REC);
		}
		else
		{
			fwrite(&main_buff,sizeof(struct MAIN_REC),1,plik);
			i = i + sizeof(struct MAIN_REC);
		}
	}
	fclose(plik);
	fclose(src_file);
	_tremove(buff);
	

				


}

int	Export_to_text_file()
{
//TODO: napisaæ eksport do pliku tekstowego bazy danych

	return 0;
}

int	Import_from_text_file()
{
//TODO: Napisaæ import bazy danych z pliku tekstowego
	return 0;
}

void FirstRec()
{
	curPos = 0;
	ReadRec(curPos);

}

void PrevRec()
{
	if (curPos >= sizeof(main_db))
	{
		curPos = curPos - sizeof(main_db);
		ReadRec(curPos);
	}
	else
	{
		// do nothing
	}

}

void NextRec()
{
	curPos = curPos + sizeof(main_db);
	if (curPos <= SearchLastPos())
	{
		ReadRec(curPos);
	}
	else
	{
		curPos = 0;
		ReadRec(curPos);
	}
}

void LastRec()
{
	curPos = SearchLastPos();
	ReadRec(curPos);

}

void Load_pic()
{
	//Npsaæ ³adowanie zdjêcia ok³adki do bazy danych i na main form
	wchar_t filenam[1024];
	OPENFILENAME ofn;
	ofn.lpstrFile = filenam;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = MAIN_hWnd;
	ofn.lpstrFilter = _TEXT("Bitmapy\0*.bmp\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 1024;
	ofn.lpstrTitle = _TEXT("Podaj plik bitamy do wczytania do programu MK Games Catalogue");
	ofn.Flags = OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		wchar_t *buff = new wchar_t;
		_itow(main_db.ID,buff,10);
		wchar_t *dest = new wchar_t;
		_tcscpy(dest,COVERS_DIR);
		_tcscat(dest, buff);
		CopyFile(filenam,dest,FALSE);
		_tcscpy(main_db.pathtopic,dest);
		Save_rec();
	}
	
	
}

void Del_pic()
{
// Napisaæ usuwanie zdjêcia ok³adki z bazy danych i main form
	DeleteFile(main_db.pathtopic);
	_tcscpy(main_db.pathtopic,_TEXT(""));
	Save_rec();
	

}

void ReadRec(long pos)
{
	if (CheckDBExists() == 0)
	{
		ClearListCTRLS();
		ClearARRS();
		if (ReadFromFile(pos) == 0)
		{
			UpdateRCTRLS();
		}
		else
		{
			MessageBox(MAIN_hWnd,TEXT("Wystapi³ b³¹d podczas odczytu bazy danych !. Ecountered error during DB read operation !."),TEXT("MK Games Catalogue"),MB_OK);
		}
	}


}

int	ReadFromFile(long pos)
{
	struct _stat status;
	if (_wstat(MAIN_FN_PATH,&status) == 0)
	{
		if (status.st_size > 0)
		{
			FILE *fs_db;
			fs_db = _tfopen(MAIN_FN_PATH,TEXT("rb"));
			if (fs_db != NULL)
			{
				fseek(fs_db,pos,SEEK_SET);
				fread(&main_db,sizeof(main_db),1,fs_db);
				fclose(fs_db);
				
				FILE *exp_fs;
				struct _stat stop1;
				_wstat(EXP_FN_PATH,&stop1);
				exp_fs = _tfopen(EXP_FN_PATH,TEXT("rb"));
				if (exp_fs != NULL)
				{
					for (unsigned long offset = 0; offset < stop1.st_size ; )
					{
						fseek(exp_fs,offset,SEEK_SET);
						fread(&exp_db,sizeof(exp_db),1,exp_fs);
						if (exp_db.IDMAIN == main_db.ID)
						{
							if (exp_db.ID != -1)
							{
								exp_db_arr.push_back(exp_db);
							}
						}
						offset = offset + sizeof(exp_db);
					}
					fclose(exp_fs);
				}
							
				FILE *mod_fs;
				struct _stat stop2;
				_wstat(MODS_FN_PATH,&stop2);
				mod_fs = _tfopen(MODS_FN_PATH,TEXT("rb"));
				if (mod_fs != NULL)
				{
					for (unsigned long offset = 0; offset < stop2.st_size; )
					{
						fseek(mod_fs,offset,SEEK_SET);
						fread(&mods_db,sizeof(mods_db),1,mod_fs);
						if (mods_db.IDMAIN == main_db.ID)
						{
							if (mods_db.ID != -1)
							{
								mods_db_arr.push_back(mods_db);					
							}
						}

						offset = offset + sizeof(mods_db);
					}
					fclose(mod_fs);
				}

				return 0;
			}
			else
			{
				return -1;
			}
		}
	}
	else
	{
		return 0;
	}
	
}

int	SaveToFile(long pos)
{
	FILE *fs_db;
	LONGLONG stop = 0;
	if (new_main_rec == false)
	{
		fs_db = _tfopen(MAIN_FN_PATH,TEXT("r+b"));
		if (fs_db != NULL)
		{
			fseek(fs_db,pos,SEEK_SET);
			fwrite(&main_db,sizeof(main_db),1,fs_db);
			
			FILE *fs_mod;
			fs_mod = _tfopen(MODS_FN_PATH,TEXT("r+b"));
			if (fs_mod != NULL)
			{
				
				struct _stat status;
				if (_wstat(MODS_FN_PATH,&status))
				{
					stop = status.st_size;
				}

				MODS_REC mods_buff;
				for (int x = 0; x < mods_db_arr.size() ; x++)
				{
					if (main_db.ID == mods_db_arr[x].IDMAIN)
					{
						for (long offset = 0; offset < stop ; )		
						{
							fseek(fs_mod,offset,SEEK_SET);
							fread(&mods_buff,sizeof(mods_db),1,fs_mod);
							if (mods_db_arr[x].ID == mods_buff.ID)
							{
								fseek(fs_mod,offset,SEEK_SET);
								fwrite(&mods_db_arr[x],sizeof(mods_db),1,fs_mod);
							}
							offset = offset + sizeof(mods_db);

						}

					}
					
				}
				fclose(fs_mod);
			}
			
			FILE *fs_exp;
			fs_exp = _tfopen(EXP_FN_PATH,TEXT("r+b"));
			if (fs_exp != NULL)
			{
				stop = 0;
				struct _stat status;
				if (_wstat(MODS_FN_PATH,&status))
				{
					stop = status.st_size;
				}
				EXPANSIONS_REC exp_buff;
				for (int x = 0; x < exp_db_arr.size() ; x++)
				{
					if (main_db.ID == exp_db_arr[x].IDMAIN)
					{
						for (long offset = 0; offset < stop ; )
						{
							fseek(fs_exp,offset,SEEK_SET);
							fread(&exp_buff,sizeof(exp_db),1,fs_exp);
							if (exp_db_arr[x].ID == exp_buff.ID)
							{
								fseek(fs_exp,offset,SEEK_SET);
								fwrite(&exp_db_arr[x],sizeof(exp_db),1,fs_exp);
							}
							offset = offset + sizeof(exp_db);

						}

					}
					
				}
				fclose(fs_exp);
			}

			fclose(fs_db);
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		fs_db = _tfopen(MAIN_FN_PATH,TEXT("a+b"));
		if (fs_db != NULL)
		{
			fseek(fs_db,(SearchLastPos() + sizeof(main_db)),SEEK_SET);
			fwrite(&main_db,sizeof(main_db),1,fs_db);
			
			fclose(fs_db);
			new_main_rec = false;
			return 0;
		}
		else
		{
			new_main_rec = false;
			return -1;
		}
	}

	
}

void UpdateRCTRLS()
{
	// main_db - MAIN FORM
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_TYTUL),main_db.tytul);
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_PUBLISHER),main_db.publisher);
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_DISTRIBUTOR),main_db.distributor);
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_PRODUCER),main_db.producer);
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_COMBO_GENRE),main_db.genre);
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_PRICE),main_db.price);
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_NRKAT),main_db.nrkat);
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_COMBO_MEDIUM),main_db.medium);
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),main_db.platform);
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_COMBO_GRADE),main_db.grade);
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_WWW),main_db.WWW);
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_PUBDATE), main_db.pubdate);
	
	if (main_db.single == 1)
	{
		Button_SetCheck(GetDlgItem(MAIN_hWnd,1040),BST_CHECKED);
	}
	else if (main_db.single == 0)
	{
		Button_SetCheck(GetDlgItem(MAIN_hWnd,1040),BST_UNCHECKED);
	}

	if (main_db.multi == 1)
	{
		Button_SetCheck(GetDlgItem(MAIN_hWnd,1041),BST_CHECKED);
	}
	else if (main_db.multi == 0)
	{
		Button_SetCheck(GetDlgItem(MAIN_hWnd,1041),BST_UNCHECKED);
	}

	if (main_db.expansion == 1)
	{
		Button_SetCheck(GetDlgItem(MAIN_hWnd,1042),BST_CHECKED);
	}
	else if (main_db.expansion == 0)
	{
		Button_SetCheck(GetDlgItem(MAIN_hWnd,1042),BST_UNCHECKED);
	}

	// main_db - TAB_INFO

	SetWindowText(GetDlgItem(TAB_KINFO,10201),main_db.prod_www);
	SetWindowText(GetDlgItem(TAB_KINFO,10202),main_db.prod_email);
	SetWindowText(GetDlgItem(TAB_KINFO,10203),main_db.prod_adres);
	SetWindowText(GetDlgItem(TAB_KINFO,10204),main_db.prod_tel);

	SetWindowText(GetDlgItem(TAB_KINFO,10205),main_db.pub_www);
	SetWindowText(GetDlgItem(TAB_KINFO,10206),main_db.pub_email);
	SetWindowText(GetDlgItem(TAB_KINFO,10207),main_db.pub_adres);
	SetWindowText(GetDlgItem(TAB_KINFO,10208),main_db.pub_tel);

	SetWindowText(GetDlgItem(TAB_KINFO,10209),main_db.dist_www);
	SetWindowText(GetDlgItem(TAB_KINFO,10210),main_db.dist_email);
	SetWindowText(GetDlgItem(TAB_KINFO,10211),main_db.dist_adres);
	SetWindowText(GetDlgItem(TAB_KINFO,10212),main_db.dist_tel);

	// main_db - TAB_KODY

	SetWindowText(GetDlgItem(TAB_KODY,10102),main_db.cheats);

	// exp_db - TAB_EXPANSIONS
	Refresh_Dod();

	// mod_db - TAB_MODS
	Refresh_Mods();

	//Napisaæ ³adowanie ok³adek
	struct _stat status;
	if (_tcscmp(main_db.pathtopic,_TEXT("")) != 0)
	{
		if (_wstat(main_db.pathtopic,&status) == 0)
		{
			HANDLE okladka;
			okladka = LoadImage(NULL,main_db.pathtopic,IMAGE_BITMAP,119,139,LR_LOADFROMFILE);
			SendMessage(GetDlgItem(MAIN_hWnd,1031),STM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)okladka);
			
		}
	}
	else
	{
		//do nothing
		
	}
	

}

void UpdateWCTRLS()
{
	GetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_TYTUL),main_db.tytul,500);
	GetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_PUBLISHER),main_db.publisher,500);
	GetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_DISTRIBUTOR),main_db.distributor,500);
	GetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_PRODUCER),main_db.producer,500);
	GetWindowText(GetDlgItem(MAIN_hWnd,IDC_COMBO_GENRE),main_db.genre,250);
	GetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_PRICE),main_db.price,100);
	GetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_NRKAT),main_db.nrkat,250);
	GetWindowText(GetDlgItem(MAIN_hWnd,IDC_COMBO_MEDIUM),main_db.medium,250);
	GetWindowText(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),main_db.platform,250);
	GetWindowText(GetDlgItem(MAIN_hWnd,IDC_COMBO_GRADE),main_db.grade,100);
	GetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_WWW),main_db.WWW,500);
	GetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_PUBDATE), main_db.pubdate,50);

	if (Button_GetCheck(GetDlgItem(MAIN_hWnd,1040)) == BST_CHECKED)
	{
		main_db.single = 1;
	}
	else if (Button_GetCheck(GetDlgItem(MAIN_hWnd,1040)) == BST_UNCHECKED)
	{
		main_db.single = 0;
	}
	
	if (Button_GetCheck(GetDlgItem(MAIN_hWnd,1041)) == BST_CHECKED)
	{
		main_db.multi = 1;
	}
	else if (Button_GetCheck(GetDlgItem(MAIN_hWnd,1041)) == BST_UNCHECKED)
	{
		main_db.multi = 0;
	}

	if (Button_GetCheck(GetDlgItem(MAIN_hWnd,1042)) == BST_CHECKED)
	{
		main_db.expansion = 1;
	}
	else if (Button_GetCheck(GetDlgItem(MAIN_hWnd,1042)) == BST_UNCHECKED)
	{
		main_db.expansion = 0;
	}

	// main_db - TAB_INFO

	GetWindowText(GetDlgItem(TAB_KINFO,10201),main_db.prod_www,500);
	GetWindowText(GetDlgItem(TAB_KINFO,10202),main_db.prod_email,500);
	GetWindowText(GetDlgItem(TAB_KINFO,10203),main_db.prod_adres,500);
	GetWindowText(GetDlgItem(TAB_KINFO,10204),main_db.prod_tel,150);

	GetWindowText(GetDlgItem(TAB_KINFO,10205),main_db.pub_www,500);
	GetWindowText(GetDlgItem(TAB_KINFO,10206),main_db.pub_email,500);
	GetWindowText(GetDlgItem(TAB_KINFO,10207),main_db.pub_adres,500);
	GetWindowText(GetDlgItem(TAB_KINFO,10208),main_db.pub_tel,150);

	GetWindowText(GetDlgItem(TAB_KINFO,10209),main_db.dist_www,500);
	GetWindowText(GetDlgItem(TAB_KINFO,10210),main_db.dist_email,500);
	GetWindowText(GetDlgItem(TAB_KINFO,10211),main_db.dist_adres,500);
	GetWindowText(GetDlgItem(TAB_KINFO,10212),main_db.dist_tel,150);

	// main_db - TAB_KODY

	GetWindowText(GetDlgItem(TAB_KODY,10102),main_db.cheats,2048);

	// ListView-y sa update-owane przy zmianach zawartoœci kontrolek, i tak samo tablice struktur.
	

}



INT_PTR CALLBACK Dlg_InfoKontWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	TAB_KINFO = hWnd;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	//case WM_COMMAND:
	//	if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
	//	{
	//		EndDialog(hDlg, LOWORD(wParam));
	//		return (INT_PTR)TRUE;
	//	}
	//	break;
	}
	return (INT_PTR)FALSE;

}

INT_PTR CALLBACK Dlg_KodyWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	TAB_KODY = hWnd;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	/*case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;*/
	}
	return (INT_PTR)FALSE;


}

INT_PTR CALLBACK Dlg_ModyWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	TAB_MODY = hWnd;
	int wmId, wmEvent;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		// init tab cols
		HWND LV_hWnd;
		LV_hWnd = GetDlgItem(hWnd,10400);
		LVCOLUMN lv_col;
		lv_col.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lv_col.iSubItem = 0;
		lv_col.pszText = TEXT("Nazwa");
		lv_col.cx = 50;
		ListView_InsertColumn(LV_hWnd,0,&lv_col);

		lv_col.iSubItem = 1;
		lv_col.pszText = TEXT("Wersja gry");
		lv_col.cx = 50;
		ListView_InsertColumn(LV_hWnd,1,&lv_col);

		lv_col.iSubItem = 2;
		lv_col.pszText = TEXT("Wersja moda");
		lv_col.cx = 50;
		ListView_InsertColumn(LV_hWnd,2,&lv_col);

		lv_col.iSubItem = 3;
		lv_col.pszText = TEXT("Ocena");
		lv_col.cx = 50;
		ListView_InsertColumn(LV_hWnd,3,&lv_col);

		lv_col.iSubItem = 4;
		lv_col.pszText = TEXT("WWW");
		lv_col.cx = 50;
		ListView_InsertColumn(LV_hWnd,4,&lv_col);

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmEvent)
		{
		case 0:
			switch (wmId)
			{
			case 10401:
				// prepare struct for newly created list view item, id dlg_box it will be filled with data
				// then in Add_Mody data will be added to array and to the control via getdispinfo
				mods_db.IDMAIN = main_db.ID;
				mods_db.ID = GetLastIDMody() + 1;
				new_mods_rec = true;
				if (DialogBox(hInst, MAKEINTRESOURCE(134), hWnd, EditMody_WndProc) == 10505)
				{
					Add_Mody();
					//Save_rec();
					
				}
				break;
			case 10402:
				// get selected item on list view control, get its data and copy to tmp struct, thaen operate
				// on this struct in dlg_box and then copy back its contents to array and to control via getdispinfo
				Prep_Edit_Mody();
				if (DialogBox(hInst,MAKEINTRESOURCE(134),hWnd,EditMody_WndProc) == 10505)
				{
					Edit_Mody();
					//Save_rec();
				}
				break;
			case 10403:
				//get selected item and delete it from array and list view control via getdispinfo
				int item;
				item = ListView_GetNextItem(GetDlgItem(hWnd,10400),-1,LVIS_SELECTED);
				if (item != -1)
				{
					Save_rec();
					Del_rec_ModsDB(item);
					ReadRec(curPos);
					Refresh_Mods();
				}
				break;
			default:
				break;

			}
			default:
			break;
		}
		return (INT_PTR)TRUE;
		
		break;
	case WM_NOTIFY:
		switch (((LPNMHDR) lParam)->code)
        {
        case LVN_GETDISPINFO:
            {
                NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lParam; 
				int x;
				x = plvdi->item.iItem;  

                switch (plvdi->item.iSubItem)
                {
                case 0:
                    plvdi->item.pszText = mods_db_arr[x].nazwa;
                    break;

                case 1:
					plvdi->item.pszText = mods_db_arr[x].wersja_gry;
                    break;

                case 2:
					plvdi->item.pszText = mods_db_arr[x].wersja_moda;
                    break;

                case 3:
					plvdi->item.pszText = mods_db_arr[x].ocena;
                    break;

				case 4:
					plvdi->item.pszText = mods_db_arr[x].www;
                    break;

                default:
                    break;
                }
                return TRUE;
            }
      // More notifications...
      }
	}
	return (INT_PTR)FALSE;

}

INT_PTR CALLBACK Dlg_DodatkiWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	TAB_DODATKI = hWnd;
	int wmId, wmEvent;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		HWND LV_hWnd;
		LV_hWnd = GetDlgItem(hWnd,10300);
		LVCOLUMN lv_col;
		lv_col.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lv_col.iSubItem = 0;
		lv_col.pszText = TEXT("Nazwa");
		lv_col.cx = 50;
		ListView_InsertColumn(LV_hWnd,0,&lv_col);

		lv_col.iSubItem = 1;
		lv_col.pszText = TEXT("Cena");
		lv_col.cx = 50;
		ListView_InsertColumn(LV_hWnd,1,&lv_col);

		lv_col.iSubItem = 2;
		lv_col.pszText = TEXT("Multi");
		lv_col.cx = 50;
		ListView_InsertColumn(LV_hWnd,2,&lv_col);

		lv_col.iSubItem = 3;
		lv_col.pszText = TEXT("Single");
		lv_col.cx = 50;
		ListView_InsertColumn(LV_hWnd,3,&lv_col);

		lv_col.iSubItem = 4;
		lv_col.pszText = TEXT("WWW");
		lv_col.cx = 50;
		ListView_InsertColumn(LV_hWnd,4,&lv_col);

		lv_col.iSubItem = 5;
		lv_col.pszText = TEXT("Ocena");
		lv_col.cx = 50;
		ListView_InsertColumn(LV_hWnd,5,&lv_col);


		return (INT_PTR)TRUE;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmEvent)
		{
		case 0:
			switch (wmId)
			{
			case 10301:
				// prepare struct for newly created list view item, id dlg_box it will be filled with data
				// then in Add_Mody data will be added to array and to the control via getdispinfo
				exp_db.IDMAIN = main_db.ID;
				exp_db.ID = GetLastIDDod() + 1;
				new_exp_rec = true;
				if (DialogBox(hInst, MAKEINTRESOURCE(135), hWnd, EditDodatki_WndProc) == 10607)
				{
					Add_Dod();
					//Save_rec();
					
				}
				break;
			case 10302:
				// get selected item on list view control, get its data and copy to tmp struct, thaen operate
				// on this struct in dlg_box and then copy back its contents to array and to control via getdispinfo
				Prep_Edit_Dod();
				if (DialogBox(hInst,MAKEINTRESOURCE(135),hWnd,EditDodatki_WndProc) == 10607)
				{
					Edit_Dod();
					//Save_rec();
				}
				break;
			case 10303:
				//get selected item and delete it from array and list view control via getdispinfo
				int item;
				item = ListView_GetNextItem(GetDlgItem(hWnd,10300),-1,LVIS_SELECTED);
				if (item != -1)
				{
					Save_rec();
					Del_rec_ExpDB(item);
					ReadRec(curPos);
					Refresh_Dod();
				}
				break;
			default:
				break;

			}
			default:
			break;
		}
		return (INT_PTR)TRUE;
		
		break;
	case WM_NOTIFY:
		switch (((LPNMHDR) lParam)->code)
        {
        case LVN_GETDISPINFO:
            {
                NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lParam; 
				int x;
				x = plvdi->item.iItem;  

                switch (plvdi->item.iSubItem)
                {
                case 0:
                    plvdi->item.pszText = exp_db_arr[x].nazwa;
                    break;

                case 1:
					plvdi->item.pszText = exp_db_arr[x].cena;
                    break;

                case 2:
					plvdi->item.pszText = exp_db_arr[x].multi;
                    break;

                case 3:
					plvdi->item.pszText = exp_db_arr[x].single;
                    break;

				case 4:
					plvdi->item.pszText = exp_db_arr[x].www;
                    break;

				case 5:
					plvdi->item.pszText = exp_db_arr[x].ocena;
                    break;

                default:
                    break;
                }
                return TRUE;
            }
      // More notifications...
      }
	}
	return (INT_PTR)FALSE;

}

INT_PTR CALLBACK EditMody_WndProc(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
{
	
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		if (edit_id != -1)
		{
			SetWindowText(GetDlgItem(hWnd,10500),mods_db_arr[edit_id].nazwa);
			SetWindowText(GetDlgItem(hWnd,10501),mods_db_arr[edit_id].wersja_gry);
			SetWindowText(GetDlgItem(hWnd,10502),mods_db_arr[edit_id].wersja_moda);
			SetWindowText(GetDlgItem(hWnd,10503),mods_db_arr[edit_id].ocena);
			SetWindowText(GetDlgItem(hWnd,10504),mods_db_arr[edit_id].www);

		}
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == 10505 || LOWORD(wParam) == 10506)
		{
			if (LOWORD(wParam) == 10505)
			{
				GetWindowText(GetDlgItem(hWnd,10500),mods_db.nazwa,500);
				GetWindowText(GetDlgItem(hWnd,10501),mods_db.wersja_gry,50);
				GetWindowText(GetDlgItem(hWnd,10502),mods_db.wersja_moda,50);
				GetWindowText(GetDlgItem(hWnd,10503),mods_db.ocena,50);
				GetWindowText(GetDlgItem(hWnd,10504),mods_db.www,250);

			}
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;

	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK EditDodatki_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		if (edit_id != -1)
		{
			SetWindowText(GetDlgItem(hWnd,10600),exp_db_arr[edit_id].nazwa);
			SetWindowText(GetDlgItem(hWnd,10601),exp_db_arr[edit_id].cena);
			SetWindowText(GetDlgItem(hWnd,10602),exp_db_arr[edit_id].www);
			SetWindowText(GetDlgItem(hWnd,10603),exp_db_arr[edit_id].ocena);
			SetWindowText(GetDlgItem(hWnd,10605),exp_db_arr[edit_id].multi);
			SetWindowText(GetDlgItem(hWnd,10606),exp_db_arr[edit_id].single);

		}

		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == 10607 || LOWORD(wParam) == 10608)
		{
			if (LOWORD(wParam) == 10607)
			{
				GetWindowText(GetDlgItem(hWnd,10600),exp_db.nazwa,500);
				GetWindowText(GetDlgItem(hWnd,10601),exp_db.cena,50);
				GetWindowText(GetDlgItem(hWnd,10602),exp_db.www,250);
				GetWindowText(GetDlgItem(hWnd,10603),exp_db.ocena,50);
				GetWindowText(GetDlgItem(hWnd,10605),exp_db.multi,50);
				GetWindowText(GetDlgItem(hWnd,10606),exp_db.single,50);

			}
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;

	}
	return (INT_PTR)FALSE;

}
int GetLastIDMody()
{
	struct _stat status;
	if (_wstat(MODS_FN_PATH,&status) == 0)
	{
		if (status.st_size > 0)
		{
			int ret_id = 0;
			FILE *fs_mods;
			fs_mods = _tfopen(MODS_FN_PATH,TEXT("rb"));
			for (LONGLONG offset = 0; offset < status.st_size; )
			{
				fseek(fs_mods,offset,SEEK_SET);
				fread(&mods_db,sizeof(mods_db),1,fs_mods);
				if (ret_id < mods_db.ID)
				{
					ret_id = mods_db.ID;
				}
				offset = offset + sizeof(mods_db);
			}
			return ret_id;
		}
		else
		{
			return 0;
		}

	}
	else 
	{
		return 0;
	}
}
void Prep_Edit_Mody()
{
	for (int x = 0; x < mods_db_arr.size(); x++)
	{
		if (SendMessage(GetDlgItem(TAB_MODY,10400),LVM_GETITEMSTATE,(WPARAM)x,(LPARAM)(UINT)LVIS_SELECTED) == LVIS_SELECTED)
		{
			edit_id = x;
			return;
		}

	}

}

void Edit_Mody()
{
	_tcscpy(mods_db_arr[edit_id].nazwa,mods_db.nazwa);
	_tcscpy(mods_db_arr[edit_id].ocena,mods_db.ocena);
	_tcscpy(mods_db_arr[edit_id].wersja_gry,mods_db.wersja_gry);
	_tcscpy(mods_db_arr[edit_id].wersja_moda,mods_db.wersja_moda);
	_tcscpy(mods_db_arr[edit_id].www,mods_db.www);
	Refresh_Mods();
	edit_id = -1;
}

void Prep_Edit_Dod()
{
	for (int x = 0; x < exp_db_arr.size(); x++)
	{
		if (SendMessage(GetDlgItem(TAB_DODATKI,10300),LVM_GETITEMSTATE,(WPARAM)x,(LPARAM)(UINT)LVIS_SELECTED) == LVIS_SELECTED)
		{
			edit_id = x;
			return;
		}

	}
}

void Add_Dod()
{
	if (CheckDBExists() == 0)
	{
		exp_db_arr.push_back(exp_db);
		Refresh_Dod();
		FILE *fs_exp;
		fs_exp = _tfopen(EXP_FN_PATH,TEXT("a+b"));
		fwrite(&exp_db,sizeof(exp_db),1,fs_exp);
		fclose(fs_exp);
	}
	new_exp_rec = false;

}

void Add_Mody()
{
	if (CheckDBExists() == 0)
	{
		mods_db_arr.push_back(mods_db);
		Refresh_Mods();
		FILE *fs_mods;
		fs_mods = _tfopen(MODS_FN_PATH,TEXT("a+b"));
		fwrite(&mods_db,sizeof(mods_db),1,fs_mods);
		fclose(fs_mods);
	}
	new_mods_rec = false;
}

void Edit_Dod()
{
	_tcscpy(exp_db_arr[edit_id].nazwa,exp_db.nazwa);
	_tcscpy(exp_db_arr[edit_id].cena,exp_db.cena);
	_tcscpy(exp_db_arr[edit_id].multi,exp_db.multi);
	_tcscpy(exp_db_arr[edit_id].ocena,exp_db.ocena);
	_tcscpy(exp_db_arr[edit_id].single,exp_db.single);
	_tcscpy(exp_db_arr[edit_id].www,exp_db.www);
	Refresh_Dod();
	edit_id = -1;
}

long SearchLastPos()
{
	long offset = 0;
	MAIN_REC main_buff;

	FILE *fs_db;
	fs_db = _tfopen(MAIN_FN_PATH,TEXT("rb"));
	if (fs_db != NULL)
	{
		while (!(feof(fs_db)))
		{
			fseek(fs_db,offset,SEEK_SET);
			fread(&main_buff,sizeof(main_buff),1,fs_db);

			offset = ftell(fs_db);
		}
		offset = offset - sizeof(main_buff);

		fclose(fs_db);
		return offset;
	}
	else
	{
		return -1;
	}
	
}

void InitializeCOMBOS()
{
	// Combo Gatunek
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GENRE),TEXT("Akcja"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GENRE),TEXT("Przygodowa"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GENRE),TEXT("RPG"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GENRE),TEXT("FPS"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GENRE),TEXT("MMO"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GENRE),TEXT("RTS"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GENRE),TEXT("Grand Strategy"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GENRE),TEXT("Strategia Turowa"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GENRE),TEXT("Horror"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GENRE),TEXT("Tycoon"));
	
	//Combo Medium
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_MEDIUM),TEXT("DVD"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_MEDIUM),TEXT("CD"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_MEDIUM),TEXT("DLC"));

	//combo platform
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("Windows 95/98/Me"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("DOS"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("Windows 2000"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("Windows XP"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("Windows 2000/XP"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("Windows Vista"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("Windows 7"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("Windows Vista/7"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("Windows 2000/XP/Vista/7"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("Windows 95/98"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("Windows 95/98/Me/2000/XP"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("Windows 95/98/Me/2000/XP/Vista/7"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("Linux"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("MacOS"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("Wii"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("XBOX"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("PlayStation"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT("NDS"));

	//combo grade
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GRADE),TEXT("1/10"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GRADE),TEXT("2/10"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GRADE),TEXT("3/10"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GRADE),TEXT("4/10"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GRADE),TEXT("5/10"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GRADE),TEXT("6/10"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GRADE),TEXT("7/10"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GRADE),TEXT("8/10"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GRADE),TEXT("9/10"));
	ComboBox_AddString(GetDlgItem(MAIN_hWnd,IDC_COMBO_GRADE),TEXT("10/10"));

}

int Find_Last_id()
{
	struct _stat status;
	if (_wstat(MAIN_FN_PATH,&status) == 0)
	{
		if (status.st_size > 0)
		{
			int L_ID = 0;
			long offset = 0;
			MAIN_REC main_buff;
			FILE *fs_db;
			fs_db = _tfopen(MAIN_FN_PATH,TEXT("rb"));
			if (fs_db != NULL)
			{
				while (!(feof(fs_db)))
				{
					fseek(fs_db,offset,SEEK_SET);
					fread(&main_buff,sizeof(main_buff),1,fs_db);
					if (main_buff.ID > L_ID)
					{
						L_ID = main_buff.ID;

					}
					offset = offset + sizeof(main_buff);
				}
				fclose(fs_db);
				return L_ID;
			}
			else
			{

				return -1;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}


}

void ClearARRS()
{
	mods_db_arr.resize(0);
	exp_db_arr.resize(0);
}

void ClearMDB()
{
	main_db.ID = 0;
	_tcscpy(main_db.cheats,TEXT(""));
	_tcscpy(main_db.desc,TEXT(""));
	_tcscpy(main_db.dist_adres,TEXT(""));
	_tcscpy(main_db.dist_email,TEXT(""));
	_tcscpy(main_db.dist_tel,TEXT(""));
	_tcscpy(main_db.dist_www,TEXT(""));
	_tcscpy(main_db.distributor,TEXT(""));
	main_db.expansion = 0;
	_tcscpy(main_db.genre,TEXT(""));
	_tcscpy(main_db.grade,TEXT(""));
	_tcscpy(main_db.medium,TEXT(""));
	main_db.multi = 0;
	_tcscpy(main_db.net_grade,TEXT(""));
	_tcscpy(main_db.nrkat,TEXT(""));
	_tcscpy(main_db.pathtopic,TEXT(""));
	_tcscpy(main_db.platform,TEXT(""));
	_tcscpy(main_db.price,TEXT(""));
	_tcscpy(main_db.prod_adres,TEXT(""));
	_tcscpy(main_db.prod_email,TEXT(""));
	_tcscpy(main_db.prod_tel,TEXT(""));
	_tcscpy(main_db.prod_www,TEXT(""));
	_tcscpy(main_db.producer,TEXT(""));
	_tcscpy(main_db.pub_adres,TEXT(""));
	_tcscpy(main_db.pub_email,TEXT(""));
	_tcscpy(main_db.pub_tel,TEXT(""));
	_tcscpy(main_db.pub_www,TEXT(""));
	_tcscpy(main_db.pubdate,TEXT(""));
	_tcscpy(main_db.publisher,TEXT(""));
	main_db.single = 0;
	_tcscpy(main_db.tytul,TEXT(""));
	_tcscpy(main_db.WWW,TEXT(""));

}

void ClearCTRLS()
{

	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_TYTUL),TEXT(""));
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_PUBLISHER),TEXT(""));
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_DISTRIBUTOR),TEXT(""));
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_PRODUCER),TEXT(""));
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_COMBO_GENRE),TEXT(""));
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_PRICE),TEXT(""));
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_NRKAT),TEXT(""));
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_COMBO_MEDIUM),TEXT(""));
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_COMBO_PLATFORM),TEXT(""));
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_COMBO_GRADE),TEXT(""));
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_WWW),TEXT(""));
	SetWindowText(GetDlgItem(MAIN_hWnd,IDC_EDIT_PUBDATE), TEXT(""));

	Button_SetCheck(GetDlgItem(MAIN_hWnd,1040),BST_UNCHECKED);
	Button_SetCheck(GetDlgItem(MAIN_hWnd,1041),BST_UNCHECKED);
	Button_SetCheck(GetDlgItem(MAIN_hWnd,1042),BST_UNCHECKED);

	SetWindowText(GetDlgItem(TAB_KINFO,10201),TEXT(""));
	SetWindowText(GetDlgItem(TAB_KINFO,10202),TEXT(""));
	SetWindowText(GetDlgItem(TAB_KINFO,10203),TEXT(""));
	SetWindowText(GetDlgItem(TAB_KINFO,10204),TEXT(""));

	SetWindowText(GetDlgItem(TAB_KINFO,10205),TEXT(""));
	SetWindowText(GetDlgItem(TAB_KINFO,10206),TEXT(""));
	SetWindowText(GetDlgItem(TAB_KINFO,10207),TEXT(""));
	SetWindowText(GetDlgItem(TAB_KINFO,10208),TEXT(""));

	SetWindowText(GetDlgItem(TAB_KINFO,10209),TEXT(""));
	SetWindowText(GetDlgItem(TAB_KINFO,10210),TEXT(""));
	SetWindowText(GetDlgItem(TAB_KINFO,10211),TEXT(""));
	SetWindowText(GetDlgItem(TAB_KINFO,10212),TEXT(""));

	SetWindowText(GetDlgItem(TAB_KODY,10102),TEXT(""));

	Refresh_Mods();
	Refresh_Dod();



}
void Refresh_Mods()
{
	ListView_DeleteAllItems(GetDlgItem(TAB_MODY,10400));
	for (int x=0; x < mods_db_arr.size() ; x++)
	{
		LVITEM lvitm;
		lvitm.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
		lvitm.iItem = x;
		lvitm.iSubItem = 0;
		lvitm.state = 0;
		lvitm.stateMask = 0;
		lvitm.lParam = (LPARAM) &mods_db_arr[x];
		lvitm.pszText = LPSTR_TEXTCALLBACK;
		ListView_InsertItem(GetDlgItem(TAB_MODY,10400),&lvitm); 
	}


}

void Refresh_Dod()
{
	ListView_DeleteAllItems(GetDlgItem(TAB_DODATKI,10300));

	for (int x= 0; x < exp_db_arr.size(); x++)
	{
		LVITEM lvitm;
		lvitm.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
		lvitm.iItem = x;
		lvitm.iSubItem = 0;
		lvitm.state = 0;
		lvitm.stateMask = 0;
		lvitm.lParam = (LPARAM) &exp_db_arr[x];
		lvitm.pszText = LPSTR_TEXTCALLBACK;
		ListView_InsertItem(GetDlgItem(TAB_DODATKI,10300),&lvitm); 
	}

}

INT_PTR CALLBACK MAIN_WND_PROC(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, i;
	RECT rcTab;
	MAIN_hWnd = hWnd;
	HANDLE bt_new, bt_del, bt_save, bt_first, bt_last, bt_prev, bt_next;
	
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		HWND Tab_hwnd;
		Tab_hwnd = GetDlgItem(hWnd,1035);
		if (Tab_hwnd != NULL)
		{
			TCITEM tie;
			/*pHdr->hwndTab = Tab_hwnd;*/
			tie.mask = TCIF_TEXT | TCIF_IMAGE; 
			tie.iImage = -1; 
			tie.pszText = TEXT("Info. Kontakt."); 
			TabCtrl_InsertItem(Tab_hwnd,0,&tie); 
			tie.pszText = TEXT("Kody"); 
			TabCtrl_InsertItem(Tab_hwnd,1,&tie);
			tie.pszText = TEXT("Mody"); 
			TabCtrl_InsertItem(Tab_hwnd,2,&tie);
			tie.pszText = TEXT("Dodatki");
			TabCtrl_InsertItem(Tab_hwnd,3,&tie);
			
			
			/*SendMessage(Tab_hwnd,TCM_GETITEMRECT,0,(LPARAM)&rcTab);
			MapDialogRect(hWnd,&rcTab);*/
			rcTab.left = 420;
			rcTab.top = 260;
			rcTab.right = 315;
			rcTab.bottom = 380;
			
			TAB_KINFO = CreateDialog(hInst,MAKEINTRESOURCE(IDD_KONTAKT),hWnd,Dlg_InfoKontWndProc);
			TAB_KODY = CreateDialog(hInst,MAKEINTRESOURCE(IDD_KODY),hWnd,Dlg_KodyWndProc);
			TAB_MODY = CreateDialog(hInst,MAKEINTRESOURCE(IDD_MODY),hWnd,Dlg_ModyWndProc);
			TAB_DODATKI = CreateDialog(hInst,MAKEINTRESOURCE(IDD_DODATKI),hWnd,Dlg_DodatkiWndProc);
				
			SetWindowPos(TAB_KINFO,NULL, rcTab.left,rcTab.top, rcTab.right, rcTab.bottom, SWP_SHOWWINDOW);
			SetWindowPos(TAB_KODY,NULL, rcTab.left, rcTab.top, rcTab.right, rcTab.bottom, SWP_HIDEWINDOW);
			SetWindowPos(TAB_MODY,NULL, rcTab.left, rcTab.top, rcTab.right, rcTab.bottom, SWP_HIDEWINDOW);
			SetWindowPos(TAB_DODATKI,NULL, rcTab.left, rcTab.top, rcTab.right, rcTab.bottom, SWP_HIDEWINDOW);
			
			SendMessage(Tab_hwnd,TCM_SETCURSEL,0,NULL);
		}
		

		bt_new = LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON,NULL,NULL,NULL);
		bt_del = LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON2),IMAGE_ICON,NULL,NULL,NULL);
		bt_save = LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON7),IMAGE_ICON,NULL,NULL,NULL);
		bt_first = LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON3),IMAGE_ICON,NULL,NULL,NULL);
		bt_last = LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON4),IMAGE_ICON,NULL,NULL,NULL);
		bt_prev = LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON6),IMAGE_ICON,NULL,NULL,NULL);
		bt_next = LoadImage(hInst,MAKEINTRESOURCE(IDI_ICON5),IMAGE_ICON,NULL,NULL,NULL);
		
		SendMessage(GetDlgItem(hWnd,1018),BM_SETIMAGE,(WPARAM)IMAGE_ICON,(LPARAM)bt_new);
		SendMessage(GetDlgItem(hWnd,1019),BM_SETIMAGE,(WPARAM)IMAGE_ICON,(LPARAM)bt_save);
		SendMessage(GetDlgItem(hWnd,1020),BM_SETIMAGE,(WPARAM)IMAGE_ICON,(LPARAM)bt_del);

		SendMessage(GetDlgItem(hWnd,1014),BM_SETIMAGE,(WPARAM)IMAGE_ICON,(LPARAM)bt_first);
		SendMessage(GetDlgItem(hWnd,1015),BM_SETIMAGE,(WPARAM)IMAGE_ICON,(LPARAM)bt_prev);
		SendMessage(GetDlgItem(hWnd,1016),BM_SETIMAGE,(WPARAM)IMAGE_ICON,(LPARAM)bt_next);
		SendMessage(GetDlgItem(hWnd,1017),BM_SETIMAGE,(WPARAM)IMAGE_ICON,(LPARAM)bt_last);


		// po zainicjalizowaniu ikonek, t³umaczenia, ustaiweñ, odpal pierwszy rekord w istniej¹cej bazie danych
		ReadSettings();
		InitializeCOMBOS();
		LoadDBPATH(false,NULL);
		if (CheckDBExists() == -1)
		{
			CreateDBFN();
		}
		FirstRec();


		return (INT_PTR)TRUE;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmEvent)
		{
		case 0:
			switch (wmId)
			{
				case 1014:
					FirstRec();
					return (INT_PTR)TRUE;
					break;
				case 1015:
					PrevRec();
					return (INT_PTR)TRUE;
					break;
				case 1016:
					NextRec();
					return (INT_PTR)TRUE;
					break;
				case 1017:
					LastRec();
					return (INT_PTR)TRUE;
					break;
				case 1018:
					New_rec();
					return (INT_PTR)TRUE;
					break;
				case 1019:
					Save_rec();
					return (INT_PTR)TRUE;
					break;
				case 1020:
					Del_rec();
					curPos = 0;
					FirstRec();
					return (INT_PTR)TRUE;
					break;
				case 1036:
					Load_pic();
					return (INT_PTR)TRUE;
					break;
				case 1037:
					Del_pic();
					return (INT_PTR)TRUE;
				break;
			}
		default :
			return (INT_PTR)TRUE;
		}
		break;
	case WM_NOTIFY:
            switch (((LPNMHDR)lParam)->code)
            {
            case TCN_SELCHANGE:
                LRESULT tab = SendMessage(((LPNMHDR)lParam)->hwndFrom,TCM_GETCURSEL,NULL,NULL);
				switch (tab)
				{
				case 0:
					ShowWindow(TAB_DODATKI,FALSE);
					ShowWindow(TAB_MODY,FALSE);
					ShowWindow(TAB_KODY,FALSE);
					ShowWindow(TAB_KINFO,TRUE);
					return (INT_PTR)TRUE;
					break;
				case 1:
					ShowWindow(TAB_DODATKI,FALSE);
					ShowWindow(TAB_MODY,FALSE);
					ShowWindow(TAB_KINFO,FALSE);
					ShowWindow(TAB_KODY,TRUE);
					return (INT_PTR)TRUE;
					break;
				case 2:
					ShowWindow(TAB_DODATKI,FALSE);
					ShowWindow(TAB_KINFO,FALSE);
					ShowWindow(TAB_KODY,FALSE);
					ShowWindow(TAB_MODY,TRUE);
					return (INT_PTR)TRUE;
					break;
				case 3:
					ShowWindow(TAB_KINFO,FALSE);
					ShowWindow(TAB_KODY,FALSE);
					ShowWindow(TAB_MODY,FALSE);
					ShowWindow(TAB_DODATKI,TRUE);
					return (INT_PTR)TRUE;
					break;
				}
				break;
			}
	break;
	}
	return (INT_PTR)FALSE;


}

void Start_AU()
{
	if (sett_db.enabled == true)
	{
		int cur_dt = 0;
		time_t u_dt;
		time(&u_dt);
		cur_dt = u_dt;
		if ((cur_dt - sett_db.last_au) > (sett_db.days * 24 * 60 * 60 * 60))
		{
			_spawnl(_P_NOWAIT,"MK_AUTOUPDATE.exe","MK_AUTOUPDATE.exe",NULL);
			sett_db.last_au = cur_dt;
			SaveSettings();
		}
	}


}

int CheckDBExists()
{
	struct _stat buff;
	if ((_tstat(MAIN_FN_PATH,&buff) == 0) && (_tstat(EXP_FN_PATH,&buff) == 0) && (_tstat(MODS_FN_PATH,&buff) == 0))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

void CreateDBFN()
{
	_tmkdir(cur_db_path);
	_tmkdir(COVERS_DIR);
	FILE* fs_db;
	fs_db = _tfopen(MAIN_FN_PATH,TEXT("wb"));
	fclose(fs_db);
	fs_db = _tfopen(MODS_FN_PATH,TEXT("wb"));
	fclose(fs_db);
	fs_db = _tfopen(EXP_FN_PATH, TEXT("wb"));
	fclose(fs_db);
	

}

void LoadDBPATH(bool cust, wchar_t* cust_path)
{
	if (cust)
	{
		_tcscpy(cur_db_path,cust_path);
		_tcscpy(MAIN_FN_PATH, cust_path);
		_tcscat(MAIN_FN_PATH, MAIN_FN);

		_tcscpy(EXP_FN_PATH, cust_path);
		_tcscat(EXP_FN_PATH, EXP_FN);

		_tcscpy(MODS_FN_PATH, cust_path);
		_tcscat(MODS_FN_PATH, MODS_FN);

		_tcscpy(COVERS_DIR,cust_path);
		_tcscat(COVERS_DIR, db_covers);

	}
	else
	{
		wchar_t *cur_dir;
		cur_dir = _tgetcwd(NULL,0);
	
		_tcscpy(cur_db_path,cur_dir);
		_tcscat(cur_db_path,db_path);

		_tcscpy(MAIN_FN_PATH,cur_dir);
		_tcscat(MAIN_FN_PATH,db_path);
		_tcscat(MAIN_FN_PATH,MAIN_FN);
		
		_tcscpy(EXP_FN_PATH, cur_dir);
		_tcscat(EXP_FN_PATH,db_path);
		_tcscat(EXP_FN_PATH,EXP_FN);
		
		_tcscpy(MODS_FN_PATH, cur_dir);
		_tcscat(MODS_FN_PATH, db_path);
		_tcscat(MODS_FN_PATH, MODS_FN);
		
		_tcscpy(COVERS_DIR,cur_dir);
		_tcscat(COVERS_DIR, db_path);
		_tcscat(COVERS_DIR, db_covers);
		
		free(cur_dir);
	}

}

INT_PTR CALLBACK Dlg_Search_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
int wmId, wmEvent, i;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		DLG_WYSZ = hWnd;
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),0,TEXT("Tytu³"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),1,TEXT("Wydawca"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),2,TEXT("Dystrybutor"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),3,TEXT("Producent"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),4,TEXT("Gatunek"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),5,TEXT("Ocena"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),6,TEXT("Cena"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),7,TEXT("Nr Kat"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),8,TEXT("Platforma"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),9,TEXT("Noœnik"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),10,TEXT("WWW"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),11,TEXT("Premiera"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),12,TEXT("Opis"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),13,TEXT("Lista kodów"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),14,TEXT("Producent - WWW"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),15,TEXT("Producent - E-mail"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),16,TEXT("Producent - Adres"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),17,TEXT("Producent - Telefon"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),18,TEXT("Wydawca - WWW"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),19,TEXT("Wydawca - E-mail"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),20,TEXT("Wydawca - Adres"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),21,TEXT("Wydawca - Telefon"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),22,TEXT("Dystrybutor - WWW"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),23,TEXT("Dystrybutor - E-mail"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),24,TEXT("Dystrybutor - Adres"));
		ComboBox_InsertString(GetDlgItem(hWnd,IDC_COMBO_SZPOLE),25,TEXT("Dystrybutor - Telefon"));
		
		HWND LV_hWnd;
		LV_hWnd = GetDlgItem(hWnd,IDC_LIST_WYNIKI);
		LVCOLUMN lv_col;
		lv_col.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lv_col.iSubItem = 0;
		lv_col.pszText = TEXT("Tytu³");
		lv_col.cx = 50;
		ListView_InsertColumn(LV_hWnd,0,&lv_col);

		lv_col.iSubItem = 1;
		lv_col.pszText = TEXT("Gatunek");
		lv_col.cx = 50;
		ListView_InsertColumn(LV_hWnd,1,&lv_col);

		lv_col.iSubItem = 2;
		lv_col.pszText = TEXT("Premiera");
		lv_col.cx = 50;
		ListView_InsertColumn(LV_hWnd,2,&lv_col);

				
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		switch (wmEvent)
		{
		case 0:
			switch (wmId)
			{
			case BUTTON_SZUKAJ:
				Search_BTSZUKAJ();
				Refresh_SZUKAJ_LV();
				break;
			//TODO: Napisaæ obs³ugê zdarzeñ IDD_SZUKAJ -listview
			default:
				break;
			}
			
		default:
			break;
		}
	break;
	case WM_NOTIFY:
	switch (((LPNMHDR) lParam)->code)
        {
        case LVN_GETDISPINFO:
            {
                NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lParam; 
				int x;
				x = plvdi->item.iItem;  

                switch (plvdi->item.iSubItem)
                {
                case 0:
					plvdi->item.pszText = main_wysz[x].tytul;
                    break;

                case 1:
					plvdi->item.pszText = main_wysz[x].genre;
                    break;

                case 2:
					plvdi->item.pszText = main_wysz[x].pubdate;
                    break;

                default:
                    break;
                }
                return TRUE;
            }
		case NM_DBLCLK:
			{
				NMITEMACTIVATE *itemact = (NMITEMACTIVATE*)lParam;
				ReadRec(BrowseMAINForID(main_wysz[itemact->iItem].ID));
				break;
			}
      // More notifications...
      }
	/*default:
		{
			return DefDlgProc(hWnd,message,wParam,lParam);

		}*/
	}
	return (INT_PTR)FALSE;
}

void Search_BTSZUKAJ()
{
	main_wysz.clear();
	int poz = SendMessage(GetDlgItem(DLG_WYSZ,IDC_COMBO_SZPOLE),CB_GETCURSEL ,NULL,NULL);
	wchar_t *fraza = new wchar_t;
	GetWindowText(GetDlgItem(DLG_WYSZ,IDC_EDIT_SZFRAZA),fraza,1024);
	if (poz != CB_ERR)
	{
		MAIN_REC main_buff;
		wchar_t *pdest;
		int result;
		FILE *plik;
		plik = _tfopen(MAIN_FN_PATH,TEXT("rb"));
		LONGLONG stop = 0;
		LONGLONG x;
		struct _stat status;
		if (_wstat(MAIN_FN_PATH,&status) == 0)
		{
			stop = status.st_size;

		}

		switch (poz)
		{
		case 0:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.tytul,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 1:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.publisher,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 2:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.distributor,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 3:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.producer,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 4:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.genre,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 5:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.grade,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 6:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.price,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 7:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.nrkat,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 8:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.platform,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 9:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.medium,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 10:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.WWW,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 11:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.pubdate,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 12:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.desc,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 13:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.cheats,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 14:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.prod_www,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 15:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.prod_email,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 16:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.prod_adres,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 17:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.prod_tel,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 18:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.pub_www,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 19:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.pub_email,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 20:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.pub_adres,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 21:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.pub_tel,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 22:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.dist_www,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 23:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.dist_email,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 24:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.dist_adres,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		case 25:
			for (x=0;x<stop; )
			{
				fseek(plik,x,SEEK_SET); 
				fread(&main_buff,sizeof(struct MAIN_REC),1,plik);
				pdest = wcsstr(main_buff.dist_tel,fraza);
				if (pdest != NULL)
				{
					main_wysz.push_back(main_buff);  
				}	
				x = x + (sizeof(struct MAIN_REC));
			}
			break;
		
		}
	}


}

int GetLastIDDod()
{
	struct _stat status;
	if (_wstat(EXP_FN_PATH,&status) == 0)
	{
		if (status.st_size > 0)
		{
			int ret_id = 0;
			
			FILE *fs_exp;
			fs_exp = _tfopen(EXP_FN_PATH,TEXT("rb"));
			for (LONGLONG offset = 0; offset < status.st_size ; )
			{
				fseek(fs_exp, offset, SEEK_SET);
				fread(&exp_db,sizeof(exp_db),1,fs_exp);
				if (ret_id < exp_db.ID)
				{
					ret_id = exp_db.ID;
				}
				offset = offset + sizeof(exp_db);
			}
			return ret_id;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}

}


void Del_rec_ModsDB(int item)
{
	struct _stat status;
	struct MODS_REC mods_buff, mods_read;
	mods_buff.ID = -1;

	LONGLONG stop = 0;
	if (_wstat(MODS_FN_PATH,&status) == 0)
	{
		stop = status.st_size;
	}
	FILE* mods_fnb;
	mods_fnb = _tfopen(MODS_FN_PATH,_TEXT("r+b"));
	for (LONGLONG offset = 0 ; offset < stop; )
	{
		fseek(mods_fnb,offset,SEEK_SET);
		fread(&mods_read,sizeof(MODS_REC),1,mods_fnb);
		if (mods_read.ID == mods_db_arr[item].ID)
		{
			fwrite(&mods_buff,sizeof(MODS_REC),1,mods_fnb);
		}
		offset = offset + sizeof(MODS_REC);


	}
	fclose(mods_fnb);
	

	
}

void Del_rec_ExpDB(int item)
{
	struct _stat status;
	struct EXPANSIONS_REC exp_buff, exp_read;
	exp_buff.ID = -1;

	LONGLONG stop = 0;
	if (_wstat(EXP_FN_PATH,&status) == 0)
	{
		stop = status.st_size;
	}
	FILE* exp_fnb;
	exp_fnb = _tfopen(EXP_FN_PATH,_TEXT("r+b"));
	for (LONGLONG offset = 0 ; offset < stop; )
	{
		fseek(exp_fnb,offset,SEEK_SET);
		fread(&exp_read,sizeof(EXPANSIONS_REC),1,exp_fnb);
		if (exp_read.ID == exp_db_arr[item].ID)
		{
			fwrite(&exp_buff,sizeof(EXPANSIONS_REC),1,exp_fnb);
		}
		offset = offset + sizeof(EXPANSIONS_REC);

	}
	fclose(exp_fnb);
	

}
void OpenDB()
{
	//Napisac otwieranie / jesli istanieje/ lu tworzenioe nowej bazy danych za pomoca okna otwieranie nowego katalogu
	BROWSEINFO dir_info;
	dir_info.hwndOwner = MAIN_hWnd;
	dir_info.pidlRoot = NULL;
	dir_info.lpszTitle = TEXT("Podaj katalog z baz¹ danych w formacie MKGC");
	dir_info.ulFlags = BIF_USENEWUI;
	if (SHBrowseForFolderW(&dir_info) != NULL)
	{
		struct _stat status;
		if (_wstat(dir_info.pszDisplayName,&status) == 0)
		{
			LoadDBPATH(true,dir_info.pszDisplayName);
			if (CheckDBExists() == -1)
			{
				CreateDBFN();
			}
		}
	}
}
void ClearListCTRLS()
{
	ListView_DeleteAllItems(GetDlgItem(TAB_MODY, 10400));
	ListView_DeleteAllItems(GetDlgItem(TAB_DODATKI, 10300));

}

void Refresh_SZUKAJ_LV()
{
	ListView_DeleteAllItems(GetDlgItem(DLG_WYSZ,IDC_LIST_WYNIKI));

	for (int x= 0; x < main_wysz.size(); x++)
	{
		LVITEM lvitm;
		lvitm.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
		lvitm.iItem = x;
		lvitm.iSubItem = 0;
		lvitm.state = 0;
		lvitm.stateMask = 0;
		lvitm.lParam = (LPARAM) &main_wysz[x];
		lvitm.pszText = LPSTR_TEXTCALLBACK;
		ListView_InsertItem(GetDlgItem(DLG_WYSZ,IDC_LIST_WYNIKI),&lvitm); 
	}



}

int BrowseMAINForID(int id)
{
	FILE *main_fn;
	struct MAIN_REC main_buff;
	main_fn = _tfopen(MAIN_FN_PATH,TEXT("rb"));
	struct _stat status;
	LONGLONG stop = 0;
	if (_wstat(MAIN_FN_PATH,&status) == 0)
	{
		stop = status.st_size;
	}
	for (LONGLONG i = 0; i < stop; )
	{
		fseek(main_fn,i,SEEK_SET);
		fread(&main_buff,sizeof(MAIN_REC),1,main_fn);
		if (main_buff.ID == id)
		{
			 return i;
		}
		i = i + sizeof(MAIN_REC);
	}

}