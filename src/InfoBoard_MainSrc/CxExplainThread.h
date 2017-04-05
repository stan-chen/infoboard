

#pragma once

#include "CxType.h"
#include "CxThread.hpp"
#include "CxPort/CxBuffer.hpp"
#include <boost/signals2.hpp>


BEGIN_NAMESPACE(MSG_SIGN)

namespace MachineComponent
{
	class	CxMachine;
}

using MachineComponent::CxMachine; 

class	CxTransaction;


class CxExplainThread  :  public  CxThread
{
	typedef CxLinerBuffer<CxTransaction *>			_Buffer;

	typedef boost::signals2::signal<void(char )>	_MsgSig;

public:
	
	CxExplainThread(CxMachine *,_Buffer *);
	~CxExplainThread();

public:

	_MsgSig				sig_msg;

protected:

	void		run();

private:

	volatile	bool		b_spin;
	CxMachine	*m_machine;
	_Buffer		*m_transqueue;

};


END_NAMESPACE 