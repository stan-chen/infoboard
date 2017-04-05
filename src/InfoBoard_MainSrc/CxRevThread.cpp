

#include "CxType.h"
#include "CxRevThread.h"




using namespace MSG_SIGN;


//Init  member  variable
CxRevThread::CxRevThread(_Serial *port):m_mode(mode_serial),m_protohandle(new CxProtocolHandle_1),
										m_serialport(port),m_tcpclient(NULL),
										m_transbuffer(1024),b_spin(true)
{
	port->sig_exitsignal.connect(boost::bind(&CxRevThread::slot_portExit,this));
}


CxRevThread::CxRevThread(_TcpClient *port):m_mode(mode_tcp),m_protohandle(new CxProtocolHandle_1),
										m_serialport(NULL),m_tcpclient(port),
			/*the buffer is infinite*/	m_transbuffer(1024),b_spin(true)
{
}


/*  destory  , must close port first */
CxRevThread::~CxRevThread()
{
	if(m_serialport!=NULL)
		m_serialport->close();	
	b_spin=false;
	m_thread.interrupt();
	m_thread.join();
}


/*  handle port exit signals */
void			CxRevThread::slot_portExit()
{
	switch(m_mode)
	{
	case mode_serial:
		//m_serialport=NULL;
		break;
	case mode_tcp:
		//m_tcpclient=NULL;
		break;
	}
}


/*  switch  to another mode */
inline void  CxRevThread::switchMode( _TcpClient *port)
{
	CX_ASSERT(port);
	_Mutex::scoped_lock lock(m_mu);
	switch(m_mode)
	{
	case mode_serial:
		m_serialport=NULL;
		m_mode=mode_tcp;
		m_tcpclient=port;
		break;
	case mode_tcp:
		m_tcpclient=port;
		break;
	}
}


inline void  CxRevThread::switchMode(_Serial *port)
{
	CX_ASSERT(port);
	_Mutex::scoped_lock lock(m_mu);
	switch(m_mode)
	{
	case mode_tcp:
		m_tcpclient=NULL;
		m_mode=mode_serial;
		m_serialport=port;
		break;
	case mode_serial:
		m_serialport=port;
		break;
	}

	port->sig_exitsignal.connect(boost::bind(&CxRevThread::slot_portExit,this));
}

void		CxRevThread::run()
{
	CX_ASSERT(b_spin);
	//Main handle
	while(b_spin)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));

		//lock the port
		_Mutex::scoped_lock lock(m_mu);

		switch(m_mode)
		{
		case mode_serial:
			if(/*m_serialport!=NULL && */m_serialport->isOpen())
				m_protohandle->HandlePort(*m_serialport,m_transbuffer);
			break;
		case mode_tcp:
			if(/*m_tcpclient!=NULL &&*/ m_tcpclient->isOpen())
				m_protohandle->HandlePort(*m_tcpclient,m_transbuffer);
			break;
		}

		boost::this_thread::interruption_point();
	}
}