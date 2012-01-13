// MK Games Catalogue.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MK Games Catalogue.h"
#include "db_def_all.h"
#include <vector>
#include <time.h>
#include <process.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>


#define MAX_LOADSTRING 100
#define MAX_MODS_REC 20
#define MAX_EXP_REC 20
#define SETT_FN TEXT("settings.cfg")
#define MAIN_FN TEXT("MKGC.db")
#define MODS_FN TEXT("MKGC_M.db")
#define EXP_FN TEXT("MKGC_E.db")
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
bool new_mods_rec = false;
bool new_exp_rec = false;
bool new_main_rec = false;
TCHAR db_name[50] = TEXT("default\\"); // nazwa bazy danych;
TCHAR db_path[550] = TEXT("db\\");
TCHAR MAIN_FN_PATH[1000];
TCHAR EXP_FN_PATH[1000];
TCHAR MODS_FN_PATH[1000];
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
INT_PTR CALLBACK		Dlg_ManageDB_WndProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK		Dlg_Search_WndProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR CALLBACK		Dlg_NewDB_WndProc(HWND,UINT,WPARAM,LPARAM);


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
void				LoadDBPATH(); // ³aduje œcie¿kê do aktualnej bazy danych;
void				ManDB_NewDB(); //dodaje now¹ baze danych
void				ManDB_SelDB(); // ³aduje wybran¹ baze danych
void				ManDB_DelDB(); // kasuje dana baze danych
void				Search_BTSZUKAJ();
int					GetLastIDDod(); // zwraca najwy¿sze id rekordu EXP_DB w pliku EXP_FN


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
				DialogBox(hInst,MAKEINTRESOURCE(IDD_MANAGEDB),hWnd,Dlg_ManageDB_WndProc);
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
	Save_rec();
	new_main_rec = false;


}
void Save_rec()
{
	if (CheckDBExists() == 0)
	{
		UpdateWCTRLS();
		if (SaveToFile(curPos) == 0)
		{
			//do nothing
		}
		else
		{
			MessageBox(MAIN_hWnd,TEXT("Wystapi³ b³¹d podczas zapisu bazy danych !. Ecounetred error during DB write operation !."),TEXT("MK Games Catalogue"),MB_OK);
		}
	}


}
void Del_rec()
{
//TODO: Napisaæ kasowanie rekordu g³ownego i podrzêdnych

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
//TODO: Npsaæ ³adowanie zdjêcia ok³adki do bazy danych i na main form

}

void Del_pic()
{
//TODO: Napisaæ usuwanie zdjêcia ok³adki z bazy danych i main form

}

void ReadRec(long pos)
{
	if (CheckDBExists() == 0)
	{
		ClearARRS();
		if (ReadFromFile(pos) == 0)
		{
			UpdateRCTRLS();
		}
		else
		{
			MessageBox(MAIN_hWnd,TEXT("Wystapi³ b³¹d podczas odczytu bazy danych !. Ecounetred error during DB read operation !."),TEXT("MK Games Catalogue"),MB_OK);
		}
	}


}

