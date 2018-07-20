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


#include "DynaDAG.h"

using namespace std;

namespace Dynagraph {
namespace DynaDAG {

//#define DOUBLE_MEDIANS
#ifdef DOUBLE_MEDIANS
#define med_type double
#define cvt_med
#define med_eq(m1,m2) (fabs(m1-m2)<0.25)
#else
#define med_type int
#define cvt_med ROUND
#define med_eq(m1,m2) (m1==m2)
#endif

struct ConstraintMatrixSwitchable {
    struct BadRank {
        int r_;
        BadRank(int r) : r_(r) {}
    };
    void initRank(int r,int size) {
        vector<vector<bool> > &rm = data_[r];
        rm.resize(size);
        for(int i=0;i<size;++i)
            rm[i].resize(size,true);
    }
    // could do triangular matrix but instead go redundant
    void set(DDModel::Node *l,DDModel::Node *r,bool val) {
        dgassert(gd<DDNode>(l).rank==gd<DDNode>(r).rank);
        int rank = gd<DDNode>(l).rank,
            u = gd<DDNode>(l).preorder,
            v = gd<DDNode>(r).preorder;
        data_[rank][u][v] = data_[rank][v][u] = val;
    }
    bool canSwitch(DDModel::Node *l,DDModel::Node *r) const {
        dgassert(gd<DDNode>(l).rank==gd<DDNode>(r).rank);
        int rank = gd<DDNode>(l).rank,
            u = gd<DDNode>(l).preorder,
            v = gd<DDNode>(r).preorder;
        ConstraintMatrix::const_iterator rowMat = data_.find(rank);
        if(rowMat==data_.end())
            throw BadRank(rank);
        return rowMat->second[u][v];
    }
private:
    typedef map<int,vector<vector<bool> > > ConstraintMatrix;
    ConstraintMatrix data_;
};
struct MedianCompare {
    UpDown m_dir;
    bool allowEqual_;
    MedianCompare(UpDown dir,bool allowEqual) : m_dir(dir),allowEqual_(allowEqual) {}
    bool comparable(DDModel::Node *n) {
        return MValExists(n,m_dir);
    }
    bool shouldSwitch(DDModel::Node *l,DDModel::Node *r) {
        med_type lm = cvt_med(MVal(l,m_dir)),
            rm = cvt_med(MVal(r,m_dir));
        return lm > rm || lm==rm && allowEqual_;
    }
};
struct CrossingCompare {
    Config &config;
    SiftMatrix &matrix;
    bool allowEqual_;
    CrossingCompare(Config &config,SiftMatrix &matrix,bool allowEqual) : config(config),matrix(matrix),allowEqual_(allowEqual) {}
    bool comparable(DDModel::Node *n) {
        return true;
    }
    bool shouldSwitch(DDModel::Node *l,DDModel::Node *r) {
        dgassert(gd<DDNode>(l).rank==gd<DDNode>(r).rank);
        dgassert(gd<DDNode>(l).order<gd<DDNode>(r).order);
        Rank *rank = config.ranking.GetRank(gd<DDNode>(l).rank);
        int numcross=0;
        for(int o = gd<DDNode>(l).order; o<=gd<DDNode>(r).order; ++o)
            numcross += matrix.allCrossings(rank->order[o],l) - matrix.allCrossings(l,rank->order[o]);
        return numcross<0 || numcross==0&&allowEqual_;
    }
};
void moveBefore(Config &config,SiftMatrix &matrix,DDModel::Node *n,DDModel::Node *before) {
    matrix.move(n,before);
    /*
    int rank = gd<DDNode>(n).rank;
    config.RemoveNode(n);
    if(before)
        config.InstallAtOrder(n,rank,gd<DDNode>(before).order);
    else
        config.InstallAtRight(n,rank);
        */
    config.MoveNodeBefore(n,before);
}
template<class Switchable, class Compare>
void bubblePassR(Config &config,SiftMatrix &matrix,Rank *r,const Switchable &switchable,Compare &compare) {
    NodeV::iterator li;
    for(li = r->order.begin(); li<r->order.end()-1; ++li) {
        if(!compare.comparable(*li))
            continue;
        // search to right: if you find something you can't switch, go on to next li
        // if you find something to switch, jump to place it's been put
        for(NodeV::iterator ri = li+1;ri!=r->order.end(); ++ri)
            if(!switchable.canSwitch(*li,*ri))
                break;
            else if(compare.comparable(*ri)) {
                if(compare.shouldSwitch(*li,*ri)) {
                    moveBefore(config,matrix,*li,(ri==r->order.end()-1)?0:*(ri+1));
                    li = ri;
                }
                break;
            }
    }
}
template<class Switchable, class Compare>
void bubblePassL(Config &config,SiftMatrix &matrix,Rank *r,const Switchable &switchable,Compare &compare) {
    NodeV::reverse_iterator ri;
    for(ri = r->order.rbegin(); ri<r->order.rend()-1; ++ri) {
        if(!compare.comparable(*ri))
            continue;
        for(NodeV::reverse_iterator li = ri+1;li!=r->order.rend(); ++li)
            if(!switchable.canSwitch(*li,*ri))
                break;
            else if(compare.comparable(*li)) {
                if(compare.shouldSwitch(*li,*ri)) {
                    moveBefore(config,matrix,*ri,*li);
                    ri = li-1;
                }
                break;
            }
    }
}
template<class Switchable, class Compare>
void bubblePass(Config &config,SiftMatrix &matrix,UpDown dir,LeftRight way,const Switchable &switchable,Compare &compare) {
    if(dir==DOWN)
        for(Config::Ranks::iterator ri = config.ranking.begin(); ri!=config.ranking.end(); ++ri) {
            Rank *r = *ri;
            if(way==RIGHT)
                bubblePassR(config,matrix,r,switchable,compare);
            else
                bubblePassL(config,matrix,r,switchable,compare);
        }
    else
        for(Config::Ranks::reverse_iterator ri = config.ranking.rbegin(); ri!=config.ranking.rend(); ++ri) {
            Rank *r = *ri;
            if(way==RIGHT)
                bubblePassR(config,matrix,r,switchable,compare);
            else
                bubblePassL(config,matrix,r,switchable,compare);
        }
    //matrix.check();
}
template<int MaxPasses,int Tire,int GoodEnough,int AbsoluteMustScore,int AbsoluteMaxPasses,typename ToDo>
int borableRun(Config &config,SiftMatrix &matrix,ToDo &toDo) {
    SiftMatrix backupM(config);
    Config::Ranks backupC(config.current);

    config.checkX();
    config.ranking.backup_x();
    backupC = config.ranking;

    matrix.recompute();
    backupM = matrix;

    unsigned best = toDo.Score();
    int pass = 0;
    while((best>AbsoluteMustScore && pass<AbsoluteMaxPasses) || pass<MaxPasses && best>GoodEnough) {
        int tired = 0;
        unsigned score=0;
        while((best>AbsoluteMustScore && pass<AbsoluteMaxPasses) || pass<MaxPasses && best>GoodEnough && tired<Tire) {
            score = toDo.Pass(pass);
            if(score<best) {
                best = score;
                config.checkX();
                config.ranking.backup_x();
                backupC = config.ranking;
                backupM = matrix;
                tired = 0;
            }
            else
                ++tired;
            ++pass;
        }
        if(score>best || tired==Tire) {
            config.Restore(backupC);
            matrix = backupM;
            tired = 0;
        }
    }
    return best;
};

struct LightPass {
    Config &config_;
    SiftMatrix &matrix_;
    ConstraintMatrixSwitchable switchable_;
    MedianCompare median_;
    CrossingCompare crossing_;
    LightPass(Config &config,SiftMatrix &matrix,ConstraintMatrixSwitchable &switchable) : 
        config_(config),
        matrix_(matrix),
        switchable_(switchable),
        median_(DOWN,false),
        crossing_(config_,matrix_,false) 
    {
        matrix_.light_ = true;
        matrix_.recompute();
    }
    int Score() {
        return matrix_.sumCrossings();
    }
    int Pass(int pass) {
        median_.allowEqual_ = pass%8>4;
        crossing_.allowEqual_ = !median_.allowEqual_;
        LeftRight way = (pass%2) ? RIGHT : LEFT;
        UpDown dir;
        if(pass&2) {
            median_.m_dir = UP;
            dir = DOWN;
        }
        else {
            median_.m_dir = DOWN;
            dir = UP;
        }

        bubblePass(config_,matrix_,dir,way,switchable_,median_);

        unsigned score2 = Score(),prevScore;
        do {
            prevScore = score2;
            bubblePass(config_,matrix_,dir,way,switchable_,crossing_);
            score2 = Score();
            dgassert(score2<=prevScore);
        }
        while(score2<prevScore);
        return score2;
    }
};
struct HeavyPass {
    Config &config_;
    SiftMatrix &matrix_;
    ConstraintMatrixSwitchable switchable_;
    CrossingCompare crossing_;
    HeavyPass(Config &config,SiftMatrix &matrix,ConstraintMatrixSwitchable &switchable) :
        config_(config),
        matrix_(matrix),
        switchable_(switchable),
        crossing_(config_,matrix_,false) 
    {
        matrix_.light_ = false;
        matrix_.recompute();
    }
    int Score() {
        return matrix_.sumCrossings();
    }
    int Pass(int pass) {
        LeftRight way = (pass%2) ? RIGHT : LEFT;
        UpDown dir = (pass&2) ? UP : DOWN;
        crossing_.allowEqual_ = pass%3;
        bubblePass(config_,matrix_,dir,way,switchable_,crossing_);
        return Score();
    }
};
struct OrderLess {
    bool operator()(DDModel::Node *u,DDModel::Node *v) {
        return gd<DDNode>(u).order < gd<DDNode>(v).order;
    }
};
struct RankLess {
    bool operator()(DDModel::Node *u,DDModel::Node *v) {
        if(gd<DDNode>(u).rank == gd<DDNode>(v).rank)
            return OrderLess().operator()(u,v);
        return gd<DDNode>(u).rank < gd<DDNode>(v).rank;
    }
};
#define TIRE 6
void DotlikeOptimizer::Reorder(DDChangeQueue &Q,DynaDAGLayout &nodes,DynaDAGLayout &edges) {
    vector<int> affectedRanks;
    ConstraintMatrixSwitchable switchable;
    for(Config::Ranks::iterator ri = config.ranking.begin(); ri!=config.ranking.end(); ++ri)
        switchable.initRank(Config::Ranks::Xlator::CoordToRank(config.current,((*ri)->yBase)), 
            (*ri)->order.size());
    {
        NodeV optimVec;
        getCrossoptModelNodes(nodes,edges,optimVec);
        if(optimVec.empty())
            return;
        sort(optimVec.begin(),optimVec.end(),RankLess());
        NodeV::iterator wot = optimVec.begin();
        DDModel::Node *lastNoOpti=0;
        for(Config::Ranks::iterator ri = config.ranking.begin(); ri!=config.ranking.end(); ++ri)
            for(NodeV::iterator ni = (*ri)->order.begin(); ni!=(*ri)->order.end(); ++ni) {
                gd<DDNode>(*ni).preorder = gd<DDNode>(*ni).order;
                if(wot!=optimVec.end() && *ni==*wot) {
                    //gd<DDNode>(*ni).orderConstraint = -1;
                    ++wot;
                }
                else {
                    if(lastNoOpti&&gd<DDNode>(lastNoOpti).rank==gd<DDNode>(*ni).rank)
                        switchable.set(lastNoOpti,*ni,false);
                    lastNoOpti = *ni;
                    //gd<DDNode>(*ni).orderConstraint = gd<DDNode>(*ni).order;
                }
            }
        dgassert(wot==optimVec.end());
        loops.Field(dgr::crossopt,"model nodes for crossopt",optimVec.size());
        loops.Field(dgr::crossopt,"total model nodes",optimVec.front()->g->nodes().size());

        for(NodeV::iterator ni = optimVec.begin(); ni!=optimVec.end(); ++ni)
            if(affectedRanks.empty() || affectedRanks.back()!=gd<DDNode>(*ni).rank)
                affectedRanks.push_back(gd<DDNode>(*ni).rank);
        loops.Field(dgr::crossopt,"ranks for crossopt",affectedRanks.size());
        loops.Field(dgr::crossopt,"total ranks",config.ranking.size());
    }
    NodeV fanVec;
    for(DynaDAGLayout::node_iter ni = config.current->nodes().begin(); ni!=config.current->nodes().end(); ++ni) {
        if(gd<NodeGeom>(*ni).freezeInOrder) {
            // klunky but efficient?
            DDModel::Node *top = DDp(*ni)->top();
            fanVec.resize(0);
            fanVec.reserve(top->ins().size());
            for(DDModel::inedge_iter ei = top->ins().begin(); ei!=top->ins().end(); ++ei) {
                dgassert(gd<DDEdge>(*ei).amEdgePart());
                // only freeze old edges
                if(!Q.insE.find(gd<DDEdge>(*ei).path->layoutE))
                    fanVec.push_back((*ei)->tail);
            }
            sort(fanVec.begin(),fanVec.end(),OrderLess());
            for(NodeV::iterator ni = fanVec.begin(); ni!=fanVec.end(); ++ni)
                if(ni!=fanVec.begin())
                    switchable.set(*(ni-1),*ni,false);
        }
        if(gd<NodeGeom>(*ni).freezeOutOrder) {
            // klunky but efficient?
            DDModel::Node *bottom = DDp(*ni)->bottom();
            fanVec.resize(0);
            fanVec.reserve(bottom->outs().size());
            for(DDModel::outedge_iter ei = bottom->outs().begin(); ei!=bottom->outs().end(); ++ei) {
                dgassert(gd<DDEdge>(*ei).amEdgePart());
                // only freeze old edges
                if(!Q.insE.find(gd<DDEdge>(*ei).path->layoutE))
                    fanVec.push_back((*ei)->head);
            }
            sort(fanVec.begin(),fanVec.end(),OrderLess());
            for(NodeV::iterator ni = fanVec.begin(); ni!=fanVec.end(); ++ni)
                if(ni!=fanVec.begin())
                    switchable.set(*(ni-1),*ni,false);
        }
    }
    SiftMatrix matrix(config);
    LightPass lightPass(config,matrix,switchable);
    borableRun<32,6,0,INT_MAX,32>(config,matrix,lightPass);

    HeavyPass heavyPass(config,matrix,switchable);
    int score = heavyPass.Score();
    if(score<NODECROSS_PENALTY) {
        loops.Field(dgr::crossopt,"weighted crossings after heavy pass",-1);
        return;
    }
    score = borableRun<32,5,NODECROSS_PENALTY-1,NODECROSS_PENALTY*NODECROSS_PENALTY,100>(config,matrix,heavyPass);

    loops.Field(dgr::crossopt,"weighted crossings after heavy pass",score);
    // absolutely must not leave here with nodes crossing nodes!!
    dgassert(score<NODECROSS_PENALTY*NODECROSS_PENALTY);
}
double DotlikeOptimizer::Reopt(DDModel::Node *n,UpDown dir) {
    return gd<DDNode>(n).cur.x;
}

} // namespace DynaDAG
} // namespace Dynagraph
