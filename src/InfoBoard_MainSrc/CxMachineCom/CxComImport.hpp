
#pragma once

#include "CxMachine.hpp"
#include <boost/typeof/typeof.hpp>
#include <boost/signals2.hpp>

BEGIN_NAMESPACE(MSG_SIGN)
	BEGIN_NAMESPACE(MachineComponent)

using std::string;




//////////////////////////////////////////////////////////////////////////
/*	Import  class	 */
///////////////////////////////////////////////////////////////////////////

class			CxComImport			:		public		CxComLeaf
{
public:
	CxComImport(CxMachine *p=NULL):CxComLeaf(p)
	{
	}
	virtual		~CxComImport(){}
public:

	virtual		_Result				handleMsg(const _Msg &msg,_Data &data);

public:

	CX_CLASS_ID(COM_IMPORT)

public:

	void		setBulbBug(int ,int );
	void		setLight(unsigned char );

};

	END_NAMESPACE
END_NAMESPACE
