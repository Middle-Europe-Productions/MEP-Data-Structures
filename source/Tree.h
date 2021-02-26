////////////////////////////////////////////////////////////
//
//	This file is a modified version of my private repository.
//  https://github.com/PioSkay/EADS_Dictionary
//  In that repository there is also an early varion of the MEP Data Structures.
//
////////////////////////////////////////////////////////////

#pragma once
#include<iostream>
template<typename Key, typename Info>
class Tree {
public:
	/**
	* \brief Types of traversing the tree.
	*/
	enum class TravelType {
		Preorder,
		Inorder,
		Postorder
	};
protected:
	struct Node {
		Key m_key;
		Info m_info;
		Node* left;
		Node* right;
		Node* up;
		int height;
		Node(const Key& key, const Info& info, int h = 1) : m_key(key), m_info(info), left(nullptr), right(nullptr), up(nullptr), height(h) {}
		Node(const Key& key, const Info& info, Node* l, Node* r, Node* u, int h) : m_key(key), m_info(info), left(l), right(r), up(u), height(h) {}
	};
	Node* head;
	/**
	* Clears the AVL & updates the curr element.
	*/
	void clear(Node*& curr);
	/**
	* Copy the elements.
	*/
	Node* copy(Node* toCopy, Node* prev = nullptr);
	/**
	* Outputs the current height of a node.
	*/
	int getHeight(Node* curr) const;
	/**
	* Outputs the calculated balance of a node. According to the left and right subtree.
	*/
	int getBalance(Node* curr) const;
	/**
	* Outputs a node with the minimal value of the subtree.
	*/
	Node* getMinimalValueNode(Node* curr) const;
	/**
	* Outputs a node with the maximal value of the subtree.
	*/
	Node* getMaximalValueNode(Node* curr) const;
	/**
	* Left rotation:
	*   A               B
	*  / \             / \___
	* ... B    ->     A      C
	*    / \         / \    / \
	*   X1  C	     .. X1  ...
	*      / \
	*      ...
	*/
	void leftRotate(Node*& A);
	/**
	* Right rotation:
	*      A              B
	*     / \            / \___
	*     B ...  ->     C      A
	*    / \           / \    / \
	*   C	X1         ...   X1 ...
	*  / \
	*  ...
	*/
	void rightRotate(Node*& A);
	/**
	* Inserts the node and balances the tree.
	*/
	void insert(Node*& curr, const Key& key, const Info& info, Node*& newNode);
	/**
	* Removes an element from the tree.
	*/
	Node* remove(Node* curr, const Key& key);
	/**
	* Compares the nodes. In a recursive way.
	*/
	bool compareNodes(Node* a, Node* b) const;
	/**
	* A universal method of traversing the tree.
	*/
	template<typename ToDo>
	void uniTraversal(Node* curr, ToDo method, const TravelType type) const;
public:
	/**
	* Iterator of the AVL tree.
	*/
	class iterator {
		friend Tree<Key, Info>;
		const Tree<Key, Info>* master;
		mutable Node* curr;
		iterator(Node* x, const Tree<Key, Info>* m);
	public:
		iterator();
		iterator(const iterator& x);
		~iterator() = default;

		bool operator==(const iterator& x) const;

		bool operator!=(const iterator& x) const;

		const iterator& operator++() const;

		iterator operator++(int) const;

		const iterator& operator--() const;

		iterator operator--(int) const;

		const Info& operator*() const;

		Info& operator*();

		const Key& getKey() const;

		const Info& getInfo() const;

		bool isNull() const;

		friend std::ostream& operator<<(std::ostream& out, const iterator& it) {
			if (it.isNull()) {
				out << "Key: " << " - " << ", info: " << " - ";
			}
			else {
				out << "Key: " << it.getKey() << ", info: " << it.getInfo();
			}
			return out;
		}
	};
	/**
	* Reverse iterator of the AVL tree.
	*/
	class reverse_iterator : public iterator {
		friend Tree<Key, Info>;
		reverse_iterator(Node* x, const Tree<Key, Info>* m);
	public:
		reverse_iterator();

		reverse_iterator(const reverse_iterator& x);

		~reverse_iterator() = default;

		const reverse_iterator& operator++() const;

		reverse_iterator operator++(int) const;

		const reverse_iterator& operator--() const;

