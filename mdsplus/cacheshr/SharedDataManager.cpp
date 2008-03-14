#ifndef HAVE_WINDOWS_H
typedef unsigned long long _int64;
#endif

#include "SharedDataManager.h"



FreeSpaceManager SharedDataManager::freeSpaceManager;
SharedMemManager SharedDataManager::sharedMemManager;
char *SharedDataManager::startAddress;
LockManager SharedDataManager::lock;
SharedMemTree SharedDataManager::sharedTree;

#ifdef HAVE_VXWORKS_H
SEM_ID *LockManager::semaphores;
#endif

//SharedDataManager reserves the first _int64 of the sherd memory to hold the address(offset)
//of the node tree root.

 SharedDataManager::SharedDataManager(int size)
{
	void *header;

	bool created = lock.initialize(TREE_LOCK);
	if(created)
	{
		lock.lock();
		startAddress = sharedMemManager.initialize(size);
		freeSpaceManager.initialize((char *)startAddress + sizeof(_int64), size - sizeof(_int64));
		sharedTree.initialize(&freeSpaceManager, &header, &lock);
		//Store address (offset) of tree root in the first _int64word of the shared memory segment
		*(_int64 *)startAddress = reinterpret_cast<_int64>(header) -
		reinterpret_cast<_int64>(startAddress);

	}
	else
	{
		lock.lock();
		startAddress = sharedMemManager.initialize(size);
		freeSpaceManager.map(reinterpret_cast<char *>(startAddress) + sizeof(_int64));
		header = reinterpret_cast<char *>(*reinterpret_cast<_int64 *>(startAddress) +
		reinterpret_cast<_int64>(startAddress));
		sharedTree.map(&freeSpaceManager, header);
	}
	lock.unlock();
}




int SharedDataManager::discardData(int treeId, int nid)
{
	lock.lock();
	SharedMemNode *node = sharedTree.find(treeId, nid);
	if(node)
	{
		node->getData()->free(&freeSpaceManager, &lock);
	}
	lock.unlock();
	return 1;
}



int SharedDataManager::setData(int treeId, int nid, char dataType, int numSamples, char *data, int size) //Write data indexed by nid
{
	lock.lock();
	SharedMemNode *node = sharedTree.find(treeId, nid);

	if(!node) 
	//No data has been written in the cache yet
	{
		SharedMemNodeData nodeData;
		nodeData.setNid(treeId, nid);
		sharedTree.insert(&nodeData, &lock);
		node = sharedTree.find(treeId, nid);
	}
	if(node)
	{
		SharedMemNodeData *nodeData = node->getData() ;
		nodeData->free(&freeSpaceManager, &lock);

		char *currData = freeSpaceManager.allocateShared(size, &lock);
		memcpy(currData, data, size);
		nodeData->setData((char *)currData, size);
		nodeData->setDataInfo(dataType, numSamples);
		CallbackManager *callback = node->getData()->getCallbackManager();
		if(callback)
			callback->callCallback();
		lock.unlock();
		
		return 1;
	}
	else
	{
		lock.unlock();
		return 0;
	}
}
	
int SharedDataManager::getData(int treeId, int nid, char *dataType, int *numSamples, char **data, int *size) //Write data indexed by nid
{
	lock.lock();
	SharedMemNode *node = sharedTree.find(treeId, nid);
	if(node)
	{
		SharedMemNodeData *nodeData = node->getData();
		if(nodeData->isSegmented())
		{
			lock.unlock();
			return 0;
		}
		nodeData->getData((char **)data, size);
		nodeData->getDataInfo(dataType, numSamples);
		lock.unlock();
		return 1;
	}
	else
	{
		lock.unlock();
		return 0;
	}
}
	
