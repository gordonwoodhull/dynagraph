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


struct SiftMatrix {
	bool m_light;
	SiftMatrix(Config &config) : m_light(true),config(config) {
	}
	unsigned weigh(Crossings cc) {
		return m_light?crosslight(cc):crossweight(cc);
	}
	std::pair<unsigned,unsigned> &crossings(DDModel::Node *u,DDModel::Node *v) {
		assert(DDd(u).rank==DDd(v).rank);
		return data[DDd(u).rank][DDd(u).order][DDd(v).order];
	}
	unsigned getCrossings(DDModel::Node *u, DDModel::Node *v,bool outs) {
		std::pair<unsigned,unsigned> &p = crossings(u,v);
		assert(p.first>=0 && p.second>=0);
		if(outs)
			return p.second;
		else
			return p.first;
	}
	void setCrossings(DDModel::Node *u, DDModel::Node *v,bool outs,unsigned val) {
		assert(val>=0);
		std::pair<unsigned,unsigned> &p = crossings(u,v);
		assert(p.first>=0 && p.second>=0);
		if(outs)
			p.second = val;
		else
			p.first = val;
	}
	unsigned allCrossings(DDModel::Node *u, DDModel::Node *v) {
		return getCrossings(u,v,true) + getCrossings(u,v,false);
	}
	SiftMatrix &operator =(SiftMatrix &other) {
		data = other.data;
		return *this;
	}
	void recompute();
	unsigned sumCrossings() {
		unsigned ret = 0;
		for(Config::Ranks::iterator ri = config.ranking.begin(); ri!=config.ranking.end(); ++ri) {
			Rank *r = *ri;
			if(r->order.size()<2)
				continue;
			NodeV::iterator end = r->order.end()-1;
			for(NodeV::iterator ni = r->order.begin(); ni<end; ++ni)
				for(NodeV::iterator ni2 = ni+1;ni2!=r->order.end(); ++ni2) {
					unsigned c = getCrossings(*ni,*ni2,false);
					assert(c>=0);
					ret += c;
				}
		}
		return ret;
	}
	void move(DDModel::Node *v, DDModel::Node *before);
	void checkWithConfig();
private:
	typedef std::pair<unsigned,unsigned> CrossCount;
	typedef std::vector<std::vector<CrossCount> > Layer;
	typedef std::map<unsigned,Layer> Data;
	Data data;
	Config &config;
	void updateOuts(DDModel::Node *v,DDModel::Node *x);
	void updateIns(DDModel::Node *v,DDModel::Node *x);
	void update(DDModel::Node *v,DDModel::Node *x);
	void rowCopy(unsigned rank,unsigned dst,unsigned src);
	void colCopy(unsigned rank,unsigned dst,unsigned src);
	void rowPaste(unsigned rank,unsigned dst,const std::vector<CrossCount> &src);
	void colPaste(unsigned rank,unsigned dst,const std::vector<CrossCount> &src);
};
