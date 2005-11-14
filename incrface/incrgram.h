typedef union	{
			int				i;
			char			*str;
} YYSTYPE;
#define	T_graph	257
#define	T_node	258
#define	T_edge	259
#define	T_open	260
#define	T_close	261
#define	T_insert	262
#define	T_delete	263
#define	T_modify	264
#define	T_lock	265
#define	T_unlock	266
#define	T_segue	267
#define	T_message	268
#define	T_request	269
#define	T_fulfil	270
#define	T_id	271
#define	T_edgeop	272
#define	T_subgraph	273


extern YYSTYPE incr_yylval;
