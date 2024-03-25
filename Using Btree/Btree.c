#include<stdio.h>
#include<stdlib.h>

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

/*Group Functions Prototypes*/
Boolean SearchNodeG(Key,TreenodeG*,int*);
Boolean PushdownG(TreeentryG newentry,TreenodeG *current,TreeentryG *medentry,TreenodeG **medright);
void PushInG(TreeentryG medentry,TreenodeG *medright,TreenodeG *current,int pos);
void SplitG(TreeentryG,TreenodeG *,TreenodeG *,int,TreeentryG *,TreenodeG **);





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
			printf("Warning:Gnserting duplicate key into B-tree");
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
	current->branch[pos]->branch[0]=t->branch[t->count];
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
	for(c=1;c<=t->count;c--){
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
	for(leaf=currrent->branch[pos];leaf->branch[0];leaf=leaf->branch[0]){
		currrent->entry[pos]=leaf->entry[1];
	}
}
void RecDeleteNodeG(TreenodeG* current,Key id){
	int pos;
	if(current==NULL){
		printf("Id not found!!\n");
		return ;
	}
	if(SearchNodeG(id,current,&pos)){
		// Target is found in the current nodeG
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

TreenodeG* Create_Group(NodeG *nptr,TreenodeG *root)
{
	root=InsertTreeG(*nptr,root);
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

void printTreeI(TreenodeI *rootI,int i)
{
	if(rootI==NULL)
	{
		
	}
	else
	{
		//printf("\ncount=%d",rootI->count);
		for(int j=i;j<=rootI->count;j++)
		{
			//printf("\nlevel");
			printTreeI(rootI->branch[j-1],1);
			printf(" %u ",rootI->entry[j].Id);
			printTreeI(rootI->branch[j],1);
		}
	}
}

void printTreeG(TreenodeG *rootG,int i)
{
	if(rootG==NULL)
	{
		//printf(" %u ",rootG->entry[i].Id);
	}
	else
	{
		for(int j=i;j<=rootG->count;j++)
		{
			//printf("\n");
			printTreeG(rootG->branch[j-1],1);
			printf(" %u ",rootG->entry[j].Id);
			printTreeG(rootG->branch[j],1);
		}
	}
}

void traversal(TreenodeI *myNode) {
  int i;
  if (myNode) {
    for (i = 0; i < myNode->count; i++) {
      traversal(myNode->branch[i]);
      printf("%d ", myNode->entry[i + 1].Id);
    }
    traversal(myNode->branch[i]);
  }
}

void main()
{
	TreenodeG *rootG=NULL;
	TreenodeI *rootI=NULL;
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
            printf("\n%u",nodeI.Id);
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
            rootG=InsertTreeG(nodeG,rootG);
        }

        fclose(ptr);
   
   	    printf("\nTree before deletion\n");
   	    traversal(rootI);
        //printTreeI(rootI,1);
        //printTreeG(rootG,1);
        DeleteNodeI(rootI,120);
        printf("\nTree after deletion\n");
        //printTreeI(rootI,1);
        traversal(rootI);
        //printf("\n%u",Search_for_Pointer_to_Individual(115,rootI)->Id);
    }

}
