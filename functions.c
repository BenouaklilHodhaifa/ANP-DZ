#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // pour verifier si un dossier existe
#include "machine_abstraite.h"
#include "machine_abstraite_lof.h"
#include "gestion_index.h"
#include "bloce_notes.h"
#include "forms.h"
#define B 111

extern int lire; extern int ecr; extern int lire_total; extern int ecr_total;



//___________________________________________________________________________________________
//                               RECHERCHE D'UN ENR DANS LE FOCHIER L÷F
//___________________________________________________________________________________________

int recherche(Fichier F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc, int matricule, int *i, int *j)
{
    int case_mc=0; char trouv=0;
    BLOC_INDEX buf_bloc_index;

     // RECHERCHE DE L'ENREGISTREMENT
    rech_dicho_mc(index_mc,&case_mc,matricule);
    //LireDir_lof(*index_ms, index_mc->tab[case_mc].adr_bloc_ind, &buf_bloc_index);
    buf_bloc_index = *(index_mc->tab[case_mc].adr_mc);
    rech_dicho_buf_ind(&buf_bloc_index, matricule, &trouv, j);


    if(trouv == 0){return 0;}

    else if(trouv == 1)
    {
        *i = buf_bloc_index.tab[*j].adr_bloc_dat;
        *j = buf_bloc_index.tab[*j].adr_dep_dat;
        return 1;
    }

};

//____________________________________________________________________________________________
//                              INSERTION DANS LE FICHIER L÷F
//____________________________________________________________________________________________

int insertion(Enr enregisrement, Fichier *F,FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc )
{
    //SETUP
    char trouv=1;
    int i=0, j=0,case_mc=0;
    lire=0; ecr=0;
    Bloc buf;
    BLOC_INDEX buf_bloc_index;
    //

    if(F->f == NULL)
    {
        return 0;
    }
    else
    {


        rech_dicho_mc(index_mc,&case_mc,enregisrement.matricule);
        //LireDir_lof(*index_ms, index_mc->tab[case_mc].adr_bloc_ind, &buf_bloc_index);
        buf_bloc_index = *(index_mc->tab[case_mc].adr_mc);
        rech_dicho_buf_ind(&buf_bloc_index, enregisrement.matricule, &trouv, &j);

        if(trouv == 1)
        {
            return 0;
        }
        // else if( trouv ==0 )
        LireDir(*F,Entete(*F,2),&buf);
        if (buf.Nb < B)
        {
            i = Entete(*F,2);
            j = buf.Nb ;
            buf.Tab[buf.Nb ] = enregisrement;
            buf.Nb++;
            EcrireDir(*F,Entete(*F,2),&buf);
        }
        else if(buf.Nb == B)
        {
            i = Alloc_Bloc(F);
            buf.suivant = i;
            EcrireDir(*F,Entete(*F,2)-1,&buf);
            Aff_entete(F,2,i);
            F->changed = 1;
            j = 0;
            buf.Tab[j]= enregisrement;
            buf.Nb = 1;
            buf.suivant = -1;
            EcrireDir(*F,Entete(*F,2),&buf);
        }
        insertion_lof_v1_1(index_ms, index_mc, enregisrement.matricule, i, j);
        //insertion_lof(index_ms,index_mc,enregisrement.matricule,i,j);
        return 1;

    }
};// END OF FUN



//_______________________________________________________________________________________________
//                                  SUPPRESSION DANS LE FICHIER L÷F
//_______________________________________________________________________________________________

