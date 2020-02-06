#pragma once

class C_Enemy;

class C_Linked_Enemy
{
public:
	static C_Linked_Enemy* create();
public:
	virtual inline C_Enemy* getData() { return m_pData; }
	virtual inline C_Linked_Enemy* getPrev() { return m_pPrev; }
	virtual inline C_Linked_Enemy* getNext() { return m_pNext; }
public:
	virtual inline void setPrev(C_Linked_Enemy* pPrev) { m_pPrev = pPrev; }
	virtual inline void setNext(C_Linked_Enemy* pNext) { m_pNext = pNext; }
	virtual inline void setData(C_Enemy* pData) { m_pData = pData; }
public:
	virtual void release();
private:
	virtual void init();
private:
	C_Enemy* m_pData;
	C_Linked_Enemy* m_pPrev;
	C_Linked_Enemy* m_pNext;
private:
	C_Linked_Enemy() {}
	virtual ~C_Linked_Enemy() {}
	C_Linked_Enemy(C_Linked_Enemy&) = delete;
	C_Linked_Enemy operator=(C_Linked_Enemy&) = delete;
};