#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "machine_abstraite.h"
#include "machine_abstraite_lof.h"
#include "gestion_index.h"

extern int lire; extern int ecr; extern int lire_total; extern int ecr_total;

Enr create_enr() ;

int randomizer( int max)
{
    int tab[6]={0};//* balak conv wella int
    int tab2[6]={0};//*

    char quitter = 0;
    char cpt = 0;

    int result = 0;
    int pui = 1;

    while(max != 0)
    {
        tab[cpt]= max%10;
        max = max-tab[cpt];
        max = (int)(max/10);//*
        cpt++;
    }


    cpt--;

    while(cpt>=0 && quitter == 0)
    {
        tab2[cpt] = rand()%(tab[cpt]+1); //le 1 machi a l'exterieur sinon jamais idji 0 !!!!

        if(tab2[cpt]!=tab[cpt])
            quitter = 1;
        else
            cpt--;
    }

    if(quitter == 1)//je pense pas vraiment necessaire mais ma3liche
        cpt--;

    while(cpt>=0)
    {
        tab2[cpt]= rand()%10;
        cpt--;
    }

    quitter = 0;//pas vraiment necessaire puisque on a initialiser tab2 a 0...
    cpt = 0;

    while(cpt<=5 && quitter == 0)
    {
        if(tab2[cpt]!=0)
        {
            result = result + tab2[cpt]*pui;
            pui = pui*10;
            cpt++;
        }
        else
          quitter = 1;


    }

    return result;


}

void charg_init(int N, Fichier *F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc)
{


    Bloc buffer;
    Enr e;
    int i,nb_bloc;
    lire=0; ecr=0;
    //char stop=0;

    int *tab = malloc(888889*sizeof(int));
    int cpt = 111111;
    int fin = 888888;

    int pos = 0;

    buffer.Nb = 0;
    buffer.suivant = -1;

    Ouvrir_lof(index_ms,"index_ms.bin",'N');
    index_mc->sup=-1;

    //END OF SETUP

    for(int z = 0; z<=888888; z++)
    {
        tab[z] = cpt;
        cpt++;
    }



    Ouvrir(F,"PERSONNEL-ANP_DZ.bin",'N');
    nb_bloc = Alloc_Bloc(F);
    for(i=0 ;i<N ;i++)
    {
        //gotoxy(75+16,15);printf("%d",i+1);
        e = create_enr();

        pos = randomizer(fin);
        e.matricule = tab[pos];

        tab[pos] = tab[fin];
        fin--;



        insertion_lof_v5('N',index_ms, index_mc, e.matricule, nb_bloc, buffer.Nb);// dert 0 0 pour tester berk

        if(buffer.Nb == B-1)
        {
            buffer.Tab[buffer.Nb] = e;
            buffer.suivant = Alloc_Bloc(F);
            buffer.Nb++;
            EcrireDir(*F,nb_bloc,&buffer);
            nb_bloc = buffer.suivant;
            buffer.Nb = 0;
        }
        else
        {
            buffer.Tab[buffer.Nb] = e;
            buffer.Nb++;
        }

    }
    if(buffer.Nb == 0)
    {
        Aff_entete(F,2,Entete(*F,2)-1);
        LireDir(*F,Entete(*F,2),&buffer);
        buffer.suivant = -1;
        EcrireDir(*F,Entete(*F,2),&buffer);
    }
    else
    {
        buffer.suivant = -1;
        EcrireDir(*F,Entete(*F,2),&buffer);
    }


    Fermer(*F);

    Ouvrir(F,"PERSONNEL-ANP_DZ.bin",'A');
    insertion_lof_v5('F',index_ms, index_mc, 0 , 0, 0); //Flushing usage
    Fermer_lof(*index_ms);
    Ouvrir_lof(index_ms, "index_ms.bin", 'A'); // ***************************************************************
};

Enr create_enr()
{
  Enr enregistrement ;
  int nb_char ;
  int charactere ;
  int i;
    // matricule
  enregistrement.matricule = 0;

  nb_char = rand()%27 +4 ; // le nombre de charactères dans le nom et le prenom

    // generation du nom

   for( i=0 ; i<nb_char ; i++)
   {
      charactere = rand() % 26 +1;
      enregistrement.nom[i] = charactere + 96 ;
   }
   enregistrement.nom[i] = '\0' ;
    // generation du prénom
   for(int i=0 ; i<nb_char ; i++)
   {
      charactere = rand() % 26 +1;
      enregistrement.prenom[i] = charactere + 96 ;
   }
   enregistrement.prenom[i] = '\0' ;

   // date de naissance

     enregistrement.date.annee = (short) (rand()%64 +1940); // entre 1940 et 2003
     enregistrement.date.mois = (char) (rand()%12 +1);

     i = (int) (enregistrement.date.mois) ;
     if((i==4) || (i==6) || (i==9) || (i==11))
     {
         enregistrement.date.jour = (char) (rand()%30 +1); //entre 1 et 30
     }
     else if (i==2)
     {
         if(enregistrement.date.annee %4 == 0) // année bisixtile
         {
             enregistrement.date.jour = (char) (rand()%29 +1); //entre 1 et 29
         }
         else
         {
             enregistrement.date.jour = (char) (rand()%28 +1);// entre 1 et 28
         }
     }
     else
     {
         enregistrement.date.jour = (char) (rand()%31 +1); //entre 1 et 31
     }


   // wilaya
    enregistrement.wilaya = (char)(rand()%58 +1);

   // groupe sanguin
    enregistrement.sang = (char) (rand()%8 +1);

   // grade
    enregistrement.grade = (char) (rand()%17 +1);

   // force armee
    enregistrement.force = (char) (rand()%8 +1);

   // région militaire
    enregistrement.region = (char) (rand()%6 +1);

    return enregistrement;
};
