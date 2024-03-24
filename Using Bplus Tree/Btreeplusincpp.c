int bucketSize = 3;

// Create 2 classes, one for node and one for tree;

typedef struct node {
	Boolean isLeaf;
	union u_tag
	struct node** ptr;
	unsigned int *key;
	int size;
} node;
void Create_node(node **npptr)
{
	(*npptr)->key=(NodeG*)malloc(bucketSize*sizeof(NodeG));
	(*npptr)->ptr =(node*)malloc((bucketSize + 1)*sizeof(node));
}
typedef struct Btree {
	// Root of tree stored here;
	node* root;
	Btree();
	void deleteNode(int);

	int search(int);
	void display(node*);
	void insert(int);
	node* findParent(node*, node*);
	node* getRoot();
	void shiftLevel(int, node*, node*);
} Btree;

node* getRoot(Btree *b) 
{ 
	return b->root; 
}
void Create_Btree(Btree *b) 
{ 
	b->root = NULL; 
}

void insert(Btree *b,int x)
{
	if (root == NULL) {
		root = (node*)malloc(sizeof(node));
		root->key[0] = x;
		root->isLeaf = true;
		root->size = 1;
	}

	else {
		node* current = root;
		node* parent;

		while (current->isLeaf == false) {
			parent = current;

			for (int i = 0; i < current->size; i++) {
				if (x < current->key[i]) {
					current = current->ptr[i];
					break;
				}

				if (i == current->size - 1) {
					current = current->ptr[i + 1];
					break;
				}
			}
		}

		// now we have reached leaf;
		if (current->size
			< bucketSize) { // if the node to be inserted is
							// not filled
			int i = 0;

			// Traverse btree
			while (x > current->key[i] && i < current->size)
				// goto pt where needs to be inserted.
				i++;

			for (int j = current->size; j > i; j--)
				// adjust and insert element;
				current->key[j] = current->key[j - 1];

			current->key[i] = x;

			// size should be increased by 1
			current->size++;

			current->ptr[current->size]
				= current->ptr[current->size - 1];
			current->ptr[current->size - 1] = NULL;
		}

		// if block does not have enough space;
		else {
			node* newLeaf = new node;
			int tempNode[bucketSize + 1];

			for (int i = 0; i < bucketSize; i++)
				// all elements of this block stored
				tempNode[i] = current->key[i];
			int i = 0, j;

			// find the right posn of num to be inserted
			while (x > tempNode[i] && i < bucketSize)
				i++;

			for (int j = bucketSize + 1; j > i; j--)
				tempNode[j] = tempNode[j - 1];
			tempNode[i] = x;
			// inserted element in its rightful position;

			newLeaf->isLeaf = true;
			current->size = (bucketSize + 1) / 2;
			newLeaf->size
				= (bucketSize + 1)
				- (bucketSize + 1)
						/ 2; // now rearrangement begins!

			current->ptr[current->size] = newLeaf;
			newLeaf->ptr[newLeaf->size]
				= current->ptr[bucketSize];

			current->ptr[newLeaf->size]
				= current->ptr[bucketSize];
			current->ptr[bucketSize] = NULL;

			for (int i = 0; i < current->size; i++)
				current->key[i] = tempNode[i];

			for (int i = 0, j = current->size;
				i < newLeaf->size; i++, j++)
				newLeaf->key[i] = tempNode[j];

			// if this is root, then fine,
			// else we need to increase the height of tree;
			if (current == root) {
				node* newRoot = new node;
				newRoot->key[0] = newLeaf->key[0];
				newRoot->ptr[0] = current;
				newRoot->ptr[1] = newLeaf;
				newRoot->isLeaf = false;
				newRoot->size = 1;
				root = newRoot;
			}
			else
				shiftLevel(
					newLeaf->key[0], parent,
					newLeaf); // parent->original root
		}
	}
}

