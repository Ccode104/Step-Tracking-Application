#include <stdio.h>
#include <stdlib.h>
#include<string.h>

typedef enum
{
    NO,
    YES
} belongs;
/*Node for Individuals*/
typedef struct NodeI_tag
{
    unsigned int Id;
    char Name[20];
    int reward;
    unsigned int Age;
    unsigned int Daily_Step_Goal;
    unsigned int Weekly_Step_Count[7];
    belongs belong;
    struct NodeI_tag *next;
} NodeI;

/*Node type for Group*/
typedef struct NodeG_tag
{
    unsigned int Id;
    unsigned int Rank;
    char Name[20];
    NodeI *Members[5];
    unsigned int Weekly_Group_Goal;
    struct NodeG_tag *next;
} NodeG;

/*Queue for Groups*/
typedef struct QueueG_tag
{
    NodeG *front;
    NodeG *rear;
} QueueG;

/*Hash Table to perform Radix Sort*/
typedef struct HashG_tag
{
    QueueG array[10];
} HashG;

/*Status Code to trace the errors*/
typedef enum
{
    FAILURE,
    SUCCESS
} status_code;

typedef enum
{
    FALSE,
    TRUE
} Boolean;

/*typedef enum
{
	NOT,
	ID,
	STEPS
} sorted;
*/

//Helper Functions
/*Store the Member pointer in the Group*/
status_code Store_Member_Pointers(NodeG*,unsigned int*);

/*Get the Member Pointer given the Member Id*/
NodeI* Search_for_Member_pointer(unsigned int);

/*Get the Group Pointer given the Group Id*/
NodeG* Search_for_Group_pointer(unsigned int);

/*Check if Unique Member*/
Boolean Check_Unique(unsigned int);

/*Compute the Number of Steps Completed by a Member in a week*/
unsigned int Compute_Number_Of_Steps_In_a_Week(NodeI*);

/*Compute the Number of Steps Completed by a Group in a week*/
unsigned int Compute_Number_Of_Steps_by_a_Group(NodeG*);

/*Create the Nodes*/
void CreateNodeI(NodeI**);
void CreateNodeG(NodeG**);

//Main Functionalities
/*Add a Person*/
status_code Add_Person(NodeI*);

/*Create a Group*/
status_code Create_Group(NodeG*);

/*Delete a Group*/
status_code Delete_Group(unsigned int);

/*Merge Groups*/
status_code Merge_Groups(unsigned int,unsigned int);

status_code Display_Member_Info(NodeI*);

/*Display Group Info*/
status_code Display_Group_Info();

/*Check Group Achievement*/
Boolean Check_Group_Achievement(unsigned int);

/*Generate the Leader board for Groups*/
status_code Generate_Leader_Board();

/*Radix Sort Groups based on Id*/
void radixsort_groups_Id();

/*Radix Sort Groups based on Steps*/
void radixsort_groups_steps();

/*Insert an element in the Queue*/
void InsertQueue(QueueG*,NodeG*);

/*Checkif Queue is Empty*/
Boolean isQueueEmpty(QueueG*);

/*Initialise the Hashtable*/
void InitHash(HashG*);

/*Get the maximum Id of group*/
unsigned int getmax_GId();

/*Get the maximum steps of group*/
unsigned int getmax_GSteps();

NodeI *lptrI = NULL;
NodeG *lptrG = NULL;
unsigned int first, second, third;
NodeI *position[3];
//sorted s=NOT;

void CreateNodeI(NodeI **npptr)
{
    *npptr = (NodeI *)malloc(sizeof(NodeI));
    (*npptr)->belong = 0;
    (*npptr)->next = NULL;
}

void CreateNodeG(NodeG **npptr)
{
    *npptr = (NodeG *)malloc(sizeof(NodeG));
    (*npptr)->next = NULL;
}

