/*Assignment 4: This is a C++ project using a stack to implement a Binary tree. The Binary tree is used to evaluate expressions like (a+b*c). The traversal starts from the leftmost leaf to its root, then to the right child of the same root, to its leftmost leaf, and so on. The postfix, preorder, etc., have different orders as shown below. I have also managed to avoid memory leaks and ensured various data types are acceptable for our Expression. */
/*Masoud Rafiee*/
#include <iostream>
#include <stack>
#include <cstdlib>
#include <string>

struct TreeNode {
	char element;
	TreeNode* leftChild;
	TreeNode* rightChild;

	TreeNode(char data) : element(data), leftChild(nullptr), rightChild(nullptr) {}//constructor for each node
};

class ExpressionTree {
private:
	TreeNode* root;//for hodling the root our expression tree

	bool isOperator(char ch) {//to see if the input is an operator
		return ch == '+' || ch == '-' || ch == '*' || ch == '/';//returning a bool value
	}

	int getPrecedence(char op) {//takes an operator as input 
		if (op == '+' || op == '-')
			return 1;//setting the priority of + and - 
		if (op == '*' || op == '/')
			return 2;//same for * and /
		return 0;
	}

	void buildTreeHelper(const std::string& expr, std::stack<TreeNode*>& nodes) {//takes a const reference to a str representing the expression and a reference to a stack of pointers to TreeNode (construcnting a expression tree from given expression and its starting node
		std::stack<char> operators;//storing characters as operators in a stack . to use it identify operartors when traversing string 
		for (char ch : expr) {//loop over each character in expression
			if (std::isalnum(ch)) {//checking if its a decimal-character  
				nodes.push(new TreeNode(ch));//if it is, creating a new tree node with chas its elem and push it to nodes stack
			}
			else if (isOperator(ch)) {//if it's a operator (not operand like above)
				while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(ch)) {//to handle operator priority , if there exists operator on the operator stack and precednce of it is greater or equal than current operator ch
					TreeNode* right = nodes.top();//retrive top node ptr from the nodes stack  (right operand of the current operator)
					nodes.pop();//remove the top nodes from the nodes stck (because we have already stored it in variable "right"
					TreeNode* left = nodes.top();//left operand of the current operator 
					nodes.pop();//pop it <same reason>
					TreeNode* current = new TreeNode(operators.top());//operation to be performed at this level of the tree
					current->leftChild = left;//connecting the left operan to the operator node
					current->rightChild = right;//same for right child
					nodes.push(current);//constrcutred subtree with current as the root , we push it to nodes stack , to be treated as a single node
					operators.pop();//we already pushed it to nodes stack so we dont need it anymore
				}
				operators.push(ch);//pushing this new operator to operator stack/to ensure the precednce of operaotrs
			}
		}
		while (!operators.empty()) {//to handle any remaining operator on the operators stack after proccessing all characters in the expression (until the stack is empty
			TreeNode* right = nodes.top();//retriving the right operand of the current operator
			nodes.pop();//remove it as it has been proccessed 
			TreeNode* left = nodes.top();//same fir left operand 
			nodes.pop();//remove again
			TreeNode* current = new TreeNode(operators.top());//create a mew node current with the operaotr at the tp of the operators stack
			current->leftChild = left;//set the left child of the current node to the left operand node  
			current->rightChild = right;//same for right node
			nodes.push(current);//pushes the subtree current as root onto the nodes stack
			operators.pop();//remove it 
		}
		if (nodes.size() != 1) {//if there is exatly one node remaining in the nodes stack after constructing the expression tree
			/*if there are too many operands or not enough operators, the condition evaluates to true.*/
			std::cerr << "Invalid expression: too many operands or not enough operators" << std::endl;//error message to the standard error stream
			exit(EXIT_FAILURE);//exit with an failurre status code to indicate the expreesion cannot proceed to an error
		}
		root = nodes.top();//assigning the top node stack as root of the expression tree
		nodes.pop();//remove it as it has been assigned as root 
	}

	void printInfixHelper(TreeNode* node) {
		if (node) {//if it's not nullptr
			if (node->leftChild && isOperator(node->leftChild->element))//if the current node has a left child and if the element of the left child is an operator 
				std::cout << "(";//to ensure correct precednce 
			printInfixHelper(node->leftChild);//printing the infix notation of the left subtree recursively 
			std::cout << node->element << " ";// printing the elemens of the current node elems
			printInfixHelper(node->rightChild);//same for the right subtree
			if (node->rightChild && isOperator(node->rightChild->element))
				std::cout << ")";
		}
	}

	void printPrefixHelper(TreeNode* node) {
		if (node) {//if the current node pointer node is not nulptr(it's a valid node)
			std::cout << node->element << " ";
			printPrefixHelper(node->leftChild);//printing the prefix notation of the left subtree
			printPrefixHelper(node->rightChild);//same for right 
		}
	}

	void printPostfixHelper(TreeNode* node) {//same but for post fix
		if (node) {
			printPostfixHelper(node->leftChild);
			printPostfixHelper(node->rightChild);
			std::cout << node->element << " ";
		}
	}
	int evaluateHelper(TreeNode* node) {//takes a pointer to a tree node and returns an integer
		if (!node)//basecase for recursion
			return 0;
		if (!node->leftChild && !node->rightChild) {//if it's a leaf node
			if (std::isalpha(node->element)) {//if it's alphabet character
				std::cout << "It has no evaluation result" << std::endl;
				exit(EXIT_FAILURE);//failure status code to exit
			}
			return node->element - '0'; // Convert char to integer (using ASCII value )
		}
		int leftVal = evaluateHelper(node->leftChild);//evaluated value of left subtree
		int rightVal = evaluateHelper(node->rightChild);//same for right subtree
		switch (node->element) {
		case '+':
			return leftVal + rightVal;
		case '-':
			return leftVal - rightVal;
		case '*':
			return leftVal * rightVal;
		case '/':
			return leftVal / rightVal;
		default:
			return 0;
		}
	}

public:
	ExpressionTree(const std::string& expr) {//constructor 
		std::stack<TreeNode*> nodes;//storing pointers to TreeNode object in stack (nodes) to build the expr tree
		buildTreeHelper(expr, nodes);//expr passed by value to avoid unneccassry copying the input str
		//nodes passed by value to modify its content while building the tree
	}

	~ExpressionTree() {//destructor to handle memory deallocation
	}

	void printInfix() {//printing the infix 
		printInfixHelper(root);//root to take for recursive printing
		std::cout << std::endl;
	}

	void printPrefix() {
		printPrefixHelper(root);//same for Prefix
		std::cout << std::endl;
	}

	void printPostfix() {
		printPostfixHelper(root);//same for Postfix
		std::cout << std::endl;
	}
	void evaluate() {//function to print the result
		std::cout << "Result: " << evaluateHelper(root) << std::endl;
	}
};

int main() {
	std::string expression;
	std::cout << "Type an Expression: " << std::endl;
	std::getline(std::cin, expression);

	ExpressionTree tree(expression);

	std::cout << "Infix: ";
	tree.printInfix();
	std::cout << "Prefix: ";
	tree.printPrefix();
	std::cout << "Postfix: ";
	tree.printPostfix();
	tree.evaluate();
	return 0;
}

