
/*
*
*/

#pragma once

#include "CxType.h"
#include <string>
#include <vector>
using std::vector;
using std::string;

BEGIN_NAMESPACE(MSG_SIGN)

typedef	char			CommandID;
typedef size_t			CardID;

/*all  command id*/
////////////////////////////////////////////////////////////////
const CommandID  CID_settime		= 'D' ;
const CommandID  CID_readtime		= 'E' ;
const CommandID  CID_markimg		= 'F' ;
const CommandID  CID_sendimg		= 'G' ;
const CommandID  CID_sendprolist	= 'H' ;
const CommandID  CID_showtext		= 'J' ;
const CommandID  CID_line			= 'K' ;
const CommandID  CID_switchscene	= 'L' ;
const CommandID  CID_copyscene		= 'M' ;
const CommandID  CID_clearscene		= 'N' ;
const CommandID  CID_setzh			= 'O' ;
const CommandID  CID_light			= 'P' ;
const CommandID  CID_settemper		= 'Q' ;
const CommandID  CID_settempershift = 'R' ;
const CommandID  CID_setrelyswitch	= 'S' ;
const CommandID  CID_losepointon	= 'T' ;
const CommandID  CID_loseiscomplete	= 'U' ;
const CommandID  CID_readpower		= 'V' ;
const CommandID  CID_readlose		= 'W' ;
const CommandID  CID_clearpower		= 'X' ;
const CommandID  CID_readworkstate	= 'Y' ;
const CommandID  CID_setprolist		= 'x' ;
const CommandID  CID_setautotime	= 'y' ;
////////////////////////////////////////////////////////////////

namespace MachineComponent
{
	class			CxMachine;
}

using MachineComponent::CxMachine;
//the abstruct transaction class
class CxTransaction
{
protected:
	// common define
	typedef unsigned		char	byte;
	typedef unsigned		char*	bytes;
	typedef vector<byte>			_Data;

public:
	CxTransaction(_Data	d,bool isnull,CardID cardid):m_data(d),b_isnull(isnull),m_cardid(cardid){}
	//fatory method 
	static		CxTransaction * __stdcall create(CommandID  id,size_t cardid,const _Data &data);
public:
	virtual		~CxTransaction(){}	
	bool		isNull()const{return  b_isnull;}


	virtual		CommandID		getid()const=0;
	//   Main  exec  func
	virtual		void			exec(CxMachine *)=0;
protected:

	//Make standard command
	void		sendStdCmd(const _Data &src);

protected:
	bool		b_isnull;
	CardID		m_cardid;
	_Data		m_data;
};



//NULL  command
class CxNullCommand  : public CxTransaction
{
public:
	CxNullCommand(CardID  id):CxTransaction(_Data(),true,id){}

	~CxNullCommand(){}

	CommandID			getid()const{return 0;}
	void				exec(CxMachine *);
};



//stdandard  command
class CxCommonCommand  : public CxTransaction 
{
public:
	CxCommonCommand(CommandID  i, _Data d,CardID cid):CxTransaction(d,false,cid),id(i){}
	~CxCommonCommand(){}

	CommandID			getid()const{return id;}

	void			exec(CxMachine	*);
private:

	void			settmpOption(CxMachine * );
	void			setrelyOption(CxMachine *);

private:	
	CommandID		id;
};



END_NAMESPACE 