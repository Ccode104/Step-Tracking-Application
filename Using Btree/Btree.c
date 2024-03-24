#include<stdio.h>
#include<stdlib.h>

#define MAX 4 //ORDER of the tree is 5, so number of keys at max is 5-1=4
#define MIN 2//Minimum number of keys =(ceil of 5/2) -1 = 2

typedef enum{NO,YES} belongs;
typedef enum{FALSE,TRUE} Boolean;

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
} NodeI;

/*Node type for Group*/
typedef struct NodeG_tag
{
    unsigned int Id;
    unsigned int Rank;
    char Name[20];
    NodeI *Members[5];
    unsigned int Weekly_Group_Goal;
} NodeG;

typedef unsigned int Key;
typedef NodeG TreeentryG;


typedef struct treenodeG {
	int count;/*Except for the root the lower limit is MIN*/
	TreeentryG entry[MAX + 1];
	struct treenodeG *branch[MAX +1];
} TreenodeG;

/*Group Functions*/
Boolean SearchNodeG(Key,TreenodeG*,int*);
Boolean PushdownG(TreeentryG newentry,TreenodeG *current,TreeentryG *medentry,TreenodeG **medright);
void PushInG(TreeentryG medentry,TreenodeG *medright,TreenodeG *current,int pos);
void SplitG(TreeentryG,TreenodeG *,TreenodeG *,int,TreeentryG *,TreenodeG **);




/*
SearchTreeG: traverse B-Tree looking for a target (group).
Assumption
->The Btree pointed to by root has been created.

What will it do?
->If the key target is present in the B-tree, 
then return value points to the node containing target in position targetpos.
Otherwise, the return value is NULL, and targetpos is undefined.

Components:
->SearchTreeG recursively
->SearchNodeG

*/

TreenodeG *SearchTreeG(Key target,TreenodeG *rootG,int *targetpos)
{
	TreenodeG *ret_ptr;
	if(rootG==NULL)
	{
		//Empty tree
		ret_ptr=NULL;
	}
	else if(SearchNodeG(target,rootG,targetpos))
	{
		
		//The target is in the root itself
		ret_ptr=rootG;
	}
	else
	{
		//Search in the branch
		ret_ptr=SearchTreeG(target,rootG->branch[*targetpos],targetpos);
	}
	return ret_ptr;
}

/*The above function is tail recursion*/

/***************************************************************************************/

/*
SearchNode:seraches keys in node for target

Assumptions
->target is a key and current points to a node of a B-Tree

What will it do?
->Searches keys in node for target.Returns location pos of targetor 
branch on which to continue search
*/

Boolean SearchNodeG(Key target,TreenodeG *current,int *pos)
{
	Boolean bool;
	if(target<current->entry[1].Id)
	{
		/*Take the leftmost branch*/
		*pos=0;
		bool=FALSE;
	}
	else
	{
		/*Start a sequential search through keys from end*/

		for(*pos=current->count;(target<current->entry[*pos].Id)&&(*pos>1);(*pos)--);

		if(target==current->entry[*pos].Id)
		{
			bool=TRUE;
		}
		else
		{
			bool=FALSE;
		}
	}
	return bool;
}

TreenodeG* InsertTreeG(TreeentryG newentry,TreenodeG *rootG)
{
	TreeentryG medentry; /*node to be reinserted as new root */
	TreenodeG *medright; /*subtree on right of medentry */
	TreenodeG *newroot; /*used when the height of the tree increases */
	if(PushdownG(newentry,rootG,&medentry,&medright))
	{
		/*Tree grows in height */
	
		newroot=(TreenodeG*)malloc(sizeof(TreenodeG)); /*Make a new root */
		newroot->count=1;
		newroot->entry[1]=medentry;
		newroot->branch[0]=rootG;
		newroot->branch[1]=medright;
		return newroot;
	}
	return rootG;
}

/* PushDownG:recursively belongs in the subtree to which current points*/
Boolean PushdownG(TreeentryG newentry,TreenodeG *current,TreeentryG *medentry,TreenodeG **medright)
{
	Boolean bool;
	int pos; /*branch on which to continue the search*/
	if(current==NULL)
	{
		/*cannot insert into empty tree;terminates*/
		*medentry = newentry;
		*medright=NULL;
		bool=TRUE;
	}
	else
	{
		/*Search the current node*/
		if(SearchNodeG(newentry.Id,current,&pos))
		{
			printf("Warning:Inserting duplicate key into B-tree");
		}
		if(PushdownG(newentry,current->branch[pos],medentry,medright))
		{

			if(current->count<MAX)
			{
				/*Reinsert median key*/
				PushInG(*medentry,*medright,current,pos);
				bool=FALSE;
			}
			else
			{
				SplitG(*medentry,*medright,current,pos,medentry,medright);
				bool=FALSE;
			}
		}
		bool=FALSE;
	}
	return bool;
}

