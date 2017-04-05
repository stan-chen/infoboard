
#pragma once


#include "CxMachine.hpp"
#include "../CxThread.hpp"
#include <boost/function.hpp>
#include <boost/timer.hpp>
#include <boost/signals2.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

BEGIN_NAMESPACE(MSG_SIGN)
	BEGIN_NAMESPACE(MachineComponent)

using boost::thread;
using std::endl;
using std::cout;

//////////////////////////////////////////////////////////////////////////
/*		time component 		*/
///////////////////////////////////////////////////////////////////////////


class CxComTime :		public CxComLeaf  ,		public		CxThread
{
	typedef boost::mutex				_Mutex;
	typedef boost::posix_time::ptime	_Time;
	typedef	boost::signals2::signal<void(const _Time&)>		_SigShowTime;
	
public:

	CxComTime(CxMachine *p=NULL):CxComLeaf(p),b_spin(true)
	{
		time_t  t;
		time(&t);
		tm *_t=localtime(&t);
		m_time=boost::posix_time::ptime_from_tm(*_t);	
		m_closeTime=_Time(m_time+boost::gregorian::years(10));
		m_openTime=_Time(m_time-boost::gregorian::years(10));
	}
	~CxComTime()
	{
		exit();
	}
public:
	_SigShowTime						sig_showTime;
	_SigShowTime						sig_showOpenTime;
	_SigShowTime						sig_showCloseTime;

public:

	boost::posix_time::ptime	getCurrent()
	{
		_Mutex::scoped_lock lock(m_mu);
		return m_time;
	}

	void		set(boost::posix_time::ptime t)
	{
		_Mutex::scoped_lock lock(m_mu);
		m_time=t;
	}

	void		exit()
	{
		_Mutex::scoped_lock lock(m_mu);
		b_spin=false;
		m_thread.interrupt();
		m_thread.join();
	}


public:

	//message and class id
	CX_CLASS_ID(COM_TIME)	

	CX_BEGIN_MSG
		CX_MSG(MSG_GETTIME,getTime)
		CX_MSG(MSG_SETTIME,setTime)
		CX_MSG(MSG_AUTOTIME,setAutoTime)
	CX_END_MSG

private:

	_Result		setAutoTime(const _Data &data)
	{
		short   h=(data[1] >>4)*10+(data[1] & 0x0F);
		short   m=(data[2] >>4)*10+(data[2] & 0x0F);
		short   _h=(data[3] >>4)*10+(data[3] & 0x0F);
		short   _m=(data[4] >>4)*10+(data[4] & 0x0F);

		m_openTime=_Time(m_time.date(),boost::posix_time::hours(h)+boost::posix_time::minutes(m));
		m_closeTime=_Time(m_time.date(),boost::posix_time::hours(_h)+boost::posix_time::minutes(_m));
		
		sig_showOpenTime(m_openTime);
		sig_showCloseTime(m_closeTime);
		return CX_OK;
	}

	_Result		setTime(const _Data &data)
	{
		if(data.size() < 7)
				return CX_FAIL;

		short   y=(data[1] >>4)*10+(data[1] & 0x0F)+2000;
		short   m=(data[2] >>4)*10+(data[2] & 0x0F);
		short   d=(data[3] >>4)*10+(data[3] & 0x0F);

		short   _h=(data[4] >>4)*10+(data[4] & 0x0F);
		short   _m=(data[5] >>4)*10+(data[5] & 0x0F);
		short   _s=(data[6] >>4)*10+(data[6] & 0x0F);

		set(boost::posix_time::ptime(boost::gregorian::date(y,m,d),
					boost::posix_time::hours(_h)+boost::posix_time::minutes(_m)+boost::posix_time::seconds(_s))
					);
	}

	_Result		getTime(_Data &data)
	{
		_Time t;
		t=getCurrent();
		data.push_back((t.date().year()-2000));
		data.push_back(t.date().month());
		data.push_back(t.date().day());
		data.push_back(t.time_of_day().hours());
		data.push_back(t.time_of_day().minutes());
		data.push_back(t.time_of_day().seconds());
		data.push_back(t.date().day_of_week());

		return CX_OK;
	}

protected:

	void		run()
	{
		_Data close(3);
		_Data open(3);
		close[1]=0;
		close[2]=0;

		open[1]=1;
		open[2]=0;
		try
		{
			while(b_spin)
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(990));
				_Mutex::scoped_lock lock(m_mu);
				m_time+=boost::posix_time::seconds(1);

				sig_showTime(m_time);

				if(m_time < m_openTime || m_time > m_closeTime)
				{
					if(getParent()!=NULL)
						getParent()->handleMsg(_Msg(COM_POWERDETECT,MSG_SETPOWERSTATE),close);
				}
				boost::this_thread::interruption_point();
			}
		}
		catch(boost::thread_interrupted &)
		{
		}
	}

	volatile	bool		b_spin;
	_Time					m_time;
	_Time					m_openTime;
	_Time					m_closeTime;
	_Mutex					m_mu;
};

	END_NAMESPACE
END_NAMESPACE