/*
    Alexandre GARCIA - Antoine LEVY - Clémentine Thornary
    garden-project - menu.c
    Version 1.0 - 20/03/2018
*/
#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
void Home(int temperature){
    printHome(temperature);
    selectionHome();
}
void printHome(int temperature){
    system("clear");
    printf("___________________________________________________\n");
    printf("%40s","|             Mon potager connecté !              |\n");
    printf("| Température :                         %d        |\n", temperature);
    printf("|_________________________________________________|\n");
    printf("|             Menu de configuration               |\n");
    printf("| 1. Refresh                                      |\n");
    printf("| 2. Exit                                         |\n");
    printf("|_________________________________________________|\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
int getnblong (int value){
	int cnt;
	cnt = 0;
	while (value != 0){
		cnt++;
		value = value/10;
	}
	return cnt;
}
void selectionHome(){
    int response = -1;
    scanf("%d", &response);
    getchar ();
    switch(response){
        case 1:{
            return;
            }
        break;      
        case 2:{
            exit(0);
            }
        break;
    }
}
// Menu 1
void menu1(){
    printMenu1();
    selectionMenu1();
}
void printMenu1(){
    system("clear");
    printf(" __________________________________________\n");
    printf("%40s","|          Configuration humidité          |\n");
    printf("| 1) Modifier                              |\n");
    printf("| 2) Retour home                           |\n");
    printf("|__________________________________________|\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
void selectionMenu1(){
    int response;
    scanf("%d", &response);
    printf("%d",response);
    switch(response){
        case 1:
            {
                action1();
                menu1();
            }
            break;
        case 2:
            {
                // Home();
            }
            break;
        case 0:
            {
                printf("mauvaise input\n");
                menu1();
            }
            break;
    }    
}
void action1(){
    printf("Action 1 !\n");
}
void action2(){
    printf("Action 2 !\n");
}
//Menu 2
void menu2(){
    printMenu2();
    selectionMenu2();
}
void printMenu2(){
    system("clear");
    printf(" ________________________________________\n");
    printf("%40s","|         Configuration débit d'eau      |\n");
    printf("| 1) Modifier                            |\n");
    printf("| 2) Retour home                         |\n");
    printf("|________________________________________|\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
void selectionMenu2(){
    int response;
    scanf("%d", &response);
    switch(response){
        case 1:
            {
                action3();
                menu2();
            }
            break;
        case 2:
            {
                // Home();
            }
            break;
        case 3:
            {
                exit(0);
            }
            break;
    }    
}
void action3(){
    printf("Action 3 !\n");
}
void action4(){
    printf("Action 4 !\n");
}