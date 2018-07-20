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
*               This version available from               *
*                   http://dynagraph.org                  *
**********************************************************/

#ifndef bufferGraphStream_h
#define bufferGraphStream_h

//#include <io.h>
#include <fcntl.h>

namespace Dynagraph {

// at one point i was able to nest two yacc parsers okay, but it stopped working,
// ag_yyparse seems to mess up the state of the input stream for gs_yyparse.
// so just make a brand new stream for each agread.
// (this is only successful sometimes)
struct bufferGraphStream {
    FILE *fin;
    static std::pair<bool,int> braceCount(const char *s) {
        bool action = false;
        int ct = 0;
        for(const char *ci = s; *ci; ++ci)
            if(*ci=='{')
                ++ct,action=true;
            else if(*ci=='}')
                --ct,action=true;
        return std::make_pair(action,ct);
    }
    bufferGraphStream(FILE *f) : fin(0) {
        fin = tmpfile();
        bool action=false;
        int countBrace = 0;
        while(!action || countBrace) {
            char buf2[400];
            fgets(buf2,400,f);
            fputs(buf2,fin);
            std::pair<bool,int> bi = braceCount(buf2);
            action |= bi.first;
            countBrace += bi.second;
        }
        fseek(fin,0,SEEK_SET);
        setvbuf(fin,0,_IONBF,0);
    }
    ~bufferGraphStream() {
        if(fin)
            fclose(fin);
    }
};

} // namespace Dynagraph

#endif //bufferGraphStream_h
