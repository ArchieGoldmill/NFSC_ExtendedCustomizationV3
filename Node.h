#pragma once

template <typename T>
class Node
{
public:
	Node<T>* Prev;
	Node<T>* Next;
};