int suppression(int matricule,Fichier *F,FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc)
{
    //SETUP
    char trouv=1;
    lire=0; ecr=0;
    int j=0, case_mc=0 , adr_bloc_suppr= 0, adr_dep_suppr= 0;
    Enr dernier_enr;
    Bloc buf1;
    Bloc buf2;
    BLOC_INDEX buf_bloc_index;
    //

    if (matricule> index_mc->tab[index_mc->sup].cle)
        return 0;


      // RECHERCHE DE L'ENREGISTREMENT ‡ SUPPRIMER
    //trouv = recherche(*F, index_ms, index_mc, matricule, &i, &j);
    rech_dicho_mc(index_mc,&case_mc,matricule);
    //LireDir_lof(*index_ms, index_mc->tab[case_mc].adr_bloc_ind, &buf_bloc_index);
    buf_bloc_index = *(index_mc->tab[case_mc].adr_mc);
    rech_dicho_buf_ind(&buf_bloc_index, matricule, &trouv, &j);

    if (trouv == 0)
    {
        return 0; // il n'existe pas
    }

    LireDir(*F,Entete(*F,2),&buf2); // LIRE LE DERNIER BLOC

    dernier_enr = buf2.Tab[buf2.Nb -1];
    buf2.Nb--;
    // TRAITEMENT DU DERNIER BLOC
    if(buf2.Nb > 0)
    {
        EcrireDir(*F, Entete(*F,2), &buf2);
    }
    else // le dernier bloc est vide
    {
        int avant_q =-1/*, suivant=-1*/;

        /*LireDir(*F,T,&buf1);
        suivant = buf1.suivant;
        while (suivant != -1) // TROUVER LE N∞ DE BLOC AVANT LA QEUE ( je peut modifier la structure
                              //                                       de l'entete de L÷F pour stocker le avant qeue )
        {
            avant_q = T;
            T = suivant;
            LireDir(*F,T,&buf1);
            suivant = buf1.suivant;
        }*/
        avant_q = Entete(*F,2)-1;

        if (avant_q == 0)
        {
            Aff_entete(F,1,-1);
            Aff_entete(F,2,-1);
            F->changed = 1;
            return 1;
        }

        F->changed = 1;
        Aff_entete(F,2,avant_q);
        LireDir(*F, avant_q,&buf1);
        buf1.suivant = -1;
        EcrireDir(*F, avant_q, &buf1);
    }
    // FIN DE TRAITEMENT DU DERNIER BLOC

     // ECRASEMENT DE L'ENREGISTREMENT ‡ SUPPRIMER
    adr_bloc_suppr = buf_bloc_index.tab[j].adr_bloc_dat ;
    adr_dep_suppr = buf_bloc_index.tab[j].adr_dep_dat;


    LireDir(*F,adr_bloc_suppr, &buf1); // LIRE LE BLOC DE L'ENR ‡ SUPPRIMER
    buf1.Tab[adr_dep_suppr] = dernier_enr;
    EcrireDir(*F,adr_bloc_suppr, &buf1);

    suppression_lof_v1_1(index_ms, index_mc, matricule);
    //suppression_lof(index_ms, index_mc, matricule);

    /*
     la modification dans la LOF pour le dernier enr qu'on a ecraser avec l'enr ‡ supprimer
     */

    rech_dicho_mc(index_mc,&case_mc,dernier_enr.matricule);
    //LireDir_lof(*index_ms, index_mc->tab[case_mc].adr_bloc_ind, &buf_bloc_index);
    buf_bloc_index = *(index_mc->tab[case_mc].adr_mc);
    rech_dicho_buf_ind(&buf_bloc_index, matricule, &trouv, &j);

    buf_bloc_index.tab[j].adr_bloc_dat = adr_bloc_suppr;
    buf_bloc_index.tab[j].adr_dep_dat = adr_dep_suppr;

    EcrireDir_lof(*index_ms, index_mc->tab[case_mc].adr_bloc_ind, &buf_bloc_index);


    return 1;

};// END OF FUN

//____________________________________________________________________________________________
//                                 MODIFICATIONS DE LA REGION MILITAIRE
//____________________________________________________________________________________________


int modification_rg_ml (int matricule, char region, Fichier F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc)
{

    char trouv=0;
    lire=0; ecr=0;
    int case_mc=0,j=0 ;
    BLOC_INDEX buf_bloc_index;
    Bloc buf;

    if ( matricule> index_mc->tab[index_mc->sup].cle )
        return 0;

      // RECHERCHE DE L'ENREGISTREMENT ‡ MODIFIER

    rech_dicho_mc(index_mc,&case_mc,matricule);
    //LireDir_lof(*index_ms, index_mc->tab[case_mc].adr_bloc_ind, &buf_bloc_index);
    buf_bloc_index = *(index_mc->tab[case_mc].adr_mc);
    rech_dicho_buf_ind(&buf_bloc_index, matricule, &trouv, &j);

    if(trouv == 0 )
    {
        return 0;
    }
    LireDir(F,buf_bloc_index.tab[j].adr_bloc_dat, &buf);

    buf.Tab[buf_bloc_index.tab[j].adr_dep_dat ].region = region;

    EcrireDir(F,buf_bloc_index.tab[j].adr_bloc_dat, &buf);

    return 1;

};// END OF FUN

