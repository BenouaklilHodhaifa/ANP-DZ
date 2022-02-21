#include <stdio.h>
#include <stdlib.h>
#include "machine_abstraite_lof.h"
#define B_FICHIER_INDEX_MC 1062


void rech_dicho_mc(INDEX_NON_DENSE *index_mc, int *case_ind_mc, int const cle)
{
    //SETUP
    char trouv = 0;
    int bsup = 0, binf = 0;
    int comp = 0; //comparateur
    int i = 0;
    //END_OF_SETUP

    if( index_mc->sup == -1)//hadi ta3 on sait jamais
        *case_ind_mc = 0;
    else
    {
        bsup = index_mc->sup;
        while( (bsup >= binf) && (!trouv) )
        {
                i = (bsup+binf)/2;
                comp = index_mc->tab[i].cle;

                if( comp == cle)
                    trouv = 1;
                else if (comp < cle)
                    binf = i+1;
                else
                    bsup = i-1;

        }

        if (trouv == 1)
            *case_ind_mc = i;
        else if(binf <= index_mc->sup)
            *case_ind_mc = binf;
        else
            *case_ind_mc =index_mc->sup; //qui correspond a binf - 1 on est d'accord...
    }
}

//_____________________________________________________________________________________________________

void rech_dicho_buf_ind(BLOC_INDEX *buf, int cle, char *trouv, int *adr_dep)//rercherche dichotomique dans le buffer de l'index en ms, utile pour les cle unique seulement !
{
    //SETUP
    *trouv = 0;
    int bsup = (buf->Nb)-1, binf = 0;
    int comp = 0; //comparateur
    int j = 0;
    //END_OF_SETUP

    while( (bsup >= binf) && (!(*trouv)) )
    {
        j = (bsup+binf)/2;
        comp = buf->tab[j].cle;

        if( comp == cle)
            *trouv = 1;
        else if (comp < cle)
            binf = j+1;
        else
            bsup = j-1;

    }

    if(*trouv == 1)
        *adr_dep = j;
    else
        *adr_dep = binf;

}

//________________________________________________________________________________________________________

