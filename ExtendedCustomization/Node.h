#pragma once

template <typename T>
struct bNode
{
	bNode<T>* Next;
	bNode<T>* Prev;
};

template <typename T>
struct bList
{
	bNode<T> HeadNode;
};

template <class T>
struct bSNode
{
	T* Next;
};

template <class T>
struct bSlist
{
	T* Head;
	T* Tail;
};