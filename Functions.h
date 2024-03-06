typedef enum{NO,YES} belongs;

/*Node for Individuals*/
typedef struct NodeI_tag
{
	unsigned int Id;
	char Name[20];
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
typedef enum{FAILURE,SUCCESS} status_code;



typedef enum{FALSE,TRUE} Boolean;
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

/*Get teh maximum steps of group*/
unsigned int getmax_GSteps();


