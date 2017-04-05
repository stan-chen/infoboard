
/*		
*			thread interface
*/
#pragma once

#include "CxType.h"
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>

using boost::noncopyable;

class CxThread  : public noncopyable
{

protected:

	typedef boost::thread	_Thread;
	typedef boost::mutex	_Mutex;

public:
	CxThread():b_isrun(true)	
	{
		m_thread=_Thread(boost::bind(&CxThread::__ThreadFunc,this));
	}
	virtual  ~CxThread()
	{
	}

	bool		isRun()const{return b_isrun;}

//Thread funcs
protected:

	static	void	__ThreadFunc(CxThread *_this)
	{
		_this->run();
	}

	virtual	void	run()=0;

protected:
	_Thread		m_thread;
	bool		b_isrun;
};
