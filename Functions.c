#include<stdlib.h>
#include<stdio.h>
#include"Functions.h"

NodeI *lptrI;
NodeG *lptrG;

NodeI* CreateNodeI()
{
	NodeI *nptr=(NodeI*)malloc(sizeof(NodeI));
	return nptr;
}

NodeG* CreateNodeG()
{
	NodeG *nptr=(NodeG*)malloc(sizeof(NodeG));
	return nptr;
}

status_code Add_Person(NodeI *nptr)
{
	//To be defined
}

status_code Create_Group(NodeG *nptr)
{
	NodeG *ptr=lptrG;
	NodeG *prev=NULL;
	status_code sc=SUCCESS;

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

	while((ptr->next!=NULL)&&(bool))
	{
		for(int i=0;i<5,bool;i++)
			if(Member_Id==ptr->Member_Id[i])
				bool=FALSE;
	}
	return bool;
}

