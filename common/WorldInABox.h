#ifndef WorldInABox_h
#define WorldInABox_h

namespace Dynagraph {

template<typename OuterLayout,typename InnerLayout,typename InTranslator,typename OutTranslator>
struct WorldInABox : LinkedChangeProcessor<OuterLayout> {
	ChangingGraph<InnerLayout> world_;
	EnginePair<InnerLayout> innerEngines_;
	ChangeProcessor<OuterLayout> *topEngine_;
	OutTranslator *xlateOut_;
	WorldInABox() : topEngine_(0),xlateOut_(0) {}
	void assignEngine(DString engines,ChangingGraph<OuterLayout> &topWorld,InTranslator *inTranslator = new InTranslator,OutTranslator *outTranslator = new OutTranslator) {
		if(innerEngines_.second)
			innerEngines_.second->next_ = 0;
		ChangeTranslator<OuterLayout,InnerLayout> *xlateIn = inTranslator;
		xlateOut_ = outTranslator;
		innerEngines_ = createEngine(engines,&world_.whole_,&world_.current_);
		static_cast<LinkedChangeProcessor<InnerLayout>*>(xlateIn)->next_ = innerEngines_.first;
		innerEngines_.second->next_ = xlateOut_;
		topEngine_ = xlateIn;
	}
	EnginePair<OuterLayout> engines() {
		return EnginePair<OuterLayout>(this,xlateOut_);
	}
	void Process(ChangeQueue<OuterLayout> &Q) {
		xlateOut_->transition_.AssignNext(&Q);
		topEngine_->Process(Q);
	}
};

} // namespace Dynagraph

#endif // WorldInABox_h
