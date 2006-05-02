#ifndef WorldInABox_h
#define WorldInABox_h

namespace Dynagraph {

template<typename OuterLayout,typename InnerLayout,typename InTranslator,typename OutTranslator>
struct WorldInABox : LinkedChangeProcessor<OuterLayout> {
	DynagraphWorld<InnerLayout> world_;
	EnginePair<InnerLayout> innerEngines_;
	ChangeProcessor<OuterLayout> *topEngine_;
	OutTranslator *xlateOut_;
	WorldInABox() : topEngine_(0),xlateOut_(0) {}
	void assignEngine(DString engines,DynagraphWorld<OuterLayout> &topWorld) {
		if(innerEngines_.second)
			innerEngines_.second->next_ = 0;
		ChangeTranslator<OuterLayout,InnerLayout> *xlateIn = new InTranslator(GoingNamedTransition<OuterLayout,InnerLayout>(&world_.whole_,&world_.current_));
		xlateOut_ = new OutTranslator;
		innerEngines_ = createEngine(engines,&world_.whole_,&world_.current_);
		xlateIn->next_ = innerEngines_.first;
		innerEngines_.second->next_ = xlateOut_;
		topEngine_ = xlateIn;
	}
	void Process(ChangeQueue<OuterLayout> &Q) {
		xlateOut_->transition_.nextQ_ = &Q;
		topEngine_->Process(Q);
		NextProcess(Q);
	}
};

} // namespace Dynagraph

#endif // WorldInABox_h
