#include <set>


template <class T>
typename std::set<T> intersectionSet(std::set<T> s1, std::set<T> s2)
{
	typename std::set<T>::iterator it;
	typename std::set<T> s3;
	for(it = s1.begin(); it != s1.end(); it++)
	{
		if(s2.find(*it) != s2.end())
		{
			s3.insert(*it);
		}
	}

	return s3;
}


template <class T>
typename std::set<T> unionSet(std::set<T> s1, std::set<T> s2)
{
	typename std::set<T> s3;
	s3.insert(s1.begin(), s1.end());
	s3.insert(s2.begin(), s2.end());

	return s3;
}
