/**********************************************************
*      This software is part of the graphviz toolset      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2005 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                                                         *
*               This version available from               *
*                   http://dynagraph.org                  *
**********************************************************/


#ifndef StringDict_h
#define StringDict_h

#include <string>
#include <algorithm>
#include <iostream>

#ifndef DYNAGRAPH_NO_THREADS
#define STRINGDICT_USE_MUTEX
#include <boost/thread/mutex.hpp>
#endif

// adaptation of agraph's refstr

// I'm sure there's a more modern way to do this....
//#define STRINGDICT_USE_STL
#ifndef STRINGDICT_USE_STL
#include "cdt.h"
#else
#include <map>
#include <string>
#endif

namespace Dynagraph {

struct StringDict {
	StringDict() { init(); }
	void init();
	const char *enter(const char *val);
	void release(const char *val);
	void ref(const char *val); // MUST have come from here!
	static StringDict &GlobalStringDict();
private:
#ifndef STRINGDICT_USE_STL
	Dict_t *dict;
#else
	typedef std::map<std::string,int> mapstrs;
	mapstrs *strs;
#endif // STRINGDICT_USE_STL
#ifdef STRINGDICT_USE_MUTEX
	boost::mutex mutex_;
#endif
};

struct DString { // imitation of std::string
private:
	const char *val;
public:
	typedef const char *iterator;
	typedef const char *const_iterator;
	typedef char value_type;
	typedef size_t size_type;
	static const size_type npos;

	DString() : val(0) {}
	DString(const char *v) : val(StringDict::GlobalStringDict().enter(v)) {}
	DString(const DString &ds) : val(ds.val) {
		StringDict::GlobalStringDict().ref(val);
	}
	DString(const std::string &s) : val(StringDict::GlobalStringDict().enter(s.c_str())) {}
	~DString() {
		StringDict::GlobalStringDict().release(val);
	}
	DString &operator =(const DString &ds) {
		const char *old = val; // do ref first for unlikely s = s
		StringDict::GlobalStringDict().ref(val = ds.val);
		StringDict::GlobalStringDict().release(old);
		return *this;
	}
    operator std::string() const {
        return val?std::string(val):std::string();
	}
	// these are what make this super-cool: single-word compare!
    // (obviously this is not a typological sort, but one rarely wants
    // attributes in alphabetical order, usually just to look them up quickly)
	bool operator <(const DString &ds) const {
		return val<ds.val;
	}
	bool operator ==(const DString &ds) const {
		return val==ds.val;
	}
	bool operator !=(const DString &ds) const {
		return val!=ds.val;
	}
	operator bool() const {
		return val!=0;
	}
	const char *c_str() const {
		return val;
	}
	size_type size() const {
		if(!val)
			return 0;
		return strlen(val);
	}
	size_type length() const {
		return size();
	}
	bool empty() const {
		return length()==0;
	}
	void clear() {
		*this = 0;
	}
	char operator[](size_t pos) const {
		return val[pos];
	}
	const char &operator[](size_t pos) {
		return val[pos];
	}
	iterator begin() {
		return val;
	}
	iterator end() {
		return val+size();
	}
	const_iterator begin() const {
		return val;
	}
	const_iterator end() const {
		return val+size();
	}
	size_type find(char c,size_type pos) const {
		if(pos>=size())
			return npos;
		const_iterator i = std::find(begin()+pos,end(),c);
		if(i==end())
			return npos;
		else
			return i-begin();
	}
	size_type find(char const *str,size_type pos) const {
		const_iterator i = std::search(begin()+pos,end(),str,str+strlen(str));
		if(i==end())
			return npos;
		else
			return i-begin();		
	}
	DString substr(size_type pos,size_type len=npos) const {
		DString ret;
		if(pos>=size())
			return ret;
		ret.assign(begin()+pos,len);
		return ret;
	}
	DString &assign(const char *v,size_type len=npos) {
	  if(!v) {
	    return *this = 0;
	  }
		if(len>=strlen(v))
			return *this = v;
		char *copy = new char[len+1];
		strncpy(copy,v,len);
		copy[len] = 0;
		*this = copy;
		delete [] copy;
		return *this;
	}
	int compare(const DString&s) const {
		return strcmp(c_str(),s.c_str());
	}
	int compare(size_type _Pos1,size_type _Num1,const DString& _Str) {
		return strncmp(c_str()+_Pos1,_Str.c_str(),_Num1);
	}
	int compare(size_type _Pos1,size_type _Num1,const DString& _Str,size_type _Off,size_type _Count) {
		int ret = strncmp(c_str()+_Pos1,_Str.c_str()+_Off,std::min(_Num1,_Count));
		if(ret==0 &&_Num1!=_Count) // comparing strings of different sizes
			return int(_Num1)-int(_Count);
		else
			return ret;
	}
	int compare(const value_type* _Ptr) const {
		return strcmp(c_str(),_Ptr);
	}
	int compare(size_type _Pos1,size_type _Num1,const value_type* _Ptr,size_type _Num2 = npos) const {
		int ret = strncmp(c_str()+_Pos1,_Ptr,std::min(_Num1,_Num2));
		if(ret==0 && _Num1!=_Num2)
			return int(_Num1)-int(_Num2);
		else
			return ret;
	}

};
inline std::ostream &operator <<(std::ostream &os,const DString &s) {
  if(s.length())
    os << s.c_str();
  return os;
}
int ds2int(const DString &s);

} // namespace Dynagraph

#endif // StringDict_h
