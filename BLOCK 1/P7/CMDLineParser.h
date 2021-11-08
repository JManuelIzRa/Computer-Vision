#ifndef _CMDLineParser_
#define _CMDLineParser_

#include <string>

using std::string;

class CmdLineParser{
   int argc;
   char **argv;
public:
  CmdLineParser(int _argc,char **_argv):argc(_argc),argv(_argv){}
  bool operator[] ( string param ) {int idx=-1; for ( int i=0; i<argc && idx==-1; i++ ) if ( string ( argv[i] ) ==param ) idx=i; return ( idx!=-1 ) ; }
  string operator()(string param,string defvalue="-1"){int idx=-1; for ( int i=0; i<argc && idx==-1; i++ ) if ( string ( argv[i] ) ==param ) idx=i; if ( idx==-1 ) return defvalue; else return ( argv[ idx+1] ); }
};

#endif