//
//  LinkedList.h
//  Climber
//
//  Created by Ethan Coeytaux on 7/18/15.
//  Copyright (c) 2015 Science Class Games. All rights reserved.
//

#ifndef __Climber__LinkedList__
#define __Climber__LinkedList__

#include <stdio.h>
#include <stdexcept>

//TODO by adding a tailNode, cycling through entire list to add item would be unneccessary

template<typename T>
class LinkedList {
public:
    struct Node {
        Node(T element) : element(element), next(nullptr) { }
        
        T element;
        struct Node * next;
    };
    
    LinkedList() : size(0), headNode(nullptr) { }
    
    virtual ~LinkedList() {
        clear();
    }
    
    int getSize() {
        return size;
    }
    
    Node * getHeadNode() {
        return headNode;
    }
    
    bool addElement(T t) {
        Node * newNode = new Node(t);
        
        if (headNode == nullptr) {
            headNode = newNode;
            
            size++;
            return true;
        } else {
            Node * tempNode = headNode;
            
            while (tempNode->next != nullptr)
                tempNode = tempNode->next;
            tempNode->next = newNode;
            
            size++;
            return true;
        }
    }
    
    bool removeElement(T t) {
        if (headNode == nullptr) //list is empty
            return false;
        
        if (headNode->value == t) {
            Node * toDelete = headNode;
            headNode = headNode->next;
            delete toDelete;
            
            size--;
            return true;
        }
        
        Node * tempNode = headNode;
        while (tempNode->next != nullptr) {
            if (tempNode->next->value == t) {
                Node * toDelete = headNode;
                tempNode->next = tempNode->next->next;
                delete toDelete;
                
                size--;
                return true;
            }
            tempNode = tempNode->next;
        }
        
        printf("LinkedList Error: attempted to remove element not found in list\n");
        return false; //t not found in list
    }
    
    bool clear() {
        Node * tempNode = headNode;
        while (tempNode != nullptr) {
            //Node * toDelete = tempNode;
            tempNode = tempNode->next;
            //delete toDelete; //TODO why isn't this working?!?!?
        }
        
        size = 0;
        return true;
    }
    
    bool containsElement(T t) {
        Node * tempNode = headNode;
        while (tempNode != nullptr) {
            if (tempNode->value == t)
                return true;
            tempNode = tempNode->next;
        }
        return false;
    }
    
    T getElement(int index) {
        if (index > size - 1)
            throw std::invalid_argument("LinkedList Error: index greater than size of LinkedList");
        else if (index < 0)
            throw std::invalid_argument("LinkedList Error: cannot have negative index");
        
        Node * tempNode = headNode;
        while (index > 0) {
            tempNode = tempNode->next;
            index--;
        }
        return tempNode->element;
    }
    
private:
    int size = 0;
    Node * headNode = nullptr;
};

#endif /* defined(__Climber__LinkedList__) */
