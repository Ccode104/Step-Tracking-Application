#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 4 //ORDER of the tree is 5, so number of keys at max is 5-1=4
#define MIN 2//Minimum number of keys =(ceil of 5/2) -1 = 2

typedef enum{FAILURE,SUCCESS} status_code;
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
    char Name[20];
    NodeI *Members[5];
    unsigned int Weekly_Group_Goal;
} NodeG;

typedef struct NodeS_tag
{
    unsigned int Id;
    char Name[20];
    unsigned int steps;
} NodeS;



typedef unsigned int Key;
typedef NodeG TreeentryG;
typedef NodeI TreeentryI;
typedef NodeS TreeentryS;

typedef struct treenodeI {
	int count;/*Except for the root the lower limit is MIN*/
	TreeentryI entry[MAX + 1];
	struct treenodeI *branch[MAX +1];
} TreenodeI;

typedef struct treenodeG {
	int count;/*Except for the root the lower limit is MIN*/
	TreeentryG entry[MAX + 1];
	struct treenodeG *branch[MAX +1];
} TreenodeG;

typedef struct treenodeS {
	int count;/*Except for the root the lower limit is MIN*/
	TreeentryS entry[MAX + 1];
	struct treenodeS *branch[MAX +1];
} TreenodeS;

NodeI *position[3];
unsigned int first,second,third;

/*Store the Member pointer in the Group*/
status_code Store_Member_Pointers(NodeG*,unsigned int*,TreenodeI*);

/*Get the Member Pointer given the Member Id*/
NodeI* Search_for_Pointer_to_Individual(unsigned int,TreenodeI*);

/*Get the Group Pointer given the Group Id*/
NodeG* Search_for_Pointer_to_Group(unsigned int,TreenodeG*);

/*Check if Unique Member*/
Boolean Check_Unique(unsigned int);

/*Compute the Number of Steps Completed by a Member in a week*/
unsigned int Compute_Number_Of_Steps_In_a_Week(NodeI*);

/*Compute the Number of Steps Completed by a Group in a week*/
unsigned int Compute_Number_Of_Steps_by_a_Group(NodeG*);

TreenodeS* DeleteNodeS(TreenodeS*,unsigned int);

/*Create the Nodes*/
NodeI* CreateNodeI();
NodeG* CreateNodeG();
NodeS* CreateNodeS();

//Main Functionalities
/*Add a Person*/
status_code Add_Person(NodeI*);

/*Create a Group*/
TreenodeG* Create_Group(NodeG*,TreenodeG*,status_code*,TreenodeS**);

TreenodeS* Create_Sroup(NodeS*,TreenodeS*,status_code*);

/*Delete a Group*/
TreenodeG* Delete_Group(unsigned int,TreenodeG*,TreenodeS**);

TreenodeS* Delete_Sroup(unsigned int,TreenodeS*);

/*Merge Groups*/
TreenodeG* Merge_Groups(unsigned int,unsigned int,TreenodeG*,TreenodeS**);

status_code Display_Member_Info(NodeI*);

/*Display Group Info*/
status_code Display_Group_Info();

/*Check Group Achievement*/
Boolean Check_Group_Achievement(unsigned int,TreenodeG*);

/*Generate the Leader board for Groups*/
void Generate_Leader_Board(TreenodeG*,TreenodeS*);

/*Group Functions Prototypes*/
Boolean SearchNodeG(Key,TreenodeG*,int*);
Boolean PushdownG(TreeentryG newentry,TreenodeG *current,TreeentryG *medentry,TreenodeG **medright);
void PushInG(TreeentryG medentry,TreenodeG *medright,TreenodeG *current,int pos);
void SplitG(TreeentryG,TreenodeG *,TreenodeG *,int,TreeentryG *,TreenodeG **);
void traversalG(TreenodeG*,int,int);




/*
SearchTreeG: traverse B-Tree looking for a target (group).
Assumption
->The Btree pointed to by root has been created.

What will it do?
->Gf the key target is present in the B-tree,
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
		return NULL;
	}
	else if(SearchNodeG(target,rootG,targetpos))
	{

		//The target is in the root itself
		return rootG;
	}
	else
	{
		//Search in the branch
		return SearchTreeG(target,rootG->branch[*targetpos],targetpos);
	}
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

		for(*pos=current->count;(target<current->entry[*pos].Id)&&((*pos)>1);(*pos)--);

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
		return TRUE;
	}
	else
	{
		/*Search the current node*/
		if(SearchNodeG(newentry.Id,current,&pos))
		{
			printf("Warning:Inserting duplicate key into B-tree");
		}
		else if(PushdownG(newentry,current->branch[pos],medentry,medright))
		{

			if(current->count<MAX)
			{
				/*Reinsert median key*/
				PushInG(*medentry,*medright,current,pos);
				return FALSE;
			}
			else
			{
				SplitG(*medentry,*medright,current,pos,medentry,medright);
				return TRUE;
			}
		}
		return FALSE;
	}
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
		return NULL;
	}
	else if(SearchNodeI(target,rootI,targetpos))
	{

		//The target is in the root itself
		return rootI;
	}
	else
	{
		//Search in the branch
		return SearchTreeI(target,rootI->branch[*targetpos],targetpos);
	}
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

		for(*pos=current->count;(target<current->entry[*pos].Id)&&((*pos)>1);(*pos)--);

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
		return TRUE;
	}
	else
	{
		/*Search the current node*/
		if(SearchNodeI(newentry.Id,current,&pos))
		{
			printf("Warning:Inserting duplicate key into B-tree");
		}
		else if(PushdownI(newentry,current->branch[pos],medentry,medright))
		{

			if(current->count<MAX)
			{
				/*Reinsert median key*/
				PushInI(*medentry,*medright,current,pos);
				return FALSE;
			}
			else
			{
				SplitI(*medentry,*medright,current,pos,medentry,medright);
				return TRUE;
			}
		}
		return FALSE;
	}
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

