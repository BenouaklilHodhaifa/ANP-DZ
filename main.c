#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // pour verifier si un dossier existe
#include "machine_abstraite.h"
#include "machine_abstraite_lof.h"
#include "gestion_index.h"
#include "charg_init.h"
#include "functions.h"
#include "graphics.h"
#include "conio2.h"

int lire_total=0, ecr_total=0, lire=0, ecr=0;

int main()
{
     //setup
    srand(time(NULL)) ;
    Fichier F;
    FICHIER_LOF index_ms;
    INDEX_NON_DENSE index_mc;
    Fichier F1, F2, F3, F4, F5, F6;
    int choix;

    system("mode con LINES=50 COLS=170");
    system("color f0");
    textcolor(BLACK);textbackground(WHITE);

    if ( access("main_fich",F_OK) ) // VERIFIER SI LE DOSSIER "main_fich" EXISTE
    {
        system("mkdir main_fich");
    }
     if ( access("bloc_note",F_OK) ) // VERIFIER SI LE DOSSIER "bloc_note" EXISTE
    {
        system("mkdir bloc_note");
    }

    startup(); // animation d'entrée

landing:
    choix = landing();
    switch(choix)
    {
    case 1:
        affichage_charg_init(&F, &index_ms, &index_mc);
        break;
    case 2:
        if ( !affichage_ancien_fich(&F, &index_ms, &index_mc) )
            goto landing;
        break;
    case 3:
        goto fin;
        break;
    default:
        goto fin;
        break;
    }


menu_principal:

    choix = menu_principal();
    switch(choix)
    {
    case 1:
        affichage_insertion(&F, &index_ms, &index_mc);
        goto menu_principal;
        break;
    case 2:
        affichage_suppression(&F, &index_ms, &index_mc);
        goto menu_principal;
        break;
    case 3:
        affichage_recherche(F, &index_ms, &index_mc);
        goto menu_principal;
        break;
    case 4:
        affichage_modification_rg_ml(F, &index_ms, &index_mc);
        goto menu_principal;
        break;
    case 5:
        affichage_suppr_fa(&F, &index_ms, &index_mc);
        goto menu_principal;
        break;
    case 6:
        affichage_requete_intervalle(F);
        goto menu_principal;
        break;
    case 7:
        affichage_consulter_ctg_grd(F);
        goto menu_principal;
        break;
    case 8:
        affichage_fragmenter(F, &F1, &F2, &F3, &F4, &F5, &F6);
        goto menu_principal;
        break;
    case 9:
        affichage_qst5(F);
        goto menu_principal;
        break;
    case 10:
        goto landing;
        break;
    default:
        goto fin;
        break;
    }




fin:      // Sauvegarde des données
    chaDecha('D',index_ms, &index_mc);
    Fermer(F);
    Fermer(F1);
    Fermer(F2);
    Fermer(F3);
    Fermer(F4);
    Fermer(F5);
    Fermer(F6);
    Fermer_lof(index_ms);

    return 0;
}




