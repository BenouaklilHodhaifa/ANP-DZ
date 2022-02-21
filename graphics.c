#include <stdio.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include "conio2.h"
#include "machine_abstraite.h"
#include "machine_abstraite_lof.h"
#include "functions.h"
#include "bloce_notes.h"
#include "charg_init.h"

extern int lire; extern int ecr; extern int lire_total; extern int ecr_total;

//_______________________________________________________
//              EFFACEMENT D'UNE PLAGE DANS LA CONSOLE
//_______________________________________________________

void effacement(int y, int lignes)
{
  int j=0;
  gotoxy(1,y);
  textbackground(WHITE);textcolor(BLACK);
  for (int i =0; i<lignes ; i++)
  {
      gotoxy(1,y+i);
      for(j=0 ; j<170; j++)
      {
          printf(" ");
      }
  }
};



//_________________________________________________________________________
//            UNE GRAND BOUCLE POUR FAIRE LE PROGRAMME ATTENDRE
//_________________________________________________________________________

void wait(int a)
{
    int j;
    for(int i=0; i<a*1000; i++)
    {
        for(j=0; j<a*100; j++)
        {

        }
    }
}



//-------------------LECTURE D'UNE TOUCHE DE CLAVIER AVEC GERANCE DU BEFFER---------------//
int getkey(void)
{
    int ch;
    clearkeybuf(); //on initialise le buffer
    ch=getch();    //lit immediatement le ch entré
    return ch;
}




//___________________________________________________________________________________________
//                                  SCANER AGE
//__________________________________________________________________________________________
int scanstr_age(char *num)
{
    int x,y,i=0,c;
    const int ENTRER=13;const int BACK_SPACE=8;
    x=wherex();
    y=wherey();
    do
        {
        gotoxy(x+i,y);
        c=getkey();                         //on enregistre le caractere dans le buffer
        if (c==27)
            return 0;
         if((c>=48&&c<=57 ) && i<2 ) // (code asci des nombre 1..9)
        {
            num[i]=c;                      //on affecte le chiffre a num
            printf("%c",c);               // on affiche le caractere
            i++;                          //on passe au prochain c
        }
        else if ((c==BACK_SPACE)&&(i>0)) // on supprime un caractere
        {
            gotoxy(x,y);
            i--;
            num[i]='\0';                    //on arrive a la fin de num si on ajoute rien
            printf("%s",num);               //on affiche le num sans le caractere supprimé
            gotoxy(x+i,y);
            printf(" ");                  // on affiche un vide
        }
        else if (c!=ENTRER ) flashbackground(4,100); //si le caractere n'est pas un chiffre ni un (.) ni un espace ni une suppression
    }while(c!=ENTRER || i==0);          // i==0 pour que le num ne soit pas vide
    num[i]='\0';                        //la fin de num
    gotoxy(1,y+1);                      //on saute un ligne
    return 1;
}


int scan_age(int *choix)
{
    int tmp;
    int x=wherex();
    int y=wherey();
    do{
    char num[3];            // l'initialisation du num à vide
    sprintf(num,"%s","");
    gotoxy(x,y);
    printf("  ");
    gotoxy(x,y);
    if ( ! ( scanstr_age(num) ) )   //on lit le num
        return 0;
    tmp=atoi(num);         // chaine vers un un réel
    if (tmp<19 || tmp>82)            // on verifie si num est un entier
    {
        gotoxy(x,y);
        printf("   ");
        gotoxy(x,y);
        textbackground(RED);textcolor(WHITE);
        gotoxy(63,40);
        printf(" L%c%cge doit %ctre entre 19 et 82 ",39,131,136);
        gotoxy(1,50);
        textbackground(WHITE);textcolor(BLACK);
        flashbackground(4,100);
        wait(100);
        gotoxy(63,40);
        printf("                                               ");
    }
    }while (tmp<19 || tmp>82);        // on sort du boucle jusqu'a num est un entier positif strictement
    *choix=tmp;
    return 1;
}









//------------------------ LECTURE DES  NUMEROS --------------------------------------------//
int scanstr_matricule(char *num)
{
    int x,y,i=0,c;
    const int ENTRER=13;const int BACK_SPACE=8;
    x=wherex();
    y=wherey();
    do
        {
        gotoxy(x+i,y);
        c=getkey();
        if (c==27){return 0;}                         //on enregistre le caractere dans le buffer
         if((c>=48&&c<=57 ) && i<6 ) // (code asci des nombre 1..9)
        {
            num[i]=c;                      //on affecte le chiffre a num
            printf("%c",c);               // on affiche le caractere
            i++;                          //on passe au prochain c
        }
        else if ((c==BACK_SPACE)&&(i>0)) // on supprime un caractere
        {
            gotoxy(x,y);
            i--;
            num[i]='\0';                    //on arrive a la fin de num si on ajoute rien
            printf("%s",num);               //on affiche le num sans le caractere supprimé
            gotoxy(x+i,y);
            printf(" ");                  // on affiche un vide
        }
        else if (c!=ENTRER ) flashbackground(4,100); //si le caractere n'est pas un chiffre ni un (.) ni un espace ni une suppression
    }while(c!=ENTRER || i==0);          // i==0 pour que le num ne soit pas vide
    num[i]='\0';                        //la fin de num
    gotoxy(1,y+1);                      //on saute un ligne
    return 1;
}

//------------------------------VALIDATION DU CHOIX ---------------------------------------//
int scan_matricule(int* choix, Fichier F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc, char mode)
{
    int tmp, i, j;
    int x=wherex();
    int y=wherey();

    do{
    char num[6];            // l'initialisation du num à vide
    sprintf(num,"%s","");
    gotoxy(x,y);
    if (! (scanstr_matricule(num) ) )        //on lit le num
        return 0;
    //tmp=atoll(num);         // chaine vers un un réel
    tmp=atoi(num);
    if (tmp<111111)            // on verifie si num est un entier
    {
        gotoxy(x,y);
        printf("          ");
        gotoxy(x,y);
        textbackground(RED);textcolor(WHITE);
        gotoxy(63,40);
        printf(" Le matricule doit %ctre entre 111111 et 999999 ",136);
        gotoxy(1,50);
        textbackground(WHITE);textcolor(BLACK);
        flashbackground(4,100);
        wait(100);
        gotoxy(63,40);
        printf("                                               ");
    }
    else if ( ( recherche(F, index_ms, index_mc ,tmp ,&i, &j) && mode == 'i' ) || ( !recherche(F, index_ms, index_mc ,tmp ,&i, &j) && mode == 's' ) )
    {
        gotoxy(x,y);
        printf("          ");
        gotoxy(x,y);
        textbackground(RED);textcolor(WHITE);
        gotoxy(63,40);
        if(mode  == 'i')
        {
            printf(" Ce matricule existe d%cj%c dans le fichier ",130,133);
        }
        else if(mode == 's')
        {
            printf(" Ce matricule n%cexiste pas dans le fichier ",39);
        }

        gotoxy(1,50);
        textbackground(WHITE);textcolor(BLACK);
        flashbackground(4,100);
        wait(100);
        gotoxy(63,40);
        printf("                                               ");
    }
    }while (tmp<111111 || ( ( recherche(F, index_ms, index_mc ,tmp ,&i, &j) && mode == 'i' ) || ( !recherche(F, index_ms, index_mc ,tmp ,&i, &j) && mode == 's' ) ));
    *choix=tmp;
    return 1;
}

//__________________________________________________________________________________
//                          SCANER DATE DE NAISSANCE
//__________________________________________________________________________________

void scanstr_date_naissance(char *num,int mode)
{
    int x,y,i=0,a,c;
    const int ENTRER=13;const int BACK_SPACE=8;
    x=wherex();
    y=wherey();
    if(mode==1 || mode==2){a=2;} // jour et mois
    else if(mode==3){a=4;} // année
    do
        {
        gotoxy(x+i,y);
        c=getkey();                         //on enregistre le caractere dans le buffer
         if((c>=48&&c<=57 ) && i<a ) // (code asci des nombre 1..9)
        {
            num[i]=c;                      //on affecte le chiffre a num
            printf("%c",c);               // on affiche le caractere
            i++;                          //on passe au prochain c
        }
        else if ((c==BACK_SPACE)&&(i>0)) // on supprime un caractere
        {
            gotoxy(x,y);
            i--;
            num[i]='\0';                    //on arrive a la fin de num si on ajoute rien
            printf("%s",num);               //on affiche le num sans le caractere supprimé
            gotoxy(x+i,y);
            printf(" ");                  // on affiche un vide
        }
        else if (c!=ENTRER ) flashbackground(4,100); //si le caractere n'est pas un chiffre ni un (.) ni un espace ni une suppression
    }while(c!=ENTRER || i==0);          // i==0 pour que le num ne soit pas vide
    num[i]='\0';                        //la fin de num
    gotoxy(1,y+1);                      //on saute un ligne
}

