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


// maintains a two-way map such that A[a] contains b iff B[b] contains a
// it is intended that one side be a condensation of the other
// for example A is a full graph and B has portions collapsed into subgraph-nodes
template<typename Type>
struct two_way_map {
    typedef std::set<Type> tset;
    typedef std::map<Type,tset> tmap;
    tmap A,B;

    void connect(Type a,Type b) {
        A[a].insert(b);
        B[b].insert(a);
    }
    void disconnect(Type a,Type b) {
        tset &Aa = A[a],
            &Bb = B[b];
        check(Aa.erase(b)==1);
        check(Bb.erase(a)==1);
        if(Aa.empty())
            check(A.erase(a)==1);
        if(Bb.empty())
            check(B.erase(b)==1);
    }
    void erase_a(Type a) {
        rename(A,B,a);
    }
    void erase_b(Type b) {
        rename(B,A,b);
    }
    void rename_a(Type a,Type a2) {
        rename(A,B,a,a2);
    }
    void rename_b(Type b,Type b2) {
        rename(B,A,b,b2);
    }
    void rename(tmap &M,tmap &N,Type v,Type v2 = Type()) {
        assert(v2==Type()||M[v2].empty());
        tset &S = M[v];
        for(typename tset::iterator in = S.begin(); in!=S.end(); ++in) {
            tset &T = N[*in];
            check(T.erase(v)==1);
            if(v2!=Type())
                check(T.insert(v2).second);
            if(T.empty())
                check(N.erase(*in)==1);
        }
        if(v2!=Type())
            M[v2] = M[v];
        check(M.erase(v)==1);
    }
};
