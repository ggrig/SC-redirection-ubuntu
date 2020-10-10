#ifndef TMULTITHREADSINGLEQUEUE_H
#define TMULTITHREADSINGLEQUEUE_H

/*
   Multi Threading Queue After:
   M. Michael and M. Scott. "Nonblocking algorithms and preemption-safe locking on
                             multiprogrammed shared - memory multiprocessors."
      Journal of Parallel and Distributed Computing, 51(1):1–26, 1998.

   N.B.: This is the template version: use a pointer for the template parameter
         in order to preserve the "move semantics" as opposed to the "copy semantics"
         inherent to the templated form.
*/

/*
code source http://www.codeproject.com/Articles/115859/A-Standard-Multi-threaded-Dynamic-Queue

The algorithm - Brief explanation

A multithreaded queue has basically an initialize section and two operations: push and pop, 
or alternatively, enqueue and dequeue. In a multithreaded environment, the emphasis is on the 
locking - unlocking of the resource while concurrently enqueueing or dequeueing items from the queue.

The two operations on the queue must allow the concurrent access to the shared resource. 
Hence, they must use locks (on mutexes) to hold on in order to regulate access to the shared resource.

This standard implementation of the dynamic queue solves this problem by representing the queue itself 
as a simple, singly linked list that is never empty; it always contains at least one single dummy node. 
This allows for parallel concurrent accesses of the head / tail of the queue as one element will always 
be there in order to prevent concurrent access of the same node. Hence, the two operations push and pop 
need only a lock each to regulate concurrent access to the tail / head of the queue, respectively.

It never occurs that such locks need to be held simultaneously: each operation will access the 
head / tail of the queue in complete independence from the other operation. This allows for the fastest 
implementation possible for a multithreaded queue as each lock will be held only to independently serialize 
the two operations; i.e., each lock is necessary only to serialize multithread access to the head or tail 
of the queue but not to serialize simultaneous access to the head and tail of the queue itself.

Hence the two operations push and pop run in complete independence, and may run in parallel without 
disrupting the state of the queue. The locks are necessary only to safely access each of the two operations 
alone from multiple threads; i.e., we need to serialize access to the head or tail of the queue. This is the 
fastest implementation possible for a multithreaded queue using locks. Non-blocking algorithms are possible, 
but they require increased complexity coding
*/


#if _MSC_VER > 1000
#pragma warning (disable: 4786)
#pragma warning (disable: 4748)
#pragma warning (disable: 4103)
#endif /* _MSC_VER > 1000 */

template <typename T>
class CTMultiThreadSingleQueue
{
   /*template <typename T>*/
   class CTNode
   {
   public:
      CTNode(void) { pNextNode = NULL; };
      ~CTNode(void) {};

      CTNode* pNextNode;
      T       pValuePointer;

   private:
      // Don't allow these sorts of things
      CTNode( const CTNode& ) {};
      CTNode& operator = ( const CTNode& ){ return( *this ); };
   };

public:
   CTMultiThreadSingleQueue(void);
   virtual ~CTMultiThreadSingleQueue(void);

private:
   // Don't allow these sorts of things
   CTMultiThreadSingleQueue( const CTMultiThreadSingleQueue& ) {};
   CTMultiThreadSingleQueue& operator = ( const CTMultiThreadSingleQueue& ){ return( *this ); };

protected:
   // Critical sections guarding Head and Tail code sections
   //CRITICAL_SECTION m_HeadCriticalSection;
   //CRITICAL_SECTION m_TailCriticalSection;
   // The queue, two pointers to head and tail respectively
   CTNode* pHeadNode;
   CTNode* pTailNode;
   // Queue size
   volatile long m_Size;
   //HG - introduce the virtual functions to allow checks for "queue allowed to push" and "queue empty" states 
   virtual void incrementSize() {m_Size++;/*::InterlockedIncrement(&m_Size);*/};
   virtual void decrementSize() {m_Size--;/*::InterlockedDecrement(&m_Size);*/};

public:
   // Enqueue, pass by value
   /*template <typename T>*/
   bool Push(T pNewValue);
   // Dequeue, pass by reference
   /*template <typename T>*/
   bool Pop (T& pValue);
   // for accurate sizes change the code to use the Interlocked functions calls
   long GetSize() { return m_Size; }
};

