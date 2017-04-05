
#include "CxMachine.hpp"
#include "CxTransaction.h"
#include "CxGlobalVariable.h"


using namespace MSG_SIGN;
using namespace MSG_SIGN::MachineComponent;

void		CxCommonCommand::exec(CxMachine *machine)
{
	_Data	*d=NULL;

	CxGlobalVariable  gl;

	switch(id)
	{
	case CID_settime:
		machine->handleMsg(_Msg(COM_TIME,MSG_SETTIME),m_data);
		sendStdCmd(m_data);
		break;

	case CID_sendimg:
		machine->handleMsg(_Msg(COM_IMGSTORAGE,MSG_IMGINSERT),m_data);
		sendStdCmd(m_data);
		break;

	case CID_sendprolist:
		machine->handleMsg(_Msg(COM_PLAYOPTION,MSG_SETPROLIST),m_data);
		sendStdCmd(m_data);
		break;

	case CID_markimg:
		machine->handleMsg(_Msg(COM_IMGSTORAGE,MSG_MARKIMAGE),m_data);
		sendStdCmd(m_data);
		break;

	case CID_setrelyswitch:
		machine->handleMsg(_Msg(COM_POWERDETECT,MSG_SETPOWERSTATE),m_data);
		sendStdCmd(m_data);
		break;

	case CID_light:
		machine->handleMsg(_Msg(COM_PLAYOPTION,MSG_SETLEDLIGHT),m_data);
		sendStdCmd(m_data);
		break;

	case CID_settemper:
		machine->handleMsg(_Msg(COM_TEMPERDETECT,MSG_SETTEMPER),m_data);
		sendStdCmd(m_data);
		break;

	case CID_readworkstate:
		d=new _Data;
		machine->handleMsg(_Msg(COM_STATEDETECT,MSG_GETSTATE),*d);
		gl.getPort()->async_write(d->data(),d->size(),[d](bool){delete d;});
		break;
	case CID_readpower:
		d=new _Data;
		machine->handleMsg(_Msg(COM_POWERDETECT,MSG_GETPOWERDETECT),*d);
		gl.getPort()->async_write(d->data(),d->size(),[d](bool){delete d;});
		break;
	case CID_setautotime:
		machine->handleMsg(_Msg(COM_TIME,MSG_AUTOTIME),m_data);
		sendStdCmd(m_data);
		break;
	default:
		sendStdCmd(m_data);
		break;
	}
}
