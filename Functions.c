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
/*
status_code Merge_Groups(unsigned int Group_Id_1,unsigned int Group_Id_2)
{
	NodeG *ptr1=lptrG;
	NodeG *prev=NULL;
	status_code sc=SUCCESS;

	while((ptr1!=NULL)&&(ptr1->ID!=Group_Id_1)&&(ptr1->ID!=Group_Id_2))
	{
		ptr1=ptr1->next;
	}

	if(ptr1==NULL)
	{
		sc=FAILURE;
	}
	else if(ptr1->ID==Group_Id_1)
	{
		ptr2=ptr1->next;

		while((ptr2!=NULL)&&(ptr2->ID!=Group_Id_2))
		{
			ptr2=ptr2->next;
		}

		if(ptr2==NULL)
		{
			sc=FAILURE;
		}
		else
		{

		}


	}
	



}*/