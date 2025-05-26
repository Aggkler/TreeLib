#pragma once
#include <iostream>
using namespace std;

template <typename T>
class BTreeNode {
public:
    T* keys;
    int t;
    BTreeNode** children;
    int n;
    bool leaf;

    BTreeNode(bool isLeaf, int minDegree) {
        t = minDegree;
        leaf = isLeaf;
        keys = new T[2 * t - 1];
        children = new BTreeNode * [2 * t];
        n = 0;
    }

    ~BTreeNode() {
        delete[] keys;
        delete[] children;
    }
};

template <typename T>
class BTree {
private:
    BTreeNode<T>* root;
    int t;

    void traverse(BTreeNode<T>* node);
    BTreeNode<T>* search(BTreeNode<T>* node, T k);
    void splitChild(BTreeNode<T>* x, int i);
    void insertNonFull(BTreeNode<T>* node, T k);
    void clear(BTreeNode<T>* node);
    void printRecursive(BTreeNode<T>* node, int indent);

public:
    BTree(int minDegree) {
        root = nullptr;
        t = minDegree;
    }

    ~BTree() {
        clear(root);
    }
    void traverse();
    BTreeNode<T>* search(T k);
    void insert(T k);
    void print();
};

template<typename T>
inline void BTree<T>::traverse(BTreeNode<T>* node) {
    int i;
    for (i = 0; i < node->n; i++) {
        if (!node->leaf)
            traverse(node->children[i]);
        cout << " " << node->keys[i];
    }
    if (!node->leaf)
        traverse(node->children[i]);
}

template<typename T>
inline BTreeNode<T>* BTree<T>::search(BTreeNode<T>* node, T k) {
    int i = 0;
    while (i < node->n && k > node->keys[i])
        i++;

    if (i < node->n && node->keys[i] == k)
        return node;

    if (node->leaf)
        return nullptr;

    return search(node->children[i], k);
}

template<typename T>
inline void BTree<T>::splitChild(BTreeNode<T>* x, int i) {
    BTreeNode<T>* y = x->children[i];
    BTreeNode<T>* z = new BTreeNode<T>(y->leaf, t);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    if (!y->leaf) {
        for (int j = 0; j < t; j++)
            z->children[j] = y->children[j + t];
    }

    y->n = t - 1;

    for (int j = x->n; j >= i + 1; j--)
        x->children[j + 1] = x->children[j];

    x->children[i + 1] = z;

    for (int j = x->n - 1; j >= i; j--)
        x->keys[j + 1] = x->keys[j];

    x->keys[i] = y->keys[t - 1];
    x->n++;
}

template<typename T>
inline void BTree<T>::insertNonFull(BTreeNode<T>* node, T k) {
    int i = node->n - 1;

    if (node->leaf) {
        while (i >= 0 && k < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        node->keys[i + 1] = k;
        node->n++;
    }
    else {
        while (i >= 0 && k < node->keys[i])
            i--;

        i++;
        if (node->children[i]->n == 2 * t - 1) {
            splitChild(node, i);
            if (k > node->keys[i])
                i++;
        }
        insertNonFull(node->children[i], k);
    }
}

template<typename T>
inline void BTree<T>::clear(BTreeNode<T>* node) {
    if (!node) return;

    if (!node->leaf) {
        for (int i = 0; i <= node->n; i++)
            clear(node->children[i]);
    }
    delete node;
}

template<typename T>
inline void BTree<T>::traverse() {
    if (root != nullptr)
        traverse(root);
    else
        cout << "Tree is empty\n";
}

template<typename T>
inline BTreeNode<T>* BTree<T>::search(T k){
    return (root == nullptr) ? nullptr : search(root, k);
}

template<typename T>
inline void BTree<T>::insert(T k) {
    if (root == nullptr) {
        root = new BTreeNode<T>(true, t);
        root->keys[0] = k;
        root->n = 1;
    }
    else {
        if (root->n == 2 * t - 1) {
            BTreeNode<T>* s = new BTreeNode<T>(false, t);
            s->children[0] = root;
            splitChild(s, 0);
            int i = (s->keys[0] < k) ? 1 : 0;
            insertNonFull(s->children[i], k);
            root = s;
        }
        else {
            insertNonFull(root, k);
        }
    }
}

template<typename T>
inline void BTree<T>::print() {
    printRecursive(root, 0);
}

template<typename T>
inline void BTree<T>::printRecursive(BTreeNode<T>* node, int indent) {
    if (!node) return;

    for (int i = 0; i < indent; ++i)
        cout << "  ";

    cout << "[";
    for (int i = 0; i < node->n; ++i) {
        cout << node->keys[i];
        if (i != node->n - 1) cout << " ";
    }
    cout << "]\n";

    if (!node->leaf) {
        for (int i = 0; i <= node->n; ++i)
            printRecursive(node->children[i], indent + 1);
    }
}