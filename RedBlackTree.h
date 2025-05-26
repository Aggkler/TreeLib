#pragma once

template<class K, class T>
class RedBlackTree {
private:
	enum color {
		BLACK,
		RED
	};
	
	struct rbNode {
		K     key;
		T     val;
		color	color;
		rbNode* parent;
		rbNode* left;
		rbNode* right;

		rbNode() :parent(nullptr), left(nullptr), right(nullptr), color(RED) {}
	};

	int   size;
	rbNode* root;

	int cmp(const K a, const K b) const;
	void leftRotate(rbNode* node);
	void rightRotate(rbNode* node);
	void removeNode(rbNode* node);
	void printHelper(rbNode* node, std::string indent, bool last);

public:
	RedBlackTree() : root(nullptr) {};
	void insert(K key, T val);
	bool remove(const K key);
	bool search(const K key, T val) const;
	void clear();
	int getSize() const;
	void print();
};

template<class K, class T>
void RedBlackTree<K, T>::insert(K key, T val) {
	rbNode* node = new rbNode;
	node->key = key;
	node->val = val;

	if (root == nullptr) {
		root = node;
		node->color = BLACK;
		return;
	}

	rbNode* curr = root;
	while (curr->left != nullptr || curr->right != nullptr)
	{
		if (cmp(key, curr->key) == -1)
		{
			if (curr->left)
				curr = curr->left;
			else
				break;
		}
		else
		{
			if (curr->right)
				curr = curr->right;
			else
				break;
		}
	}
	node->parent = curr;
	if (cmp(key, curr->key) == -1)
		curr->left = node;
	else
		curr->right = node;

	while (curr->color == RED && curr->parent != nullptr)
	{
		bool isRight = (curr == curr->parent->right);
		rbNode* uncle;
		if (isRight)
			uncle = curr->parent->left;
		else
			uncle = curr->parent->right;

		if (uncle == nullptr) {
			curr->color = BLACK;
			curr->parent->color = RED;
			if (uncle == curr->parent->right) {
				rightRotate(curr->parent);
			}
			else {
				leftRotate(curr->parent);
			}
			break;
		}
		else if (uncle->color == RED) {
			curr->color = BLACK;
			uncle->color = BLACK;
			curr->parent->color = RED;
			curr = curr->parent;
		}
		else {
			curr->color = BLACK;
			curr->parent->color = RED;

			if (isRight) {
				if (node == curr->left) {
					rightRotate(curr);
					curr = node;
				}
				leftRotate(curr->parent);
			}
			else {
				if (node == curr->right) {
					leftRotate(curr);
					curr = node;
				}
				rightRotate(curr->parent);
			}
		}
		root->color = BLACK;
	}

	this->size++;
}

template<class K, class T>
bool RedBlackTree<K, T>::remove(const K key) {
	auto curr = root;
	while (curr->left != nullptr | curr->right != nullptr)
	{
		if (curr->key == key)
			break;

		if (cmp(key, curr->key) >= 0)
			curr = curr->right;
		else
			curr = curr->left;
	}
	if (curr->key != key)
		return 0;

	this->removeNode(curr);
	(this->size)--;
	return 1;
}