int SharedDataManager::beginSegment(int treeId, int nid, int idx, char *start, int startSize, char *end, int endSize, 
			char *dim, int dimSize, char *shape, int shapeSize, char *data, int dataSize, int *retIdx)
{
	Segment *segment;
	lock.lock();
	SharedMemNode *node = sharedTree.find(treeId, nid);
	if(!node)
	{
		SharedMemNodeData nodeData;
		nodeData.setNid(treeId, nid);
		nodeData.setSegmented(true);
		sharedTree.insert(&nodeData, &lock);
		node = sharedTree.find(treeId, nid);
	}

	if(node)
	{
		SharedMemNodeData *nodeData = node->getData();
		if(!nodeData->isSegmented())
		{
			nodeData->free(&freeSpaceManager, &lock);
			nodeData->setSegmented(true);
		}
		int numSegments = nodeData->getNumSegments();
		if(idx > numSegments)
		{
			lock.unlock();
			return 0;
		}
		if(idx == numSegments || idx < 0)
		{
			segment = (Segment *)freeSpaceManager.allocateShared(sizeof(Segment), &lock);
			segment->initialize();
			nodeData->appendSegment(segment);
			*retIdx = numSegments;
		}
		else
		{
			segment = nodeData->getSegmentAt(idx);
			*retIdx = idx;
		}

		char *currPtr;
		segment->free(&freeSpaceManager, &lock);

		currPtr = freeSpaceManager.allocateShared(startSize, &lock);
		memcpy(currPtr, start, startSize);
		segment->setStart(currPtr, startSize);

		currPtr = freeSpaceManager.allocateShared(endSize, &lock);
		memcpy(currPtr, end, endSize);
		segment->setEnd(currPtr, endSize);

		currPtr = freeSpaceManager.allocateShared(dimSize, &lock);
		memcpy(currPtr, dim, dimSize);
		segment->setDim(currPtr, dimSize);

		currPtr = freeSpaceManager.allocateShared(shapeSize, &lock);
		memcpy(currPtr, shape, shapeSize);
		segment->setShape(currPtr, shapeSize);

		currPtr = freeSpaceManager.allocateShared(dataSize, &lock);
		memcpy(currPtr, data, dataSize);
		segment->setData(currPtr, dataSize);

		segment->setTimestamped(false);

		lock.unlock();
		return 1;
	}
	lock.unlock();
	return 0;
}


int SharedDataManager::beginTimestampedSegment(int treeId, int nid, int idx, int numItems, char *shape, 
											   int shapeSize, char *data, int dataSize, _int64 start, 
											   _int64 end, char *dim, int dimSize, int *retIdx)
{
	Segment *segment;
	lock.lock();
	SharedMemNode *node = sharedTree.find(treeId, nid);
	if(!node)
	{
		SharedMemNodeData nodeData;
		nodeData.setNid(treeId, nid);
		nodeData.setSegmented(true);
		sharedTree.insert(&nodeData, &lock);
		node = sharedTree.find(treeId, nid);
	}

	if(node)
	{
		SharedMemNodeData *nodeData = node->getData();
		if(!nodeData->isSegmented())
		{
			nodeData->free(&freeSpaceManager, &lock);
			nodeData->setSegmented(true);
		}
		int numSegments = nodeData->getNumSegments();
		if(idx > numSegments)
		{
			lock.unlock();
			return 0;
		}
		if(idx == numSegments || idx < 0)
		{
			segment = (Segment *)freeSpaceManager.allocateShared(sizeof(Segment), &lock);
			segment->initialize();
			nodeData->appendSegment(segment);
			*retIdx = numSegments;
		}
		else
		{
			segment = nodeData->getSegmentAt(idx);
			*retIdx = idx;
		}

		char *currPtr;
		segment->free(&freeSpaceManager, &lock);

		if(dimSize == 0)
		{
			currPtr = freeSpaceManager.allocateShared(8 * numItems, &lock);
			segment->setDim(currPtr, 8 * numItems);
		}
		currPtr = freeSpaceManager.allocateShared(shapeSize, &lock);
		memcpy(currPtr, shape, shapeSize);
		segment->setShape(currPtr, shapeSize);

		currPtr = freeSpaceManager.allocateShared(dataSize, &lock);
		memcpy(currPtr, data, dataSize);
		segment->setData(currPtr, dataSize);
	
		//The following parameters are meaningful only when copying a segment from the tree into cache
		segment->setStartTimestamp(start);
		segment->setEndTimestamp(end);
		if(dimSize > 0)
		{
			currPtr = freeSpaceManager.allocateShared(dimSize, &lock);
			memcpy(currPtr, dim, dimSize);
			segment->setDim(currPtr, dimSize);
			segment->setStartTimestamp(*(_int64 *)dim);
			if(end == 0) segment->setEndTimestamp(*(_int64 *)dim);
		}
		segment->setTimestamped(true);

		lock.unlock();
		return 1;
	}
	lock.unlock();
	return 0;
}