void scan_date_naissance(int* choix, int mode)
{
    int tmp, max,min;
    int x=wherex();
    int y=wherey();

    if(mode==1){max=31;min=-1;}
    else if(mode==2){max=12;min=-1;}
    else if(mode==3){max=2003;min=1940;}

    do{
    char num[6];            // l'initialisation du num à vide
    sprintf(num,"%s","");
    gotoxy(x,y);
    scanstr_date_naissance(num,mode);        //on lit le num
    //tmp=atoll(num);         // chaine vers un un réel
    tmp=atoi(num);
    if (tmp==0)            // on verifie si num est un entier
    {
        gotoxy(x,y);
        printf("    ");
        gotoxy(x,y);
        textbackground(RED);textcolor(WHITE);
        gotoxy(63,40);
        printf(" Les entr%ces doit %ctre strictement positives ",130,136);
        gotoxy(1,50);
        textbackground(WHITE);textcolor(BLACK);
        flashbackground(4,100);
        wait(100);
        gotoxy(63,40);
        printf("                                               ");
    }
    if(tmp<min || tmp>max)
    {
        gotoxy(x,y);
        printf("    ");
        gotoxy(x,y);
        textbackground(RED);textcolor(WHITE);
        gotoxy(63,40);
        if(mode==1){printf(" Le jour doit %ctre entre 1 et 31 ",136);}
        else if(mode==2){printf(" Le mois doit %ctre entre 1 et 12 ",136);}
        else if(mode==3){printf(" L%cann%ce doit %ctre entre 1940 et 2003 ",39,130,136);}
        gotoxy(1,50);
        textbackground(WHITE);textcolor(BLACK);
        flashbackground(4,100);
        wait(100);
        gotoxy(63,40);
        printf("                                               ");
    }
    }while (tmp==0 || (tmp<min || tmp>max) );
    *choix=tmp;

};
//________________________________________________________________________________
//           SCANER LE RESTE WILAYA, SANG, GRADE, FORCE ARMEE, REGION MILITAIRE
//________________________________________________________________________________

int scanstr_reste(char *num,int mode)
{
    int x,y,i=0,a,c;
    const int ENTRER=13;const int BACK_SPACE=8;
    x=wherex();
    y=wherey();
    if(mode==1 || mode==3){a=2;} //deux numero
    else {a=1;} // un seul
    do
        {
        gotoxy(x+i,y);
        c=getkey();                         //on enregistre le caractere dans le buffer
        if (c==27){return 0;}
         if((c>=48&&c<=57 ) && i<a ) // (code asci des nombre 1..9)
        {
            num[i]=c;                      //on affecte le chiffre a num
            printf("%c",c);               // on affiche le caractere
            i++;                          //on passe au prochain c
        }
        else if ((c==BACK_SPACE)&&(i>0)) // on supprime un caractere
        {
            gotoxy(x,y);
            i--;
            num[i]='\0';                    //on arrive a la fin de num si on ajoute rien
            printf("%s",num);               //on affiche le num sans le caractere supprimé
            gotoxy(x+i,y);
            printf(" ");                  // on affiche un vide
        }
        else if (c!=ENTRER ) flashbackground(4,100); //si le caractere n'est pas un chiffre ni un (.) ni un espace ni une suppression
    }while(c!=ENTRER || i==0);          // i==0 pour que le num ne soit pas vide
    num[i]='\0';                        //la fin de num
    gotoxy(1,y+1);                      //on saute un ligne
    return 1;
}

int scan_reste(int* choix, int mode)
{
    int tmp, max,min;
    int x=wherex();
    int y=wherey();

    if(mode==1){max=58;min=1;}
    else if(mode==2 || mode==4){max=8;min=1;} // mode: 1-wilaya 2-sang 3-grade 4-fa 5-region 6-ctg_grade
    else if(mode==3){max=17;min=1;}
    else if(mode==5){max=6;min=1;}
    else if (mode==6){max=5;min=1;}

    do{
    char num[6];            // l'initialisation du num à vide
    sprintf(num,"%s","");
    gotoxy(x,y);
     if ( ! (scanstr_reste(num,mode) ) )       //on lit le num
             return 0;
    //tmp=atoll(num);         // chaine vers un un réel
    tmp=atoi(num);
    if(tmp<min || tmp>max)
    {
        gotoxy(x,y);
        printf("  ");
        gotoxy(x,y);
        textbackground(RED);textcolor(WHITE);
        gotoxy(63,40);
        printf(" La valeur doit %ctre entre %d et %d ",136,min,max);
        gotoxy(1,50);
        textbackground(WHITE);textcolor(BLACK);
        flashbackground(4,100);
        wait(100);
        gotoxy(63,40);
        printf("                                               ");
    }
    }while (tmp==0 || (tmp<min || tmp>max) );
    *choix=tmp;
    return 1;

};

//___________________________________________________________________________________________________
//                           SCANER LE NOMBRE D'ENR à CREER LORS DE CHARG_INIT
//___________________________________________________________________________________________________

void scanstr_N(char *num)
{
    int x,y,i=0,c;
    const int ENTRER=13;const int BACK_SPACE=8;
    x=wherex();
    y=wherey();
    do
        {
        gotoxy(x+i,y);
        c=getkey();                         //on enregistre le caractere dans le buffer
         if((c>=48&&c<=57 ) && i<6 ) // (code asci des nombre 1..9)
        {
            num[i]=c;                      //on affecte le chiffre a num
            printf("%c",c);               // on affiche le caractere
            i++;                          //on passe au prochain c
        }
        else if ((c==BACK_SPACE)&&(i>0)) // on supprime un caractere
        {
            gotoxy(x,y);
            i--;
            num[i]='\0';                    //on arrive a la fin de num si on ajoute rien
            printf("%s",num);               //on affiche le num sans le caractere supprimé
            gotoxy(x+i,y);
            printf(" ");                  // on affiche un vide
        }
        else if (c!=ENTRER ) flashbackground(4,100); //si le caractere n'est pas un chiffre ni un (.) ni un espace ni une suppression
    }while(c!=ENTRER || i==0);          // i==0 pour que le num ne soit pas vide
    num[i]='\0';                        //la fin de num
    gotoxy(1,y+1);                      //on saute un ligne
}

void scan_N(int* choix)
{
    int tmp, max=888888,min=1;
    int x=wherex();
    int y=wherey();

    do{
    char num[6];            // l'initialisation du num à vide
    sprintf(num,"%s","");
    gotoxy(x,y);
    scanstr_N(num);        //on lit le num
    //tmp=atoll(num);         // chaine vers un un réel
    tmp=atoi(num);
    if(tmp<min || tmp>max)
    {
        gotoxy(x,y);
        printf("       ");
        gotoxy(x,y);
        textbackground(RED);textcolor(WHITE);
        gotoxy(63,40);
        printf(" La valeur doit %ctre entre %d et %d ",136,min,max);
        gotoxy(1,50);
        textbackground(WHITE);textcolor(BLACK);
        flashbackground(4,100);
        wait(100);
        gotoxy(63,40);
        printf("                                               ");
    }
    }while (tmp==0 || (tmp<min || tmp>max) );
    *choix=tmp;

};






//_____________________________________________________________________________________________

