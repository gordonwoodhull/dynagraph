#ifndef WorldInABox_h
#define WorldInABox_h

namespace Dynagraph {

template<typename OuterLayout,typename InnerLayout,typename InTranslator,typename OutTranslator>
struct WorldInABox : LinkedChangeProcessor<OuterLayout> {
	ChangingGraph<InnerLayout> inWorld_;
	EnginePair<InnerLayout> innerEngines_;
	ChangeProcessor<OuterLayout> *topEngine_;
	OutTranslator *xlateOut_;
	WorldInABox(ChangingGraph<OuterLayout> *world) 
		: LinkedChangeProcessor<OuterLayout>(world),topEngine_(0),xlateOut_(0) {}
	void assignEngine(DString engines) {
		InTranslator *inTranslator = new InTranslator(this->world_,&inWorld_);
		OutTranslator *outTranslator = new OutTranslator(&inWorld_,this->world_);
		if(innerEngines_.second)
			innerEngines_.second->next_ = 0;
		ChangeTranslator<OuterLayout,InnerLayout> *xlateIn = inTranslator;
		xlateOut_ = outTranslator;
		innerEngines_ = createEngine(engines,&inWorld_);
		static_cast<LinkedChangeProcessor<InnerLayout>*>(xlateIn)->next_ = innerEngines_.first;
		innerEngines_.second->next_ = xlateOut_;
		topEngine_ = xlateIn;
	}
	EnginePair<OuterLayout> engines() {
		return EnginePair<OuterLayout>(this,xlateOut_);
	}
	void Process() {
		topEngine_->Process();
	}
};

} // namespace Dynagraph

#endif // WorldInABox_h
