/*Node for Individuals*/
typedef struct NodeI_tag
{
	unsigned int Id;
	char Name[20];
	unsigned int Age;
	unsigned int Daily_Step_Goal;
	unsigned int Weekly_Step_count[7];
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

/*Status Code to trace the errors*/
typedef enum{FALSE,TRUE} status_code;

//Helper Functions
status_code Store_Member_Pointers(NodeG*,unsigned int*);

//Main Functionalities
/*Add a Person*/
status_code Add_Person(NodeI*);

/*Create a Group*/
status_code Create_Group(NodeG*);

/*Get top 3 individuals*/
status_code Get_top_3(NodeI**);

/*Check the group achievement*/
