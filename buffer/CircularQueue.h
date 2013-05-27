#ifndef _QR_CIRCULARQUEUE_
#define _QR_CIRCULARQUEUE_
#include<cstddef>
#include<cstring>
class UnCopyable
{
protected:
	UnCopyable(){}
	~UnCopyable(){}
private:
	UnCopyable(const UnCopyable&);
	UnCopyable& operator =(const UnCopyable&);
};


template<typename T>
class CircularQueue
{
private:
	//MAXSIZE 指元素的个数
	size_t MAXSIZE;
	size_t _curCount; //当前元素个数
	T* _chain;
//	size_t _tailPos;
//	size_t _headPos;
	T* _headPtr;
	T* _tailPtr;
public:
	CircularQueue(size_t maxsize = 102400);
	int enQueue(const T* buf,size_t num);
	int deQueue(T* buf,size_t num);
	int deQueueAll(T* buf);
	//CircularQueue& operator=(const CircularQueue&);
	int moveTo(CircularQueue&,size_t);
	size_t count() const;
	~CircularQueue();
};

/*default num is 102400
 *can't not use malloc to template
 *既可以在函数声明也可以在函数定义中指定默认实参。但是，在一个文件中，只能为一个形参指定默认实参一次。
 */
template<typename T>
CircularQueue<T>::CircularQueue(size_t maxsize):MAXSIZE(maxsize),_curCount(0)
{
	//_chain = (T*)malloc(MAXSIZE * sizeof(T));
	_chain = new T[MAXSIZE];
	_headPtr = _chain;
	_tailPtr = _chain;
}

/*循环链表中，空一位，_tailPtr + 1 == _headPtr表示链表满
 *函数返回进入队列的个数
 *如果空间不够就返回-1
 */
template<typename T>
int CircularQueue<T>::enQueue(const T* buf,size_t num)
{
	if(_tailPtr >= _headPtr)
	{
		///队列不用翻回来就够的情况
		///if中的不能直接比较，前面的是有符号数，num是无符号数
		if(_chain + MAXSIZE - _tailPtr -1 >= 0 &&
			 (size_t)(_chain + MAXSIZE - _tailPtr -1) >= num)
		{
			memcpy((void*)_tailPtr,(void*)buf,num*sizeof(T));
			_tailPtr += num;
			//_tailPos += size;
			_curCount += num;
			return num;
		}
		///队列翻回来才够的情况
		else if((_chain + MAXSIZE - _tailPtr) + (_headPtr - _chain) -1 >= 0 &&
				 size_t((_chain + MAXSIZE - _tailPtr) + (_headPtr - _chain)) -1 >= num)
		{
			size_t numCopyToTail = _chain + MAXSIZE - _tailPtr;
			memcpy((void*)_tailPtr,(void*)buf,numCopyToTail*sizeof(T));
			memcpy((void*)_chain,(void*)(buf+numCopyToTail),(num-numCopyToTail)*sizeof(T));

			_tailPtr = (_tailPtr + num) - MAXSIZE;
			_curCount += num;
			return num;
		}
		///不够了
		else
		{
			return -1;
		}
	}
	///队列已经翻过来了的情况
	else
	{
		if(_headPtr - _tailPtr - 1 >= 0 &&
			 (size_t)(_headPtr - _tailPtr - 1) >= num)
		{
			memcpy((void*)_tailPtr,(void*)buf,num*sizeof(T));
			_tailPtr += num;
			_curCount += num;
			return num;
		}
		else
		{
			return -1;
		}
	}
}

/*********************************************
 *函数返回出队列的个数，不够的话有几个返回几个
 *********************************************/