int SharedDataManager::isSegmented(int treeId, int nid, int *segmented)
{
	lock.lock();
	SharedMemNode *node = sharedTree.find(treeId, nid);
	if(node)
	{
		SharedMemNodeData *nodeData = node->getData();
		*segmented = nodeData->isSegmented()?1:0;
		lock.unlock();
		return 1;
	}
	lock.unlock();
	return 0;
}


int SharedDataManager::getNumSegments(int treeId, int nid, int *numSegments)
{
	lock.lock();
	SharedMemNode *node = sharedTree.find(treeId, nid);
	if(node)
	{
		SharedMemNodeData *nodeData = node->getData();
		*numSegments = nodeData->getNumSegments();
		lock.unlock();
		return 1;
	}
	lock.unlock();
	return 0;
}
/*
//Return Shape and type information. The coding is the following:
//1) data type
//2) item size in bytes
//3) number of dimensions 
//4) total dimension in bytes 
//The remaining elements are the dimension limits

*/
int SharedDataManager::appendSegmentData(int treeId, int nid, int *bounds, int boundsSize, char *data, 
										 int dataSize, int idx, int startIdx, 
										 bool isTimestamped, _int64 *timestamps, int numTimestamps, int *segmentFilled, 
										 int *retIdx)
{
	int numSegments;
	int *shape;
	int shapeSize;
	char *segmentData;
	int segmentSize;

	lock.lock();
	SharedMemNode *node = sharedTree.find(treeId, nid);
	if(node)
	{
		SharedMemNodeData *nodeData = node->getData();
		numSegments = nodeData->getNumSegments();
		if(numSegments == 0) //May happen the first time putRow is called
		{
			lock.unlock();
			return TRUNCATED;
		}

		if(idx >= numSegments || numSegments == 0)
		{
			lock.unlock();
			return BAD_INDEX;
		}
		Segment *segment = nodeData->getSegmentAt(idx);
/*		if(!segment->isTimestamped())
		{
		    lock.unlock();
		    return 0;
		}
*/		
		segment->getShape((char **)&shape, &shapeSize);
		//Check Shape. Meaning of bound array:
//		1) data type
//		2) item size in bytes
//		3) number of dimensions 
//		4) total dimension in bytes 
//		The remaining elements are the dimension limits
		if(bounds[0] != shape[0])
		{
			lock.unlock();
			return BAD_TYPE;
		}
		if(bounds[2] < shape[2] - 1 || bounds[2] > shape[2])
		{
			lock.unlock();
			return BAD_SHAPE;
		}

		for(int i = 0; i < shape[2] - 1; i++)
		{
			if(bounds[4 + i] != shape[4 + i])
			{
				lock.unlock();
				return BAD_SHAPE;
			}
		}
		int currSegmentSize = segment->getCurrDataSize();
		segment->getData((char **)&segmentData, &segmentSize);
		if(startIdx < 0)
		{
			int leftSize = segmentSize - currSegmentSize;
			if(dataSize > leftSize)
			{
				lock.unlock();
				return TRUNCATED;
			}
			if(leftSize == dataSize)
				*segmentFilled = 1;
			else
				*segmentFilled = 0;

			memcpy(&segmentData[currSegmentSize], data, dataSize);
			segment->setCurrDataSize(currSegmentSize + dataSize);
			if(isTimestamped)
				segment->appendTimestamps(timestamps, numTimestamps);
			*retIdx = numSegments - 1;
			CallbackManager *callback = node->getData()->getCallbackManager();

			if(callback)
				callback->callCallback();
			lock.unlock();
			return 1;
		}
		else
		{
			int itemSize = bounds[3]; //Total size of a data item (array)
			int startOfs = startIdx * itemSize;
			int leftSize = segmentSize - startOfs;
			if(dataSize > leftSize)
			{
				lock.unlock();
				return TRUNCATED;
			}
			if(leftSize == dataSize)
				*segmentFilled = 1;
			else
				*segmentFilled = 0;
			memcpy(&segmentData[startOfs], data, dataSize);
			*retIdx = numSegments - 1;
			//Segment size does not change
			CallbackManager *callback = node->getData()->getCallbackManager();
			if(callback)
				callback->callCallback();
			lock.unlock();
			return 1;
		}
	}
	lock.unlock();
	return 0;
}


