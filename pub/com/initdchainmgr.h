#ifndef __INITDCHAINMGR_H__
#define __INITDCHAINMGR_H__

#include <stdio.h>

typedef enum _chain_template_equal_method
{
    CtEM_ALLOW = 0,      //允许出现相同节点
    CtEM_REJECT = 77701, //不允许出现相同节点
    CtEM_REPLACE         //替换相同节点
} CtEqualMethod;

//双向不循环链表模板管理类
template <class T, const int sort = 0, const CtEqualMethod equal = CtEM_ALLOW, const int pointer = 1>
class CTDChain {
public:
    CTDChain<T, sort, equal, pointer> *m_pPrev;
    CTDChain<T, sort, equal, poniter> *m_pNext;
    T m_tItem;

protected:
    //向链表头搜索一个节点，失败时返回空指针，direct指示搜索方向
    virtual CTDChain * SearchPrev(T &item) {
        if (TypeComp(m_tItem, item, CtCS_EQ))
            return this;
        else if (m_pPrev)
            return m_pPrev->CTDChain<T, sort, equal, pointer>::SearchPrev(item);
        else
            return NULL;
    }
}

#endif