void scan_nom(char *num)
{
    int x,y,i=0,c;
    const int ENTRER=13;const int BACK_SPACE=8;
    x=wherex();
    y=wherey();
    do
        {
        gotoxy(x+i,y);
        c=getkey();                         //on enregistre le caractere dans le buffer
         if( ( (c>=65&&c<=90) || (c>=97&&c<=122 ) ) && i<30 ) // (code asci des nombre 1..9)
        {
            num[i]=c;                      //on affecte le chiffre a num
            printf("%c",c);               // on affiche le caractere
            i++;                          //on passe au prochain c
        }
        else if ((c==BACK_SPACE)&&(i>0)) // on supprime un caractere
        {
            gotoxy(x,y);
            i--;
            num[i]='\0';                    //on arrive a la fin de num si on ajoute rien
            printf("%s",num);               //on affiche le num sans le caractere supprimé
            gotoxy(x+i,y);
            printf(" ");                  // on affiche un vide
        }
        else if (c!=ENTRER ) flashbackground(4,100); //si le caractere n'est pas un chiffre ni un (.) ni un espace ni une suppression
    }while(c!=ENTRER || i==0);          // i==0 pour que le num ne soit pas vide
    num[i]='\0';                        //la fin de num
    gotoxy(1,y+1);                      //on saute un ligne
}

void startup()
{
    clrscr();
    system("color f0");
    textcolor(BLACK);
    textbackground(WHITE);
    printf("\n\t     *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   \n");
    textcolor(BLUE);
    printf("\t\t       _");
    printf("\t                                                            \n");
    printf("\t              ");
    printf("|_|");
    printf("\t                                                           \n");
    printf("\t              ");
    printf("/_/");
    printf("\t                                                             \n");
    printf("\t     ");
    printf("___  ___ _");
    printf("\t                                                              \n");
    printf("\t    ");
    printf("/ _ \\/ __| |");
    printf("\t                                                            \n");
    printf("\t   ");
    printf("|  __/\\__ \\ |");
    printf("\t           \n");
    printf("\t    ");
    printf("\\___||___/_|");
    printf("\t             \n\n\n");
    printf("\t    ");
    printf("\t\t\t\t\t\t\t");
    printf("E");
    Sleep(100);
    textcolor(BLACK);
    printf("C");
    Sleep(100);
    printf("O");
    Sleep(100);
    printf("L");
    Sleep(100);
    printf("E");
    Sleep(100);
    printf(" ");
    Sleep(100);
    textcolor(BLUE);
    printf("N");
    textcolor(BLACK);
    Sleep(100);
    printf("A");
    Sleep(100);
    printf("T");
    Sleep(100);
    printf("I");
    Sleep(100);
    printf("O");
    Sleep(100);
    printf("N");
    Sleep(100);
    printf("A");
    Sleep(100);
    printf("L");
    Sleep(100);
    printf("E");
    Sleep(100);
    printf(" ");
    Sleep(100);
    textcolor(BLUE);
    printf("S");
    textcolor(BLACK);
    Sleep(100);
    printf("U");
    Sleep(100);
    printf("P");
    Sleep(100);
    printf("E");
    Sleep(100);
    printf("R");
    Sleep(100);
    printf("I");
    Sleep(100);
    printf("E");
    Sleep(100);
    printf("U");
    Sleep(100);
    printf("R");
    Sleep(100);
    printf("E");
    Sleep(100);
    printf(" ");
    Sleep(100);
    textcolor(BLUE);
    printf("D");
    Sleep(100);
    printf("'");
    Sleep(100);
    printf("I");
    Sleep(100);
    textcolor(BLACK);
    printf("N");
    Sleep(100);
    printf("F");
    Sleep(100);
    printf("O");
    Sleep(100);
    printf("R");
    Sleep(100);
    printf("M");
    Sleep(100);
    printf("A");
    Sleep(100);
    printf("T");
    Sleep(100);
    printf("I");
    Sleep(100);
    printf("Q");
    Sleep(100);
    printf("U");
    Sleep(100);
    printf("E");
    Sleep(100);
    printf("\n\n\t\t\t\t\t\t\t     STRUCTURES DES FICHIERS ET STRUCTURES DE DONNEES\n");
    Sleep(50);
    textcolor(BLUE);
    printf("\t\t\t\t\t\t\t\t\t\t* SFSD * \n");
    Sleep(50);
    printf("\n");
    textcolor(BLACK);
    printf("\t\t\t\t\t\t-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_- \n") ;
    Sleep(50);
    textcolor(BLUE);
    printf("\n\n\t\t\t\t\t\t\t\t\t   Travail Pratique ");
    Sleep(50);
    printf("\n\n\t\t\t\t\t\t\t\t\tGESTION PERSONNEL ANP ");
    Sleep(50);
    printf("\n\n");
    textcolor(BLACK);
    printf("\t\t\t\t\t\t\t\t\t     Realis%c par :",130);
    printf("\n\n");
    textcolor(BLACK);
    printf("\t\t\t\t\t\t\t       Benouaklil Hodhaifa ");
    textcolor(BLUE);
    printf("& ");
    textcolor(BLACK);
    printf("Boukacem Younes  ");
    Sleep(50);
    printf("\n\n\t\t\t\t\t\t\t\t\t     Groupe : 08");
    printf("\n\n\t\t\t\t\t\t\t\t    Encadr%c Par :  ",130);
    Sleep(400);
    textcolor(BLUE);
    printf("D");
    Sleep(200);
    printf("r");
    Sleep(200);
    printf(" ");
    Sleep(200);
    printf("K");
    textcolor(BLACK);
    Sleep(200);
    printf("E");
    Sleep(200);
    printf("R");
    Sleep(200);
    printf("M");
    Sleep(200);
    printf("I");
    Sleep(200);
    printf(" ");
    Sleep(200);
    textcolor(BLUE);
    printf("A");
    Sleep(200);
    textcolor(BLACK);
    printf("D");
    Sleep(200);
    printf("E");
    Sleep(200);
    printf("L");
    Sleep(200);
    printf("\n\n\t\t\t\t\t\t\t\tAnn%ce Universitaire : 2021 / 2022  ",130);
    Sleep(50);
    printf("\n\n\n");
    textcolor(BLACK);
    printf("\t\t\t\t\t\t-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-  \n") ;
    Sleep(50);
    printf("\n");
    textcolor(BLACK);
    printf("\n\t     *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   ");
    Sleep(50);
    printf("\n\n\t\t\t\t\t\t\t       ");
    printf("Appuyer sur un button pour continuer...");
    getch();
};

int landing()
{
  int cpt=1, x, y;
  char c=0;

  clrscr();
  system("color f0");
  textcolor(GREEN);
  textbackground(WHITE);
  printf("\n\n\t\t\t\t\t\t          d8888 888b    888 8888888b.       8888888b. 8888888888P \n");
  printf("\t\t\t\t\t\t         d88888 8888b   888 888   Y88b      888  \"Y88b      d88P  \n");
  printf("\t\t\t\t\t\t        d88P888 88888b  888 888    888      888    888     d88P   \n");
  printf("\t\t\t\t\t\t       d88P 888 888Y88b 888 888   d88P      888    888    d88P   \n");
  printf("\t\t\t\t\t\t      d88P  888 888 Y88b888 8888888P\"       888    888   d88P     \n");
  printf("\t\t\t\t\t\t     d88P   888 888  Y88888 888      888888 888    888  d88P      \n");
  printf("\t\t\t\t\t\t    d8888888888 888   Y8888 888             888  .d88P d88P       \n");
  printf("\t\t\t\t\t\t   d88P     888 888    Y888 888             8888888P\" d8888888888 \n");

    textcolor(BLACK);
    c=1,cpt=1;
    x=70; y=18;
    do
      {
      textcolor(BLACK);textbackground(WHITE);

      gotoxy(x,y);printf("_______________________________");gotoxy(x-1,y+2);printf(" _______________________________ ");
      gotoxy(x-1,y+1);printf("|");gotoxy(x+31,y+1);printf("|");
      gotoxy(x,y+1);printf("      Chargement Initial       ");

      gotoxy(x,y+4);printf("_______________________________");gotoxy(x-1,y+6);printf(" _______________________________ ");
      gotoxy(x-1,y+5);printf("|");gotoxy(x+31,y+5);printf("|");
      gotoxy(x,y+5);printf("   Charger un ancien fichier   ");

      gotoxy(x,y+8);printf("_______________________________");gotoxy(x-1,y+10);printf(" _______________________________ ");
      gotoxy(x-1,y+9);printf("|");gotoxy(x+31,y+9);printf("|");
      gotoxy(x,y+9);printf("            Quitter            ");





      if (c==80){cpt%=3;cpt++;}
      if (c==72){cpt--;if(cpt==0)cpt=3;}

      if (cpt==1)
      {
          textcolor(WHITE);textbackground(WHITE);
          gotoxy(x,y);printf("_________________________________");
          textcolor(GREEN);textbackground(GREEN);
          gotoxy(x-1,y+1);printf("|");gotoxy(x+20,y+1);printf("|");
          gotoxy(x-1,y+2);printf("_________________________________");
          textcolor(WHITE);textbackground(GREEN);
          gotoxy(x,y+1);printf("      Chargement Initial        ");
          textcolor(BLACK);textbackground(WHITE);
          gotoxy(1,50);
      }

      if (cpt==2)
      {
          textcolor(WHITE);textbackground(WHITE);
          gotoxy(x,y+4);printf("_________________________________");
          textcolor(GREEN);textbackground(GREEN);
          gotoxy(x-1,y+5);printf("|");gotoxy(x+20,y+5);printf("|");
          gotoxy(x-1,y+6);printf("_________________________________");
          textcolor(WHITE);textbackground(GREEN);
          gotoxy(x,y+5);printf("   Charger un ancien fichier    ");
          textcolor(BLACK);textbackground(WHITE);
          gotoxy(1,50);

      }

      if (cpt==3)
      {
          textcolor(WHITE);textbackground(WHITE);
          gotoxy(x,y+8);printf("_________________________________");
          textcolor(GREEN);textbackground(GREEN);
          gotoxy(x-1,y+9);printf("|");gotoxy(x+20,y+9);printf("|");
          gotoxy(x-1,y+10);printf("_________________________________");
          textcolor(WHITE);textbackground(GREEN);
          gotoxy(x,y+9);printf("            Quitter             ");

          textcolor(BLACK);textbackground(WHITE);
          gotoxy(1,50);
      }


      c=getch();

      }while(c!=13);

  clrscr();
  return cpt;

};


