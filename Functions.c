#include<stdlib.h>
#include<stdio.h>
#include"functions.h"

status_code CreateGroup(NodeG *nptr)
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