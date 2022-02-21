#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "machine_abstraite_lof.h"
#include "machine_abstraite.h"
#include "forms.h"

extern int lire; extern int ecr; extern int lire_total; extern int ecr_total;

void lof_Afficheur(FICHIER_LOF index_ms)
{

    //SETUP
    int scan1 = 0, scan2 = 0;
    FILE *bloc_note = NULL;
    char khet[] = "\t\t\t\t+--------------------+--------------------+--------------------+\n";
    CCC forms;
    BLOC_INDEX buf;

    //END OF SETUP

    bloc_note = fopen("main_fich/lof.txt", "w+");
    fprintf(bloc_note,"%s", khet);
    fprintf(bloc_note,"\t\t\t\t|%-20s|%-20s|%-20s|\n","MATRICULE", "ADR_BLOC", "ADR_DEP" );
    fprintf(bloc_note,"%s", khet);

    scan1 = index_ms.entete.T;
    while (scan1 != -1)
    {
        LireDir_lof(index_ms, scan1, &buf);
        lire_total--;lire--;
        for(scan2 = 0; scan2<buf.Nb; scan2++)
        {
            forms = buf.tab[scan2];
            fprintf(bloc_note,"\t\t\t\t|%-20d|%-20d|%-20d|\n", forms.cle, forms.adr_bloc_dat, forms.adr_dep_dat);
            fprintf(bloc_note,"%s", khet);
        }


       scan1 = buf.suivant;
    }


    fclose(bloc_note);

}




void lobf_Afficheur(Fichier f, char nom[])
{
    //SETUP
    int scan1 = 0, scan2 = 0;
    FILE *bloc_note = NULL;
    char khet[] = "+--------------------+-----------------------------------+-----------------------------------+--------------------+--------------------+--------------------+-------------------------+-------------------------+--------------------+\n";

    char temp[10]="";
    char anniv[20]="";

    char wilaya[20]="";
    char sang[20] = "";
    char grade[25] = "";
    char force[25] = "";
    char region[20] = "";

    Enr forms;
    Bloc buf;

    //END OF SETUP

    bloc_note = fopen(nom,"w+");
    fprintf(bloc_note,"\t\t%s", khet);
    fprintf(bloc_note,"\t\t|%-20s|%-35s|%-35s|%-20s|%-20s|%-20s|%-25s|%-25s|%-20s|\n","MATRICULE", "NOM", "PRENOM", "DATE DE NAISSANCE", "WILAYA DE NAISSANCE", "GROUPE SANGUIN","GRADE","FORCE ARMEE", "REGION MILITAIRE");
    fprintf(bloc_note,"+---------------%s", khet);

    scan1 = 1;//f.entete.tete;

    while(scan1 != -1)
    {
        LireDir(f, scan1, &buf);
        lire--;lire_total--;

        for(scan2 = 0; scan2<buf.Nb; scan2++)
        {
            forms = buf.Tab[scan2];

            strcpy(anniv, "");

            sprintf(temp,"%d", forms.date.jour);
                strcat(anniv,temp);
                    strcat(anniv,"/");
            sprintf(temp,"%d", forms.date.mois);
                strcat(anniv,temp);
                    strcat(anniv,"/");
            sprintf(temp,"%d", forms.date.annee);
                strcat(anniv,temp);

            wilayaReturner(forms.wilaya, wilaya);
            sang_returner(forms.sang, sang);
            grade_returner(forms.grade, grade);
            force_returner(forms.force, force);
            region_returner(forms.region, region);

            fprintf(bloc_note,"BLOC:%-5d [%-3d]|",scan1, scan2);
            fprintf(bloc_note,"%-20d|%-35s|%-35s|%-20s|%-20s|%-20s|%-25s|%-25s|%-20s|\n", forms.matricule, forms.nom, forms.prenom, anniv, wilaya, sang, grade, force, region );
            fprintf(bloc_note,"+---------------%s", khet);
        }

        scan1=buf.suivant;


    }

    fclose(bloc_note);

}


//_________________________________________________________________________

