#include <stdio.h>
#include <stdlib.h>
#include "machine_abstraite_lof.h"

extern int taille_entete_lof = sizeof(ENTETE_LOF);
extern int taille_bloc_lof = sizeof(BLOC_INDEX);
extern int lire; extern int ecr; extern int lire_total; extern int ecr_total;

char Ouvrir_lof(FICHIER_LOF *F,char *nom, char mode)
{
    if (mode == 'A')
    {
        F->ptr_f = fopen(nom,"rb+");
        if(F->ptr_f == NULL)
            return 1;
        rewind(F->ptr_f);
        fread(&(F->entete), sizeof(ENTETE_LOF), 1, F->ptr_f);
        F->changed = 0;
            return 0;// maybe a check for the fread would be better
    }
    else if(mode == 'N')
    {
        F->ptr_f = fopen(nom,"wb+");
        if(F->ptr_f == NULL)
            return 1;
        F->entete.T = -1;
        F->entete.Q = -1;
        F->entete.TLBV = 1;
        F->entete.QLBV = 1;
        F->entete.AvantQ = 1;
        fwrite(&(F->entete), sizeof(ENTETE_LOF), 1, F->ptr_f);
        F->changed = 0;
            return 0;//same goes here

    }
}

//____________________________________________________________

void Fermer_lof(FICHIER_LOF F)
{
   if((F.ptr_f != NULL) && (F.changed == 1) )
    {
        rewind (F.ptr_f);
        fwrite(&(F.entete), sizeof(ENTETE_LOF), 1, F.ptr_f);
        fclose(F.ptr_f);
    }
}

//____________________________________________________________

void LireDir_lof(FICHIER_LOF F, int num, BLOC_INDEX *buf)
{
    if(F.ptr_f != NULL)
    {
        int deplacement;
        deplacement = taille_entete_lof + (num-1)*taille_bloc_lof;
        fseek(F.ptr_f, deplacement, SEEK_SET);
        fread(buf, taille_bloc_lof, 1, F.ptr_f);
    }

    lire_total++;
    lire++;
}

//______________________________________________________________

void EcrireDir_lof(FICHIER_LOF F, int num, BLOC_INDEX *buf)
{
    if(F.ptr_f != NULL)
    {
        int deplacement;
        deplacement = taille_entete_lof + (num-1)*taille_bloc_lof;
        fseek(F.ptr_f, deplacement, SEEK_SET);
        //printf("\n ftell = %d\n", ftell(F.ptr_f));
        fwrite(buf, taille_bloc_lof, 1, F.ptr_f);
        fflush(F.ptr_f);
    }

    ecr_total++;
    ecr++;
}

//__________________________________________________________________

int AllocBloc_lof (FICHIER_LOF *F)
{
    //SETUP
    int save = 0;
    BLOC_INDEX buf;
    //END OF SETUP

    save = F->entete.TLBV;

    if( (F->entete.TLBV) == (F->entete.QLBV) )
    {
        (F->entete).TLBV +=1;
        (F->entete).QLBV +=1;
    }

    else
    {
        LireDir_lof(*F, F->entete.TLBV, &buf);
        F->entete.TLBV = buf.suivant;
    }

    F->changed = 1;

    return save;
}

//_____________________________________________________

void Aff_entete_lof(FICHIER_LOF *F, char num, int val)
{
    switch (num)
    {
    case 1:
        F->entete.T = val;
        break;

    case 2:
        F->entete.Q = val;
        break;

    case 3:
        F->entete.TLBV = val;
        break;

    case 4:
        F->entete.QLBV = val;
        break;

    case 5:
        F->entete.AvantQ = val;
        break;
    }

    F->changed = 1;
}

//______________________________________________________

int Entete_lof(FICHIER_LOF F, char num)
{
    switch (num)
    {
    case 1:
        return F.entete.T;
        break;
    case 2:
        return F.entete.Q ;
        break;
    case 3:
        return F.entete.TLBV;
        break;
    case 4:
        return F.entete.QLBV;
        break;
    case 5:
        return F.entete.AvantQ;
        break;
    }
}
