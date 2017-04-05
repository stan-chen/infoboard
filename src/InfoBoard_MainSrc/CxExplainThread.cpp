
#include "CxMachine.hpp"
#include "CxExplainThread.h"
#include "CxTransaction.h"
#include "CxComDef.h"
#include <iostream>
using std::cout;
using std::endl;

using namespace MSG_SIGN;
using namespace MSG_SIGN::MachineComponent;

CxExplainThread::CxExplainThread(CxMachine *machine,_Buffer *buffer):
					m_machine(machine),m_transqueue(buffer),b_spin(true)
{
}
CxExplainThread::~CxExplainThread()
{
}
void		CxExplainThread::run()
{
	while(b_spin)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(30));
		CxTransaction  * trans = m_transqueue->Pop();
		trans->exec(m_machine);
		char comm=trans->getid();
        //sig_msg(comm);
        (comm!='Y' && comm!='G')?sig_msg(comm):0;
		delete trans;	
	}
}
