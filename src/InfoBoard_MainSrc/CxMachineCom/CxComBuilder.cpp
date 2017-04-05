
#include "CxComPlay.hpp"
#include "CxComScene.hpp"
#include "CxComStorage.hpp"
#include "CxComTime.hpp"
#include "CxComExport.hpp"
#include "CxComImport.hpp"
#include "CxComDetect.hpp"
#include "CxComBuilder.hpp"

using namespace MSG_SIGN::MachineComponent;


CxMachine *			CxComBuilder_1::build(int cx,int cy)
{
	com=new CxComponent;

	com->addCom(makeComTime(com));
	com->addCom(makeComDetect(com));
	com->addCom(makeComStorage(com));
	com->addCom(makeScene(com,cx,cy));

	comExport = new CxComExport(com);
	comImport =	new CxComImport(com);

	com->addCom(comExport);
	com->addCom(comImport);
	return com;
}


CxMachine	*CxComBuilder_1::makeComTime(CxMachine *p)
{
	comTime	= new CxComTime(p);
	return comTime;
}

CxMachine	*CxComBuilder_1::makeComDetect(CxMachine *p)
{
	comDetect=new CxComDetect(p);

	powerDetect=new CxPowerDetect(comDetect);
	loseDetect=new CxLosePointDetect(comDetect);

	temperDetect=new CxTemperDetect(comDetect);
	stateDetect=new CxStateDetect(temperDetect,comDetect);

	comDetect->addCom(powerDetect);
	comDetect->addCom(loseDetect);
	comDetect->addCom(temperDetect);
	comDetect->addCom(stateDetect);

	return comDetect;
}

CxMachine	*CxComBuilder_1::makeComStorage(CxMachine *p)
{
	comStorage=new CxComStorage(p);
	imgStorage=new CxImgStorage(comStorage);
	comPlay=new CxComPlay(imgStorage,comStorage);

	comStorage->addCom(imgStorage);
	comStorage->addCom(comPlay);

	return  comStorage;
}

CxMachine		*CxComBuilder_1::makeScene(CxMachine *p,int cx,int cy)
{
	comScene=new CxComScene(p);
	ledScene=new CxLedScene(cx,cy,comScene);

	comScene->addCom(ledScene);
	return comScene;
}




void	CxComBuilder_1::deleteAll()
{
	delete	ledScene;
	delete	imgStorage;

	delete	stateDetect;
	delete	temperDetect;
	delete  loseDetect;
	delete  powerDetect;

	delete	comTime;
	delete	comPlay;

	delete  comExport;
	delete	comImport;

	delete	comScene;
	delete	comStorage;
	delete	comDetect;
	delete	com;
}