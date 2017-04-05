
#pragma once

#include <boost/assert.hpp>
#include <boost/system/error_code.hpp>
#include <boost/function.hpp>

#include <iostream>

using std::cout;
using std::endl;

#ifndef		_WIN32_WINNT
	#define		_WIN32_WINNT			0x0501
#endif

#define		CX_ASSERT(expr)			BOOST_ASSERT(expr)

#define		CX_ASSERT_MSG(expr,msg)	BOOST_ASSERT_MSG(expr,msg)

#define		CX_S_ASSERT(expr)		BOOST_STATIC_ASSERT(expr)

#define		CX_S_ASSERT_MSG(expr,msg)		BOOST_STATIC_ASSERT_MSG(expr,msg);

#ifdef _DEBUG

	#define CX_ERR_MESSAGE(msg)		{\
								cout<<"Files:\n\t"<<__FILE__<<endl;\
								cout<<"\nLines:\n\t"<<__LINE__<<endl;\
								cout<<"\nMessage:\n\t"<<(msg)<<endl;}

#else 
	#define CX_ERR_MESSAGE(msg)
#endif



#define		BEGIN_NAMESPACE(space)		\
										namespace space	{
#define		END_NAMESPACE				\
										}


#define		CX_NO_IMP					CX_ASSERT_MSG(0,"Have no implement...")

#define     CX_SLOT                     protected

typedef			unsigned short		WORD;
typedef _W64	unsigned long ULONG_PTR, *PULONG_PTR;
typedef			unsigned long		DWORD;
typedef			ULONG_PTR			DWORD_PTR;

#ifndef MAKEWORD
	#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#endif
#ifndef MAKELONG
	#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#endif
#ifndef LOWORD
	#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#endif	
#ifndef HIWORD
	#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#endif	
#ifndef LOBYTE
	#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#endif
#ifndef HIBYTE
	#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
#endif


enum
{
	THREAD_SUSPEND	,				//Ïß³ÌÔÝÍ£
	THREAD_CLOSE	,				//Ïß³Ì¹Ø±Õ
	THREAD_RUNNING					//Ïß³ÌÕýÔÚÔËÐÐ
};

enum 
{
	PORT_CLOSED		,				//¶Ë¿Ú¹Ø±Õ×´Ì¬	
	PORT_OPENED						//¶Ë¿Ú´ò¿ª×´Ì¬
};
		
//»ù±¾Êý¾Ý¶¨Òå


typedef		unsigned int	UINT	;
typedef		unsigned long	ULONG	;
typedef		unsigned char	UCHAR	;
typedef		UCHAR			BYTE	;


typedef		boost::system::error_code	ERRCODE ;



inline	void   	__EmptyFunc(bool){}


typedef		boost::function<void(const ERRCODE&,size_t )>	 Async_Handler;

