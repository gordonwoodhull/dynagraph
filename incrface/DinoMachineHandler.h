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


// act like a string graph but adjust the real thing too
struct DinoMachineHandler : AbsGraphHandler<DinoMachine> {
    DinoMachineHandler(DinoMachine *dino) : AbsGraphHandler<DinoMachine>(dino) {} // required!

    DString incr_ev_ins_node(DString name, const StrAttrs &attrs, bool merge);
    DString incr_ev_ins_edge(DString name, DString tailname, DString headname, const StrAttrs &attrs);
    bool incr_ev_mod_node(DString name,const StrAttrs &attrs);
    bool incr_ev_mod_edge(DString name,const StrAttrs &attrs);
	bool incr_ev_del_node(DString name);
	bool incr_ev_del_edge(DString name);

    void modify_node(DString name,const StrAttrs &attrs);
    void modify_edge(DString name,const StrAttrs &attrs);
};