void insertion_lof_v5(char mode, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc, int cle, int adr_bloc_dat, int adr_dep_dat )//SR pour Sans Repetition
{
    //SETUP
    int case_mc = 0, adr = 0,adr2 = 0, k = 0,j = 0;
    char trouv = 0;
    static int ara = 1; //actual read adress

    BLOC_INDEX *ptr = NULL, *ptr2 = NULL;
    static BLOC_INDEX *ara_mc = NULL; //ara dans mc

    CCC le_debordant;
    CCB forms, switcher;
    static BLOC_INDEX buf_bloc_index;


    //END_OF_SETUP

    if(mode == 'N')//mode normal
    {

            if(index_mc->sup == -1) //le fichier est encore vide ou s'est vidé par suppression ...
            {
                //On remplie la premiere case du buf
                buf_bloc_index.Nb = 1;
                buf_bloc_index.tab[0].cle = cle;
                buf_bloc_index.tab[0].adr_bloc_dat = adr_bloc_dat;
                buf_bloc_index.tab[0].adr_dep_dat = adr_dep_dat;
                buf_bloc_index.suivant = -1;

                //On alloue un emplacement pour le buf chez index_ms
                adr = AllocBloc_lof(index_ms);
                //en mc aussi
                ara_mc = malloc(sizeof(BLOC_INDEX));

                Aff_entete_lof(index_ms, 1, adr); //T
                Aff_entete_lof(index_ms, 2, adr); //Q

                //On met a jour l'index en mc
                index_mc->sup = 0;
                index_mc->tab[0].cle = cle;
                index_mc->tab[0].adr_bloc_ind = adr;
                index_mc->tab[0].adr_mc = ara_mc;// adr_mc aussi
            }
            else//le fichier n'est pas vide
            {
                //On fait une recherche dicho de "cle" dans index_mc, on recupere la case du tab dans "case_mc"
                rech_dicho_mc(index_mc, &case_mc, cle);

                //Si le bloc a lire n'est pas celui actuellement lu
                if( ( index_mc->tab[case_mc].adr_bloc_ind ) != ara)
                {
                  //j 'ecris en mc l'actuellement lu
                  *ara_mc = buf_bloc_index;
                  //je recupere l'adr_mc de celui qu'il faut lire
                   ptr = index_mc->tab[case_mc].adr_mc;
                  //je lis depuis mc
                   buf_bloc_index = *ptr;
                   //je mets a jour les ara
                   ara_mc = ptr;
                   ara =  index_mc->tab[case_mc].adr_bloc_ind;
                }

                //On rech dicho dans "buf" la "cle", on recupere "trouv" et l'indice dans le tab "j"
                rech_dicho_buf_ind(&buf_bloc_index, cle, &trouv, &j);

                if(!trouv)//on ne trouve pas donc on insere
                {
                    //Cas n°1 : j est vers l'avant et il y a de la place
                    if( ( j>(buf_bloc_index.Nb-1) ) && j<B_INDEX_MS)
                    {
                       buf_bloc_index.tab[j].cle = cle;
                       buf_bloc_index.tab[j].adr_bloc_dat = adr_bloc_dat;
                       buf_bloc_index.tab[j].adr_dep_dat = adr_dep_dat;
                       buf_bloc_index.Nb++;
                       //on met a jour la cle dans index_mc
                       index_mc->tab[case_mc].cle = cle;

                    }

                    else
                    {
                        if( j<= (buf_bloc_index.Nb-1) )
                        {

                            //On sauvegarde le dernier CCC de tab de buf
                            le_debordant = buf_bloc_index.tab[buf_bloc_index.Nb-1];

                            //On decale tout le monde a partir de j vers l'avant
                            k = buf_bloc_index.Nb-1;

                            while(k != j)
                            {
                                buf_bloc_index.tab[k] = buf_bloc_index.tab[k-1];
                                k--;
                            }

                            //On remplie la case tab[j] du buf avec le CCC a inserer
                            buf_bloc_index.tab[j].cle = cle;
                            buf_bloc_index.tab[j].adr_bloc_dat = adr_bloc_dat;
                            buf_bloc_index.tab[j].adr_dep_dat = adr_dep_dat;

                        }
                        else
                        {
                            le_debordant.cle = cle;
                            le_debordant.adr_bloc_dat = adr_bloc_dat;
                            le_debordant.adr_dep_dat = adr_dep_dat;
                        }

                        if(buf_bloc_index.Nb != B_INDEX_MS)  //Si il y a de la place dans le buf
                        {
                            buf_bloc_index.Nb++;
                            buf_bloc_index.tab[buf_bloc_index.Nb-1] = le_debordant;

                        }

                        else //Sinon on scinde en deux le bloc
                        {
                            //On garde la premiere moitie dans le vieux bloc
                            buf_bloc_index.Nb = B_INDEX_MS/2; //b div 2
                            //On sauvegarde le suiv du vieux pour que le new puisse pointer dessus apres
                            adr2 = buf_bloc_index.suivant;

                            //On met dans le suiv du vieux l'adr du new
                            adr = AllocBloc_lof(index_ms);
                            buf_bloc_index.suivant = adr;

                            //on alloue pour le nouveau bloc (la deuxieme moitié) un emplacement en mc (pour plus tard)
                            ptr2 = malloc(sizeof(BLOC_INDEX));

                            //on degage le vieux en mc a son adr qui est ara_mc
                            *ara_mc = buf_bloc_index;

                            //On met a jour la cle dominante dans l'index_mc
                            index_mc->tab[case_mc].cle = buf_bloc_index.tab[buf_bloc_index.Nb-1].cle;

                            //On decale la deuxieme moitie vers le debut du buf
                            k = (B_INDEX_MS/2); //b div 2 + 1 et -1 a cause du decalage en C donc b div 2
                            j = 0;
                            while(k < B_INDEX_MS ) //ou k<=3 qui est b-1
                            {
                              buf_bloc_index.tab[j] = buf_bloc_index.tab[k];
                              k++;
                              j++;
                            }
                            //On met le_debordant a sa place dominante
                            buf_bloc_index.tab[j] = le_debordant;
                            //On ajuste le Nb du buf
                            buf_bloc_index.Nb = B_INDEX_MS-(B_INDEX_MS/2)+1; // b- b Div 2 +1(du le_debordant)
                            //On met dans suiv l'adr2 sauvegardé a partir du vieux bloc
                            buf_bloc_index.suivant = adr2;

                            //on met a jour les ara
                            ara = adr; //ms
                            ara_mc = ptr2;//mc

                            //On met a jour l'index_mc
                            k = case_mc+1;
                            forms.cle = le_debordant.cle ;
                            forms.adr_bloc_ind = adr;
                            forms.adr_mc = ara_mc; //champ adr_mc aussi

                            while(k<=index_mc->sup)
                                {
                                    switcher = index_mc->tab[k];
                                    index_mc->tab[k] = forms;
                                    forms = switcher;
                                    k++;
                                }
                            index_mc->tab[k] = forms;
                            index_mc->sup ++;

                        }


                    }

                }

            }

    }

    else //mode flush
    {
        *ara_mc = buf_bloc_index;
        for(int i = 0; i<=index_mc->sup ; i++)
        {

        EcrireDir_lof(*index_ms, index_mc->tab[i].adr_bloc_ind, index_mc->tab[i].adr_mc );
        //nbreLiDi++;
        }

        ara_mc =NULL;
        ptr = NULL;
        ptr2 = NULL;

    }

}//END_OF_FUN

