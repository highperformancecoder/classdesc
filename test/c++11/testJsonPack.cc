#include <json_pack_base.h>
#include <classdesc_epilogue.h>
#include <vector>
#include <string>
#include <assert.h>
using namespace std;
using namespace classdesc;

int main()
{
  json_pack_t j{"a","b","c"};
  vector<string> s1{"a","b","c"}, s2;
  j>>s2;
  assert(s1==s2);
}