int	ReadFromFile(long pos)
{
	FILE *fs_db;
	fs_db = _tfopen(MAIN_FN_PATH,TEXT("rb"));
	if (fs_db != NULL)
	{
		fseek(fs_db,pos,SEEK_SET);
		fread(&main_db,sizeof(main_db),1,fs_db);
		fclose(fs_db);
		
		long offset = 0;
		FILE *exp_fs;
		exp_fs = _tfopen(EXP_FN_PATH,TEXT("rb"));
		if (exp_fs != NULL)
		{
			while (!(feof(exp_fs)))
			{
				fseek(exp_fs,offset,SEEK_SET);
				fread(&exp_db,sizeof(exp_db),1,exp_fs);
				if (exp_db.IDMAIN == main_db.ID)
				{
					exp_db_arr.push_back(exp_db);
					

				}
				offset = offset + sizeof(exp_db);
			}
			fclose(exp_fs);
		}
		offset = 0;
		
		FILE *mod_fs;
		mod_fs = _tfopen(MODS_FN_PATH,TEXT("rb"));
		if (mod_fs != NULL)
		{
			while (!(feof(mod_fs)))
			{
				fseek(mod_fs,offset,SEEK_SET);
				fread(&mods_db,sizeof(mods_db),1,mod_fs);
				if (mods_db.IDMAIN == main_db.ID)
				{
					mods_db_arr.push_back(mods_db);

					
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

int	SaveToFile(long pos)
{
	long offset = 0;
	FILE *fs_db;
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
				while (!(feof(fs_mod)))
				{
					fseek(fs_mod,offset,SEEK_SET);
					fread(&mods_db,sizeof(mods_db),1,fs_mod);
					if (main_db.ID == mods_db.IDMAIN)
					{
						for (int x = 0; x < mods_db_arr.size() ; x++)
						{
							if (mods_db.ID == mods_db_arr[x].ID)
							{
								fwrite(&mods_db_arr[x],sizeof(mods_db),1,fs_mod);
							}

						}

					}
					offset = offset + sizeof(mods_db);
				}
				fclose(fs_mod);
			}
			
			FILE *fs_exp;
			offset = 0;
			fs_exp = _tfopen(EXP_FN_PATH,TEXT("r+b"));
			if (fs_exp != NULL)
			{
				while (!(feof(fs_exp)))
				{
					fseek(fs_exp,offset,SEEK_SET);
					fread(&exp_db,sizeof(exp_db),1,fs_exp);
					if (main_db.ID == exp_db.IDMAIN)
					{
						for (int y = 0; y < exp_db_arr.size(); y++)
						{
							if (exp_db.ID == exp_db_arr[y].ID)
							{
								fwrite(&exp_db_arr[y],sizeof(exp_db),1,fs_exp);
							}

						}
					}
					offset = offset + sizeof(exp_db);
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
		fs_db = _tfopen(MAIN_FN_PATH,TEXT("ab"));
		if (fs_db != NULL)
		{
			fseek(fs_db,(SearchLastPos() + sizeof(main_db)),SEEK_SET);
			fwrite(&main_db,sizeof(main_db),1,fs_db);
			
			fclose(fs_db);
			return 0;
		}
		else
		{

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

	//TODO : Napisaæ ³adowanie ok³adek
	

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

				}
				break;
			case 10402:
				// get selected item on list view control, get its data and copy to tmp struct, thaen operate
				// on this struct in dlg_box and then copy back its contents to array and to control via getdispinfo
				Prep_Edit_Mody();
				if (DialogBox(hInst,MAKEINTRESOURCE(134),hWnd,EditMody_WndProc) == 10505)
				{
					Edit_Mody();
				}
				break;
			case 10403:
				//TODO: get selected item and delete it from array and list view control via getdispinfo
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

	/*case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;*/
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
	int ret_id = 0;
	FILE *fs_mods;
	fs_mods = _tfopen(MODS_FN_PATH,TEXT("rb"));
	while (!(feof(fs_mods)))
	{
		fread(&mods_db,sizeof(mods_db),1,fs_mods);
		if (ret_id < mods_db.ID)
		{
			ret_id = mods_db.ID;
		}
	}
	return ret_id;
}
void Prep_Edit_Mody()
{
	for int x = 0; x < mods_db_arr.size(); x++)
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
	mods_db_arr[edit_id].nazwa = mods_db.nazwa;
	mods_db_arr[edit_id].ocena = mods_db.ocena;
	mods_db_arr[edit_id].wersja_gry = mods_db.wersja_gry;
	mods_db_arr[edit_id].wersja_moda = mods_db.wersja_moda;
	mods_db_arr[edit_id].www = mods_db.www;
	Refresh_Mods();
	edit_id = -1;
}

void Prep_Edit_Dod()
{
	for int x = 0; x < exp_db_arr.size(); x++)
	{
		if (SendMessage(GetDlgItem(TAB_EXP,10300),LVM_GETITEMSTATE,(WPARAM)x,(LPARAM)(UINT)LVIS_SELECTED) == LVIS_SELECTED)
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
		fs_exp = _tfopen(EXP_FN_PATH,TEXT("ab"));
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
		fs_mods = _tfopen(MODS_FN_PATH,TEXT("ab"));
		fwrite(&mods_db,sizeof(mods_db),1,fs_mods);
		fclose(fs_mods);
	}
	new_mods_rec = false;
}

void Edit_Dod()
{
	exp_db_arr[edit_id].nazwa = exp_db.nazwa;
	exp_db_arr[edit_id].cena = exp_db.cena;
	exp_db_arr[edit_id].multi = exp_db.multi;
	exp_db_arr[edit_id].ocena = exp_db.ocena;
	exp_db_arr[edit_id].single = exp_db.single;
	exp_db_arr[edit_id].www = exp_db.www;
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
			if (main_db.ID > L_ID)
			{
				L_ID = main_db.ID;

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
	Refresh_Dod()



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

		InitializeCOMBOS();
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
					return (INT_PTR)TRUE;
					break;
				case 1035:
					Load_pic();
					return (INT_PTR)TRUE;
					break;
				case 1036:
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
	LoadDBPATH();
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
	FILE *fs_db;
	TCHAR dir_buff[1000];
	_tgetdcwd(_getdrive(),dir_buff,sizeof(dir_buff));
	_tcscat(dir_buff,'\\');
	_tcscat(dir_buff,db_path);
	if (_tchdir(dir_buff) == -1)
	{
		_tmkdir(dir_buff);
	}

	_tcscat(dir_buff,db_name);
	if (_tchdir(dir_buff) == -1)
	{
		_tmkdir(dir_buff);
	}
	else
	{
		MessageBox(hWnd,L"Baza danych ju¿ jest w u¿yciu",L"MK Games Catalogue",MB_OK);
		return;
	}
	LoadDBPATH();

	fs_db = _tfopen(MAIN_FN_PATH,TEXT("wb"));
	fclose(fs_db);
	FILE *fs_db2;
	fs_db2 = _tfopen(EXP_FN_PATH,TEXT("wb"));
	fclose(fs_db2);
	FILE *fs_db3;
	fs_db3 = _tfopen(MODS_FN_PATH,TEXT("wb"));
	fclose(fs_db3);

}

void LoadDBPATH()
{
	wchar_t buff[550];
	wchar_t cur_dir[550];
	_tgetdcwd(_getdrive(),cur_dir,sizeof(cur_dir));

	_tcscpy(MAIN_FN_PATH,cur_dir);
	_tcscat(MAIN_FN_PATH,'\\');
	_tcscat(MAIN_FN_PATH,db_path);
	_tcscat(MAIN_FN_PATH,db_name);
	_tcscat(MAIN_FN_PATH,MAIN_FN);

	_tcscpy(EXP_FN_PATH,cur_dir);
	_tcscat(EXP_FN_PATH,'\\');
	_tcscat(EXP_FN_PATH,db_path);
	_tcscat(EXP_FN_PATH,db_name);
	_tcscat(EXP_FN_PATH,EXP_FN);

	_tcscpy(MODS_FN_PATH,cur_dir);
	_tcscat(MODS_FN_PATH,'\\');
	_tcscat(MODS_FN_PATH,db_path);
	_tcscat(MODS_FN_PATH,db_name);
	_tcscat(MODS_FN_PATH,MODS_FN);
}

INT_PTR CALLBACK Dlg_ManageDB_WndProc(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, i;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		//TODO:Napisaæ inicjalizacjê MANAGEDBS - wczytywanie nazw baz danych, wyœwietlanie tego

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmEvent)
		{
		case 0:
			switch (wmId)
			{
			case IDC_BUTTON_NEW:
				ManDB_NewDB();
				break;
			case IDC_BUTTON_SEL:
				ManDB_SelDB();
				break;
			case IDC_BUTTON_DEL:
				ManDB_DelDB();
				break;
			case IDC_BUTTON_EXIT:
				EndDialog(hWnd,LOWORD(wParam));
				break;
			default:
				break;
			}
		default:
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void ManDB_NewDB()
{
	DialogBox(hInst,MAKEINTRESOURCE(IDD_NEWDB),hWnd,Dlg_NewDB_WndProc);
}

void ManDB_SelDB()
{
	//TODO:Napisaæ ManDB_SelDB()

}

void ManDB_DelDB()
{
	//TODO: Napisaæ ManDB_DelDB()

}

INT_PTR CALLBACK Dlg_Search_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
int wmId, wmEvent, i;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		//TODO:Napisaæ inicjalizacjê IDD_SZUKAJ - combo pole

		return (INT_PTR)TRUE;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmEvent)
		{
		case 0:
			switch (wmId)
			{
			case BUTTON_SZUKAJ:
				Search_BTSZUKAJ();
				break;
			//TODO: Napisaæ obs³ugê zdarzeñ IDD_SZUKAJ -listview
			default:
				break;
			}
		default:
			break;
		}
	break;
	}
	return (INT_PTR)FALSE;
}

void Search_BTSZUKAJ()
{
	//TODO: Napisaæ proces wyszukiwania i umieszczania wyszukanych w listView.

}

int GetLastIDDod()
{
	int ret_id = 0;
	FILE *fs_exp;
	fs_exp = _tfopen(EXP_FN_PATH,TEXT("rb"));
	while (!(feof(fs_exp)))
	{
		fread(&exp_db,sizeof(exp_db),1,fs_exp);
		if (ret_id < exp_db.ID)
		{
			ret_id = exp_db.ID;
		}
	}
	return ret_id;


}

INT_PTR CALLBACK Dlg_NewDB_WndProc(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, i;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmEvent)
		{
		case 0:
			switch (wmId)
			{
			case IDOK:
				wchar_t *buff;
				_tcscpy(buff,db_name);
				_tcscpy(db_name,GetDlgItem(hWnd,UINT(IDC_EDIT_NAME));
				_tcscat(db_name,'\\');
				CreateDBFN();
				_tcscpy(db_name,buff);
				LoadDBPATH();
				EndDialog(hWnd,LOWORD(wParam));
				
				break;
			case IDCANCEL:
				EndDialog(hWnd,LOWORD(wParam));
				break;
			default:
				break;
			}
		default:
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}