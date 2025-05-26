#pragma once
#include <iostream>
#include <memory>

template <typename T>
class SplayTree {
private:
    struct Node {
        T key;
        Node* left;
        Node* right;
        Node* parent;

        Node(T k, Node* l = nullptr, Node* r = nullptr, Node* p = nullptr)
            : key(k), left(l), right(r), parent(p) {}
    };

    Node* root;

    void setParent(Node* child, Node* parent);
    void keepParent(Node* v);
    void rotate(Node* parent, Node* child);
    Node* splay(Node* v);
    Node* find(Node* v, T key);
    std::pair<Node*, Node*> split(Node* root, T key) {
        if (root == nullptr) {
            return { nullptr, nullptr };
        }

        root = find(root, key);

        if (root->key == key) {
            setParent(root->left, nullptr);
            setParent(root->right, nullptr);
            return { root->left, root->right };
        }

        if (root->key < key) {
            Node* right = root->right;
            root->right = nullptr;
            setParent(right, nullptr);
            return { root, right };
        }
        else {
            Node* left = root->left;
            root->left = nullptr;
            setParent(left, nullptr);
            return { left, root };
        }
    }
    Node* merge(Node* left, Node* right);
    void clear(Node* node);
    void print(Node* node, int depth = 0) const;

public:
    SplayTree() : root(nullptr) {}
    ~SplayTree() { clear(root); }
    void insert(T key);
    void remove(T key);
    bool contains(T key);
    void print() const;
};

template<typename T>
inline void SplayTree<T>::setParent(Node* child, Node* parent) {
    if (child != nullptr) {
        child->parent = parent;
    }
}

template<typename T>
inline void SplayTree<T>::keepParent(Node* v) {
    setParent(v->left, v);
    setParent(v->right, v);
}

template<typename T>
inline void SplayTree<T>::rotate(Node* parent, Node* child) {
    Node* gparent = parent->parent;

    if (gparent != nullptr) {
        if (gparent->left == parent) {
            gparent->left = child;
        }
        else {
            gparent->right = child;
        }
    }

    if (parent->left == child) {
        parent->left = child->right;
        child->right = parent;
    }
    else {
        parent->right = child->left;
        child->left = parent;
    }

    keepParent(child);
    keepParent(parent);
    child->parent = gparent;
}

template<typename T>
inline SplayTree<T>::Node* SplayTree<T>::splay(Node* v) {
    if (v->parent == nullptr) {
        return v;
    }

    Node* parent = v->parent;
    Node* gparent = parent->parent;

    if (gparent == nullptr) {
        rotate(parent, v);
        return v;
    }
    else {
        bool zigzig = (gparent->left == parent) == (parent->left == v);
        if (zigzig) {
            rotate(gparent, parent);
            rotate(parent, v);
        }
        else {
            rotate(parent, v);
            rotate(gparent, v);
        }
        return splay(v);
    }
};

template<typename T>
inline SplayTree<T>::Node* SplayTree<T>::find(Node* v, T key) {
    if (v == nullptr) return nullptr;

    if (key == v->key) {
        return splay(v);
    }

    if (key < v->key && v->left != nullptr) {
        return find(v->left, key);
    }

    if (key > v->key && v->right != nullptr) {
        return find(v->right, key);
    }

    return splay(v);
}

template<typename T>
inline SplayTree<T>::Node* SplayTree<T>::merge(Node* left, Node* right) {
    if (right == nullptr) return left;
    if (left == nullptr) return right;

    right = find(right, left->key);
    right->left = left;
    left->parent = right;
    return right;
}

template<typename T>
inline void SplayTree<T>::clear(Node* node) {
    if (node != nullptr) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

template<typename T>
inline void SplayTree<T>::insert(T key) {
    auto [left, right] = split(root, key);
    root = new Node(key, left, right);
    keepParent(root);
}

template<typename T>
inline void SplayTree<T>::remove(T key) {
    root = find(root, key);
    if (root != nullptr && root->key == key) {
        setParent(root->left, nullptr);
        setParent(root->right, nullptr);
        root = merge(root->left, root->right);
    }
}

template<typename T>
inline bool SplayTree<T>::contains(T key) {
    root = find(root, key);
    return root != nullptr && root->key == key;
}

template<typename T>
inline void SplayTree<T>::print(Node* node, int depth) const {
    if (node != nullptr) {
        print(node->right, depth + 1);
        std::cout << std::string(depth * 4, ' ') << node->key << std::endl;
        print(node->left, depth + 1);
    }
}

template<typename T>
inline void SplayTree<T>::print() const {
    print(root);
    std::cout << "----------------" << std::endl;
}