//_______________________________________________________________________________________________________
//                                          MENU PRINCIPAL
//_______________________________________________________________________________________________________

int menu_principal()
{
   int cpt=1, x,y;
   char c=0;

   clrscr();
   system("color f0");
   textcolor(GREEN);
   textbackground(WHITE);


   printf("\n\n\t\t\t\t\t         __  __ _____ _   _ _   _   ____  ____  ___ _   _  ____ ___ ____   _    _     \n");
   printf("\t\t\t\t\t        |  \\/  | ____| \\ | | | | | |  _ \\|  _ \\|_ _| \\ | |/ ___|_ _|  _ \\ / \\  | |    \n");
   printf("\t\t\t\t\t        | |\\/| |  _| |  \\| | | | | | |_) | |_) || ||  \\| | |    | || |_) / _ \\ | |    \n");
   printf("\t\t\t\t\t        | |  | | |___| |\\  | |_| | |  __/|  _ < | || |\\  | |___ | ||  __/ ___ \\| |___ \n");
   printf("\t\t\t\t\t        |_|  |_|_____|_| \\_|\\___/  |_|   |_| \\_\\___|_| \\_|\\____|___|_| /_/   \\_\\_____|\n");

    textcolor(BLACK);
    c=1,cpt=1;
    x=70; y=18;
    do
      {
      textcolor(BLACK);textbackground(WHITE);

      gotoxy(x,y-8);printf("_______________________________");gotoxy(x-1,y-6);printf(" _______________________________ ");
      gotoxy(x-1,y-7);printf("|");gotoxy(x+31,y-7);printf("|");
      gotoxy(x,y-7);printf("           Insertion           ");

      gotoxy(x,y-4);printf("_______________________________");gotoxy(x-1,y-2);printf(" _______________________________ ");
      gotoxy(x-1,y-3);printf("|");gotoxy(x+31,y-3);printf("|");
      gotoxy(x,y-3);printf("          Suppression          ");

      gotoxy(x,y);printf("_______________________________");gotoxy(x-1,y+2);printf(" _______________________________ ");
      gotoxy(x-1,y+1);printf("|");gotoxy(x+31,y+1);printf("|");
      gotoxy(x,y+1);printf("           Recherche           ");

      gotoxy(x,y+4);printf("_______________________________");gotoxy(x-1,y+6);printf(" _______________________________ ");
      gotoxy(x-1,y+5);printf("|");gotoxy(x+31,y+5);printf("|");
      gotoxy(x,y+5);printf("   Modification rg militaire   ");

      gotoxy(x,y+8);printf("_______________________________");gotoxy(x-1,y+10);printf(" _______________________________ ");
      gotoxy(x-1,y+9);printf("|");gotoxy(x+31,y+9);printf("|");
      gotoxy(x,y+9);printf("    Suppression force arm%ce    ",130);

      gotoxy(x,y+12);printf("_______________________________");gotoxy(x-1,y+14);printf(" _______________________________ ");
      gotoxy(x-1,y+13);printf("|");gotoxy(x+31,y+13);printf("|");
      gotoxy(x,y+13);printf("  Requ%cte %c intervalle d'%cges  ",138,133,131);

      gotoxy(x,y+16);printf("_______________________________");gotoxy(x-1,y+18);printf(" _______________________________ ");
      gotoxy(x-1,y+17);printf("|");gotoxy(x+31,y+17);printf("|");
      gotoxy(x,y+17);printf("  Consultation ctg de grades   ");

      gotoxy(x,y+20);printf("_______________________________");gotoxy(x-1,y+22);printf(" _______________________________ ");
      gotoxy(x-1,y+21);printf("|");gotoxy(x+31,y+21);printf("|");
      gotoxy(x,y+21);printf("         Fragmentation         ");

      gotoxy(x,y+24);printf("_______________________________");gotoxy(x-1,y+26);printf(" _______________________________ ");
      gotoxy(x-1,y+25);printf("|");gotoxy(x+31,y+25);printf("|");
      gotoxy(x,y+25);printf("           Question 5          ");

      gotoxy(x,y+28);printf("_______________________________");gotoxy(x-1,y+30);printf(" _______________________________ ");
      gotoxy(x-1,y+29);printf("|");gotoxy(x+31,y+29);printf("|");
      gotoxy(x,y+29);printf("            Retour             ");


      if (c==80){cpt%=10;cpt++;}
      if (c==72){cpt--;if(cpt==0)cpt=10;}

      if(cpt==1)
      {
          textcolor(WHITE);textbackground(WHITE);
          gotoxy(x,y-8);printf("_________________________________");
          textcolor(GREEN);textbackground(GREEN);
          gotoxy(x-1,y-7);printf("|");gotoxy(x+20,y-7);printf("|");
          gotoxy(x-1,y-6);printf("_________________________________");
          textcolor(WHITE);textbackground(GREEN);
          gotoxy(x,y-7);printf("           Insertion            ");
          textcolor(BLACK);textbackground(WHITE);
          gotoxy(1,50);
      }
      if(cpt==2)
      {
          textcolor(WHITE);textbackground(WHITE);
          gotoxy(x,y-4);printf("_________________________________");
          textcolor(GREEN);textbackground(GREEN);
          gotoxy(x-1,y-3);printf("|");gotoxy(x+20,y-3);printf("|");
          gotoxy(x-1,y-2);printf("_________________________________");
          textcolor(WHITE);textbackground(GREEN);
          gotoxy(x,y-3);printf("          Suppression           ");
          textcolor(BLACK);textbackground(WHITE);
          gotoxy(1,50);
      }
      if (cpt==3)
      {
          textcolor(WHITE);textbackground(WHITE);
          gotoxy(x,y);printf("_________________________________");
          textcolor(GREEN);textbackground(GREEN);
          gotoxy(x-1,y+1);printf("|");gotoxy(x+20,y+1);printf("|");
          gotoxy(x-1,y+2);printf("_________________________________");
          textcolor(WHITE);textbackground(GREEN);
          gotoxy(x,y+1);printf("           Recherche            ");
          textcolor(BLACK);textbackground(WHITE);
          gotoxy(1,50);
      }

      if (cpt==4)
      {
          textcolor(WHITE);textbackground(WHITE);
          gotoxy(x,y+4);printf("_________________________________");
          textcolor(GREEN);textbackground(GREEN);
          gotoxy(x-1,y+5);printf("|");gotoxy(x+20,y+5);printf("|");
          gotoxy(x-1,y+6);printf("_________________________________");
          textcolor(WHITE);textbackground(GREEN);
          gotoxy(x,y+5);printf("   Modification rg militaire    ");
          textcolor(BLACK);textbackground(WHITE);
          gotoxy(1,50);

      }

      if (cpt==5)
      {
          textcolor(WHITE);textbackground(WHITE);
          gotoxy(x,y+8);printf("_________________________________");
          textcolor(GREEN);textbackground(GREEN);
          gotoxy(x-1,y+9);printf("|");gotoxy(x+20,y+9);printf("|");
          gotoxy(x-1,y+10);printf("_________________________________");
          textcolor(WHITE);textbackground(GREEN);
          gotoxy(x,y+9);printf("    Suppression force arm%ce     ",130);
          textcolor(BLACK);textbackground(WHITE);
          gotoxy(1,50);
      }
      if (cpt==6)
      {
          textcolor(WHITE);textbackground(WHITE);
          gotoxy(x,y+12);printf("_________________________________");
          textcolor(GREEN);textbackground(GREEN);
          gotoxy(x-1,y+13);printf("|");gotoxy(x+20,y+13);printf("|");
          gotoxy(x-1,y+14);printf("_________________________________");
          textcolor(WHITE);textbackground(GREEN);
          gotoxy(x,y+13);printf("  Requ%cte %c intervalle d'%cges   ",138,133,131);
          textcolor(BLACK);textbackground(WHITE);
          gotoxy(1,50);
      }
      if (cpt==7)
      {
          textcolor(WHITE);textbackground(WHITE);
          gotoxy(x,y+16);printf("_________________________________");
          textcolor(GREEN);textbackground(GREEN);
          gotoxy(x-1,y+17);printf("|");gotoxy(x+20,y+17);printf("|");
          gotoxy(x-1,y+18);printf("_________________________________");
          textcolor(WHITE);textbackground(GREEN);
          gotoxy(x,y+17);printf("  Consultation ctg de grades    ");
          textcolor(BLACK);textbackground(WHITE);
          gotoxy(1,50);
      }
      if (cpt==8)
      {
          textcolor(WHITE);textbackground(WHITE);
          gotoxy(x,y+20);printf("_________________________________");
          textcolor(GREEN);textbackground(GREEN);
          gotoxy(x-1,y+21);printf("|");gotoxy(x+20,y+21);printf("|");
          gotoxy(x-1,y+22);printf("_________________________________");
          textcolor(WHITE);textbackground(GREEN);
          gotoxy(x,y+21);printf("         Fragmentation          ");
          textcolor(BLACK);textbackground(WHITE);
          gotoxy(1,50);
      }
      if (cpt==9)
      {
          textcolor(WHITE);textbackground(WHITE);
          gotoxy(x,y+24);printf("_________________________________");
          textcolor(GREEN);textbackground(GREEN);
          gotoxy(x-1,y+25);printf("|");gotoxy(x+20,y+25);printf("|");
          gotoxy(x-1,y+26);printf("_________________________________");
          textcolor(WHITE);textbackground(GREEN);
          gotoxy(x,y+25);printf("           Question 5           ");
          textcolor(BLACK);textbackground(WHITE);
          gotoxy(1,50);
      }
      if (cpt==10)
      {
          textcolor(WHITE);textbackground(WHITE);
          gotoxy(x,y+28);printf("_________________________________");
          textcolor(GREEN);textbackground(GREEN);
          gotoxy(x-1,y+29);printf("|");gotoxy(x+20,y+29);printf("|");
          gotoxy(x-1,y+30);printf("_________________________________");
          textcolor(WHITE);textbackground(GREEN);
          gotoxy(x,y+29);printf("            Retour              ");
          textcolor(BLACK);textbackground(WHITE);
          gotoxy(1,50);
      }


      c=getch();

      }while(c!=13);

      return cpt;


};

