

#pragma once
#include "CxComDef.h"
#include <map>
#include <vector>
#include <hash_map>

BEGIN_NAMESPACE(MSG_SIGN)
	BEGIN_NAMESPACE(MachineComponent)

using std::map;
using std::vector;
using std::string;
using std::hash_map;



//////////////////////////////////////////////////////////////////////////
/*	Machine 			 */
///////////////////////////////////////////////////////////////////////////

class		CxMachine
{
protected:

	typedef		vector<unsigned char>		_Data;

	//typedef		hash_map<_MsgID,_ComID>		_MsgMap;

public:

	CxMachine(CxMachine *p=NULL):parent(p)
	{
	}
	virtual	~CxMachine(){}
public:

	virtual		_Result			addCom(CxMachine *)=0;
	virtual		_Result			removeCom(const _ComID &)=0;
	virtual		_Result			removeCom(CxMachine *)=0;

	virtual		CxMachine*		findCom(const _ComID& )=0;
	virtual		_ComID			getID()const=0;

//	virtual		_Result			sendMsg(_MsgID ,_Data &d=_Data())=0;

public:

	virtual		_Result			handleMsg(const _Msg &,_Data &d=_Data())=0;

protected:


public:

	CxMachine	*getParent()const{return parent;}
	_Result		setParent(CxMachine *p){CX_ASSERT(p);parent=p;return CX_OK;}

private:

	//static			_MsgMap		s_msgmap;
	CxMachine		*parent;

};


//////////////////////////////////////////////////////////////////////////
/*	the  base  composite */
///////////////////////////////////////////////////////////////////////////

class		CxComponent		:  public	CxMachine 
{

protected:

	typedef			hash_map<_ComID , CxMachine *>			_Map;

public:

	CxComponent(CxMachine *parent=NULL):CxMachine(parent)
	{
	}
	virtual		~CxComponent()
	{
	}

public:
	//declar class id
	CX_CLASS_ID(COM_COMPONENT)

public:
	virtual		_Result			addCom(CxMachine *child)
	{
		CX_ASSERT(child);
		if(m_childmap.find(child->getID())!=m_childmap.end())
			return CX_EXISTING;

		m_childmap.insert(std::make_pair(child->getID(),child));

		return	CX_OK;
	}

	virtual		_Result			removeCom(const _ComID &id)
	{
		_Map::iterator it;
		if((it=m_childmap.find(id))==m_childmap.end())
		{
			for(it=m_childmap.begin();it!=m_childmap.end();++it)
			{
				if(it->second->removeCom(id))
					return CX_OK;
			}
			return CX_NOTFOUND;
		}
		m_childmap.erase(it);
		return CX_OK;
	}

	virtual		_Result			removeCom(CxMachine *child)
	{
		CX_ASSERT(child);
		_ComID id=child->getID();
		return removeCom(id);
	}

	virtual		CxMachine*		findCom(const _ComID& id)
	{
		_Map::iterator it;
		if((it=m_childmap.find(id))==m_childmap.end())
		{
			for(it=m_childmap.begin();it!=m_childmap.end();++it)
			{
				CxMachine* machine=it->second->findCom(id);
				if(machine!=NULL)return machine;
			}
			return NULL;
		}
		return  it->second;
	}

public:

	virtual		_Result			handleMsg(const _Msg &msg,_Data &data=_Data())
	{
		if(msg.id==getID())
			return CX_FAIL;

		CxMachine *machine=findCom(msg.id);

		if(machine!=NULL)
			return machine->handleMsg(msg,data);

		else if(getParent()==NULL) 
			return CX_NOTFOUND;

		else	return	getParent()->handleMsg(msg,data);
	}

protected:

	_Map		m_childmap;
};



//////////////////////////////////////////////////////////////////////////
/*	the  base  component leaf */
///////////////////////////////////////////////////////////////////////////


class		CxComLeaf   : public CxMachine
{
public:
	CxComLeaf(CxMachine *p=NULL):CxMachine(p)
	{
	}
	virtual		~CxComLeaf(){}
public:
	virtual		_Result			addCom(CxMachine *)
	{
		return CX_ISLEAF;
	}
	virtual		_Result			removeCom(const _ComID &)
	{
		return CX_ISLEAF;
	}
	virtual		_Result			removeCom(CxMachine *)
	{
		return CX_ISLEAF;
	}
	virtual		CxMachine*		findCom(const _ComID& )
	{
		return NULL;
	}

public:

	virtual		_Result			handleMsg(const _Msg &msg,_Data &data)
	{
		if(msg.id!=getID())
		{
			if(getParent()!=NULL)
				return getParent()->handleMsg(msg,data);
		}
		return  CX_FAIL;
	}
protected:

	/*  can  output  one string */
	virtual		_Result			outputInfo(const string & str)
	{
		_Data	data(str.begin(),str.end());
		return handleMsg(_Msg(COM_EXPORT,MSG_PUSHINFO),data);
	}

};



//////////////////////////////////////////////////////////////////////////
/*	the  unit card			 */
///////////////////////////////////////////////////////////////////////////


class		CxComUnitCard   :  public CxComponent
{
public:
	CxComUnitCard(CxMachine *p=NULL):CxComponent(p){}
	virtual		~CxComUnitCard(){}
public:
	CX_CLASS_ID(COM_UNITCARD)
};

	END_NAMESPACE
END_NAMESPACE