		reverse_iterator operator--(int) const;
	};
	/**
	* The iterator class is capable of traversing throught a constant Dictionary because Node* is specified as mutable.
	* Instead of writing a seperate class ill specift a constant iterator like that:
	*/
	typedef const iterator const_iterator;
	/**
	* The reverse iterator class is capable of traversing throught a constant Dictionary because Node* is specified as mutable.
	* Instead of writing a seperate class ill specift a constant reverse_iterator like that:
	*/
	typedef const reverse_iterator const_reverse_iterator;
	/**
	* An iterator to the smalles element of the tree.
	* Complexity O(logN) where N is a number of element or O(h) here h is a height of the tree.
	*/
	iterator begin() const;
	/**
	* An iterator to the end.
	*/
	iterator end() const;
	/**
	* An reverese iterator to the biggest element of the tree.
	* Complexity O(logN) where N is a number of element or O(h) here h is a height of the tree.
	*/
	reverse_iterator rbegin() const;
	/**
	* An iterator to the end.
	*/
	reverse_iterator rend() const;
	/**
	* Constructor creates an empty tree.
	*/
	Tree();
	/**
	* Copy constructor.
	*/
	Tree(const Tree<Key, Info>& x);
	/**
	* Destructor.
	*/
	~Tree();
	/**
	* Outputs the height of a tree. O(1).
	*/
	int getHeight() const;
	/**
	* Checks whether the tree is empty.
	*/
	bool empty() const;
	/**
	* Deletes all of the elements from the tree.
	*/
	void clear();
	/**
	* Assign operator.
	*/
	Tree& operator=(const Tree<Key, Info>& x);
	/**
	* Comparision operator.
	*/
	bool operator==(const Tree<Key, Info>& x) const;
	/**
	* Comparision operator.
	*/
	bool operator!=(const Tree<Key, Info>& x) const;
	/**
	* Functionallity: Inserts the node to the AVL tree.
	* Approche: I am using recursive insert. There is possible throw when key is already in the tree, however, public method outputs true/false in that case.
	* param[in] key : Key of the element that is going to be inserted.
	* param[in] info : Info of the element that is going to be inserted.
	*/
	bool insert(const Key& key, const Info& info);
	/**
	* Functionallity: Slightly modified insert method. It outputs the iterator to the a new element or an iterator to existing element.
	* Approche: I am using recursive insert. There is possible throw when key is already in the tree, however, public method outputs true/false in that case.
	* param[in] key : Key of the element that is going to be inserted.
	* param[in] info : Info of the element that is going to be inserted.
	* param[in] it : A reference to iterator to which we want to provide information about the added/existing element.
	*/
	bool insert(const Key& key, const Info& info, iterator& it);
	/**
	* Functionality: Delete the node from the AVL tree.
	* Approche: I am using recursive remove. There is possible throw when key is not a memeber of the tree, however, public method outputs true/false in that case.
	* param[in] key : Key of the element that is going to be deleted.
	*/
	bool remove(const Key& key);
	/**
	* Functionality: Executes set of instruction for all of the nodes of the AVL tree.
	* Approche: Recursive method executes ToDo method according to the TravelType (preorder by default)
	* ToDo method do need to have const_iterator to the element at the imput. In order to work properly.
	* Proper construction of method:
	* [](Dictionary<int,int>::const_iterator& in)->void {...}
	* Auto is also fine:
	* [](auto in)->void {...}
	* param[in] method : Aforementioned method specifing todo operations in every step.
	* param[in] type : By default TravelType::Preorder. Specifies the type of traversing.
	*/
	template<typename ToDo>
	void executeForAllNodes(ToDo method, TravelType type = TravelType::Preorder) const;
	/**
	* There are also three classic BST traversals.
	* param[in] type : Travel type.
	* param[in] out : By default std::cout. Specifies the ostream variable.
	*/
	void traversal(const TravelType type, std::ostream& out = std::cout) const;
	/**
	* This is classic BST traversals. I have implemented bacause traversal method requires some additional typing.
	* param[in] out : By default std::cout. Specifies the ostream variable.
	*/
	void preorder(std::ostream& out = std::cout) const;
	/**
	* This is classic BST traversals. I have implemented bacause traversal method requires some additional typing.
	* param[in] out : By default std::cout. Specifies the ostream variable.
	*/
	void inorder(std::ostream& out = std::cout) const;
	/**
	* This is classic BST traversals. I have implemented bacause traversal method requires some additional typing.
	* param[in] out : By default std::cout. Specifies the ostream variable.
	*/
	void postorder(std::ostream& out = std::cout) const;
	/**
	* The method finds an element with a given key.
	* If an element was not found the empty iterator is being returned.
	* To check whether thew iterator has a walue use InNull method.
	* param[in] key : Key to find a value.
	*/
	iterator find(const Key& key) const;
};

