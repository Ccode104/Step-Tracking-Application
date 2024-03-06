#include<stdio.h>
#include<stdlib.h>
#include"Functions.h"



void main()
{
	
	NodeI *nptrI;
	NodeG *nptrG;
	unsigned int Member_Id[5];

	//Input the details
    FILE* ptr = fopen("data.txt", "r");
    if (ptr == NULL) 
    {
        printf("no such file.");
        
    }
    else
    {



    /*Input the Individual Details*/
    for(int j=0;j<20;j++)
    {
    	CreateNodeI(&nptrI);
     	fscanf(ptr,"%u",&nptrI->Id);
     	//printf("%u",nptrI->Id);
    	fscanf(ptr,"%s",nptrI->Name);
    	//printf("%s",nptrI->Name);
   	 	fscanf(ptr,"%u",&nptrI->Age);
   	 	//printf("%u",nptrI->Age);
     	fscanf(ptr,"%u",&nptrI->Daily_Step_Goal);
     	//printf("%u",nptrI->Daily_Step_Goal);
     	for(int i=0;i<7;i++)
     	{
			fscanf(ptr,"%u",&nptrI->Weekly_Step_Count[i]);
			//printf("%u",nptrI->Weekly_Step_Count[i]);
	 	}
	 	Add_Person(nptrI);
	}

	for(int i=0;i<5;i++)
	{
		CreateNodeG(&nptrG);
		fscanf(ptr,"%u",&nptrG->Id);
		//printf("\n%u",nptrG->Id);
    	fscanf(ptr,"%s",nptrG->Name);
    	//printf("\n%s",nptrG->Name);

    	/*
    	for(int j=0;j<5;j++)
    	{
   	 		fscanf(ptr,"%u",&Member_Id[j]);
   	 		printf("\n%u",Member_Id[j]);	
   		}*/
    	//Store_Member_Pointers(nptrG,Member_Id);

    	fscanf(ptr,"%u",&nptrG->Weekly_Group_Goal);
    	//printf("\n%u",nptrG->Weekly_Group_Goal);
    	//Create_Group(nptrG);
    	

	}

	fclose(ptr);
	//Display_Group_Info(1);
	
	//Check_Group_Achievement(1);
	//Generate_Leader_Board();
	}
}