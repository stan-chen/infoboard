

#pragma once

#include "CxMachine.hpp"
#include "../CxGlobalVariable.h"
#include <boost/signals2.hpp>
#include "../CxThread.hpp"
BEGIN_NAMESPACE(MSG_SIGN)
	BEGIN_NAMESPACE(MachineComponent)


//////////////////////////////////////////////////////////////////////////
/*	detect component			 */
///////////////////////////////////////////////////////////////////////////
class		CxComDetect   :  public		CxComponent
{
public:
	explicit	CxComDetect(CxMachine *p=NULL):CxComponent(p){}
	virtual		~CxComDetect(){}
public:

	CX_CLASS_ID(COM_DETECT)
};


//////////////////////////////////////////////////////////////////////////
/*abstruct 	detect leaf			 */
///////////////////////////////////////////////////////////////////////////
class		CxDetectLeaf		:	public		CxComLeaf
{
	public:
	explicit	CxDetectLeaf(CxMachine *p=NULL):CxComLeaf(p){}
	virtual		~CxDetectLeaf(){}
public:

	CX_CLASS_ID(COM_DETECTLEAF)
};

//////////////////////////////////////////////////////////////////////////
/*	power detect		 */
///////////////////////////////////////////////////////////////////////////
class		CxPowerDetect		:	public		CxDetectLeaf
{
	typedef			boost::signals2::signal<void(bool,bool,unsigned char)>		_SignalSet;	
	typedef			boost::signals2::signal<void()>								_Signal;

public:
	/*	direct call signals */
	_SignalSet					sig_setPower;

	/* connect signals */
	_Signal						sig_powerOff;
	_Signal						sig_powerOn;

public:
	explicit	CxPowerDetect(CxMachine *p=NULL):CxDetectLeaf(p),
				b_ispowerFanOn(false),b_ispowerOn(true),m_result(0)
	{
		sig_setPower.connect(boost::bind(&CxPowerDetect::_setPower,this,_1,_2,_3,true));
	}
	virtual		~CxPowerDetect(){}
public:
	//class id
	CX_CLASS_ID(COM_POWERDETECT)
		
	CX_BEGIN_MSG
		CX_MSG(MSG_GETPOWERSTATE,getPowerState)
		CX_MSG(MSG_SETPOWERSTATE,setPower)
		CX_MSG(MSG_GETPOWERDETECT,getPowerDetect)
	CX_END_MSG

private:

	_Result								getPowerDetect(_Data &);
	_Result								getPowerState(_Data &);

public:
	
protected:
	/*out link set*/
	void							_setPower(bool,bool,unsigned char,bool);
	_Result							setPower(_Data &);
private:

	bool							b_ispowerFanOn;
	bool							b_ispowerOn;
	unsigned char					m_result;			
};

//////////////////////////////////////////////////////////////////////////
/*	lose point detect		 */
///////////////////////////////////////////////////////////////////////////
class		CxLosePointDetect		: public		CxDetectLeaf
{
public:

	explicit	CxLosePointDetect(CxMachine *p=NULL):CxDetectLeaf(p){}
	~CxLosePointDetect(){}

public:

	CX_CLASS_ID(COM_LOSEPOINTDETECT)

	CX_BEGIN_MSG
	CX_END_MSG

public:
};

//////////////////////////////////////////////////////////////////////////
/*	temper detect		 */
///////////////////////////////////////////////////////////////////////////
class		CxTemperDetect			:		public		CxDetectLeaf	,public		CxThread
{
public:

	explicit	CxTemperDetect(CxMachine *p=NULL):CxDetectLeaf(p),
		b_spin(true){}

	virtual		~CxTemperDetect()
	{
		b_spin=false;
		m_thread.interrupt();
		m_thread.join();
	}
public:

	CX_CLASS_ID(COM_TEMPERDETECT)
		
	CX_BEGIN_MSG
		CX_MSG(MSG_SETTEMPER,setTemper)
	CX_END_MSG



private:
	_Result							setTemper(_Data &);

public:
	const		tmpOption			&getOption()const{return  m_tmperOption;}

protected:

	void		run();

private:

	volatile	bool	b_spin;
	tmpOption			m_tmperOption;

};


//////////////////////////////////////////////////////////////////////////
/*	state detect	 */
///////////////////////////////////////////////////////////////////////////
class		CxStateDetect			:  public		CxDetectLeaf
{
public:

	explicit	CxStateDetect(CxTemperDetect* ptmper,CxMachine *p=NULL):CxDetectLeaf(p),
					m_tmperDetect(ptmper)
	{}
	virtual		~CxStateDetect(){}

public:

	CX_CLASS_ID(COM_STATEDETECT)

	CX_BEGIN_MSG
		CX_MSG(MSG_GETSTATE,stateDetect)
	CX_END_MSG

private:

	_Result					stateDetect(_Data &);

public:

private:

	void			getnLoseEff(_Data &);
	void			getledLight(_Data &);
	void			getcurrentPlay(_Data &);
	void			getpowerState(_Data &);

private:

	CxTemperDetect	*m_tmperDetect;

};

	END_NAMESPACE
END_NAMESPACE