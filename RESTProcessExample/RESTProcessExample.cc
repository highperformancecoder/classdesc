/*
  @copyright Russell Standish 2019
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "RESTProcessExample.h"
#include "classdesc_epilogue.h"
#include "RESTProcess_epilogue.h"

using namespace classdesc;
using namespace std;

#include <readline/readline.h>
#include <readline/history.h>

Root root;

const int Foo::csi;
int Foo::si;


int main()
{
  RESTProcess_t registry;
  RESTProcess(registry,"/root",root);

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
          cout << i.first << endl;
      else
        {
          try
            {
              json_pack_t jin(json_spirit::mValue::null);
              string t;
              getline(cin,t);
              if (!t.empty())
                read(t,jin);
              write(registry.process(cmd, jin),cout);
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