//_____________________________________________________________________________________________________
//                   4.6 CONSULTER LES ENR APPARTENANT ‡ UNE CATÈGORIE DE GRADES DONNÈE
//_____________________________________________________________________________________________________

/*
    officier generaux: 1, 2, 3;
    officiers-supÈrieurs: 4, 5, 6;
    officiers: 7, 8, 9, 10;
    sous-officiers: 11, 12, 13, 14;
    hommes de troupes: 15, 16, 17;

*/

void consulter_ctg_grade(Fichier F, int ctg_grade)
{
    // CONSULTER TOUS LES ENR D'UNE CATEGORIE DE GRADE DONNEE

  int T=-1, i=0, j=1;
  lire=0; ecr=0;
  char grade;

  FILE *bloc_note = NULL;
  char khet[] = "+--------------------+-----------------------------------+-----------------------------------+--------------------+--------------------+--------------------+-------------------------+-------------------------+--------------------+\n";
  char temp[20]="";
  char anniv[20]="";
  char wilaya[20]="";

    char sang[20] = "";
    char gr[25] = "";
    char force[25] = "";
    char region[20] = "";

  char correspond = 0;

  Enr forms;


  Bloc buf;
  //END OF SETUP

  bloc_note = fopen("bloc_note/ctg_grade.txt","w+");

  fprintf(bloc_note,"%s", khet);
  fprintf(bloc_note,"|%-20s|%-35s|%-35s|%-20s|%-20s|%-20s|%-25s|%-25s|%-20s|\n","MATRICULE", "NOM", "PRENOM", "DATE DE NAISSANCE", "WILAYA DE NAISSANCE", "GROUPE SANGUIN","GRADE","FORCE ARMEE", "REGION MILITAIRE");
  fprintf(bloc_note,"%s", khet);



  T = Entete(F,1);


  while(T != -1)
  {
      j = 0;
      LireDir(F, T, &buf);
      for(i=0 ;i<buf.Nb ; i++)
      {
          grade = buf.Tab[j].grade;

          if ( ( (grade==1) || (grade==2) || (grade==3) ) && (ctg_grade == 1) )
          {
             correspond = 1;
          }
          else if( ( (grade==4) || (grade==5) || (grade==6) ) && (ctg_grade == 2) )
              {
                correspond = 1;
              }
              else if( ( (grade==7) || (grade==8) || (grade==9) || (grade==10) ) && (ctg_grade == 3) )
                  {
                    correspond = 1;
                  }
                  else if( ( (grade==11) || (grade==12) || (grade==13) || (grade==14) ) && (ctg_grade == 4) )
                      {
                        correspond = 1;
                      }
                      else if( ( (grade==15) || (grade==16) || (grade==17) ) && (ctg_grade == 5) )
                          {
                            correspond = 1;
                          }
        if(correspond == 1)
        {
            correspond = 0;

            forms = buf.Tab[j];

            strcpy(anniv, "");

            sprintf(temp,"%d", forms.date.jour);
                    strcat(anniv,temp);
                        strcat(anniv,"/");
            sprintf(temp,"%d", forms.date.mois);
                    strcat(anniv,temp);
                        strcat(anniv,"/");
            sprintf(temp,"%d", forms.date.annee);
                        strcat(anniv, temp);

            wilayaReturner(forms.wilaya, wilaya);
            sang_returner(forms.sang, sang);
            grade_returner(forms.grade, gr);
            force_returner(forms.force, force);
            region_returner(forms.region, region);

            fprintf(bloc_note,"|%-20d|%-35s|%-35s|%-20s|%-20s|%-20s|%-25s|%-25s|%-20s|\n", forms.matricule, forms.nom, forms.prenom, anniv, wilaya, sang, gr, force, region );
            fprintf(bloc_note,"%s", khet);
        }

          j++;
      }

      T = buf.suivant;
  }

  fclose(bloc_note);


};// END OF FUN

//______________________________________________________________________________________________________________________
//                                  FRAGMENTER PERSONNEL-ANP_DZ.BIN
//______________________________________________________________________________________________________________________