void MoveRightI(TreenodeI* current,int pos){
	int c;
	TreenodeI* t;
	t=current->branch[pos];
	for(c=t->count;c>0;c--){
		t->entry[c+1]=t->entry[c];
		t->branch[c+1]=t->branch[c];
	}
	t->branch[1]=t->branch[0] ; // Move key from parent to rightnode;
	t->count++;
	t->entry[1]=current->entry[pos];
	t=current->branch[pos-1]; // MOve last key of left node int parent;
	current->entry[pos]=t->entry[t->count];
	current->branch[pos]=t->branch[t->count];
	t->count--;
}
void MoveLeftI(TreenodeI* current,int pos){
	int c;
	TreenodeI* t;
	t=current->branch[pos-1];  // Move key from parent to left node
	t->count++;
	t->entry[t->count]=current->entry[pos];
	t->branch[t->count]=current->branch[pos]->branch[0];
	t=current->branch[pos];
	current->entry[pos]=t->entry[1];
	t->branch[0]=t->branch[1];
	t->count--;
	for(c=1;c<=t->count;c++){
		// Shift all keys in right node one position leftward
		t->entry[c]=t->entry[c+1];
		t->branch[c]=t->branch[c+1];
	}

}
void combineI(TreenodeI* current,int pos){
	int c;
	TreenodeI* right,*left;
	right=current->branch[pos];
	left=current->branch[pos-1]; // Work with left node
	left->count++; // INsert the key from the parent;
	left->entry[left->count]=current->entry[pos];
	left->branch[left->count]=right->branch[0];
	for(c=1;c<=right->count;c++){
		left->count++;
		left->entry[left->count]=right->entry[c];
		left->branch[left->count]=right->branch[c];
	}
	for(c=pos;c<current->count;c++){
		// Deleting key from parent node and shifting others 
		current->entry[c]=current->entry[c+1];
		current->branch[c]=current->branch[c+1];
	}
	current->count--;
	free(right);

}
void RestoreI(TreenodeI* current,int pos){
	if(pos==0){
		// Leftmost key
		if(current->branch[1]->count>MIN){
			MoveLeftI(current,1);
		}
		else{
			combineI(current,1);
		}
	}
	else if(pos==current->count){
		// rightmost key;
	    if(current->branch[pos-1]->count>MIN){
			MoveRightI(current,pos);

		}
		else{
			combineI(current,pos);
		}
	}
	else if(current->branch[pos-1]->count>MIN){
		MoveRightI(current,pos);
	}
	else if(current->branch[pos+1]->count>MIN)
	{
		MoveLeftI(current,pos+1);
	}
	else{
		combineI(current,pos);
	}
}
void RemoveI(TreenodeI* current,int pos){
	int i;
	for(i=pos+1;i<=current->count;i++){
		current->entry[i-1]=current->entry[i];
		current->branch[i-1]=current->branch[i];

	}
	current->count--;
}
void SuccessorI(TreenodeI* currrent,int pos){
	TreenodeI* leaf;
	for(leaf=currrent->branch[pos];leaf->branch[0];leaf=leaf->branch[0]);
	currrent->entry[pos]=leaf->entry[1];
	
}
void RecDeleteNodeI(TreenodeI* current,Key id){
	int pos;
	if(current==NULL){
		printf("Id not found!!\n");
		return ;
	}
	if(SearchNodeI(id,current,&pos)){
		// Target is founfd in the current nodeI
		if(current->branch[pos-1]){
			// When deletion not from leaf node so we take successor node of child to parent 
			SuccessorI(current,pos);
			RecDeleteNodeI(current->branch[pos],current->entry[pos].Id);
		}
		else{
			// When the entry is removed from leaf node
			RemoveI(current,pos);

		}
	}
	else{
		RecDeleteNodeI(current->branch[pos],id);
		if(current->branch[pos]){
			if(current->branch[pos]->count<MIN){
				RestoreI(current,pos);

			}
		}
	}
}
TreenodeI* DeleteNodeI(TreenodeI* root,Key id){
	TreenodeI* oldroot;
	RecDeleteNodeI(root,id);
	if(root->count==0){
		oldroot=root;
		root=root->branch[0];
		free(oldroot);
	}
	return root;

}

