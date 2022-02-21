#ifndef MACHINE_ABSTRAITE_LOF_H_INCLUDED
#define MACHINE_ABSTRAITE_LOF_H_INCLUDED
#define B_INDEX_MS 708
#define B_INDEX_MC 2600
//____________________________________STRUCTS_______________________________
//__________________________________________________________________________

typedef struct
{
    int T;
    int Q;
    int TLBV;
    int QLBV;
    int AvantQ;

}ENTETE_LOF;

//-----------------------------------------

typedef struct
{
  FILE *ptr_f;
  ENTETE_LOF entete;
  char changed;

}FICHIER_LOF;

//_____________________________________________
//_____________________________________________

typedef struct
{
    int cle;
    int adr_bloc_dat;//adresse bloc dans le fichier de data
    int adr_dep_dat;//adresse deplacement dans le fichier de data
} CCC; //enregistrement du fichier index dense: couple-clé-{coordonnées dans le fichier de donnees}

//-----------------------------------------

typedef struct
{
   int suivant;
   int Nb;
   CCC tab[B_INDEX_MS]; //708

}BLOC_INDEX;

//_______________________________________________
//_______________________________________________

typedef struct
{
    int cle;
    int adr_bloc_ind; //adresse_bloc_dans le fichier d'index
    BLOC_INDEX* adr_mc;// adresse du bloc en mc
}CCB; // enregistrement de la table d'index cluster: couple-clé-{adr_bloc dans le fichier index dense}

//--------------------------------------------

typedef struct
{
    CCB tab[B_INDEX_MC]; //2600 pour la surete...
    int sup; // a initialiser avec -1
}INDEX_NON_DENSE;





//___________________________________________________________________________________
//____________________________________________MODULES________________________________

char Ouvrir_lof(FICHIER_LOF *F,char *nom, char mode);
void Fermer_lof(FICHIER_LOF F);
void LireDir_lof(FICHIER_LOF F, int num, BLOC_INDEX *buf );
void EcrireDir_lof(FICHIER_LOF F, int num, BLOC_INDEX *buf);
int AllocBloc_lof (FICHIER_LOF *F);
void Aff_entete_lof(FICHIER_LOF *F, char num, int val);
int Entete_lof(FICHIER_LOF F, char num);
void EcrireDir_lof_test(FICHIER_LOF F, int num, const BLOC_INDEX buf);

#endif // MACHINE_ABSTRAITE_LOF_H_INCLUDED
