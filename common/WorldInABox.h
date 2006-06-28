#ifndef WorldInABox_h
#define WorldInABox_h

namespace Dynagraph {

template<typename OuterLayout,typename InnerLayout,typename InTranslator,typename OutTranslator>
struct WorldInABox : LinkedChangeProcessor<OuterLayout> {
	ChangingGraph<InnerLayout> inWorld_;
	LinkedChangeProcessor<OuterLayout> *inXlator_,*outXlator_;
	WorldInABox(ChangingGraph<OuterLayout> *world) 
		: LinkedChangeProcessor<OuterLayout>(world),inXlator_(0),outXlator_(0) {}
	void assignEngine(DString engines) {
		InTranslator *inTranslator = new InTranslator(this->world_,&inWorld_);
		OutTranslator *outTranslator = new OutTranslator(&inWorld_,this->world_);
		EnginePair<InnerLayout> innerEngines = createEngine(engines,&inWorld_);
		static_cast<LinkedChangeProcessor<InnerLayout>*>(inTranslator)->next_ = innerEngines.first;
		innerEngines.second->next_ = outTranslator;
		inXlator_ = inTranslator;
		outXlator_ = outTranslator;
	}
	EnginePair<OuterLayout> engines() {
		return EnginePair<OuterLayout>(this,outXlator_);
	}
	void Process() {
		inXlator_->Process();
		// there is no particular reason WiaB should be responsible for
		// clearing the queue but neither does it make sense to 
		// create an "Okay Engine" when we don't *really* understand changes yet
		// in effect this assumes that what "really matters" is in the inner world
		// which is true at the moment
		this->world_->Q_.ExecuteDeletions();
		this->world_->Q_.Clear();
	}
};

} // namespace Dynagraph

#endif // WorldInABox_h
