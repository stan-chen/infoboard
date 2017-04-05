

/*
*		Global variable define
*/

#pragma once

#include "CxType.h"
#include "CxPort/CxSerialPort.h"
#include "CxComBuilder.hpp"

BEGIN_NAMESPACE(MSG_SIGN)

using MachineComponent::CxComBuilder;
using MachineComponent::CxMachine;


class CxGlobalVariable
{
	typedef boost::mutex	_Mutex;
	typedef unsigned char	_Byte;

private:

	static	_Byte			s_temper;
	static	_Byte			s_smoke;
	static	CxSerialPort	*s_globalPort;
	static	_Mutex			s_globalmu;

	static	MachineComponent::CxMachine		*s_machine;
	static	MachineComponent::CxComBuilder	*s_builder;

public:

	CxGlobalVariable()
	{
		if(s_globalPort==NULL)
			s_globalPort=new CxSerialPort;
	}
	~CxGlobalVariable()
	{
	}

	void		destoryAll()
	{
		s_globalPort->close();
		delete s_globalPort;
		delete s_builder;
	}

public:

	void				fromBuilder(MachineComponent::CxComBuilder	*builder,int cx,int cy);

	MachineComponent::CxMachine*	getMachine()const
	{
		return s_machine;
	}
	CxSerialPort		*getPort()
	{
		if(s_globalPort==NULL)
		{
			s_globalPort=new CxSerialPort;
		}
		return s_globalPort;
	}
	_Byte					getTmp()const{return s_temper;}
	_Byte					getSmoke()const{return  s_smoke;}

	void					setTmp(_Byte tmp){s_temper=tmp;}
	void					setSmoke(_Byte  smok){ s_smoke=smok;}
};

END_NAMESPACE