status_code Add_Person(NodeI *nptr)
{
    NodeI *ptr = lptrI;
    NodeI *prev = NULL;
    status_code sc = SUCCESS;

    if (lptrI == NULL)
    {
        // 0 node case;
        lptrI = nptr;
        nptr->next=NULL;
    }
    else
    {
        while ((ptr != NULL) && (nptr->Id > ptr->Id))
        {
            prev = ptr;
            ptr = ptr->next;
        }
        if (ptr != NULL)
        {
            if (ptr->Id == nptr->Id)
            {
                sc = FAILURE;
            }
            else
            {
                if (prev == NULL)
                {
                    nptr->next = lptrI;
                    lptrI = nptr;
                }
                else
                {
                    nptr->next = ptr;
                    prev->next = nptr;
                }
            }
        }
        else
        {
            prev->next = nptr;
        }
    }

    return sc;
}

status_code Create_Group(NodeG *nptr)
{
    NodeG *ptr = lptrG;
    NodeG *prev = NULL;
    status_code sc = SUCCESS;

    if (lptrG == NULL)
    {
        // Empty list
        lptrG = nptr;
        nptr->next=NULL;
    }
    else
    {
        while ((ptr != NULL) && (nptr->Id > ptr->Id))
        {
            // The Id of the Group to be inserted is larger
            prev = ptr;
            ptr = ptr->next;
        }
        if (ptr != NULL)
        {
            if (ptr->Id == nptr->Id)
            {
                sc = FAILURE;
            }
            else
            {
                if (prev == NULL)
                {
                    // Insert At Start
                    nptr->next = lptrG;
                    lptrG = nptr;
                }
                else
                {
                    nptr->next = ptr;
                    prev->next = nptr;
                }
            }
        }
        else
        {
            prev->next = nptr;
        }
    }
    return sc;
}

NodeI *Search_for_Member_pointer(unsigned int Member_Id)
{
    NodeI *Member_ptr = NULL;
    NodeI *ptr = lptrI;
    while (ptr != NULL && ptr->Id != Member_Id)
    {
        ptr = ptr->next;
    }
    if (ptr != NULL)
    {
        Member_ptr = ptr;
    }
    else
    {
        printf("Member id Not found\n");
    }
    return Member_ptr;
}

NodeG *Search_for_Group_pointer(unsigned int Group_Id)
{
    NodeG *Group_ptr = NULL;
    NodeG *ptr = lptrG;

    while (ptr != NULL && ptr->Id < Group_Id)
    {
        ptr = ptr->next;
    }
    if ((ptr != NULL)&&(ptr->Id==Group_Id))
    {
        Group_ptr = ptr;
    }
    return Group_ptr;
}

status_code Store_Member_Pointers(NodeG *nptr, unsigned int Member_Id[])
{
    status_code sc = SUCCESS;

    for (int i = 0; i < 5; i++)
    {
        if ((Member_Id[i] != 0))
        {
            nptr->Members[i] = Search_for_Member_pointer(Member_Id[i]);
            if (nptr->Members[i]->belong == 1)
            {
                nptr->Members[i] = NULL;
            }
            else
            {
                nptr->Members[i]->belong = 1;
            }
        }
        else
        {
            // printf("NULL");
            nptr->Members[i] = NULL;
        }
    }

    return sc;
}

void Mark_belongs_as_zero(NodeG *head)
{
    for (int i = 0; i < 5; i++)
    {
        if (head->Members[i] != NULL)
        {
            head->Members[i]->belong = 0;
        }
    }
    return;
}
status_code Delete_Group(unsigned int Group_Id)
{
    NodeG *ptr = lptrG;
    NodeG *prev = NULL;
    status_code sc = SUCCESS;
  
    while ((ptr != NULL) && (ptr->Id < Group_Id))
    {
        prev = ptr;
        ptr = ptr->next;
    }

    if (ptr == NULL || ptr->Id > Group_Id)
    {
        printf("Enter the correct group id. \n");
        sc = FAILURE;
    }
    else
    {
        // Mark all Members of this group as belongs=0;
        Mark_belongs_as_zero(ptr);
        if (prev == NULL)
        {
            lptrG = lptrG->next;
            free(ptr);
        }
        else
        {
            prev->next = ptr->next;
            free(ptr);
        }
    }
    
    return sc;
}

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
            scanf("%s", newgroup->Name);
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