template<typename Key, typename Info>
void Tree<Key, Info>::clear(Node*& curr)
{
	if (curr) {
		clear(curr->left);
		clear(curr->right);
		delete curr;
		curr = nullptr;
	}
}

template<typename Key, typename Info>
typename Tree<Key, Info>::Node* Tree<Key, Info>::copy(Node* toCopy, Node* prev) {
	Node* cache = nullptr;
	if (toCopy) {
		cache = new Node(toCopy->m_key, toCopy->m_info, toCopy->height);
		cache->up = prev;
		cache->left = copy(toCopy->left, cache);
		cache->right = copy(toCopy->right, cache);
	}
	return cache;
}

template<typename Key, typename Info>
int Tree<Key, Info>::getHeight(Node* curr) const {
	if (curr)
		return curr->height;
	else
		return 0;
}

template<typename Key, typename Info>
int Tree<Key, Info>::getBalance(Node* curr) const {
	if (curr)
		return getHeight(curr->left) - getHeight(curr->right);
	else
		return 0;
}

template<typename Key, typename Info>
typename Tree<Key, Info>::Node* Tree<Key, Info>::getMinimalValueNode(Node* curr) const {
	Node* cache = curr;
	if (cache)
		while (cache->left)
			cache = cache->left;
	return cache;
}

template<typename Key, typename Info>
typename Tree<Key, Info>::Node* Tree<Key, Info>::getMaximalValueNode(Node* curr) const {
	Node* cache = curr;
	if (cache)
		while (cache->right)
			cache = cache->right;
	return cache;
}

template<typename Key, typename Info>
void Tree<Key, Info>::leftRotate(Node*& A) {
	Node* B = A->right;
	Node* x1 = B->left;
	//rotation
	B->up = A->up;
	B->left = A;
	A->up = B;
	A->right = x1;
	if (x1)
		x1->up = A;
	//new heights
	A->height = std::max(getHeight(A->left), getHeight(A->right)) + 1;
	B->height = std::max(getHeight(B->left), getHeight(B->right)) + 1;
	//update the A
	A = B;
}

template<typename Key, typename Info>
void Tree<Key, Info>::rightRotate(Node*& A) {
	Node* B = A->left;
	Node* x1 = B->right;
	//rotation
	B->right = A;
	B->up = A->up;
	A->up = B;
	A->left = x1;
	if (x1)
		x1->up = A;
	//new heights
	A->height = std::max(getHeight(A->left), getHeight(A->right)) + 1;
	B->height = std::max(getHeight(B->left), getHeight(B->right)) + 1;
	//update the A
	A = B;
}

template<typename Key, typename Info>
void Tree<Key, Info>::insert(Node*& curr, const Key& key, const Info& info, Node*& newNode) {
	//We do need to insert the node and balance the tree.
	if (!curr) {
		curr = new Node(key, info);
		newNode = curr;
		return;
	}
	if (curr->m_key == key) {
		newNode = curr;
		throw std::invalid_argument("This element already exist!");
	}
	if (curr->m_key < key) {
		if (!curr->right) {
			insert(curr->right, key, info, newNode);
			curr->right->up = curr;
		}
		else {
			insert(curr->right, key, info, newNode);
		}
		//firstly we need to update the height of a current node
		curr->height = std::max(getHeight(curr->left), getHeight(curr->right)) + 1;
		//then we calculate the balance factor of the curr node
		int balance = getBalance(curr);
		//Right-right case.
		if (balance < -1 and key > curr->right->m_key) {
			leftRotate(curr);
			return;
		}
		//Right-left case.
		if (balance < -1 and key < curr->right->m_key) {
			rightRotate(curr->right);
			leftRotate(curr);
			return;
		}
	}
	else if (curr->m_key > key) {
		if (!curr->left) {
			insert(curr->left, key, info, newNode);
			curr->left->up = curr;
		}
		else {
			insert(curr->left, key, info, newNode);
		}
		//firstly we need to update the height of a current node
		curr->height = std::max(getHeight(curr->left), getHeight(curr->right)) + 1;
		//then we calculate the balance factor of the curr node
		int balance = getBalance(curr);
		//Left-left case.
		if (balance > 1 and key < curr->left->m_key) {
			rightRotate(curr);
			return;
		}
		//Left-right
		if (balance > 1 and key > curr->left->m_key) {
			leftRotate(curr->left);
			rightRotate(curr);
			return;
		}
	}
}

