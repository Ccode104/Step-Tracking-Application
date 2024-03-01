#include<stdio.h>
#include<conio.h>

void main()
{
	NodeI *lptr;
	NodeG *lptr;

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
     	fscanf(ptr,"%u",&nptrI.Id);
    	fscanf(ptr,"%s",nptrI.Name);
   	 	fscanf(ptr,"%u",&nptrI.Age);
     	fscanf(ptr,"%u",&nptrI.Daily_Step_Goal);
     	for(int i=0;i<7;i++)
     	{
			fscanf(ptr,"%u",&nptrI.Weekly_step_Count[i]);
	 	}
	 	Add_Person(nptrI);
	}

	for(int i=0;i<5;i++)
	{
		nptrG=CreateNodeI();
		fscanf(ptr,"%u",&nptrG.Id);
    	fscanf(ptr,"%s",nptrG.Name);
    	
    	for(int j=0;j<5;j++)
    	{
    		fscanf(ptr,"%u",&Member_Id[j]);	
    	}

    	for(int j=0;j<5;j++)
    	{
    		//Get member pointers given the Id and store them in Node
    		Store_Member_Pointers(nptrG,Member_Id);
    	}

    	fscanf(ptr,"%u",&nptrG.Weekly_Group_Goal);
    	CreateGroup(nptrG);
    	
	}

	fclose(ptr);
}