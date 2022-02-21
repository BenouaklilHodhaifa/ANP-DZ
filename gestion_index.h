#ifndef GESTION_INDEX_H_INCLUDED
#define GESTION_INDEX_H_INCLUDED


void rech_dicho_mc(INDEX_NON_DENSE *index_mc, int *case_ind_mc, int const cle);

void rech_dicho_buf_ind(BLOC_INDEX *buf, int cle, char *trouv, int *adr_dep);//rercherche dichotomique dans le buffer de l'index en ms, utile pour les cle unique seulement !

void insertion_lof_v5(char mode, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc, int cle, int adr_bloc_dat, int adr_dep_dat );

void insertion_lof_v1_1(FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc, int cle, int adr_bloc_dat, int adr_dep_dat );

void suppression_lof_v2 (char mode, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc, int const cle, int bloc, int pos);

void suppression_lof_v1_1 (FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc, int const cle);

int chaDecha(char mode,FICHIER_LOF index_ms, INDEX_NON_DENSE *index_mc);


#endif // GESTION_INDEX_H_INCLUDED
