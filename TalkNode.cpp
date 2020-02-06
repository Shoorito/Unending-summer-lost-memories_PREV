#include "TalkNode.h"

C_TalkNode * C_TalkNode::create()
{
	C_TalkNode* pNode(nullptr);

	pNode = C_TalkNode::create("", false, 0);
	
	return pNode;
}

C_TalkNode * C_TalkNode::create(const std::string & strDialogue, const int nTalkType, const int nType)
{
	C_TalkNode* pNode(nullptr);

	pNode = new(std::nothrow) C_TalkNode();

	pNode->init();
	pNode->setDialogue(strDialogue);
	pNode->setTalkType(nTalkType);
	pNode->setType(nType);

	return pNode;
}

void C_TalkNode::setAll(const std::string& strDial, const int nTalkType, const int nType)
{
	setDialogue(strDial);
	setTalkType(nTalkType);
	setType(nType);
}

void C_TalkNode::init()
{
	m_strDialogue	= "";
	m_nTalkType		= 0;
	m_nType			= 0;
}
