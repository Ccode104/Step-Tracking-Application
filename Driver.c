#include<stdio.h>
#include"Functions.h"



void main()
{
	
	NodeI *nptrI;
	NodeG *nptrG;

	//Input the details
    FILE* ptr = fopen("data.txt", "r");
    if (ptr == NULL) 
    {
        printf("no such file.");
        
    }

    /*Input the Individual Details*/
    for(int j=0;j<20;j++)
    {
    	nptrI=CreateNodeI();
     	fscanf(ptr,"%u",&nptrI->Id);
    	fgets(nptrI->Name,20,ptr);
   	 	fscanf(ptr,"%u",&nptrI->Age);
     	fscanf(ptr,"%u",&nptrI->Daily_Step_Goal);
     	for(int i=0;i<7;i++)
     	{
			fscanf(ptr,"%u",&nptrI->Weekly_Step_Count[i]);
	 	}
	 	Add_Person(nptrI);
	}

	for(int i=0;i<5;i++)
	{
		nptrG=CreateNodeG();
		fscanf(ptr,"%u",&nptrG->Id);
    	fgets(nptrG->Name,20,ptr);
    	
    	for(int j=0;j<5;j++)
    	{
    		fscanf(ptr,"%u",&nptrG->Member_Id[j]);	
    	}

    	for(int j=0;j<5;j++)
    	{
    		//Get member pointers given the Id and store them in Node
    		Store_Member_Pointers(nptrG);
    	}

    	fscanf(ptr,"%u",&nptrG->Weekly_Group_Goal);
    	Create_Group(nptrG);
    	
	}

	fclose(ptr);
}