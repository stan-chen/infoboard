
#pragma once

#include "CxIoSevice.h"
#include <vector>
#include <boost\bind.hpp>
#include <boost\signals2.hpp>

using	std::vector;
using	boost::noncopyable;
using	boost::function;

class CxSerialPort : public noncopyable
{
	//type
	typedef unsigned char				byte;
	typedef unsigned char*				bytes;

	typedef boost::mutex				_Mutex;

	// function define
	typedef boost::system::error_code	ERRCODE;
    typedef boost::function<void(bool)>			_FinishHandler;
	typedef 
			boost::signals2::signal<void(const ERRCODE &)>	
										_ErrorSignal;
	typedef 
			boost::signals2::signal<void()>	
										_ExitSignal;

	typedef boost::asio::serial_port	_SerialPort;
	//Option	
	typedef _SerialPort::baud_rate				baud_rate;
	typedef _SerialPort::character_size			char_size;
	typedef _SerialPort::parity					parity;
	typedef _SerialPort::stop_bits				stop_bits;
	
public:
	//Option Type
	typedef _SerialPort::parity::type			t_parity;
	typedef _SerialPort::stop_bits::type		t_stop_bits;
	enum t_baud_rate
	{
		baud_rate_9600=9600,
		baud_rate_19200=19200,
		baud_rate_28800=28800,
		baud_rate_38400=38400,
		baud_rate_43000=43000,
		baud_rate_56000=56000,
		baud_rate_57600=57600,
		baud_rate_115200=115200,
		baud_rate_128000=128000,
		baud_rate_256000=256000,
	};
	enum t_char_size
	{
		char_8 = 8,
		char_7 = 7,
		char_6 = 6
	};

public:
	//constructor
	CxSerialPort(const std::string &,t_baud_rate  rate=baud_rate_9600	,
				t_char_size charsize=char_8,t_parity par=parity::none ,
				t_stop_bits stop=stop_bits::one 
				);

	CxSerialPort();
	~CxSerialPort();


/* handle function */
public:
	bool		open(const std::string &, t_baud_rate  rate=baud_rate_9600	,
						t_char_size charsize=char_8,t_parity par=parity::none ,
						t_stop_bits stop=stop_bits::one);

	void		close();
	bool		isOpen()const{return b_isopened;}




//read and write handle
public:
	
	size_t		sync_read(vector<byte> &,size_t );
	size_t		sync_write(const vector<byte> &);

	size_t		sync_read(bytes ,size_t );
	size_t		sync_write(const bytes,size_t);

	//async read & write handler 
	void		async_read(bytes , size_t , _FinishHandler );
	void		async_write(const bytes ,size_t ,_FinishHandler);

	void		async_handler(_FinishHandler ,const ERRCODE &,size_t  );

/*set option*/
public:

	void		setOption(t_baud_rate	rate)
	{t_setOption(baud_rate(rate));}
	void		setOption(t_char_size	charsize)
	{t_setOption(char_size(charsize));}
	void		setOption(t_parity		par)
	{t_setOption(parity(par));}
	void		setOption(t_stop_bits	stop)
	{t_setOption(stop_bits(stop));}
	

private:
	//set option template
	template<typename Type>
	void		t_setOption(Type t)
	{
		ERRCODE ec;
		m_port.set_option(t,ec);
		if(ec)
		{
			cout<<ec.message()<<endl;
		}
	}

	void			handleError(const ERRCODE &e);


/* Error signal when error occurence */
public:
	_ErrorSignal	sig_errorsignal;

	_ExitSignal		sig_exitsignal;
	
private:
	bool			b_isopened;
	CxIoService		m_ioservice;
	_SerialPort		m_port;
};

