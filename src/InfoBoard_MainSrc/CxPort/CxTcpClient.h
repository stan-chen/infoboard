/* 
*
*		Usage  :
*			 	The CxTcpClient have two mode to connect service async_connect and sync_connect
*				The async_connect have to transmit a callback function 
*				The callback function as :
*										void connectfunc(bool )
*				If  connect successfully call the connectfunc(true) , else call the connectfunc(false)
*
*/


#pragma once

#include "CxTcp.h"
#include <string>
#include <boost\signals2.hpp>
#include <boost\function.hpp>


inline void	__emptyFunc(bool	 )
{
}


class CxTcpClient   : public CxTcp
{
public:

	enum ConnectMode
	{
		async_connect,
		sync_connect
	};

	typedef boost::function<void(bool)>							_ConnectFunc;
	typedef boost::function<void()>								_AsyncFunc;
	typedef boost::function<void(const ERRCODE &,size_t)>		Async_Handler;
		
public:

	CxTcpClient(const std::string& ,unsigned short ,ConnectMode mode=sync_connect,_ConnectFunc func=__emptyFunc);
	CxTcpClient(_Socket *);
	CxTcpClient();
	~CxTcpClient();

public:
	//OpenPort
	bool		OpenPort(const std::string &,unsigned short ,ConnectMode mode=sync_connect, _ConnectFunc=__emptyFunc);

	bool		ClosePort();

	bool		isOpen()const{return m_isOpened;}

	//get address
	std::string	getAddr()const{return m_addr.to_string();}

	_ushort		getPortno()const{return m_portno;}

private:
	//async open port 
	void		Async_OpenPort(const _EndPoint &,_ConnectFunc );
	bool		Sync_OpenPort(const _EndPoint&);
	bool		Resolver_Connect(const std::string &, unsigned short );

public:
	//sync read port
	size_t		ReadPort(	Bytes	,size_t );
	size_t		WritePort(	const Bytes ,size_t );
	//async read port
	void		Async_ReadPort(Bytes ,size_t ,Async_Handler );
	void		Async_WritePort( const Bytes ,size_t ,Async_Handler );


private:
	
	void		ConnectedFunc(const ERRCODE &e,_EndPoint , _ConnectFunc );

	void		handleError(const ERRCODE &);

private:
	
	bool					m_isOpened;

private:
	_Socket					*m_socket;

//signals 
public:
	boost::signals2::signal<void()>			sig_break;
};



