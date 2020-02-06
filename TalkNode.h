#pragma once
#include "cocos2d.h"

USING_NS_CC;

const int g_nTalkCount = 30;
const int g_nEndTalkCount = 18;

class C_TalkNode
{
public:
	static C_TalkNode* create();
	static C_TalkNode* create(const std::string&, const int, const int);
public:
	virtual inline const std::string& getDialogue() const { return m_strDialogue; }
	virtual inline const int& getTalkType() const		  { return m_nTalkType; }
	virtual inline const int& getType() const			  { return m_nType; }
public:
	virtual void setAll(const std::string&, const int, const int);
	virtual inline void setDialogue(const std::string& strDialogue) { m_strDialogue = strDialogue; }
	virtual inline void setTalkType(const int nType)			    { m_nTalkType = nType; }
	virtual inline void setType(const int nType)					{ m_nType = nType; }
private:
	virtual void init();
private:
	std::string m_strDialogue;
	int			m_nTalkType;
	int			m_nType;
private:
	C_TalkNode() {}
	virtual ~C_TalkNode() {}
	C_TalkNode(C_TalkNode&) = delete;
	C_TalkNode operator=(C_TalkNode&) = delete;
};
