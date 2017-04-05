
#pragma once


#include "CxClientList.hpp"
#include "CxTcpClient.h"
#include <string>


class CxTcpService : public boost::noncopyable ,public CxTcp
{
	typedef CxClientList		_SockList;

public:
	//async service
	CxTcpService(size_t ,unsigned short ,_SockList *slist=new _SockList());
	~CxTcpService();
	
private:
	CxTcpService();

public:

	/*static factory member function connect one*/

	//connect one client
	static		bool				sync_wait_one(_ushort ,CxTcpService * &	/*_IN OUT */ , 
													CxTcpClient *&  /*_IN OUT*/);

	static		void				async_wait_one(_ushort ,CxTcpService *& /*IN OUT */ ,
													CxTcpClient *& /*IN OUT*/ ,_ConnectFunc func=__emptyFunc);

public:

	//verdicate 
	bool		isServiced()const {return m_isServiced;}

public:

	/* get client list  , its  capacity is the  max client number */

	_SockList	*getList()const{return m_socklist;}

private:

	//create service with  async
	bool	buildService(_ushort );

	//break service
	bool	breakService();

private:

	//async  accept
	void	async_accept();
	void	async_accept_one(CxTcpClient *&/*IN OUT */);

	void	accpet_handle(_Socket *,const ERRCODE &	);
	void	accept_handle_one(CxTcpClient *&/* OUT */,_Socket *psock,const ERRCODE &);

	//sync  accept

	void	sync_accept_one(CxTcpClient *&);
	
	//handle  error function , this  function to handle every posible error 
	void	handleError(const ERRCODE&);

private:

	//verdicate member to  verdicate the state of this service
	bool				m_isServiced;
	size_t				m_maxClient;


	//recv socket list cap the every  link client 
	_Acceptor			m_acceptor;
	_SockList			*m_socklist;
	
};