void remplir_fich(Fichier *F, Enr e, int *nb_bloc, Bloc *buf)
{
    // FONCTION QUI REMPLISSENT UN FICHIER AVEC DES PARAMETRES ON ENTREE
    // SON "F" , L'ENR , LE NOMBRE DE BLOC O˘ ON VA ECRIRE ET LE BUFFER

  if(buf->Nb == B-1) // le buf est remplis
    {
        buf->Tab[buf->Nb] = e;
        //Aff_entete(F,2,*nb_bloc);
        buf->suivant = Alloc_Bloc(F);
        buf->Nb++;
        EcrireDir(*F,*nb_bloc,buf);
        *nb_bloc = buf->suivant;
        buf->Nb = 0;
    }
    else //remplissage du buf
    {
        buf->Tab[buf->Nb] = e;
        buf->Nb++;
    }
};




void fragmenter(Fichier F, Fichier *F1, Fichier *F2, Fichier *F3, Fichier *F4, Fichier *F5, Fichier *F6  )
{
    // FRAGMENTER F SELON LA REGION MILITAIRE DANS 6 FICHIER F1,F2,...,F6

    // SETUP
    int T=-1, i=0, j=0;
    lire=0; ecr=0;
    int  nb_bloc, nb_bloc1=0, nb_bloc2=0, nb_bloc3=0, nb_bloc4=0, nb_bloc5=0, nb_bloc6=0;
    char region;
    Bloc buf, buf1, buf2, buf3, buf4, buf5, buf6;
    Fichier fich;
    // END OF SETUP


    T = Entete(F,1); // LA TETE DE LA L÷F DANS F

    if ( access("fragmenter",F_OK) ) // VERIFIER SI LE DOSSIER "FRAGMENTER" EXISTE
    {
        system("mkdir fragmenter");
    }

      // INITIALISATIONS

    Ouvrir(F1, "fragmenter/F1.bin", 'N');
    Ouvrir(F2, "fragmenter/F2.bin", 'N');
    Ouvrir(F3, "fragmenter/F3.bin", 'N');
    Ouvrir(F4, "fragmenter/F4.bin", 'N');
    Ouvrir(F5, "fragmenter/F5.bin", 'N');
    Ouvrir(F6, "fragmenter/F6.bin", 'N');

    nb_bloc1 = Alloc_Bloc(F1);
    nb_bloc2 = Alloc_Bloc(F2);
    nb_bloc3 = Alloc_Bloc(F3);
    nb_bloc4 = Alloc_Bloc(F4);
    nb_bloc5 = Alloc_Bloc(F5);
    nb_bloc6 = Alloc_Bloc(F6);

    buf.Nb = 0;
    buf1.Nb = 0;
    buf2.Nb = 0;
    buf3.Nb = 0;
    buf4.Nb = 0;
    buf5.Nb = 0;
    buf6.Nb = 0;

    buf.suivant = -1;
    buf1.suivant = -1;
    buf2.suivant = -1;
    buf3.suivant = -1;
    buf4.suivant = -1;
    buf5.suivant = -1;
    buf6.suivant = -1;

      // FIN DES INITIALISATIONS

    while(T != -1) // VISITER TOUS LES ENR ET SELON SA REGION ON VA L'ECRIRE DANS UN FICHIER
    {
        j = 0;
        LireDir(F, T, &buf);

        for(i=0 ;i<buf.Nb ; i++)
        {
           region = buf.Tab[j].region;
           if(region == 1)
           {
                remplir_fich(F1, buf.Tab[j], &nb_bloc1, &buf1);
           }
           else if(region == 2)
           {
                remplir_fich(F2, buf.Tab[j], &nb_bloc2, &buf2);
           }
           else if(region == 3)
           {
                remplir_fich(F3, buf.Tab[j], &nb_bloc3, &buf3);
           }
           else if(region == 4)
           {
                remplir_fich(F4, buf.Tab[j], &nb_bloc4, &buf4);
           }
           else if(region == 5)
           {
                remplir_fich(F5, buf.Tab[j], &nb_bloc5, &buf5);
           }
           else if(region == 6)
           {
                remplir_fich(F6, buf.Tab[j], &nb_bloc6, &buf6);
           }

           j++;
        }


        T = buf.suivant;
    }

    // TRAITEMENT DES DERNIER BUFFERS

    for(i=1; i<=6; i++)
    {
        switch(i)
        {
        case 1:
            buf = buf1;
            fich = *F1;
            nb_bloc = nb_bloc1;
            break;
        case 2:
            buf = buf2;
            fich = *F2;
            nb_bloc = nb_bloc2;
            break;
        case 3:
            buf = buf3;
            fich = *F3;
            nb_bloc = nb_bloc3;
            break;
        case 4:
            buf = buf4;
            fich = *F4;
            nb_bloc = nb_bloc4;
            break;
        case 5:
            buf = buf5;
            fich = *F5;
            nb_bloc = nb_bloc5;
            break;
        case 6:
            buf = buf6;
            fich = *F6;
            nb_bloc = nb_bloc6;
            break;
        }


        if(buf.Nb == 0)
        {
            Aff_entete(&fich,2,Entete(fich,2)-1);
            LireDir(fich,Entete(fich,2),&buf);
            buf.suivant = -1;
            EcrireDir(fich,Entete(fich,2),&buf);
        }
        else
        {
            buf.suivant = -1;
            EcrireDir(fich, nb_bloc,&buf);
        }

    }

    lobf_Afficheur(*F1, "fragmenter/region1.txt");
    lobf_Afficheur(*F2, "fragmenter/region2.txt");
    lobf_Afficheur(*F3, "fragmenter/region3.txt");
    lobf_Afficheur(*F4, "fragmenter/region4.txt");
    lobf_Afficheur(*F5, "fragmenter/region5.txt");
    lobf_Afficheur(*F6, "fragmenter/region6.txt");



};// END OF FUN