template<typename Key, typename Info>
typename Tree<Key, Info>::Node* Tree<Key, Info>::remove(Node* curr, const Key& key) {
	//simple way of finding a node.
	if (!curr)
		throw false;
	if (curr->m_key > key)
		curr->left = remove(curr->left, key);
	else if (curr->m_key < key)
		curr->right = remove(curr->right, key);
	else {
		//if the node has only one child we can simply delete it like that:
		if (curr->left == nullptr or curr->right == nullptr) {
			Node* cache = nullptr;
			if (curr->left)
				cache = curr->left;
			else
				cache = curr->right;
			//when node has no child case(1)
			if (!cache) {
				cache = curr;
				//curr will simple be a nullptr
				curr = nullptr;
			}
			else { //case(2)
				//if there is a child we do need to copy the content of cache to curr
				curr->m_key = cache->m_key; curr->m_info = cache->m_info;
				curr->left = nullptr; curr->right = nullptr;
			}
			delete cache;
		}
		else {  //case(3)	
			//otherwise we do need to find a minimal node of the right subtree
			Node* cache = getMinimalValueNode(curr->right);
			//current node becames the minimal node of the right subtree
			curr->m_key = cache->m_key; curr->m_info = cache->m_info;
			//the problem here is that we do need to delete the cache node
			//currently we do have 2 nodes with cache in it 
			//unfortunately to delete the cache we need to inform the parent
			//the easiest way to do this is to run the remove method again and simply delete the
			//cache key
			curr->right = remove(curr->right, cache->m_key);
		}
	}
	//we do need to make sure that curr exist
	if (!curr)
		return nullptr;
	//after deleting we do need to update the height and obtain a new balance
	curr->height = 1 + std::max(getHeight(curr->left), getHeight(curr->right));
	int balance = getBalance(curr);
	//finally we do need to rebalance the tree if it is necessary
	if (balance > 1 and //as always we do need to balance if balance factor > 1 or < 1
		getBalance(curr->left) >= 0) {
		//left-left case bacause left subtree balance is >= 0
		rightRotate(curr);
		return curr;
	}
	else if (balance > 1 and getBalance(curr->left) < 0) {
		//left-right case bacause left subtree balance is < 0 (right subtree of the left subtree is grater)
		leftRotate(curr->left);
		rightRotate(curr);
		return curr;
	}
	else if (balance < -1 and getBalance(curr->right) <= 0) {
		//right-right case bacause (same as prev) plus balance is negative
		leftRotate(curr);
		return curr;
	}
	else if (balance < -1 and getBalance(curr->right) > 0) {
		//right-left case (same as before)
		rightRotate(curr->right);
		leftRotate(curr);
		return curr;
	}
	else {
		//if there are no roatations we simply output curr node.
		return curr;
	}
}

template<typename Key, typename Info>
bool Tree<Key, Info>::compareNodes(Node* a, Node* b) const {
	if (a == b)
		return true;
	if (a == nullptr)
		return false;
	if (b == nullptr)
		return false;
	return (a->m_key == b->m_key) and
		(a->m_info == b->m_info) and
		compareNodes(a->right, b->right) and
		compareNodes(a->left, b->left);
}

template<typename Key, typename Info>
template<typename ToDo>
void Tree<Key, Info>::uniTraversal(Node* curr, ToDo method, const TravelType type) const {
	if (curr) {
		if (type == TravelType::Preorder) {
			method(const_iterator(curr, this));
		}
		uniTraversal(curr->left, method, type);
		if (type == TravelType::Inorder) {
			method(const_iterator(curr, this));
		}
		uniTraversal(curr->right, method, type);
		if (type == TravelType::Postorder) {
			method(const_iterator(curr, this));
		}
	}
}

template<typename Key, typename Info>
Tree<Key, Info>::iterator::iterator(Node* x, const Tree<Key, Info>* m) : master(m), curr(x) {}

template<typename Key, typename Info>
Tree<Key, Info>::iterator::iterator() : master(nullptr), curr(nullptr) {}

template<typename Key, typename Info>
Tree<Key, Info>::iterator::iterator(const iterator& x) : master(x.master), curr(x.curr) {}

template<typename Key, typename Info>
bool Tree<Key, Info>::iterator::operator==(const iterator& x) const {
	return curr == x.curr and master == x.master;
}

template<typename Key, typename Info>
bool Tree<Key, Info>::iterator::operator!=(const iterator& x) const {
	return curr != x.curr or master != x.master;
}

