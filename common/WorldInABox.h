#ifndef WorldInABox_h
#define WorldInABox_h

#include "common/NamedToNamedChangeTranslator.h"

namespace Dynagraph {

template<typename Layout1,typename Layout2,typename InTranslator,typename OutTranslator>
struct WorldInABox : LinkedChangeProcessor<Layout1> {
	typedef NamedToNamedChangeTranslator<Layout1,Layout2,GoingNamedTransition<Layout1,Layout2>,InTranslator> XlateIn;
	typedef NamedToNamedChangeTranslator<Layout2,Layout1,ReturningNamedTransition<Layout2,Layout1>,OutTranslator> XlateOut;
	IncrWorld<Layout2> world_;
	EnginePair<Layout2> innerEngines_;
	ChangeProcessor<Layout1> *topEngine_;
	XlateOut *xlateOut_;
	WorldInABox() : topEngine_(0),xlateOut_(0) {}
	void assignEngine(DString engines,IncrWorld<Layout1> &topWorld) {
		if(innerEngines_.second)
			innerEngines_.second->next_ = 0;
		XlateIn *xlateIn = new XlateIn(GoingNamedTransition<Layout1,Layout2>(&world_.whole_,&world_.current_));
		xlateOut_ = new XlateOut;
		innerEngines_ = createEngine(engines,&world_.whole_,&world_.current_);
		xlateIn->next_ = innerEngines_.first;
		innerEngines_.second->next_ = xlateOut_;
		topEngine_ = xlateIn;
	}
	void Process(ChangeQueue<Layout1> &Q) {
		xlateOut_->transition_.nextQ_ = &Q;
		topEngine_->Process(Q);
		NextProcess(Q);
	}
};

} // namespace Dynagraph

#endif // WorldInABox_h
