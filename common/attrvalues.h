extern DString g_NLPNames[5]; // in stringsOut.cpp
NodeLabelAlignment string2nlp(DString s) {
	for(int i=0;i<5;++i)
		if(g_NLPNames[i]==s)
			return static_cast<NodeLabelAlignment>(i);
	return DG_NODELABEL_CENTER; 
}
