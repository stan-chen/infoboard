
#include "CxTcpClient.h"
#include <boost\lexical_cast.hpp>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

CxTcpClient::CxTcpClient(const string &portname,unsigned short portno,ConnectMode mode,_ConnectFunc _func):
				m_socket(new _Socket(m_ioService.getIoService())),
				m_isOpened(false)
{
	CX_ASSERT(!portname.empty());
	OpenPort(portname,portno,mode,_func);
}

CxTcpClient::CxTcpClient(_Socket *psock):
				m_isOpened(false)
{

	if(psock==NULL)
		m_socket=new _Socket(m_ioService.getIoService());

	else 
	{
		m_socket=psock;
		m_isOpened = psock->is_open() ? true :false; 
		m_addr=psock->remote_endpoint().address();
		m_portno=psock->remote_endpoint().port();
	}
}

CxTcpClient::CxTcpClient():
				m_socket(new _Socket(m_ioService.getIoService())),
				m_isOpened(false)
{
}


CxTcpClient::~CxTcpClient()
{
	ClosePort();
	delete m_socket;
}


bool	CxTcpClient::ClosePort()
{
	if(!isOpen())
		return true;

    m_socket->cancel();
	m_socket->close();
	m_portno=-1;
	
	//verdicate param false
	m_isOpened=false;

	//sent break signal

	sig_break();
	return true;
}

bool	CxTcpClient::OpenPort(const string &portname,unsigned short portno,ConnectMode mode,_ConnectFunc _func)
{
	 CX_ASSERT(!portname.empty());

	 ERRCODE ec;

	 _Addr addr=_Addr::from_string(portname,ec);
	
	if(ec)
	{
		if(mode==async_connect)
		{
			_func(false);
			return false;
		}
		else 
		{
			//iterator connect
			return Resolver_Connect(portname,portno);
		}
	}

	//ip addr connect 
	_EndPoint	_ep(addr,portno);

	switch(mode)
	{
	case async_connect:
		Async_OpenPort(boost::ref(_ep),_func);
		break;

	case sync_connect:
		return Sync_OpenPort(boost::ref(_ep));

	default:
		break;
	}

	return true;
}

void		CxTcpClient::Async_OpenPort(const _EndPoint &_ep,_ConnectFunc func)
{
	m_socket->async_connect(_ep,boost::bind<void>(&CxTcpClient::ConnectedFunc,this,
													boost::asio::placeholders::error,_ep,boost::ref(func))
						  );
}


bool			CxTcpClient::Sync_OpenPort(const _EndPoint & _ep)
{
	ERRCODE e;
	m_socket->connect(_ep,e);

	if(e)
	{
		handleError(e);
		return false;
	}
	else 
	{
		m_isOpened=true;
		m_addr=_ep.address();
		m_portno=_ep.port();
		return true;
	}
}


//Resolver connect
bool		CxTcpClient::Resolver_Connect(const std::string &name, unsigned short no)
{
	_Resolver			solv(m_ioService.getIoService());
	_Resolver::query	quy(name , boost::lexical_cast<string>(no) );

	_Resolver::iterator  iter=solv.resolve(quy);

	_Resolver::iterator	 end;
	ERRCODE				 ec=boost::asio::error::host_not_found;

	for(;ec && iter!=end;++iter)
	{
		m_socket->close();
		m_socket->connect(*iter,ec);
	}
	if(!ec)
	{
		m_isOpened=true;
		m_portno=no;
		m_addr=iter->endpoint().address();
		return true;
	}
	else 
	{
		handleError(ec);

		return false;
	}
}


//handle  connect func
void	CxTcpClient::ConnectedFunc(const ERRCODE &e,_EndPoint _ep,_ConnectFunc func)
{
	if(e)
	{
        m_socket->cancel();
		handleError(e);
		func(false);
	}
	else 
	{
		m_isOpened=true;
		m_addr=_ep.address();
		m_portno=_ep.port();
		func(true);
	}
}


void	CxTcpClient::handleError(const ERRCODE &e)
{
	m_socket->close();
	m_portno=-1;
	m_isOpened=false;

	cout<<e.message()<<endl;
}


/*   Direct  Read Port handle*/
size_t		CxTcpClient::ReadPort(Bytes _buf,size_t _size)
{
	if(!isOpen())
		return 0;

	CX_ASSERT(_buf);
	CX_ASSERT(_size);

	ERRCODE e;
	size_t rtsize=m_socket->read_some(boost::asio::buffer(_buf,_size),e);

	if(e)
	{
		ClosePort();
		handleError(e);
	}
	return rtsize;
}

/*   Direct  Wirte	Port handle*/
size_t		CxTcpClient::WritePort(	const Bytes _buf,size_t _size)
{
	if(!isOpen())return 0;
	CX_ASSERT(_buf);
	CX_ASSERT(_size);

	ERRCODE e;
	size_t rtsize=m_socket->write_some(boost::asio::buffer(_buf,_size),e);

	if(e)
	{
		ClosePort();
		handleError(e);
	}
	return rtsize;

}

/*Async Read Port*/
void		CxTcpClient::Async_ReadPort(Bytes _buf,size_t _size,Async_Handler  handler)
{
	if(!isOpen())return ;
	CX_ASSERT(_buf);
	CX_ASSERT(_size);

	m_socket->async_read_some(boost::asio::buffer(_buf,_size),
												boost::bind<void>(handler,
															boost::asio::placeholders::error,
															boost::asio::placeholders::bytes_transferred
															)
							);

}

/*Async Write port*/
void		CxTcpClient::Async_WritePort(const Bytes _buf,size_t _size, Async_Handler handler)
{
	if(!isOpen())return ;
	CX_ASSERT(_buf);
	CX_ASSERT(_size);

	m_socket->async_write_some(boost::asio::buffer(_buf,_size),
												boost::bind<void>(handler,
															boost::asio::placeholders::error,
															boost::asio::placeholders::bytes_transferred
															)
							 );
}