void ageRegion(Fichier f,char region, int ageMin, int ageSup)
{
     //Setup
     int scan1= 0, scan2 = 0;
     lire=0; ecr=0;
     FILE *bloc_note = NULL;
     char khet[] = "+--------------------+-----------------------------------+-----------------------------------+--------------------+--------------------+--------------------+-------------------------+-------------------------+--------------------+\n";

     char temp[20]="";
     char anniv[20]="";

     char wilaya[20]="";
     char sang[20] = "";
     char grade[25] = "";
     char force[25] = "";
     char reg[20] = "";

     Enr forms;
     Bloc buf;
     //END OF SETUP


    bloc_note = fopen("bloc_note/age_region.txt","w+");

    fprintf(bloc_note,"%s", khet);
    fprintf(bloc_note,"|%-20s|%-35s|%-35s|%-20s|%-20s|%-20s|%-25s|%-25s|%-20s|\n","MATRICULE", "NOM", "PRENOM", "DATE DE NAISSANCE", "WILAYA DE NAISSANCE", "GROUPE SANGUIN","GRADE","FORCE ARMEE", "REGION MILITAIRE");
    fprintf(bloc_note,"%s", khet);

     scan1 = f.entete.tete;
     while(scan1 != -1)
     {
            LireDir(f, scan1, &buf);
            for(scan2 = 0; scan2<buf.Nb; scan2++)
            {
                if( (buf.Tab[scan2].region==region) && ( 2022-(buf.Tab[scan2].date.annee)<=ageSup ) && ( 2022-(buf.Tab[scan2].date.annee)>=ageMin ) )
                     {
                         forms = buf.Tab[scan2];

                        strcpy(anniv,"");

                        sprintf(temp,"%d", forms.date.jour);
                            strcat(anniv,temp);
                                strcat(anniv,"/");
                        sprintf(temp,"%d", forms.date.mois);
                            strcat(anniv,temp);
                                strcat(anniv,"/");
                        sprintf(temp,"%d", forms.date.annee);
                            strcat(anniv,temp);
                            strcat(anniv,"-(");
                        sprintf(temp, "%d", 2022-forms.date.annee);
                            strcat(anniv,temp);
                            strcat(anniv,"ans)");

                        wilayaReturner(forms.wilaya, wilaya);

                        sang_returner(forms.sang, sang);
                        grade_returner(forms.grade, grade);
                        force_returner(forms.force, force);
                        region_returner(forms.region, reg);

                        fprintf(bloc_note,"|%-20d|%-35s|%-35s|%-20s|%-20s|%-20s|%-25s|%-25s|%-20s|\n", forms.matricule, forms.nom, forms.prenom, anniv, wilaya, sang, grade, force, reg );
                        fprintf(bloc_note,"%s", khet);

                     }
            }

        scan1 = buf.suivant;
     }

  fclose(bloc_note);

}//END OF FUN


