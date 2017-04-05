

#pragma once

#include <boost\assert.hpp>
#include <boost\asio.hpp>
#include <boost\ref.hpp>
#include <vector>
#include <boost\noncopyable.hpp>
#include <boost\thread.hpp>
#include <boost\shared_ptr.hpp>



#define	CX_ASSERT(expr)		BOOST_ASSERT(expr)


class CxIoService : public boost::noncopyable
{
public:

	typedef	boost::system::error_code			ERRCODE;
	typedef boost::asio::io_service				_IoService;
	typedef boost::thread_group					_Thread_Group;
	typedef boost::asio::io_service::work		_IoWork;
	typedef boost::mutex						_Mutex;
	typedef boost::shared_ptr<_IoWork>			_WorkPtr;

public:

	CxIoService();
	~CxIoService();

public:

	_IoService&	getIoService()const{return boost::ref(s_ioservice);}


private:

	void		initIO();
	void		releaseIO();

	void		runFunc();
	void		handleError(const ERRCODE &e);

private:

	static		size_t								s_ioref;		
	static		_IoService							s_ioservice;
	static		_Thread_Group						s_gthread;
	static		_Mutex								s_mu;
	static		std::vector<_WorkPtr>               s_workgroup;

};


//////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////

