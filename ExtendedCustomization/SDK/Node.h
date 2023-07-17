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