template<class K, class T>
void RedBlackTree<K, T>::removeNode(rbNode* node) {
	if (node->color == RED) {
		if (node->left != nullptr && node->right != nullptr) {
			auto successor = node->right;
			while (successor->left != nullptr)
				successor = successor->left;
			node->key = successor->key;
			node->val = successor->val;
			this->removeNode(successor);
		}
		else if (node->left != nullptr) {
			node->key = node->left->key;
			node->val = node->left->val;
			node->color = node->left->color;
			this->removeNode(node->left);
		}
		else if (node->right != nullptr) {
			node->key = node->right->key;
			node->val = node->right->val;
			node->color = node->right->color;
			this->removeNode(node->right);
		}
		else {
			if (node->parent == nullptr) {
				root = nullptr;
				delete(node);
				return;
			}

			if (node->parent->left == node)
				node->parent->left = nullptr;
			else
				node->parent->right = nullptr;

			delete(node);
		}
	}
	else {
		if (node->left != nullptr && node->right != nullptr) {
			auto successor = node->right;
			while (successor->left != nullptr)
				successor = successor->left;
			node->key = successor->key;
			node->val = successor->val;
			this->removeNode(successor);
		}
		else if (node->left != nullptr) {
			node->key = node->left->key;
			node->val = node->left->val;
			this->removeNode(node->left);
		}
		else if (node->right != nullptr) {
			node->key = node->right->key;
			node->val = node->right->val;
			this->removeNode(node->right);
		}
		else {
			if (node->parent == nullptr) {
				delete(node);
				root = nullptr;
				return;
			}

			if (node->parent->left == node) {
				node->parent->left = nullptr;
				if (node->parent->right != nullptr
					&& node->parent->right->color == RED) {
					node->parent->right->color = BLACK;
					leftRotate(node->parent);
				}
			}
			else {
				node->parent->right = nullptr;
				if (node->parent->left != nullptr
					&& node->parent->left->color == RED) {
					node->parent->left->color = BLACK;
					rightRotate(node->parent);
				}
			}

			if (node->parent->left == nullptr
				&& node->parent->right == nullptr
				&& node->parent->parent != nullptr) {
				rightRotate(node->parent->parent);
			}
			delete(node);
		}
	}
}

template<class K, class T>
bool RedBlackTree<K, T>::search(const K key, T val) const {
	auto curr = root;
	while (curr->left != nullptr | curr->right != nullptr)
	{
		if (curr->key == key) {
			val = curr->val;
			break;
		}

		if (cmp(key, curr->key) < 0)
			curr = curr->left;
		else
			curr = curr->right;
	}

	if (curr->key != key)
		return 0;
	return 1;
}

template<class K, class T>
int RedBlackTree<K, T>::cmp(const K a, const K b) const {
	if (typeid(a) == typeid(char*)) {
		char* x = (char*)a;
		char* y = (char*)b;
		return strcmp(x, y);
	}
	else {
		if (a < b) return -1;
		if (a == b) return 0;
		if (a > b) return 1;
	}
	return 0;
}

template<class K, class T>
void RedBlackTree<K, T>::leftRotate(rbNode* node) {
	auto temp = node->right;

	node->right = temp->left;
	if (temp->left != nullptr)
		temp->left->parent = node;
	temp->left = node;
	temp->parent = node->parent;
	node->parent = temp;

	if (root == node) {
		root = temp;
		return;
	}
	if (temp->parent->left == node)
		temp->parent->left = temp;
	else
		temp->parent->right = temp;
}

template<class K, class T>
void RedBlackTree<K, T>::rightRotate(rbNode* node) {
	auto temp = node->left;

	node->left = temp->right;
	if (temp->right != nullptr)
		temp->right->parent = node;
	temp->right = node;
	temp->parent = node->parent;
	node->parent = temp;

	if (root == node) {
		root = temp;
		return;
	}
	if (temp->parent->left == node)
		temp->parent->left = temp;
	else
		temp->parent->right = temp;
}

template<class K, class T>
int RedBlackTree<K, T>::getSize() const {
	return this->size;
}

template<class K, class T>
void RedBlackTree<K, T>::clear()
{
	delete this->root;
	this->root = nullptr;
}

template<class K, class T>
void RedBlackTree<K, T>::printHelper(rbNode* node, std::string indent, bool last) {
	if (node != nullptr) {
		std::cout << indent;
		if (last) {
			std::cout << "R----";
			indent += "     ";
		}
		else {
			std::cout << "L----";
			indent += "|    ";
		}

		std::string colorStr = (node->color == RED) ? "RED" : "BLACK";
		std::cout << node->key << "(" << colorStr << ")" << std::endl;

		printHelper(node->left, indent, false);
		printHelper(node->right, indent, true);
	}
}

template<class K, class T>
void RedBlackTree<K, T>:: print() {
	printHelper(root, "", true);
}