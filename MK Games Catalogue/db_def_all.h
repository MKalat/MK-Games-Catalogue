struct MAIN_REC
{
	int ID;
	TCHAR tytul[500];
	TCHAR publisher[500];
	TCHAR distributor[500];
	TCHAR producer[500];
	TCHAR genre[250];
	TCHAR grade[100];
	TCHAR price[100];
	TCHAR nrkat[250];
	TCHAR platform[250];
	TCHAR medium[250];
	TCHAR WWW[500];
	TCHAR pubdate[50];
	TCHAR desc[2048];
	int single;
	int multi;
	int expansion;
	TCHAR pathtopic[1024];
	TCHAR cheats[2048];
	TCHAR pub_adres[500];
	TCHAR pub_www[500];
	TCHAR pub_email[500];
	TCHAR pub_tel[150];
	TCHAR dist_adres[500];
	TCHAR dist_www[500];
	TCHAR dist_email[500];
	TCHAR dist_tel[150];
	TCHAR prod_adres[500];
	TCHAR prod_www[500];
	TCHAR prod_email[500];
	TCHAR prod_tel[150];
	TCHAR net_grade[100];
};


struct SETTINGS_REC
{
	int enabled;
	int days;
	TCHAR language[250];
	int autosave;
	int conf_exit;
	int last_au;
};


struct MODS_REC
{
	int ID;
	int IDMAIN;
	TCHAR nazwa[250];
	TCHAR wersja_gry[50];
	TCHAR wersja_moda[50];
	TCHAR ocena[50];
	TCHAR www[250];
};



struct EXPANSIONS_REC
{
	int ID;
	int IDMAIN;
	TCHAR nazwa[250];
	TCHAR cena[50];
	TCHAR multi[50];
	TCHAR single[50];
	TCHAR www[250];
	TCHAR ocena[50];
};


struct LANG_STRS_REC
{
	TCHAR title[11];
	TCHAR publisher[11];
	TCHAR distributor[11];
	TCHAR producer[11];
	TCHAR genre[11];
	TCHAR grade[11];
	TCHAR price[11];
	TCHAR nrkat[11];
	TCHAR platform[11];
	TCHAR medium[11];
	TCHAR www[11];
	TCHAR pub_date[11];
	TCHAR st_desc[20];
	TCHAR single[15];
	TCHAR multi[15];
	TCHAR exp[15];
	TCHAR bt_load_pic[10];
	TCHAR bt_del_pic[10];
	TCHAR st_record[8];
	TCHAR st_of[3];
	TCHAR bt_new[10];
	TCHAR bt_save[10];
	TCHAR bt_del[10];
	TCHAR tab_cheats[10];
	TCHAR tab_mods[10];
	TCHAR tab_exp[10];
	TCHAR tab_exp_st_desc[20];
	TCHAR tab_exp_st_list_name[10];
	TCHAR tab_exp_st_list_price[10];
	TCHAR tab_exp_st_list_multi[10];
	TCHAR tab_exp_st_list_single[10];
	TCHAR tab_exp_st_list_www[10];
	TCHAR tab_exp_st_list_grade[10];
	TCHAR tab_exp_bt_new[10];
	TCHAR tab_exp_bt_edit[10];
	TCHAR tab_exp_bt_del[10];
	TCHAR dlg_edit_exp_st_desc[20];
	TCHAR dlg_edit_exp_bt_OK[10];
	TCHAR dlg_edit_exp_bt_cancel[10];
	TCHAR tab_cheats_st_desc[20];
	TCHAR tab_cheats_bt_clear[10];
	TCHAR tab_mods_st_desc[20];
	TCHAR tab_mods_list_st_name[10];
	TCHAR tab_mods_list_st_gamever[10];
	TCHAR tab_mods_list_st_modver[10];
	TCHAR tab_mods_list_st_grade[10];
	TCHAR tab_mods_list_st_www[10];
	TCHAR tab_mods_bt_new[10];
	TCHAR tab_mode_bt_save[10];
	TCHAR tab_mods_bt_del[10];
	TCHAR dlg_edit_mods_desc[20];
	TCHAR dlg_edit_mods_bt_OK[10];
	TCHAR dlg_edit_mods_bt_cancel[10];
	TCHAR dlg_contact_adres[11];
	TCHAR dlg_contact_email[11];
	TCHAR dlg_contact_www[11];
	TCHAR dlg_contact_tel[11];
	TCHAR dlg_sett_gr_au[20];
	TCHAR dlg_sett_chk_en_au[100];
	TCHAR dlg_sett_st_aufreq[100];
	TCHAR dlg_sett_combo_aufeq_7d[10];
	TCHAR dlg_sett_combo_aufreq_14d[10];
	TCHAR dlg_sett_combo_aufreq_30d[10];
	TCHAR dlg_sett_st_lang[50];
	TCHAR dlg_sett_chk_autosave[100];
	TCHAR dlg_sett_chk_askclose[100];
	TCHAR dlg_sett_bt_OK[10];
	TCHAR dlg_sett_bt_cancel[10];
	
	

	

};
typedef struct tag_dlghdr { 
    HWND hwndTab;       // tab control 
    HWND hwndDisplay[4];   // current child dialog box 
    RECT rcDisplay;     // display rectangle for the tab control 
    DLGTEMPLATE *apRes[4]; 
} DLGHDR; 
