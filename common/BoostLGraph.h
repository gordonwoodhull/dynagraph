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


namespace boost {

	struct LGraph_traversal_category :
		public virtual bidirectional_graph_tag,
		public virtual adjacency_graph_tag,
		public virtual vertex_list_graph_tag { };

	template <class ADT,class gdata,class vdata,class edata>
	struct graph_traits< LGraph<ADT,gdata,vdata,edata> > {
		typedef LGraph<ADT,gdata,vdata,edata> ELGraph;
		typedef typename ELGraph::Node *vertex_descriptor;
		typedef typename ELGraph::Edge *edge_descriptor;

		typedef typename ELGraph::node_iter vertex_iterator;
		typedef typename ELGraph::outedge_iter out_edge_iterator;
		typedef typename ELGraph::inedge_iter in_edge_iterator;

		typedef directed_tag directed_category;
		typedef disallow_parallel_edge_tag edge_parallel_category;
		typedef LGraph_traversal_category traversal_category;
		typedef size_t vertices_size_type;
		typedef size_t edges_size_type;
		typedef size_t degree_size_type;
	};
	template <class gdata,class vdata, class edata>
	struct graph_traits< NamedGraph<gdata,vdata,edata> > {
		typedef NamedGraph<gdata,vdata,edata> ELGraph;
		typedef typename ELGraph::Node *vertex_descriptor;
		typedef typename ELGraph::Edge *edge_descriptor;

		typedef typename ELGraph::node_iter vertex_iterator;
		typedef typename ELGraph::outedge_iter out_edge_iterator;
		typedef typename ELGraph::inedge_iter in_edge_iterator;

		typedef directed_tag directed_category;
		typedef disallow_parallel_edge_tag edge_parallel_category;
		typedef LGraph_traversal_category traversal_category;
		typedef size_t vertices_size_type;
		typedef size_t edges_size_type;
		typedef size_t degree_size_type;
	};

	// Vertex List Graph concept
	template <class ADT,class gdata,class vdata, class edata>
	inline std::pair< typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_iterator,
		typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_iterator >
	vertices(const LGraph<ADT,gdata,vdata,edata> &g)
	{
		return std::make_pair(g.nodes().begin(),g.nodes().end());
	}
	template <class ADT,class gdata,class vdata, class edata>
	inline typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertices_size_type
	num_vertices(const LGraph<ADT,gdata,vdata,edata> &g)
	{
		return g.nodes().size();
	}

	// Incidence Graph concept
	template <class ADT,class gdata,class vdata, class edata>
	inline typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_descriptor
	source(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::edge_descriptor e,
		const LGraph<ADT,gdata,vdata,edata> &g)
	{
		return e->tail;
	}
	template <class ADT,class gdata,class vdata, class edata>
	inline typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_descriptor
	target(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::edge_descriptor e,
		const LGraph<ADT,gdata,vdata,edata> &g)
	{
		return e->head;
	}
	template <class ADT,class gdata,class vdata, class edata>
	inline std::pair< typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::out_edge_iterator,
		typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::out_edge_iterator >
	out_edges(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_descriptor u,
		const LGraph<ADT,gdata,vdata,edata> &g)
	{
		return std::make_pair(u->outs().begin(),u->outs().end());
	}
	template <class ADT,class gdata,class vdata, class edata>
	inline typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::degree_size_type
	out_degree(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_descriptor u,
		const LGraph<ADT,gdata,vdata,edata> &g)
	{
		return u->outs().size();
	}

	// Bidirectional Graph concept
	template <class ADT,class gdata,class vdata, class edata>
	inline std::pair< typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::in_edge_iterator,
		typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::in_edge_iterator >
	in_edges(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_descriptor u,
		const LGraph<ADT,gdata,vdata,edata> &g)
	{
		return std::make_pair(u->ins().begin(),u->ins().end());
	}
	template <class ADT,class gdata,class vdata, class edata>
	inline typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::degree_size_type
	in_degree(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_descriptor u,
		const LGraph<ADT,gdata,vdata,edata> &g)
	{
		return u->ins().size();
	}
	template <class ADT,class gdata,class vdata, class edata>
	inline typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::degree_size_type
	degree(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_descriptor u,
		const LGraph<ADT,gdata,vdata,edata> &g)
	{
		return u->degree();
	}