template<typename Key, typename Info>
const typename Tree<Key, Info>::iterator& Tree<Key, Info>::iterator::operator++() const {
	if (!master) {
		throw "Master does not exist!";
	}
	//current is null
	if (!curr) {
		curr = master->head;
		if (!curr) {
			throw "Master is empty!";
		}
		//finding the smallest value of a subtree
		while (curr->left) {
			curr = curr->left;
		}
	}
	else {
		if (curr->right) {
			curr = curr->right;
			//finding the smallest value of a subtree
			while (curr->left) {
				curr = curr->left;
			}
		}
		else {
			Node* prev = curr->up;
			while (prev != nullptr and curr == prev->right) {
				curr = prev;
				prev = prev->up;
			}
			curr = prev;
		}
	}
	return *this;
}

template<typename Key, typename Info>
typename Tree<Key, Info>::iterator Tree<Key, Info>::iterator::operator++(int) const {
	iterator cache(*this);
	operator++();
	return cache;
}

template<typename Key, typename Info>
const typename Tree<Key, Info>::iterator& Tree<Key, Info>::iterator::operator--() const {
	if (!master) {
		throw "Master does not exist!";
	}
	//current is null
	if (!curr) {
		curr = master->head;
		if (!curr) {
			throw "Master is empty!";
		}
		//finding the gratest value of a subtree
		while (curr->right) {
			curr = curr->right;
		}
	}
	else {
		if (curr->left) {
			curr = curr->left;
			//finding the gratest value of a subtree
			while (curr->right) {
				curr = curr->right;
			}
		}
		else {
			Node* prev = curr->up;
			while (prev != nullptr and curr == prev->left) {
				curr = prev;
				prev = prev->up;
			}
			curr = prev;
		}
	}
	return *this;
}

template<typename Key, typename Info>
typename Tree<Key, Info>::iterator Tree<Key, Info>::iterator::operator--(int) const {
	iterator cache(*this);
	operator--();
	return cache;
}

template<typename Key, typename Info>
const Info& Tree<Key, Info>::iterator::operator*() const {
	if (!curr)
		throw "Iterator does not have a value!";
	return curr->m_info;
}

template<typename Key, typename Info>
Info& Tree<Key, Info>::iterator::operator*() {
	if (!curr)
		throw "Iterator does not have a value!";
	return curr->m_info;
}

template<typename Key, typename Info>
const Key& Tree<Key, Info>::iterator::getKey() const {
	if (!curr)
		throw "Iterator does not have a value!";
	return curr->m_key;
}

template<typename Key, typename Info>
const Info& Tree<Key, Info>::iterator::getInfo() const {
	if (!curr)
		throw "Iterator does not have a value!";
	return curr->m_info;
}

template<typename Key, typename Info>
bool Tree<Key, Info>::iterator::isNull() const {
	return !curr;
}

template<typename Key, typename Info>
Tree<Key, Info>::reverse_iterator::reverse_iterator(Node* x, const Tree<Key, Info>* m) : iterator(x, m) {}

template<typename Key, typename Info>
Tree<Key, Info>::reverse_iterator::reverse_iterator() : iterator() {}

template<typename Key, typename Info>
Tree<Key, Info>::reverse_iterator::reverse_iterator(const reverse_iterator& x) : iterator(x.curr, x.master) {}

template<typename Key, typename Info>
const typename Tree<Key, Info>::reverse_iterator& Tree<Key, Info>::reverse_iterator::operator++() const {
	iterator::operator--();
	return *this;
}

template<typename Key, typename Info>
typename Tree<Key, Info>::reverse_iterator Tree<Key, Info>::reverse_iterator::operator++(int) const {
	reverse_iterator cache(*this);
	iterator::operator--();
	return cache;
}

template<typename Key, typename Info>
const typename Tree<Key, Info>::reverse_iterator& Tree<Key, Info>::reverse_iterator::operator--() const {
	iterator::operator++();
	return *this;
}

template<typename Key, typename Info>
typename Tree<Key, Info>::reverse_iterator Tree<Key, Info>::reverse_iterator::operator--(int) const {
	reverse_iterator cache(*this);
	iterator::operator++();
	return cache;
}

template<typename Key, typename Info>
typename Tree<Key, Info>::iterator Tree<Key, Info>::begin() const {
	return iterator(getMinimalValueNode(head), this);
}

template<typename Key, typename Info>
typename Tree<Key, Info>::iterator Tree<Key, Info>::end() const {
	return iterator(nullptr, this);
}