int SharedDataManager::appendRow(int treeId, int nid, int *bounds, int boundsSize, char *data, int dataSize, 
								 _int64 timestamp, int *segmentFilled, int *retIdx, bool *newSegmentCreated)
{
	
	int status = appendSegmentData(treeId, nid, bounds, boundsSize, data, dataSize, -1, -1, true, 
		&timestamp, 1, segmentFilled, retIdx);
	if((status & 1) && *segmentFilled)
	{
		int *prevBounds, prevBoundsSize;
		char *dim, *prevData;
		int dimSize, prevDataSize, currDataSize, actSamples;
		bool timestamped;
		*newSegmentCreated = true;
		status = getSegmentData(treeId, nid, 0, &dim, &dimSize, &prevData, &prevDataSize,
									  (char **)&prevBounds, &prevBoundsSize, &currDataSize, &timestamped, &actSamples);

//Return Shape and type information. The coding is the following:
//1) data type
//2) item size in bytes
//3) number of dimensions 
//4) total dimension in bytes 
//The remaining elements are the dimension limits
		int newDataSize = prevBounds[3];
		char *newData = new char[newDataSize];
		memset((void *)newData, 0, newDataSize);

		status = beginTimestampedSegment(treeId, nid, -1, newDataSize/dataSize, (char *)prevBounds, prevBoundsSize, 
			newData, newDataSize, 0, 0, 0, 0, retIdx);
		delete [] newData;
	}
	else
		*newSegmentCreated = false;
	return status;
}



int SharedDataManager::updateSegment(int treeId, int nid, int idx, char *start, int startSize, char *end, int endSize, 
								  char *dim, int dimSize)
{
	Segment *segment;
	lock.lock();
	SharedMemNode *node = sharedTree.find(treeId, nid);
	if(node)
	{
		SharedMemNodeData *nodeData = node->getData();
		if(!nodeData->isSegmented())
		{
			lock.unlock();
			return 0;
		}
		int numSegments = nodeData->getNumSegments();
		if(idx > numSegments || idx < 0)
		{
			lock.unlock();
			return 0;
		}
		segment = nodeData->getSegmentAt(idx);

		char *currPtr;
		int currSize;
		segment->getStart(&currPtr, &currSize);
		if(startSize > 0)
		{
		    if(currSize > 0)
			freeSpaceManager.freeShared((char *)currPtr, currSize, &lock);
		    currPtr = freeSpaceManager.allocateShared(startSize, &lock);
		    memcpy(currPtr, start, startSize);
		    segment->setStart(currPtr, startSize);
		}
		if(endSize > 0)
		{
		    segment->getEnd(&currPtr, &currSize);
		    if(currSize > 0)
			freeSpaceManager.freeShared((char *)currPtr, currSize, &lock);
		    currPtr = freeSpaceManager.allocateShared(endSize, &lock);
		    memcpy(currPtr, end, endSize);
		    segment->setEnd(currPtr, endSize);
		}
		
		if(dimSize > 0)
		{
		    segment->getDim(&currPtr, &currSize);
		    if(currSize > 0)
			freeSpaceManager.freeShared((char *)currPtr, currSize, &lock);
		    currPtr = freeSpaceManager.allocateShared(dimSize, &lock);
		    memcpy(currPtr, dim, dimSize);
		    segment->setDim(currPtr, dimSize);
  		}
		CallbackManager *callback = node->getData()->getCallbackManager();
		if(callback)
			callback->callCallback();
		lock.unlock();
		return 1;
	}
	lock.unlock();
	return 0;
}

