#include<stdlib.h>
#include<stdio.h>
#include"Functions.h"

NodeI *lptrI=NULL;
NodeG *lptrG=NULL;

void CreateNodeI(NodeI **npptr)
{
	*npptr=(NodeI*)malloc(sizeof(NodeI));
	(*npptr)->next=NULL;
}

void CreateNodeG(NodeG **npptr)
{
	*npptr=(NodeG*)malloc(sizeof(NodeG));
	(*npptr)->next=NULL;
}

status_code Add_Person(NodeI *nptr)
{
	NodeI* ptr=lptrI;
	NodeI* prev=NULL;
	status_code sc=SUCCESS;

	if(lptrI==NULL)
	{
		lptrI=nptr;
	}
	else
	{
		while(ptr->Id>nptr->Id){
			prev=ptr;
			ptr=ptr->next;
		}
		if(ptr->Id==nptr->Id){
			sc=FAILURE;
		}
		else{
			if(prev==NULL){
				nptr->next=lptrI;
				lptrI=nptr;
			}
			else{
				nptr->next=ptr;
				prev->next=nptr;
			}
		}
	}
	return sc;

}

status_code Create_Group(NodeG *nptr)
{
	NodeG *ptr=lptrG;
	NodeG *prev=NULL;
	status_code sc=SUCCESS;

	if(lptrG==NULL)
	{
		lptrG=nptr;
	}
	else
	{
		while(ptr->Id>nptr->Id)
		{
			prev=ptr;
			ptr=ptr->next;
		}

		if(ptr->Id==nptr->Id)
		{
			sc=FAILURE;
		}
		else{
			if(prev==NULL)
			{
				//Insert At Start
				nptr->next=lptrG;
				lptrG=nptr;
			}
			else{
				nptr->next=ptr;
				prev->next=nptr;
			}
		}
	}
	return sc;
}

NodeI* Search_for_Member_pointer(unsigned int Member_Id)
{
	NodeI* Member_ptr=NULL;
    NodeI* ptr=lptrI;
    while(ptr!=NULL  && ptr->Id!=Member_Id){
        ptr=ptr->next;
    }
    if(ptr!=NULL){
        Member_ptr=ptr;
    }
    return Member_ptr;
}

NodeG* Search_for_Group_pointer(unsigned int Group_Id)
{
	NodeI* Group_ptr=NULL;
    NodeI* ptr=lptrG;
    while(ptr!=NULL  && ptr->Id!=Group_Id){
        ptr=ptr->next;
    }
    if(ptr!=NULL){
        Group_ptr=ptr;
    }
    return Group_ptr;
}

status_code Store_Member_Pointers(NodeG *nptr)
{
	status_code sc=SUCCESS;

	for(int i=0;i<5;i++)
	{
		if((nptr->Member_Id[i]!=-1)&&(Check_Unique(nptr->Member_Id[i])))
			nptr->Members[i]=Search_for_Member_pointer(nptr->Member_Id[i]);
		else
			nptr->Members[i]=NULL;
	}

	return sc;
}

Boolean Check_Unique(unsigned int Member_Id)
{
	NodeG *ptr=lptrG;
	Boolean bool=TRUE;

	while((ptr!=NULL)&&(bool))
	{
		for(int i=0;i<5,bool;i++)
			if(Member_Id==ptr->Member_Id[i])
				bool=FALSE;
	}
	return bool;
}

status_code Delete_Group(unsigned int Group_Id)
{
	NodeG *ptr=lptrG;
	NodeG *prev=NULL;
	status_code sc=SUCCESS;

	while((ptr!=NULL)&&(ptr->Id!=Group_Id))
	{
		prev=ptr;
		ptr=ptr->next;
	}

	if(ptr==NULL)
	{
		sc=FAILURE;
	}
	else
	{
		if(prev==NULL)
		{
			lptrG=lptrG->next;
			free(ptr);
		}
		else
		{
			prev->next=ptr->next;
			free(ptr);
		}
		
	}
	return sc;
}

