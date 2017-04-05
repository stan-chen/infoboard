

#pragma once
#include "CxType.h"
#include "CxTransaction.h"
#include "CxPort/CxTcpClient.h"
#include "CxPort/CxSerialPort.h"
#include "CxPort/CxBuffer.hpp"
#include <boost/noncopyable.hpp>

BEGIN_NAMESPACE(MSG_SIGN)

using boost::noncopyable;

class CxProtocolHandle   :	public noncopyable
{
protected:
	typedef		CxSerialPort					_Serial;
	typedef		CxTcpClient						_TcpClient;
	typedef		CxLinerBuffer<CxTransaction *>	_Buffer;

public:

	CxProtocolHandle(){}
	virtual	~CxProtocolHandle(){}

public:

	virtual		void	HandlePort(_Serial &,_Buffer & )=0;
	virtual		void	HandlePort(_TcpClient &,_Buffer& )=0;

};

class		CxProtocolHandle_1  : public  CxProtocolHandle
{
	typedef  unsigned char		byte;
	typedef  unsigned char*		bytes;

public:

	CxProtocolHandle_1(){}
	virtual	~CxProtocolHandle_1(){}

public:

	void	HandlePort(_Serial &,_Buffer & );
	void	HandlePort(_TcpClient &,_Buffer& );

};



END_NAMESPACE