//_____________________________________________________________________________________________________


void insertion_lof_v1_1(FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc, int cle, int adr_bloc_dat, int adr_dep_dat )//SR pour Sans Repetition
{
    //SETUP
    int case_mc = 0, adr = 0,adr2 = 0, k = 0,j = 0;
    char trouv = 0;
    BLOC_INDEX *ptr = NULL, *ptr2 = NULL, *save = NULL;

    CCC le_debordant;
    CCB forms, switcher;
    BLOC_INDEX buf_bloc_index;
    //END_OF_SETUP


            if(index_mc->sup == -1) //le fichier est encore vide ou s'est vidé par suppression ...
            {
                //On remplie la premiere case du buf
                buf_bloc_index.Nb = 1;
                buf_bloc_index.tab[0].cle = cle;
                buf_bloc_index.tab[0].adr_bloc_dat = adr_bloc_dat;
                buf_bloc_index.tab[0].adr_dep_dat = adr_dep_dat;
                buf_bloc_index.suivant = -1;

                //On alloue un emplacement pour le buf chez index_ms
                adr = AllocBloc_lof(index_ms);

                //On ecrit le buffer en ms
                EcrireDir_lof(*index_ms, adr, &buf_bloc_index );
                //nbreLiDi++;

                //On ecrit le bloc en mc
                ptr = malloc(sizeof(BLOC_INDEX)); //on alloue une place
                *ptr = buf_bloc_index;
                //On met a jour l'entete
                Aff_entete_lof(index_ms, 1, adr); //T
                Aff_entete_lof(index_ms, 2, adr); //Q

                //On met a jour l'index en mc
                index_mc->sup = 0;
                index_mc->tab[0].cle = cle;
                index_mc->tab[0].adr_bloc_ind = adr;
                index_mc->tab[0].adr_mc = ptr;//l'adr en mc est la aussi
            }
            else//le fichier n'est pas vide
            {
                //On fait une recherche dicho de "cle" dans index_mc, on recupere la case du tab dans "case_mc"
                rech_dicho_mc(index_mc, &case_mc, cle);

                //On recupere depuis mc le bloc a lire
                ptr = index_mc->tab[case_mc].adr_mc;
                buf_bloc_index = *ptr;

                //On rech dicho dans "buf" la "cle", on recupere "trouv" et l'indice dans le tab "j"
                rech_dicho_buf_ind(&buf_bloc_index, cle, &trouv, &j);

                if(!trouv)//on ne trouve pas donc on insere
                {
                    //Cas n°1 : j est vers l'avant et il y a de la place
                    if( ( j>(buf_bloc_index.Nb-1) ) && j<B_INDEX_MS)
                    {
                       buf_bloc_index.tab[j].cle = cle;
                       buf_bloc_index.tab[j].adr_bloc_dat = adr_bloc_dat;
                       buf_bloc_index.tab[j].adr_dep_dat = adr_dep_dat;
                       buf_bloc_index.Nb++;

                       //On ecrit en ms
                       EcrireDir_lof(*index_ms, index_mc->tab[case_mc].adr_bloc_ind, &buf_bloc_index);
                       //nbreLiDi++;

                       //et on ecrit en mc
                       *ptr = buf_bloc_index;

                       //on met a jour index_mc
                       index_mc->tab[case_mc].cle = cle;
                    }

                    else
                    {
                        if( j<= (buf_bloc_index.Nb-1) )
                        {

                            //On sauvegarde le dernier CCC de tab de buf
                            le_debordant = buf_bloc_index.tab[buf_bloc_index.Nb-1];//------------------******

                            //On decale tout le monde a partir de j vers l'avant
                            k = buf_bloc_index.Nb-1;

                            while(k != j)
                            {
                                buf_bloc_index.tab[k] = buf_bloc_index.tab[k-1];
                                k--;
                            }

                            //On remplie la case tab[j] du buf avec le CCC a inserer
                            buf_bloc_index.tab[j].cle = cle;
                            buf_bloc_index.tab[j].adr_bloc_dat = adr_bloc_dat;
                            buf_bloc_index.tab[j].adr_dep_dat = adr_dep_dat;

                        }
                        else
                        {
                            le_debordant.cle = cle;
                            le_debordant.adr_bloc_dat = adr_bloc_dat;
                            le_debordant.adr_dep_dat = adr_dep_dat;
                        }

                        if(buf_bloc_index.Nb != B_INDEX_MS)  //Si il y a de la place dans le buf
                        {
                            buf_bloc_index.Nb++;
                            buf_bloc_index.tab[buf_bloc_index.Nb-1] = le_debordant;

                            //on ecrit en ms
                            EcrireDir_lof(*index_ms, index_mc->tab[case_mc].adr_bloc_ind, &buf_bloc_index);
                            //nbreLiDi++;

                            //et en mc
                            *ptr = buf_bloc_index;
                        }

                        else //Sinon on scinde en deux le bloc
                        {
                            //On garde la premiere moitie dans le vieux bloc
                            buf_bloc_index.Nb = B_INDEX_MS/2; //b div 2
                            //On sauvegarde le suiv du vieux pour que le new puisse pointer dessus apres
                            adr2 = buf_bloc_index.suivant;
                            //On met dans le suiv du vieux l'adr du new
                            adr = AllocBloc_lof(index_ms);
                            buf_bloc_index.suivant = adr;


                           //on alloue en mc un emplacement pour le new bloc (deuxieme moitie) pour plus tard
                            ptr2 = malloc(sizeof(BLOC_INDEX));//en mc aussi

                           //Finalement on ecrit en ms le vieux bloc
                            EcrireDir_lof(*index_ms, index_mc->tab[case_mc].adr_bloc_ind, &buf_bloc_index);//***********
                            //nbreLiDi++;

                           //et en mc
                            *ptr = buf_bloc_index;

                            //On met a jour la cle dominante dans l'index_mc
                            index_mc->tab[case_mc].cle = buf_bloc_index.tab[buf_bloc_index.Nb-1].cle;

                            //On decale la deuxieme moitie vers le debut du buf
                            k = (B_INDEX_MS/2); //b div 2 + 1 et -1 a cause du decalage en C donc b div 2
                            j = 0;
                            while(k < B_INDEX_MS ) //ou k<=3 qui est b-1
                            {
                              buf_bloc_index.tab[j] = buf_bloc_index.tab[k];
                              k++;
                              j++;
                            }

                            //On met le_debordant a sa place dominante
                            buf_bloc_index.tab[j] = le_debordant;
                            //On ajuste le Nb du buf
                            buf_bloc_index.Nb = B_INDEX_MS-(B_INDEX_MS/2)+1; // b- b Div 2 +1(du le_debordant)
                            //On met dans suiv l'adr2 sauvegardé a partir du vieux bloc
                            buf_bloc_index.suivant = adr2;

                            //On ecrit finalement a l'adr alloué plutot
                            EcrireDir_lof(*index_ms, adr, &buf_bloc_index);
                            //nbreLiDi++;

                            //en mc aussi
                            *ptr2 = buf_bloc_index;

                            //On met a jour l'index_mc
                            k = case_mc+1;
                            forms.cle = le_debordant.cle ;
                            forms.adr_bloc_ind = adr;
                            forms.adr_mc = ptr2;//l'adr en mc...

                            while(k<=index_mc->sup)
                                {
                                    switcher = index_mc->tab[k];
                                    index_mc->tab[k] = forms;
                                    forms = switcher;
                                    k++;
                                }
                            index_mc->tab[k] = forms;
                            index_mc->sup ++;

                        }


                    }

                }

            }


}


