

#pragma once

#include "CxIoSevice.h"

class CxTcp
{
public:
	CxTcp():m_portno(0){}
	virtual	~CxTcp(){}


protected:

	typedef boost::asio::ip::address						_Addr;
	typedef boost::system::error_code						ERRCODE;
	typedef boost::asio::ip::tcp::endpoint					_EndPoint;
	typedef boost::asio::ip::tcp::socket					_Socket;
	typedef boost::asio::ip::tcp::acceptor					_Acceptor;

	typedef boost::asio::ip::tcp::resolver					_Resolver;
	typedef boost::thread									_Thread;
	typedef CxIoService::_Mutex								_Mutex; 
	typedef unsigned char*									Bytes; 
	typedef unsigned short									_ushort;

	typedef boost::function<void(bool)>						_ConnectFunc;

protected :

	_Addr			    m_addr;
	_ushort				m_portno;
	CxIoService			m_ioService;
};