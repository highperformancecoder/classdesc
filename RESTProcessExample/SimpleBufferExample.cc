/*
  @copyright Russell Standish 2019
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

// replace the default json_pack_t object with another implementation of the buffer concept
#include "SimpleBuffer.h"
#define REST_PROCESS_BUFFER SimpleBuffer

#include "RESTProcessExample.h"
#include "classdesc_epilogue.h"

using namespace classdesc;
using namespace std;

#include <readline/readline.h>
#include <readline/history.h>

Root root;

const int Foo::csi;
int Foo::si;

string toREST(string x)
{
  replace(x.begin(),x.end(),'.','/');
  return '/'+x;
}

int main()
{
  RESTProcess_t registry;
  RESTProcess(registry,"root",root);

  char* c;
  string cmd;
  
  //  while ((c=readline("cmd>"))!=nullptr)
  while (getline(cin,cmd))
    {
      //      string cmd=c;
      //cout << cmd << endl;
      if (cmd[0]=='#') continue; //ignore comments
      if (cmd[0]!='/')
        cerr << cmd << "command doesn't starts with /"<<endl;
      else if (cmd=="/list")
        for (auto& i: registry)
          cout << toREST(i.first) << endl;
      else
        {
          // trim any trailing whitespace
          int i=cmd.length()-1;
          for (; i>=0 && isspace(cmd[i]); --i);
          cmd=cmd.substr(1,i);
          replace(cmd.begin(),cmd.end(),'/','.');
          try
            {
              json_pack_t jin(json5_parser::mValue::null);
              string t;
              getline(cin,t);
              if (!t.empty())
                read(t,jin);
              cout << toREST(cmd) <<"=>";
              SimpleBuffer args(jin);
              auto ret=boost::apply_visitor(ToJsonVisitor(), registry.process(cmd, args)->asBuffer());
              write(ret,cout,json5_parser::remove_trailing_zeros);
              cout << endl;
            }
          catch (const std::exception& ex)
            {
              cerr << "Exception: "<<ex.what() << endl;
            }
        }
//      if (strlen(c)) add_history(c); 
//      free(c);
    }
}
