#ifndef LIST_HEAD_FILE
#define LIST_HEAD_FILE

//ZhouFeng 2011.10.29

//简单的链表模板类
//节点不使用类来实现,只用一个类完成
//使用时在实例化时,NODE的结构中指向下一个节点的指针名称必须为pNext;

//如果在节点中有指针,并且需要在退出时释放,请重载类(而非简单实例化),并重写ReleaseNodeMem及析构函数,
//在析构函数中显式调用ReleaseList来释放所有资源。否则重写的ReleaseNodeMem不会被调用,不能保证资源释放正确

//如果需要对链表进行排序,需要重载CompareKeyWord函数,以进行关键字比较.函数里可以实现多关键字排序.
//排序算法使用最简单的冒泡法。

//如何遍历链表中的数据:
//先使用reset将返回节点置为第一个,然后用GetNextNode(),只要返回值不为NULL就一直调用此函数得到下一个节点.

template <class NODE>

class CSimpleLinkList
{
public:
	CSimpleLinkList();
	~CSimpleLinkList();

	//增加一个节点
	//注意 pNode2Add指向的内存必须使用new分配。
	//如果使用快速模式,新增加的点会直接放在链表结束,函数isSameNode不被调用.
	//否则会将所有所有己有的点与新增加的点进行比较,花费的时间会相当长.
	int AddNode(NODE *pNode2Add,bool bQuick = false);

	//将返回节点置为第一个
	void Reset();
	//获取下一个节点,注意,如果需要取出所有数据,请先调用Reset将返回节点置为第一个节点。
	NODE* GetNextNode();

	void SortList();//排序
	//返回共有多少个节点
	int GetItemNumber();
	//删除指定节点
	//注意:index从1开始,如果是0则不删除任何节点。
	int DeleteItem(int index);
	//获取指定节点
	//注意:index从1开始,失败则返回NULL
	NODE* GetItem(int index);
	//删除所有节点
	void DeleteAllItem(void);
protected:
	//释放一个节点中的数据(不包括节点自身)
	//如果在节点中有指针,并且需要在退出时释放,请重载类(而非简单实例化),并重写ReleaseNodeMem及析构函数,
	//此时在析构函数中显式调用ReleaseList来释放所有资源。否则重写的ReleaseNodeMem不会被调用,不能保证资源释放正确
	virtual void ReleaseNodeMem(NODE *pNodeToRelease);
	//释放所有链表
	void ReleaseList();

	/*得到两个节点的比较结果,如果
	需要交换,则返回<0
	否则返回>=0/**/
	virtual int CompareKeyWord(NODE *pNode1, NODE *pNode2);

	virtual bool isSameNode(NODE *pTestNode,NODE *pAddNode);

	NODE *pHead,*pReturn;
	NODE *pLast;
};
//----------------------------------------
template <class NODE>
CSimpleLinkList<NODE>::CSimpleLinkList()
{
	pHead = new NODE;
	memset(pHead, 0, sizeof(NODE));
	pLast = pHead;
}

//----------------------------------------
template <class NODE>
CSimpleLinkList<NODE>::~CSimpleLinkList()
{
	ReleaseList();
}
//----------------
template <class NODE>
void CSimpleLinkList<NODE>::DeleteAllItem(void)
{
	ReleaseList();//清除整个链表
	//重新分配一个头结点。
	pHead = new NODE;
	ZeroMemory(pHead,sizeof(NODE));
	pLast = pHead;
}
//----------------
template <class NODE>
void CSimpleLinkList<NODE>::ReleaseList()
{
	if(pHead != nullptr)
	{
		pReturn = pHead->pNext;
		delete pHead;//正常情况下pHead不会有额外分配的内存
		while (pReturn != nullptr)
		{
			pHead = pReturn->pNext;
			ReleaseNodeMem(pReturn);//释放额外分配的内存(如果中间有指针并且分配了内存的话)
			delete pReturn;//释放节点。
			pReturn = pHead;
		}
		pHead = nullptr;
		pLast = nullptr;
	}
}
//-----------------------------------------
template <class NODE>
void CSimpleLinkList<NODE>::ReleaseNodeMem(NODE *pNodeToRelease)
{
	//正常情况下不需要做任何处理,只需要直接返回即可
	//但是如果结构中有指针,并且分配了内存,需要重载些函数并进行释放.
}
//-----------------------------------------
template <class NODE>
int CSimpleLinkList<NODE>::AddNode(NODE *pNodeToAdd,bool bQuick)
{
	NODE *pWork;
	if(bQuick)
	{
		pWork = pLast;
	}
	else
	{
		pWork = pHead;
		while((pWork->pNext)!=NULL)//搜索到最后一个节点
		{
			pWork = pWork->pNext;
			if(isSameNode(pWork,pNodeToAdd))//要增加的节点与已经存在的节点判断是否相等。
			{
				return 1;//存在相同的节点,直接返回.
			}
		}
	}
	pWork->pNext = pNodeToAdd;//-直接将节点加入,所以在增加节点时,节点数据必须是new分配,否则在释放时会出错。
	pWork = pWork->pNext;
	pWork->pNext = NULL;
	pLast = pWork;
	return 0;
}

//-----------------------------------------
template <class NODE>
void CSimpleLinkList<NODE>::Reset()
{
	pReturn = pHead;
}

