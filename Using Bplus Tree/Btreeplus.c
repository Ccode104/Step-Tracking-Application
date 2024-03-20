typedef struct Data_Node_tag
{
	int val[CAPACITY];
	int size;
}

typedef struct Bptree_Node
{
	int val[ORDER];
	union u_tag
	{
		struct Bptree_Key_Node *child[ORDER+1];
		Data_Node *data[M+1];
	} u;
	char next_node;//k or d
}