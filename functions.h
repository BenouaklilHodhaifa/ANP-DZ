#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED


int recherche(Fichier F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc, int matricule, int *i, int *j);

int insertion(Enr enregisrement, Fichier *F,FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc );

int suppression(int matricule,Fichier *F,FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc);

int modification_rg_ml (int matricule, char region, Fichier F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc);

void consulter_ctg_grade(Fichier F, int ctg_grade);

void remplir_fich(Fichier *F, Enr e, int *nb_bloc, Bloc *buf);

void fragmenter(Fichier F, Fichier *F1, Fichier *F2, Fichier *F3, Fichier *F4, Fichier *F5, Fichier *F6  );

void ageRegion(Fichier f,char region, int ageMin, int ageSup);

void faRemover(Fichier *f, char fa, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc);

int ancienCreator(Fichier *f, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc);

#endif // FUNCTIONS_H_INCLUDED
