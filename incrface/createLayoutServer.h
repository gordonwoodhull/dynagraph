/**********************************************************
*      This software is part of the graphviz toolset      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2005 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
*                                                         *
*                   *        *        *                   *
*            Current source code available from           *
*                http://gordon.woodhull.com               *
**********************************************************/

struct ServerUnknown : DGException {
  DString serverName;
  ServerUnknown() : DGException("incremental engine name unknown") {}
};
// creates the servers specified in gd<StrAttrs>(client)["engines"]
Server *createLayoutServer(Layout *client,Layout *current);
