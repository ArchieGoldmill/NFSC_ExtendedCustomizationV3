#pragma once
#include "Hashes.h"

enum class FEType
{
	None,
	Image,
	String,
	Model,
	List,
	Group
};

struct FEColor
{
	int B;
	int G;
	int R;
	int A;
};

struct FEVector3
{
	float X;
	float Y;
	float Z;
};

struct FEQuaternion
{
	float X;
	float Y;
	float Z;
	float W;
};

struct FEObjData
{
	FEColor Color;
	FEVector3 Pivot;
	FEVector3 Position;
	FEQuaternion Rotation;
	FEVector3 Size;
};

class FEObject;
struct FEMinList
{
	void* Vtable;
	int NumElements;
	FEObject* Head;
	FEObject* Tail;
};

class FEObject
{
public:
	void* Vtable;
	FEObject* Next;
	FEObject* Prev;
	int Guid;
	Hash NameHash;
	int Name;
	FEType Type;
	int Flags;
	short RenderContext;
	short ResourceIndex;
	int Handle;
	int UserParam;
	FEObjData* pData;
	int DataSize;
	FEMinList Responses;
	FEMinList Scripts;
	int pCurrentScript;
	int Cached;
};