void table_villes(char str[58][40])
{

    strcpy(str[0], "Adrar");
    strcpy(str[1], "Chlef");
    strcpy(str[2], "Laghouat");
    strcpy(str[3], "Oum El Bouaghi");
    strcpy(str[4], "Batna");
    strcpy(str[5], "Bejaia");
    strcpy(str[6], "Biskra");
    strcpy(str[7], "Bechar");
    strcpy(str[8], "Blida");
    strcpy(str[9], "Bouira");
    strcpy(str[10], "Tamanrasset");
    strcpy(str[11], "Tebessa");
    strcpy(str[12], "Tlemcen");
    strcpy(str[13], "Tiaret");
    strcpy(str[14], "Tizi Ouzou");
    strcpy(str[15], "Alger");
    strcpy(str[16], "Djelfa");
    strcpy(str[17], "Jijel");
    strcpy(str[18], "Setif");
    strcpy(str[19], "Saida");
    strcpy(str[20], "Skikda");
    strcpy(str[21], "Sidi Bel Abbes");
    strcpy(str[22], "Annaba");
    strcpy(str[23], "Guelma");
    strcpy(str[24], "Constantine");
    strcpy(str[25], "Medea");
    strcpy(str[26], "Mostaganem");
    strcpy(str[27], "M'Sila");
    strcpy(str[28], "Mascara");
    strcpy(str[29], "Ouargla");
    strcpy(str[30], "Oran");
    strcpy(str[31], "El Bayadh");
    strcpy(str[32], "Illizi");
    strcpy(str[33], "Bordj Bou Arreridj");
    strcpy(str[34], "Boumerdes");
    strcpy(str[35], "El Tarf");
    strcpy(str[36], "Tindouf");
    strcpy(str[37], "Tissemsilt");
    strcpy(str[38], "El Oued");
    strcpy(str[39], "Khenchela");
    strcpy(str[40], "Souk Ahras");
    strcpy(str[41], "Tipaza");
    strcpy(str[42], "Mila");
    strcpy(str[43], "Ain Defla");
    strcpy(str[44], "Naama");
    strcpy(str[45], "Ain Temouchent");
    strcpy(str[46], "Ghardaia");
    strcpy(str[47], "Relizane");
    strcpy(str[48], "Timimoun");
    strcpy(str[49], "Bordj Badji Mokhtar");
    strcpy(str[50], "Ouled Djellal");
    strcpy(str[51], "Beni Abbes");
    strcpy(str[52], "In Salah");
    strcpy(str[53], "In Guezzam");
    strcpy(str[54], "Touggourt");
    strcpy(str[55], "Djanet");
    strcpy(str[56], "El M'Ghair");
    strcpy(str[57], "El Meniaa");
};

void table_sang(char str[8][4])
{

    strcpy(str[0], "O+");
    strcpy(str[1], "A+");
    strcpy(str[2], "B+");
    strcpy(str[3], "O-");
    strcpy(str[4], "A-");
    strcpy(str[5], "AB+");
    strcpy(str[6], "B-");
    strcpy(str[7], "AB-");
};

void table_grade(char str[17][30])
{

    strcpy(str[0], "Général de corps d’armée");
    str[0][1]=130;str[0][3]=130;str[0][18]=39;str[0][22]=130;
    strcpy(str[1], "Général-Major");
    str[1][1]=130; str[1][3]=130;
    strcpy(str[2], "Général");
    str[2][1]=130;str[2][3]=130;
    strcpy(str[3], "Colonel");
    strcpy(str[4], "Lieutenant-colonel");
    strcpy(str[5], "Commandant");
    strcpy(str[6], "Capitaine");
    strcpy(str[7], "Lieutenant");
    strcpy(str[8], "Sous-lieutenant");
    strcpy(str[9], "Aspirant");
    strcpy(str[10], "Adjudant-chef");
    strcpy(str[11], "Adjudant");
    strcpy(str[12], "Sergent-chef");
    strcpy(str[13], "Sergent");
    strcpy(str[14], "Caporal-chef");
    strcpy(str[15], "Caporal");
    strcpy(str[16], "Djoundi");
};

