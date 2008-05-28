#ifndef SHARED_MEM_NODE_H
#define SHARED_MEM_NODE_H
#include "SharedMemNodeData.h"
//Class SharedMemNode keeps Tree-related information for the info tree
#define RED 1
#define BLACK 2

class SharedMemNode
{
	friend class SharedMemTree;
	bool isValid; 
	_int64 left, right;

//Shared Memory related stuff
	SharedMemNodeData data;



public:
	
/*	SharedMemNode(SharedMemNodeData *inData)
	{
		data = *inData;
		isValid = true;
	}
	
	void initialize(SharedMemNodeData *inData)
	{
		data = *inData;
		isValid = true;
	}

*/

    int  compare(SharedMemNodeData &nodeData)
	{
		return data.compare(&nodeData); 
	}

	SharedMemNodeData *  setData(SharedMemNodeData *nodeData)
	{
		data = *nodeData;
		data.adjustOffsets(nodeData);
		isValid = true;
		return &data;
	}


	void invalidate()
	{
		isValid = false;
	}


	void  print()
	{
		data.print();
	}


	SharedMemNode * setRightChild(SharedMemNode *node)
	{
      if(!node) 
         right = 0;
      else
		   right = (_int64)(reinterpret_cast<char *>(node) - reinterpret_cast<char *>(this));
		return node;
	}

	SharedMemNode * setLeftChild(SharedMemNode *node)
	{
      if(!node)
         left = 0;
      else
		   left = (_int64)(reinterpret_cast<char *>(node) - reinterpret_cast<char *>(this));
		return node;
	}


	SharedMemNode *leftChild()
	{
      if(left == 0) return 0;
		return reinterpret_cast<SharedMemNode *>((reinterpret_cast<char *>(this) + left));
	}

	SharedMemNode *rightChild()
	{
      if(right == 0) return 0;
		return reinterpret_cast<SharedMemNode *>((reinterpret_cast<char *>(this) + right));
	}

	SharedMemNodeData *getData() { return &data;}

	void free(FreeSpaceManager *fsm, LockManager *lock)
	{
		data.free(fsm, lock);
		fsm->freeShared(reinterpret_cast<char *>(this), sizeof(SharedMemNode), lock);
	}


};

#endif
