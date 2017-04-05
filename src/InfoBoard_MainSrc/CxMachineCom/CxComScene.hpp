#pragma once

#include "CxMachine.hpp"
#include <boost/typeof/typeof.hpp>
#include <boost/signals2.hpp>

BEGIN_NAMESPACE(MSG_SIGN)
	BEGIN_NAMESPACE(MachineComponent)


//////////////////////////////////////////////////////////////////////////
/*	the  unit card			 */
///////////////////////////////////////////////////////////////////////////
class		CxComScene		:		public		CxComponent
{
public:

	explicit	CxComScene(CxMachine *p=NULL):CxComponent(p){}
	virtual		~CxComScene(){}

public:

	CX_CLASS_ID(COM_SCENE)

private:

	size_t				m_cx;
	size_t				m_cy;

};


//////////////////////////////////////////////////////////////////////////
/*	the  unit card			 */
///////////////////////////////////////////////////////////////////////////

class CxBulb
{
public:
	CxBulb():b_isBug(false),b_isLight(true){}
	~CxBulb(){}
	
	bool			is_Bug()const{return b_isBug;}
	bool			is_Light()const{return b_isLight;}
	void			set_Bug(){b_isBug=true;}

private:
	bool			b_isBug;
	bool			b_isLight;
};




//////////////////////////////////////////////////////////////////////////
/*	the  led scene	 */
///////////////////////////////////////////////////////////////////////////
class		CxLedScene		:		public		CxComLeaf
{

protected:

    typedef		vector< vector<CxBulb> >					_Bulb;

public:

	CxLedScene(size_t	width,size_t	height,CxMachine *p=NULL):CxComLeaf(p),
		m_cx(width),m_cy(height),m_bulb(m_cx)
	{
		/* Init the bulbs */
		std::for_each(m_bulb.begin(),m_bulb.end(),[height](_Bulb::reference i){i.resize(height);});
	}

	virtual		~CxLedScene(){}

public:


	/*message define  and class id*/
	CX_CLASS_ID(COM_LEDSCENE)

	CX_BEGIN_MSG
		CX_MSG(MSG_GETSCENESIZE,getSceneSize)
		CX_MSG(MSG_GETNLOSEEFF,getNLoseEff)
		CX_MSG(MSG_SETLEDBUG,setLedBug)
	CX_END_MSG


private:

	_Result		getSceneSize(_Data &);
	_Result		getNLoseEff(_Data &);
	_Result		setLedBug(_Data &data);


protected:
	unsigned short					getnBug()const;
	
	void							setBug(int x,int y)
	{
		if(x > m_cx || x < 0)
			return ;
		if(y>m_cy || y<0)
			return ;

		m_bulb[x][y].set_Bug();
	}
private:

	size_t			m_cx;
	size_t			m_cy;
	_Bulb			m_bulb;
};
                                    
	END_NAMESPACE
END_NAMESPACE
