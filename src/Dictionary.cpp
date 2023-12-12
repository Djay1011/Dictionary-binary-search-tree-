#include "Dictionary.h"

Dictionary::Dictionary() : root(nullptr) {}


void Dictionary::insert(int key, const std::string& item) {
    root = insertWorker(root, key, item);
}

// Recursive worker for insert
Dictionary::Node* Dictionary::insertWorker(Node* node, int key, const std::string& item) {
    if (node == nullptr) {
        return new Node(key, item);
    }

    if (key < node->key) {
        node->left = insertWorker(node->left, key, item);
    }
    else if (key > node->key) {
        node->right = insertWorker(node->right, key, item);
    }
    else {
        // Key already exists, update the item
        node->item = item;
    }
    return node;
}


std::string* Dictionary::lookup(int key) {
    return lookupWorker(root, key);
}

std::string* Dictionary::lookupWorker(Node* currentNode, int key) {
    if (currentNode == nullptr) {
        return nullptr; // Key not found
    }

    if (key == currentNode->key) {
        return &(currentNode->item); // Key found
    }
    else if (key < currentNode->key) {
        return lookupWorker(currentNode->left, key); // Search in the left subtree
    }
    else {
        return lookupWorker(currentNode->right, key); // Search in the right subtree
    }
}

void Dictionary::displayEntries() {
    displayEntriesWorker(root);  // (1) Start the traversal from the root
}

void Dictionary::displayEntriesWorker(Node* currentNode) {
    if (currentNode == nullptr) return;

    std::cout << "Key: " << currentNode->key << ", Item: " << currentNode->item << std::endl;
    displayEntriesWorker(currentNode->left);  // (2) Traverse the left subtree
    displayEntriesWorker(currentNode->right);  // (3) Traverse the right subtree
}

void Dictionary::displayTree() {
    displayTreeWorker(root, 0);
}

void Dictionary::displayTreeWorker(Node* node, int depth) {
    if (node == nullptr) {
        printIndent(depth);
        std::cout << "LEAF" << std::endl;
        return;
    }

    displayTreeWorker(node->left, depth + 1); // Traverse left subtree

    printIndent(depth);
    std::cout << "Key: " << node->key << ", Item: " << node->item << std::endl; // Display current node

    displayTreeWorker(node->right, depth + 1); // Traverse right subtree
}

void Dictionary::printIndent(int depth) {
    for (int i = 0; i < depth; ++i) {
        std::cout << "  "; // Two spaces for each level of depth
    }
}

void Dictionary::remove(int key) {
    root = removeWorker(root, key);
}

Dictionary::Node* Dictionary::removeWorker(Node* node, int key) {
    if (node == nullptr) {
        return nullptr; // Key not found
    }

    if (key < node->key) {
        node->left = removeWorker(node->left, key);
    }
    else if (key > node->key) {
        node->right = removeWorker(node->right, key);
    }
    else {
        // Node with two children
        if (node->left != nullptr && node->right != nullptr) {
            Node* successor = findAndDetachMinNode(node->right);
            node->key = successor->key;
            node->item = successor->item;
            node->right = removeWorker(node->right, successor->key);
        }
        else {
            // Node with one or no child
            Node* temp = (node->left != nullptr) ? node->left : node->right;
            delete node;
            return temp;
        }
    }
    return node;
}

Dictionary::Node* Dictionary::findAndDetachMinNode(Node* node) {
    if (node->left == nullptr) {
        return node;
    }
    else {
        return findAndDetachMinNode(node->left);
    }
}

Dictionary::~Dictionary() {
    deepDeleteWorker(root);
}

void Dictionary::deepDeleteWorker(Node* node) {
    if (node != nullptr) {
        deepDeleteWorker(node->left);  // Delete left subtree
        deepDeleteWorker(node->right); // Delete right subtree
        delete node;              // Delete the current node
    }
}


Dictionary::Dictionary(const Dictionary& other)
{
    root = copyTree(other.root);
}

Dictionary::Node* Dictionary::copyTree(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }

    Node* newNode = new Node(node->key, node->item);
    newNode->left = copyTree(node->left);
    newNode->right = copyTree(node->right);
    return newNode;
}

Dictionary::Node* Dictionary::rotateRight(Node* a) {
    Node* b = a->left;
    Node* beta = b->right;

    // Perform rotation
    b->right = a;
    a->left = beta;

    // Return new root of this subtree
    return b;
}

Dictionary::Node* Dictionary::rotateLeft(Node* a) {
    Node* b = a->right;
    Node* beta = b->left;

    // Perform rotation
    b->left = a;
    a->right = beta;

    // Return new root of this subtree
    return b;
}

void Dictionary::testRotations() {
    // Hardcoded test of rotations on specific nodes
    // Note: This requires knowledge of the tree structure and the nodes you want to rotate
    // For example:
    root = rotateRight(root); // Rotate right at root
    root = rotateLeft(root);  // Then rotate left at root

    displayTree(); // Display the tree to check the results
}

Dictionary::Dictionary(Dictionary&& other)
    : root(other.root) { // Transfer ownership of the internal tree
    other.root = nullptr; // Leave the source object in a valid state
}

Dictionary& Dictionary::operator=(const Dictionary& other) {
    if (this != &other) { // Check for self-assignment
        deepDeleteWorker(root); // Deallocate current tree
        root = copyTree(other.root); // Deep copy the tree from 'other'
    }
    return *this; // Return a reference to the current object
}

Dictionary& Dictionary::operator=(Dictionary&& other) {
    if (this != &other) { // Check for self-assignment
        deepDeleteWorker(root); // Deallocate current tree

        // Transfer ownership of resources
        root = other.root;
        other.root = nullptr; // Set the source object's pointer to nullptr
    }
    return *this; // Return a reference to the current object
}

void Dictionary::collectKeysToRemove(Node* node, std::function<bool(int)> predicate, std::vector<int>& keysToRemove) {
    if (node == nullptr) {
        return;
    }

    // Check the predicate for the current node
    if (predicate(node->key)) {
        keysToRemove.push_back(node->key);
    }

    // Continue in the left and right subtrees
    collectKeysToRemove(node->left, predicate, keysToRemove);
    collectKeysToRemove(node->right, predicate, keysToRemove);
}

void Dictionary::removeIf(std::function<bool(int)> predicate) {
    std::vector<int> keysToRemove;
    collectKeysToRemove(root, predicate, keysToRemove);

    // Now remove the nodes with the collected keys
    for (int key : keysToRemove) {
        remove(key); // Assuming you have a 'remove(int key)' function
    }
}