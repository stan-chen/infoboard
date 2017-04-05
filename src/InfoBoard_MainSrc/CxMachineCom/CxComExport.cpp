
#include "CxComExport.hpp"

using namespace MSG_SIGN::MachineComponent;

_Result			CxComExport::handleMsg(const _Msg &msg,_Data &data)
{
	if(msg.id!=COM_EXPORT)
		return CxComLeaf::handleMsg(msg,data);

	switch(msg.id)
	{
	case MSG_PUSHINFO:
		pushString(data);
		return	CX_OK;

	default:
		return	CX_NOTMACH;
	}
}

void		CxComExport::pushString(const _Data &data)
{
	string		info(data.begin(),data.end());
	sig_pushStr(info);
}
