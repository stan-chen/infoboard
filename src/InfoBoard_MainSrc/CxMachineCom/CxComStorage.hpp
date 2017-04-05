
#pragma once


#include "CxMachine.hpp"
#include <boost/thread.hpp>
#include <boost/signals2.hpp>
#include <fstream>

BEGIN_NAMESPACE(MSG_SIGN)
	BEGIN_NAMESPACE(MachineComponent)

using std::fstream;
using std::string;

/*	the image x and the y is reversal*/
class	CxImage
{
public:
	typedef vector< vector<unsigned char>  >	_ImageData;
public:

	CxImage(int cx=512,int  cy=128):m_cx(cx),m_cy(cy),m_data(cy)
	{
		std::for_each(m_data.begin(),m_data.end(),
			[cx](_ImageData::reference p){p.resize(cx);});
	}

	bool		insert(size_t  line,const vector<unsigned char> &data)
	{
		if(line > m_data.size())return false;

		vector<unsigned char >  &src=m_data.at(line);
		if(data.size() != src.size())return false;

		if(m_cy<=64)
		{
			src.assign(data.begin(),data.end());
		}
		else 
		{
			for(int i=0;i!=src.size();++i)
			{
				src[i]=data[i] & 0x0F;
				if(line < m_cy-64)
					m_data.at(line+64)[i] = (data[i]>>4) & 0x0F;
			}
		}
		return true;
	}

	bool		getLine(size_t line,vector<unsigned char> &data)const
	{
		if(line > m_data.size())return false;
		data=m_data.at(line);
		return true;
	}

	void		outputFile(const string &filename)
	{
		fstream fout;
		fout.open(filename,fstream::ate | fstream::binary | fstream::out);
		for(auto i=m_data.begin();i!=m_data.end();++i)
		{
			for(auto j=i->begin();j!=i->end();++j)
				fout<<*j;	
		}
	}
public:

	int			m_cx;
	int			m_cy;
	_ImageData  m_data;
};


//////////////////////////////////////////////////////////////////////////
/*	storage component 	 */
///////////////////////////////////////////////////////////////////////////
class CxComStorage    :   public	CxComponent
{
public:
	explicit	CxComStorage(CxMachine *p=NULL):CxComponent(p){}
	virtual		~CxComStorage(){}
public:
	//message and class id
	CX_CLASS_ID(COM_STORAGE)	
};


//////////////////////////////////////////////////////////////////////////
/*	Image	storage		 */
///////////////////////////////////////////////////////////////////////////
class	CxImgStorage		:	public	CxComLeaf
{
protected:

	typedef			CxImage								_Img;
	typedef			boost::mutex						_Mutex;
	typedef			map<size_t,_Img*>					_ImgMap;
	typedef			boost::signals2::signal<void()>		_Signal;

public:

	explicit	CxImgStorage(CxMachine *p=NULL):CxComLeaf(p)
	{
	}
	virtual		~CxImgStorage()
	{
		clear();
	}

public:

	CX_CLASS_ID(COM_IMGSTORAGE)	

	CX_BEGIN_MSG
		CX_MSG(MSG_IMGINSERT,insertImage)
		CX_MSG(MSG_MARKIMAGE,restPlay)
	CX_END_MSG

private:

	_Result				restPlay(_Data &)
	{
		sig_reset();
		return CX_OK;
	}

public:


	_Result		getImage(int  npic,_Img::_ImageData &data)
	{
		_Mutex::scoped_lock lock(m_mu);

		_ImgMap::iterator  pd=m_imgmap.find(npic);

		if(pd==m_imgmap.end())
			return  CX_FAIL;
		else 
		{
			data.assign(pd->second->m_data.begin(),pd->second->m_data.end());
			return CX_OK;
		}
	}

	size_t		getnPic()
	{
		_Mutex::scoped_lock lock(m_mu);
		return m_imgmap.size();
	}

/*signals */
public:

	_Signal						sig_pause;
	_Signal						sig_reset;

private:

	/*debug function*/
	void		saveFile()
	{
		for(auto i=m_imgmap.begin();i!=m_imgmap.end();++i)
		{
			char filename[32];
			sprintf(filename,"Pic%d.dat",i->first);
			i->second->outputFile(string(filename));
		}
	}

	_Result		insertImage(_Data &v)
	{
		_Data data;
		data.assign(v.begin(),v.end()-2);

		int  npic=data[1];
		int  line=data[2]; 
		
		if(npic==0	&&  line==0)
		{
			/* send  pause signals */
			sig_pause();
			clear();
		}

		data.erase(data.begin(),data.begin()+3);

		_ImgMap::iterator  pd=m_imgmap.find(npic);

		if(pd!=m_imgmap.end())
		{
			_Mutex::scoped_lock lock(m_mu);
			if(!pd->second->insert(line,data))
			{
				//cout<<"The image size is not match"<<endl;
				return CX_FAIL;
			}
		}
		else 
		{
			_Data size;
			int  width,height;

			if( CX_CHECK(getParent()->handleMsg(_Msg(COM_LEDSCENE,MSG_GETSCENESIZE),size)) )
			{
				width=MAKEWORD(size[1],size[0]);
				height=MAKEWORD(size[3],size[2]);
			}
			_Img *tmp=new _Img(width,height);

			_Mutex::scoped_lock lock(m_mu);
			tmp->insert(line,data);
			m_imgmap.insert(std::make_pair(npic,tmp));
		}
		return CX_OK;
	}

	void			clear()
	{
		_Mutex::scoped_lock lock(m_mu);
		std::for_each(m_imgmap.begin(),m_imgmap.end(),[](_ImgMap::reference i){delete i.second;});
		m_imgmap.clear();
	}

private:
	_Mutex						m_mu;
	_ImgMap						m_imgmap;
};

	END_NAMESPACE
END_NAMESPACE