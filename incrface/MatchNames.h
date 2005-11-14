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


struct MatchNames : DinoInternalChanges {
    DinoInternalChanges *m_chain;
	DinoMachine::Edge *m_dinoe;
	StrGraph *m_source;
	DynaView *m_dest;
    MatchNames(DinoInternalChanges *chain,DinoMachine::Edge *de) : m_chain(chain),m_dinoe(de) {
        DinoMachine::Node *t = de->tail,
			*h = de->head;
		assert(gd<DinoMachNode>(t).handler->dinotype()=="abstract");
		assert(gd<DinoMachNode>(h).handler->dinotype()=="layout");
		m_source = static_cast<AbsGraphHandler<StrGraph>*>(gd<DinoMachNode>(t).handler)->g;
		m_dest = static_cast<DynaView*>(gd<DinoMachNode>(h).handler);
	}
    ~MatchNames() {
        if(m_chain)
            delete m_chain;
    }

	void GraphChanged() {
        if(m_chain)
            m_chain->GraphChanged();
        m_source->oopsRefreshDictionary();

		// finding changes is easier than e.g. in diff_strgraph
		// because everything must have a name and must be entered in the map
		// so we compare the *map* to the changed graph to see what to change on the other side

        // this could be generalized if Layout were a NamedGraph
		DinoMachEdge &dme = gd<DinoMachEdge>(m_dinoe);
        IncrLangEvents *desthand = gd<DinoMachNode>(m_dinoe->head).handler;
        desthand->incr_ev_lock();
        for(NEID_map::tmap::iterator ii = dme.tailmap().begin(),ji; ii!=dme.tailmap().end(); ii = ji) {
			(ji = ii)++;
            if(ii->second.empty())
                continue;
			if(ii->first.isEdge) {
				if(!m_source->edict[ii->first.name]) { // deleted edge
					NEID_map::tset &s = ii->second;
					assert(s.size()==1);
					DString bname = s.begin()->name;
                    desthand->incr_ev_del_edge(bname);
					dme.erase_tail(ii->first);
					dme.erase_head(NEID(true,bname));
				}
			}
		}
        for(NEID_map::tmap::iterator ii = dme.tailmap().begin(),ji; ii!=dme.tailmap().end(); ii = ji) {
			(ji = ii)++;
            if(ii->second.empty())
                continue;
			if(!ii->first.isEdge) {
				if(!m_source->ndict[ii->first.name]) { // deleted node
					NEID_map::tset &s = ii->second;
					assert(s.size()==1);
					DString bname = s.begin()->name;
                    desthand->incr_ev_del_node(bname);
					dme.erase_tail(ii->first);
					dme.erase_head(NEID(false,bname));
				}
			}
        }
        for(StrGraph::NDict::iterator ni = m_source->ndict.begin(); ni!=m_source->ndict.end(); ++ni) {
            if(!ni->second)
                continue;
			NEID_map::tset &s = dme.tailmap()[NEID(false,ni->first)];
			if(s.empty()) { // added node
                assert(dme.tailmap()[NEID(false,ni->first)].size()==0);
                assert(dme.headmap()[NEID(false,ni->first)].size()==0);
                desthand->incr_ev_ins_node(ni->first,StrAttrs(),true);
			    dme.connect(NEID(false,ni->first),NEID(false,ni->first));
                assert(dme.tailmap()[NEID(false,ni->first)].size()==1);
                assert(dme.headmap()[NEID(false,ni->first)].size()==1);
                assert(dme.tailmap()[NEID(false,ni->first)].begin()->name==ni->first);
                assert(dme.headmap()[NEID(false,ni->first)].begin()->name==ni->first);
			}
		}
        for(StrGraph::EDict::iterator ei = m_source->edict.begin(); ei!=m_source->edict.end(); ++ei) {
            if(!ei->second)
                continue;
			NEID_map::tset &s = dme.tailmap()[NEID(true,ei->first)];
			if(s.empty()) { // added edge
                DString bt = dme.tailmap()[NEID(false,gd<Name>(ei->second->tail))].begin()->name,
                    bh = dme.tailmap()[NEID(false,gd<Name>(ei->second->head))].begin()->name;
                assert(dme.tailmap()[NEID(true,ei->first)].size()==0);
                assert(dme.headmap()[NEID(true,ei->first)].size()==0);
                desthand->incr_ev_ins_edge(ei->first,bt,bh,StrAttrs());
			    dme.connect(NEID(true,ei->first),NEID(true,ei->first));
                assert(dme.tailmap()[NEID(true,ei->first)].size()==1);
                assert(dme.headmap()[NEID(true,ei->first)].size()==1);
                assert(dme.tailmap()[NEID(true,ei->first)].begin()->name==ei->first);
                assert(dme.headmap()[NEID(true,ei->first)].begin()->name==ei->first);
			}
        }
        desthand->incr_ev_unlock();
    }
};
