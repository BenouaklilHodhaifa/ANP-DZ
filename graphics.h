#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED
#include "machine_abstraite.h"

void effacement(int y, int lignes);

void wait(int a);

int getkey(void);

int scanstr_age(char *num);

int scan_age(int *choix);

int scanstr_matricule(char *num);

int scan_matricule(int* choix, Fichier F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc, char mode);

void scanstr_date_naissance(char *num,int mode);

void scan_date_naissance(int* choix, int mode);

int scanstr_reste(char *num,int mode);

int scan_reste(int* choix, int mode);

void scanstr_N(char *num);

void scan_N(int* choix);

void scan_nom(char *num);

void startup();

int landing();

int menu_principal();

void table_villes(char str[58][40]);

void table_sang(char str[8][4]);

void table_grade(char str[17][30]);

void table_fa(char str[8][50]);

void table_region(char str[6][20]);

void affichage_table_ville(int x, int y, int max , char str[58][40]);

void affichage_table_sang(int x, int y, int max, char str[8][4]);

void affichage_table_grade(int x, int y, int max, char str[17][30]);

void affichage_table_fa(int x, int y, int max, char str[8][50]);

void affichage_table_rg(int x, int y, int max, char str[6][20]);

void affichage_enr(Enr e, int y);

void affichage_insertion(Fichier *F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc);

void affichage_suppression(Fichier *F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc);

void affichage_modification_rg_ml(Fichier F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc);

void affichage_recherche(Fichier F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc);

void affichage_suppr_fa(Fichier *F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc);

void affichage_requete_intervalle(Fichier F);

void affichage_consulter_ctg_grd(Fichier F);

void affichage_fragmenter(Fichier F, Fichier *F1, Fichier *F2, Fichier *F3, Fichier *F4, Fichier *F5, Fichier *F6);

void affichage_qst5(Fichier F);

void affichage_charg_init(Fichier *F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc);

int affichage_ancien_fich(Fichier *F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc);


#endif // GRAPHICS_H_INCLUDED
