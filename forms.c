#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void wilayaReturner(char num, char wil[])
{

    switch (num)
    {
       case 1:
        strcpy(wil,"Adrar");
        break;
       case 2:
        strcpy(wil,"Chlef");
        break;
       case 3:
        strcpy(wil,"Laghouat");
        break;
       case 4:
        strcpy(wil,"Oum El Bouaghi");
        break;
       case 5:
        strcpy(wil,"Batna");
        break;
       case 6:
        strcpy(wil,"Bejaia");
        break;
       case 7:
        strcpy(wil,"Biskra");
        break;
       case 8:
        strcpy(wil,"Bechar");
        break;
       case 9:
        strcpy(wil,"Blida");
        break;
       case 10:
        strcpy(wil,"Bouira");
        break;
       case 11:
        strcpy(wil,"Tamanrasset");
        break;
       case 12:
        strcpy(wil,"Tebessa");
        break;
        strcpy(wil,"Tlemcen");
       case 13:
        break;
       case 14:
        strcpy(wil,"Tiaret");
        break;
       case 15:
        strcpy(wil,"Tizi-Ouzou");
        break;
       case 16:
        strcpy(wil,"Alger");
        break;
       case 17:
        strcpy(wil,"Djelfa");
        break;
       case 18:
        strcpy(wil,"Jijel");
        break;
       case 19:
        strcpy(wil,"Setif");
        break;
       case 20:
        strcpy(wil,"Saida");
        break;
       case 21:
        strcpy(wil,"Skikda");
        break;
       case 22:
        strcpy(wil,"Sidi Bel Abbas");
        break;
       case 23:
        strcpy(wil,"Annaba");
        break;
       case 24:
        strcpy(wil,"Guelma");
        break;
       case 25:
        strcpy(wil,"Constantine");
        break;
       case 26:
        strcpy(wil,"Medea");
        break;
       case 27:
        strcpy(wil,"Mostaganem");
        break;
       case 28:
        strcpy(wil,"M'sila");
        break;
       case 29:
        strcpy(wil,"Mascara");
        break;
       case 30:
        strcpy(wil,"Ouargla");
        break;
       case 31:
        strcpy(wil,"Oran");
        break;
       case 32:
        strcpy(wil,"El Bayyadh");
        break;
       case 33:
        strcpy(wil,"Illizi");
        break;
       case 34:
        strcpy(wil,"Bourdj Bou Arreridj");
        break;
       case 35:
        strcpy(wil,"Boumerdes");
        break;
       case 36:
        strcpy(wil,"Taref");
        break;
       case 37:
        strcpy(wil,"Tindouf");
        break;
       case 38:
        strcpy(wil,"Tissemsilt");
        break;
       case 39:
        strcpy(wil,"El Oued");
        break;
       case 40:
        strcpy(wil,"Khenchla");
        break;
       case 41:
        strcpy(wil,"Souk Ahras");
        break;
       case 42:
        strcpy(wil,"Tipaza");
        break;
       case 43:
        strcpy(wil,"Mila");
        break;
       case 44:
        strcpy(wil,"Ain Defla");
        break;
       case 45:
        strcpy(wil,"Naama");
        break;
       case 46:
        strcpy(wil,"Ain Timouchent");
        break;
       case 47:
        strcpy(wil,"Ghardaia");
        break;
       case 48:
        strcpy(wil,"Relizane");
        break;
       case 49:
        strcpy(wil,"Timimoun");
        break;
       case 50:
        strcpy(wil,"Bordj Badji Mokhtar");
        break;
       case 51:
        strcpy(wil,"Ouled Djellal");
        break;
       case 52:
        strcpy(wil,"Beni Abbes");
        break;
       case 53:
        strcpy(wil,"Salah");
        break;
       case 54:
        strcpy(wil,"Guezzam");
        break;
       case 55:
        strcpy(wil,"Touggourt");
        break;
       case 56:
        strcpy(wil,"Djanet");
        break;
       case 57:
        strcpy(wil,"M'Ghair");
        break;
       case 58:
        strcpy(wil,"Meniaa");
        break;
    }
}//END OF FUN


//___________________________________________________________________________

void sang_returner(char num, char gs[])
{

 switch (num)
 {

   case 1:
       strcpy(gs, "A-");
       break;
   case 2:
       strcpy(gs, "A+");
       break;
   case 3:
       strcpy(gs, "B+");
       break;
   case 4:
       strcpy(gs, "B-");
       break;
   case 5:
       strcpy(gs, "O+");
       break;
   case 6:
       strcpy(gs, "O-");
       break;
   case 7:
       strcpy(gs, "AB+");
       break;
   case 8:
       strcpy(gs, "AB-");
       break;
 }

}

//_____________________________________________

void grade_returner(char num, char grade[])
{
    switch(num)
    {
            case 1:
                strcpy(grade,"General corps d'armee");
                break;
            case 2:
                strcpy(grade,"General-Major");
                break;
            case 3:
                strcpy(grade,"General");
                break;
            case 4:
                strcpy(grade,"Colonel");
                break;
            case 5:
                strcpy(grade,"Lieutenant-colonel");
                break;
            case 6:
                strcpy(grade,"Commandant");
                break;
            case 7:
                strcpy(grade,"Capitaine");
                break;
            case 8:
                strcpy(grade,"Lieutenant");
                break;
            case 9:
                strcpy(grade,"Sous-Lieutenant");
                break;
            case 10:
                strcpy(grade,"Aspirant");
                break;
            case 11:
                strcpy(grade,"Adjudant-Chef");
                break;
            case 12:
                strcpy(grade,"Adjudant");
                break;
            case 13:
                strcpy(grade,"Sergent-chef");
                break;
            case 14:
                strcpy(grade,"Sergent");
                break;
            case 15:
                strcpy(grade,"Caporal-chef");
                break;
            case 16:
                strcpy(grade,"Caporal");
                break;
            case 17:
                strcpy(grade,"Djoundi");
                break;
    }

}


void force_returner(char num, char force[])
{

   switch(num)
   {
   case 1:
    strcpy(force, "Armee de terre");
    break;
   case 2:
    strcpy(force, "Armee de l'air");
    break;
    case 3:
    strcpy(force, "Marine nationale");
    break;
    case 4:
    strcpy(force, "Defense Aerienne");
    break;
    case 5:
    strcpy(force, "Gendarmerie nationale");
    break;
    case 6:
    strcpy(force, "Garde republicaine");
    break;
    case 7:
    strcpy(force, "D.R.S");
    break;
    case 8:
    strcpy(force, "Sante militaire");
    break;

   }

}


void region_returner(char num, char region[])
{
    switch (num)
    {
        case 1:
            strcpy(region,"Blida");
            break;
        case 2:
            strcpy(region,"Oran");
            break;
        case 3:
            strcpy(region,"Bechar");
            break;
        case 4:
            strcpy(region,"Ouargla");
            break;
        case 5:
            strcpy(region,"Constantine");
            break;
        case 6:
            strcpy(region,"Tamanrasset");
            break;

    }
}







