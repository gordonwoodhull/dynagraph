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


template<class G>
void findAllPaths(G &top,G &a,G &b,bool followOuts,bool followIns,bool firstOnly,int limit,G &ret) {
    ret = b;

    BFS<G> bfs(&top,false,followIns,followOuts);
    for(typename G::node_iter ni = a.nodes().begin(); ni!=a.nodes().end(); ++ni)
        bfs.add(top.find(*ni));

    std::vector<typename G::Edge*> prev;
    prev.resize(top.nodes().size(),0);
    for(bfs.start();!bfs.stopped(); ++bfs) {
        EN<G> v = *bfs;
        if(v.e) {
			typename G::Node *n = 0;
            if(v.n) {
                if(ret.find(v.n)) {
                    ret.insert(v.e);
					n = v.e->other(v.n);
                }
                else
                    prev[gd<typename G::Seq>(v.n).id] = v.e;
            }
			else { // node's not new
                if(ret.find(v.e->tail) && followIns)
					n = v.e->head;
				if(ret.find(v.e->head) && followOuts)
					n = v.e->tail;
				if(n)
                    ret.insert(v.e);
			}
			if(n) {
                while(typename G::Edge *e = prev[gd<typename G::Seq>(n).id]) {
                    n = e->other(n);
                    ret.insert(e);
                }
				if(firstOnly)
					break;
				if(limit>0 && ret.nodes().size()>size_t(limit))
					break;
			}
        }
    }
    ret |= a;
}