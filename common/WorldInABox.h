#ifndef WorldInABox_h
#define WorldInABox_h

namespace Dynagraph {

// not entirely pretty: yes it makes sense to have an engine that just clears Qs
// (if it's the responsibility of engines to clear Qs, which seems to be so)
// but what's up with whoseNext?  it's just there so that WorldInABox can look like a regular engine
template<typename Layout>
struct OkayEngine : LinkedChangeProcessor<Layout> {
	LinkedChangeProcessor<Layout> *whoseNext_;
	OkayEngine(ChangingGraph<Layout> *world,LinkedChangeProcessor<Layout> *whoseNext) : LinkedChangeProcessor<Layout>(world),whoseNext_(whoseNext) {}
	void Process() {
		whoseNext_->next_->Process();
		this->world_->Q_.ExecuteDeletions();
		this->world_->Q_.Clear();
	}
};
template<typename OuterLayout,typename InnerLayout,typename InTranslator,typename OutTranslator>
struct WorldInABox : LinkedChangeProcessor<OuterLayout> {
	ChangingGraph<InnerLayout> inWorld_;
	LinkedChangeProcessor<OuterLayout>  *inXlator_,*outXlator_,*okayEngine_;

	WorldInABox(ChangingGraph<OuterLayout> *world) 
		: LinkedChangeProcessor<OuterLayout>(world) {
	}
	~WorldInABox() {
		// chain of deletion will get outXlator and okayEngine automagically
		if(inXlator_)
			delete inXlator_;
	}
	void assignEngine(DString engines) {
		InTranslator *inTranslator = new InTranslator(this->world_,&inWorld_);
		OutTranslator *outTranslator = new OutTranslator(&inWorld_,this->world_);
		EnginePair<InnerLayout> innerEngines = createEngine(engines,&inWorld_);
		innerEngines.Prepend(inTranslator);
		innerEngines.Append(outTranslator);
		okayEngine_  = new OkayEngine<OuterLayout>(this->world_,this);
		static_cast<LinkedChangeProcessor<OuterLayout>*>(outTranslator)->next_ = okayEngine_;
		inXlator_ = inTranslator;
		outXlator_ = outTranslator;
	}
	void Process() {
		inXlator_->Process();
	}
};

} // namespace Dynagraph

#endif // WorldInABox_h