void representation_globale(Fichier f)
{

  //SETUP

  char oufou9i[] = "\t\t\t+------------------------+\n";
  char amoudi[] = "\t\t\t|            ...         |\n";
  FILE *bloc_notes = NULL;
  Bloc buf;
  //END OF SETUP
  bloc_notes = fopen("bloc_note/rep_glo.txt", "w+");


  for(int i = 1; i<=3; i++)
  {
    fprintf(bloc_notes,"\t\t\tBLOC : %d\n", i);
    fprintf(bloc_notes,"%s", oufou9i);
    LireDir(f, i, &buf);
    lire_total--;

    fprintf(bloc_notes,"\t\t\t|MAT : %d\t         |\n", buf.Tab[0].matricule);
    fprintf(bloc_notes,"%s", oufou9i);

    fprintf(bloc_notes,"%s", amoudi);
    fprintf(bloc_notes,"%s", amoudi);
    fprintf(bloc_notes,"%s", amoudi);

    fprintf(bloc_notes,"%s", oufou9i);
    fprintf(bloc_notes,"\t\t\t|MAT : %d\t         |\n", buf.Tab[buf.Nb-1].matricule);
    fprintf(bloc_notes,"%s", oufou9i);
    fprintf(bloc_notes,"\t\t\t|Nb: %-3d| Suiv: %-9d|\n", buf.Nb, buf.suivant);
    fprintf(bloc_notes,"%s", oufou9i);
    fprintf(bloc_notes,"\t\t\t|\n\t\t\t|\n\t\t\t|\n\t\t\t|\n\t\t\tV\n");

  }

  fclose(bloc_notes);


}

void lobf_Afficheur_V2(Fichier f, char nom[])
{
    //SETUP
    int scan1 = 0, scan2 = 0, cpt=1;
    FILE *bloc_note = NULL;
    char khet[] = "+--------------------+-----------------------------------+-----------------------------------+--------------------+--------------------+--------------------+-------------------------+-------------------------+--------------------+\n";

    char temp[10]="";
    char anniv[20]="";

    char wilaya[20]="";
    char sang[20] = "";
    char grade[25] = "";
    char force[25] = "";
    char region[20] = "";

    Enr forms;
    Bloc buf;

    //END OF SETUP

    bloc_note = fopen(nom,"w+");
    fprintf(bloc_note,"\t\t%s", khet);
    fprintf(bloc_note,"\t\t|%-20s|%-35s|%-35s|%-20s|%-20s|%-20s|%-25s|%-25s|%-20s|\n","MATRICULE", "NOM", "PRENOM", "DATE DE NAISSANCE", "WILAYA DE NAISSANCE", "GROUPE SANGUIN","GRADE","FORCE ARMEE", "REGION MILITAIRE");
    fprintf(bloc_note,"+---------------%s", khet);

    scan1 = 1;//f.entete.tete;

    while(scan1 != -1 && cpt<=3)
    {
        LireDir(f, scan1, &buf);
        lire_total--;
        lire--;

        for(scan2 = 0; scan2<buf.Nb; scan2++)
        {
            forms = buf.Tab[scan2];

            strcpy(anniv, "");

            sprintf(temp,"%d", forms.date.jour);
                strcat(anniv,temp);
                    strcat(anniv,"/");
            sprintf(temp,"%d", forms.date.mois);
                strcat(anniv,temp);
                    strcat(anniv,"/");
            sprintf(temp,"%d", forms.date.annee);
                strcat(anniv,temp);

            wilayaReturner(forms.wilaya, wilaya);
            sang_returner(forms.sang, sang);
            grade_returner(forms.grade, grade);
            force_returner(forms.force, force);
            region_returner(forms.region, region);

            fprintf(bloc_note,"BLOC:%-5d [%-3d]|",scan1, scan2);
            fprintf(bloc_note,"%-20d|%-35s|%-35s|%-20s|%-20s|%-20s|%-25s|%-25s|%-20s|\n", forms.matricule, forms.nom, forms.prenom, anniv, wilaya, sang, grade, force, region );
            fprintf(bloc_note,"+---------------%s", khet);
        }

        scan1=buf.suivant;

        cpt++;
    }

    fclose(bloc_note);

}