void MoveRightG(TreenodeG* current,int pos){
	int c;
	TreenodeG* t;
	t=current->branch[pos];
	for(c=t->count;c>0;c--){
		t->entry[c+1]=t->entry[c];
		t->branch[c+1]=t->branch[c];
	}
	t->branch[1]=t->branch[0] ; // Move key from parent to rightnode;
	t->count++;
	t->entry[1]=current->entry[pos];
	t=current->branch[pos-1]; // MOve last key of left node int parent;
	current->entry[pos]=t->entry[t->count];
	current->branch[pos]=t->branch[t->count];
	t->count--;
}
void MoveLeftG(TreenodeG* current,int pos){
	int c;
	TreenodeG* t;
	t=current->branch[pos-1];  // Move key from parent to left node
	t->count++;
	t->entry[t->count]=current->entry[pos];
	t->branch[t->count]=current->branch[pos]->branch[0];
	t=current->branch[pos];
	current->entry[pos]=t->entry[1];
	t->branch[0]=t->branch[1];
	t->count--;
	for(c=1;c<=t->count;c++){
		// Shift all keys in right node one position leftward
		t->entry[c]=t->entry[c+1];
		t->branch[c]=t->branch[c+1];
	}

}
void combineG(TreenodeG* current,int pos){
	int c;
	TreenodeG* right,*left;
	right=current->branch[pos];
	left=current->branch[pos-1]; // Work with left node
	left->count++; // GNsert the key from the parent;
	left->entry[left->count]=current->entry[pos];
	left->branch[left->count]=right->branch[0];
	for(c=1;c<=right->count;c++){
		left->count++;
		left->entry[left->count]=right->entry[c];
		left->branch[left->count]=right->branch[c];
	}
	for(c=pos;c<current->count;c++){
		// Deleting key from parent node and shifting others 
		current->entry[c]=current->entry[c+1];
		current->branch[c]=current->branch[c+1];
	}
	current->count--;
	free(right);

}
void RestoreG(TreenodeG* current,int pos){
	if(pos==0){
		// Leftmost key
		if(current->branch[1]->count>MIN){
			MoveLeftG(current,1);
		}
		else{
			combineG(current,1);
		}
	}
	else if(pos==current->count){
		// rightmost key;
	    if(current->branch[pos-1]->count>MIN){
			MoveRightG(current,pos);

		}
		else{
			combineG(current,pos);
		}
	}
	else if(current->branch[pos-1]->count>MIN){
		MoveRightG(current,pos);
	}
	else if(current->branch[pos+1]->count>MIN)
	{
		MoveLeftG(current,pos+1);
	}
	else{
		combineG(current,pos);
	}
}
void RemoveG(TreenodeG* current,int pos){
	int i;
	for(i=pos+1;i<=current->count;i++){
		current->entry[i-1]=current->entry[i];
		current->branch[i-1]=current->branch[i];

	}
	current->count--;
}
void SuccessorG(TreenodeG* currrent,int pos){
	TreenodeG* leaf;
	for(leaf=currrent->branch[pos];leaf->branch[0];leaf=leaf->branch[0]);
	currrent->entry[pos]=leaf->entry[1];
	
}
void RecDeleteNodeG(TreenodeG* current,Key id){
	int pos;
	if(current==NULL){
		printf("Id not found!!\n");
		return ;
	}
	if(SearchNodeG(id,current,&pos)){
		// Target is founfd in the current nodeG
		if(current->branch[pos-1]){
			// When deletion not from leaf node so we take successor node of child to parent 
			SuccessorG(current,pos);
			RecDeleteNodeG(current->branch[pos],current->entry[pos].Id);
		}
		else{
			// When the entry is removed from leaf node
			RemoveG(current,pos);

		}
	}
	else{
		RecDeleteNodeG(current->branch[pos],id);
		if(current->branch[pos]){
			if(current->branch[pos]->count<MIN){
				RestoreG(current,pos);

			}
		}
	}
}
TreenodeG* DeleteNodeG(TreenodeG* root,Key id){
	TreenodeG* oldroot;
	RecDeleteNodeG(root,id);
	if(root->count==0){
		oldroot=root;
		root=root->branch[0];
		free(oldroot);
	}
	return root;

}
/*Group leaderboard tree Functions*/
typedef NodeS TreeentryS;




/*Sndividual Functions Prototypes*/
Boolean SearchNodeS(Key,TreenodeS*,int*);
Boolean PushdownS(TreeentryS newentry,TreenodeS *current,TreeentryS *medentry,TreenodeS **medright);
void PushInS(TreeentryS medentry,TreenodeS *medright,TreenodeS *current,int pos);
void SplitS(TreeentryS,TreenodeS *,TreenodeS *,int,TreeentryS *,TreenodeS **);




/*
SearchTreeS: traverse B-Tree looking for a target (group).
Assumption
->The Btree pointed to by root has been created.

What will it do?
->Sf the key target is present in the B-tree,
then return value points to the node containing target in position targetpos.
Otherwise, the return value is NULL, and targetpos is undefined.

Components:
->SearchTreeS recursively
->SearchNodeS

*/