void delete_individual_from_group(NodeI *ptr)
{
    NodeG *head = lptrG;
    int flag = 0;
    int i;

    while (head && flag == 0)
    {
        for (i = 0; i < 5 && flag == 0; i++)
        {
            if (head->Members[i] == ptr)
            {
                flag = 1;
                head->Members[i] = NULL;
            }
        }
        head = head->next;
    }
    while (i < 5)
    {
        head->Members[i - 1] = head->Members[i];
        i++;
    }

    //In case we had 5 Members earlier
    head->Members[4] = NULL;

}
status_code Delete_individual(unsigned int Member_Id)
{
    status_code sc = SUCCESS;
    NodeI *ptr = lptrI;
    NodeI *prev = NULL;

    while (ptr != NULL && ptr->Id < Member_Id)
    {
        prev = ptr;
        ptr = ptr->next;
    }
    if (ptr == NULL || ptr->Id > Member_Id)
    {
        // No member found with the given id;
        printf("Enter the correct Member_ID \n");
        sc = FAILURE;
    }
    else
    {
        // The id must be equal means it is present
        // Now if the individual belong to a gruop then we have to delete it from that group also;
        if (ptr->belong == 1)
        {
            // It belong to some group;
            delete_individual_from_group(ptr);
        }
        if (prev == NULL)
        {
            NodeI *nptr = ptr->next;
            free(ptr);
            lptrI = nptr;
        }
        else
        {
            NodeI *nptr = ptr->next;
            free(ptr);
            prev->next = nptr;
        }
        ptr = NULL;
    }

    return sc;
}

status_code Display_Member_Info(NodeI *ptr)
{
    status_code sc = SUCCESS;

    if (ptr == NULL)
    {
        sc = FAILURE;
    }
    else
    {

        printf("\nMember Id : %u ", ptr->Id);
        printf("\nMember Name : %s ", ptr->Name);
        printf("\nMember Age : %u ", ptr->Age);
    }
    return sc;
}
status_code Display_Group_Info_by_Id(unsigned int Id)
{
    status_code sc=SUCCESS;
    NodeG *gptr=Search_for_Group_pointer(Id);
    NodeG *ptr=lptrG;
    unsigned int rank=1;

    if(gptr==NULL)
    {
        sc=FAILURE;
        printf("\nThe group does not exist!\n");
    }
    else{
        radixsort_groups_steps();
        ptr=lptrG;
        rank=1;
        while(ptr!=gptr)
        {
            rank++;
            ptr=ptr->next;
        }

        radixsort_groups_Id();
       
        printf("\nRank of the Group is %u\n",rank);
        printf("\nGroup Id : %u ",gptr->Id);
        printf("\nGroup Name : %s ",gptr->Name);
        printf("\nDetails of the Members");
        for(int i=0;i<5;i++)
        {
            Display_Member_Info(gptr->Members[i]);
        }
        printf("\nWeekly Group Goal : %u \n",gptr->Weekly_Group_Goal);
    }
    return sc;



}
status_code Display_Group_Info()
{
    status_code sc=SUCCESS;
    unsigned int rank=1;
    NodeG *ptr=lptrG;

    if(ptr==NULL)
    {
        sc=FAILURE;
    }
    else
    {
        radixsort_groups_steps();

        ptr=lptrG;
        rank=1;
	    while(ptr!=NULL)
        {
            ptr->Rank=rank;
            rank++;
            ptr=ptr->next;
        }

        radixsort_groups_Id();
       

        ptr=lptrG;

        while(ptr!=NULL)
        {
            printf("\nRank of the Group is %u\n",ptr->Rank);
            printf("\nGroup Id : %u ",ptr->Id);
            printf("\nGroup Name : %s ",ptr->Name);
            printf("\nDetails of the Members");
            for(int i=0;i<5;i++)
            {
                Display_Member_Info(ptr->Members[i]);
            }
            printf("\nWeekly Group Goal : %u \n",ptr->Weekly_Group_Goal);
            ptr=ptr->next;
        }
    }
    return sc;
}

