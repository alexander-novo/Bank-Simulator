#pragma once

template<class ItemType>
class Queue
{
    private:
        template<class ItemType>
        struct Node
        {
            ItemType value;

            Node<ItemType>* next;
        };

        Node<ItemType>* backPtr;

        unsigned length;
    public:
        Queue ();

        bool empty () const;
        void push ( const ItemType& );
        void pop ();
        unsigned size () const;
        ItemType& front () const;
};

template<class ItemType>
Queue<ItemType>::Queue ()
{
    backPtr = nullptr;

    length = 0;
}

template<class ItemType>
bool Queue<ItemType>::empty () const
{
    return length == 0;
}

template<class ItemType>
void Queue<ItemType>::push ( const ItemType& item )
{
    Node<ItemType>* newNode = new Node<ItemType>;

    newNode->value = item;

    if ( size () == 0 )
    {
        newNode->next = newNode;
    }
    else
    {
        newNode->next = backPtr->next;
        backPtr->next = newNode;
    }
    
    backPtr = newNode;

    length++;
}

template<class ItemType>
void Queue<ItemType>::pop ()
{
    if ( empty () ) return;

    Node<ItemType>* oldFront = backPtr->next;

    backPtr->next = backPtr->next->next;

    length--;
    delete oldFront;
}

template<class ItemType>
unsigned Queue<ItemType>::size () const
{
    return length;
}

template<class ItemType>
ItemType& Queue<ItemType>::front () const
{
    return backPtr->next->value;
}