int SharedDataManager::getSegmentLimits(int treeId, int nid, int idx, char **start, int *startSize, 
		char **end, int *endSize, bool *timestamped)
{
	Segment *segment;
	lock.lock();
	SharedMemNode *node = sharedTree.find(treeId, nid);
	if(node)
	{
		SharedMemNodeData *nodeData = node->getData();
		if(!nodeData->isSegmented())
		{
			lock.unlock();
			return 0;
		}
		int numSegments = nodeData->getNumSegments();
		if(idx >= numSegments || idx < 0)
		{
			lock.unlock();
			return 0;
		}
		segment = nodeData->getSegmentAt(idx);

		if(segment->isTimestamped())
		{
			*timestamped = true;
			segment->getStartTimestamp(start, startSize);
			segment->getEndTimestamp(end, endSize);
		}
		else
		{
			segment->getStart((char **)start, startSize);
			segment->getEnd((char **)end, endSize);
			*timestamped = false;
		}
		
		lock.unlock();
		return 1;
	}
	lock.unlock();
	return 0;
}


int SharedDataManager::getSegmentInfo(int treeIdx, int nid, int **shape, int *shapeSize, int *currDataSize)
{
	Segment *segment;
	lock.lock();
	SharedMemNode *node = sharedTree.find(treeIdx, nid);
	if(node)
	{
		SharedMemNodeData *nodeData = node->getData();
		if(!nodeData->isSegmented())
		{
			lock.unlock();
			return 0;
		}
		int numSegments = nodeData->getNumSegments();
		segment = nodeData->getSegmentAt(numSegments - 1);
		segment->getShape(reinterpret_cast<char **>(shape), shapeSize);
		*currDataSize = segment->getCurrDataSize();
		lock.unlock();
		return 1;
	}
	lock.unlock();
	return 0;
}




int SharedDataManager::getSegmentData(int treeId, int nid, int idx, char **dim, int *dimSize, char **data, int *dataSize,
									  char **shape, int *shapeSize, int *currDataSize, bool *timestamped, int *actSamples)
{
	Segment *segment;
	lock.lock();
	SharedMemNode *node = sharedTree.find(treeId, nid);
	if(node)
	{
		SharedMemNodeData *nodeData = node->getData();
		if(!nodeData->isSegmented())
		{
			lock.unlock();
			return 0;
		}
		int numSegments = nodeData->getNumSegments();
		if(idx >= numSegments || idx < 0)
		{
			lock.unlock();
			return 0;
		}
		segment = nodeData->getSegmentAt(idx);

		segment->getDim((char **)dim, dimSize);
		segment->getData((char **)data, dataSize);
		segment->getShape((char **)shape, shapeSize);
		*currDataSize = segment->getCurrDataSize();
		*timestamped = segment->isTimestamped();
		*actSamples = segment->getActSamples();

		lock.unlock();
		return 1;
	}
	lock.unlock();
	return 0;
}


int SharedDataManager::discardOldSegments(int treeId, int nid, _int64 timestamp)
{
	//printf("START DISCARD\n");
	lock.lock();
	SharedMemNode *node = sharedTree.find(treeId, nid);
	if(node)
	{
		SharedMemNodeData *nodeData = node->getData();
		if(!nodeData->isSegmented())
		{
			lock.unlock();
			return 0;
		}
		nodeData->discardOldSegments(timestamp, &freeSpaceManager, &lock);		
		lock.unlock();
	//printf("END DISCARD\n");
		return 1;
	}
	lock.unlock();
	return 0;
}


int SharedDataManager::discardFirstSegment(int treeId, int nid)
{
	//printf("START DISCARD\n");
	lock.lock();
	SharedMemNode *node = sharedTree.find(treeId, nid);
	if(node)
	{
		SharedMemNodeData *nodeData = node->getData();
		if(!nodeData->isSegmented())
		{
			lock.unlock();
			return 0;
		}
		nodeData->discardFirstSegment(&freeSpaceManager, &lock);		
		lock.unlock();
	//printf("END DISCARD\n");
		return 1;
	}
	lock.unlock();
	return 0;
}


