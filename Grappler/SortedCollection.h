//
//  SortedCollection.h
//  Climber
//
//  Created by Ethan Coeytaux on 7/21/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef Climber_SortedCollection_h
#define Climber_SortedCollection_h

#include <string>

template<typename T>
class SortedCollection {
public:
    struct Node {
        Node(T * element, int value, SortedCollection * parent, std::string (*toString)(const T *)) : element(element), value(value), prev(nullptr), next(nullptr), parent(parent), toString(toString) { }
        
        T * element;
        int value;
        
        struct Node * prev;
        struct Node * next;
        
        SortedCollection * parent;
        
        std::string (*toString)(const T *);
    };
    
    SortedCollection() : size(0), headNode(nullptr), tailNode(nullptr) { }
    
    virtual ~SortedCollection() {
        clear();
    }
    
    int getSize() const {
        return size;
    }
    
    Node * getHeadNode() {
        return headNode;
    }
    
    Node * getTailNode() {
        return tailNode;
    }
    
    //returns first node that is equal to or greater than value
    Node * getNodeByValue(int value) {
        for (Node * traversalNode = headNode; traversalNode != nullptr; traversalNode = traversalNode->next) {
            if (traversalNode->value >= value) return traversalNode; //returns first node in range of dP
        }
        return NULL; //if no node is within range
    }
    
    Node * addElement(T * element, int value, std::string (*toString)(const T *)) {
        Node * node = new Node(element, value, this, toString);
        return addNode(node);
    }
    
    Node * addNode(Node * node) {
        if (headNode == nullptr) {
            headNode = node;
            tailNode = node;
        } else {
            //insert at back
            tailNode->next = node;
            node->prev = tailNode;
            tailNode = node;
            floatNode(node);
            
            //insert at front
            //headNode->prev = newNode;
            //newNode->next = headNode;
            //headNode = newNode;
            //sinkNode(newNode);
        }
        
        size++;
        return node;
    }
    
    bool removeElement(T * element) {
        if (size == 0) {
            printf("SortedCollection Error: attempted to remove element from empty list\n");
            return false;
        }
        
        for (Node * traversalNode = headNode; traversalNode != nullptr; traversalNode = traversalNode->next) {
            if (traversalNode->element == element)
                return removeNode(traversalNode);
        }
        printf("SortedCollection Error: element to remove could not be found in list\n");
        
        return false;
    }
    
    bool removeNode(Node * node) {
        if (node->parent != this) {
            printf("SortedCollection Error: attempted to remove node from non-parent collection\n");
            return false;
        }
        
        if (node->prev == nullptr) { //node is head
            if (node->next == nullptr) { //node is only node
                headNode = tailNode = nullptr;
            } else {
                node->next->prev = nullptr;
                headNode = node->next;
            }
        } else if (node->next == nullptr) { //node is tail
            node->prev->next = nullptr;
            tailNode = node->prev;
        } else {
            Node * tempNode = node->prev;
            node->next->prev = node->prev;
            tempNode->next = node->next;
        }
        
        delete node;
        size--;
        
        return true;
    }
    
    bool clear() {
        Node * traversalNode = headNode;
        while (traversalNode != nullptr) {
            Node * nodeToDelete = traversalNode;
            traversalNode = traversalNode->next;
            delete nodeToDelete;
        }
        headNode = tailNode = nullptr;
        
        size = 0;
        return true;
    }
    
    bool updateElementValue(T * element, int value) {
        for (Node * traversalNode = headNode; traversalNode != nullptr; traversalNode = traversalNode->next) {
            if (traversalNode->element == element)
                return updateValue(traversalNode, value);
        }
        printf("SortedCollection Error: element to update could not be found in list\n");
        
        return false;
    }
    
    bool updateNodeValue(Node * node, int value) {
        if (node->parent != this) {
            printf("SortedCollection Error: attempted to update node from non-parent collection\n");
            return false;
        }
        
        node->value = value;
        return (floatNode(node) && sinkNode(node));
    }
    
    //TODO download for mom - tony braxton don't break my heart - breathe again - yesterday
    
    void iterate(void (*func)(T *)) {
        for (Node * traversalNode = headNode; traversalNode != nullptr; traversalNode = traversalNode->next) {
            func(traversalNode->element);
        }
    }
    
    void printList() const {
        printf("Printing vertices list:\n");
        for (Node * traversalNode = headNode; traversalNode != nullptr; traversalNode = traversalNode->next) {
            if (traversalNode->toString != nullptr)
                printf("%s\n", traversalNode->toString(traversalNode->element).c_str());
        }
    }
    
    
private:
    int size = 0;
    Node * headNode = nullptr, * tailNode = nullptr;
    
    int floatNode(Node * node) {
        if (node->parent != this) {
            printf("SortedCollection Error: attempted to float node from non-parent collection\n");
            return 0;
        }
        
        int floats = 0;
        
        while ((node->prev != nullptr) && (node->value < node->prev->value)) { //once node encounters smaller node it stops - this depends on all smaller nodes being sorted
            Node * tempNode = node->prev;
            
            tempNode->next = node->next;
            
            node->prev = tempNode->prev;
            node->next = tempNode;
            
            tempNode->prev = node;
            
            if (node->prev != nullptr) node->prev->next = node;
            else headNode = node;
            
            if (tempNode->next != nullptr) tempNode->next->prev = tempNode;
            else tailNode = tempNode;
            
            floats++;
        }
        
        return floats;
    }
    
    int sinkNode(Node * node) {
        if (node->parent != this) {
            printf("SortedCollection Error: attempted to sink node from non-parent collection\n");
            return 0;
        }
        
        int sinks = 0;
        
        while ((node->next != nullptr) && (node->next->value < node->value)) { //once node encounters larger node it stops - this depends on all larger nodes being sorted
            Node * tempNode = node->next;
            
            tempNode->prev = node->prev;
            
            node->next = tempNode->next;
            node->prev = tempNode;
            
            tempNode->next = node;
            
            if (node->next != nullptr) node->next->prev = node;
            else tailNode = node;
            
            if (tempNode->prev != nullptr) tempNode->prev->next = tempNode;
            else headNode = tempNode;
            
            sinks++;
        }
        
        return sinks;
    }
};

#endif
