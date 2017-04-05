

#pragma once


#include "../CxType.h"


BEGIN_NAMESPACE(MSG_SIGN)

	BEGIN_NAMESPACE(MachineComponent)

typedef		size_t					_ComID;
typedef		unsigned long			_MsgID;
typedef     unsigned long			_Result;
typedef		size_t					_ComID;



/*	return  result	*/

#define				CX_OK					0x00000000
#define				CX_FAIL					0x00000001
#define				CX_NOTFOUND				0x00000002
#define				CX_EXISTING				0x00000003
#define				CX_ISLEAF				0x00000004
#define				CX_NOTMACH				0x00000005

#define				CX_CHECK(result)	(((result)==(CX_OK))?true:false)

/*	component ID	*/
const  _ComID		COM_COMPONENT	=		0x00000001;
const  _ComID		COM_UNITCARD	=		0x00000002;
const  _ComID		COM_LEAF		=		0x00000003;	
const  _ComID		COM_TIME		=		0x00000004;
const  _ComID		COM_STORAGE		=		0x00000005;
const  _ComID		COM_IMGSTORAGE	=		0x00000006;
const  _ComID		COM_SCENE		=		0x00000007;
const  _ComID		COM_LEDSCENE	=		0x00000008;
const  _ComID		COM_DETECT		=		0x00000009;
const  _ComID		COM_POWERDETECT	=		0x0000000A;
const  _ComID		COM_LOSEPOINTDETECT	=	0x0000000B;
const  _ComID		COM_TEMPERDETECT	=	0x0000000C;
const  _ComID		COM_STATEDETECT		=	0x0000000D;
const  _ComID		COM_PLAYOPTION		=	0x0000000E;
const  _ComID		COM_BUILDER			=	0x0000000F;
const  _ComID		COM_DETECTLEAF		=	0x00000010;
const  _ComID		COM_EXPORT			=	0x00000011;
const  _ComID		COM_IMPORT			=	0x00000012;


/*	componente message ID */

/*	 set & detect  msessage */
#define		MSG_GETTIME				0x00000000
#define		MSG_SETTIME				0x00000001
#define		MSG_IMGINSERT			0x00000002
#define		MSG_GETSCENESIZE		0x00000003
#define		MSG_GETSTATE			0x00000004
#define		MSG_SETPROLIST			0x00000005
#define		MSG_BEGINSHOW			0x00000006
#define		MSG_MARKIMAGE			0x00000007
#define		MSG_GETCURRENTPLAY		0x00000008
#define		MSG_GETLEDLIGHT			0x00000009
#define		MSG_SETLEDLIGHT			0x0000000A
#define		MSG_GETNLOSEEFF			0x0000000B
#define		MSG_GETPOWERSTATE		0x0000000C
#define		MSG_SETPOWERSTATE		0x0000000D
#define		MSG_SETTEMPER			0x0000000F
#define		MSG_SETLEDBUG			0x00000010
#define		MSG_AUTOTIME			0x00000011
#define		MSG_GETPOWERDETECT		0x00000012

/*	output  message */

#define		MSG_PUSHINFO			0x00000011


#define		OUTMESSAGE			{cout<<"The Component is "<<getID()<<endl; \
									cout<<"The Message is "<<msg.msg<<endl; \
}	


/////////////////////////////////////////////////////////////////////////////


#define		CX_CLASS_ID(id)			virtual	 _ComID	 getID()const{return id;}


#define		CX_BEGIN_MSG			virtual	 _Result	handleMsg(const _Msg &msg,_Data &data){	\
										if(msg.id!=getID())			\
										{							\
												if(getParent()!=NULL)		\
													return getParent()->handleMsg(msg,data);	\
												else return  CX_FAIL;				\
										}								\
										switch(msg.msg){	

#define		CX_END_MSG				default:			\
										return CX_NOTMACH;	\
										}			\
										}

#define		CX_MSG(msg,func)		case msg: return func(data);

/////////////////////////////////////////////////////////////////////////////


/* message  tag */
typedef		struct	tagMsg
{
	_ComID			id;
	_MsgID			msg;
	tagMsg(_ComID i,_MsgID mid):id(i),msg(mid){}
}_Msg;



/* tag temper option*/
typedef struct	tagTempOption
{
	typedef     unsigned short				_Word;
	typedef		unsigned char				_Byte;

	_Byte		m_tmppowerMax;
	_Byte		m_tmppowerMin;
	_Byte		m_tmpfanMax;
	_Byte		m_tmpfanMin;

	bool		b_isautoOff;
	bool		b_isautoFan;
	bool		b_isautoPower;
	bool		b_issmokeautoOff;

	tagTempOption():m_tmppowerMax(60),m_tmppowerMin(25),m_tmpfanMax(60),m_tmpfanMin(25),
		b_isautoOff(false),b_isautoFan(false),b_isautoPower(false),b_issmokeautoOff(false)
	{
	}
}tmpOption;

/* tag show mode */
typedef struct	tagshowMode
{
	typedef     unsigned short				_Word;
	bool			b_ispicSeq;
	bool			b_staytimeUnit;
	_Word			m_showSpeed;
	_Word			m_showmodeIn;
	_Word			m_showmodeOut;
	_Word			m_timeshowMode;
	_Word			m_timeshowPos;
	size_t			m_staytime;

	tagshowMode():b_ispicSeq(false),b_staytimeUnit(false),
		m_staytime(8)
	{}

}showMode;




	END_NAMESPACE

END_NAMESPACE