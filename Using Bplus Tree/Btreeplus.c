#define CAPACITY 5
#define ORDER 5

#include<stdlib.h>
#include<stdio.h>

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

typedef struct Data_NodeG_tag
{
    NodeG val[CAPACITY];
    int size;
} Data_NodeG;

typedef struct Bptree_NodeG_tag
{
    unsigned int key[ORDER-1];
    unsigned int num_of_keys; 
    union u_tag
    {
        struct Bptree_NodeG_tag *child[ORDER];
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

    //leaf node
    nptr->child_type='d';

    //0 keys
    nptr->num_of_keys=0;

    return nptr;
}
/*
Bptree_NodeG* Search_for_id(unsigned int id,Bptree_NodeG *root)
{
    int i=0;
    while((root->key[i]!=-1)&&(i<ORDER))
    {
        if(id>root->key[i])
        {
            //Continue
        }
        else if(id>root->key[i])
        {
            if(root->child_type='k')
            {
                if(root->u.child[i]!=NULL)
                {
                        root=Search_for_id(id,root->u.child[i]);
                        break;
                }
                else{
                    break;
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            if(root->child_type='k')
            {
                if(root->u.child[i]!=NULL)
                {
                        root=Search_for_id(id,root->u.child[i+1]);
                        break;
                }
                else{
                    break;
                }
            }
            else
            {
                break;	
            }
        }
        i++;
    }
    if(i==ORDER)
    {	if(root->child_type='k')
        {
            if(root->u.child[i+1]!=NULL)
            {
                root=Search_for_id(id,root->u.child[i+1]);
            }
            else
            {
                
            }
        }
        else
        {
            
        }	
    }
    return root;
}
Bptree_NodeG* InsertNodeG(Bptree_NodeG *root,NodeG *nptr)
{
    if(root==NULL)
    {
        printf("Empty Tree");
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
            printf("Full Tree Node");
            //The tree node is full
            //Split the tree node
        }
        else
        {
            printf("i = %d ",i);
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

            Data_NodeG *dptr;
            //Create a new data node in case of need
            if(ptr->u.data_ptr[insert_at_index]!=NULL)
            {
                dptr=ptr->u.data_ptr[insert_at_index];
            }
            else
            {
                dptr=Create_Data_NodeG();
                ptr->u.data_ptr[insert_at_index]=dptr;
            }
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
*/
int find_key_insert_index(Bptree_NodeG *ptr,unsigned int Id)
{
    int i=0;
    while((i<ORDER-1)&&(ptr->key[i]<Id)&&(ptr->key[i]!=-1))
    {
        i++;
    }
    if(ptr->key[i]==Id)
    {
    	i=-1;
    }
    
    return i;
}
Bptree_NodeG* Search_for_BptreeNode(Bptree_NodeG*ptr,unsigned int Id,Bptree_NodeG **parent,int *k
{
    int i=0;
    *parent=NULL;
    if(ptr->child_type=='d')
    {
        //leaf node

    }
    else
    {
        //Internal Node
        *parent=ptr;
        i=find_key_insert_index(ptr,Id);
        *k=i;
        ptr=Search_for_BptreeNode(ptr,Id,parent);
    }
    return ptr;
}

Boolean Insert_data(Data_NodeG *dptr,NodeG *gptr)
{
    Boolean bool=FALSE;
    int i=0;
    while((gptr->Id>dptr->val[i].Id)&&(i<CAPACITY)&&(dptr->val[i].Id!=-1))
    {
        i++;
    }
    if(dptr->val[i].Id==-1)
    {
        //Empty Slot
        //Insert At end

        dptr->val[i]=*gptr;
    }
    else if(i==CAPACITY)
    {
        //Data Node FULL
        bool=TRUE;
    }
    else
    {
        //Shift the data nodes downwards

        for(int j=CAPACITY-1;j>i;j--)
        {
            dptr->val[j]=dptr->val[j-1];
        }
        dptr->val[i]=*gptr;
    }
    return bool;
}

Boolean full_of_keys(Bptree_NodeG *ptr)
{
    //Check if full of Keys
    Boolean bool=FALSE;
    int i=0;
    while((i<ORDER-1)&&(ptr->key[i]!=-1))
    {
        i++;
    }

    if(i==ORDER-1)
    {
        bool=TRUE;
    }
    return bool;
}
void split_tree_node(Bptree_NodeG **curr_ptr,Bptree_NodeG *parent,int k)
{
	Bptree_NodeG *curr=*curr_ptr;
	if(curr->child_type=='d')
	{
		//leaf node split

		if(parent!=NULL)&&(parent->key[k]>curr->u.data_ptr[2])
		{
			//key to be inserted on right(i+1)
			if(k==0)
			{
				Insert_at_start(curr,curr->u.data_ptr[])
			}
			//shift
			//create new data node at (i+1)
		}
	}
}
void Insert_Bplustree(NodeG *gptr)
{
    int i=0;
    Bptree_NodeG *ptr=root;

    //Create the Bplustree Node
    Bptree_NodeG *nptr=Create_Bptree_NodeG();

    if(root==NULL)
    {
        printf("\nEmpty tree node Insert");
        //Tree is Empty
        ptr=nptr;

        //nptr is leaf node

        //gptr is the pointer to the group
        nptr->key[0]=gptr->Id;

        //Create Data Node
        Data_NodeG *dptr=Create_Data_NodeG();

        //Data Node is now empty
        //Insert the group

        dptr->val[0]=*gptr;

        //Attach the data node to tree leaf node
        nptr->u.data_ptr[1]=dptr;

        root=ptr;

    }
    else
    {
        Bptree_NodeG *parent;
        int k=0;
        ptr=Search_for_BptreeNode(root,gptr->Id,&parent,&k);

        //Check if full of Keys
        if(full_of_keys(ptr))
        {
            //The Node is full of keys
            //So insert the group in data node
            //No need to insert the key

            Boolean full=FALSE;
            i=find_key_insert_index(ptr,gptr->Id);
            if(i==ORDER-1)
            {
                full=Insert_data(ptr->u.data_ptr[i],gptr);
            }
            else
            {	
                Boolean full;
                full=Insert_data(ptr->u.data_ptr[i],gptr);

                if(full==TRUE)
                {
                    //The data node is full
                    //Split the data node and push the median up as key
                    //The tree node is full of keys
                    //Split the tree node and push the median(got from data node) 
                    //up again as a key in new tree node
                    
                    split_tree_node(ptr,*k);
                    
                }
            }
        }
        else
        {
            //The Node has space to accomodate one more key

            printf("\nAt start=%d To be Inserted=%d",ptr->key[i],gptr->Id);
            i=find_key_insert_index(ptr,gptr->Id);
            if(i==-1)
            {
                printf("\nDuplicate Group!!\n");
            }
            else
            {
                if(i!=0)
                {
                    Insert_Between_at_index(ptr,gptr,i);
                }
                else
                {
                	Insert_at_start(ptr,gptr);
                }
                    

            }

        }
    }
}
void Insert_at_start(Bptree_NodeG *ptr,NodeG *gptr)
{
	int i=0;
    printf("\nInsert At Start");
    //Insert At Start

    //Shift the keys to right 

    for(int j=ORDER-2;j>i;j--)
    {
    ptr->key[j]=ptr->key[j-1];
                    }

                    //Shift the data ptrs
                    
                    for(int j=ORDER-1;j>i;j--)
                    {
                        ptr->u.data_ptr[j]=ptr->u.data_ptr[j-1];
                    }
                    ptr->u.data_ptr[0]=NULL;

                    //Insert the key (group Id)
                    ptr->key[0]=gptr->Id;

                    //Create and Attach a data node at 0th pos in data_ptrs

                    Data_NodeG *dptr=Create_Data_NodeG();
                    ptr->u.data_ptr[0]=dptr;

                    Boolean full;
                    if(ptr->u.data_ptr[1]!=NULL)
                    {
                        //Insert the group Id in next data node
                        full=Insert_data(ptr->u.data_ptr[1],gptr);
                    }
                    else
                    {
                        ptr->u.data_ptr[1]=Create_Data_NodeG();
                        Insert_data(ptr->u.data_ptr[1],gptr);
                    }

                    //Copy all the groups in the next data node with Id less than
                    //current Id in the 0th data node

                    dptr=ptr->u.data_ptr[1];

                    int j=0;
                    while((j<CAPACITY)&&(dptr->val[j].Id!=-1))
                    {
                        if(dptr->val[j].Id<gptr->Id)
                        {
                            Insert_data(ptr->u.data_ptr[0],&(dptr->val[j]));
                            dptr->val[j].Id=-1;
                        }
                        j++;
                    }
                
}
void Insert_Between_at_index(Bptree_NodeG *ptr,NodeG *gptr,int i)
{
    printf("\nInsert Between");
    //Shift the keys to right 

    for(int j=ORDER-1;j>i;j--)
    {
        ptr->key[j]=ptr->key[j-1];
    } 	

    ptr->key[i]=gptr->Id;

    //Shift the data ptrs

    for(int j=ORDER-1;j>i+1;j--)
    {
        ptr->u.data_ptr[j]=ptr->u.data_ptr[j-1];
    }
    //No data node being pointed 
    ptr->u.data_ptr[i+1]=NULL;

    //Create and Attach new data node

    Data_NodeG *dptr=Create_Data_NodeG();
    ptr->u.data_ptr[i+1]=dptr;

    //Insert the group
    ptr->u.data_ptr[i+1]->val[0]=*gptr;

    dptr=ptr->u.data_ptr[i];

    //Copy the values from previous data node(if any) greater than current group id
    //to new data node 

    Boolean full=FALSE;
    int j=0;
    while((j<CAPACITY)&&(dptr->val[j].Id!=-1)&&(full==FALSE))
    {
        if(dptr->val[j].Id>=gptr->Id)
        {
            full=Insert_data(ptr->u.data_ptr[i+1],&(dptr->val[j]));
            dptr->val[j].Id=-1;
        }
        j++;
    }

    if(full==TRUE)
    {
        //The new data node has got full
        //Split the new data node
        //Push the median up as a key
        //Insert the key(median val from data node)
                        
        NodeG tgrp;	
        for(int k=3;k<CAPACITY;k++)
        {
        tgrp=ptr->u.data_ptr[i+1]->val[j];
        ptr->u.data_ptr[i+1]->val[j].Id=-1;
        Insert_Bplustree(&tgrp);
        }
    }
}

void main()
{
    NodeG *nptr=(NodeG*)malloc(sizeof(NodeG));
    nptr->Id=3;
    Insert_Bplustree(nptr);

    nptr->Id=10;
    Insert_Bplustree(nptr);
    
    nptr->Id=1;
    Insert_Bplustree(nptr);

    nptr->Id=2;
    Insert_Bplustree(nptr);

    nptr->Id=9;
    Insert_Bplustree(nptr);

    nptr->Id=0;
    Insert_Bplustree(nptr);
    
    printf("\n%u",root->u.data_ptr[1]->val[0].Id);
    printf("\n%u",root->u.data_ptr[2]->val[0].Id);
    printf("\n%u",root->u.data_ptr[3]->val[0].Id);
    printf("\n%u",root->u.data_ptr[4]->val[0].Id);
    printf("\n%u",root->u.data_ptr[3]->val[1].Id);
    printf("\n%u",root->u.data_ptr[0]->val[0].Id);

}


