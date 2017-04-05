
#pragma once

#include "CxMachine.hpp"
#include "CxComStorage.hpp"
#include "../CxThread.hpp"
#include <boost/signals2.hpp>

BEGIN_NAMESPACE(MSG_SIGN)
	BEGIN_NAMESPACE(MachineComponent)

	
//////////////////////////////////////////////////////////////////////////
/*	play  component	 */
///////////////////////////////////////////////////////////////////////////

class	CxComPlay		:		public		CxComLeaf , public	CxThread
{
	typedef		vector<	_Data >					_ImageData;
	typedef		boost::mutex					_Mutex;
	typedef		boost::condition_variable_any	_Cond;
	typedef		boost::signals2::signal <void(const _ImageData&,const showMode&,int)>	_SigShow;		
	typedef		vector<showMode>				_vShowMode;	

public:

	/* show image signals */
	_SigShow			sig_show;

public:

	CxComPlay(CxImgStorage *storage,CxMachine *p=NULL):CxComLeaf(p),m_storage(storage),
		b_spin(true),m_currentPic(0),b_isPause(true),m_light(16)
	{
		storage->sig_pause.connect(boost::bind(&CxComPlay::pauseShow,this));
		storage->sig_reset.connect(boost::bind(&CxComPlay::resetShow,this));
	}

	virtual		~CxComPlay()
	{
		b_spin=false;
		m_thread.interrupt();
		m_thread.join();
	}

public:

	CX_CLASS_ID(COM_PLAYOPTION)


	CX_BEGIN_MSG
		CX_MSG(MSG_SETPROLIST,setProlist)
		CX_MSG(MSG_GETCURRENTPLAY,getCurrentPlay)
		CX_MSG(MSG_GETLEDLIGHT,getLedLight)
		CX_MSG(MSG_SETLEDLIGHT,setLedLight)
	CX_END_MSG


private:

	_Result					getCurrentPlay(_Data &);
	_Result					setProlist(const _Data &d);
	_Result					getLedLight(_Data &);
	_Result					setLedLight(const _Data&);


protected:

	void					pauseShow()
	{
		b_isPause=true;
	}

	void					resetShow()
	{	
		m_currentPic=0;
		b_isPause=false;
		m_showcond.notify_one();
	}

protected:

	
	void					play(size_t  npic);
	void					run();
	
private:

	volatile	bool		b_spin;
	volatile	bool		b_isPause;
	volatile	size_t		m_currentPic;
	unsigned	char		m_light;

	_Mutex					m_mu;
	_Cond					m_showcond;

	CxImgStorage			*m_storage;
	_vShowMode				m_showMode;
};

	END_NAMESPACE
END_NAMESPACE
