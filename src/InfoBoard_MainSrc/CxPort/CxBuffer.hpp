

#pragma once

#include <string>
#ifdef WIN32
		#include <allocators>
#else
	#include <bits/allocator.h>
#endif

#include <deque>
#include <boost\thread.hpp>
#include <boost\thread\condition.hpp>
using std::deque;
using std::string;


template<	typename _Type,		
		template<	typename _Ty , typename _Ax = std::allocator<_Ty> > /* choose the element container*/
				class _StoreType >
class CxCommonBuffer
{

protected:

    size_t						m_maxbufsiz;
    _StoreType< _Type >			m_buffer;		/*elements container*/

public:
	CxCommonBuffer(size_t  _bufsiz):m_maxbufsiz(_bufsiz)
	{
	}
	virtual		~CxCommonBuffer(){}
public:

	virtual		_Type		Pop()=0;
	virtual		void		Push(const _Type & )=0;
	virtual		void		Push(const _Type &&)=0;
	
	virtual		void		Clear()=0;
	virtual		bool		IsEmpty()const=0;
	virtual		bool		IsFull()const=0;

	virtual		string		BufferType()const=0;
};


/*  Liner buffer , the buffe are thread-safe */
template<	typename Type	>
class CxLinerBuffer :  public CxCommonBuffer< Type , deque >
{
public:
	explicit CxLinerBuffer(size_t size=32):
			CxCommonBuffer<Type ,deque>(size),
			m_effictive(true)
			{}
	~CxLinerBuffer()
	{
		Clear();
	}
public:
	/*   Push the elements   */
	void		Push(const Type &_data)
	{
		try
		{	
			Mutex__::scoped_lock lock(m_mu);

			if(!m_effictive)return ;
			//if buffer lose efficitive return 
			while(IsFull())
			{
				m_writecond.wait(m_mu);
			}
			m_buffer.push_front(_data);

			//notify to other thread to read
			m_readcond.notify_one();			
		}

		//thread interrupt 
		catch(boost::thread_interrupted &)
		{
			// rethrow the exception
			throw ;
		}
	}

	//push the elements to use R-Value
	void		Push(const Type &&_data)
	{
		try
		{	
			Mutex__::scoped_lock lock(m_mu);

			//if buffer lose efficitive return 
			if(!m_effictive)return ;
				
			while(IsFull())
			{
				m_writecond.wait(m_mu);
			}

			m_buffer.push_front(_data);

			//notify to other thread to read
			m_readcond.notify_one();		
		}
		//thread interrupt 
		catch(boost::thread_interrupted &)
		{
			// rethrow the exception
			throw ;
		}
	}
	//pop the elements
	Type   Pop()
	{
		try
		{
			Mutex__::scoped_lock lock(m_mu);

			if(!m_effictive)return Type();

			//if empty wait 
			while(IsEmpty())
			{
				m_readcond.wait(m_mu);

				if(!m_effictive)
					return  Type();
			}
			//call  the Type(const Type &)  
			Type _tmp=m_buffer.back();
			m_buffer.pop_back();

			// notify the write thread to write elements
			m_writecond.notify_one();
			return	_tmp;
		}

		//re throw the exception
		catch(const boost::lock_error &)
		{
			throw ;
		}
		catch(const boost::thread_interrupted &)
		{
			throw ;
		}
	}

	//Clear the buffer all elements
	void		Clear()
	{	
		Mutex__::scoped_lock lock(m_mu);	
		m_buffer.clear();
	}

	//make the buffer lose efficitive
	void		loseEffictive()
	{
		m_effictive=false;
		m_readcond.notify_one();
	}

	//make the buffer recover efficitive
	void		recoverEfficitive()
	{
		m_effictive=true;
	}

	bool		IsEmpty()const
	{
		 return this->m_buffer.size()==0;
	}
	
	bool		IsFull()const
	{
		 return this->m_buffer.size()== this->m_maxbufsiz;
	}

	//return the buffer type
	string		BufferType()const
	{
		return string("LinerBuffer");
	}

private:
	typedef boost::condition_variable_any	Cond__;
	typedef boost::mutex					Mutex__;

	//the private member datas
	Mutex__						m_mu;
	Cond__						m_readcond;
	Cond__						m_writecond;
	volatile	 bool			m_effictive;

};



