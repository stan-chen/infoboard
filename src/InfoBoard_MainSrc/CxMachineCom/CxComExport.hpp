
#pragma once

#include "CxMachine.hpp"
#include <boost/typeof/typeof.hpp>
#include <boost/signals2.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

BEGIN_NAMESPACE(MSG_SIGN)
	BEGIN_NAMESPACE(MachineComponent)

using std::string;



//////////////////////////////////////////////////////////////////////////
/*	export class			*/
///////////////////////////////////////////////////////////////////////////
class			CxComExport				:		public		CxComLeaf
{
	typedef		boost::posix_time::ptime							_Time;
	typedef		boost::signals2::signal<void(const string&)>		_SigPushStr;

public:
	CxComExport(CxMachine *p=NULL):CxComLeaf(p)
	{
	}

	virtual		~CxComExport(){}

public:

	_SigPushStr						sig_pushStr;

public:

	virtual		_Result				handleMsg(const _Msg &msg,_Data &data);

public:

	CX_CLASS_ID(COM_EXPORT)

private:

	void		pushString(const _Data &);

private:


};

	END_NAMESPACE
END_NAMESPACE
