#ifndef SPREAD_H_
#define SPREAD_H_

#include <bits/stdc++.h>
#include <memory>
#include <vector>

namespace stupid
{

template <typename T, 
		  template <typename E, typename = std::allocator<E>> class container = std::vector>
class spreadable
{
public:
	spreadable(const T* father, const size_t depth = 0)
		: _father(father)
		, _depth(depth)
	{
	}

	virtual ~spreadable() {  }

	// set this as father
	virtual container<std::unique_ptr<T>> spread() const = 0;
	
	virtual size_t get_depth() const
	{
		return _depth;
	}

	virtual const T* get_father() const
	{
		return _father;
	}

private:
	const T* _father;
	const size_t _depth;
};

}

#endif 	// SPREAD_H_
