// act like a string graph but adjust the real thing too
struct DinoMachineHandler : AbsGraphHandler<DinoMachine> {
    DinoMachineHandler(DinoMachine *dino) : AbsGraphHandler<DinoMachine>(dino) {} // required!

    DString incr_ev_ins_node(DString name, const StrAttrs &attrs, bool merge);
    DString incr_ev_ins_edge(DString name, DString tailname, DString headname, const StrAttrs &attrs);
    bool incr_ev_mod_node(DString name,const StrAttrs &attrs);
    bool incr_ev_mod_edge(DString name,const StrAttrs &attrs);
	bool incr_ev_del_node(DString name);
	bool incr_ev_del_edge(DString name);

    void modify_node(DString name,const StrAttrs &attrs);
    void modify_edge(DString name,const StrAttrs &attrs);
};
