
/*
*		Rev thread implement
*/

#pragma once

#include "CxThread.hpp"
#include "CxProtocolHandle.h"

BEGIN_NAMESPACE(MSG_SIGN)

class CxRevThread: public CxThread
{
public:
	typedef CxLinerBuffer<CxTransaction *>		_Buffer;
	typedef CxSerialPort						_Serial;
	typedef CxTcpClient							_TcpClient;

public:

	CxRevThread(_Serial *);
	CxRevThread(_TcpClient *);

	~CxRevThread();

protected:

	void	run();

public:

	//Port mode
	enum Mode
	{
		mode_serial,
		mode_tcp
	};
public:
	//interface 
	_Buffer			&getBuffer(){return m_transbuffer;}

	void			switchMode(_Serial		*);
	void			switchMode(_TcpClient	* );
private:

	void			slot_portExit();


private:
	volatile	bool	b_spin;
	Mode				m_mode;

	CxSerialPort		*m_serialport;
	CxTcpClient			*m_tcpclient;
	CxProtocolHandle	*m_protohandle;


	_Mutex				m_mu;
	_Buffer				m_transbuffer;

};

END_NAMESPACE