void table_fa(char str[8][50])
{

    strcpy(str[0], "Armée de terre");
    str[0][3]=130;
    strcpy(str[1], "Armée de l’air");
    str[1][3]=130;str[1][10]=39;
    strcpy(str[2], "Marine nationale");
    strcpy(str[3], "Défense aérienne du territoire");
    str[3][1]=130;str[3][9]=130;
    strcpy(str[4], "Gendarmerie nationale");
    strcpy(str[5], "Garde républicaine");
    str[5][7]=130;
    strcpy(str[6], "Département du renseignement et de la sécurité");
    str[6][1]=130;str[6][39]=130;str[6][45]=130;
    strcpy(str[7], "Santé militaire");
    str[7][4]=130;
};

void table_region(char str[6][20])
{

    strcpy(str[0], "1RM-Blida");
    strcpy(str[1], "2RM-Oran");
    strcpy(str[2], "3RM-Béchar");
    str[2][5]=130;
    strcpy(str[3], "4RM-Ouargla");
    strcpy(str[4], "5RM-Constantine");
    strcpy(str[5], "6RM-Tamanrasset");

};

void affichage_table_ville(int x, int y, int max , char str[58][40])
{
    gotoxy(x,y);

    for(int i=0; i<max; i++)
    {
        if (i%7 == 0)
            printf("\n\n");
        printf("%-2d- %-20s",i+1,str[i]);
    }
};

void affichage_table_sang(int x, int y, int max, char str[8][4])
{
    gotoxy(x,y);

    for(int i=0; i<max; i++)
    {
        printf("%-2d- %-10s",i+1,str[i]);
    }
};

void affichage_table_grade(int x, int y, int max, char str[17][30])
{
    gotoxy(x,y);

    for(int i=0; i<max; i++)
    {
        if (i%6 == 0)
            printf("\n");
        printf("%-2d- %-25s",i+1,str[i]);
    }
};

void affichage_table_fa(int x, int y, int max, char str[8][50])
{
    gotoxy(x,y);

    for(int i=0; i<max; i++)
    {
        if (i%3 == 0)
            printf("\n\n");
        printf("%-2d- %-50s",i+1,str[i]);
    }
};

void affichage_table_rg(int x, int y, int max, char str[6][20])
{
    gotoxy(x,y);

    for(int i=0; i<max; i++)
    {
        printf("%-2d- %-20s",i+1,str[i]);
    }
};

//____________________________________________________________________________________
//                          AFFICHAGE D'UN ENREGISTREMENT
//____________________________________________________________________________________

void affichage_enr(Enr e, int y)
{
  char tab_wilaya[58][40];
  char tab_sang[8][4];
  char tab_grade[17][30];
  char tab_fa[8][50];
  char tab_rg[6][20];

  table_villes(tab_wilaya);
  table_sang(tab_sang);
  table_grade(tab_grade);
  table_fa(tab_fa);
  table_region(tab_rg);

  gotoxy(10,y);
  textcolor(BLACK);textbackground(WHITE);

  printf("Matricule: %d",e.matricule); gotoxy(40,y);printf("Nom: %s",e.nom); gotoxy(80,y);printf("Pr%cnom: %s",130,e.prenom);
  gotoxy(120, y);printf("Date De Naissance: %d/%d/%d", e.date.jour, e.date.mois, e.date.annee);

  gotoxy(10,y+3);printf("Wilaya De Naissance: %s (%d)",tab_wilaya[e.wilaya-1],e.wilaya);

  gotoxy(80,y+3);printf("Groupe Sanguin: %s (%d)",tab_sang[e.sang-1], e.sang);

  gotoxy(120,y+3);printf("Grade: %s (%d)",tab_grade[e.grade-1], e.grade);

  gotoxy(10, y+6);printf("Force Arm%ce: %s (%d)",130, tab_fa[e.force-1], e.force);

  gotoxy(80,y+6);printf("R%cgion Militaire: %s (%d)",130,tab_rg[e.region-1],e.region);

};

//________________________________________________________________________________________
//                                 AFFICHAGE DE L'INSERTION
//________________________________________________________________________________________


void affichage_insertion(Fichier *F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc)
{
    char tab_ville[58][40];
    char tab_sang[8][4];
    char tab_grade[17][30];
    char tab_fa[8][50];
    char tab_reg[6][20];
    int x1=20;
    int y1=10;
    int choix;
    char nom[31];
    Enr e;


    clrscr();
    system("color f0");
    gotoxy(x1+45,y1-5);
    textcolor(GREEN);
    textbackground(WHITE);
    printf("INSERTION D%cUN NOUVEL ENREGISTREMENT",39);

    textbackground (WHITE);
    textcolor(BLACK);

    //MATRICULE
    gotoxy(x1,y1);printf(" _________________");
    gotoxy(x1,y1+1);printf("|");
    gotoxy(x1+18,y1+1);printf("|");
    gotoxy(x1,y1+2);;printf(" _________________");
    gotoxy(x1-10,y1+1);printf("Matricule");

    gotoxy(x1+1,y1+1);
    if (! (scan_matricule(&choix,*F, index_ms, index_mc, 'i')) )
        goto fin;
    e.matricule = choix;

    //NOM_PRENOM
    gotoxy(x1+44,y1);printf(" ________________________________");
    gotoxy(x1+44,y1+1);printf("|");
    gotoxy(x1+77,y1+1);printf("|");
    gotoxy(x1+44,y1+2);;printf(" ________________________________");
    gotoxy(x1+40,y1+1);printf("Nom");
    gotoxy(x1+45,y1+1);
    scan_nom(nom);
    strcpy(e.nom,nom);

    gotoxy(x1+106,y1);printf(" ________________________________");
    gotoxy(x1+106,y1+1);printf("|");
    gotoxy(x1+139,y1+1);printf("|");
    gotoxy(x1+106,y1+2);;printf(" ________________________________");
    gotoxy(x1+99,y1+1);printf("Prenom");
    gotoxy(x1+107,y1+1);
    scan_nom(nom);
    strcpy(e.prenom, nom);

    //DATE DE NAISSANCE
    gotoxy(x1-10,y1+6);printf("Date de naissance:");
    gotoxy(x1+10, y1+6);printf("Jour:"); gotoxy(x1+20,y1+6);printf("Mois:");gotoxy(x1+30,y1+6);printf("Ann%ce:",130);
    gotoxy(x1+16,y1+6);scan_date_naissance(&choix,1);
    e.date.jour = (char)choix;
    gotoxy(x1+26,y1+6);scan_date_naissance(&choix,2);
    e.date.mois = (char)choix;
    gotoxy(x1+37,y1+6);scan_date_naissance(&choix,3);
    e.date.annee = (short)choix;

    //WILAYA DE NAISSANCE
    gotoxy(x1+55,y1+6);printf("Wilaya de naissance:");

    table_villes(tab_ville);
    affichage_table_ville(1,y1+10,58,tab_ville);

    gotoxy(x1+76,y1+6);scan_reste(&choix,1);
    e.wilaya = (char)choix;

    effacement(y1+11,18);

    //GROUPE SANGUIN
    gotoxy(x1+92,y1+6);printf("Groupe sanguin:");

    table_sang(tab_sang);
    affichage_table_sang(x1+5,y1+12,8,tab_sang);

    gotoxy(x1+108,y1+6);
    scan_reste(&choix, 2);
    e.sang = (char)choix;

    effacement(y1+12,1);

    //GRADE
    gotoxy(x1-10,y1+11);printf("Grade:");

    table_grade(tab_grade);
    affichage_table_grade(1,y1+15,17,tab_grade);

    gotoxy(x1-3, y1+11);
    scan_reste(&choix, 3);
    e.grade = (char)choix;

    effacement(y1+16,6);

    //FORCE ARMEE
    gotoxy(x1+13,y1+11);printf("Force arm%ce:",130);

    table_fa(tab_fa);
    affichage_table_fa(1,y1+14,8,tab_fa);

    gotoxy(x1+26,y1+11);
    scan_reste(&choix, 4);
    e.force = (char)choix;

    effacement(y1+15,6);

    //REGION MILITAIRE
    gotoxy(x1+42,y1+11);printf("R%cgion militaire:",130);

    table_region(tab_reg);
    affichage_table_rg(1, y1+16, 6, tab_reg);

    gotoxy(x1+60,y1+11);
    scan_reste(&choix, 5);
    e.region = (char)choix;

    effacement(y1+16,1);

    insertion(e, F, index_ms, index_mc);

    lobf_Afficheur(*F, "main_fich/data.txt");
    lof_Afficheur(*index_ms);

    gotoxy(55,43);printf("Nombre de lectures: %d      Nombre des ecritures: %d",lire, ecr);
    gotoxy(65, 45);
    system("main_fich\\data.txt");
    printf("Appuyez sur un bouton pour continuer...");
    getch();
fin:

    clrscr();


};


