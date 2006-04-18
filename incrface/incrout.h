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

namespace Dynagraph {

// (too tricky by half. but i want no unnecessary locking!)
struct Locker {
	int something_;
	std::ostream &os_;
	std::stringstream strs;
	const char *view_;
	Locker(std::ostream &os,const char *view) : something_(0),os_(os),view_(view) {}
	template<typename T>
	std::ostream &operator <<(const T &x) {
		switch(something_++) {
			case 0:
				return strs << x;
			case 1:
				os_ << "lock graph " << view_ << std::endl;
				os_ << strs.str();
			default:
				return os_ << x;
		}
	}
	~Locker() {
		if(something_>1)
			os_ << "unlock graph " << view_ << std::endl;
		else
			os_ << strs.str();
	}
};
template<typename NGraph>
void emitChanges(std::ostream &os,ChangeQueue<NGraph> &Q,const char *view) {
	Locker locker(os,view);
	if(!igd<StrAttrChanges>(Q.ModGraph()).empty())
		locker << "modify graph " << view << " " << changes(Q.ModGraph()) << std::endl;
	for(typename NGraph::graphedge_iter ei = Q.delE.edges().begin(); ei!=Q.delE.edges().end(); ++ei)
		locker << "delete edge " << view << " " << mquote(gd<Name>(*ei).c_str()) << std::endl;
	for(typename NGraph::node_iter ni = Q.delN.nodes().begin(); ni!=Q.delN.nodes().end(); ++ni) 
		locker << "delete node " << view << " " << mquote(gd<Name>(*ni).c_str()) << std::endl;
	for(typename NGraph::node_iter ni = Q.insN.nodes().begin(); ni!=Q.insN.nodes().end(); ++ni) 
		locker << "insert node " << view << " " << mquote(gd<Name>(*ni).c_str())
			<< " " << gd<StrAttrs>(*ni) << std::endl;
	for(typename NGraph::graphedge_iter ei = Q.insE.edges().begin(); ei!=Q.insE.edges().end(); ++ei) 
		locker << "insert edge " << view << " " << mquote(gd<Name>(*ei))
			<< " " << mquote(gd<Name>((*ei)->tail))
			<< " " << mquote(gd<Name>((*ei)->head)) << " " << gd<StrAttrs>(*ei) << std::endl;
	for(typename NGraph::node_iter ni = Q.modN.nodes().begin(); ni!=Q.modN.nodes().end(); ++ni) 
		if(!igd<StrAttrChanges>(*ni).empty())
			locker << "modify node " << view << " " << mquote(gd<Name>(*ni).c_str())
				<< " " << changes(*ni) << std::endl;
	for(typename NGraph::graphedge_iter ei = Q.modE.edges().begin(); ei!=Q.modE.edges().end(); ++ei) 
		if(!igd<StrAttrChanges>(*ei).empty())
			locker << "modify edge " << view << " " << mquote(gd<Name>(*ei))
				<< " " << changes(*ei) << std::endl;
}

} // namespace Dynagraph
