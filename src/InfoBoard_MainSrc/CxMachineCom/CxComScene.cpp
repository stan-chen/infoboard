
#include "CxComScene.hpp"

using namespace MSG_SIGN::MachineComponent;


_Result				CxLedScene::getSceneSize(_Data &data)
{
	unsigned short	cx=m_cx;
	unsigned short	cy=m_cy;	
	data.push_back(cx>>8);
	data.push_back(cx);
	data.push_back(cy>>8);
	data.push_back(cy);
	return CX_OK;
}

unsigned short CxLedScene::getnBug()const
{
	unsigned short number=0;

	for(int i=0;i<m_cx;++i)
	{
		for(int j=0;j<m_cy;++j)
		{
			if(m_bulb[i][j].is_Bug())
				++number;
		}
	}
	return number;
}

_Result				CxLedScene::getNLoseEff(_Data	&data)
{
	unsigned short n=getnBug();
	data.push_back((n>>8) &  0x0F);
	data.push_back(n & 0x0F);
	return CX_OK;
}


_Result				CxLedScene::setLedBug(_Data &data)
{
	unsigned short x=MAKEWORD(data[0],data[1]);
	unsigned short y=MAKEWORD(data[2],data[3]);

	if(data.size()<2)
		return CX_FAIL;

	else setBug(x,y);
	return CX_OK;
}
