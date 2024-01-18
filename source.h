#include<iostream>
using namespace std;

class TaskNode
{
public:
	int id;
	string description;
	int priority;
	string AssigneeID;
	string color;
	bool completed;
	TaskNode* parent;
	TaskNode* right;
	TaskNode* left;
	TaskNode(int i, string d, int p, string ai)
	{
		id = i;
		description = d;
		priority = p;
		AssigneeID = ai;
		color = "";
		parent = NULL;
		right = NULL;
		left = NULL;
		completed = false;
	}
};

class AssigneeNode
{
public:
	int id;
	string ID;
	string firstName;
	string lastName;
	string address;
	string dateOfBirth;
	string color;
	AssigneeNode* parent;
	AssigneeNode* left;
	AssigneeNode* right;
	TaskNode* Tasks;
	AssigneeNode(string f, string l, string a, string d, int i)
	{
		ID = "A001";
		int index = 3;
		id = i;
		while (id > 0)
		{
			ID[index] = char((id % 10) + 48);
			id /= 10;
			index--;
		}
		firstName = f;
		lastName = l;
		address = a;
		dateOfBirth = d;
		left = NULL;
		right = NULL;
		parent = NULL;
		Tasks = NULL;
		color = "";
	}
};

class AssigneeTree
{
public:
	int id;
	AssigneeNode* root;
	AssigneeTree()
	{
		id = 1;
		root = NULL;
	}
	void addAssignee(string f, string l, string a, string d)
	{
		AssigneeNode* leaf = root, * parent = root;
		bool right = false;
		while (1)
		{
			if (leaf == NULL)
			{
				leaf = new AssigneeNode(f, l, a, d, id);
				if (root == NULL)
				{
					root = leaf;
					root->color = "black";
				}
				else
				{
					if (right)
					{
						parent->right = leaf;
					}
					else
					{
						parent->left = leaf;
					}
					leaf->parent = parent;
					leaf->color = "red";
					fixColorsAfterInsertion(leaf);
				}
				break;
			}
			if (id > leaf->id)
			{
				parent = leaf;
				leaf = leaf->right;
				right = true;
			}
			else
			{
				parent = leaf;
				leaf = leaf->left;
				right = false;
			}
		}
		id++;
	}
	void fixColorsAfterInsertion(AssigneeNode* leaf)
	{
		if (leaf == root)
		{
			leaf->color = "black";
		}
		else if (leaf->parent->color != "black")
		{
			if (leaf->parent->parent != NULL)
			{
				AssigneeNode* grandParent = leaf->parent->parent;
				if (grandParent->right == leaf->parent)
				{
					if (grandParent->left != NULL && grandParent->left->color == "red")
					{
						grandParent->color = "red";
						grandParent->left->color = "black";
						grandParent->right->color = "black";
						fixColorsAfterInsertion(grandParent);
					}
					else
					{
						if (grandParent->right->right == leaf)
						{
							RightRightRotation(leaf);
						}
						else if (grandParent->right->left == leaf)
						{
							RightLeftRotation(leaf);
						}
					}
				}
				else if (grandParent->left == leaf->parent)
				{
					if (grandParent->right != NULL && grandParent->right->color == "red")
					{
						grandParent->color = "red";
						grandParent->left->color = "black";
						grandParent->right->color = "black";
						fixColorsAfterInsertion(grandParent);
					}
					else
					{
						if (grandParent->left->right == leaf)
						{
							LeftRightRotation(leaf);
						}
						else if (grandParent->left->left == leaf)
						{
							LeftLeftRotation(leaf);
						}
					}
				}
			}
		}
	}
	AssigneeNode* search(string id)
	{
		AssigneeNode* leaf = root;
		while (1)
		{
			if (leaf == NULL)
			{
				return leaf;
			}
			else if (leaf->ID == id)
			{
				return leaf;
			}
			if (id > leaf->ID)
			{
				leaf = leaf->right;
			}
			else
			{
				leaf = leaf->left;
			}
		}
	}
	void attachTasks(int i, string d, int p, string ai)
	{
		AssigneeNode* leaf = search(ai);
		if (leaf != NULL)
		{
			while (leaf->Tasks != NULL)
			{
				leaf->Tasks = leaf->Tasks->right;
			}
			leaf->Tasks = new TaskNode(i, d, p, ai);
		}
	}
	void ShiftTask(string id1, string id2)
	{
		AssigneeNode* shift1 = search(id1), * shift2 = search(id2);
		string temp = shift2->Tasks->AssigneeID;
		shift2->Tasks = shift1->Tasks;
		shift2->Tasks->AssigneeID = temp;
		delete shift1->Tasks;
		shift1->Tasks = NULL;
	}
	void DeleteAssignee(string value)
	{
		AssigneeNode* leaf = root, * parent = root;
		if (root == NULL)
		{
			cout << "The Tree is empty" << endl;
			return;
		}
		bool right = false;
		while (1)
		{
			if (leaf == NULL)
			{
				return;
			}
			else if (leaf->ID == value)
			{
				if (leaf->Tasks != NULL)
				{
					return;
				}
				if (leaf->right == NULL && leaf->left == NULL)
				{
					AssigneeNode* temp = fixColorsAfterDeletion(leaf, leaf->right);
					delete leaf;
					removeDoubleBlack(temp);
					if (right)
					{
						parent->right = NULL;
					}
					else
					{
						parent->left = NULL;
					}
				}
				else if (leaf->right == NULL)
				{
					AssigneeNode* temp = fixColorsAfterDeletion(leaf, leaf->left);
					if (right)
					{
						parent->right = leaf->left;
					}
					else
					{
						parent->left = leaf->left;
					}
					leaf->left->parent = parent;
					delete leaf;
					removeDoubleBlack(temp);
				}
				else if (leaf->left == NULL)
				{
					AssigneeNode* temp = fixColorsAfterDeletion(leaf, leaf->right);
					if (right)
					{
						parent->right = leaf->right;
					}
					else
					{
						parent->left = leaf->right;
					}
					leaf->right->parent = parent;
					delete leaf;
					removeDoubleBlack(temp);
				}
				else
				{
					AssigneeNode* r = leaf->left;
					while (r->right != NULL)
					{
						r = r->right;
					}
					leaf->ID = r->ID;
					leaf->address = r->address;
					leaf->dateOfBirth = r->dateOfBirth;
					leaf->firstName = r->firstName;
					leaf->lastName = r->lastName;
					leaf->id = r->id;
					deleteFromTree(r);
				}
				return;
			}
			if (value > leaf->ID)
			{
				parent = leaf;
				leaf = leaf->right;
				right = true;
			}
			else if (value < leaf->ID)
			{
				parent = leaf;
				leaf = leaf->left;
				right = false;
			}
		}
	}
	void deleteFromTree(AssigneeNode*& treeNode)
	{
		AssigneeNode* leaf = root, * parent = root;
		if (root == NULL)
		{
			cout << "The Tree is empty" << endl;
			return;
		}
		bool right = false;
		bool first = true;
		while (1)
		{
			if (leaf == NULL)
			{
				return;
			}
			if (leaf->ID == treeNode->ID)
			{
				if (first)
				{
					first = false;
					right = false;
					parent = leaf;
					leaf = leaf->left;
				}
				else if (leaf->Tasks != NULL)
				{
					return;
				}
				else
				{
					if (leaf->right == NULL && leaf->left == NULL)
					{
						AssigneeNode* temp = fixColorsAfterDeletion(leaf, leaf->right);
						delete leaf;
						removeDoubleBlack(temp);
						if (right)
						{
							parent->right = NULL;
						}
						else
						{
							parent->left = NULL;
						}
					}
					else if (leaf->right == NULL)
					{
						AssigneeNode* temp = fixColorsAfterDeletion(leaf, leaf->left);
						if (right)
						{
							parent->right = leaf->left;
						}
						else
						{
							parent->left = leaf->left;
						}
						leaf->left->parent = parent;
						delete leaf;
						removeDoubleBlack(temp);
					}
					else if (leaf->left == NULL)
					{
						AssigneeNode* temp = fixColorsAfterDeletion(leaf, leaf->right);
						if (right)
						{
							parent->right = leaf->right;
						}
						else
						{
							parent->left = leaf->right;
						}
						leaf->right->parent = parent;
						delete leaf;
						removeDoubleBlack(temp);
					}
					return;
				}
			}
			if (leaf == NULL)
			{
				return;
			}
			if (treeNode->ID > leaf->ID)
			{
				parent = leaf;
				leaf = leaf->right;
				right = true;
			}
			else if (treeNode->ID < leaf->ID)
			{
				parent = leaf;
				leaf = leaf->left;
				right = false;
			}
		}
	}
	AssigneeNode* fixColorsAfterDeletion(AssigneeNode* deleted, AssigneeNode* replacer)
	{
		if (deleted->color == "red" || (replacer != NULL && replacer->color == "red"))
		{
			if (replacer != NULL)
			{
				replacer->color = "black";
			}
		}
		else if (deleted->color == "black" && (replacer == NULL || replacer->color == "black"))
		{
			if (replacer != NULL)
			{
				replacer->color = "double black";
			}
			else
			{
				string n = "NULL";
				replacer = new AssigneeNode(n, n, n, n, -1);
				replacer->color = "double black";
				if (deleted == root)
				{
					root = replacer;
				}
				else
				{
					AssigneeNode* dParent = deleted->parent;
					replacer->parent = dParent;
					if (dParent->right == deleted)
					{
						dParent->right = replacer;
					}
					else if (dParent->left == deleted)
					{
						dParent->left = replacer;
					}
				}
			}
		}
		return replacer;
	}
	void removeDoubleBlack(AssigneeNode* replacer)
	{
		if (replacer == NULL)
		{
			return;
		}
		if (replacer->color != "double black")
		{
			if (replacer->firstName == "NULL")
			{
				if (replacer->parent->left == replacer)
				{
					replacer->parent->left = NULL;
				}
				else if (replacer->parent->right == replacer)
				{
					replacer->parent->right = NULL;
				}
				delete replacer;
			}
			return;
		}
		if (replacer == root)
		{
			replacer->color = "black";
			return;
		}
		else
		{
			AssigneeNode* parent = replacer->parent, * sibling;
			bool rightSibling = false;
			if (parent->left == replacer)
			{
				sibling = parent->right;
				rightSibling = true;
			}
			else if (parent->right == replacer)
			{
				sibling = parent->left;
			}
			if (sibling->color == "black")
			{
				if ((sibling->left == NULL || sibling->left->color == "black") && (sibling->right == NULL || sibling->right->color == "black"))
				{
					sibling->color = "red";
					if (replacer->firstName == "NULL")
					{
						if (rightSibling)
						{
							parent->left = NULL;
						}
						else
						{
							parent->right = NULL;
						}
						delete replacer;
					}
					else
					{
						replacer->color = "black";
					}
					if (parent->color == "black")
					{
						parent->color = "double black";
						removeDoubleBlack(parent);
					}
					else
					{
						parent->color = "black";
					}
				}
				else if ((sibling->left != NULL && sibling->left->color == "red") || (sibling->right != NULL && sibling->right->color == "red"))
				{
					if (sibling->right != NULL && sibling->right->color == "red")
					{
						if (rightSibling)
						{
							RightRightRotation(sibling->right);
						}
						else
						{
							RightLeftRotation(sibling->right);
						}
					}
					else if (sibling->left != NULL && sibling->left->color == "red")
					{
						if (rightSibling)
						{
							RightLeftRotation(sibling->left);
						}
						else
						{
							LeftLeftRotation(sibling->left);
						}
					}
				}
			}
			else if (sibling->color == "red")
			{
				if (rightSibling)
				{
					RightRightRotation(sibling->right);
					parent->color = "black";
					parent->right->color = "red";
					if (parent->parent != NULL && parent->parent->right != NULL)
					{
						parent->parent->right->color = "black";
					}
				}
				else
				{
					LeftLeftRotation(sibling->left);
					parent->color = "black";
					parent->left->color = "red";
					if (parent->parent != NULL && parent->parent->left != NULL)
					{
						parent->parent->left->color = "black";
					}
				}
			}
			if (replacer != NULL && replacer->color == "double black")
			{
				replacer->color = "black";
			}
		}
	}
	void LeftLeftRotation(AssigneeNode* leaf)
	{
		AssigneeNode* parent = leaf->parent;
		AssigneeNode* grandParent = parent->parent;
		if (root == grandParent)
		{
			root = parent;
			parent->parent = NULL;
		}
		else
		{
			parent->parent = grandParent->parent;
			if (grandParent->parent->right == grandParent)
			{
				grandParent->parent->right = parent;
			}
			else if (grandParent->parent->left == grandParent)
			{
				grandParent->parent->left = parent;
			}
		}
		grandParent->left = parent->right;
		if (grandParent->left != NULL)
		{
			grandParent->left->parent = grandParent;
		}
		parent->right = grandParent;
		grandParent->parent = parent;
		string temp = parent->color;
		parent->color = grandParent->color;
		grandParent->color = temp;
		if (parent->left != NULL)
		{
			parent->left->color = temp;
		}
	}
	void RightRightRotation(AssigneeNode* leaf)
	{
		AssigneeNode* parent = leaf->parent;
		AssigneeNode* grandParent = parent->parent;
		if (root == grandParent)
		{
			root = parent;
			parent->parent = NULL;
		}
		else
		{
			parent->parent = grandParent->parent;
			if (grandParent->parent->right == grandParent)
			{
				grandParent->parent->right = parent;
			}
			else if (grandParent->parent->left == grandParent)
			{
				grandParent->parent->left = parent;
			}
		}
		grandParent->right = parent->left;
		if (grandParent->right != NULL)
		{
			grandParent->right->parent = grandParent;
		}
		parent->left = grandParent;
		grandParent->parent = parent;
		string temp = parent->color;
		parent->color = grandParent->color;
		grandParent->color = temp;
		if (parent->right != NULL)
		{
			parent->right->color = temp;
		}
	}
	void LeftRightRotation(AssigneeNode* leaf)
	{
		AssigneeNode* parent = leaf->parent;
		parent->right = leaf->left;
		if (parent->right != NULL)
		{
			parent->right->parent = parent;
		}
		leaf->parent = parent->parent;
		parent->parent = leaf;
		leaf->left = parent;
		leaf->parent->left = leaf;
		LeftLeftRotation(leaf->left);
	}
	void RightLeftRotation(AssigneeNode* leaf)
	{
		AssigneeNode* parent = leaf->parent;
		parent->left = leaf->right;
		if (parent->left != NULL)
		{
			parent->left->parent = parent;
		}
		leaf->parent = parent->parent;
		parent->parent = leaf;
		leaf->right = parent;
		leaf->parent->right = leaf;
		RightRightRotation(leaf->right);
	}
	void displaybyID(stringstream& output, string id)
	{
		AssigneeNode* leaf = search(id);
		output << "Name: " << leaf->firstName << " " << leaf->lastName << ", Address: " << leaf->address << ", DOB: " << leaf->dateOfBirth << ", Assignee ID: " << leaf->ID << endl;
	}
	void displaybyname(stringstream& output, AssigneeNode* root, string f, string l)
	{
		if (root == NULL)
		{
			return;
		}
		else if (root->firstName == f && root->lastName == l)
		{
			output << "Name: " << root->firstName << " " << root->lastName << ", Address: " << root->address << ", DOB: " << root->dateOfBirth << ", Assignee ID: " << root->ID << endl;
		}
		if (root->right == NULL)
		{
			displaybyname(output, root->left, f, l);
		}
		else if (root->left == NULL)
		{
			displaybyname(output, root->right, f, l);
		}
		else if (root->right != NULL && root->left != NULL)
		{
			displaybyname(output, root->left, f, l);
			displaybyname(output, root->right, f, l);
		}
	}
	void AssigneeWithNoTask(stringstream& output, AssigneeNode* root)
	{
		if (root == NULL)
		{
			return;
		}
		else
		{
			AssigneeWithNoTask(output, root->left);
			if (root->Tasks == NULL)
			{
				output << "Name: " << root->firstName << " " << root->lastName << ", Address: " << root->address << ", DOB: " << root->dateOfBirth << ", Assignee ID: " << root->ID << endl;
			}
			AssigneeWithNoTask(output, root->right);
		}
	}
	void AssigneeInOrder(stringstream& output, AssigneeNode* root)
	{
		if (root == NULL)
		{
			return;
		}
		else
		{
			AssigneeInOrder(output, root->left);
			output << root->ID << " (" << root->color << ")" << endl;
			AssigneeInOrder(output, root->right);
		}
	}
	void DisplayAssignee(stringstream& output, AssigneeNode* root)
	{
		if (root == NULL)
		{
			return;
		}
		else
		{
			DisplayAssignee(output, root->left);
			output << "Name: " << root->firstName << " " << root->lastName << ", Address: " << root->address << ", DOB: " << root->dateOfBirth << ", Assignee ID: " << root->ID << endl;
			DisplayAssignee(output, root->right);
		}
	}
};

