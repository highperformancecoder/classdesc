/*
  @copyright Russell Standish 2019
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "json_pack_base.h"
#include "RESTProcessExample.h"
#include "classdesc_epilogue.h"
#include "RESTProcess_epilogue.h"

using namespace classdesc;
using namespace std;
using namespace json5_parser;

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
              json_pack_t jin;
              string t;
              getline(cin,t);
              if (!t.empty())
                read(t,jin);
              cout << toREST(cmd) <<"=>";
              auto result=registry.process(cmd, jin);
              write(result->asBuffer(),cout,json5_parser::remove_trailing_zeros);
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
