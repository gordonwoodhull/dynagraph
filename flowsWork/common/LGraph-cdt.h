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


#ifndef LGRAPH_CDT_H
#define LGRAPH_CDT_H

#include "LGraph.h"
#include "dt.h"

struct ADTisCDT {
	template<typename Edge,typename Node,typename EdgeCompare,typename HeadCompare,typename NodeCompare>
	struct defs {
		struct inseqlink : cdt::seqlink {};
		struct outseqlink : cdt::seqlink {};
		struct intreelink : cdt::treelink {};
		struct outtreelink : cdt::treelink {};
		struct headtreelink : cdt::treelink {};

		typedef cdt::derived_accessor<Edge,inseqlink> inedge_sequence_accessor;
		typedef cdt::derived_accessor<Edge,outseqlink> outedge_sequence_accessor;
		typedef cdt::sequence<inedge_sequence_accessor> inedge_sequence;
		typedef cdt::sequence<outedge_sequence_accessor> outedge_sequence;

		typedef cdt::derived_accessor<Edge,intreelink> inedge_tree_accessor;
		typedef cdt::derived_accessor<Edge,outtreelink> outedge_tree_accessor;
		typedef cdt::disc<inedge_tree_accessor,EdgeCompare> inedge_tree_disc;
		typedef cdt::disc<outedge_tree_accessor,EdgeCompare> outedge_tree_disc;
		typedef cdt::tree_dict<inedge_tree_disc> inedge_tree_dict;
		typedef cdt::tree_dict<outedge_tree_disc> outedge_tree_dict;
		typedef cdt::tree<inedge_tree_disc> inedge_tree;
		typedef cdt::tree<outedge_tree_disc> outedge_tree;

		typedef cdt::derived_accessor<Edge,headtreelink> head_tree_accessor;
		typedef cdt::disc<head_tree_accessor,HeadCompare> head_tree_disc;
		typedef cdt::tree_dict<head_tree_disc> head_tree_dict;

		// node stuff
		typedef cdt::derived_accessor<Node,cdt::seqlink> node_sequence_accessor;
		typedef cdt::derived_accessor<Node,cdt::treelink> node_tree_accessor;
		typedef cdt::sequence<node_sequence_accessor> node_sequence;
		typedef cdt::disc<node_tree_accessor,NodeCompare> node_tree_disc;
		typedef cdt::tree_dict<node_tree_disc> node_tree_dict;
		typedef cdt::tree<node_tree_disc> node_tree;

		typedef cdt::ordering<inedge_tree,inedge_sequence> inedge_order;
		typedef cdt::ordering<outedge_tree,outedge_sequence> outedge_order;
		typedef cdt::tree<head_tree_disc> edge_by_head_order;
		typedef cdt::ordering<node_tree,node_sequence> node_order;


		struct graph_adtdata {
			graph_adtdata() :
				m_inedgetreedict(m_inedgetreedisc),
				m_outedgetreedict(m_outedgetreedisc),
				m_headtreedict(m_headdisc),
				m_nodetreedict(m_nodetreedisc),
				m_nodes(m_nodetreedisc,m_nodetreedict)
			{}

			inedge_tree_disc m_inedgetreedisc;
			outedge_tree_disc m_outedgetreedisc;
			inedge_tree_dict m_inedgetreedict;
			outedge_tree_dict m_outedgetreedict;
			head_tree_disc m_headdisc;
			head_tree_dict m_headtreedict;
			node_tree_disc m_nodetreedisc;
			node_tree_dict m_nodetreedict;
			node_order m_nodes;

			typename node_order::iterator get_iter(Node *n) {
				return m_nodes.make_iter(static_cast<cdt::seqlink*>(n));
			}
		};
		struct edge_parent : inseqlink,outseqlink,intreelink,outtreelink,headtreelink {};
		struct edge_data {};
		struct node_parent : cdt::seqtreelink {};
		struct node_data {
			inedge_order m_ins;
			outedge_order m_outs;
			edge_by_head_order m_outFinder; // used exclusively by find_edge(t,h)
			node_data(graph_adtdata &gadt) :
				m_ins(gadt.m_inedgetreedisc,gadt.m_inedgetreedict),
				m_outs(gadt.m_outedgetreedisc,gadt.m_outedgetreedict),
				m_outFinder(gadt.m_headdisc,gadt.m_headtreedict)
			{}
		};
	};
};

#endif //LGRAPH_CDT_H