//____________________________________________________________________________________________________________


void suppression_lof_v2 (char mode, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc, int const cle, int bloc, int pos)
{
//SETUP
    char trouv = 0;
    int case_mc = 0;
    int j = 0;
    int save = 0;
    static BLOC_INDEX *ara = NULL; //ici ara est ara_mc...

    static BLOC_INDEX buf_bloc_index;


//END_OF_SETUP

        if(mode == 'N' || mode == 'M')
        {

            //Si le fichier n'est pas vide
            if(index_mc->sup != -1)
            {
                //rech dicho de la cle a supprimer dans index_mc pour trouver son "potentiel" bloc
                rech_dicho_mc(index_mc, &case_mc, cle);

                //si le bloc a lire n'est pas celui de l'ara
                if(index_mc->tab[case_mc].adr_mc != ara)
                {
                  if(ara != NULL)
                    *ara = buf_bloc_index;
                  buf_bloc_index = *(index_mc->tab[case_mc].adr_mc);
                  ara = index_mc->tab[case_mc].adr_mc;

                }

                //rech dicho dans le bloc, on recupere trouv et j
                rech_dicho_buf_ind(&buf_bloc_index, cle, &trouv, &j);

                //Pas la peine de verififer si trouv... i guess

                    if(mode == 'N') //si on doit supprimer...
                    {

                            //on l'ecrase avec des decalage
                            while( j!= (buf_bloc_index.Nb-1) )
                            {
                                buf_bloc_index.tab[j] = buf_bloc_index.tab[j+1];
                                j++;
                            }
                            //on oublie pas de mettre a jour le buf.Nb
                            buf_bloc_index.Nb --;

                            //Si le bloc ne se vide pas apres suppression
                            if(buf_bloc_index.Nb != 0)
                            {
                                //on met a jour la cle dominante en index_mc (on sait jamais on a supprimer la dominante)
                                index_mc->tab[case_mc].cle = buf_bloc_index.tab[buf_bloc_index.Nb-1].cle;

                            }
                            //Si le bloc se vide
                            else
                            {
                                //on libere la place qu'occupe le buffer/bloc en mc...
                                free(ara);
                                ara = NULL;
                                //on sauvegarde le suivant du bloc vidé
                                save = buf_bloc_index.suivant;

                                //on fait pointer le nouveau bloc vide vers la QLBV
                                buf_bloc_index.suivant = index_ms->entete.QLBV; //maybe i should use Entete()...Nah
                                //On ecrit le new empty bloc en ms
                                EcrireDir_lof(*index_ms, index_mc->tab[case_mc].adr_bloc_ind, &buf_bloc_index);
                                //nbreLiDifa++;
                                //si la TLBV = QLBV ie ils sont tous les deux vers la fin du fichier
                                if(index_ms->entete.TLBV == index_ms->entete.QLBV)
                                    //On met a jours la TLBV avec l'adr du new empty bloc
                                    index_ms->entete.TLBV = index_mc->tab[case_mc].adr_bloc_ind;
                                //Sinon la LBV n'est pas vide, donc on utilise le bloc Avant_QLBV
                                else
                                {
                                    LireDir_lof(*index_ms, index_ms->entete.AvantQ, &buf_bloc_index);
                                    //nbreLiDifa++;
                                    buf_bloc_index.suivant = index_mc->tab[case_mc].adr_bloc_ind;
                                    EcrireDir_lof(*index_ms, index_ms->entete.AvantQ, &buf_bloc_index);
                                    //nbreLiDifa++;
                                }
                                    //On met a jour le AvantQ qui doit toujours correspondre au new empty bloc
                                    index_ms->entete.AvantQ = index_mc->tab[case_mc].adr_bloc_ind ;

                                //si le new empty bloc est la tete T, on la met a jour avec save
                                if(index_ms->entete.T == index_mc->tab[case_mc].adr_bloc_ind)
                                    index_ms->entete.T = save;
                                //Sinon on recupere le bloc qui pointait vers le new empty bloc et on le fait pointer vers save
                                else
                                {
                                     //le bloc before new empty bloc se situe a case_mc-1
                                    buf_bloc_index = *(index_mc->tab[case_mc-1].adr_mc);
                                    //on met a jour l'ara (puisque maintenant on a lu le before new empty bloc
                                    ara = index_mc->tab[case_mc-1].adr_mc;
                                    //on ajuste le suivant avec save
                                    buf_bloc_index.suivant = save;
                                }

                                //On met a jours l'index_mc en ecrasant le new empty bloc
                                while(case_mc != index_mc->sup)
                                {
                                    index_mc->tab[case_mc] = index_mc->tab[case_mc + 1];
                                    case_mc++;
                                }
                                index_mc->tab[index_mc->sup].adr_mc = NULL;
                                index_mc->sup --;
                            }
                    }
                    else // on doit modifier
                    {
                            buf_bloc_index.tab[j].adr_bloc_dat = bloc;
                            buf_bloc_index.tab[j].adr_dep_dat = pos;
                    }
            }

        }

        else
        {
            if(ara != NULL)
                *ara = buf_bloc_index;
            for(int i = 0; i<=index_mc->sup; i++)
            {
                EcrireDir_lof(*index_ms, index_mc->tab[i].adr_bloc_ind, index_mc->tab[i].adr_mc );
                //nbreLiDifa++;
            }

            ara = NULL;

        }

}//END_OF_FUN