template<typename Key, typename Info>
typename Tree<Key, Info>::reverse_iterator Tree<Key, Info>::rbegin() const {
	return reverse_iterator(getMaximalValueNode(head), this);
}

template<typename Key, typename Info>
typename Tree<Key, Info>::reverse_iterator Tree<Key, Info>::rend() const {
	return reverse_iterator(nullptr, this);
}

template<typename Key, typename Info>
Tree<Key, Info>::Tree() : head(nullptr)
{}

template<typename Key, typename Info>
Tree<Key, Info>::Tree(const Tree<Key, Info>& x) : head(nullptr)
{
	head = copy(x.head);
}

template<typename Key, typename Info>
Tree<Key, Info>::~Tree()
{
	clear(head);
}

template<typename Key, typename Info>
int Tree<Key, Info>::getHeight() const {
	return getHeight(head);
}

template<typename Key, typename Info>
bool Tree<Key, Info>::empty() const {
	return head == nullptr;
}

template<typename Key, typename Info>
void Tree<Key, Info>::clear() {
	clear(head);
}

template<typename Key, typename Info>
Tree<Key, Info>& Tree<Key, Info>::operator=(const Tree<Key, Info>& x) {
	if (this != &x) {
		clear(head);
		head = copy(x.head);
	}
	return *this;
};

template<typename Key, typename Info>
bool Tree<Key, Info>::operator==(const Tree<Key, Info>& x) const {
	return compareNodes(head, x.head);
};

template<typename Key, typename Info>
bool Tree<Key, Info>::operator!=(const Tree<Key, Info>& x) const {
	return !(*this == x);
};

template<typename Key, typename Info>
bool Tree<Key, Info>::insert(const Key& key, const Info& info)
{
	try {
		Node* node;
		insert(head, key, info, node);
	}
	catch (...) {
		//element already in the tree.
		return false;
	}
	return true;
}

template<typename Key, typename Info>
bool Tree<Key, Info>::insert(const Key& key, const Info& info, iterator& it)
{
	Node* node = nullptr;
	try {
		insert(head, key, info, node);
	}
	catch (...) {
		//element already in the tree.
		it = iterator(node, this);
		return false;
	}
	it = iterator(node, this);
	return true;
}

template<typename Key, typename Info>
bool Tree<Key, Info>::remove(const Key& key) {
	try {
		head = remove(head, key);
	}
	catch (...) {
		//element not found
		return false;
	}
	return true;
}

template<typename Key, typename Info>
template<typename ToDo>
void Tree<Key, Info>::executeForAllNodes(ToDo method, Tree<Key, Info>::TravelType type) const {
	uniTraversal(head, method, type);
}

template<typename Key, typename Info>
void Tree<Key, Info>::traversal(const TravelType type, std::ostream& out) const {
	uniTraversal(head,
		[&out](Node* curr)->void
		{
			out << curr->m_key << "_" << curr->m_info << std::endl;
		},
		type);
}

template<typename Key, typename Info>
void Tree<Key, Info>::preorder(std::ostream& out) const {
	uniTraversal(head,
		[&out](const_iterator& curr)->void
		{
			out << "Key:" << curr.getKey() << ", info: " << curr.getInfo() << std::endl;
		},
		TravelType::Preorder);
}

template<typename Key, typename Info>
void Tree<Key, Info>::inorder(std::ostream& out) const {
	uniTraversal(head,
		[&out](const_iterator& curr)->void
		{
			out << "Key:" << curr.getKey() << ", info: " << curr.getInfo() << std::endl;
		},
		TravelType::Inorder);
}

template<typename Key, typename Info>
void Tree<Key, Info>::postorder(std::ostream& out) const {
	uniTraversal(head,
		[&out](const_iterator& curr)->void
		{
			out << "Key:" << curr.getKey() << ", info: " << curr.getInfo() << std::endl;
		},
		TravelType::Postorder);
}

template<typename Key, typename Info>
typename Tree<Key, Info>::iterator Tree<Key, Info>::find(const Key& key) const {
	iterator x;
	try {
		uniTraversal(head,
			[&x, key](const_iterator& node)
			{
				//we do not want to access an empty node.
				if (!node.isNull()) {
					if (node.getKey() == key) {
						x = node;
						//a simple way to stop the recursion
						throw true;
					}
				}
			},
			TravelType::Preorder);
	}//we do not need to do anything in case of catching an exception
	catch (...) {}
	return x;
}