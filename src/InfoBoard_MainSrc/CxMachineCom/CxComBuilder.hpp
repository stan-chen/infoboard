

#pragma once

#include "CxMachine.hpp"

BEGIN_NAMESPACE(MSG_SIGN)
	BEGIN_NAMESPACE(MachineComponent)

	class CxMachine;
	class CxComDetect;
	class CxComStorage;
	class CxComScene;

	class CxComExport;
	class CxComImport;

	/* main component */
	class CxComPlay	;
	class CxComTime	;
		
	/* detect component */
	class CxPowerDetect	;
	class CxLosePointDetect;
	class CxTemperDetect;
	class CxStateDetect	;

	/* storage component */
	class CxImgStorage;

	/* scene component */
	class  CxLedScene	;


/*abstruct  builder*/
class		CxComBuilder			:public		CxComLeaf
{
public:
	CxComBuilder(CxMachine *p=NULL):CxComLeaf(p){}

	virtual		~CxComBuilder(){}
public:

	virtual		CxMachine *			build(int cx,int cy)=0;
};




class		CxComBuilder_1			: public	CxComBuilder
{
private:
	CxMachine				*com;
	CxComDetect				*comDetect;
	CxComStorage			*comStorage;
	CxComScene				*comScene;

	CxComExport				*comExport;
	CxComImport				*comImport;

	/* main component */
	CxComPlay				*comPlay;
	CxComTime				*comTime;
		
	/* detect component */
	CxPowerDetect			*powerDetect;
	CxLosePointDetect		*loseDetect;
	CxTemperDetect			*temperDetect;
	CxStateDetect			*stateDetect;

	/* storage component */
	CxImgStorage			*imgStorage;

	/* scene component */
	CxLedScene				*ledScene;

public:

	CxComBuilder_1(CxMachine *p=NULL):CxComBuilder(p)
	{
	}

	virtual		~CxComBuilder_1()
	{
		deleteAll();
	}
	
	void	deleteAll();

public:

	virtual		CxMachine *			build(int ,int );
	
protected:

	virtual		CxMachine	*makeComTime(CxMachine *);
	virtual		CxMachine	*makeComDetect(CxMachine *);
	virtual		CxMachine	*makeComStorage(CxMachine *);
	virtual		CxMachine	*makeScene(CxMachine *,int ,int );

public:

	CX_CLASS_ID(COM_BUILDER)

public:

	CxPowerDetect	*				getComPower(){return powerDetect;}
	CxComPlay		*				getComPlay(){return comPlay;}
	CxComTime		*				getComTime(){return comTime;}
	CxComExport		*				getComExport(){return comExport;}
	CxComImport		*				getComImport(){return comImport;}

};


	END_NAMESPACE 
END_NAMESPACE
