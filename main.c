#include<stdio.h.
#include<stdlib.h>

#include"globals.h"
#include"structs.h"
#include"features.h"
#include"sorting.h"

int MainMenu(void){
    int choice;
    printf("Main Menu\n");
    printf("Crop Recommendation\n");
    printf("Government Schemes\n");
    printf("Profit Calculation\n");
    printf("Exit\n");
    printf("Enter your choice: ");
    int ret=scanf("%d",&choice);
    if(ret==EOF)return 0;
    if(ret!=1)return 0;
    return choice;
}