//______________________________________________________________________________
//                      AFFICHAGE DE SUPPRESSION
//______________________________________________________________________________

void affichage_suppression(Fichier *F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc)
{
    int x=75, y=15 , choix;


    clrscr();
    system("color f0");
    gotoxy(x-10,y-10);
    textcolor(GREEN);
    textbackground(WHITE);
    printf("SUPPRESSION D%cUN ENREGISTREMENT",39);


    textcolor(BLACK); textbackground(WHITE);
    //MATRICULE
    gotoxy(x,y);printf(" _________________");
    gotoxy(x,y+1);printf("|");
    gotoxy(x+18,y+1);printf("|");
    gotoxy(x,y+2);;printf(" _________________");
    gotoxy(x-10,y+1);printf("Matricule");

    gotoxy(x+1,y+1);
    if(! ( scan_matricule(&choix,*F, index_ms, index_mc, 's') ))
        goto fin;

    suppression(choix, F, index_ms, index_mc);
    lobf_Afficheur(*F, "main_fich/data.txt");
    lof_Afficheur(*index_ms);

    gotoxy(55,43);printf("Nombre de lectures: %d      Nombre des ecritures: %d",lire, ecr);
    gotoxy(65, 45);
    system("main_fich\\data.txt");
    printf("Appuyez sur un bouton pour continuer...");
    getch();
fin:
    clrscr();
};

//____________________________________________________________________________
//                    AFFIHAGE MODIFICATION REGION MILITAIRE
//____________________________________________________________________________


void affichage_modification_rg_ml(Fichier F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc)
{
    int x=75, y=15 , choix, i, j, matricule;
    char tab_rg[6][20];
    Bloc buf;

    clrscr();
    system("color f0");
    gotoxy(x-17,y-10);
    textcolor(GREEN);
    textbackground(WHITE);
    printf("MODIFICATION DE LA REGION MILITAIRE D%cUN ENREGISTREMENT",39);


    textcolor(BLACK); textbackground(WHITE);
    //MATRICULE
    gotoxy(x-40,y);printf(" _________________");
    gotoxy(x-40,y+1);printf("|");
    gotoxy(x-22,y+1);printf("|");
    gotoxy(x-40,y+2);;printf(" _________________");
    gotoxy(x-50,y+1);printf("Matricule");

    gotoxy(x-39,y+1);
    if ( ! (scan_matricule(&choix, F, index_ms, index_mc,'s') ) )
        goto fin;

     matricule = choix;

    gotoxy(x,y+6);textcolor(GREEN);textbackground(WHITE);
    printf("L%cENREGISTREMENT",39);
    textcolor(BLACK);textbackground(WHITE);

    recherche(F, index_ms, index_mc, matricule, &i, &j);
    LireDir(F, i, &buf);
    affichage_enr(buf.Tab[j], 24);

    gotoxy(x-2,y+20);textcolor(GREEN);textbackground(WHITE);
    printf("LES REGIONS MILITAIRE");
    textcolor(BLACK);textbackground(WHITE);

    table_region(tab_rg);
    affichage_table_rg(10, y+23, 6, tab_rg);

    gotoxy(x+20,y);printf("La nouvelle r%cgion militaire:",130);
    scan_reste(&choix,5);

    modification_rg_ml(matricule, choix, F, index_ms, index_mc);
    lobf_Afficheur(F, "main_fich/data.txt");
    lof_Afficheur(*index_ms);

    gotoxy(55,43);printf("Nombre de lectures: %d      Nombre des ecritures: %d",lire, ecr);
    gotoxy(65, 45);
    printf("Appuyez sur un bouton pour continuer...");
    getch();
fin:
    clrscr();
};

//__________________________________________________________________________________________
//                                  AFFICHAGE RECHERCHE
//__________________________________________________________________________________________

void affichage_recherche(Fichier F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc)
{
    int x=75, y=15, choix, i, j;
    lire=0; ecr=0;
    Bloc buf;

    clrscr();
    system("color f0");
    gotoxy(x-8,y-10);
    textcolor(GREEN);
    textbackground(WHITE);
    printf("RECHERCHE D%cUN ENREGISTREMENT",39);

    textcolor(BLACK); textbackground(WHITE);
    //MATRICULE
    gotoxy(x-5,y);printf(" _________________");
    gotoxy(x-5,y+1);printf("|");
    gotoxy(x+13,y+1);printf("|");
    gotoxy(x-5,y+2);;printf(" _________________");
    gotoxy(x-15,y+1);printf("Matricule");

    gotoxy(x-4,y+1);
    if (! (scan_matricule(&choix,F, index_ms, index_mc, 's')) )
        goto fin;

    if (! recherche(F, index_ms, index_mc, choix, &i, &j) )
        printf("un erreur lors de la recherche de l'enr dans la fonction 'affichage recherche'\n");
    else
    {
        LireDir(F, i, &buf);

        gotoxy(x-3,y+6);textcolor(GREEN);textbackground(WHITE);
        printf("L%cENREGISTREMENT",39);
        textcolor(BLACK);textbackground(WHITE);
        affichage_enr(buf.Tab[j], y+9);
    }


    gotoxy(55,43);printf("Nombre de lectures: %d      Nombre des ecritures: %d",lire, ecr);
    gotoxy(65, 45);
    printf("Appuyez sur un bouton pour continuer...");
    getch();

fin:
    clrscr();

};

//_______________________________________________________________________________________________________
//                              SUPPRESSION FORCE ARMEE
//_______________________________________________________________________________________________________

void affichage_suppr_fa(Fichier *F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc)
{
    int choix, x=75, y=15;
    char tab_fa[8][50];


    clrscr();
    system("color f0");
    gotoxy(x-23,y-10);
    textcolor(GREEN);
    textbackground(WHITE);
    printf("SUPPRESSION DE TOUS LES ENREGISTREMENTS RELATIFS %c UNE FORCE ARM%cE",183,144);
    gotoxy(x,y+4);printf("LES FORCE ARM%cE",144);
    textcolor(BLACK); textbackground(WHITE);

    table_fa(tab_fa);
    affichage_table_fa(1, y+5, 8, tab_fa);
    gotoxy(x-5, y);printf("La Force Arm%ce:",130);
    if ( ! (scan_reste(&choix, 4) ) )
        goto fin;

    faRemover(F, (char)choix, index_ms, index_mc);
    lobf_Afficheur(*F, "main_fich/data.txt");
    lof_Afficheur(*index_ms);

    gotoxy(63,40);
    textcolor(WHITE);textbackground(GREEN);
    printf(" L%cop%cration s%cest termin%ce avec succ%cs ",39,130,39,130,138);
    gotoxy(1,50);
    wait(100);
    textcolor(BLACK);textbackground(WHITE);
    gotoxy(63,40);
    printf("                                               ");

    gotoxy(55,43);printf("Nombre de lectures: %d      Nombre des ecritures: %d",lire, ecr);
    gotoxy(65, 45);
    system("main_fich\\data.txt");
    printf("Appuyez sur un bouton pour continuer...");
    getch();
fin:
    clrscr();
};

//____________________________________________________________________________________________________
//                              REQUETE A INTERVALLE A'AGE
//____________________________________________________________________________________________________

