typedef union	{
			int				i;
			char			*str;
} YYSTYPE;
#define	T_graph	257
#define	T_node	258
#define	T_edge	259
#define	T_pattern	260
#define	T_search	261
#define	T_input	262
#define	T_open	263
#define	T_close	264
#define	T_insert	265
#define	T_delete	266
#define	T_modify	267
#define	T_lock	268
#define	T_unlock	269
#define	T_segue	270
#define	T_define	271
#define	T_id	272
#define	T_edgeop	273
#define	T_subgraph	274


extern YYSTYPE gs_yylval;
