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


// adaptation of agraph's refstr

#include <stdlib.h>

#include "StringDict.h"

StringDict g_stringDict;
const DString::size_type DString::npos = DString::size_type(-1);

#ifndef STRINGDICT_USE_STL
typedef struct refstr_t {
	Dtlink_t		link;
	unsigned int	refcnt;
	char			s[1];
} refstr_t;

static Dtdisc_t Refstrdisc = {
	offsetof(refstr_t,s[0]),
	0,
	0,
	((Dtmake_f)0),
	((Dtfree_f)0),
	((Dtcompar_f)0),			/* use strcmp */
	((Dthash_f)0),
	((Dtmemory_f)0),
	((Dtevent_f)0)
};

#ifdef DEBUG
static int refstrprint(refstr_t* r)
{
	fprintf(stderr,"%s\n",r->s); return 0;
}

agrefstrdump(void)
{
	dtwalk(dict,refstrprint);
}
#endif

StringDict::StringDict() {
	init();
}
void StringDict::init() {
	if(!dict)
		dict = dtopen(&Refstrdisc,Dttree);
}
const char *StringDict::enter(const char* s) {
	if(!dict)
		init();
	refstr_t		*key,*r;

	if (s == NULL) return s;

	key = (refstr_t*)(s - offsetof(refstr_t,s[0]));
	r = (refstr_t*) dtsearch(dict,key);
	if (r) r->refcnt++;
	else {
		int size = sizeof(refstr_t)+strlen(s);
		r = (refstr_t*) malloc(size);
		r->refcnt = 1;
		strcpy(r->s,s);
		dtinsert(dict,r);
	}
	return r->s;
}


void StringDict::release(const char* s) {
	refstr_t *r;
	if(!s)
		return;
#ifdef SLOW_RELEASE
	refstr_t *key;

	if ((dict == NULL) || (s == NULL)) return;
	key = (refstr_t*)(s - offsetof(refstr_t,s[0]));
	r = (refstr_t*) dtsearch(dict,key);

#else
	// not sure why agraph version looks it up.
	// one should never call this function with a value
	// that wasn't returned from enter
	r = (refstr_t*)(s - offsetof(refstr_t,s[0]));
#endif
	if (r) {
		r->refcnt--;
		if (r->refcnt <= 0) {
			dtdelete(dict,r);
			free(r);
		}
	}
	else throw DictStringLost(s);
}
void StringDict::ref(const char *s) {
	if(!s)
		return;
	refstr_t *key = (refstr_t*)(s - offsetof(refstr_t,s[0]));
	key->refcnt++;
}
#endif