TreenodeS *SearchTreeS(Key target,TreenodeS *rootS,int *targetpos)
{
	TreenodeS *ret_ptr;
	if(rootS==NULL)
	{
		//Empty tree
		return NULL;
	}
	else if(SearchNodeS(target,rootS,targetpos))
	{

		//The target is in the root itself
		return rootS;
	}
	else
	{
		//Search in the branch
		return SearchTreeS(target,rootS->branch[*targetpos],targetpos);
	}
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

Boolean SearchNodeS(Key target,TreenodeS *current,int *pos)
{
	Boolean bool;
	if(target>current->entry[1].steps)
	{
		/*Take the leftmost branch*/
		*pos=0;
		bool=FALSE;
	}
	else
	{
		/*Start a sequential search through keys from end*/

		for(*pos=current->count;(target>current->entry[*pos].steps)&&((*pos)>1);(*pos)--);

		if(target==current->entry[*pos].steps)
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

TreenodeS* InsertTreeS(TreeentryS newentry,TreenodeS *rootS)
{
	TreeentryS medentry; /*node to be reinserted as new root */
	TreenodeS *medright; /*subtree on right of medentry */
	TreenodeS *newroot; /*used when the height of the tree increases */
	if(PushdownS(newentry,rootS,&medentry,&medright))
	{
		/*Tree grows in height */

		newroot=(TreenodeS*)malloc(sizeof(TreenodeS)); /*Make a new root */
		newroot->count=1;
		newroot->entry[1]=medentry;
		newroot->branch[0]=rootS;
		newroot->branch[1]=medright;
		return newroot;
	}
	return rootS;
}

/* PushDownS:recursively belongs in the subtree to which current points*/
Boolean PushdownS(TreeentryS newentry,TreenodeS *current,TreeentryS *medentry,TreenodeS **medright)
{
	Boolean bool;
	int pos; /*branch on which to continue the search*/
	if(current==NULL)
	{
		/*cannot insert into empty tree;terminates*/
		*medentry = newentry;
		*medright=NULL;
		return TRUE;
	}
	else
	{
		/*Search the current node*/
		if(SearchNodeS(newentry.steps,current,&pos))
		{
			printf("Warning:Inserting duplicate key into B-tree");
		}
		else if(PushdownS(newentry,current->branch[pos],medentry,medright))
		{

			if(current->count<MAX)
			{
				/*Reinsert median key*/
				PushInS(*medentry,*medright,current,pos);
				return FALSE;
			}
			else
			{
				SplitS(*medentry,*medright,current,pos,medentry,medright);
				return TRUE;
			}
		}
		return FALSE;
	}
}

void PushInS(TreeentryS medentry,TreenodeS *medright,TreenodeS *current,int pos)
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


void SplitS(TreeentryS medentry,TreenodeS *medright,TreenodeS *current,int pos,TreeentryS *newmedian,TreenodeS **newright)
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

	*newright=(TreenodeS*)malloc(sizeof(TreenodeS));

	for(i=median+1;i<=MAX;i++)
	{
		(*newright)->entry[i-median]=current->entry[i];
		(*newright)->branch[i-median]=current->branch[i];
	}

	(*newright)->count=MAX-median;
	current->count=median;
	if(pos<=MIN)
	{
		PushInS(medentry,medright,current,pos);
	}
	else
	{
		PushInS(medentry,medright,*newright,pos-median);
	}
	*newmedian=current->entry[current->count];
	(*newright)->branch[0]=current->branch[current->count];
	current->count--;
}

void MoveRightS(TreenodeS* current,int pos){
	int c;
	TreenodeS* t;
	t=current->branch[pos];
	for(c=t->count;c>0;c--){
		t->entry[c+1]=t->entry[c];
		t->branch[c+1]=t->branch[c];
	}
	t->branch[1]=t->branch[0] ; // Move key from parent to rightnode;
	t->count++;
	t->entry[1]=current->entry[pos];
	t=current->branch[pos-1]; // MOve last key of left node int parent;
	current->entry[pos]=t->entry[t->count];
	current->branch[pos]=t->branch[t->count];
	t->count--;
}
void MoveLeftS(TreenodeS* current,int pos){
	int c;
	TreenodeS* t;
	t=current->branch[pos-1];  // Move key from parent to left node
	t->count++;
	t->entry[t->count]=current->entry[pos];
	t->branch[t->count]=current->branch[pos]->branch[0];
	t=current->branch[pos];
	current->entry[pos]=t->entry[1];
	t->branch[0]=t->branch[1];
	t->count--;
	for(c=1;c<=t->count;c++){
		// Shift all keys in right node one position leftward
		t->entry[c]=t->entry[c+1];
		t->branch[c]=t->branch[c+1];
	}

}
void combineS(TreenodeS* current,int pos){
	int c;
	TreenodeS* right,*left;
	right=current->branch[pos];
	left=current->branch[pos-1]; // Work with left node
	left->count++; // SNsert the key from the parent;
	left->entry[left->count]=current->entry[pos];
	left->branch[left->count]=right->branch[0];
	for(c=1;c<=right->count;c++){
		left->count++;
		left->entry[left->count]=right->entry[c];
		left->branch[left->count]=right->branch[c];
	}
	for(c=pos;c<current->count;c++){
		// Deleting key from parent node and shifting others 
		current->entry[c]=current->entry[c+1];
		current->branch[c]=current->branch[c+1];
	}
	current->count--;
	free(right);

}
void RestoreS(TreenodeS* current,int pos){
	if(pos==0){
		// Leftmost key
		if(current->branch[1]->count>MIN){
			MoveLeftS(current,1);
		}
		else{
			combineS(current,1);
		}
	}
	else if(pos==current->count){
		// rightmost key;
	    if(current->branch[pos-1]->count>MIN){
			MoveRightS(current,pos);

		}
		else{
			combineS(current,pos);
		}
	}
	else if(current->branch[pos-1]->count>MIN){
		MoveRightS(current,pos);
	}
	else if(current->branch[pos+1]->count>MIN)
	{
		MoveLeftS(current,pos+1);
	}
	else{
		combineS(current,pos);
	}
}
void RemoveS(TreenodeS* current,int pos){
	int i;
	for(i=pos+1;i<=current->count;i++){
		current->entry[i-1]=current->entry[i];
		current->branch[i-1]=current->branch[i];

	}
	current->count--;
}
void SuccessorS(TreenodeS* currrent,int pos){
    TreenodeS* leaf;
    for(leaf=currrent->branch[pos];leaf->branch[0];leaf=leaf->branch[0]);
    currrent->entry[pos]=leaf->entry[1];
    
}
void RecDeleteNodeS(TreenodeS* current,Key id){
    int pos;
    if(current==NULL){
        printf("Id not found!!\n");
        return ;
    }
    if(SearchNodeS(id,current,&pos)){
        // Target is founfd in the current nodeS
        if(current->branch[pos-1]){
            // When deletion not from leaf node so we take successor node of child to parent 
            SuccessorS(current,pos);
            RecDeleteNodeS(current->branch[pos],current->entry[pos].Id);
        }
        else{
            // When the entry is removed from leaf node
            RemoveS(current,pos);

        }
    }
    else{
        RecDeleteNodeS(current->branch[pos],id);
        if(current->branch[pos]){
            if(current->branch[pos]->count<MIN){
                RestoreS(current,pos);

            }
        }
    }
}
TreenodeS* DeleteNodeS(TreenodeS* root,Key id){
    TreenodeS* oldroot;
    RecDeleteNodeS(root,id);
    if(root->count==0){
        oldroot=root;
        root=root->branch[0];
        free(oldroot);
    }
    return root;

}
TreenodeS* Create_Sroup(NodeS *nptr,TreenodeS *root,status_code* sc)
{
	root=InsertTreeS(*nptr,root);
	return root;
}

TreenodeG* Create_Group(NodeG *nptr,TreenodeG *root,status_code* sc,TreenodeS**rootS)
{
	*sc=SUCCESS;
	if(nptr==NULL)
		*sc=FAILURE;
	else
	{
		root=InsertTreeG(*nptr,root);
		NodeS *nptrS=CreateNodeS();
		nptrS->Id=nptr->Id;
		strcpy(nptrS->Name,nptr->Name);
		nptrS->steps=Compute_Number_Of_Steps_by_a_Group(nptr);
		*rootS=Create_Sroup(nptrS,*rootS,sc);
	}

	return root;
}
NodeI* Search_for_Pointer_to_Individual(unsigned int Individual_Id,TreenodeI *rootI)
{
	int targetpos,pos;
	TreenodeI *current;
	NodeI *loc;
	current=SearchTreeI(Individual_Id,rootI,&targetpos);
	if(current!=NULL)
		if(SearchNodeI(Individual_Id,current,&pos))
		{
			//printf("\n%d",pos);
			loc=&(current->entry[pos]);
		}
		else
		{
			printf("The Individual Id does not exists");
			loc=NULL;
		}
	return loc;
}

NodeG* Search_for_Pointer_to_Group(unsigned int Group_Id,TreenodeG *rootG)
{
	int targetpos,pos;
	TreenodeG *current;
	NodeG *loc;
	current=SearchTreeG(Group_Id,rootG,&targetpos);
	if(current!=NULL)
		if(SearchNodeG(Group_Id,current,&pos))
		{
			loc=&(current->entry[pos]);
		}
		else
		{
			printf("The Individual Id does not exists");
			loc=NULL;
		}
	return loc;
}

status_code Store_Member_Pointers(NodeG *nptr, unsigned int Member_Id[],TreenodeI *rootI)
{
    status_code sc = SUCCESS;

    for (int i = 0; i < 5; i++)
    {
        if ((Member_Id[i] != 0))
        {
            nptr->Members[i] = Search_for_Pointer_to_Individual(Member_Id[i],rootI);
            if (nptr->Members[i]->belong == 1)
            {
                nptr->Members[i] = NULL;
                break;
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


void traversalI(TreenodeI *myNode) {
  int i;
  if (myNode) {
    for (i = 0; i < myNode->count; i++) {
      traversalI(myNode->branch[i]);
      printf("%d ", myNode->entry[i + 1].Id);
    }
    traversalI(myNode->branch[i]);
  }
}
int traversalS(TreenodeS *myNode,int rank) {
  int i;
  if (myNode) {
    for (i = 0; i < myNode->count; i++) {
      rank=traversalS(myNode->branch[i],rank);
      printf("\n----------------------------------------------------");
      printf("\nRank = %d",rank);
      printf("\nName %s ", myNode->entry[i + 1].Name);
      printf("\nNum of Steps = %u\n",myNode->entry[i+1].steps);
      rank++;
      printf("----------------------------------------------------\n");
    }
    rank=traversalS(myNode->branch[i],rank);
  }
  return rank;
}
TreenodeS* Get_Leader_Board_tree(TreenodeG *root,TreenodeS *rootS)
{
  	int i;
  	NodeS nodeS;
  	if (root) {
    	for (i = 0; i < root->count; i++) {
      		rootS=Get_Leader_Board_tree(root->branch[i],rootS);
     	 	strcpy(nodeS.Name,root->entry[i + 1].Name);
     	 	//printf("\nName %s",nodeS.Name);
      		nodeS.Id=root->entry[i + 1].Id;
      		//printf("\nId %u",nodeS.Id);
      		nodeS.steps=Compute_Number_Of_Steps_by_a_Group(&root->entry[i+1]);
      		rootS=InsertTreeS(nodeS,rootS);
    	}
    	rootS=Get_Leader_Board_tree(root->branch[i],rootS);
  	}
  return rootS;
}

void traversalG(TreenodeG *myNode,int lower,int upper) {
  int i;
  if (myNode) {
    for (i = 0; i < myNode->count; i++) {
      if((myNode->branch[i]!=NULL)&&((myNode->branch[i]->entry[1].Id<=upper)))
      	  traversalG(myNode->branch[i],lower,upper);
      if((myNode->entry[i + 1].Id>=lower)&&(myNode->entry[i + 1].Id<=upper))
      	 { 
      	 	printf("\n----------------------------------------------------");
      	 	printf("\nId = %d ", myNode->entry[i + 1].Id);
      	 	printf("\nName : %s",myNode->entry[i+1].Name);
      	 	printf("\nWeekly_Group_Goal = %u",myNode->entry[i+1].Weekly_Group_Goal);
      	 	printf("\nMembers: \n");
      	 	for (int j = 0; j < 5; ++j)
      	 	{
      	 		if(myNode->entry[i+1].Members[j]!=NULL)
      	 		{
      	 			printf("\tName : %s\n",myNode->entry[i+1].Members[j]->Name);
      	 		}
      	 		else
      	 		{
      	 			break;
      	 		}
      	 	}
      	 	printf("----------------------------------------------------\n");
      	 }

    }
    if((myNode->branch[i]!=NULL)&&((myNode->branch[i]->entry[1].Id<=upper)))
    	traversalG(myNode->branch[i],lower,upper);
  }
}


NodeG* CreateNodeG()
{
	NodeG *nptr;
	nptr=(NodeG*)malloc(sizeof(NodeG));
	nptr->Id=0;
	strcpy(nptr->Name,"unnamed");
	for(int i=0;i<5;i++)
	{
		nptr->Members[i]=NULL;
	}
	nptr->Weekly_Group_Goal=0;

	return nptr;
}

NodeS* CreateNodeS()
{
	NodeS *nptr;
	nptr=(NodeS*)malloc(sizeof(NodeS));
	nptr->Id=0;
	strcpy(nptr->Name,"unnamed");
	nptr->steps=0;

	return nptr;
}

TreenodeG* Merge_Groups(unsigned int Group_Id_1, unsigned int Group_Id_2,TreenodeG *rootG,TreenodeS **rootS)
{

	int pos;
    status_code sc = SUCCESS;
    NodeG *group1, *group2;
    group1 = group2 = NULL;

    //Search the Groups

    group1=Search_for_Pointer_to_Group(Group_Id_1,rootG);
    group2=Search_for_Pointer_to_Group(Group_Id_2,rootG);

    if (group1 == NULL || group2 == NULL)
    {
        printf("Enter the correct IDs \n");
        sc = FAILURE;
    }
    else
    {
        printf("Both the groups exist \n");
        NodeG *newgroup;
        newgroup=CreateNodeG();

        int i = 0;
        while (i < 5 && group1->Members[i] != NULL)
        {
            newgroup->Members[i] = group1->Members[i];
            printf("\nName %s",newgroup->Members[i]->Name);
            i++;
        }
        int p = 0;
        while (i < 5 && group2->Members[p] != NULL)
        {
            newgroup->Members[i] = group2->Members[p];
            printf("\nName %s",newgroup->Members[i]->Name);
            i++;
            p++;
        }
        if (i >=5 && group2->Members[p] != NULL )
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
            status_code sc;
            rootG=Delete_Group(Group_Id_1,rootG,rootS);
            rootG=Delete_Group(Group_Id_2,rootG,rootS);
            rootG=Create_Group(newgroup,rootG,&sc,rootS);
        }
    }
    return rootG;
}


TreenodeS* Delete_Sroup(unsigned int steps,TreenodeS *rootS)
{
	return(DeleteNodeS(rootS,steps));
}

TreenodeG* Delete_Group(unsigned int id,TreenodeG *rootG,TreenodeS **rootS)
{
	*rootS=Delete_Sroup(Compute_Number_Of_Steps_by_a_Group(Search_for_Pointer_to_Group(id,rootG)),*rootS);
	printf("\nDeletion of the Group is Successful!! \n");
	return(DeleteNodeG(rootG,id));
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

void Generate_Leader_Board(TreenodeG *rootG,TreenodeS *rootS)
{
	int count=0;
	//TreenodeS *root=Get_Leader_Board_tree(rootG,rootS);
	
	printf("\nDisplaying the leader board for Groups\n");
	printf("\n----------------------------------------------------\n");
	count=traversalS(rootS,1);

	printf("\nTotal number of Groups is %d",count-1);
	printf("\n----------------------------------------------------\n");
}

Boolean Check_Group_Achievement(unsigned int Group_Id,TreenodeG *rootG)
{
    Boolean bool = TRUE;
    unsigned int count = 0;
    NodeG *ptr;

    //Get the Pointer to the Group
    ptr = Search_for_Pointer_to_Group(Group_Id,rootG);

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

void Display_group_range_info(TreenodeG *rootG,int lower,int upper)
{
	printf("\n----------------------------------------------------\n");
	printf("\nDisplaying the info of groups within the range of Ids %d to %d\n",lower,upper);
	printf("\n----------------------------------------------------\n");
	traversalG(rootG,lower,upper);
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

    // printf("\n.........................................................\n");
    return sc;
}

void display_individualDatacompletely(TreenodeI* myNode){
    int i;
    if (myNode)
    {
        for (i = 0; i < myNode->count; i++)
        {
            display_individualDatacompletely(myNode->branch[i]);
            //printf("%d ", myNode->entry[i + 1].Id);
            Display_Member_Info(&(myNode->entry[i+1]));
        }
        display_individualDatacompletely(myNode->branch[i]);
    }
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

void gettop3list(TreenodeI *rootI)
{
    int i;
    if (rootI)
    {
        for (i = 0; i < rootI->count; i++)
        {
            gettop3list(rootI->branch[i]);
            int steps = 0;
            Boolean check = target_complete(&(rootI->entry[i+1]),&steps);
            if (check == TRUE)
            {
                // Target is completed;

                // The member has completed therir daily step goals

                if (position[0] == NULL)
                {
                    position[0] = &(rootI->entry[i+1]);

                    first = steps;
                }
                else if (position[1] == NULL)
                {
                    if (steps > first)
                    {
                        // The current one has more number of steps
                        position[1] = position[0];
                        position[0] = &(rootI->entry[i+1]);
                        second = first;
                        first = steps;
                    }
                    else
                    {
                        position[1] = &(rootI->entry[i+1]);
                        second = steps;
                    }
                }
                else if (position[2] == NULL)
                {
                    if (steps > first)
                    {
                        position[2] = position[1];
                        position[1] = position[0];
                        position[0] = &(rootI->entry[i+1]);
                        third = second;
                        second = first;
                        first = steps;
                    }
                    else if (steps > second)
                    {
                        position[2] = position[1];
                        position[1] = &(rootI->entry[i+1]);
                        third = second;
                        second = steps;
                    }
                    else
                    {
                        position[2] = &(rootI->entry[i+1]);
                        third = steps;
                    }
                }
                else
                {
                    if (steps > first)
                    {
                        position[2] = position[1];
                        position[1] = position[0];
                        position[0] = &(rootI->entry[i+1]);
                        third = second;
                        second = first;
                        first = steps;
                    }
                    else if (steps > second)
                    {
                        position[2] = position[1];
                        position[1] = &(rootI->entry[i+1]);
                        third = second;
                        second = steps;
                    }
                    else if (steps > third)
                    {
                        position[2] = &(rootI->entry[i+1]);
                        third = steps;
                    }
                    // If it don't eneter into any of the condition then this means that the member_steps < third;
                }
            }
        }
        gettop3list(rootI->branch[i]);
    }
}
status_code get_Top3(TreenodeI* rootI){
    printf("\n");
    gettop3list(rootI);
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
status_code check_individual_Rewards(unsigned int Member_Id,TreenodeI* rootI)
{
    int prize[3] = {100, 75, 50};
    status_code sc = SUCCESS;
    NodeI *ptr = Search_for_Pointer_to_Individual(Member_Id,rootI);
    if (ptr == NULL)
    {
        sc = FAILURE;
    }
    else
    {
        gettop3list(rootI);
        for (int i = 0; i < 3; i++)
        {
            if (position[i] != NULL)
                position[i]->reward += prize[i];
        }
        printf("Reward: %d\n", ptr->reward);
    }
    return sc;
}
status_code suggest_goalUpdates(unsigned int id,TreenodeI* rootI)
{
    status_code sc = SUCCESS;
    
    NodeI *ptr = Search_for_Pointer_to_Individual(id,rootI);
    if (ptr == NULL)
    {
        printf("Enter the valid id input.\n");
    }
    else
    {
        gettop3list(rootI);
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
	TreenodeG *rootG=NULL;
	TreenodeI *rootI=NULL;
	TreenodeS *rootS=NULL;
    NodeI nodeI;
    NodeG nodeG;
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
            fscanf(ptr, "%u", &nodeI.Id);
            nodeI.reward = 0;
            nodeI.belong=0;
            //printf("\n%u",nodeI.Id);
            fscanf(ptr, "%s", nodeI.Name);
            //printf("\n%s",nodeI.Name);
            fscanf(ptr, "%u", &nodeI.Age);
            //printf("\n%u",nodeI.Age);
            fscanf(ptr, "%u", &nodeI.Daily_Step_Goal);

            //printf("\n%u",nodeI.Daily_Step_Goal);
            for (int i = 0; i < 7; i++)
            {
                fscanf(ptr, "%u", &nodeI.Weekly_Step_Count[i]);
                //printf("%u ",nodeI.Weekly_Step_Count[i]);
            }
            rootI=InsertTreeI(nodeI,rootI);
            nodeI.Id=0;
        }


        //Input the Group Details
        for (int i = 0; i < 5; i++)
        {
            fscanf(ptr, "%u", &nodeG.Id);
            //printf("\n%u",nodeG.Id);
            fscanf(ptr, "%s", nodeG.Name);
            //printf("\n%s",nodeG.Name);

            for (int j = 0; j < 5; j++)
            {
                fscanf(ptr, "%u", &Member_Id[j]);
                //printf("\n%u",Member_Id[j]);
                //Search_for_Pointer_to_Individual(Member_Id[j],rootI);
            }
            Store_Member_Pointers(&nodeG,Member_Id,rootI);

            fscanf(ptr, "%u", &nodeG.Weekly_Group_Goal);
            //printf("\n%u",nodeG.Weekly_Group_Goal);
            status_code sc;
            rootG=Create_Group(&nodeG,rootG,&sc,&rootS);
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
                nodeI.belong = 0;
                nodeI.reward = 0;
                printf("Enter the Id: ");
                scanf("%u", &nodeI.Id);
                printf("Enter the Name: ");
                scanf("%s", nodeI.Name);
                printf("Enter the Age: ");
                scanf("%u", &nodeI.Age);
                printf("Enter daily step goals: ");
                scanf("%u", &nodeI.Daily_Step_Goal);
                for (int i = 0; i < 7; i++)
                {
                    nodeI.Weekly_Step_Count[i] = 0;
                }
                rootI = InsertTreeI(nodeI, rootI);
                // traversal(rootI);
            }
            else if (value == 2)
            {
                NodeG nptr;

                printf("Enter the Id: ");
                unsigned int id;
                scanf("%u", &id);
                nptr.Id = id;
                status_code sc = SUCCESS;
                
                if (sc == SUCCESS)
                {
                    printf("Enter the Name: ");
                    fflush(stdin);
                    scanf("%s", nptr.Name);
                    printf("Enter the weekly group goals :");
                    scanf("%u", &nptr.Weekly_Group_Goal);
                    unsigned int memberid;
                    int i = 0;
                    int complete = 1;

                    while (i < 5 && complete == 1)
                    {

                        printf("Enter the id of individual : ");
                        scanf("%u", &memberid);
                        NodeI *temp = Search_for_Pointer_to_Individual(memberid, rootI);
                        if (temp != NULL)
                        {
                            if (temp->belong == 1)
                            {
                                printf("The Member:  %s id: %u already belongs to some group .\n", temp->Name, temp->Id);
                            }
                            else
                            {
                                temp->belong = 1;
                                nptr.Members[i] = temp;
                                i++;
                            }
                        }
                        printf("Enter 1 to add more member else 0 if completed\n");
                        scanf("%d", &complete);
                    }
                    while (i < 5)
                    {
                        nptr.Members[i] = NULL;
                        i++;
                    }
                    //printf("\n%u %u\n",Compute_Number_Of_Steps_by_a_Group(&nptr),nptr.Weekly_Group_Goal);
                    rootG = Create_Group(&nptr, rootG, &sc,&rootS);
                    //Display_group_range_info(rootG,1,5);
                }
                else
                {
                    printf("Already exist \n");
                }
            }
            else if (value==3)
            {
                /* code */
                status_code sc=get_Top3(rootI);
            }
            else if (value == 4)
            {
                printf("\nEnter the Details of the Group whose achievement is to be checked\n");
               
                // Only group 4 hasn't complete its weekly step goal, rest all have done it
                printf("Enter the group_id to check its Achievement : ");
                unsigned id;
                scanf("%u", &id);

                Boolean check = Check_Group_Achievement(id,rootG);
                printf("\n");
            }
            else if (value == 5)
            {
                printf("\nGroups Leaderboard based on Number of Steps completed\n");
                // Generate Leader board
                Generate_Leader_Board(rootG,rootS);
            }
            else if(value==6){
                 printf("Enter the id to check its reward: ");
                unsigned int id;
                scanf("%u", &id);
                sc = check_individual_Rewards(id,rootI);
                if (sc == FAILURE)
                {
                    printf("Enter the valid ID input \n");
                }
             
            }
              else if (value == 7)
            {
                printf("Enter the id to be deleted:  ");
                unsigned int id;
                scanf("%u", &id);
                rootI=DeleteNodeI(rootI,id);
                //traversal(rootI);
                // if (sc == SUCCESS)
                display_individualDatacompletely(rootI);
            }
            else if (value == 8)
            {
                printf("Enter the Group id to be deleted:  ");
                unsigned int id;
                scanf("%u", &id);
                
                rootG=Delete_Group(id,rootG,&rootS);
            }
            else if (value == 9)
            {
                unsigned int id1, id2;
                printf("Enter the two Group IDs to be merged : ");
                scanf("%u%u", &id1, &id2);

                rootG=Merge_Groups(id1, id2,rootG,&rootS);
                Display_group_range_info(rootG,1,5);
            }

            else if (value == 10)
            {
                unsigned int lower=0,upper=0;

                printf("\nEnter the Group id range whose info is to be dispalyed:  ");
                printf("\nLower bound = ");
                scanf("%u",&lower);
                printf("\nUpper = ");
                scanf("%u",&upper);
                if(lower>upper)
                	printf("\nPlease enter valid range");
                else
                {
                	Display_group_range_info(rootG,lower,upper);
                }
            }
            else if (value == 11)
            {
                printf("Enter the id for which suggession is required: ");
                unsigned int id;
                scanf("%u", &id);
                sc = suggest_goalUpdates(id,rootI);
            }
            else
            {
                printf("Enter the valid input !!\n");
            }
        }
    }

}
