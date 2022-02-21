#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED
#define B 111

// Declarations Globales
//________________________________________STRUCTS_________________________
typedef struct  //sous type de "Enr"
          {
              char jour ;
              char mois ;
              short annee ;
          }DATE;

typedef struct
        {
            int matricule ;
            char nom[31] ;
            char prenom[31] ;
            DATE date;
            char wilaya ;
            char sang ;
            char grade ;
            char force ;
            char region ;
        }Enr;

typedef struct
        {
            Enr Tab[B] ;
            int Nb ;
            int suivant ;
        }Bloc;

typedef struct  //sous type de "Fichier"
        {
            int tete ;
            int queue ;
        }ENTETE;

typedef struct
        {
            FILE *f ;
            ENTETE entete ;
            char changed;

        }Fichier;

//______________________________________MODULES__________________________________________
void LireDir(Fichier fich, int num, Bloc* buf );
void EcrireDir(Fichier fich, int num, Bloc* buf );
void Aff_entete(Fichier* fich, char num,int data ); //Notice : can use  (void* data) void pointer for non specific data type instead of (int data) for generic data types
int  Entete(Fichier fich, char num); //Notice : can use (void*) as a return type for generic data types
int Ouvrir(Fichier *F, char *nom_fichier, char mode);
void Fermer(Fichier F);
int Alloc_Bloc(Fichier *F);

#endif // MACHINE_H_INCLUDED
