

#include "CxIoSevice.h"


#include <boost\bind.hpp>
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::vector;


size_t											CxIoService::s_ioref=0;		
CxIoService::_IoService							CxIoService::s_ioservice;
CxIoService::_Thread_Group						CxIoService::s_gthread;
std::vector<CxIoService::_WorkPtr>              CxIoService::s_workgroup;
CxIoService::_Mutex								CxIoService::s_mu;

CxIoService::CxIoService()
{
	++s_ioref;
	initIO();
}

CxIoService::~CxIoService()
{
	--s_ioref;
	releaseIO();
}

void	CxIoService::runFunc()
{
	ERRCODE e;

	s_workgroup.push_back(_WorkPtr(new boost::asio::io_service::work(s_ioservice)));
	
	s_ioservice.run(e);

	if(e)
	{
        handleError(e);
	}  
}

void	CxIoService::initIO()
{
	if(s_ioref!=1)return ;

	size_t processnumber=boost::thread::hardware_concurrency();

    for(int i=0;i<processnumber;++i)
        s_gthread.add_thread(new boost::thread(boost::bind<void>(&CxIoService::runFunc,this)));	
	
}

void	CxIoService::releaseIO()
{
	if(s_ioref!=0)return;

	size_t processnumber=boost::thread::hardware_concurrency();

	for(int i=0;i<processnumber;++i)
		s_workgroup[i].reset();
	
    s_ioservice.stop();

    s_gthread.join_all();

    cout<<"IO EXIT "<<endl;
}


void	CxIoService::handleError(const ERRCODE &e)
{
	cout<<e.message()<<endl;
}

