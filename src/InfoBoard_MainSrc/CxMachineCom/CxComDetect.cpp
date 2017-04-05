
#include "CxComDetect.hpp"
#include "../CxGlobalVariable.h"
#include "CxComScene.hpp"

using namespace MSG_SIGN::MachineComponent;


//////////////////////////////////////////////////////
/*state detect handle			*/
/////////////////////////////////////////////////////

void			CxStateDetect::getnLoseEff(_Data &data)
{
	data.clear();
	data.assign(2,0);
	if(getParent()!=NULL)
		getParent()->handleMsg(_Msg(COM_LEDSCENE,MSG_GETNLOSEEFF),data);
}
void			CxStateDetect::getledLight(_Data &data)
{
	data.resize(1);
	data[0]=16;
	if(getParent()!=NULL)
		getParent()->handleMsg(_Msg(COM_PLAYOPTION,MSG_GETLEDLIGHT),data);
}
void			CxStateDetect::getcurrentPlay(_Data &data)
{
	data.resize(1);
	data[0]=0;
	if(getParent()!=NULL)
		getParent()->handleMsg(_Msg(COM_PLAYOPTION,MSG_GETCURRENTPLAY),data);
}
void			CxStateDetect::getpowerState(_Data &data)
{
	//current state
	data.clear();
	data.assign(2,0);
	if(getParent()!=NULL)
		getParent()->handleMsg(_Msg(COM_POWERDETECT,MSG_GETPOWERSTATE),data);
}
_Result			CxStateDetect::stateDetect(_Data &d)
{
	_Data _data;

	//current play 
	getcurrentPlay(_data);

	unsigned char	currentPlay=_data[0];

	//led light
	getledLight(_data);
	
	unsigned char	currentLight=_data[0];

	//number of lose eff
	getnLoseEff(_data);

	unsigned short	nloseEff=MAKEWORD(_data[1],_data[0]);

	CxGlobalVariable gl;

	d.resize(16);

	d[0]	=	0x0f;
	d[1]	=	(8 << 4) | (currentLight & 0x0F);
	d[2]	=	(8 << 4) | (currentLight>>4);
	d[3]	=	gl.getTmp();
	d[4]	=	gl.getSmoke();

	d[5]	=	(nloseEff >> 8) & 0x0F;
	d[6]	=	nloseEff & 0x0F;

	d[7]	=	m_tmperDetect->getOption().m_tmppowerMax;
	d[8]	=	m_tmperDetect->getOption().m_tmppowerMin;

	d[9]	=	m_tmperDetect->getOption().m_tmpfanMax;
	d[10]	=	m_tmperDetect->getOption().m_tmpfanMin;

	d[11]	=	0;
	d[11]	=	d[11] | (m_tmperDetect->getOption().b_isautoOff ? 8 : 0);
	d[11]	=	d[11] | (m_tmperDetect->getOption().b_isautoFan ? 4 : 0);
	d[11]	=	d[11] | (m_tmperDetect->getOption().b_isautoPower? 2 :0);
	d[11]	=	d[11] | (m_tmperDetect->getOption().b_issmokeautoOff? 1 :0);
	
	//power state
	getpowerState(_data);

	d[12]	=	_data[0];
	
	d[13]	=   _data[1];
	
	d[14]	=	currentPlay;

	for(int i=1;i<15;++i)
		d[15]+=d[i];

	return CX_OK;
}


//////////////////////////////////////////////////////
/*power detect handle			*/
/////////////////////////////////////////////////////

_Result			CxPowerDetect::getPowerState(_Data &data)
{	
	unsigned char result=b_ispowerOn?0:m_result;
	data.push_back(result);
	result=b_ispowerOn?1:0;
	data.push_back(result);
	result=b_ispowerFanOn?1:0;
	data.push_back(result);
	return CX_OK;
}

_Result			CxPowerDetect::setPower(_Data &data)
{
	if(data.size()==10)
		_setPower(data[1],data[2],0,false);
	else _setPower(data[1],data[2],0,true);
	return CX_OK;	
}

_Result			CxPowerDetect::getPowerDetect(_Data &data)
{
	if(getParent()!=NULL)
	{
		/*CxLedScene *scene=getParent()->findCom(COM_LEDSCENE);
		if(scene=NULL)return ;*/
		data.resize(129,0);
		data[0]=0x80;
		return CX_OK;
	}
	return CX_FAIL;
}

void			CxPowerDetect::_setPower(bool bpower,bool	bfan,unsigned char result,bool	isforce)
{
	if(!isforce  && !b_ispowerOn)
		return ;

	if(b_ispowerOn!=bpower)
	{
		if(!bpower)
		{
			m_result=result;
			sig_powerOff();
			outputInfo("The power off..");
			}
		else 
		{
			sig_powerOn();
			outputInfo("The power on..");
		}
		b_ispowerOn=bpower;
	}

	if(b_ispowerFanOn!=bfan)
	{
		if(!bfan)
			outputInfo("The fan power off because of temperature low..");
		else 	
			outputInfo("The fan power on because of temperature high..");
		b_ispowerFanOn=bfan;
	}
}


//////////////////////////////////////////////////////
/*temper detect handle			*/
/////////////////////////////////////////////////////
_Result			CxTemperDetect::setTemper(_Data &data)
{
	tmpOption  option;
	option.m_tmppowerMax=data[1];
	option.m_tmppowerMin=data[2];
	option.m_tmpfanMax=data[4];
	option.m_tmpfanMin=data[5];
	option.b_isautoOff=(data[6] & 0x08)>>3;
	option.b_isautoFan=(data[6] & 0x04)>>2;
	option.b_isautoPower=(data[6] & 0x02) >> 1;
	option.b_issmokeautoOff=data[6] & 0x01;

	m_tmperOption=option;
	return CX_OK;
}

void			CxTemperDetect::run()
{
	CxGlobalVariable	gl;
	_Data d;
	d.resize(10);
	while(b_spin)
	{
		boost::this_thread::sleep(boost::posix_time::seconds(2));

		if(gl.getTmp() > m_tmperOption.m_tmppowerMax)
		{
			outputInfo("Warning....fan on , powe off because temperature is so high..");		
			d[1]=0;
			d[2]=1;
			if(getParent()!=NULL)
				getParent()->handleMsg(_Msg(COM_POWERDETECT,MSG_SETPOWERSTATE),d);
		}
		else if( gl.getTmp() < m_tmperOption.m_tmppowerMax && gl.getTmp() > m_tmperOption.m_tmpfanMax)
		{
			outputInfo("Warning....fan on because temperature is so high..");
			d[1]=1;
			d[2]=1;
			if(getParent()!=NULL)
				getParent()->handleMsg(_Msg(COM_POWERDETECT,MSG_SETPOWERSTATE),d);

		}
		else 
		{
			d[1]=1;
			d[2]=0;
			if(getParent()!=NULL)
				getParent()->handleMsg(_Msg(COM_POWERDETECT,MSG_SETPOWERSTATE),d);
		}

		boost::this_thread::interruption_point();
	}
}


