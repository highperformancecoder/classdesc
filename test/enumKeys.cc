/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "enumKeys.h"
#include "classdesc_epilogue.h"
#include <vector>
#include <iostream>
using namespace std;
using namespace classdesc;

int main()
{
  vector<string> label(enum_keys<foo>().sbegin(), enum_keys<foo>().send());
  assert(label.size()==3);
  for (size_t i=0; i<label.size(); ++i) cout << label[i]<<endl;
  vector<foo> vals(enum_keys<foo>().vbegin(), enum_keys<foo>().vend());
  assert(vals.size()==3);
}
