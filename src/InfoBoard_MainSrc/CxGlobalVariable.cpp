
#include "CxGlobalVariable.h"

#include "CxComBuilder.hpp"
using namespace MSG_SIGN;
using namespace MSG_SIGN::MachineComponent;


unsigned char						CxGlobalVariable::s_temper=37;
unsigned char						CxGlobalVariable::s_smoke=0;
	
CxSerialPort						*CxGlobalVariable::s_globalPort;
boost::mutex						CxGlobalVariable::s_globalmu;
CxMachine							*CxGlobalVariable::s_machine=NULL;
MachineComponent::CxComBuilder		*CxGlobalVariable::s_builder=NULL;

void		CxGlobalVariable::fromBuilder(MachineComponent::CxComBuilder	*builder,int cx,int cy)
{
	s_builder=builder;
	s_machine=builder->build(cx,cy);
}