status_code Merge_Groups(unsigned int Group_Id_1,unsigned int Group_Id_2)
{
	NodeG *ptr=lptrG;
	NodeG *prev=NULL;
	NodeG *nptr,*tptr,*tprev;
	CreateNodeG(&nptr);

	status_code sc=SUCCESS;
	unsigned int t;


	if(Group_Id_1>Group_Id_2)
	{
		t=Group_Id_1;
		Group_Id_1=Group_Id_2;
		Group_Id_2=t;
	}

	while((ptr!=NULL)&&(ptr->Id!=Group_Id_1))
	{
		prev=ptr;
		ptr=ptr->next;
	}

	if(ptr==NULL)
	{
		sc=FAILURE;
	}
	else 
	{
		nptr->Id=Group_Id_1;
		printf("\nEnter the Group Name\t");
		scanf("%s",nptr->Name);
		i=0;
		while(ptr->Members[i]!=NULL)
		{
			nptr->Members[i]=ptr->Members[i];
			i++;
		}
		printf("\nEnter the Weekly Group Goal\t");
		scanf("%u",&nptr->Weekly_Group_Goal);

		tptr=ptr;
		tprev=prev;
		
		ptr=tptr->next;
		while((ptr!=NULL)&&(ptr->ID!=Group_Id_2))
		{
			prev=ptr;
			ptr=ptr->next;
		}
		//prev cannot be NULL
		prev->next=ptr->next;
		
		while((i+j)<5)
		{
			nptr->Members[i+j]=ptr->Members[j];
			j++;
		}
		if(tptr->Members[j]==NULL)
		{
			if(tprev==NULL)
			{
				nptr->next=lptrG;
				lptrG=nptr;
				free(tptr);
			}
			else
			{
				tprev->next=nptr;
				nptr->next=tptr->next;
				free(tptr);
			}
		}
		else
		{
			sc=FAILURE;
		}
	}

}

status_code Delete_individual(unsigned int Member_Id){
	status_code sc=SUCCESS;
	NodeI* ptr=lptrI;
	NodeI* prev=NULL;
	while(ptr!=NULL && ptr->Id>Member_Id){
		prev=ptr;
		ptr=ptr->next;
	}
	if(ptr==NULL || ptr->Id<Member_Id){
		// No member found with the given id;
		printf("Enter the correct Member_ID \n");
		sc=FAILURE;

	}
	else{
		// The id must be equal means it is present 
		if(prev==NULL){
			NodeI* nptr=ptr->next;
			free(ptr);
			lptrI=nptr;
		}
		else{
			NodeI* nptr=ptr->next;
			free(ptr);
			prev->next=nptr;


		}
		ptr=NULL;
	}
	return sc;
	
}
//If already defined Ignore the below definition
status_code Display_Member_Info(unsigned int Member_Id)
{
	status_code sc=SUCCESS;
	NodeI *ptr=Search_for_Member_pointer(Member_Id);

	if(ptr==NULL)
	{
		sc=FAILURE;
	}
	else
	{
		printf("\nMember Id : ",ptr->Id);
		printf("\nMember Name : ",ptr->Id);
		printf("\nMember Age : ",ptr->Age);
	}
	return sc;
}

status_code Display_Group_Info(unsigned int Group_Id)
{
	status_code sc=SUCCESS;
	NodeG *ptr=Search_for_Group_Pointer(Group_Id);

	if(ptr==NULL)
	{
		sc=FAILURE;
	}
	else
	{
		printf("\nGroup Id : ",Group_Id);
		printf("\nGroup Name : ",ptr->Name);
		printf("\nDetails of the Members");
		for(int i=0;i<5;i++)
		{
			Display_Member_Info(ptr->Members[i]);
		}
		printf("\n\nWeekly Group Goal :  ",ptr->Weekly_Group_Goal);
		printf("\nRank of the Group : ");
	}
	return sc;
}

unsigned int Compute_Number_Of_Steps_In_a_Week(NodeI *ptr)
{
	//To be defined
}

Boolean Check_Group_Achievement(unsigned int Group_Id)
{
	Boolean bool=TRUE;
	unsigned int count=0;
	NodeG *ptr=Search_for_Group_Pointer(Group_Id);

	if(ptr==NULL)
	{
		bool=FALSE;
		printf("\nGroup Does Not Exist!");
	}
	else
	{
		for(int i=0;i<5;i++)
			count+=Compute_Number_Of_Steps_In_a_Week(ptr->Members[i]);
		if(count>=Weekly_Group_Goal)
		{
			bool=TRUE;
			printf("\nThe group has completed the Weekly Goal");
		}
		else{
			bool=FALSE;
			printf("\nThe group has not complted the Weekly Goal");
		}
	}

	return bool;
}