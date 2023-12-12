#pragma once
#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <iostream>
#include <functional>
#include <vector>

class Dictionary {
public:
    Dictionary();  // Default constructor declaration
    ~Dictionary();  // Destructor declaration

    Dictionary(const Dictionary &); // Copy Constructor
    Dictionary(Dictionary&&);// Move Constructor
    // Copy assignment operator
    Dictionary& operator=(const Dictionary& other);
    // Move assignment operator
    Dictionary& operator=(Dictionary&& other);

    void insert(int key, const std::string& item);
    std::string* lookup(int key);
    void displayEntries();
    void displayTree();
    void remove(int key);
    void testRotations(); // Temporary function for testing rotations
    void removeIf(std::function<bool(int)> predicate); // Higher-order function declaration
private:

    struct Node {
        int key;
        std::string item;
        Node* left;
        Node* right;

        Node(int key, const std::string& item, Node* next = nullptr)
            : key(key), item(item), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void displayEntriesWorker(Node* currentNode);
    void displayTreeWorker(Node* node, int depth);
    void printIndent(int depth);
    std::string* lookupWorker(Node* currentNode, int key);
    Node* insertWorker(Node* node, int key, const std::string& item);
    Node* removeWorker(Node* node, int key);
    Node* findAndDetachMinNode(Node* node);
    void deepDeleteWorker(Node*); // recursive worker performing deep delete
    Node* copyTree(Node*);
    Node* rotateLeft(Node* a);
    Node* rotateRight(Node* a);
    void collectKeysToRemove(Node* node, std::function<bool(int)> predicate, std::vector<int>& keysToRemove);
};

#endif // DICTIONARY_H