#include <iostream>
#include <cstdlib>

//Сразу скажу, что есть разные вариации декартового дерева, и для каждого из них своя node, например
//неявное декартово дерево хранит ключи(т.е ключ это размер поддерева), но это уже совсем другой тип,
//поэтому я его(и все другие не реализовывал)
class Node {
public:
    int key;
    int priority;
    Node *left, *right;
    
    Node(int keyToPaste);
};

//конструктор Node
Node::Node(int keyToPaste) {
    key = keyToPaste;
    priority = rand();
    left = nullptr;
    right = nullptr;
}

class Pair {
public:
    Node* first;
    Node* second;
    
    Pair(Node* f, Node* s);
};

Pair::Pair(Node* f, Node* s) {
    first = f;
    second = s;
}

class Treap {
public:
    Node* root = nullptr;

    // Вспомогательные методы
    Pair split(Node* tree, int x) {
        if (!tree) return Pair(nullptr, nullptr);
        
        if (tree->key <= x) {
            Pair newtree = split(tree->right, x);
            tree->right = newtree.first;
            return Pair(tree, newtree.second);
        } else {
            Pair newtree = split(tree->left, x);
            tree->left = newtree.second;
            return Pair(newtree.first, tree);
        }
    }

    Node* merge(Node* left, Node* right) {
        if (!left) return right;
        if (!right) return left;
        
        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            return left;
        } else {
            right->left = merge(left, right->left);
            return right;
        }
    }

    Node* deleteTree(Node* tree, int key) {
        if (!tree) return nullptr;
        
        if (key == tree->key) {
            Node* result = merge(tree->left, tree->right);
            delete tree;
            return result;
        }
        
        if (key < tree->key) {
            tree->left = deleteTree(tree->left, key);
        } else {
            tree->right = deleteTree(tree->right, key);
        }
        
        return tree;
    }

    void insert(int x) {
        Pair q = split(root, x);
        root = merge(q.first, merge(new Node(x), q.second));
    }

    bool search(int x) {
        Node* current = root;
        while (current != nullptr) {
            if (x == current->key) {
                return true;
            } else if (x < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return false;
    }

    //вообще использует это  delet, только его почему-то ещё наывают erase
    void erase(int key) {
        root = deleteTree(root, key);
    }

    // Обход дерева
    void inOrderTraversal(Node* node) {
        if (!node) return;
        inOrderTraversal(node->left);
        std::cout << node->key << " ";
        inOrderTraversal(node->right);
    }

    void print() {
        inOrderTraversal(root);
        std::cout << std::endl;
    }

    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }
};

int main() {
    //генератор случайных чисел
    srand(time(0)); 

    Treap treap;

    treap.insert(5);
    treap.insert(3);
    treap.insert(7);
    treap.insert(2);
    treap.insert(4);
    treap.insert(6);
    treap.insert(8);

    std::cout << "Проверим наше дерево ";
    treap.print();

    std::cout << "Есть 4?: " << (treap.search(4) ? "Да" : "Нет") << std::endl;
    std::cout << "Есть 10?: " << (treap.search(10) ? "Да" : "Нет") << std::endl;

    treap.erase(4);
    std::cout << "Дерево после удаления 4: ";
    treap.print();

    treap.erase(7);
    std::cout << "Дерево после удаления 7: ";
    treap.print();

    return 0;
}