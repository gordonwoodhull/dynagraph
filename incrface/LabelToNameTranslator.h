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


struct LabelToNameTranslator : DinoInternalChanges {
    DinoInternalChanges *m_chain;
	DinoMachine::Edge *m_dinoe;
	DynaView *m_source;
	StrGraph *m_dest;
    bool m_useNodeLabels,m_useEdgeLabels;
    LabelToNameTranslator(DinoInternalChanges *chain,DinoMachine::Edge *de,bool useNodeLabels,bool useEdgeLabels)
            : m_chain(chain),m_dinoe(de),m_useNodeLabels(useNodeLabels),m_useEdgeLabels(useEdgeLabels) {
        DinoMachine::Node *t = de->tail,
			*h = de->head;
		assert(gd<DinoMachNode>(t).handler->dinotype()=="layout");
		assert(gd<DinoMachNode>(h).handler->dinotype()=="abstract");
		m_source = static_cast<DynaView*>(gd<DinoMachNode>(t).handler);
		m_dest = static_cast<AbsGraphHandler<StrGraph>*>(gd<DinoMachNode>(h).handler)->g;
	}
    ~LabelToNameTranslator() {
        if(m_chain)
            delete m_chain;
    }

	void GraphChanged() {
        if(m_chain)
            m_chain->GraphChanged();
        m_dest->oopsRefreshDictionary();
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
				if(!m_source->edges[ii->first.name]) { // deleted edge
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
				if(!m_source->nodes[ii->first.name]) { // deleted node
					NEID_map::tset &s = ii->second;
					assert(s.size()==1);
					DString bname = s.begin()->name;
                    desthand->incr_ev_del_node(bname);
					dme.erase_tail(ii->first);
					dme.erase_head(NEID(false,bname));
				}
			}
        }
		for(DynaView::nodeDict::iterator ni = m_source->nodes.begin(); ni!=m_source->nodes.end(); ++ni) {
            if(!ni->second)
                continue;
			NEID_map::tset &s = dme.tailmap()[NEID(false,ni->first)];
			DString label;
            if(m_useNodeLabels)
                 label = gd<StrAttrs>(ni->second)["label"];
            if(label.empty())
                label = ni->first;
			if(s.empty()) { // added node
                DString newname;
                StrAttrs attrs;
				if(m_dest->ndict[label]) {
                    attrs["label"] = label;
                    newname = randomName('n');
				}
				else
                    newname = label;
                assert(dme.headmap()[NEID(false,newname)].size()==0);
			    dme.connect(NEID(false,ni->first),NEID(false,newname));
                assert(dme.headmap()[NEID(false,newname)].size()==1);
                desthand->incr_ev_ins_node(newname,attrs,true);
                assert(dme.headmap()[NEID(false,newname)].size()==1);
                assert(dme.tailmap()[NEID(false,ni->first)].size()==1);
                assert(dme.tailmap()[NEID(false,ni->first)].begin()->name==newname);
                assert(dme.headmap()[NEID(false,newname)].begin()->name==ni->first);
			}
			else {
                // not new, but label may have changed
                // or we might be able to grab name that was unavailable
				assert(s.size()==1);
                DString bname = s.begin()->name;
                if(bname!=label)
                    if(!m_dest->ndict[label]) {
                        assert(dme.headmap()[NEID(false,label)].size()==0);
                        assert(dme.headmap()[NEID(false,bname)].size()==1);
                        m_dest->rename(m_dest->ndict[bname],label);
                        assert(dme.headmap()[NEID(false,label)].size()==0);
                        assert(dme.headmap()[NEID(false,bname)].size()==1);
                        dme.rename_head(NEID(false,bname),NEID(false,label));
                        assert(dme.headmap()[NEID(false,bname)].size()==0);
                        assert(dme.headmap()[NEID(false,label)].size()==1);
                        assert(dme.tailmap()[NEID(false,ni->first)].size()==1);
                        assert(dme.tailmap()[NEID(false,ni->first)].begin()->name==label);
                        assert(dme.headmap()[NEID(false,label)].begin()->name==ni->first);
                    }
            }
			assert(s.size()==1);
		}
        for(DynaView::edgeDict::iterator ei = m_source->edges.begin(); ei!=m_source->edges.end(); ++ei) {
            if(!ei->second)
                continue;
			NEID_map::tset &s = dme.tailmap()[NEID(true,ei->first)];
			DString label;
            if(m_useEdgeLabels)
                label = gd<StrAttrs>(ei->second)["label"];
            if(label.empty())
                label = ei->first;
			if(s.empty()) { // added edge
                DString newname;
                StrAttrs attrs;
                if(m_dest->edict[label]) {
                    newname = randomName('e');
                    attrs["label"] = label;
                }
				else
                    newname = label;
                DString bt = dme.tailmap()[NEID(false,gd<Name>(ei->second->tail))].begin()->name,
                    bh = dme.tailmap()[NEID(false,gd<Name>(ei->second->head))].begin()->name;
                assert(dme.headmap()[NEID(true,newname)].size()==0);
			    dme.connect(NEID(true,ei->first),NEID(true,newname));
                assert(dme.headmap()[NEID(true,newname)].size()==1);
                desthand->incr_ev_ins_edge(newname,bt,bh,attrs);
                assert(dme.headmap()[NEID(true,newname)].size()==1);
                assert(dme.tailmap()[NEID(true,ei->first)].size()==1);
                assert(dme.tailmap()[NEID(true,ei->first)].begin()->name==newname);
                assert(dme.headmap()[NEID(true,newname)].begin()->name==ei->first);
			}
			else { // not new, but maybe we can grab the label name
				assert(s.size()==1);
                DString bname = s.begin()->name;
                if(bname!=label)
                    if(!m_dest->edict[label]) {
                        assert(dme.headmap()[NEID(true,label)].size()==0);
                        assert(dme.headmap()[NEID(true,bname)].size()==1);
                        m_dest->rename(m_dest->edict[bname],label);
                        assert(dme.headmap()[NEID(true,label)].size()==0);
                        assert(dme.headmap()[NEID(true,bname)].size()==1);
                        dme.rename_head(NEID(true,bname),NEID(true,label));
                        assert(dme.headmap()[NEID(true,bname)].size()==0);
                        assert(dme.tailmap()[NEID(true,ei->first)].size()==1);
                        assert(dme.headmap()[NEID(true,label)].size()==1);
                        assert(dme.tailmap()[NEID(true,ei->first)].begin()->name==label);
                        assert(dme.headmap()[NEID(true,label)].begin()->name==ei->first);
                    }
            }
			assert(s.size()==1);
        }
        desthand->incr_ev_unlock();
    }
};
