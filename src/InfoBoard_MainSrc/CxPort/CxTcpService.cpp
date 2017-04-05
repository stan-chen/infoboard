#include "CxTcpService.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;


CxTcpService::CxTcpService(size_t maxclient,unsigned short portno,_SockList *slist):
			m_socklist(slist),
			m_acceptor(m_ioService.getIoService()),
			m_maxClient(maxclient),
			m_isServiced(false)
{
	if(buildService(portno))
		async_accept();
}

CxTcpService::CxTcpService():
			m_socklist(NULL),
			m_acceptor(m_ioService.getIoService()),
			m_maxClient(1),
			m_isServiced(false)
{
}



CxTcpService::~CxTcpService()
{
	breakService();
}

//open portnumber 
bool	 CxTcpService::buildService(_ushort portno)
{
	_EndPoint	_ep(boost::asio::ip::tcp::v4(),portno);

	m_addr=_ep.address();

	ERRCODE ec;
	m_acceptor.open(_ep.protocol(),ec);

	if(!ec)
	{
		m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		m_acceptor.bind(_ep);
		m_acceptor.listen();
		m_portno=portno;
		m_isServiced=true;
		return true;
	}

	else 
	{
		handleError(ec);
		return false;
	}
}

bool CxTcpService::breakService()
{
	m_acceptor.cancel();
	m_acceptor.close();
	return false;
}


/*   async accept all */ 
void	CxTcpService::async_accept()
{
	if(!isServiced())return ;

	CX_ASSERT(m_acceptor.is_open());

	_Socket *psock=new _Socket(m_ioService.getIoService());

	m_acceptor.async_accept(*psock,boost::bind<void>(&CxTcpService::accpet_handle,this,psock,
														boost::asio::placeholders::error));

}

void	CxTcpService::accpet_handle(_Socket *psock,const ERRCODE &ec)
{
	cout<<"Have Connect...."<<endl;
	async_accept();
	if(!ec)
	{
		if(m_socklist != NULL && m_socklist->size() < m_maxClient)
			m_socklist->push_back(new CxTcpClient(psock));
		else 
		{
			delete psock;
			cout<<"list is NULL"<<endl;
		}
	}
	else 
	{
		delete psock;
		handleError(ec);
	}
}



/*   async accept one client*/ 
void	CxTcpService::async_accept_one(CxTcpClient *& pclient)
{
	if(!isServiced())return ;
	CX_ASSERT(m_acceptor.is_open());

	_Socket *psock=new _Socket(m_ioService.getIoService());

	m_acceptor.async_accept(*psock,boost::bind<void>(&CxTcpService::accept_handle_one,this,pclient,psock,
														boost::asio::placeholders::error));

}


void	CxTcpService::accept_handle_one(CxTcpClient * &pclient,_Socket *psock,const ERRCODE &ec)
{
	cout<<"Have Connect...."<<endl;
	if(ec) 
	{
		delete psock;
		pclient=NULL;
		handleError(ec);
	}
	else 
	{
		pclient=new CxTcpClient(psock);
	}
}

void  CxTcpService::sync_accept_one(CxTcpClient *&pclient)
{
	if(!isServiced())return ;

	CX_ASSERT(m_acceptor.is_open());

	_Socket *psock=new _Socket(m_ioService.getIoService());
	ERRCODE ec;
	m_acceptor.accept(*psock,ec);
	if(ec)
	{
		handleError(ec);
		pclient=NULL;
	}
	else 
	{
		pclient=new CxTcpClient(psock);
	}

}

/* Static  member function connect one client*/

bool				CxTcpService::sync_wait_one(_ushort portno,CxTcpService *&server	/*_IN OUT */ , 
		CxTcpClient * &pclient /*_IN OUT*/)
{
	server=new CxTcpService;
	server->buildService(portno);
	server->sync_accept_one(pclient);
	if(pclient==NULL)
		return false;
	else 
		return true;

}

void			CxTcpService::async_wait_one(_ushort portno , CxTcpService * &server,	 
		CxTcpClient * &pclient, _ConnectFunc func)
{
	server=new CxTcpService;
	server->buildService(portno);
	server->async_accept_one(pclient);
}


void	CxTcpService::handleError(const ERRCODE &e)
{
	m_acceptor.close();
	m_portno=0;
	m_isServiced=false;
	cout<<e.message()<<endl;
}
