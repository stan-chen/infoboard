

#include "CxTransaction.h"
#include "CxGlobalVariable.h"


using namespace MSG_SIGN;


void		CxTransaction::sendStdCmd(const _Data &src)
{
	CxGlobalVariable  gl;
	_Data *dest=new _Data;
	dest->push_back(0x05);
	dest->push_back(src[0]);
	dest->push_back(src[1]);
	dest->push_back(src[2]);
	dest->push_back('O');
	dest->push_back('K');
	gl.getPort()->async_write(dest->data(),dest->size(),[dest](bool){delete dest;});
}

CxTransaction  * CxTransaction::create(CommandID id, size_t cardid ,const vector<byte>& data)
{
	return new CxCommonCommand(id,data,cardid);
}