void Btree::shiftLevel(int x, node* current, node* child)
{ // insert or create an internal node;
	if (current->size
		< bucketSize) { // if can fit in this level, do that
		int i = 0;
		while (x > current->key[i] && i < current->size)
			i++;
		for (int j = current->size; j > i; j--)
			current->key[j] = current->key[j - 1];

		for (int j = current->size + 1; j > i + 1; j--)
			current->ptr[j] = current->ptr[j - 1];

		current->key[i] = x;
		current->size++;
		current->ptr[i + 1] = child;
	}

	// shift up
	else {
		node* newInternal = new node;
		int tempKey[bucketSize + 1];
		node* tempPtr[bucketSize + 2];

		for (int i = 0; i < bucketSize; i++)
			tempKey[i] = current->key[i];

		for (int i = 0; i < bucketSize + 1; i++)
			tempPtr[i] = current->ptr[i];

		int i = 0, j;
		while (x > tempKey[i] && i < bucketSize)
			i++;

		for (int j = bucketSize + 1; j > i; j--)
			tempKey[j] = tempKey[j - 1];

		tempKey[i] = x;
		for (int j = bucketSize + 2; j > i + 1; j--)
			tempPtr[j] = tempPtr[j - 1];

		tempPtr[i + 1] = child;
		newInternal->isLeaf = false;
		current->size = (bucketSize + 1) / 2;

		newInternal->size
			= bucketSize - (bucketSize + 1) / 2;

		for (int i = 0, j = current->size + 1;
			i < newInternal->size; i++, j++)
			newInternal->key[i] = tempKey[j];

		for (int i = 0, j = current->size + 1;
			i < newInternal->size + 1; i++, j++)
			newInternal->ptr[i] = tempPtr[j];

		if (current == root) {
			node* newRoot = new node;
			newRoot->key[0] = current->key[current->size];
			newRoot->ptr[0] = current;
			newRoot->ptr[1] = newInternal;
			newRoot->isLeaf = false;
			newRoot->size = 1;
			root = newRoot;
		}

		else
			shiftLevel(current->key[current->size],
					findParent(root, current),
					newInternal);
	}
}
int Btree::search(int x)
{
	if (root == NULL)
		return -1;

	else {
		node* current = root;
		while (current->isLeaf == false) {
			for (int i = 0; i < current->size; i++) {
				if (x < current->key[i]) {
					current = current->ptr[i];
					break;
				}

				if (i == current->size - 1) {
					current = current->ptr[i + 1];
					break;
				}
			}
		}

		for (int i = 0; i < current->size; i++) {
			if (current->key[i] == x) {
				// cout<<"Key found "<<endl;
				return 1;
				// return;
			}
		}

		// cout<<"Key not found"<<endl;
		return 0;
	}
}

// Print the tree
void Btree::display(node* current)
{
	if (current == NULL)
		return;
	queue<node*> q;
	q.push(current);
	while (!q.empty()) {
		int l;
		l = q.size();

		for (int i = 0; i < l; i++) {
			node* tNode = q.front();
			q.pop();

			for (int j = 0; j < tNode->size; j++)
				if (tNode != NULL)
					cout << tNode->key[j] << " ";

			for (int j = 0; j < tNode->size + 1; j++)
				if (tNode->ptr[j] != NULL)
					q.push(tNode->ptr[j]);

			cout << "\t";
		}
		cout << endl;
	}
}

node* Btree::findParent(node* current, node* child)
{
	node* parent;
	if (current->isLeaf || (current->ptr[0])->isLeaf)
		return NULL;

	for (int i = 0; i < current->size + 1; i++) {
		if (current->ptr[i] == child) {
			parent = current;
			return parent;
		}
		else {
			parent = findParent(current->ptr[i], child);
			if (parent != NULL)
				return parent;
		}
	}
	return parent;
}

signed main()
{
	ios_base::sync_with_stdio(false);
	Btree node;
	cout << "The size of bucket is " << bucketSize << "! "
		<< endl;

	node.insert(1);
	node.insert(2);
	node.insert(3);
	node.display(node.getRoot());
	node.insert(4);
	node.insert(5);
	node.display(node.getRoot());

	return 0;
}