//______________________________________________________________________________________________

void suppression_lof_v1_1 (FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc, int const cle)
{
//SETUP
    char trouv = 0;
    int case_mc = 0;
    int j = 0;
    int save = 0;
    BLOC_INDEX buf_bloc_index;
    BLOC_INDEX *ptr = NULL;

//END_OF_SETUP

    //Si le fichier n'est pas vide
    if(index_mc->sup != -1)
    {
        //rech dicho de la cle a supprimer dans index_mc pour trouver son "potentiel" bloc
        rech_dicho_mc(index_mc, &case_mc, cle);

        //On recupere depuis mc le bloc
        ptr = index_mc->tab[case_mc].adr_mc;
        buf_bloc_index = *ptr;

        //rech dicho dans le bloc, on recupere trouv et j
        rech_dicho_buf_ind(&buf_bloc_index, cle, &trouv, &j);

        //si on trouv ie si elle existe
        if(trouv == 1)
        {
            //on l'ecrase avec des decalage
            while( j!= (buf_bloc_index.Nb-1) )
            {
                buf_bloc_index.tab[j] = buf_bloc_index.tab[j+1];
                j++;
            }
            //on oublie pas de mettre a jour le buf.Nb
            buf_bloc_index.Nb --;

            //Si le bloc ne se vide pas apres suppression
            if(buf_bloc_index.Nb != 0)
            {
                //On l'ecrit en ms
                EcrireDir_lof(*index_ms, index_mc->tab[case_mc].adr_bloc_ind, &buf_bloc_index );
                //nbreLiDifa++;

                //en mc aussi
                *ptr = buf_bloc_index;

                //on met a jour la cle dominante en index_mc (on sait jamais on a supprimer la dominante)
                index_mc->tab[case_mc].cle = buf_bloc_index.tab[buf_bloc_index.Nb-1].cle;

            }
            //Si le bloc se vide
            else
            {
                //On libere la place qu'occupait le new empty bloc
                free(ptr);
                //on sauvegarde le suivant du bloc vidé
                save = buf_bloc_index.suivant;

                //on fait pointer le nouveau bloc vide vers la QLBV
                buf_bloc_index.suivant = index_ms->entete.QLBV; //maybe i should use Entete()...Nah
                //On ecrit le new empty bloc en ms
                EcrireDir_lof(*index_ms, index_mc->tab[case_mc].adr_bloc_ind, &buf_bloc_index);
                //nbreLiDifa++;
                //si la TLBV = QLBV ie ils sont tous les deux vers la fin du fichier
                if(index_ms->entete.TLBV == index_ms->entete.QLBV)
                    //On met a jours la TLBV avec l'adr du new empty bloc
                    index_ms->entete.TLBV = index_mc->tab[case_mc].adr_bloc_ind;
                //Sinon la LBV n'est pas vide, donc on utilise le bloc Avant_QLBV
                else
                {
                    LireDir_lof(*index_ms, index_ms->entete.AvantQ, &buf_bloc_index);
                    //nbreLiDifa++;
                    buf_bloc_index.suivant = index_mc->tab[case_mc].adr_bloc_ind;
                    EcrireDir_lof(*index_ms, index_ms->entete.AvantQ, &buf_bloc_index);
                    //nbreLiDifa++;
                }
                    //On met a jour le AvantQ qui doit toujours correspondre au new empty bloc
                    index_ms->entete.AvantQ = index_mc->tab[case_mc].adr_bloc_ind ;

                //si le new empty bloc est la tete T, on la met a jour avec save
                if(index_ms->entete.T == index_mc->tab[case_mc].adr_bloc_ind)
                    index_ms->entete.T = save;
                //Sinon on recupere le bloc qui pointait vers le new empty bloc et on le fait pointer vers save
                else
                {
                    //le bloc before new empty bloc se situe a case_mc-1
                    buf_bloc_index = *(index_mc->tab[case_mc-1].adr_mc);
                    buf_bloc_index.suivant = save;

                    //on ecrit le bloc en ms
                    EcrireDir_lof(*index_ms, index_mc->tab[case_mc-1].adr_bloc_ind, &buf_bloc_index);
                    //nbreLiDifa++;

                    //et en mc
                    *(index_mc->tab[case_mc-1].adr_mc) = buf_bloc_index;
                }

                //On met a jours l'index_mc en ecrasant le new empty bloc
                while(case_mc != index_mc->sup)
                {
                    index_mc->tab[case_mc] = index_mc->tab[case_mc + 1];
                    case_mc++;
                }
                index_mc->sup --;
            }
        }
    }
}//END_OF_FUN