void PushInG(TreeentryG medentry,TreenodeG *medright,TreenodeG *current,int pos)
{
	/*index to move keys to make room for medentry */
	int i;
	for(i=current->count;i>pos;i--)
	{
		/*Shift all keys and branches to the right*/
		current->entry[i+1]=current->entry[i];
		current->branch[i+1]=current->branch[i];
	}
	current->entry[pos+1]=medentry;
	current->branch[pos+1]=medright;
	current->count++;
}


void SplitG(TreeentryG medentry,TreenodeG *medright,TreenodeG *current,int pos,TreeentryG *newmedian,TreenodeG **newright)
{

	int i;
	int median;
	if(pos<=MIN)
	{
		median=MIN;
	}
	else
	{
		median=MIN+1;
	}

	*newright=(TreenodeG*)malloc(sizeof(TreenodeG));

	for(i=median+1;i<=MAX;i++)
	{
		(*newright)->entry[i-median]=current->entry[i];
		(*newright)->branch[i-median]=current->branch[i];
	}

	(*newright)->count=MAX-median;
	current->count=median;
	if(pos<=MIN)
	{
		PushInG(medentry,medright,current,pos);
	}
	else
	{
		PushInG(medentry,medright,*newright,pos-median);
	}
	*newmedian=current->entry[current->count];
	(*newright)->branch[0]=current->branch[current->count];
	current->count--;
}

/*Individual Functions*/
typedef NodeI TreeentryI;


typedef struct treenodeI {
	int count;/*Except for the root the lower limit is MIN*/
	TreeentryI entry[MAX + 1];
	struct treenodeI *branch[MAX +1];
} TreenodeI;

/*Individual Functions Prototypes*/
Boolean SearchNodeI(Key,TreenodeI*,int*);
Boolean PushdownI(TreeentryI newentry,TreenodeI *current,TreeentryI *medentry,TreenodeI **medright);
void PushInI(TreeentryI medentry,TreenodeI *medright,TreenodeI *current,int pos);
void SplitI(TreeentryI,TreenodeI *,TreenodeI *,int,TreeentryI *,TreenodeI **);




/*
SearchTreeI: traverse B-Tree looking for a target (group).
Assumption
->The Btree pointed to by root has been created.

What will it do?
->If the key target is present in the B-tree, 
then return value points to the node containing target in position targetpos.
Otherwise, the return value is NULL, and targetpos is undefined.

Components:
->SearchTreeI recursively
->SearchNodeI

*/

TreenodeI *SearchTreeI(Key target,TreenodeI *rootI,int *targetpos)
{
	TreenodeI *ret_ptr;
	if(rootI==NULL)
	{
		//Empty tree
		ret_ptr=NULL;
	}
	else if(SearchNodeI(target,rootI,targetpos))
	{
		
		//The target is in the root itself
		ret_ptr=rootI;
	}
	else
	{
		//Search in the branch
		ret_ptr=SearchTreeI(target,rootI->branch[*targetpos],targetpos);
	}
	return ret_ptr;
}

/*The above function is tail recursion*/

/***************************************************************************************/

/*
SearchNode:seraches keys in node for target

Assumptions
->target is a key and current points to a node of a B-Tree

What will it do?
->Searches keys in node for target.Returns location pos of targetor 
branch on which to continue search
*/

Boolean SearchNodeI(Key target,TreenodeI *current,int *pos)
{
	Boolean bool;
	if(target<current->entry[1].Id)
	{
		/*Take the leftmost branch*/
		*pos=0;
		bool=FALSE;
	}
	else
	{
		/*Start a sequential search through keys from end*/

		for(*pos=current->count;(target<current->entry[*pos].Id)&&(*pos>1);(*pos)--);

		if(target==current->entry[*pos].Id)
		{
			bool=TRUE;
		}
		else
		{
			bool=FALSE;
		}
	}
	return bool;
}

