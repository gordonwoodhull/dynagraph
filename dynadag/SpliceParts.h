/**********************************************************
*            This software is part of Dynagraph           *
*                http://www.dynagraph.org/                *
*                                                         *
*            Copyright (c) 2006 Gordon Woodhull           *
*        Portions Copyright (c) 1994-2005 AT&T Corp.      *
*  Licensed under the Common Public License, Version 1.0  *
*                                                         *
*      Dynagraph originates in the Graphviz toolset       *
*                   http://graphviz.org                   *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                       Many thanks.                      *
**********************************************************/


#ifndef SpliceParts_h
#define SpliceParts_h

namespace Dynagraph {
namespace DynaDAG {

template<typename Layout>
struct NodeTyping {
	typedef typename Layout::Node *ElemType;
	static ElemType Get(Layout *layout,Name name) {
		return layout->fetch_node(name);
	}
};
template<typename Layout>
struct EdgeTyping {
	typedef typename Layout::Edge *ElemType;
	static ElemType Get(Layout *layout,Name name) {
		return layout->fetch_edge(name);
	}
};
template<typename Layout,typename Typing>
struct SplicePartsIterator {
	Layout *layout_;
	Name base_;
	char prefix_;
	int i_;
	SplicePartsIterator(Layout *layout,Name base,char prefix) : layout_(layout),base_(base),prefix_(prefix),i_(1) {}
	SplicePartsIterator(SplicePartsIterator &it) : layout_(it.layout_),base_(it.base_),prefix_(it.prefix_),i_(it.i_) {}
	SplicePartsIterator &operator =(SplicePartsIterator &it) {
		layout_ = it.layout_;
		base_ = it.base_;
		prefix_ = it.prefix_;
		i_ = it.i_;
		return *this;
	}
	typename Typing::ElemType operator *() const {
		if(!layout_)
			return 0;
		char partname[40];
		sprintf(partname,"%s_%cpart%d",base_.c_str(),prefix_,i_);
		return Typing::Get(layout_,partname);
	}
	SplicePartsIterator &operator ++() {
		++i_;
		return *this;
	}
	SplicePartsIterator operator ++(int) {
		SplicePartsIterator ret = *this;
		++*this;
		return ret;
	}
};
template<typename Layout>
struct SpliceNodePartsIterator : SplicePartsIterator<Layout,NodeTyping<Layout> > {
	SpliceNodePartsIterator(Layout *layout,Name base,char prefix) : SplicePartsIterator<Layout,NodeTyping<Layout> >(layout,base,prefix) {}
	SpliceNodePartsIterator(SpliceNodePartsIterator &it) : SplicePartsIterator(it) {}
};
template<typename Layout>
struct SpliceEdgePartsIterator : SplicePartsIterator<Layout,EdgeTyping<Layout> > {
	SpliceEdgePartsIterator(Layout *layout,Name base,char prefix) : SplicePartsIterator<Layout,EdgeTyping<Layout> >(layout,base,prefix) {}
	SpliceEdgePartsIterator(SpliceEdgePartsIterator &it) : SplicePartsIterator(it) {}
};

} // namespace DynaDAG
} // namespace Dynagraph

#endif // SpliceParts_h
