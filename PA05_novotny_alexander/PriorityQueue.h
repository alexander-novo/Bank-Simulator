#pragma once

template<class ItemType>
struct Node;

template<class ItemType>
class PriorityQueue
{
    private:
    Node<ItemType>* frontPtr;
    Node<ItemType>* backPtr;

    unsigned length;
    public:
    PriorityQueue ();

    bool empty () const;
    void push ( const ItemType& );
    void pop ();
    unsigned size () const;
    ItemType& top () const;
};

template<class ItemType>
struct Node
{
    ItemType value;

    Node<ItemType>* next;
    Node<ItemType>* previous;
};

template<class ItemType>
PriorityQueue<ItemType>::PriorityQueue ()
{
    frontPtr = nullptr;
    backPtr = nullptr;

    length = 0;
}

template<class ItemType>
bool PriorityQueue<ItemType>::empty () const
{
    return length == 0;
}

template<class ItemType>
void PriorityQueue<ItemType>::push ( const ItemType& item )
{
    Node<ItemType>* nextNode = frontPtr;
    Node<ItemType>* previousNode = nullptr;
    Node<ItemType>* newNode = new Node<ItemType>;

    newNode->value = item;

    //For the first value, assign it as the front and back
    if ( empty () )
    {
        frontPtr = newNode;
        backPtr = newNode;

        newNode->next = nullptr;
        newNode->previous = nullptr;
    }
    //Otherwise, find where it goes
    else
    {
        while ( nextNode != nullptr )
        {
            //nextNode should be the node after our new node
            if ( nextNode->value > newNode->value )
            {
                break;
            }

            previousNode = nextNode;
            nextNode = nextNode->next;
        }

        //Set up next and previous locations for our new node
        newNode->next = nextNode;

        //If the next node is in the front, we have a new front
        if ( nextNode == frontPtr )
        {
            frontPtr = newNode;
        }
        else
        {
            previousNode->next = newNode;
        }

        //If the next node is nullptr, we have a new back
        if ( nextNode == nullptr )
        {
            backPtr = newNode;
        }
        else
        {
            newNode->previous = previousNode;
            nextNode->previous = newNode;
        }
    }

    length++;
}

template<class ItemType>
void PriorityQueue<ItemType>::pop ()
{
    if ( empty () ) return;

    Node<ItemType>* oldFront = frontPtr;

    frontPtr = frontPtr->next;
    if ( frontPtr != nullptr )
    {
        frontPtr->previous = nullptr;
    }

    length--;
    delete oldFront;
}

template<class ItemType>
unsigned PriorityQueue<ItemType>::size () const
{
    return length;
}

template<class ItemType>
ItemType& PriorityQueue<ItemType>::top () const
{
    return frontPtr->value;
}