void affichage_requete_intervalle(Fichier F)
{
    int choix, x=75, y=15, min ,max, repeat=0;
    char tab_rg[6][20];

    table_region(tab_rg);
    clrscr();
    system("color f0");
    gotoxy(x-5,y-10);
    textcolor(GREEN);
    textbackground(WHITE);
    printf("REQU%cTE %c INTERVALLE D%c%cGES",210,182,39,182);
    textcolor(BLACK); textbackground(WHITE);


    do
    {
        repeat=0;
        gotoxy(x-30,y+1);printf("L%c%cge minimale: ",39,131);
        gotoxy(x-4,y+1);printf("L%c%cge maximale: ",39,131);
        gotoxy(x-14,y+1);
        if ( ! (scan_age(&min) ) )
            goto fin;
        gotoxy(x+12,y+1);
        if ( ! (scan_age(&max))  )
            goto fin;

        if(min>max)
        {
            textbackground(RED);textcolor(WHITE);
            gotoxy(63,40);
            printf(" La valeur maximale est inf%crieur que la valeur minimale ",130);
            gotoxy(1,50);
            textbackground(WHITE);textcolor(BLACK);
            flashbackground(4,100);
            wait(100);
            gotoxy(63,40);
            printf("                                                               ");
            gotoxy(x+12,y+1);printf("  ");
            repeat=1;
        }else
        {
            gotoxy(x-2,y+5);textcolor(GREEN);textbackground(WHITE);
            printf("LES REGIONS MILITAIRE");
            textcolor(BLACK);textbackground(WHITE);

            affichage_table_rg(15, y+8, 6, tab_rg);
            gotoxy(x+22,y+1);printf("La r%cgion militaire: ",130);
            if ( ! (scan_reste(&choix,5) ) )
                goto fin;
        }

    }while(repeat);

    ageRegion(F, choix, min, max);

    gotoxy(55,43);printf("Nombre de lectures: %d      Nombre des ecritures: %d",lire, ecr);
    gotoxy(65, 45);
    printf("Appuyez sur un bouton pour continuer...");
    system("bloc_note\\age_region.txt");
    getch();
fin:
    clrscr();

};

//____________________________________________________________________________________________
//                                CONSULTER UNE CATEGORIE DE GRADES
//____________________________________________________________________________________________

/*
    officier generaux: 1, 2, 3;
    officiers-supérieurs: 4, 5, 6;
    officiers: 7, 8, 9, 10;
    sous-officiers: 11, 12, 13, 14;
    hommes de troupes: 15, 16, 17;
*/


void affichage_consulter_ctg_grd(Fichier F)
{
    int choix, x=75, y=15;

    clrscr();
    system("color f0");
    gotoxy(x-8,y-10);
    textcolor(GREEN);
    textbackground(WHITE);
    printf("CONSULTER UNE CAT%cGORIE DE GRADES",144);
    gotoxy(x-5,y+4);printf("LES CAT%cGORIES DE GRADES",144);
    textcolor(BLACK); textbackground(WHITE);

    gotoxy(x-5,y);printf("La cat%cogorie de grades:",130);


    gotoxy(20,y+7);
    printf("1 - Officiers-generaux     2 - Officiers-superieurs     3 - Officiers     4 - Sous-officiers     5 - Hommes de troupes");

    gotoxy(x+20,y);

    if ( ! ( scan_reste(&choix,6) ) )
        goto fin;

    consulter_ctg_grade(F, choix);

    gotoxy(55,43);printf("Nombre de lectures: %d      Nombre des ecritures: %d",lire, ecr);
    gotoxy(65, 45);
    printf("Appuyez sur un bouton pour continuer...");
    system("bloc_note\\ctg_grade.txt");
    getch();
fin:
    clrscr();
};

//______________________________________________________________________________________________________
//                                          FRAGMENTER
//______________________________________________________________________________________________________

void affichage_fragmenter(Fichier F, Fichier *F1, Fichier *F2, Fichier *F3, Fichier *F4, Fichier *F5, Fichier *F6)
{
    int x=75, y=15;

    clrscr();
    system("color f0");
    gotoxy(x-5,y-10);
    textcolor(GREEN);
    textbackground(WHITE);
    printf("FRAGMENTER \"PERSONNEL-ANP_DZ.bin\"");
    textcolor(BLACK); textbackground(WHITE);

    fragmenter(F, F1, F2, F3, F4, F5, F6);



    gotoxy(55,43);printf("Nombre de lectures: %d      Nombre des ecritures: %d",lire, ecr);

    system("fragmenter\\region1.txt");
    system("fragmenter\\region2.txt");
    system("fragmenter\\region3.txt");
    system("fragmenter\\region4.txt");
    system("fragmenter\\region5.txt");
    system("fragmenter\\region6.txt");

    gotoxy(65, 45);
    printf("Appuyez sur un bouton pour continuer...");
    getch();

    clrscr();

};

//______________________________________________________________________________________________________
//                                        QUESTION 5
//______________________________________________________________________________________________________

void affichage_qst5(Fichier F )
{
    int x=75, y=15;


    clrscr();
    system("color f0");
    gotoxy(x-2,y-10);
    textcolor(GREEN);
    textbackground(WHITE);
    printf("NOMBRES DES ACC%cS DISQUE",212);
    textcolor(BLACK); textbackground(WHITE);

    gotoxy(x-25,y+1);printf("Nombre des Lectures: %d",lire_total);
    gotoxy(x+15, y+1); printf("Nombre des Ecritures: %d",ecr_total);
    gotoxy(x-10,y+6);printf("Nombre des acc%cs Disque: %d",138,lire_total+ecr_total);
    textcolor(GREEN);textbackground(WHITE);
    gotoxy(x-5,y+10);printf("L%cEN-T%cTE DU FICHIER",39,210);
    textcolor(BLACK);textbackground(WHITE);
    gotoxy(x-15,y+13);printf("La t%cte:",136);gotoxy(x-5,y+13);printf("%d",Entete(F,1));
    gotoxy(x+10,y+13);printf("La queue:");gotoxy(x+21,y+13);printf("%d",Entete(F,2));
    representation_globale(F);
    system("bloc_note\\rep_glo.txt");

    gotoxy(65, 45);
    printf("Appuyez sur un bouton pour continuer...");
    getch();

    lobf_Afficheur_V2(F, "bloc_note/rep_int.txt");
    system("bloc_note\\rep_int.txt");

    gotoxy(55,43);printf("Nombre de lectures: %d      Nombre des ecritures: %d",lire, ecr);
    gotoxy(65, 45);
    printf("Appuyez sur un bouton pour continuer...");
    getch();

    clrscr();


};

//________________________________________________________________________________________________________
//                                     AFFICHAGE CHARG_INIT
//________________________________________________________________________________________________________

void affichage_charg_init(Fichier *F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc)
{
    int x=75, y=15, N;


    clrscr();
    system("color f0");
    gotoxy(x-2,y-10);
    textcolor(GREEN);
    textbackground(WHITE);
    printf("CHARGEMENT INITIAL");
    textcolor(BLACK); textbackground(WHITE);

    gotoxy(x-15,y);printf("Nombre d%cenregistrements %c cr%cer:",39,133,130);
    gotoxy(x+20,y);scan_N(&N);
    gotoxy(x-10,y+3);//printf("L%cenregistrement N :",39);
    printf("Veuillez attendre s%cil vous pla%ct...",39,140);

    charg_init(N, F, index_ms, index_mc);

    gotoxy(55,43);printf("Nombre de lectures: %d      Nombre des ecritures: %d",lire, ecr);
    lobf_Afficheur(*F, "main_fich/data.txt");
    lof_Afficheur(*index_ms);

    system("main_fich\\data.txt");
    system("main_fich\\lof.txt");
    gotoxy(65, 45);
    printf("Appuyez sur un bouton pour continuer...");
    getch();
};

//_______________________________________________________________________________________________________
//                                      AFFICHAGE ANCIEN FICHIER
//_______________________________________________________________________________________________________

int affichage_ancien_fich(Fichier *F, FICHIER_LOF *index_ms, INDEX_NON_DENSE *index_mc)
{
    int cas, x=75, y=15;;

    cas = ancienCreator(F, index_ms, index_mc);

    if (cas==1)
    {
        lobf_Afficheur(*F, "main_fich/data.txt");
        lof_Afficheur(*index_ms);
        return 1;
    }
    else if(cas==0)
    {
        clrscr();
        system("color f0");
        textcolor(WHITE); textbackground(RED);
        gotoxy(x-32,y+5);printf(" L%cun des fichiers n%ccessaires n%cexiste pas vous devez faire un chargement initial ",39,130,39);
        textbackground(WHITE);textcolor(BLACK);
        gotoxy(1,50);
        wait(130);
        clrscr();
        return 0;
    }
};




