
#include "CxComPlay.hpp"
using namespace MSG_SIGN::MachineComponent;




_Result			CxComPlay::getCurrentPlay(_Data &data)
{
	data[0]=m_currentPic;
	return CX_OK;
}

_Result			CxComPlay::getLedLight(_Data &data)
{
	data.push_back(m_light);
	return CX_OK;
}

_Result			CxComPlay::setLedLight(const _Data &data)
{
	m_light=(data.size()==0)?16:data[1];
	return CX_OK;
}


_Result			CxComPlay::setProlist(const _Data &d)
{
	int  npic=d[1];
	{
		m_showMode.clear();
		m_showMode.resize(npic);

		for(int i=0;i<npic;++i)
		{
			m_showMode[i].m_showmodeIn		=		(d[2+i*4]>>4) & 0x0F;
			m_showMode[i].m_showmodeOut		=		d[2+i*4] & 0x0F;
			m_showMode[i].b_ispicSeq		=		d[3+i*4] >> 7 ;
			m_showMode[i].m_showSpeed		=		(d[3+i*4] & 0x30) >> 4;
			m_showMode[i].m_timeshowPos		=		(d[3+i*4] & 0x0C) >> 2;
			m_showMode[i].m_timeshowMode	=		d[3+i*4] & 0x03;
			m_showMode[i].b_staytimeUnit	=		d[4+i*4] >> 7;		
			if(!m_showMode[i].b_staytimeUnit)
					m_showMode[i].m_staytime =	(d[5+i*4]< 4)?4:d[5+i*4];
		}

		return CX_OK;
	}
}


void			CxComPlay::play(size_t  npic)
{	
	_ImageData d;

	if(!CX_CHECK(m_storage->getImage(npic,d)))
		return ;
	else 
	{	
		char buffer[64];
		sprintf(buffer,"Now play the %d picture..",npic);
		outputInfo(buffer);
		sig_show(d,m_showMode[npic],m_light);
	}
}

void		CxComPlay::run()
{
	boost::this_thread::sleep(boost::posix_time::milliseconds(2));

	int  picmax;

	boost::posix_time::time_duration dur;

	while(b_spin)
	{
		picmax=m_storage->getnPic();

		_Mutex::scoped_lock lock(m_mu);

		if(picmax==0 || b_isPause)
		{
			m_showcond.wait(m_mu);
			picmax=m_storage->getnPic();
		}

		if(!m_showMode[m_currentPic].b_staytimeUnit)
			dur=boost::posix_time::seconds(m_showMode[m_currentPic].m_staytime);
		else 
			dur=boost::posix_time::minutes(m_showMode[m_currentPic].m_staytime);
		
		if(picmax!=0)
			play(m_currentPic);
		/*	sleep  during  play	*/
		boost::this_thread::sleep(dur);

		if(++m_currentPic == picmax)
			m_currentPic=0;
	}
}
