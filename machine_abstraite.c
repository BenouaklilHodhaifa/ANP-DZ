#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "machine_abstraite.h"

extern int taille_entete = sizeof(ENTETE);//Keep an eye on these
extern int taille_bloc = sizeof(Bloc);

extern int lire; extern int ecr; extern int lire_total; extern int ecr_total;

void LireDir(Fichier fich, int num, Bloc* buf )
{

    FILE* ptr_vers_f = fich.f ;
    int deplacement = taille_entete + (num-1)*taille_bloc;
    fseek(ptr_vers_f, deplacement, SEEK_SET);
    fread(buf, taille_bloc, 1, ptr_vers_f );

    lire_total++;
    lire++;
}
//_______________________________________________________________________

void EcrireDir(Fichier fich, int num, Bloc* buf)
{
    FILE* ptr_vers_f = fich.f ;
    int deplacement = taille_entete + (num-1)*taille_bloc;
    fseek(ptr_vers_f, deplacement, SEEK_SET);
    fwrite(buf, taille_bloc, 1, ptr_vers_f );

    ecr_total++;
    ecr++;
}
//_______________________________________________________________________

void Aff_entete(Fichier* fich, char num,int data )
{
   switch (num)
   {
       case 1 :

        fich->entete.tete = data;

       case 2 :

        fich->entete.queue = data;

   }

   fich->changed = 1;

}
//_________________________________________________________________________

int Entete(Fichier fich, char num)
{

    switch (num)
   {
       case 1 :

        return fich.entete.tete; // In the other case return &(fich->entete.tete);

       case 2 :

        return fich.entete.queue;

   }
}

//__________________________________________________________________________

int Ouvrir(Fichier *F, char *nom_fichier, char mode)
{
    if (mode == 'A')
    {
        F->f = fopen(nom_fichier, "rb+");
        if (F->f != NULL)
        {
            rewind(F->f);
            fread(&(F->entete), sizeof(F->entete), 1, F->f);
            return 0;
        }
        else
        {
            return 1; //printf("Le fichier '%s' n existe pas!\n",nom_fichier) ;
        }

    }
    else if (mode == 'N')
    {
        F->f = fopen(nom_fichier, "wb+");
        rewind(F->f);
        F->entete.tete = -1;
        F->entete.queue = -1;
        fwrite(&(F->entete), sizeof(F->entete), 1, F->f) ;
        return 0;
    }
    else
    {
        return 2; //printf("Le mode d'ouverture que vous avez introduit n'existe pas !\n");
    }
};

//______________________________________________________________________________

void Fermer(Fichier F)
{
  if (F.changed == 1)
  {
      rewind(F.f);
      fwrite(&(F.entete),sizeof(F.entete) ,1 , F.f );
  }
  fclose(F.f);
};


//_______________________________________________________________________________
int Alloc_Bloc(Fichier *F)
{
  if(F->entete.tete == -1)
  {
      F->entete.tete = 1;
      F->entete.queue = 1;
      F->changed = 1;
      return 1;
  }
  else
  {
      F->entete.queue += 1;
      F->changed = 1;
      return F->entete.queue ;
  }
};
