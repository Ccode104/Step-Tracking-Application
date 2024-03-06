#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"Functions.h"

NodeI *lptrI=NULL;
NodeG *lptrG=NULL;
unsigned int first,second,third;
NodeI *position[3];

void CreateNodeI(NodeI **npptr)
{
	*npptr=(NodeI*)malloc(sizeof(NodeI));
	(*npptr)->belong=0;
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
		nptr->next=NULL;
	}
	else
	{
		while(nptr->Id<ptr->Id){
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
		//Empty list
		lptrG=nptr;
		//nptr->next=NULL;
	}
	else
	{
		while(nptr->Id>ptr->Id)
		{ 
			//The Id of the Group to be inserted is larger
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
    else{
    	printf("Not found");
    }
    return Member_ptr;
}

NodeG* Search_for_Group_pointer(unsigned int Group_Id)
{
	NodeG* Group_ptr=NULL;
    NodeG* ptr=lptrG;
    while(ptr!=NULL  && ptr->Id!=Group_Id){
        ptr=ptr->next;
    }
    if(ptr!=NULL){
        Group_ptr=ptr;
    }
    return Group_ptr;
}

status_code Store_Member_Pointers(NodeG *nptr,unsigned int Member_Id[])
{
	status_code sc=SUCCESS;

	for(int i=0;i<5;i++)
	{
		if((Member_Id[i]!=0))
		{
			nptr->Members[i]=Search_for_Member_pointer(Member_Id[i]);
			if(nptr->Members[i]->belong==1)
			{
				nptr->Members[i]=NULL;
			}
			else{
				nptr->Members[i]->belong=1;
			}

		}
		else
		{
			//printf("NULL");
			nptr->Members[i]=NULL;
		}
	}

	return sc;
}
/*
Boolean Check_Unique(unsigned int Member_Id)
{
	NodeG *ptr=lptrG;
	Boolean bool=TRUE;

	while((ptr!=NULL)&&(bool==TRUE))
	{
		for(int i=0;i<5,bool==TRUE;i++)
			{
				if(Member_Id==ptr->Member_Id[i])
				{
					bool=FALSE;
				}else{
					bool=TRUE;
				}
			}
		ptr=ptr->next;
	}
	return bool;
}*/

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
/*
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

}*/
status_code Merge_Groups(unsigned int Group_Id_1, unsigned int Group_Id_2)
{

    NodeG *head = lptrG;
    NodeG *prev = NULL;
    status_code sc = SUCCESS;
    NodeG *group1, *group2;
    group1 = group2 = NULL;
    NodeG *group1_prev, *group2_prev;
    group1_prev = group2_prev = NULL;
    while (head != NULL)
    {
        if (head->Id == Group_Id_1)
        {
            group1_prev = prev;
            group1 = head;
        }
        else if (head->Id == Group_Id_2)
        {
            group2_prev = prev;
            group2 = head;
        }
        prev = head;
        head = head->next;
    }
    if (group1 == NULL || group2 == NULL)
    {
        printf("Enter the correct IDs \n");
        sc = FAILURE;
    }
    else
    {
        printf("Both the groups exist \n");
        NodeG *newgroup;
        CreateNodeG(&newgroup);
        

        int i = 0;
        while (i < 5 && group1->Members[i] != NULL)
        {
            newgroup->Members[i] = group1->Members[i];
            i++;
        }
        int p = 0;
        while (i < 5 && group2->Members[p] != NULL)
        {
            newgroup->Members[i] = group2->Members[p];
            i++;
            p++;
        }
        if (i >= 5 && group2->Members[p] != NULL)
        {
            printf("Merging of these groups are not possible because they both have more number of members than 5 \n");
            free(newgroup);
            sc = FAILURE;
        }
        else
        {
            while (i < 5)
            {
                newgroup->Members[i] = NULL;
                i++;
            }
            printf("Enter the New ID: ");
            unsigned int id;
            scanf("%u", &id);
            newgroup->Id = id;
            printf("Enter the new Name : ");
            scanf("%s",newgroup->Name);
            printf("Enter the New Group Goals : ");
            unsigned int newgoals;
            scanf("%u", &newgoals);
            newgroup->Weekly_Group_Goal = newgoals;
            // Now the group has been sucessfully formed now delete the old groups and place the new group further
            status_code sc1;
            sc1 = Delete_Group(group1->Id);
            sc1 = Delete_Group(group2->Id);
            sc1 = Create_Group(newgroup);
        }
    }
    return sc;
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
//Defined Twice(2 versions)
status_code Display_Member_Info(NodeI *ptr)
{
	status_code sc=SUCCESS;

	if(ptr==NULL)
	{
		sc=FAILURE;
		printf("Error");
	}
	else
	{
		
		printf("\nMember Id : %u ",ptr->Id);
		printf("\nMember Name : %s ",ptr->Name);
		printf("\nMember Age : %u ",ptr->Age);
	}
	return sc;
}

status_code Display_Group_Info(unsigned int Group_Id)
{
	status_code sc=SUCCESS;
	unsigned int rank=1;
	NodeG *ptr=Search_for_Group_pointer(Group_Id),*tptr=ptr;

	if(ptr==NULL)
	{
		sc=FAILURE;
	}
	else
	{
		printf("\nGroup Id : %u ",Group_Id);
		printf("\nGroup Name : %s ",ptr->Name);
		printf("\nDetails of the Members");
		for(int i=0;i<5;i++)
		{
			Display_Member_Info(ptr->Members[i]);
		}
		printf("\n\nWeekly Group Goal : %u ",ptr->Weekly_Group_Goal);
		/*radixsort_groups_steps();
		tptr=lptrG;
		while(tptr!=ptr)
		{
			tptr++;
			rank++;
		}
		printf("\nRank of the Group is %d",rank);
		radixsort_groups_Id();*/
	}
	return sc;
}

unsigned int Compute_Number_Of_Steps_In_a_Week(NodeI *nptr)
{
	unsigned int steps=0;
	if(nptr==NULL)
	{
		steps=0;
	}
	else
	{
		for(int i=0;i<7;i++)
		{
			steps+=nptr->Weekly_Step_Count[i];
		}
	}
	return steps;
}

unsigned int Compute_Number_Of_Steps_by_a_Group(NodeG *nptr)
{
	unsigned int steps=0;
	if(nptr==NULL)
	{
		steps=0;
	}
	else
	{
		for(int i=0;i<5;i++)
		{
			steps+=Compute_Number_Of_Steps_In_a_Week(nptr->Members[i]);
		}
	}
	return steps;
}

Boolean target_complete(NodeI* curr,unsigned int* steps){
	Boolean res=TRUE;
    for(int i=0;i<7 && res==TRUE;i++){
		*steps+=curr->Weekly_Step_Count[i];
		if(curr->Daily_Step_Goal>curr->Weekly_Step_Count[i]){
			// The target is incomplete 
			res=FALSE;
		}
	}
	return res;
	
}

Boolean Check_Group_Achievement(unsigned int Group_Id)
{
	Boolean bool=TRUE;
	unsigned int count=0;
	NodeG *ptr;
	ptr=Search_for_Group_pointer(Group_Id);

	if(ptr==NULL)
	{
		bool=FALSE;
		printf("\nGroup Does Not Exist!");
	}
	else
	{
		for(int i=0;i<5;i++)
			count+=Compute_Number_Of_Steps_In_a_Week(ptr->Members[i]);
		if(count>=ptr->Weekly_Group_Goal)
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
unsigned int getmax_GSteps()
{
	NodeG *ptr=lptrG;
	unsigned int max=0,steps=0;

	max=Compute_Number_Of_Steps_by_a_Group(ptr);
	ptr=ptr->next;	
	while(ptr!=NULL)
	{
		steps=Compute_Number_Of_Steps_by_a_Group(ptr);
		if(steps>max)
		{
			max=steps;
		}
		ptr=ptr->next;
	}
	return steps;
}
unsigned int getmax_GId()
{
	NodeG *ptr=lptrG;
	unsigned int max=0,Id=0;

	max=ptr->Id;
	ptr=ptr->next;	
	while(ptr!=NULL)
	{
		Id=ptr->Id;
		if(Id>max)
		{
			max=Id;
		}
		ptr=ptr->next;
	}
	return Id;
}
void InitHash(HashG *h)
{
	for(int i=0;i<9;i++)
		(h->array[i]).front=(h->array[i]).rear=NULL;
}
Boolean isQueueEmpty(QueueG *q)
{
	Boolean bool;

	if((q->front==NULL)&&(q->rear==NULL))
		bool=TRUE;
	else
		bool=FALSE;

	return bool;
}
void InsertQueue(QueueG *q,NodeG *nptr)
{
	if(isQueueEmpty(q))
		q->front=q->rear=nptr;
	else
	{
		q->rear->next=nptr;
		q->rear=nptr;
		nptr->next=NULL;
	}
}
void radixsort_groups_steps()
{
	NodeG *ptr=lptrG,*next_ptr=NULL;
	status_code sc=SUCCESS;
	unsigned int digits=0,div=1,flag=0,max=0,steps=0,i=0,j=0;
	HashG h;

	if(lptrG==NULL)
	{
		sc=FAILURE;
	}
	else{
		max=getmax_GSteps();
		
		while(max!=0)
		{
			max=max/10;
			digits++;
		}
		div=1;
		for(int k=0;k<digits;k++)
		{
			ptr=lptrG;
			while(ptr!=NULL)
			{
				next_ptr=ptr->next;
				steps=Compute_Number_Of_Steps_by_a_Group(ptr);
				steps=(steps/div)%10;
				InsertQueue(&h.array[steps],ptr);
				ptr=next_ptr;
			}
			i=9;
			while(isQueueEmpty(&h.array[i]))
				i--;

			lptrG=h.array[i].front;
			
			while((i>0)&&(flag==0))
			{
				j=i-1;
				while((j>=0)&&(isQueueEmpty(&h.array[j])))
				{
					j--;
				}
				if(j<0)
					flag==1;
				else
				{
					(h.array[i].rear)->next=h.array[j].front;
					i=j;
				}
			}
			InitHash(&h);
			div=div*10;
		}
	}
}
void radixsort_groups_Id()
{
	NodeG *ptr=lptrG,*next_ptr=NULL;
	status_code sc=SUCCESS;
	unsigned int digits=0,div=1,flag=0,max=0,i=0,j=0,Id=0;
	HashG h;

	if(lptrG==NULL)
	{
		sc=FAILURE;
	}
	else{
		max=getmax_GId();
		
		while(max!=0)
		{
			max=max/10;
			digits++;
		}

		div=1;
		for(int k=0;k<digits;k++)
		{
			ptr=lptrG;
			while(ptr!=NULL)
			{
				next_ptr=ptr->next;
				Id=ptr->Id;
				Id=(Id/div)%10;
				InsertQueue(&h.array[Id],ptr);
				ptr=next_ptr;
			}
			i=9;
			while(isQueueEmpty(&h.array[i]))
				i--;

			lptrG=h.array[i].front;
			
			while((i>0)&&(flag==0))
			{
				j=i-1;
				while((j>=0)&&(isQueueEmpty(&h.array[j])))
				{
					j--;
				}
				if(j<0)
					flag==1;
				else
					(h.array[i].rear)->next=(h.array[j]).front;
			}
			InitHash(&h);
			div=div*10;
		}
	}
}
status_code Generate_Leader_Board()
{
	status_code sc=SUCCESS;
	NodeG *ptr;
	unsigned int r=1;

	radixsort_groups_steps();
	ptr=lptrG;
	r=1;
	while(ptr!=NULL)
	{
		printf("Rank = %d",r);
		printf("\nGroup Id  %u",ptr->Id);
		printf("\nGroup Name %s ",ptr->Name);
		printf("\nDetails of the Members");
		for(int i=0;i<5;i++)
		{
			Display_Member_Info(ptr->Members[i]);
		}
		printf("\n\nWeekly Group Goal : %u ",ptr->Weekly_Group_Goal);
	}

	radixsort_groups_Id();

	return sc;

}
void get_top3list(){
	
	first=second=third=0;
	NodeI* curr=lptrI;
	while(curr!=NULL){
		unsigned int steps=0;
		Boolean check=target_complete(curr,&steps);
		if(check==TRUE){
			// The member has completed therir daily step goals

			if(position[0]==NULL){
				position[0]=curr;
				first=steps;
			}
			else if(position[1]==NULL){
				if(steps>first){
					// The current one has more number of steps 
					position[1]=position[0];
					position[0]=curr;
					second=first;
					first=steps;
				}
				else{
					position[1]=curr;
					second=steps;
				}
			}
			else if(position[2]==NULL){
				if(steps>first){
					position[2]=position[1];
					position[1]=position[0];
					position[0]=curr;
					third=second;
					second=first;
					first=steps;
				}
				else if(steps>second){
					position[2]=position[1];
					position[1]=curr;
					third=second;
					second=steps;
				}
				else{
					position[2]=curr;
					third=steps;
				}
			}
			else{
				if(steps>first){
					position[2]=position[1];
					position[1]=position[0];
					position[0]=curr;
					third=second;
					second=first;
					first=steps;
				}
				else if(steps>second){
					position[2]=position[1];
					position[1]=curr;
					third=second;
					second=steps;
				}
				else if(steps>third){
					position[2]=curr;
					third=steps;
				}
				// If it don't eneter into any of the condition then this means that the member_steps < third;

			}
		}
		curr=curr->next;
	}
	
}
void display_individualData(NodeI* head){
	
	printf("Name : %s\n",head->Name);
	printf("ID : %u\n",head->Id);
	printf("Age : %u\n",head->Age);
	printf("Daily_step Goals : %u\n",head->Daily_Step_Goal);
	printf(".........................................................\n");


}
status_code get_top3(){
	printf("\n");
	get_top3list();
	// Now display it;
	status_code sc=SUCCESS;
	for(int i=0;i<3;i++){
		if(position[i]==NULL){
			sc=FAILURE;
		}
		else{
			printf("Rank No. ---->>> %d\n",i);
			display_individualData(position[i]);
		}
	}
	if(sc==FAILURE){
		printf("Only these members have completed their goals properly\n");
	}
	else{
		printf("\nTotal steps covered by: \n");
		printf("Rank 1 : %u\n",first);
		printf("Rank 2 : %u\n",second);
		printf("Rank 3 : %u\n",third);

	}
	return sc;
}
status_code check_individual_Rewards(unsigned int Member_Id){
	status_code sc=SUCCESS;
    int reward[3]={100,75,50};
    int flag=0;
	for(int i=0;i<3 && flag==0;i++){
		if(position[i]->Id==Member_Id){
			printf("ID : %u -->> Rank: %d Reward: %d\n",Member_Id,i+1,reward[i]);
			flag=1;
		}
	}
	if(flag==0){
		printf("No reward is achieved\n");
	}
	return sc;
}