void faRemover(Fichier *f, char fa, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc)
{

   //Setup
   int i = 0, j = 0, iq = 0, jq = 0;
   lire=0; ecr=0;
   char moveOn = 0, deleteOnce = 0;
   Bloc buf, bufq;

   //End of setup

   if(f->entete.queue != -1) //si le fichier n'est pas vide
   {

      i = f->entete.tete;
      LireDir(*f, i, &buf);
//      nbre++;
      j = 0;

      iq = f->entete.queue;
      LireDir(*f, iq, &bufq);
  //    nbre++;
      jq = bufq.Nb-1;

      while ( i!=iq || j!= jq)
       {

           if(buf.Tab[j].force == fa)
           {
                if(bufq.Tab[jq].force != fa)
                {
                        suppression_lof_v2('N', index_ms, index_mc, buf.Tab[j].matricule, 0, 0);
                        buf.Tab[j] = bufq.Tab[jq];
                        suppression_lof_v2('M', index_ms, index_mc, bufq.Tab[jq].matricule, i, j);

                    moveOn = 1;
                    deleteOnce = 1;
                }
                else
                {
                    suppression_lof_v2('N', index_ms, index_mc, bufq.Tab[jq].matricule, 0, 0);
                }

                bufq.Nb--;
                jq--;

                if (jq == -1)
                {
                    iq--;
                    f->entete.queue = iq;
                    LireDir(*f, iq, &bufq);
    //                nbre++;
                    jq = bufq.Nb-1;
                }

                if( (moveOn == 1) && (i!=iq || j!= jq) )
                {
                    moveOn = 0;
                    j++;
                        if(j == B)
                        {
                            if(deleteOnce == 1)
                                {
                                    EcrireDir(*f, i ,&buf);
      //                              nbre++;
                                }
                            deleteOnce = 0;
                            i++;
                            LireDir(*f, i, &buf);
        //                    nbre++;
                            j = 0;
                        }
                }

           }
           else
           {
                j++;
                if(j == B)
                {
                    if(deleteOnce == 1)
                       {
                        EcrireDir(*f, i ,&buf);
          //              nbre++;
                       }
                    deleteOnce = 0;
                    i++;
                    LireDir(*f, i, &buf);
            //        nbre++;
                    j = 0;
                }

           }


       }

       if(buf.Tab[j].force == fa)
       {
           buf.Nb = j; //buf.nb -- en fin de compte
           suppression_lof_v2('N', index_ms, index_mc, buf.Tab[j].matricule, 0, 0);
           if(buf.Nb == 0)
           {
                f->entete.queue--;
                if(f->entete.queue != 0)
                {
                    LireDir(*f, f->entete.queue, &buf);
              //      nbre++;
                    buf.suivant = -1;
                    EcrireDir(*f, f->entete.queue, &buf);
                //    nbre++;
                }
                else
                {
                    f->entete.queue = -1;
                    f->entete.tete = -1;
                }
           }
           else
           {
                buf.suivant = -1;
                EcrireDir(*f, i, &buf);
               // nbre++;
           }
        }
        else
        {
            buf.Nb = j+1;
            buf.suivant = -1;
            EcrireDir(*f, i, &buf);
           // nbre++;
        }

       Fermer(*f);

       suppression_lof_v2('F', index_ms, index_mc, 0, 0, 0);

       Ouvrir(f,"PERSONNEL-ANP_DZ.bin",'A');


   }


};

//______________________________________________________________________________________
//                  VERIFIE QUE LES FICHIERS EXISTS ET LES CHARGENT
//______________________________________________________________________________________

int ancienCreator(Fichier *f, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc)
{
    int etat = 0;

    etat = Ouvrir(f,"PERSONNEL-ANP_DZ.bin",'A');
    if(etat == 1)
        return 0;
    etat = Ouvrir_lof(index_ms, "index_ms.bin", 'A');
    if(etat == 1)
        return 0;
    etat = chaDecha('C', *index_ms, index_mc);
    if(etat == 1)
        return 0;

    return 1;
}




