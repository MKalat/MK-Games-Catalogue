struct MAIN_REC
{
	int ID;
	wchar_t tytul[500];
	wchar_t publisher[500];
	wchar_t distributor[500];
	wchar_t producer[500];
	wchar_t genre[250];
	wchar_t grade[100];
	wchar_t price[100];
	wchar_t nrkat[250];
	wchar_t platform[250];
	wchar_t medium[250];
	wchar_t WWW[500];
	wchar_t pubdate[50];
	wchar_t desc[2048];
	int single;
	int multi;
	int expansion;
	wchar_t pathtopic[1024];
	wchar_t cheats[2048];
	wchar_t pub_adres[500];
	wchar_t pub_www[500];
	wchar_t pub_email[500];
	wchar_t pub_tel[150];
	wchar_t dist_adres[500];
	wchar_t dist_www[500];
	wchar_t dist_email[500];
	wchar_t dist_tel[150];
	wchar_t prod_adres[500];
	wchar_t prod_www[500];
	wchar_t prod_email[500];
	wchar_t prod_tel[150];
	wchar_t net_grade[100];
};


struct SETTINGS_REC
{
	int enabled;
	int days;
	wchar_t language[250];
	int autosave;
	int conf_exit;
	int last_au;
};


struct MODS_REC
{
	int ID;
	int IDMAIN;
	wchar_t nazwa[250];
	wchar_t wersja_gry[50];
	wchar_t wersja_moda[50];
	wchar_t ocena[50];
	wchar_t www[250];
};



struct EXPANSIONS_REC
{
	int ID;
	int IDMAIN;
	wchar_t nazwa[250];
	wchar_t cena[50];
	wchar_t multi[50];
	wchar_t single[50];
	wchar_t www[250];
	wchar_t ocena[50];
};


struct LANG_STRS_REC
{
	wchar_t title[11];
	wchar_t publisher[11];
	wchar_t distributor[11];
	wchar_t producer[11];
	wchar_t genre[11];
	wchar_t grade[11];
	wchar_t price[11];
	wchar_t nrkat[11];
	wchar_t platform[11];
	wchar_t medium[11];
	wchar_t www[11];
	wchar_t pub_date[11];
	wchar_t st_desc[20];
	wchar_t single[15];
	wchar_t multi[15];
	wchar_t exp[15];
	wchar_t bt_load_pic[10];
	wchar_t bt_del_pic[10];
	wchar_t st_record[8];
	wchar_t st_of[3];
	wchar_t bt_new[10];
	wchar_t bt_save[10];
	wchar_t bt_del[10];
	wchar_t tab_cheats[10];
	wchar_t tab_mods[10];
	wchar_t tab_exp[10];
	wchar_t tab_exp_st_desc[20];
	wchar_t tab_exp_st_list_name[10];
	wchar_t tab_exp_st_list_price[10];
	wchar_t tab_exp_st_list_multi[10];
	wchar_t tab_exp_st_list_single[10];
	wchar_t tab_exp_st_list_www[10];
	wchar_t tab_exp_st_list_grade[10];
	wchar_t tab_exp_bt_new[10];
	wchar_t tab_exp_bt_edit[10];
	wchar_t tab_exp_bt_del[10];
	wchar_t dlg_edit_exp_st_desc[20];
	wchar_t dlg_edit_exp_bt_OK[10];
	wchar_t dlg_edit_exp_bt_cancel[10];
	wchar_t tab_cheats_st_desc[20];
	wchar_t tab_cheats_bt_clear[10];
	wchar_t tab_mods_st_desc[20];
	wchar_t tab_mods_list_st_name[10];
	wchar_t tab_mods_list_st_gamever[10];
	wchar_t tab_mods_list_st_modver[10];
	wchar_t tab_mods_list_st_grade[10];
	wchar_t tab_mods_list_st_www[10];
	wchar_t tab_mods_bt_new[10];
	wchar_t tab_mode_bt_save[10];
	wchar_t tab_mods_bt_del[10];
	wchar_t dlg_edit_mods_desc[20];
	wchar_t dlg_edit_mods_bt_OK[10];
	wchar_t dlg_edit_mods_bt_cancel[10];
	wchar_t dlg_contact_adres[11];
	wchar_t dlg_contact_email[11];
	wchar_t dlg_contact_www[11];
	wchar_t dlg_contact_tel[11];
	wchar_t dlg_sett_gr_au[20];
	wchar_t dlg_sett_chk_en_au[100];
	wchar_t dlg_sett_st_aufreq[100];
	wchar_t dlg_sett_combo_aufeq_7d[10];
	wchar_t dlg_sett_combo_aufreq_14d[10];
	wchar_t dlg_sett_combo_aufreq_30d[10];
	wchar_t dlg_sett_st_lang[50];
	wchar_t dlg_sett_chk_autosave[100];
	wchar_t dlg_sett_chk_askclose[100];
	wchar_t dlg_sett_bt_OK[10];
	wchar_t dlg_sett_bt_cancel[10];
	
	

	

};
typedef struct tag_dlghdr { 
    HWND hwndTab;       // tab control 
    HWND hwndDisplay[4];   // current child dialog box 
    RECT rcDisplay;     // display rectangle for the tab control 
    DLGTEMPLATE *apRes[4]; 
} DLGHDR; 
