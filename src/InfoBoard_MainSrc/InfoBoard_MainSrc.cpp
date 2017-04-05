// InfoBoard_Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "CxRevThread.h"
#include "CxGlobalVariable.h"
#include "CxExplainThread.h"
#include "CxComExport.hpp"
#include "CxComBuilder.hpp"
#include "CxComPlay.hpp"

using namespace MSG_SIGN;
using namespace	MSG_SIGN::MachineComponent; 


void		pushstr(const string &str)
{
	cout<<str<<endl;
}

void		pushchar(char c)
{
	cout<<c<<endl;
}

typedef vector<vector< unsigned char> >  _ImageData;

void   play(const _ImageData& data,const showMode& ,int  light)
{
	_ImageData d=data;

	cout<<d.size()<<'\t'<<d[0].size()<<endl;
}


int _tmain(int argc, _TCHAR* argv[])
{
	CxGlobalVariable gl;
	gl.getPort()->open("COM1");


	CxComBuilder_1	builder;
	CxMachine *machine=builder.build(512,128);
	CxComExport	*comExport=new CxComExport(machine);
	comExport->sig_pushStr.connect(pushstr);
	machine->addCom(comExport);
	CxRevThread rev(gl.getPort());
	CxExplainThread	ex(machine,&rev.getBuffer());


	builder.getComPlay()->sig_show.connect(play);

	ex.sig_msg.connect(pushchar);
	cout<<"Now main thread is watting..."<<endl;
	Sleep(INFINITE);
	return 0;
}