/*
void SharedDataManager::callCallback(int treeId, int nid)
{
	SharedMemNode *node;

	node = sharedTree.find(treeId, nid);
	if(node)
	{
		CallbackManager *callback = node->getData()->getCallbackManager();
		if(callback)
			callback->callCallback();
	}
}

*/


 void *SharedDataManager::setCallback(int treeId, int nid, void (*callback)(int))
{

	char *retPtr = NULL;
	int status = 0;
	lock.lock();
	SharedMemNode *retNode = sharedTree.find(treeId, nid);
	if(!retNode) 
	//No data has been written in the cache yet, or the not is not cacheable,
	//then create a nid node without data
	{
		SharedMemNodeData nodeData;
		nodeData.setNid(treeId, nid);
		sharedTree.insert(&nodeData, &lock);
		retNode = sharedTree.find(treeId, nid);
	}


	if(retNode)
	{
		//Create a new Callback manager and concatenate it to the linked list of Callback managers
		//associated with this  nid node
		CallbackManager *callbackManager = (CallbackManager *)freeSpaceManager.allocateShared(sizeof(CallbackManager), &lock);
		CallbackManager *prevCallbackManager = retNode->getData()->getCallbackManager();
		if(prevCallbackManager)
			prevCallbackManager->setPrev((char *)callbackManager);
		callbackManager->setNext((char *)retNode->getData()->getCallbackManager());
		callbackManager->setPrev(NULL);
		retNode->getData()->setCallbackManager(callbackManager);
		callbackManager->initialize(nid, callback);
		SharedMemNodeData *nodeData = retNode->getData();
		nodeData->setWarm(true);
		lock.unlock();
		return callbackManager;
	}
	else
	{
		lock.unlock();
		return NULL;
	}
}

 int SharedDataManager::clearCallback(int treeId, int nid, char *callbackDescr)
{
	int status = 0;
	CallbackManager *callbackManager = (CallbackManager *)callbackDescr;
	callbackManager->dispose();
	lock.lock();
	SharedMemNode *retNode = sharedTree.find(treeId, nid);
	//Remove the CallbackManager instance from the queue associated with the nid node
	if(retNode)
	{
		CallbackManager *callbackManagerHead = retNode->getData()->getCallbackManager();
		if(callbackManagerHead == callbackManager)
			retNode->getData()->setCallbackManager(callbackManager->getNext());
		else
		{
			CallbackManager *next = callbackManager->getNext();
			CallbackManager *prev = callbackManager->getPrev();
			if(prev)
				prev->setNext((char *)next);
			if(next)
				next->setPrev((char *)prev);
		}
		freeSpaceManager.freeShared((char *)callbackManager, sizeof(CallbackManager), &lock);
		status = 1;
	}
	lock.unlock();
	return status;
}

SharedMemNodeData *SharedDataManager::getNodeData(int treeId, int nid, bool create)
{
	SharedMemNode *node;
	node = sharedTree.find(treeId, nid);
	if(node)
		return node->getData();

	if(create)
	{
		SharedMemNodeData nodeData;
		nodeData.setNid(treeId, nid);
		sharedTree.insert(&nodeData, &lock);
		node = sharedTree.find(treeId, nid);
		return node->getData();
	}

	return 0;
}

////////////////////////////////////////////////
//Cache coerency methods
////////////////////////////////////////////////
void SharedDataManager::getCoherencyInfo(int treeId, int nid, bool &isOwner, int &ownerIdx, bool &isWarm, bool &isDirty, int &timestamp)
{
	SharedMemNodeData *nodeData = getNodeData(treeId, nid, true);
	if(nodeData)
		nodeData->getCoherencyInfo(isOwner, ownerIdx, isWarm, isDirty, timestamp);

}

	
void SharedDataManager::getCoherencyInfo(int treeId, int nid, bool &isOwner, int &ownerIdx, bool &isWarm, int &timestamp, 
		char * &warmList, int &numWarm, char *&readerList, int &numReader)
{
	SharedMemNodeData *nodeData = getNodeData(treeId, nid, true);
	if(nodeData)
		nodeData->getCoherencyInfo(isOwner, ownerIdx, isWarm, timestamp, warmList, numWarm, readerList, numReader);

}

	
void SharedDataManager::addReader(int treeId, int nid, int readerIdx)
{
	lock.lock();
	SharedMemNodeData *nodeData = getNodeData(treeId, nid, true);
	if(nodeData)
		nodeData->addReader(readerIdx);
	lock.unlock();
}