class TaskTree
{
public:
	TaskNode* root;
	TaskTree()
	{
		root = NULL;
	}
	void addTask(int i, string d, int p, string ai)
	{
		TaskNode* leaf = root, * parent = root;
		bool right = false;
		while (1)
		{
			if (leaf == NULL)
			{
				leaf = new TaskNode(i, d, p, ai);
				if (root == NULL)
				{
					root = leaf;
					root->color = "black";
				}
				else
				{
					if (right)
					{
						parent->right = leaf;
					}
					else
					{
						parent->left = leaf;
					}
					leaf->parent = parent;
					leaf->color = "red";
					fixColorsAfterInsertion(leaf);
				}
				break;
			}
			if (p >= leaf->priority)
			{
				parent = leaf;
				leaf = leaf->right;
				right = true;
			}
			else
			{
				parent = leaf;
				leaf = leaf->left;
				right = false;
			}
		}
	}
	void fixColorsAfterInsertion(TaskNode* leaf)
	{
		if (leaf == root)
		{
			leaf->color = "black";
		}
		else if (leaf->parent->color != "black")
		{
			if (leaf->parent->parent != NULL)
			{
				TaskNode* grandParent = leaf->parent->parent;
				if (grandParent->right == leaf->parent)
				{
					if (grandParent->left != NULL && grandParent->left->color == "red")
					{
						grandParent->color = "red";
						grandParent->left->color = "black";
						grandParent->right->color = "black";
						fixColorsAfterInsertion(grandParent);
					}
					else
					{
						if (grandParent->right->right == leaf)
						{
							RightRightRotation(leaf);
						}
						else if (grandParent->right->left == leaf)
						{
							RightLeftRotation(leaf);
						}
					}
				}
				else if (grandParent->left == leaf->parent)
				{
					if (grandParent->right != NULL && grandParent->right->color == "red")
					{
						grandParent->color = "red";
						grandParent->left->color = "black";
						grandParent->right->color = "black";
						fixColorsAfterInsertion(grandParent);
					}
					else
					{
						if (grandParent->left->right == leaf)
						{
							LeftRightRotation(leaf);
						}
						else if (grandParent->left->left == leaf)
						{
							LeftLeftRotation(leaf);
						}
					}
				}
			}
		}
	}
	void ShiftTask(TaskNode* root, string id1, string id2)
	{
		if (root == NULL)
		{
			return;
		}
		if (root->AssigneeID == id1)
		{
			root->AssigneeID = id2;
		}
		if (root->right == NULL)
		{
			ShiftTask(root->left, id1, id2);
		}
		else if (root->left == NULL)
		{
			ShiftTask(root->right, id1, id2);
		}
		else if (root->right != NULL && root->left != NULL)
		{
			ShiftTask(root->left, id1, id2);
			ShiftTask(root->right, id1, id2);
		}
	}
	void printTaskQueue(stringstream& output, TaskNode* root, AssigneeTree* Assign)
	{
		if (root == NULL)
		{
			return;
		}
		else
		{
			printTaskQueue(output, root->left, Assign);
			if (root->completed == false)
			{
				output << "Task ID: " << to_string(root->id) << ", Description: " << root->description << ", Priority: " << to_string(root->priority) << ", Assignee: " << Assign->search(root->AssigneeID)->firstName << " (" << root->AssigneeID << ")" << endl;
			}
			printTaskQueue(output, root->right, Assign);
		}
	}
	void completeTask(int id, TaskNode* root)
	{
		if (root == NULL)
		{
			return;
		}
		else if (root->id == id)
		{
			root->completed = true;
		}
		else
		{
			completeTask(id, root->left);
			completeTask(id, root->right);
		}
	}
	TaskNode* search(int id, TaskNode* root)
	{
		if (root == NULL)
		{
			return root;
		}
		else if (root->id == id)
		{
			return root;
		}
		else
		{
			TaskNode* find = search(id, root->left);
			if (find != NULL && find->id == id)
			{
				return find;
			}
			TaskNode* find2 = search(id, root->right);
			if (find2 != NULL && find2->id == id)
			{
				return find2;
			}
		}
	}
	void findTasksByAssignee(string AssigneeID, stringstream& output, TaskNode* root, AssigneeTree* Assign)
	{
		if (root == NULL)
		{
			return;
		}
		else
		{
			findTasksByAssignee(AssigneeID, output, root->left, Assign);
			if (AssigneeID == root->AssigneeID)
			{
				output << "Task ID: " << to_string(root->id) << ", Description: " << root->description << ", Priority: " << to_string(root->priority) << ", Assignee: " << Assign->search(root->AssigneeID)->firstName << " (" << root->AssigneeID << ")" << endl;
			}
			findTasksByAssignee(AssigneeID, output, root->right, Assign);
		}
	}
	void updateTaskPriority(int id, int priority)
	{
		TaskNode* temp = search(id, root);
		int ID = temp->id;
		string description = temp->description;
		string AssigneeID = temp->AssigneeID;
		DeleteTask(temp->priority);
		addTask(ID, description, priority, AssigneeID);
	}
	void DeleteTask(int value)
	{
		TaskNode* leaf = root, * parent = root;
		if (root == NULL)
		{
			cout << "The Tree is empty" << endl;
			return;
		}
		bool right = false;
		while (1)
		{
			if (leaf == NULL)
			{
				return;
			}
			else if (leaf->priority == value)
			{
				if (leaf->right == NULL && leaf->left == NULL)
				{
					TaskNode* temp = fixColorsAfterDeletion(leaf, leaf->right);
					delete leaf;
					removeDoubleBlack(temp);
					if (right)
					{
						parent->right = NULL;
					}
					else
					{
						parent->left = NULL;
					}
				}
				else if (leaf->right == NULL)
				{
					TaskNode* temp = fixColorsAfterDeletion(leaf, leaf->left);
					if (right)
					{
						parent->right = leaf->left;
					}
					else
					{
						parent->left = leaf->left;
					}
					leaf->left->parent = parent;
					delete leaf;
					removeDoubleBlack(temp);
				}
				else if (leaf->left == NULL)
				{
					TaskNode* temp = fixColorsAfterDeletion(leaf, leaf->right);
					if (right)
					{
						parent->right = leaf->right;
					}
					else
					{
						parent->left = leaf->right;
					}
					leaf->right->parent = parent;
					delete leaf;
					removeDoubleBlack(temp);
				}
				else
				{
					TaskNode* r = leaf->left;
					while (r->right != NULL)
					{
						r = r->right;
					}
					leaf->priority = r->priority;
					leaf->id = r->id;
					leaf->AssigneeID = r->AssigneeID;
					leaf->completed = r->completed;
					leaf->description = r->description;
					deleteFromTree(r);
				}
				return;
			}
			if (value > leaf->priority)
			{
				parent = leaf;
				leaf = leaf->right;
				right = true;
			}
			else if (value < leaf->priority)
			{
				parent = leaf;
				leaf = leaf->left;
				right = false;
			}
		}
	}
	void deleteFromTree(TaskNode*& treeNode)
	{
		TaskNode* leaf = root, * parent = root;
		if (root == NULL)
		{
			cout << "The Tree is empty" << endl;
			return;
		}
		bool right = false;
		bool first = true;
		while (1)
		{
			if (leaf == NULL)
			{
				return;
			}
			if (leaf->priority == treeNode->priority)
			{
				if (first)
				{
					first = false;
					right = false;
					parent = leaf;
					leaf = leaf->left;
				}
				else
				{
					if (leaf->right == NULL && leaf->left == NULL)
					{
						TaskNode* temp = fixColorsAfterDeletion(leaf, leaf->right);
						delete leaf;
						removeDoubleBlack(temp);
						if (right)
						{
							parent->right = NULL;
						}
						else
						{
							parent->left = NULL;
						}
					}
					else if (leaf->right == NULL)
					{
						TaskNode* temp = fixColorsAfterDeletion(leaf, leaf->left);
						if (right)
						{
							parent->right = leaf->left;
						}
						else
						{
							parent->left = leaf->left;
						}
						leaf->left->parent = parent;
						delete leaf;
						removeDoubleBlack(temp);
					}
					else if (leaf->left == NULL)
					{
						TaskNode* temp = fixColorsAfterDeletion(leaf, leaf->right);
						if (right)
						{
							parent->right = leaf->right;
						}
						else
						{
							parent->left = leaf->right;
						}
						leaf->right->parent = parent;
						delete leaf;
						removeDoubleBlack(temp);
					}
					return;
				}
			}
			if (leaf == NULL)
			{
				return;
			}
			if (treeNode->priority > leaf->priority)
			{
				parent = leaf;
				leaf = leaf->right;
				right = true;
			}
			else if (treeNode->priority < leaf->priority)
			{
				parent = leaf;
				leaf = leaf->left;
				right = false;
			}
		}
	}
	TaskNode* fixColorsAfterDeletion(TaskNode* deleted, TaskNode* replacer)
	{
		if (deleted->color == "red" || (replacer != NULL && replacer->color == "red"))
		{
			if (replacer != NULL)
			{
				replacer->color = "black";
			}
		}
		else if (deleted->color == "black" && (replacer == NULL || replacer->color == "black"))
		{
			if (replacer != NULL)
			{
				replacer->color = "double black";
			}
			else
			{
				string n = "NULL";
				replacer = new TaskNode(-1, n, -1, n);
				replacer->color = "double black";
				if (deleted == root)
				{
					root = replacer;
				}
				else
				{
					TaskNode* dParent = deleted->parent;
					replacer->parent = dParent;
					if (dParent->right == deleted)
					{
						dParent->right = replacer;
					}
					else if (dParent->left == deleted)
					{
						dParent->left = replacer;
					}
				}
			}
		}
		return replacer;
	}
	void removeDoubleBlack(TaskNode* replacer)
	{
		if (replacer == NULL)
		{
			return;
		}
		if (replacer->color != "double black")
		{
			if (replacer->description == "NULL")
			{
				if (replacer->parent->left == replacer)
				{
					replacer->parent->left = NULL;
				}
				else if (replacer->parent->right == replacer)
				{
					replacer->parent->right = NULL;
				}
				delete replacer;
			}
			return;
		}
		if (replacer == root)
		{
			replacer->color = "black";
			return;
		}
		else
		{
			TaskNode* parent = replacer->parent, * sibling;
			bool rightSibling = false;
			if (parent->left == replacer)
			{
				sibling = parent->right;
				rightSibling = true;
			}
			else if (parent->right == replacer)
			{
				sibling = parent->left;
			}
			if (sibling->color == "black")
			{
				if ((sibling->left == NULL || sibling->left->color == "black") && (sibling->right == NULL || sibling->right->color == "black"))
				{
					sibling->color = "red";
					if (replacer->description == "NULL")
					{
						if (rightSibling)
						{
							parent->left = NULL;
						}
						else
						{
							parent->right = NULL;
						}
						delete replacer;
					}
					else
					{
						replacer->color = "black";
					}
					if (parent->color == "black")
					{
						parent->color = "double black";
						removeDoubleBlack(parent);
					}
					else
					{
						parent->color = "black";
					}
				}
				else if ((sibling->left != NULL && sibling->left->color == "red") || (sibling->right != NULL && sibling->right->color == "red"))
				{
					if (sibling->right != NULL && sibling->right->color == "red")
					{
						if (rightSibling)
						{
							RightRightRotation(sibling->right);
						}
						else
						{
							RightLeftRotation(sibling->right);
						}
					}
					else if (sibling->left != NULL && sibling->left->color == "red")
					{
						if (rightSibling)
						{
							RightLeftRotation(sibling->left);
						}
						else
						{
							LeftLeftRotation(sibling->left);
						}
					}
				}
			}
			else if (sibling->color == "red")
			{
				if (rightSibling)
				{
					RightRightRotation(sibling->right);
					parent->color = "black";
					parent->right->color = "red";
				}
				else
				{
					LeftLeftRotation(sibling->left);
					parent->color = "black";
					parent->left->color = "red";
				}
			}
		}
		if (replacer != NULL && replacer->color == "double black")
		{
			replacer->color = "black";
		}
	}
	void LeftLeftRotation(TaskNode* leaf)
	{
		TaskNode* parent = leaf->parent;
		TaskNode* grandParent = parent->parent;
		if (root == grandParent)
		{
			root = parent;
			parent->parent = NULL;
		}
		else
		{
			parent->parent = grandParent->parent;
			if (grandParent->parent->right == grandParent)
			{
				grandParent->parent->right = parent;
			}
			else if (grandParent->parent->left == grandParent)
			{
				grandParent->parent->left = parent;
			}
		}
		grandParent->left = parent->right;
		if (grandParent->left != NULL)
		{
			grandParent->left->parent = grandParent;
		}
		parent->right = grandParent;
		grandParent->parent = parent;
		string temp = parent->color;
		parent->color = grandParent->color;
		grandParent->color = temp;
		if (parent->left != NULL)
		{
			parent->left->color = temp;
		}
	}
	void RightRightRotation(TaskNode* leaf)
	{
		TaskNode* parent = leaf->parent;
		TaskNode* grandParent = parent->parent;
		if (root == grandParent)
		{
			root = parent;
			parent->parent = NULL;
		}
		else
		{
			parent->parent = grandParent->parent;
			if (grandParent->parent->right == grandParent)
			{
				grandParent->parent->right = parent;
			}
			else if (grandParent->parent->left == grandParent)
			{
				grandParent->parent->left = parent;
			}
		}
		grandParent->right = parent->left;
		if (grandParent->right != NULL)
		{
			grandParent->right->parent = grandParent;
		}
		parent->left = grandParent;
		grandParent->parent = parent;
		string temp = parent->color;
		parent->color = grandParent->color;
		grandParent->color = temp;
		if (parent->right != NULL)
		{
			parent->right->color = temp;
		}
	}
	void LeftRightRotation(TaskNode* leaf)
	{
		TaskNode* parent = leaf->parent;
		parent->right = leaf->left;
		if (parent->right != NULL)
		{
			parent->right->parent = parent;
		}
		leaf->parent = parent->parent;
		parent->parent = leaf;
		leaf->left = parent;
		leaf->parent->left = leaf;
		LeftLeftRotation(leaf->left);
	}
	void RightLeftRotation(TaskNode* leaf)
	{
		TaskNode* parent = leaf->parent;
		parent->left = leaf->right;
		if (parent->left != NULL)
		{
			parent->left->parent = parent;
		}
		leaf->parent = parent->parent;
		parent->parent = leaf;
		leaf->right = parent;
		leaf->parent->right = leaf;
		RightRightRotation(leaf->right);
	}
	void HighestPriorityTask(stringstream& output, AssigneeTree* Assign, TaskNode* root, int prio)
	{
		if (root == NULL)
		{
			return;
		}
		else
		{
			HighestPriorityTask(output, Assign, root->left, prio);
			if (root->priority == prio)
			{
				output << "Highest Priority Task: " << "Task ID: " << to_string(root->id) << ", Description: " << root->description << ", Priority: " << to_string(root->priority) << ", Assignee: " << Assign->search(root->AssigneeID)->firstName << " (" << root->AssigneeID << ")" << endl;
			}
			HighestPriorityTask(output, Assign, root->right, prio);
		}
	}
	void findHighestPriorityTask(stringstream& output, AssigneeTree* Assign)
	{
		TaskNode* leaf = root;
		while (leaf->left != NULL)
		{
			leaf = leaf->left;
		}
		int prio = leaf->priority;
		HighestPriorityTask(output, Assign, root, prio);
	}
	int countTotalTasks(TaskNode* root)
	{
		if (root == NULL)
		{
			return 0;
		}
		else
		{
			return 1 + countTotalTasks(root->left) + countTotalTasks(root->right);
		}
	}
	void displayCompletedTasks(stringstream& output, TaskNode* root, AssigneeTree* Assign)
	{
		if (root == NULL)
		{
			return;
		}
		else
		{
			displayCompletedTasks(output, root->left, Assign);
			if (root->completed)
			{
				output << "Task ID: " << to_string(root->id) << ", Description: " << root->description << ", Priority: " << to_string(root->priority) << ", Assignee: " << Assign->search(root->AssigneeID)->firstName << " (" << root->AssigneeID << ")" << endl;
			}
			displayCompletedTasks(output, root->right, Assign);
		}
	}
	void searchTasksByPriorityRange(int start, int end, stringstream& output, TaskNode* root, AssigneeTree* Assign)
	{
		if (root == NULL)
		{
			return;
		}
		else
		{
			searchTasksByPriorityRange(start, end, output, root->left, Assign);
			if (root->priority >= start && root->priority <= end)
			{
				output << "Task ID: " << to_string(root->id) << ", Description: " << root->description << ", Priority: " << to_string(root->priority) << ", Assignee: " << Assign->search(root->AssigneeID)->firstName << " (" << root->AssigneeID << ")" << endl;
			}
			searchTasksByPriorityRange(start, end, output, root->right, Assign);
		}
	}
	void TasksInOrder(stringstream& output, TaskNode* root)
	{
		if (root == NULL)
		{
			return;
		}
		else
		{
			TasksInOrder(output, root->left);
			output << root->id << " (" << root->color << ")" << endl;
			TasksInOrder(output, root->right);
		}
	}
};

