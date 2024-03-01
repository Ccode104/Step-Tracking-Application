/*Node for Individuals*/
typedef struct NodeI_tag
{
	unsigned int Id;
	char Name[20];
	unsigned int Age;
	unsigned int Daily_Step_Goal;
	unsigned int Weekly_Step_count[7];
} NodeI;

/*Node type for Group*/
typedef struct NodeG_tag
{
	unsigned int Id;
	char Name[20];
	NodeI *Members[5];
	unsigned int Weekly_Group_Goal;
} NodeG;

