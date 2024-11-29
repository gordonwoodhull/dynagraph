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

#ifdef DYNAGRAPH_WASM
#include <emscripten.h>
#endif

#include <stdio.h>
#include <fstream>
#include <time.h>

#include "common/emitGraph.h"
#include "common/Transform.h"
#include "incrface/incrparse.h"
#include "incrface/IncrLangEvents.h"

#include "common/time-o-matic.h"
#include "DuplicateStream.h"
#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/stream.hpp>

#include "dynagraph.version.h"

using namespace std;
using namespace Dynagraph;

char *g_outdot=0;
int g_count=1;
bool g_xeptFatal=false;
int g_maxWait=-1;
bool g_randomizeWait = false;

namespace Dynagraph {
    StrAttrs g_defaultGraphAttrs;
    Transform *g_transform;
    bool g_useDotDefaults;
}

struct CouldntOpen {};

#ifndef DYNAGRAPH_NO_THREADS
#define TEXT_OUTPUT_MUTEX
#endif
template<typename V>
struct switchval {
    char c;
    V v;
    const char *desc;
};
switchval<dgr::reportType> g_reports[] = {
    {'i',dgr::input_raw,"(raw) input"},
    {'k',dgr::input_cooked,"(cooked) input"},
    {'R',dgr::incrface,"incrface output (defaults to stdout unless preempted)"},
    {'T',dgr::dotout,"output complete dot graph each layout"},
    {'c',dgr::crossopt,"crossing optimization stats"},
    {'t',dgr::timing,"timing breakdown"},
    {'d',dgr::dynadag,"dynadag tallies"},
    {'a',dgr::ranks,"rank debugging"},
    {'g',dgr::modelDump,"dump graph"},
    {'q',dgr::dumpQueue,"dump input queue before each cycle"},
    {'r',dgr::readability,"readability"},
    {'s',dgr::stability,"stability"},
    {'p',dgr::progress,"progress"},
    {'b',dgr::bug,"bug of the day"},
    {'n',dgr::inner_input,"inner engine input"},
    {'u',dgr::inner_output,"inner engine output"}
};
int g_nreports = sizeof(g_reports)/sizeof(switchval<dgr::reportType>);
template<typename V>
pair<bool,V> findSwitchVal(switchval<V> *array,int n,char c) {
    for(int i =0;i<n;++i)
        if(array[i].c==c)
            return make_pair(true,array[i].v);
    return make_pair(false,V());
}
template<typename V>
pair<bool,char*> findDescription(switchval<V> *array,int n,V v) {
    for(int i=0; i<n; ++i)
        if(array[i].v==v)
            return make_pair(true,array[i].desc);
    return make_pair(false,(char*)0);
}
void print_version() {
    reports[dgr::cmdline] << "Dynagraph version " << DYNAGRAPH_VERSION_DOTS_QUOTED << endl;
}
template<int i>
struct char1z3 {
    BOOST_MPL_ASSERT_RELATION(i,>=,0);
    BOOST_MPL_ASSERT_RELATION(i,<,10+26+26);
    enum {result = i<10 ? i+'0' : i<36 ? i-10+'A' : i-10-26+'a'};
};
char *DYNAGRAPH_V3RS10N() {
    static char buf[8];
    buf[0] = char1z3<Dg_MAJOR>::result;
    buf[2] = char1z3<Dg_MINOR>::result;
    buf[4] = char1z3<Dg_MINIC>::result;
    buf[6] = char1z3<Dg_MNEST>::result;
    buf[1] = buf[3] = buf[5] = '.';
    buf[7] = 0;
    return buf;
}
void print_v3rs10n() {
    reports[dgr::cmdline] << DYNAGRAPH_V3RS10N() << endl;
}
void print_help() {
    reports[dgr::cmdline] <<
        "dynagraph arguments:" << endl <<
        "   -v (--version) print version information only" << endl <<
        "   -h (-? --help) print this help" << endl <<
        "   -d use dot-compatible coordinates (position in points, node size in inches)" << endl <<
        "   -a attr=value set default graph attribute" << endl <<
        "   -i filename input .dot file (static layout)" << endl <<
        "   -s filename input .incr file (incrface dynamic layout)" << endl <<
        "   -oN filename write stream N to filename" << endl <<
        "   -ra{N} report on a to stream N, where N is [0-9] for -oN, + for stdout, or stderr if N unspecified" << endl;
    for(int i = 0;i<g_nreports;++i) reports[dgr::cmdline] <<
        "      " << g_reports[i].c << ' ' << g_reports[i].desc << endl;
    reports[dgr::cmdline] <<
        "   -w[r] N wait [randomly up to] N nanoseconds before processing each line" << endl <<
        "   -e specify random number seed (for wait time or fdp)" << endl <<
        "   -x break on any exception" << endl;
}
void print_report_syntax() {
    reports[dgr::error] <<
        "-o: specify output filename" << endl <<
        "   -oN filename, N is [0-9], a file # specified in -r" << endl;
}
void print_attribute_syntax() {
    reports[dgr::error] <<
        "-a: specify default graph attribute" << endl <<
        "   -a attr=value" << endl <<
        "   effectively attr=value is prepended onto every 'open graph' attribute list" << endl;
}