	// Mutable Graph concept
	template <class ADT,class gdata,class vdata, class edata>
	inline std::pair< typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::edge_descriptor, bool >
	add_edge(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_descriptor u,
		typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_descriptor v,
		LGraph<ADT,gdata,vdata,edata> &g)
	{
		return g.create_edge(u,v);
	}
	template <class ADT,class gdata,class vdata, class edata>
	inline void
	remove_edge(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_descriptor u,
		typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_descriptor v,
		LGraph<ADT,gdata,vdata,edata> &g)
	{
		typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::edge_descriptor e = g.find_edge(u,v);
		if(e)
			g.erase(e);
	}
	template <class ADT,class gdata,class vdata, class edata>
	inline void
	remove_edge(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::edge_descriptor e,
		LGraph<ADT,gdata,vdata,edata> &g)
	{
		g.erase(e);
	}
	template <class ADT,class gdata,class vdata, class edata>
	inline void
	remove_edge(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::edge_iterator ei,
		LGraph<ADT,gdata,vdata,edata> &g)
	{
		g.erase(*ei);
	}
	// skipping *_if
	template <class ADT,class gdata,class vdata, class edata>
	inline typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_descriptor
	add_vertex(LGraph<ADT,gdata,vdata,edata> &g)
	{
		return g.create_node();
	}
	template <class ADT,class gdata,class vdata, class edata>
	inline void
	clear_vertex(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_descriptor u,
		LGraph<ADT,gdata,vdata,edata> &g)
	{
		for(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::out_edge_iterator ei = u->outs().begin(); ei!=u->outs().end(); ++ei)
			g.erase(*ei);
		for(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::in_edge_iterator ei = u->ins().begin(); ei!=u->ins().end(); ++ei)
			g.erase(*ei);
	}
	template <class ADT,class gdata,class vdata, class edata>
	inline void
	remove_vertex(typename graph_traits< LGraph<ADT,gdata,vdata,edata> >::vertex_descriptor u,
		LGraph<ADT,gdata,vdata,edata> &g)
	{
		g.erase(u);
	}

	// MemberAccessor models a Read/Write Property Map by
    // accessing a member of a graph/node/edge datum
	template <class K,class Datum,class MT,MT Datum::*Member>
	struct MemberAccessor {
		typedef Datum datum;
		typedef K key_type;
		typedef MT value_type;
		typedef MT &reference;
		typedef read_write_property_map_tag category;
	};
	template <class K,class Datum,class MT,MT Datum::*Member>
	inline void put(MemberAccessor<K,Datum,MT,Member>&,K u,MT dat) {
		gd<Datum>(u).*Member = dat;
	}
	template <class K,class Datum,class MT,MT Datum::*Member>
	inline MT &
	get(const MemberAccessor<K,Datum,MT,Member>&,K const &u) {
		return gd<Datum>(u).*Member;
	}
	// PointerAccessor models a Read/Write Property Map by
    // accessing an array (pointer) by the ID value of the node/edge
	template <class G,class K,class V>
	struct PointerAccessor {
		typedef K key_type;
		typedef V value_type;
		typedef V &reference;
		typedef read_write_property_map_tag category;

		V *ray;
		PointerAccessor(V *ray) : ray(ray) {}
	};
	template <class G,class K,class V>
	inline void put(PointerAccessor<G,K,V> &acc,K u,V dat) {
		acc.ray[gd<typename G::Seq>(u).id] = dat;
	}
	template <class G,class K,class V>
	inline V &
	get(const PointerAccessor<G,K,V> &acc,K const &u) {
		return acc.ray[gd<typename G::Seq>(u).id];
	}
} // namespace boost