unsigned int Compute_Number_Of_Steps_In_a_Week(NodeI *nptr)
{
    unsigned int steps = 0;
    if (nptr == NULL)
    {
        steps = 0;
    }
    else
    {
        for (int i = 0; i < 7; i++)
        {
            steps += nptr->Weekly_Step_Count[i];
        }
    }
    return steps;
}

unsigned int Compute_Number_Of_Steps_by_a_Group(NodeG *nptr)
{
    unsigned int steps = 0;
    if (nptr == NULL)
    {
        steps = 0;
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            steps += Compute_Number_Of_Steps_In_a_Week(nptr->Members[i]);
        }
    }
    return steps;
}

Boolean target_complete(NodeI *curr, unsigned int *steps)
{
    Boolean res = TRUE;
    for (int i = 0; i < 7 && res == TRUE; i++)
    {
        *steps += curr->Weekly_Step_Count[i];
        if (curr->Daily_Step_Goal > curr->Weekly_Step_Count[i])
        {
            // The target is incomplete
            res = FALSE;
        }
    }
    return res;
}

Boolean Check_Group_Achievement(unsigned int Group_Id)
{
    Boolean bool = TRUE;
    unsigned int count = 0;
    NodeG *ptr;

    //Get the Pointer to the Group
    ptr = Search_for_Group_pointer(Group_Id);

    if (ptr == NULL)
    {
        //Group Id does not Exists
        bool = FALSE;
        printf("\nGroup Does Not Exist!");
    }
    else
    {
        //Group Id Exists

        //Get the Number of Steps Completed by the Group
        count=Compute_Number_Of_Steps_by_a_Group(ptr);

        if (count >= ptr->Weekly_Group_Goal)
        {
            bool = TRUE;
            printf("\nThe group has completed the Weekly Goal of %u Steps by walking %u Steps in a Week",ptr->Weekly_Group_Goal,Compute_Number_Of_Steps_by_a_Group(ptr));
        }
        else
        {
            bool = FALSE;
            printf("\nThe group has not completed the Weekly Goal of %u Steps by walking %u Steps in a Week",ptr->Weekly_Group_Goal,Compute_Number_Of_Steps_by_a_Group(ptr));
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
    return max;
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
    return max;
}
void InitHash(HashG *h)
{
    for(int i=0;i<10;i++)
    {
        (h->array[i]).front=NULL;
        (h->array[i]).rear=NULL;
    }
}
Boolean isQueueEmpty(QueueG *q)
{
    Boolean bool;

    if((q->front==NULL)&&(q->rear==NULL))
    {
        bool=TRUE;
    }
    else
    {
        bool=FALSE;
    }

    return bool;
}
void InsertQueue(QueueG *q,NodeG *nptr)
{
    if(isQueueEmpty(q))
    {
        //Empty Queue
        q->front=nptr;
        q->rear=nptr;
        nptr->next=NULL;
    }
    else
    {
        (q->rear)->next=nptr;
        q->rear=nptr;
        nptr->next=NULL;
    }
}
void radixsort_groups_steps()
{
    NodeG *ptr=lptrG,*next_ptr=NULL;
    status_code sc=SUCCESS;
    unsigned int digits=0;//max number of digits
    unsigned int div=1;//dividing factor to get digits
    unsigned int flag=0;
    unsigned int max=0;//max number of steps
    unsigned int steps=0;//number of steps
    int i=0,j=0;
    HashG h;//Array of Queues used for radix sort

    InitHash(&h);//Initialise the array of Queues

    if(lptrG==NULL)
    {
        //Empty list
        sc=FAILURE;
    }
    else{

        //Get the maximum number of steps
        max=getmax_GSteps();
        //printf("\nmax steps=%u",max);
        
        //Max number of digits
        while(max>0)
        {
            max=max/10;
            digits=digits+1;
        }
        //printf("\ndigits=%u",digits);
        
        div=1;
        for(int k=0;k<digits;k++)
        {
            ptr=lptrG;

            while(ptr!=NULL)
            {
                //Store next_ptr since next field of ptr is modified by the InsertQueue
                next_ptr=ptr->next;

                steps=Compute_Number_Of_Steps_by_a_Group(ptr);
                //printf("\nSteps=%u",steps);

                //Compute the index of the Queue where the node is to be inserted
                steps=(steps/div)%10;
                //printf("\nIndex=%u",steps);

                //Insert the Node based on the digit(index)
                InsertQueue(&h.array[steps],ptr);

                ptr=next_ptr;
            }
            
            i=9;
            while(isQueueEmpty(&h.array[i]))
            {
                //Decrement 'i' till we get a non-Empty Queue
                i--;
            }
            //printf("\ni=%d",i);

            lptrG=h.array[i].front;
            flag=0;
            
            while((i>0)&&(flag!=1))
            {
                j=i-1;
                while((j>=0)&&(isQueueEmpty(&h.array[j])))
                {
                    //Decrement 'j' till we get the closest Non-Empty Queue to 'i'
                    j--;
                }
                if(j<0)
                {
                    flag=1;
                }
                else
                {
                    //Connect the Queues to get a Linked List
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
    unsigned int digits=0;//max number of digits
    unsigned int div=1;//dividing factor to get digits
    unsigned int flag=0;
    unsigned int max=0;//max Id
    unsigned int Id=0;//Id
    int i=0,j=0;
    HashG h;//Hashtable used for radix sort

    InitHash(&h);
    if(lptrG==NULL)
    {
        //Empty list
        sc=FAILURE;
    }
    else{

        //Get the maximum number of steps
        max=getmax_GId();
        //printf("\nmax Id=%u",max);
        
        //Max number of digits
        while(max>0)
        {
            max=max/10;
            digits=digits+1;
        }
        //printf("\ndigits=%u",digits);
        
        div=1;
        for(int k=0;k<digits;k++)
        {
            ptr=lptrG;
            while(ptr!=NULL)
            {
                next_ptr=ptr->next;
                Id=ptr->Id;
                //printf("\nId=%u",steps);
                Id=(Id/div)%10;
                //printf("\nIndex=%u",steps);
                InsertQueue(&h.array[Id],ptr);
                ptr=next_ptr;
            }
            
            i=0;
            while(isQueueEmpty(&h.array[i]))
            {
                i++;
            }
            //printf("\ni=%d",i);
            lptrG=h.array[i].front;
            flag=0;
            
            while((i<9)&&(flag!=1))
            {
                j=i+1;
                while((j<=9)&&(isQueueEmpty(&h.array[j])))
                {
                    j++;
                }
                if(j>9)
                {
                    flag=1;
                }
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
        printf("\nRank = %d",r);
        printf("\nGroup Name %s ",ptr->Name);
    	printf("\nSteps = %u\n", Compute_Number_Of_Steps_by_a_Group(ptr));
        ptr=ptr->next;
        r++;
    }
    radixsort_groups_Id();

    return sc;

}

void get_top3list()
{

    first = second = third = 0;
    NodeI *curr = lptrI;
    while (curr != NULL)
    {
        unsigned int steps = 0;
        Boolean check = target_complete(curr, &steps);
        if (check == TRUE)
        {
            // The member has completed therir daily step goals

            if (position[0] == NULL)
            {
                position[0] = curr;
                first = steps;
            }
            else if (position[1] == NULL)
            {
                if (steps > first)
                {
                    // The current one has more number of steps
                    position[1] = position[0];
                    position[0] = curr;
                    second = first;
                    first = steps;
                }
                else
                {
                    position[1] = curr;
                    second = steps;
                }
            }
            else if (position[2] == NULL)
            {
                if (steps > first)
                {
                    position[2] = position[1];
                    position[1] = position[0];
                    position[0] = curr;
                    third = second;
                    second = first;
                    first = steps;
                }
                else if (steps > second)
                {
                    position[2] = position[1];
                    position[1] = curr;
                    third = second;
                    second = steps;
                }
                else
                {
                    position[2] = curr;
                    third = steps;
                }
            }
            else
            {
                if (steps > first)
                {
                    position[2] = position[1];
                    position[1] = position[0];
                    position[0] = curr;
                    third = second;
                    second = first;
                    first = steps;
                }
                else if (steps > second)
                {
                    position[2] = position[1];
                    position[1] = curr;
                    third = second;
                    second = steps;
                }
                else if (steps > third)
                {
                    position[2] = curr;
                    third = steps;
                }
                // If it don't eneter into any of the condition then this means that the member_steps < third;
            }
        }
        curr = curr->next;
    }
}
void display_individualDatacompletely()
{

    NodeI *head = lptrI;
    while (head)
    {
        Display_Member_Info(head);
        printf("\n");
        head = head->next;
    }
}
status_code get_top3()
{

    printf("\n");
    get_top3list();

    status_code sc = SUCCESS;

    // if flag==1 then perform this printing statements else dont perform
    for (int i = 0; i < 3; i++)
    {
        if (position[i] == NULL)
        {
            sc = FAILURE;
        }
        else
        {

            printf("\n\nRank No. ---->>> %d", i + 1);
            Display_Member_Info(position[i]);
        }
    }
    if (sc == FAILURE)
    {
        printf("Only these members have completed their goals properly\n");
    }
    else
    {
        printf("\n\nTotal steps covered by: \n");
        printf("Rank 1 : %u\n", first);
        printf("Rank 2 : %u\n", second);
        printf("Rank 3 : %u\n", third);
        printf("\n");
    }

    return sc;
}
status_code check_individual_Rewards(unsigned int Member_Id)
{
    int prize[3] = {100, 75, 50};
    status_code sc = SUCCESS;
    NodeI *ptr = Search_for_Member_pointer(Member_Id);
    
    if (ptr == NULL)
    {
        sc = FAILURE;
    }
    else
    {
        get_top3list();

        for (int i = 0; i < 3; i++)
        {
            if (position[i] != NULL)
                position[i]->reward = prize[i];
        }
        printf("Reward: %d\n", ptr->reward);
    }
    return sc;
}
status_code suggest_goalUpdates(unsigned int id)
{
    status_code sc = SUCCESS;
    get_top3list();
    NodeI *ptr = Search_for_Member_pointer(id);
    if (ptr == NULL)
    {
        printf("Enter the valid id input.\n");
    }
    else
    {
        get_top3list();
        int flag = 0;
        for (int i = 0; i < 3; i++)
        {
            if (position[i]->Id == id)
            {
                printf("The user is already in top 3 he/she is consistent in his goals\n");
                flag = 1;
            }
        }
        if (flag == 0)
        {
            unsigned int steps = 0;
            Boolean check = target_complete(ptr, &steps);
            if (check == FALSE)
            {
                printf("The user is not consistent he/she should complete daily_step goal daily without any fail\n");
            }
            unsigned int avg = (first + second + third) / 21;
            // Average
            printf("The user should set an avg of %u daily step goal to be in top 3 and should be consistent in his job\n", avg);
        }
    }
}
void main()
{

    NodeI *nptrI;
    NodeG *nptrG;
    unsigned int Member_Id[5];

    // Input the details
    FILE *ptr = fopen("data.txt", "r");
    if (ptr == NULL)
    {
        printf("no such file.");
    }
    else
    {

        /*Input the Individual Details*/
        for (int j = 0; j < 20; j++)
        {

            CreateNodeI(&nptrI);
            fscanf(ptr, "%u", &nptrI->Id);
            nptrI->reward = 0;
            // printf("%u",nptrI->Id);
            fscanf(ptr, "%s", nptrI->Name);
            // printf("%s",nptrI->Name);
            fscanf(ptr, "%u", &nptrI->Age);
            // printf("%u",nptrI->Age);
            fscanf(ptr, "%u", &nptrI->Daily_Step_Goal);
            // printf("%u",nptrI->Daily_Step_Goal);
            for (int i = 0; i < 7; i++)
            {
                fscanf(ptr, "%u", &nptrI->Weekly_Step_Count[i]);
                // printf("%u",nptrI->Weekly_Step_Count[i]);
            }
            Add_Person(nptrI);
        }

        for (int i = 0; i < 5; i++)
        {

            CreateNodeG(&nptrG);
            fscanf(ptr, "%u", &nptrG->Id);
            // printf("\n%u",nptrG->Id);
            fscanf(ptr, "%s", nptrG->Name);
            // printf("\n%s",nptrG->Name);

            for (int j = 0; j < 5; j++)
            {
                fscanf(ptr, "%u", &Member_Id[j]);
                // printf("\n%u",Member_Id[j]);
            }
            Store_Member_Pointers(nptrG, Member_Id);

            fscanf(ptr, "%u", &nptrG->Weekly_Group_Goal);
            // printf("\n%u",nptrG->Weekly_Group_Goal);
            Create_Group(nptrG);
        }

        fclose(ptr);

        int value = 0;
        status_code sc;
        while (value != -1)
        {
            printf("\nMenu\n");
            printf("1 Add Person\n");
            printf("2 Create Group\n");
            printf("3 Get top 3 Individuals\n");
            printf("4 Check Group Achievement\n");
            printf("5 Generate Leader Board\n");
            printf("6 Check Individual Rewards\n");
            printf("7 Delete Individual\n");
            printf("8 Delete Group\n");
            printf("9 Merge Groups\n");
            printf("10 Display Group Info\n");
            printf("11 Suggest Goal Updates\n\n");
            printf("\nEnter the number of the function: ");
            scanf("%d", &value);
            if (value == 1)
            {
                printf("Enter the details for the Individual to be added\n");
                
                NodeI *nptr;
                CreateNodeI(&nptr);
                
                printf("Enter the Individual Id: ");
                unsigned int id;
                scanf("%u", &id);
                nptr->Id = id;
                
                sc = Add_Person(nptr);
                
                if (sc == SUCCESS)
                {
                    printf("Enter the Name: ");
                    scanf("%s",nptr->Name);
                    
                    printf("Enter the Age: ");
                    scanf("%u", &nptr->Age);
                    nptr->belong = 0;
                    
                    printf("Enter daily step goals: ");
                    scanf("%u", &nptr->Daily_Step_Goal);
                    
                    printf("Enter the weekly step count : \n");
                    
                    for (int i = 0; i < 7; i++)
                    {
                        scanf("%u", &nptr->Weekly_Step_Count[i]);
                    }
                    
                    printf("\n");
                    
                    display_individualDatacompletely();
                }
                else
                {
                    printf("The Id already exist \n");
                    free(nptr);
                }
            }
            else if (value == 2)
            {
                printf("\nEnter the Details of the Group to be Created\n");

                //Create a Node of Group type 
                NodeG *nptr;
                CreateNodeG(&nptr);

                //Get the Id 
                printf("Enter the Id: ");
                unsigned int id;
                scanf("%u", &id);
                nptr->Id = id;

                //Insert it in Constant Time 
                sc = Create_Group(nptr);

                if (sc == SUCCESS)
                {
                    //Successful Insertion

                    //Get the Name
                    printf("Enter the Name: ");
                    scanf("%s",nptr->Name);

                    //Get the Weekly Group Goal
                    printf("Enter the weekly group goals :");
                    scanf("%u", &nptr->Weekly_Group_Goal);

                    //Get the Members
                    unsigned int memberid;
                    int i = 0;
                    int complete = 1;

                    while (i < 5 && complete == 1)
                    {
                        //Max number of Members is 5
                        //Member Id 0 means Stop

                        //Get the Member ID  
                        printf("Enter the id of individual : ");
                        scanf("%u", &memberid);

                        NodeI *temp = Search_for_Member_pointer(memberid);
                        
                        if (temp != NULL)
                        {   
                            //If the Individual Id Exists
                            if (temp->belong == 1)
                            {
                                //If the individual already belongs to some Group
                                printf("The Member:  %s id: %u already belongs to some group .\n", temp->Name, temp->Id);
                            }
                            else
                            {
                                temp->belong = 1;
                                nptr->Members[i] = temp;
                                i++;
                            }
                        }
                        printf("Enter 1 to add more member else 0 if completed\n");
                        scanf("%d", &complete);
                    }

                    //Set the rest Member Pointers to NULL
                    while (i < 5)
                    {
                        nptr->Members[i] = NULL;
                        i++;
                    }

                    Display_Group_Info();
                }
                else
                {
                    printf("Already exist \n");
                }
            }
            else if (value == 3)
            {
                printf("\nThe top three Individuals\n");
                sc = get_top3();
            }
            else if (value == 4)
            {
                printf("\nEnter the Details of the Group whose achievement is to be checked\n");
               
                // Only group 4 hasn't complete its weekly step goal, rest all have done it
                printf("Enter the group_id to check its Achievement : ");
                unsigned id;
                scanf("%u", &id);

                Boolean check = Check_Group_Achievement(id);
                printf("\n");
            }
            else if (value == 5)
            {
                printf("\nGroups Leaderboard based on Number of Steps completed\n");
                // Generate Leader board
                Generate_Leader_Board();
            }
            else if (value == 6)
            {
                printf("\nEnter the Details of the Individual whose Reward you want to check\n");
                
                printf("Enter the id to check its reward: ");
                unsigned int id;
                scanf("%u", &id);
                
                sc = check_individual_Rewards(id);
                
                if (sc == FAILURE)
                {
                    printf("Enter the valid ID input \n");
                }
            }
            else if (value == 7)
            {
                printf("Enter the Individual id to be deleted:  ");
                unsigned int id;
                scanf("%u", &id);

                sc = Delete_individual(id);
                if (sc == SUCCESS)
                    display_individualDatacompletely();
            }
            else if (value == 8)
            {
                printf("Enter the Group id to be deleted:  ");
                unsigned int id;
                scanf("%u", &id);
                
                sc = Delete_Group(id);
            }
            else if (value == 9)
            {
                unsigned int id1, id2;
                printf("Enter the two Group IDs to be merged : ");
                scanf("%u%u", &id1, &id2);

                sc = Merge_Groups(id1, id2);
                Display_Group_Info();
            }

            else if (value == 10)
            {
                unsigned int id;

                printf("Enter the Group id to whose info is to be dispalyed:  ");
                scanf("%u",&id);
                printf("\nDetails of the Group :\n");
                Display_Group_Info_by_Id(id);
            }
            else if (value == 11)
            {
                printf("Enter the Individual Id for which suggession is required: ");
                unsigned int id;
                scanf("%u", &id);
                
                sc = suggest_goalUpdates(id);
            }
            else
            {
                printf("Enter the valid input !!\n");
            }
        }
    }
}