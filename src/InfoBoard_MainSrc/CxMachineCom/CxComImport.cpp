
#include "CxComImport.hpp"

using namespace MSG_SIGN::MachineComponent;


_Result		CxComImport::handleMsg(const _Msg &msg,_Data &data)
{
	if(msg.id!=COM_IMPORT)
		return CxComLeaf::handleMsg(msg,data);

	return CX_NOTMACH;
}

void		CxComImport::setBulbBug(int x,int y)
{
	_Data d(4);

	unsigned short _x=x;
	unsigned short _y=y;

	d[0]=LOWORD(_x);
	d[1]=HIWORD(_x);

	d[2]=LOWORD(_y);
	d[3]=HIWORD(_y);

	if(getParent()!=NULL)
		getParent()->handleMsg(_Msg(COM_LEDSCENE,MSG_SETLEDBUG),d);
}

void		CxComImport::setLight(unsigned char light)
{
	unsigned char _light=((float)light/255.0)*32;
	_Data d(1,_light);
	if(getParent()!=NULL)
		getParent()->handleMsg(_Msg(COM_PLAYOPTION,MSG_SETLEDLIGHT),d);
}