template<typename T>
int CircularQueue<T>::deQueue(T* buf,size_t num)
{
	///source queue没有翻回去
	if(_headPtr <= _tailPtr)
	{
		if(_curCount >= num)
		{
			//buf = (T*)malloc(sizeof(T) * num);
			buf = new T[num];
			memcpy((void*)buf,(void*)_headPtr,sizeof(T)*num);
			_headPtr += num;
			_curCount -= num;
			return num;
		}
		else
		{
			size_t numCanCopy = _curCount;
			//buf = (T*)malloc(sizeof(T) * numCanCopy);
			buf = new T[numCanCopy];
			memcpy((void*)buf,(void*)_headPtr,sizeof(T)*num);
			_headPtr += numCanCopy;
			_curCount -= numCanCopy;
			return numCanCopy;
		}
	}
	///source queue翻回去了
	else
	{
		if(_curCount >= num)
		{
			//buf = (T*)malloc(sizeof(T) * num);
			buf = new T[num];
			size_t numToEnd = _chain + MAXSIZE - _tailPtr;
			///从队列头到数组尾部就够了的情况
			if(numToEnd >= num)
			{
				memcpy((void*)buf,(void*)_headPtr,sizeof(T)*numToEnd);
				_headPtr += num;
				_curCount -= num;
			}
			else
			{
				memcpy((void*)buf,(void*)_headPtr,sizeof(T)*numToEnd);
				memcpy((void*)(buf + numToEnd),(void*)_chain,sizeof(T)*(num - numToEnd));
				
				_headPtr = _headPtr + num - MAXSIZE;
				_curCount -= num;
			}
			return num;
		}
		else
		{
			int numCanCopy = _curCount;
			int numToEnd = _chain + MAXSIZE - _headPtr;
			//buf = (T*)malloc(sizeof(T) * numCanCopy);
			buf = new T[numCanCopy];
			memcpy((void*)buf,(void*)_headPtr,sizeof(T)*numToEnd);
			memcpy((void*)(buf + numToEnd),(void*)_chain,sizeof(T)*(numCanCopy - numToEnd));
			
			_headPtr = _tailPtr;
			_curCount -= numCanCopy;
			return numCanCopy;
		}
	}
}

template<typename T>
int CircularQueue<T>::deQueueAll(T* buf)
{
	return deQueue(buf,_curCount);
}

template<typename T>
CircularQueue<T>::~CircularQueue()
{
	delete[] _chain;
}

template<typename T>
size_t CircularQueue<T>::count() const
{
	return _curCount;
}


//template<typename T>
//CircularQueue& CircularQueue::operator=(const CircularQueue& que)
//{
	//if(this == &que)
	//	return *this;
	//
	//MAXSIZE = que.MAXSIZE;
	//_curCount = que._curCount;
	//_chain = new 
//}

/********************************************************
 *move data to que. the data in this->_chain will be deleted
 *return the number of moved. return -1 if there is no enough space 
 *this function also will set que._tailPtr and que._curCount
 ********************************************************/
template<typename T>
int CircularQueue<T>::moveTo(CircularQueue<T>& que,size_t num)
{
	///que中有足够的位置
	if(num <= que.MAXSIZE - que._curCount - 1)
	{

	//	///source queue 没翻回来
	//	if(_tailPtr >= _headPtr)
	//	{
	//		///number of can be insert at the end of que
	//		int numMoveToEnd = que._chain + que.MAXSIZE - que._tailPtr;  

	//		///all data can be insert to que's end
	//		if(numMoveToEnd >= num)
	//		{
	//			deQueue(que._tailPtr,num);
	//			que._tailPtr += num;
	//		}
	//		else
	//		{
	//			deQueue(que._tailPtr,numMoveToEnd);
	//			deQueue(que._chain,num - numMoveToEnd);
	//			que._tailPtr = que._chain + num -numMoveToEnd;
	//		}
	//	}
	//	///翻回来了
	//	else
	//	{
	//		deQueue(que._tailPtr,num);
	//		que._tail += num;
	//	}


 		///no need to condsider wether source queue is overturn. deQueue has encapsulate it
		///number of can be insert at the end of que
		size_t numMoveToEnd = que._chain + que.MAXSIZE - que._tailPtr;  

		///all data can be insert to que's end
		if(numMoveToEnd >= num)
		{
			deQueue(que._tailPtr,num);
			que._tailPtr += num;
			que._curCount += num;
		}
		else
		{
			deQueue(que._tailPtr,numMoveToEnd);
			deQueue(que._chain,num - numMoveToEnd);
			que._tailPtr = que._chain + num -numMoveToEnd;
			que._curCount += num;
		}

		return _curCount;
	}
	else
		return -1;
}




#endif