void  SharedDataManager::addWarm(int treeId, int nid, int warmIdx)
{
	lock.lock();
	SharedMemNodeData *nodeData = getNodeData(treeId, nid, true);
	if(nodeData)
		nodeData->addWarm(warmIdx);
	lock.unlock();
}

void SharedDataManager::setOwner(int treeId, int nid, int ownerIdx, int timestamp)
{
	lock.lock();
	SharedMemNodeData *nodeData = getNodeData(treeId, nid, true);
	if(nodeData)
		nodeData->setOwner(ownerIdx, timestamp);
	lock.unlock();
}

void SharedDataManager::setCoherencyInfo(int treeId, int nid, bool isOwner, int ownerIdx, bool isWarm, int timestamp,
		char *warmNodes, int numWarmNodes, char *readerNodes, int numReaderNodes)
{
	lock.lock();
	SharedMemNodeData *nodeData = getNodeData(treeId, nid, true);
	if(nodeData)
		nodeData->setCoherencyInfo(isOwner, ownerIdx, isWarm, timestamp,warmNodes, numWarmNodes, 
			readerNodes, numReaderNodes);
	lock.unlock();
}

void SharedDataManager::setWarm(int treeId, int nid, bool warm)
{
	lock.lock();
	SharedMemNodeData *nodeData = getNodeData(treeId, nid, true);
	if(nodeData)
		nodeData->setWarm(warm);
	lock.unlock();
}

bool SharedDataManager::isWarm(int treeId, int nid)
{
	SharedMemNodeData *nodeData = getNodeData(treeId, nid, true);
	if(nodeData)
		return nodeData->isWarm();
	return false;
}

void SharedDataManager::setDirty(int treeId, int nid, bool isDirty)
{
	lock.lock();
	SharedMemNodeData *nodeData = getNodeData(treeId, nid, true);
	if(nodeData)
		nodeData->setDirty(isDirty);
	lock.unlock();
}


Event *SharedDataManager::getDataEvent(int treeId, int nid)
{
	lock.lock();
	SharedMemNodeData *nodeData = getNodeData(treeId, nid, true);
	if(nodeData)
		return nodeData->getDataEvent();
	return NULL;
	lock.unlock();
}

int SharedDataManager::getSerializedSize(int treeId, int nid)
{
	int size = 0;
	lock.lock();
	SharedMemNodeData *nodeData = getNodeData(treeId, nid, true);
	if(nodeData)
		size =  nodeData->getSerializedSize();
	lock.unlock();
	return size;
}

void SharedDataManager::getSerialized(int treeId, int nid, char *serialized)
{
	lock.lock();
	SharedMemNodeData *nodeData = getNodeData(treeId, nid, true);
	if(nodeData)
		nodeData->getSerialized(serialized);
	lock.unlock();
}

void SharedDataManager::setSerializedData(int treeId, int nid, char *serializedData, int dataLen)
{
	lock.lock();
	SharedMemNode *node = sharedTree.find(treeId, nid);

	if(!node) 
	//No data has been written in the cache yet
	{
		SharedMemNodeData nodeData;
		nodeData.setNid(treeId, nid);
		sharedTree.insert(&nodeData, &lock);
		node = sharedTree.find(treeId, nid);
	}
	if(node)
	{
		SharedMemNodeData *nodeData = node->getData();
		nodeData->free(&freeSpaceManager, &lock);
		nodeData->initialize(serializedData, &freeSpaceManager, &lock);
		CallbackManager *callback = node->getData()->getCallbackManager();
		if(callback)
			callback->callCallback();
	}
	lock.unlock();
}



