
#pragma once
#include <list>
#include <boost\thread.hpp>
class CxTcpClient;

class CxClientList
{
	typedef boost::mutex						_Mutex;
	typedef std::list<CxTcpClient* >			_List;
	typedef boost::function<void(CxTcpClient*)>	_EachFunc;

public:
	CxClientList(){}
	~CxClientList(){}

public:
	void		push_back(CxTcpClient * && val)
	{
		_Mutex::scoped_lock lock(m_mu);
		m_list.push_back(val);
	}

	void		push_back(CxTcpClient  *const  &val)
	{
		_Mutex::scoped_lock lock(m_mu);
		m_list.push_back(val);
	}

	void		push_front(CxTcpClient *const val)
	{		
		_Mutex::scoped_lock lock(m_mu);
		m_list.push_front(val);
	}

	void		push_front(CxTcpClient * && val)
	{
		_Mutex::scoped_lock lock(m_mu);
		m_list.push_front(val);
	}

	size_t      size()
	{
		_Mutex::scoped_lock lock(m_mu);
		return  m_list.size();
	}
	CxTcpClient *at(size_t index)
	{
		_Mutex::scoped_lock lock(m_mu);	
		if(index > m_list.size()-1)
			return NULL;
		_List::iterator  it=m_list.begin();
		for(int i=0;i<index;++i)
			++it;
		return *it;
	}

	void		remove(size_t index)
	{
		_Mutex::scoped_lock lock(m_mu);	
		if(index > m_list.size()-1)
			return ;

		_List::iterator  it=m_list.begin();
		for(int i=0;i<index;++i)
			++it;

		m_list.remove(*it);	
	}

	void	for_each(_EachFunc func)
	{
		_Mutex::scoped_lock lock(m_mu);

		_List::iterator  it=m_list.begin();
		_List::iterator  end=m_list.end();

		for(;it!=end;++it)
			func(*it);
	}

private:
	_Mutex		m_mu;
	_List		m_list;


};


