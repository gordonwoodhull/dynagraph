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


// cdt does not allow you to allocate your own Dt_t, which is annoying
// because you always want to put something with it for all the callbacks!
// so, fool it: on open, allow it to allocate, then copy & free
// on close, make a copy which it can free
struct derivable_dt : Dt_t {
	void open(Dtdisc_t* disc, Dtmethod_t* meth) {
		Dt_t *dt = dtopen(disc,meth);
		memcpy(static_cast<Dt_t*>(this),dt,sizeof(Dt_t));
		free(dt);
	}
	void close() {
		Dt_t *cp = (Dt_t*)malloc(sizeof(Dt_t));
		memcpy(cp,this,sizeof(Dt_t));
		dtclose(cp);
	}
};