TreenodeI* InsertTreeI(TreeentryI newentry,TreenodeI *rootI)
{
	TreeentryI medentry; /*node to be reinserted as new root */
	TreenodeI *medright; /*subtree on right of medentry */
	TreenodeI *newroot; /*used when the height of the tree increases */
	if(PushdownI(newentry,rootI,&medentry,&medright))
	{
		/*Tree grows in height */
	
		newroot=(TreenodeI*)malloc(sizeof(TreenodeI)); /*Make a new root */
		newroot->count=1;
		newroot->entry[1]=medentry;
		newroot->branch[0]=rootI;
		newroot->branch[1]=medright;
		return newroot;
	}
	return rootI;
}

/* PushDownI:recursively belongs in the subtree to which current points*/
Boolean PushdownI(TreeentryI newentry,TreenodeI *current,TreeentryI *medentry,TreenodeI **medright)
{
	Boolean bool;
	int pos; /*branch on which to continue the search*/
	if(current==NULL)
	{
		/*cannot insert into empty tree;terminates*/
		*medentry = newentry;
		*medright=NULL;
		bool=TRUE;
	}
	else
	{
		/*Search the current node*/
		if(SearchNodeI(newentry.Id,current,&pos))
		{
			printf("Warning:Inserting duplicate key into B-tree");
		}
		if(PushdownI(newentry,current->branch[pos],medentry,medright))
		{

			if(current->count<MAX)
			{
				/*Reinsert median key*/
				PushInI(*medentry,*medright,current,pos);
				bool=FALSE;
			}
			else
			{
				SplitI(*medentry,*medright,current,pos,medentry,medright);
				bool=FALSE;
			}
		}
		bool=FALSE;
	}
	return bool;
}

void PushInI(TreeentryI medentry,TreenodeI *medright,TreenodeI *current,int pos)
{
	/*index to move keys to make room for medentry */
	int i;
	for(i=current->count;i>pos;i--)
	{
		/*Shift all keys and branches to the right*/
		current->entry[i+1]=current->entry[i];
		current->branch[i+1]=current->branch[i];
	}
	current->entry[pos+1]=medentry;
	current->branch[pos+1]=medright;
	current->count++;
}


void SplitI(TreeentryI medentry,TreenodeI *medright,TreenodeI *current,int pos,TreeentryI *newmedian,TreenodeI **newright)
{

	int i;
	int median;
	if(pos<=MIN)
	{
		median=MIN;
	}
	else
	{
		median=MIN+1;
	}

	*newright=(TreenodeI*)malloc(sizeof(TreenodeI));

	for(i=median+1;i<=MAX;i++)
	{
		(*newright)->entry[i-median]=current->entry[i];
		(*newright)->branch[i-median]=current->branch[i];
	}

	(*newright)->count=MAX-median;
	current->count=median;
	if(pos<=MIN)
	{
		PushInI(medentry,medright,current,pos);
	}
	else
	{
		PushInI(medentry,medright,*newright,pos-median);
	}
	*newmedian=current->entry[current->count];
	(*newright)->branch[0]=current->branch[current->count];
	current->count--;
}


void main()
{
	TreenodeG *rootG=NULL;
	TreenodeI *rootI=NULL;
    NodeI nptrI;
    NodeG nptrG;
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
            fscanf(ptr, "%u", &nptrI.Id);
            nptrI.reward = 0;
            // printf("%u",nptrI->Id);
            fscanf(ptr, "%s", nptrI.Name);
            // printf("%s",nptrI->Name);
            fscanf(ptr, "%u", &nptrI.Age);
            // printf("%u",nptrI->Age);
            fscanf(ptr, "%u", &nptrI.Daily_Step_Goal);
            // printf("%u",nptrI->Daily_Step_Goal);
            for (int i = 0; i < 7; i++)
            {
                fscanf(ptr, "%u", &nptrI.Weekly_Step_Count[i]);
                // printf("%u",nptrI->Weekly_Step_Count[i]);
            }
            InsertTreeI(nptrI,rootI);
        }

        /*Input the Group Details*/
        for (int i = 0; i < 8; i++)
        {
            fscanf(ptr, "%u", &nptrG.Id);
            //printf("\n%u",nptrG.Id);
            fscanf(ptr, "%s", nptrG.Name);
            //printf("\n%s",nptrG.Name);

            for (int j = 0; j < 5; j++)
            {
                fscanf(ptr, "%u", &Member_Id[j]);
                //printf("\n%u",Member_Id[j]);
            }
           // Store_Member_Pointers(nptrG, Member_Id);

            fscanf(ptr, "%u", &nptrG.Weekly_Group_Goal);
            //printf("\n%u",nptrG.Weekly_Group_Goal);
            rootG=InsertTreeG(nptrG,rootG);
        }

        fclose(ptr);

    }

}