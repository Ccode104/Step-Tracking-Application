#define CAPACITY 5
#define ORDER 4

#include<stdlib.h>
#include<stdio.h>

typedef enum{NO,YES} belongs;

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

typedef struct Data_NodeG_tag
{
	NodeG val[CAPACITY];
	int size;
} Data_NodeG;

typedef struct Bptree_NodeG_tag
{
	unsigned int key[ORDER];
	union u_tag
	{
		struct Bptree_NodeG_tag *child[ORDER+1];
		Data_NodeG *data_ptr[ORDER+1];
	} u;
	char child_type;//k or d
} Bptree_NodeG;

Bptree_NodeG *root=NULL;

Data_NodeG* Create_Data_NodeG()
{
	//Dynamically allocate the data node
	Data_NodeG *dptr=(Data_NodeG*)malloc(sizeof(Data_NodeG));

	//Initialise the ids to -1
	for(int i=0;i<CAPACITY;i++)
	{
		dptr->val[i].Id=-1;
	}

	return dptr;
}
Bptree_NodeG* Create_Bptree_NodeG()
{
	//Dynamically allocate the tree node
	Bptree_NodeG *nptr=(Bptree_NodeG*)malloc(sizeof(Bptree_NodeG));

	//Initialise the data node child to NULL(assuming them to be leaf tree type nodes by default)
	for(int i=0;i<ORDER;i++)
	{
		nptr->u.data_ptr[i]=NULL;
		nptr->key[i]=-1;
	}

	return nptr;
}
Bptree_NodeG* Search_for_id(unsigned int id,Bptree_NodeG *root)
{
	int i=0;
	while(root->key[i]!=-1)
	{
		if(id>root->key[i])
		{
			//Continue
		}
		else
		{
			if(root->child_type='k')
				root=Search_for_id(id,root->u.child[i+1]);
		}
	}
	return root;
}
Bptree_NodeG* InsertNodeG(Bptree_NodeG *root,NodeG *nptr)
{
	if(root==NULL)
	{
		//Empty tree
		root=Create_Bptree_NodeG();

		//Store the id as the key
		root->key[0]=nptr->Id;

		//Fill in the details of the group at the first palce in the data node
		Data_NodeG *dptr=Create_Data_NodeG();
		dptr->val[0]=*nptr;

		//Set the data node as the child
		root->u.data_ptr[1]=dptr;
	}
	else{
		//The tree has 1 or more nodes

		//Get the tree node containing the key to be searched
		Bptree_NodeG *ptr=Search_for_id(nptr->Id,root);

		//Find where to insert the key in that tree node 
		int i=0;
		while((ptr->key[i]!=-1)&&(nptr->Id>ptr->key[i]))
		{
			i++;
		}
		if(i==ORDER)
		{
			//The tree node is full
			//Split the tree node
		}
		else
		{
			//The tree node is not full
			int insert_at_index=i;

			//Shift the keys to make place for insertion of key
			for(int j=ORDER-1;j>insert_at_index;j--)
			{
				ptr->key[j]=ptr->key[j-1];
			}
			ptr->key[insert_at_index]=nptr->Id;

			//Shift the data_ptr's 
			for(int i=ORDER;i>insert_at_index+1;i--)
			{
				ptr->key[i]=ptr->key[i-1];
			}

			//Create a new data node in case of need
			Data_NodeG *dptr=ptr->u.data_ptr[insert_at_index];
			Data_NodeG *dptr_next=Create_Data_NodeG();

			//Traverse through the data node group Ids
			int i=0;
			while((dptr->val[i].Id!=-1)&&(dptr->val[i].Id<nptr->Id))
			{
				i++;
			}

			if(dptr->val[i].Id==nptr->Id)
			{
				printf("\nDuplicate Group Id \n");
			}
			else if(i==ORDER+1)
			{
				//The data node is full
				//Split and shift upwards
				
			}
			else
			{
				//Shift the groups to dptr_next data node
				int j=0;
				while(i<CAPACITY)
				{
					dptr_next->val[j]=dptr->val[i];
					i++;
					j++;
				}
			}
		}
	}
}