//-----------------------------------------
template <class NODE>
NODE* CSimpleLinkList<NODE>::GetNextNode()
{
	if(pReturn != NULL)//未结束则取下一个节点
	{
		pReturn = pReturn->pNext;
	}
	return pReturn;//返回节点
}
//-----------------------------------------
template <class NODE>
int CSimpleLinkList<NODE>::CompareKeyWord(NODE *pNode1, NODE *pNode2)
{
	return 0;
}
//------------------------------------------
template <class NODE>
void CSimpleLinkList<NODE>::SortList()
{
	NODE *pCMP1,*pCMP2;//用于比较的两个节点
	NODE *pCMP1Pre,*pCMP2Pre;//比较的两个节点的前一个节点

	pCMP1 = pHead->pNext;
	pCMP1Pre = pHead;

	while((pCMP1 != NULL) && (pCMP1->pNext != NULL))
	{
		pCMP2 = pCMP1->pNext;
		pCMP2Pre = pCMP1;
		while(pCMP2 != NULL)
		{
			if(CompareKeyWord(pCMP1,pCMP2) < 0)//需要交换数据
			{
				//交换pCMP1和pCMP2
				//前时节点中可能有类,简单的把两个节点memcopy来交换可能会出问题,
				//所以这儿采取的是修改前后节点的方式来解决此问题
				//正常情况下链表中数据如下:
				//pCMP1Pre->next -- [pCMP1->next] -- pCMP1Next
				//pCMP2Pre->next -- [pCMP2->next] -- pCMP2Next
				//所以只需要把pCMP1Pre->next = pCMP2
				//pCMP2Pre->next = pCMP1;
				//再交换一下pCMP1->next和pCMP2->next;
				//就成了
				//pCMP1Pre->next -- [pCMP2->next] -- pCMP1Next
				//pCMP2Pre->next -- [pCMP1->next] -- pCMP2Next
				//
				//特殊情况:
				//                                    pCMP1Next
				//pCMP1Pre->next -- [pCMP1->next] -- [pCMP2->next] - pCMP2Next
				//                   pCMP2Pre->next
				//也即pCMP1 = pCMP2Pre;
				//用上面的算法也无问题
				NODE *pTmp;
				pCMP1Pre->pNext = pCMP2;
				pCMP2Pre->pNext = pCMP1;

				pTmp = pCMP1->pNext;
				pCMP1->pNext = pCMP2->pNext;
				pCMP2->pNext = pTmp;

				//现在链表就就成了
				//pCMP1Pre->next -- [pCMP2->next] -- pCMP1Next
				//pCMP2Pre->next -- [pCMP1->next] -- pCMP2Next
				//但是我们需要pCMP1继续在上部分中比较,pCMP2继续在下部分中比较,所以pCMP1和pCMP2需要交换
				pTmp = pCMP1;pCMP1 = pCMP2;pCMP2 = pTmp;
			}
			pCMP2Pre = pCMP2;
			pCMP2 = pCMP2->pNext;
		}
		pCMP1Pre = pCMP1;
		pCMP1 = pCMP1->pNext;
	}
}

template <class NODE>
int CSimpleLinkList<NODE>::GetItemNumber()
{
	int i;
	NODE *pWork;
	pWork = pHead;
	i = 0;
	while((pWork->pNext)!=NULL)//搜索到最后一个节点
	{
		pWork = pWork->pNext;
		i++;
	}
	return i;
}

//删除指定节点
//注意:index从1开始
template <class NODE>
int CSimpleLinkList<NODE>::DeleteItem(int index)
{
	int i;
	NODE *pWork;
	bool bFind = false;
	if(index <= 0)
	{
		return 0;//OK.
	}
	pWork = pHead;
	i = 0;
	while((pWork->pNext)!=NULL)//搜索到最后一个节点
	{
		i++;
		if(i == index)
		{
			//pWork = pWork->pNext;
			bFind = true;
			break;
		}
		else
		{
			pWork = pWork->pNext;
		}
	}
	if(bFind)
	{
		//
		//pWork->pNext 指向要删除的节点
		NODE *pTmp = pWork->pNext;//pTmp为要删除的节点
		pWork->pNext = pTmp->pNext;//链表跳过要删除的节点
		ReleaseNodeMem(pTmp);

		if(pWork->pNext == NULL)//if pWork is the last node.
		{
			pLast = pWork;
		}
		return 0;
	}
	return 1;//没找到
}

//获取指定节点
//注意:index从1开始,失败则返回NULL
template <class NODE>
NODE* CSimpleLinkList<NODE>::GetItem(int index)
{
	NODE* pWork;
	int i;

	pWork = pHead;
	i = 0;
	if(index <= 0)
	{
		return NULL;//
	}
	while((pWork->pNext)!=NULL)//搜索到最后一个节点
	{
		i++;
		pWork = pWork->pNext;
		if(i == index)
		{
			return pWork;
		}
	}
	return NULL;
}

//判断节点是否相等
//在增加AddNode中需要调用此函数用于避免增加相同点
//但根据需要不同，往往相同节点的判断方法不一致,请自行重载使用。
template <class NODE>
bool CSimpleLinkList<NODE>::isSameNode(NODE *pTestNode,NODE *pAddNode)
{
	return false;
}
#endif