class taskManagementSystem
{
	AssigneeTree* Assign;
	TaskTree* Tasks;
public:
	taskManagementSystem()
	{
		Assign = new AssigneeTree();
		Tasks = new TaskTree();
	}
	void addAssignee(string f, string l, string a, string d)
	{
		Assign->addAssignee(f, l, a, d);
	}
	void displaybyID(stringstream& output, string id)
	{
		Assign->displaybyID(output, id);
	}
	void displaybyname(stringstream& output, string first, string last)
	{
		Assign->displaybyname(output, Assign->root, first, last);
	}
	void addTask(int i, string d, int p, string ai)
	{
		Tasks->addTask(i, d, p, ai);
		Assign->attachTasks(i, d, p, ai);
	}
	void AssigneeWithNoTask(stringstream& output)
	{
		Assign->AssigneeWithNoTask(output, Assign->root);
	}
	void ShiftTask(string id1, string id2)
	{
		Tasks->ShiftTask(Tasks->root, id1, id2);
		Assign->ShiftTask(id1, id2);
	}
	void printTaskQueue(stringstream& output)
	{
		Tasks->printTaskQueue(output, Tasks->root, Assign);
	}
	void DeleteAssignee(string ID)
	{
		Assign->DeleteAssignee(ID);
	}
	void DisplayAssignee(stringstream& output)
	{
		Assign->DisplayAssignee(output, Assign->root);
	}
	void AssigneeInOrder(stringstream& output)
	{
		Assign->AssigneeInOrder(output, Assign->root);
	}
	void completeTask(int id)
	{
		Tasks->completeTask(id, Tasks->root);
	}
	void updateTaskPriority(int id, int priority)
	{
		Tasks->updateTaskPriority(id, priority);
	}
	void findHighestPriorityTask(stringstream& output)
	{
		Tasks->findHighestPriorityTask(output, Assign);
	}
	void findTasksByAssignee(string AssigneeID, stringstream& output)
	{
		output << "Tasks Assigned to \"" << Assign->search(AssigneeID)->firstName << " (" << AssigneeID << ")\":" << endl;
		Tasks->findTasksByAssignee(AssigneeID, output, Tasks->root, Assign);
	}
	void countTotalTasks(stringstream& output)
	{
		output << "Total Tasks in the System: " << to_string(Tasks->countTotalTasks(Tasks->root)) << endl;
	}
	void displayCompletedTasks(stringstream& output)
	{
		output << "Completed Tasks:" << endl;
		Tasks->displayCompletedTasks(output, Tasks->root, Assign);
	}
	void searchTasksByPriorityRange(int start, int end, stringstream& output)
	{
		output << "Tasks within Priority Range (" << to_string(start) << " to " << to_string(end) << "):" << endl;
		Tasks->searchTasksByPriorityRange(start, end, output, Tasks->root, Assign);
	}
	void PrintTreeInorder(stringstream& output)
	{
		Tasks->TasksInOrder(output, Tasks->root);
	}
	void deleteTask(int id)
	{
		TaskNode* temp = Tasks->search(id, Tasks->root);
		Tasks->DeleteTask(temp->priority);
	}
};