//_______________________________________________________________________________________________________



int chaDecha(char mode,FICHIER_LOF index_ms, INDEX_NON_DENSE *index_mc)
{
  //SETUP
  int i = 0;
  FILE *f = NULL;
  BLOC_INDEX buf;

  //END OF SETUP

         if(mode == 'D')//decharge l'index_mc en ms
         {
             f = fopen("index_mc.bin","wb+");
             if (f == NULL)
                return 1; // il y a eu un probleme
             fwrite(&(index_mc->sup),sizeof(int),1, f);
             fwrite(&(index_mc->tab), ((index_mc->sup)+1)*sizeof(CCB), 1, f );//be careful with this one

            fclose(f);
            f = NULL;
         }

          if(mode == 'C')//charge l'index_mc et l'index_ms en mc
          {
            f = fopen("index_mc.bin","rb+");
            if (f == NULL)
                return 1;
            rewind(f);

            fread(&(index_mc->sup), sizeof(int), 1, f);
            fread(&(index_mc->tab),((index_mc->sup)+1)*sizeof(CCB), 1, f);//here too...

            while(i<= index_mc->sup)
            {
                LireDir_lof(index_ms, index_mc->tab[i].adr_bloc_ind, &buf);
                index_mc->tab[i].adr_mc = malloc(sizeof(BLOC_INDEX));
                *(index_mc->tab[i].adr_mc) = buf;
                i++;
            }



          }

}//END OF FUN


