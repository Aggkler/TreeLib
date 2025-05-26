#pragma once
#include "ITree.h"
#include <iostream>

template<typename T>
class AVLTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        int height;
        Node(T val) : data(val), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    int height(Node* node);
    int balanceFactor(Node* node);
    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);
    Node* insert(Node* node, T key);
    void print(Node* node, const std::string& label = "Root", int indent = 0) const;
    Node* remove(Node* node, const T& key);
    Node* minValueNode(Node* node);

public:
    AVLTree() : root(nullptr) {};
    void insert(const T& value);
    void remove(const T& value);
    bool search(const T& value) const;
    void print() const;
};

template<typename T>
int AVLTree<T>::height(Node* node) {
    return node ? node->height : 0;
}

template<typename T>
int AVLTree<T>::balanceFactor(Node* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::rotateRight(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    x->right = y;

    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;

    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::insert(Node* node, T key) {
    if (!node) return new Node(key);

    if (key < node->data) node->left = insert(node->left, key);
    else if (key > node->data) node->right = insert(node->right, key);
    else return node;

    node->height = 1 + std::max(height(node->left), height(node->right));

    int balance = balanceFactor(node);

    if (balance > 1 && key < node->left->data) return rotateRight(node);
    if (balance < -1 && key > node->right->data) return rotateLeft(node);
    if (balance > 1 && key > node->left->data) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && key < node->right->data) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

template<typename T>
void AVLTree<T>::insert(const T& value) {
    root = insert(root, value);
}

template<typename T>
bool AVLTree<T>::search(const T& value) const {
    Node* current = root;
    while (current) {
        if (value == current->data) return true;
        current = (value < current->data) ? current->left : current->right;
    }
    return false;
}

template<typename T>
void AVLTree<T>::print(Node* node, const std::string& label, int indent) const {
    if (!node) return;

    std::cout << std::string(indent, ' ') << label << ": " << node->data << std::endl;

    print(node->left, "L", indent + 4);
    print(node->right, "R", indent + 4);
}

template<typename T>
void AVLTree<T>::print() const {
    if (!root) {
        std::cout << "(пусто)" << std::endl;
        return;
    }

    std::cout << "Root: " << root->data << std::endl;
    print(root->left, "L", 4);
    print(root->right, "R", 4);
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::remove(Node* node, const T& key) {
    if (!node) return nullptr;

    // Поиск ключа
    if (key < node->data) {
        node->left = remove(node->left, key);
    }
    else if (key > node->data) {
        node->right = remove(node->right, key);
    }
    else {
        // Найден узел для удаления
        Node* rightChild = node->right;

        if (!node->right) {
            Node* leftChild = node->left;
            delete node;
            return leftChild;
        }
        else if (!node->left) {
            delete node;
            return rightChild;
        }
        else {
            while (rightChild->left)
                rightChild = rightChild->left;

            node->data = rightChild->data;
            node->right = remove(node->right, rightChild->data);
        }
    }

    if (!node) return node;

    node->height = 1 + std::max(height(node->left), height(node->right));

    int balance = height(node->left) - height(node->right);

    if (balance > 1 && height(node->left) >= height(node->right))
        return rotateRight(node);

    if (balance > 1 && height(node->left) < height(node->right)) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && height(node->right) >= height(node->left))
        return rotateLeft(node);

    if (balance < -1 && height(node->right) < height(node->left)) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}


template<typename T>
void AVLTree<T>::remove(const T& value) {
    root = remove(root, value);
}