#ifdef DYNAGRAPH_WASM
EM_JS(char*, get_incrface_input, (), {
    if(window.incrface_input) {
        let ii = window.incrface_input + '\n@'; // explicit EOF to close parser
        window.incrface_input = null;
        return stringToNewUTF8(ii);
    }
    return null;
});
#endif

int main(int argc, char *args[]) {
    timer.Start();
    // enable basic dynagraph report streams
    reports.enable(dgr::error,&cerr);
    reports.enable(dgr::cmdline,&cout); // for help & anything that returns immediately
    loops.sep = ',';
    int random_seed = -1;
    typedef map<dgr::reportType,int> report_dest_map;
    report_dest_map reportDests;
    char *dotfile = 0;
    ostream *outfile[10];
    FILE *input_file = stdin;
    for(int i = 0;i<10;++i) outfile[i] = 0;
    for(int i = 1; i<argc; ++i) {
        if(args[i][0]!='-') {
            reports[dgr::error] << "not a valid command: " << args[i] << endl;
            return 1;
        }
        switch(args[i][1]) {
        case 'v':
            print_version();
            return 0;
        case 'h':
        case '?':
            print_version();
            print_help();
            return 0;
        case 'd': // dot-compatible coords
            g_transform  = &g_dotRatios;
            g_useDotDefaults = true;
            break;
        case 'a': // set default attribute
            if(i==argc-1 || args[i][2]) {
                print_attribute_syntax();
                return 1;
            }
            else {
                char *eq = strchr(args[++i],'=');
                DString attr,value;
                if(eq) {
                    attr.assign(args[i],eq-args[i]);
                    value.assign(eq+1);
                }
                else
                    attr.assign(args[i]);
                g_defaultGraphAttrs[attr] = value;
            }
            break;
        case 'i': // input dot file (for batch layout)
            if(i==argc-1) {
                reports[dgr::error] << "-i must be followed by filename" << endl;
                return 1;
            }
            if(args[i][2]) {
                reports[dgr::error] << "syntax: -i filename" << endl;
                return 1;
            }
            dotfile = args[++i];
            break;
        case 's': // dynamic script
            if(i==argc-1 || args[i][2]) {
                reports[dgr::error] <<
                    "-s: specify input incrface script" << endl <<
                    "   -s filename: use script in filename (instead of stdin)" << endl;
                return 1;
            }
            if(!(input_file = fopen(args[++i],"r"))) {
                reports[dgr::error] << "could not open input script " << args[i] << endl;
                return 1;
            }
            //setvbuf(input_file,0,_IONBF,0);
            break;
        case 'o': // output files
            if(i==argc-1 || !args[i][2]) {
                print_report_syntax();
                return 1;
            }
            else if(!isdigit(args[i][2])) {
                print_report_syntax();
                return 1;
            } else {
                int n = args[i][2]-'0';
                outfile[n] = new fstream(args[++i],fstream::out);
                if(outfile[n]->fail()) {
                    reports[dgr::error] << "-o error: couldn't open file " << args[i] << " for writing" << endl;
                    return 1;
                }
            }
            break;
        case 'r': {// reports
            char last = args[i][strlen(args[i])-1];
            int o;
            if(isdigit(last))
                o = last-'0';
            else if(last=='+')
                o = -1; // stdout
            else if(isalpha(last))
                o = -2;  // stderr
            else {
                reports[dgr::error] << "-r: generate report" << endl <<
                               "   report destination " << last << " not recognized, must be 0=9 or +" << endl;
                return 1;
            }
            for(int j = 2; args[i][j] && isalpha(args[i][j]); ++j) {
                pair<bool,dgr::reportType> val = findSwitchVal(g_reports,g_nreports,args[i][j]);
                if(!val.first) {
                    reports[dgr::error] << "-r: generate report " << endl <<
                                   "   report code '" << args[i][j] << "' not recognized" << endl;
                    return 1;
                }
                reportDests[val.second] = o;
            }
            break;
                  }
        case 'w':
            if(i==argc-1) {
                reports[dgr::error] << "-w wait time needs nanosecond arg" << endl;
                return 1;
            }
            if(args[i][2]=='r')
                g_randomizeWait = true;
            g_maxWait = atoi(args[++i]);
            break;
        case 'e':
            if(i==argc-1) {
                reports[dgr::error] << "-e random number sEed needs integer arg" << endl;
                return 1;
            }
            random_seed = atoi(args[++i]);
            break;
        case 'x':
            g_xeptFatal = true;
            break;
        case '-':
            if(!strcmp(args[i]+2,"version")) {
                print_version();
                return 0;
            }
            if(!strcmp(args[i]+2,"v3rs10n")) {
                print_v3rs10n();
                return 0;
            }
            else if(!strcmp(args[i]+2,"help")) {
                print_version();
                print_help();
                return 0;
            }
            //else fallthru
        default:
            reports[dgr::error] << "command not recognized: " << args[i] << endl;
            print_version();
            print_help();
            return 1;
        }
    }
    if(random_seed<0)
        random_seed = time(NULL);

    srand(random_seed);

#ifndef DYNAGRAPH_WASM
    setvbuf(stdin,0,_IONBF,0);
    setvbuf(stdout,0,_IONBF,0);
    setvbuf(stderr,0,_IONBF,0);
#endif

    // open report output files
    bool cout_was_grabbed=false;
    for(report_dest_map::iterator ri = reportDests.begin(); ri!=reportDests.end(); ++ri) {
        ostream *f;
        int dest_stream_id = ri->second;
        if(dest_stream_id==-1) {
            cout_was_grabbed = true;
            f = &cout;
        }
        else if(dest_stream_id==-2)
            f = &cerr;
        else {
            dgcheck(dest_stream_id>=0 && dest_stream_id<=9);
            f = outfile[dest_stream_id];
        }
        if(!f) {
            char c = '0' + dest_stream_id;
            char outfilename[120];
            if(dotfile && strlen(dotfile)<100)
                sprintf(outfilename,"%s.out.%c",dotfile,c);
            else
                sprintf(outfilename,"dynagraph.out.%c",c);
            reports[dgr::error] << "outfile " << c << " not specified; opening " << outfilename << endl;
            outfile[dest_stream_id] = new fstream(outfilename,fstream::out);
            if(outfile[dest_stream_id]->fail()) {
                reports[dgr::error] << "couldn't open " << outfilename << " for writing" << endl;
                return 1;
            }
        }
        reports.enable(ri->first,f);
    }

    // reporting may influence behavior..
    // recording raw input and inserting delays both require this thread here
    if(reports.enabled(dgr::input_raw) || g_maxWait>=0) {
        DuplicateIn *din = new DuplicateIn(input_file,reports[dgr::input_raw]);
        incr_yyin = din->getNewInput();
    }
    else
        incr_yyin = input_file; // else read direct from std::cin or -s file

    // send incrface to stdout if nothing else is going there
    if(!cout_was_grabbed) {
        // if incrface has output file, tee to cout
        if(reports.enabled(dgr::incrface)) {
            typedef boost::iostreams::tee_device<std::ostream,std::ostream> t_dev_t;
            typedef boost::iostreams::stream<t_dev_t> t_stream_t;
            t_dev_t *t_dev = new t_dev_t(reports[dgr::incrface],std::cout);
            t_stream_t *t_stream = new t_stream_t(*t_dev);
            reports.enable(dgr::incrface,t_stream);
        }
        else
            reports.enable(dgr::incrface, &cout);
    }

    // default transform
    if(!g_transform)
        g_transform = new Transform(Coord(1,1),Coord(1,1));
    while(1) {
        try {
#ifdef DYNAGRAPH_WASM
            char *incrface_input = get_incrface_input();
            if(incrface_input) {
                incr_yyin = fmemopen(incrface_input, strlen(incrface_input), "r");
                incr_yyparse();
            }
            emscripten_sleep(100);
#else
            incr_yyparse();
            break; // end of stream
#endif
        }
        catch(Assertion sert) {
            LOCK_REPORT(dgr::incrface);
            reports[dgr::incrface] << "message \"(exception) Assertion: " << sert.expr << "; " << sert.file << ", " << sert.line << '"' << endl;
            if(g_xeptFatal||sert.fatal)
                exit(23);
        }
        catch(DGException2 dgx) {
            LOCK_REPORT(dgr::incrface);
            reports[dgr::incrface] << "message \"(exception) " << dgx.exceptype << ": " << dgx.param << '"' << endl;
            if(g_xeptFatal||dgx.fatal)
                exit(23);
        }
        catch(DGException dgx) {
            LOCK_REPORT(dgr::incrface);
            reports[dgr::incrface] << "message \"(exception) " << dgx.exceptype << '"' << endl;
            if(g_xeptFatal||dgx.fatal)
                exit(23);
        }
        catch (const std::exception &exc) {
            reports[dgr::incrface] << "message \"(exception) std::exception " << exc.what() << "\"" << std::endl;
        }
        catch(...) {
            LOCK_REPORT(dgr::incrface);
            reports[dgr::incrface] << "message \"(exception) unknown exception\"" << endl;
            exit(23);
        }
    }
    incr_shutdown();
    reports[dgr::incrface] << "message \"dynagraph closing\"" << endl;
    return 0;
}
