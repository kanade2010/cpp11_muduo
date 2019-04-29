#ifndef _PTR_VECTOR_HH
#define _PTR_VECTOR_HH

#include <vector>
#include <memory>
#include <assert.h>

namespace myself{

#define ASSERT(If, Msg) \
	if(!(If)) \
	{\
		fprintf(stderr, "Error/(%s, %d): %s, abort.\n", __FILE__, __LINE__, Msg); abort();\
	}

template<typename T>
class ptr_vector{
public:
	typedef unsigned int size_type;
	typedef std::vector<T*> vector;
	typedef T* value_type;
	typedef value_type& reference;

	explicit ptr_vector(){
	}
	
	~ptr_vector(){
		clear();
	}

	void clear(){
		if(!m_vector.empty())
		{
			//typename vector::iterator it;
			for(auto it = m_vector.begin(); it != m_vector.end(); ++it)
			{
				delete *it;//释放指针指向的内存.
			}
		}

		m_vector.clear();//释放指针本身.
	}

	void push_back(T* const &v){
		ASSERT(v , "NULL point at ptr_vector push_back()");
		std::unique_ptr<T> tmp(v);
		m_vector.push_back(v); //使用 unique_ptr 保证push_back失败时，v也能正常释放.
		tmp.release();
	}

	std::unique_ptr<T> pop_back(){
		ASSERT( !m_vector.empty(), "'pop_back()' on empty container");
		std::unique_ptr<T> tmp(m_vector.back());
		m_vector.pop_back();
		return std::move(tmp);
	}

	reference operator[](size_type n){
		ASSERT(n < size(), "operator[] n out of the border")
		return m_vector[n];
	}

	bool empty(){
		return m_vector.empty();
	}

	size_type size(){
		return m_vector.size();
	}

	void reserve(size_type n){
		m_vector.reserve(n);
	}

	void resize(size_type s){
		size_type size = this->size();
		if(s < size)
		{
			for(auto it = m_vector.begin() + s; it != m_vector.end(); ++it)
			{
				delete *it;//释放指针指向的内存.
			}
			m_vector.resize(s);
		}
		else if(s > size)
		{
			for(; size != s; ++size)
			{
				push_back(new T);
			}
		}
		ASSERT(s == this->size(), "'resize' error size asymmetry");
	}

	void swap(ptr_vector<T>& v){
		m_vector.swap(v.base());
	}

private:
	ptr_vector<T>& operator=(const ptr_vector<T>&);
	ptr_vector<T>(ptr_vector<T>&);

	vector& base(){
		return m_vector;
	}

	vector m_vector;
};

}

#endif