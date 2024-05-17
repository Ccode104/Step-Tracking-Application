# Step tracking Application

A step tracking application lets a user calculate daily steps, set goals for a week, create groups to
achieve group goals, and gives rewards to individuals who complete their goals. It also maintains
leader boards to encourage people to complete steps with great rewards for top 3 individuals. It also
has a group leader board where groups are ranked according to steps completed by each group as a
whole. We have to design this step tracking application using trees. 

The project was done in a group of 2 members.

## Representation of Data

### Generates a tree of individuals with following fields
a. ID (unique ID)
b. Name
c. Age
d. Daily Step goal
e. Array of weekly step count (7 days step count recorded)
This tree is sorted on the basis of ID

### Generates a tree of groups with following fields
a. Group-ID
b. Group name
c. Member IDs with pointers to individuals in individuals tree
d. Weekly group goal
This tree is be sorted on the basis of Group-ID. A group can contain maximum 5
individuals.

Here the tree is a B-Tree.

#### Linked List Implementation:
• This was also done using Linked Lists.(Alternative to B-Tree) 
•	Algorithm for sorting used is Radix Sort and the data is stored in the form of linked lists. 
•	Traversal ,Search ,Sort, Insert and Delete operations on the Singly Linked Lists were performed and practiced.
•	Implementation of a Hash Table for performing Radix Sort was done.

## Functions

### The application should have the following functionalities

a. Add_Person: This function should add a new individual to the tree of individuals. The
tree should remain sorted
b. Create_group: This function should create a new group and be able to add existing
individuals to it from the individual tree. If an individual already belongs to a group,
he cannot be added to a new group.
c. Get_top_3: this function should display the top 3 individuals from the individual tree
who have completed their daily steps goals and achieved highest number of steps.
The individuals who have not completed daily goals but have higher number of steps
should be excluded.
d. Check_group_achievement(Group-ID): This function should display whether the
given group has completed its weekly group goal
e. Generate_leader_board: This function should take the group tree, sort it in the
order of highest number of steps (Descending) completed by each group such that
the root of the tree has the leading group, and display the group name with number
of steps based on rank using this sorted tree.
f. Check_individual_rewards(ID): This function should display the rewards earned by
the given individual if he is in the top 3 individuals. Rank 1 gets 100 points, rank 2
gets 75 points, and rank 3 gets 50 points 

# References:
  The Data Structure and Program Design in C book by Robert Kruse.  