template <typename T> CTMultiThreadSingleQueue<T>::CTMultiThreadSingleQueue(void)
{
   // node = new node() # Allocate a free node
   // node next = NULL # Make it the only node in the linked list
   CTNode* pFirstNode = new CTNode;
   // The queue
   // QHead = QTail = node # Both Head and Tail point to it
   pHeadNode = pFirstNode;
   pTailNode = pFirstNode;
   // Queue size, dummy node counted off
   m_Size = 0;
   // QHlock = QTlock = FREE # Locks are initially free
   //InitializeCriticalSection(&m_HeadCriticalSection);
   //InitializeCriticalSection(&m_TailCriticalSection);
}

template <typename T> CTMultiThreadSingleQueue<T>::~CTMultiThreadSingleQueue(void)
{
   T pDummyValue;
   while ( Pop(pDummyValue) ) ;
   if ( pHeadNode ) 
   { 
	   try 
	   { 
		   delete pHeadNode; 
	   } 
	   catch(...) 
	   { 
		   NULL; 
	   } 
   }
   //DeleteCriticalSection(&m_HeadCriticalSection);
   //DeleteCriticalSection(&m_TailCriticalSection);
}

template <typename T> bool CTMultiThreadSingleQueue<T>::Push(T pNewValue)
{
   // node = new node() # Allocate a new node from the free list
   // node->next = NULL # Set next pointer of node to NULL
   CTNode* pNewNode = new CTNode;
   // node->value = value # Copy enqueued value into node
   pNewNode->pValuePointer = pNewValue;
   // lock(&QTlock) # Acquire Tail lock in order to access Tail
   //EnterCriticalSection(&m_TailCriticalSection);
   // QTail->next = node # Link node at the end of the linked list
   pTailNode->pNextNode = pNewNode;
   // QTail = node # Swing Tail to node
   pTailNode = pNewNode;
   // Increment size - use InterlockedIncrement for accurate sizes
   //::InterlockedIncrement(&m_Size);
   //m_Size++;
   //HG - use the virtual function 
   incrementSize();
   // unlock(&QTlock) # Release Tail lock
   //LeaveCriticalSection(&m_TailCriticalSection);
   return true;
}

template <typename T> bool CTMultiThreadSingleQueue<T>::Pop(T& pValue)
{
   // lock(&QH lock) # Acquire Head lock in order to access Head
   //EnterCriticalSection(&m_HeadCriticalSection);
   // node = Q->Head # Read Head
   CTNode* pCurrentNode = pHeadNode;
   // new_head = node->next # Read next pointer
   CTNode* pNewHeadNode = pHeadNode->pNextNode;
   // if new_head == NULL # Is queue empty?
   if ( NULL == pNewHeadNode ) // # Queue was empty
   {
      //    unlock(&QH lock) # Release Head lock before return
      //LeaveCriticalSection (&m_HeadCriticalSection);
      //    return FALSE
      return false;
   }
   // endif
   // *pvalue = new_head->value # Queue not empty. Read value before release
   pValue = pNewHeadNode->pValuePointer;
   // QHead = new_head # Swing Head to next node
   pHeadNode = pNewHeadNode;
   // decrement size - use InterlockedDecrement for accurate sizes
   // ::InterlockedDecrement(&m_Size);
   //m_Size--;
   //HG - use the virtual function 
   decrementSize();
   // unlock(&QH lock) # Release H lock
   //LeaveCriticalSection (&m_HeadCriticalSection);
   // free(node) # Free node
   try { delete pCurrentNode; } catch(...) { NULL; }
   // return TRUE # Queue was not empty, dequeue succeeded
   return true;
}

#endif // ! defined(TMULTITHREADSINGLEQUEUE_H)

