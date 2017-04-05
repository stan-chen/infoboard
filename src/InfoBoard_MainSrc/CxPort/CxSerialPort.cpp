
#include "CxSerialPort.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

CxSerialPort::CxSerialPort(const std::string &portname,t_baud_rate  rate,
							t_char_size charsize,t_parity par,
							t_stop_bits stop):
								m_port(m_ioservice.getIoService()),
								b_isopened(false)
{
	open(portname,rate,charsize,par,stop);
}

CxSerialPort::CxSerialPort():m_port(m_ioservice.getIoService()),b_isopened(false)
{
}

CxSerialPort::~CxSerialPort()
{
	close();
}

bool		CxSerialPort::open(const string		&portname,	t_baud_rate	rate,
								t_char_size charsize	,	t_parity par ,
								t_stop_bits stop
								)
{

	ERRCODE ec;
	m_port.open(portname,ec);
	if(ec)
	{
//		cout<<ec.message()<<endl;
        sig_errorsignal(ec);
		b_isopened=false;
		return false;
	}
	else 
	{
		setOption(rate);
		setOption(charsize);
		setOption(par);
		setOption(stop);

		b_isopened=true;
		return true;
	}
}


void		CxSerialPort::close()
{
	if(!isOpen())
		return ;
	b_isopened=false;
	m_port.cancel();
	m_port.close();
	sig_exitsignal();
}


size_t		CxSerialPort::sync_read(vector<byte> &buffer,size_t size)
{
	if(!isOpen())return 0;

	CX_ASSERT(size);
	ERRCODE ec;
	buffer.resize(size);	
	size_t  rtsize=m_port.read_some(boost::asio::buffer(buffer),ec);

	if(ec)
	{
		handleError(ec);
		return 0;
	}
	
	else return rtsize;
}

size_t		CxSerialPort::sync_write(const vector<byte> &buffer)
{
	if(!isOpen())return 0;

	CX_ASSERT(buffer.size());

	ERRCODE ec;
	size_t  rtsize=m_port.write_some(boost::asio::buffer(buffer),ec);

	if(ec)
	{
		handleError(ec);
		return 0;
	}
	else return rtsize;
}

size_t		CxSerialPort::sync_read(bytes buffer,size_t size)
{
	if(!isOpen())return 0;

	CX_ASSERT(size);

	ERRCODE ec;
	size_t  rtsize=m_port.read_some(boost::asio::buffer(buffer,size),ec);

	if(ec)
	{
		handleError(ec);
		return 0;
	}

	else return rtsize;
}

size_t		CxSerialPort::sync_write(const bytes buffer,size_t size)
{
	if(!isOpen())return 0;

	CX_ASSERT(size);

	ERRCODE ec;
	size_t  rtsize=m_port.write_some(boost::asio::buffer(buffer,size),ec);

	if(ec)
	{
		handleError(ec);
		return 0;
	}
	else return rtsize;
}

void		CxSerialPort::async_read(bytes buffer, size_t size, _FinishHandler handler)
{
	if(!isOpen())return ;
	
	m_port.async_read_some(boost::asio::buffer(buffer,size),
				boost::bind(&CxSerialPort::async_handler,this,handler,_1,_2));

}

void		CxSerialPort::async_write(const bytes buffer,size_t  size,_FinishHandler handler)
{
	if(!isOpen())return ;
	m_port.async_write_some(boost::asio::buffer(buffer,size),
				boost::bind(&CxSerialPort::async_handler,this,handler,_1,_2));
}

void		CxSerialPort::async_handler(_FinishHandler handler,
										const ERRCODE &ec ,		size_t size
										)
{
	if(ec)
	{
		handleError(ec);
		handler(false);
	}
	else 
	{
		handler(true);
	}
}

void			CxSerialPort::handleError(const ERRCODE &e)
{
	cout<<e.message()<<endl;
	close();
	sig_errorsignal(e);
}
