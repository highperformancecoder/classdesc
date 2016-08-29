/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef WRITEOBJC_BASE_H
#define WRITEOBJC_BASE_H
#include "ObjCsupport.h"
#include "../eco_string.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include <stdio.h>

//#include "classdesc_access.h"

#include <string>
#include <algorithm>

using namespace std;

int endVarDef  = 0;          // true whn starts parsing function
int objcClass  = 0;          // true when C++ class is derived from objc_obj
int errObjCMsg = 0;          // print out class error msg once only

/* This allows the user to replace the stdlib realloc routine by redefining
   the Realloc macro */
#ifndef Realloc
#define Realloc realloc
#endif

#ifdef XDR_WRITEOBJC
#if (defined(_AIX) || defined(__osf__))
typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;
#endif
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif

#define STRING(x) #x

#define INCL #include

#define FILE_NAME(x,y)\
FNAME(x,y)

#define FNAME(head,ext)\
   STRING(head##ext)

#define WRITE_OBJC_INCL_h(classname) \
INCL FNAME(classname,.h)

#define WRITE_OBJC_INCL_cd(classname) \
INCL FNAME(classname,.cd)

#define WRITE_OBJC(classname) \
writeobjc_headers(&targ, (eco_string) STRING(classname)); \
writeobjcp(&targ, (eco_string) STRING(classname), arg);\
writeobjc_tail(&targ)

class writeobjc_t
{
  writeobjc_t operator=(writeobjc_t& x){return *this;}
  writeobjc_t(writeobjc_t& x){}
  template <class T>
    void lappend(const T& x)
    {
      data=(char*)Realloc(data,size+sizeof(x));
      memcpy(data+size,&x,sizeof(x));
      size+=sizeof(x);
    }
  template <class T>
    void lpopoff(T& x)
    {
      memcpy(&x,data+pos,sizeof(x));
      pos+=sizeof(x);
    }
public:

  ofstream out_mh, out_m, out_cc;
  char     *data;
  int      size;
  int      pos;
  int      xdr;
  string   className;

#ifdef RUNTIME_LINK_ERROR
  template <class T>
  void append(T& x) {lappend(x);}
  template <class T>
  void popoff(T& x) {lpopoff(x);}
#endif

  inline void init(eco_string cname)
  { className = cname.c_str();
    out_mh.open(cname + ".mh");
    out_m.open(cname + ".m");
    out_cc.open(cname + "ExportCpp.cc");
  }

  inline void write_mh(eco_string cname)
  { out_mh << cname << endl;  }
  inline void write_m(eco_string cname)
  { out_m << cname << endl;  }
  inline void write_cc(eco_string cname)
  { out_cc << cname << endl;  }

//  inline void write_mh(string cname)
//  { out_mh << (eco_string) cname.c_str() << endl;  }
//  inline void write_m(string cname)
//  { out_m << (eco_string) cname.c_str() << endl;  }
//  inline void write_cc(string cname)
//  { out_cc << (eco_string) cname.c_str() << endl;  }

  inline void removefiles()
  { string str;
    out_mh.close();
    out_m.close();
    out_cc.close();
    str = className + ".mh";       remove(str.c_str());
    str = className + ".m";        remove(str.c_str());
    str = className + "Export.cc"; remove(str.c_str());
  }

  inline void end()
  { out_mh.close();
    out_m.close();
    out_cc.close();
  }

  inline void translateType(eco_string type, eco_string cname, eco_string vname, eco_string dim)
  {
     eco_string str;
     cout << "\"" << vname << "\" of-type \"" << type << "\"\n";
     str = (eco_string) "      " + type + " " + vname + dim + ";";
     write_mh(str);
  }

  inline void lappend_mh(eco_string str)
  { out_mh << str.c_str(); }
  inline void lappend_m(eco_string str)
  { out_m << str.c_str(); }
  inline void lappend_cc(eco_string str)
  { out_cc << str.c_str(); }

  inline void append(eco_string cname, eco_string vname, const bool& x, const char * dim)
  { translateType( (eco_string) "bool", cname, vname, dim); }

  inline void append(eco_string cname, eco_string vname, const char& x, const char * dim)
  { translateType( (eco_string) "char", cname, vname, dim); }

  inline void append(eco_string cname, eco_string vname, const signed char& x, const char * dim)
  { translateType( (eco_string) "signed char", cname, vname, dim); }

  inline void append(eco_string cname, eco_string vname, const unsigned char& x, const char * dim)
  { translateType( (eco_string) "unsigned char", cname, vname, dim); }

  inline void append(eco_string cname, eco_string vname, const wchar_t& x, const char * dim)
  { translateType( (eco_string) "wchar_t", cname, vname, dim); }

  inline void append(eco_string cname, eco_string vname, const int& x, const char * dim)
  {  translateType( (eco_string) "int", cname, vname, dim); }

  inline void append(eco_string cname, eco_string vname, const unsigned int& x, const char * dim)
  { translateType( (eco_string) "unsigned int", cname, vname, dim); }

  inline void append(eco_string cname, eco_string vname, const short int& x, const char * dim)
  { translateType( (eco_string) "short int", cname, vname, dim); }

  inline void append(eco_string cname, eco_string vname, const unsigned short int& x, const char * dim)
  { translateType( (eco_string) "unsigned short int", cname, vname, dim); }

  inline void append(eco_string cname, eco_string vname, const long int& x, const char * dim)
  { translateType( (eco_string) "long int", cname, vname, dim); }

  inline void append(eco_string cname, eco_string vname, const unsigned long int& x, const char * dim)
  { translateType( (eco_string) "unsigned long int", cname, vname, dim); }

  inline void append(eco_string cname, eco_string vname, const float& x, const char * dim)
  { translateType( (eco_string) "float", cname, vname, dim); }

  inline void append(eco_string cname, eco_string vname, const double& x, const char * dim)
  { translateType( (eco_string) "double", cname, vname, dim); }

  inline void append(eco_string cname, eco_string vname,const long double& x, const char * dim)
  { translateType( (eco_string) "long double", cname, vname, dim); }

  void popoff(bool& x) {lpopoff(x);}
  void popoff(char& x) {lpopoff(x);}
  void popoff(signed char& x) {lpopoff(x);}
  void popoff(unsigned char& x) {lpopoff(x);}
  void popoff(wchar_t& x) {lpopoff(x);}
  void popoff(int& x) {lpopoff(x);}
  void popoff(unsigned int& x) {lpopoff(x);}
  void popoff(short int& x) {lpopoff(x);}
  void popoff(unsigned short int& x) {lpopoff(x);}
  void popoff(long int& x) {lpopoff(x);}
  void popoff(unsigned long int& x) {lpopoff(x);}
  void popoff(float& x) {lpopoff(x);}
  void popoff(double& x) {lpopoff(x);}
  void popoff(long double& x) {lpopoff(x);}
  /* we must make const objects mutable again!! */
  template <class T> void popoff(const T& x) {popoff(static_cast<T>(x));}

  template <class T>
  void writeobjc(T& x) {::writeobjc(this,eco_string(),x);}


  template <class T>
  writeobjc_t& operator<<(const T&x)
  {::writeobjc(this,eco_string(),const_cast<T&>(x)); return *this;}

//  template <class T>
//  pack_t& operator>>(T&x) {::unpack(this,eco_string(),x); return *this;}

  writeobjc_t(int sz=0) {data=(char*)realloc(NULL,sz); size=sz; pos=0; xdr=0;}

  ~writeobjc_t() {free(data);}

  class notimplemented{};
};

// Objective-C/C++ codes generation
//
void writeobjc_headers(writeobjc_t* targ,eco_string cname)
{
  eco_string str;
  cout << "\nC++ to ObjC parsing ...\n";
  targ->init(cname);
  targ->write_mh("#import <objc/Object.h>\n");
  str = (eco_string) "@interface " + cname + " : Object\n" + "{  @public";
  targ->write_mh(str);

  str = (eco_string) "#import \"" + cname + ".mh\"";
  targ->write_m(str);
  str = (eco_string) "#import <stdarg.h>\n";
  targ->write_m(str);
  str = (eco_string) "@implementation " + cname + "\n";
  targ->write_m(str);
  str = (eco_string) "- init\n{ [super init];\n  return self;\n}\n";
  targ->write_m(str);

  str = (eco_string) "#include \"" + cname + ".h\"";
  targ->write_cc(str);
  str = (eco_string) "#include <stdarg.h>\n";
  targ->write_cc(str);
}

void writeobjc_tail(writeobjc_t* targ)
{
   cout << "End translation.\n\n";
   if (objcClass)
   {
      targ->write_mh("@end\n");
      targ->write_m("@end\n");
      targ->end();
   }
   else
   {  targ->removefiles();
   }
}

// typedef pack_t unpack_t;

#ifdef XDR_WRITEOBJC
const int BUFCHUNK=1024;

class xdr_writeobjc: public writeobjc_t
{
  int asize;
public:
  XDR *input, *output;
  xdr_writeobjc(int sz=BUFCHUNK): writeobjc_t(sz), asize(0)
  {
    xdr=1;
    input=new XDR; output=new XDR;
    xdrmem_create(input,(const caddr_t)data,sz,XDR_ENCODE);
    xdrmem_create(output,(const caddr_t)data,sz,XDR_DECODE);
  }
  ~xdr_writeobjc() {xdr_destroy(input); xdr_destroy(output);
               delete input;  delete output;}
  template <class T>
    void append(T& x)
    {
      if (BUFCHUNK-xdr_getpos(input)<8)
	{
	  asize += xdr_getpos(input);
	  data = (char*) Realloc(data,size+BUFCHUNK);
	  if (data==NULL) throw runtime_error("Error encoding XDR stream");
	  xdr_destroy(input);
	  xdrmem_create(input,(const caddr_t)data+size,BUFCHUNK,XDR_ENCODE);
	}
      if (!proc(input,x)) throw runtime_error("Error encoding XDR stream");
      size=asize+xdr_getpos(input);
    }
  template <class T>
    void popoff(T& x)
    { if (!proc(output,x)) throw runtime_error("Error decoding XDR stream");}
  int proc(XDR* xdr, char& arg) {return xdr_char(xdr,&arg);}
  int proc(XDR* xdr, double& arg) {return xdr_double(xdr,&arg);}
  int proc(XDR* xdr, float& arg) {return xdr_float(xdr,&arg);}
  int proc(XDR* xdr, int& arg) {return xdr_int(xdr,&arg);}
  int proc(XDR* xdr, bool& arg)
    {char c=arg; int r=xdr_char(xdr,&c); arg=c; return r;}
  int proc(XDR* xdr, long& arg) {return xdr_long(xdr,&arg);}
  int proc(XDR* xdr, short& arg) {return xdr_short(xdr,&arg);}
  int proc(XDR* xdr, unsigned char& arg) {return xdr_u_char(xdr,&arg);}
  int proc(XDR* xdr, unsigned int& arg) {return xdr_u_int(xdr,&arg);}
  int proc(XDR* xdr, unsigned long& arg) {return xdr_u_long(xdr,&arg);}
  int proc(XDR* xdr, unsigned short& arg) {return xdr_u_short(xdr,&arg);}
  /* anything not implemented */
#ifdef RUNTIME_LINK_ERROR
  template <class T> int proc(XDR* xdr, T& arg) {return 0;}
#endif
};
#else
typedef writeobjc_t xdr_writeobjc;
#endif

/* default action is to assume a simple type */
/* xdr flag and typecasting required because virtual template member
   functions impossible :) */

// Simple data type decoding
template <class T>
void writeobjc(writeobjc_t *targ, eco_string desc, T& arg, const char * dim = "", const int iDim = 0)
{
   if (objcClass)
   {
      cout << "Translating simple data type: ";
      char  cn[50];  cn[0]  = '\0';
      char  vn[50];  vn[0]  = '\0';
      char  str[50]; str[0] = '\0';
      int   n;
      char  *cp;
      strcpy(str, desc.c_str());
      cp = strchr(str,'.');
      n = strlen(str)-strlen(cp);
      strncpy(cn, str, n);
      cn[n] = '\0';
      cp++;
      strcpy(vn, cp);
      eco_string cname = cn;
      eco_string vname = vn;
      eco_string esStr = dim;
      targ->append(cname,vname,arg,dim);
   }
   else prtObjcClassErr(targ->className);
}

/* allow for packing of temporary objects */
template <class T>
void writeobjc(writeobjc_t *targ, eco_string desc, const T& arg)
{  cout << "main write routine2 ...\n";
   writeobjc(targ,desc,const_cast<T&>(arg));
}

/*
template <class T>
void unpack(unpack_t *targ, eco_string desc, T& arg)
{
  if (targ->xdr)
    ((xdr_pack*)targ)->popoff(arg);
  else
    targ->popoff(arg);
}
*/

/* default action for pointers is to throw an error message */
template <class T>
void writeobjc(writeobjc_t *targ, eco_string desc, T*& arg)
{
   char  cn[50];  cn[0]  = '\0';
   char  vn[50];  vn[0]  = '\0';
   char  str[50]; str[0] = '\0';
   int   n;
   char  *cp;
   strcpy(str, desc.c_str());
   cp = strchr(str,'.');
   n = strlen(str)-strlen(cp);
   strncpy(cn, str, n);
   cn[n] = '\0';
   cp++;
   strcpy(vn, cp);
   eco_string cname = cn;
   eco_string vname = vn;

   if (strcmp(vname.c_str(),"isa")==0)
      cout << "Translating pointer-type: \"" << vname << "\" - no translation.\n";
   else
      throw runtime_error("Packing arbitrary pointer data not implemented");
}

//template <class T>
//void unpack(unpack_t *targ, eco_string desc, T*& arg)
//{throw runtime_error("Unpacking arbitrary pointer data not implemented");}

/* The problem is that function pointers this template also, so we
   need a separate pack to packup pointers to single objects:

   pack a flag indicating validity, and then the object pointer points
   to. Cannot handle arrays, but useful for graphs - assumes pointer is
   valid unless NULL.

   */

#ifndef IS_NODE
#define IS_NODE
class is_treenode {};
class is_graphnode {};
#endif

template <class T>
void writeobjc(writeobjc_t *targ, eco_string desc, is_treenode dum, T*& arg)
{
  int valid=0;
  if (arg==NULL)
    writeobjc(targ,desc,valid);
  else
    {
      valid=1;
      writeobjc(targ,desc,valid);
      writeobjc(targ,desc,*arg);
    }
}

/*
template <class T>
void unpack(unpack_t *targ, eco_string desc, is_treenode dum, T*& arg)
{
  int valid;
  unpack(targ,desc,valid);
  if (valid)
    {
      arg=new T;
      unpack(targ,desc,*arg);
    }
}
*/

#if 0
/* its all very well packing char*'s, but how do we unpack them? */
/* but char * are assumed to be nul terminated strings */
void writeobjc(writeobjc_t *targ, eco_string desc,  char *arg)
{targ->append(arg,strlen(arg));}

//void unpack(unpack_t *targ, eco_string desc,  char *arg)
//{targ->append(arg,strlen(arg));}
#endif

/* now define the array version  */
#ifndef IS_ARRAY
#define IS_ARRAY
class is_array {};
#endif

template <class T>
void writeobjc(writeobjc_t *targ, eco_string desc, is_array ia, T &arg,const char * sDim)
{
   if (objcClass)
   {
      char  cn[50];  cn[0]  = '\0';
      char  vn[50];  vn[0]  = '\0';
      char  str[50]; str[0] = '\0';
      int   n;
      char  *cp;
      strcpy(str, desc.c_str());
      cp = strchr(str,'.');
      n = strlen(str)-strlen(cp);
      strncpy(cn, str, n);
      cn[n] = '\0';
      cp++;
      strcpy(vn, cp);
      eco_string cname = cn;
      eco_string vname = vn;
      eco_string esDim = sDim;

      cout << "Translating array: \"" << vname << sDim << "\" - ";
      writeobjc(targ,desc,arg,esDim,0);      // 0 to prevent call to function overloading
   }
   else prtObjcClassErr(targ->className);
}


// routines for argument parsing

string TrimL(string s)
{  if (s.empty()) return s;
   string::size_type i = s.find_first_not_of(" ");
   if (i==string::npos) return string("");
   return s.substr(i); }
string TrimR(string s)
{  if (s.empty()) return s;
   string::size_type i = s.find_last_not_of(" ");
   return s.substr(0,i+1); }
string Trim(string s)  { string s1 = TrimL(s); return TrimR(s1); }
char toupperChar(char c)
{ return (char) toupper((int)c); }
string toCapStr(string s)
{ string S=s; transform(S.begin(), S.end(), S.begin(), toupperChar); return S; }
int CountArg(string s) { string s1 = Trim(s); if (s1.empty()) return 0; return  count(s.begin(), s.end(), ',') + 1; }
string getHeadArg(string s) { string::size_type i = s.find_first_of(","); return Trim(s.substr(0,i)); }
string getTailArg(string s) { string::size_type i = s.find_first_of(","); return Trim(s.substr(i+1)); }
string getArgType(string s) { string::size_type i = s.find_last_of(" &*:"); return Trim(s.substr(0,i)); }
string getArgName(string s) { string::size_type i = s.find_last_of(" &*:"); return Trim(s.substr(i)); }

string stripArgType(string s)
{
   int i;
   int n = CountArg(s);
   string argType, argName, headList, str;
   string tailList = s;
   for (i=1; i<=n; i++)
   {
      headList = getHeadArg(tailList);
      tailList = getTailArg(tailList);
      argType  = getArgType(headList);
      argName  = getArgName(headList);
      str += argName; if (i != n) str += ", ";
   }
   return str;
}
string convertArgObjc(string s)
{  int i;
   int n = CountArg(s);
   string argType, argName, headList, str;
   string tailList = Trim(s);
   if (n!=0)
   {  headList = getHeadArg(tailList);
      tailList = getTailArg(tailList);
      argType  = getArgType(headList);
      argName  = getArgName(headList);
      str += "(" + argType + ") " + argName; if (n > 1) str += " ";
      for (i=2; i<=n; i++)
      {  headList = getHeadArg(tailList);
         tailList = getTailArg(tailList);
         argType  = getArgType(headList);
         argName  = getArgName(headList);
         str += argName + ": (" + argType + ") " + argName; if (i != n) str += " ";
      }
   }
   return str;
}

void writeobjcArgN(writeobjc_t *targ, string rtype, string cname, string vname, string alist)
{  string str;
   int n = CountArg(alist);
   string s1 = stripArgType(alist);    // arg list without type
   string s2 = convertArgObjc(alist);  // objc equivalent arg list

   str = "- ";
   if (rtype != "void") str = str + "(" + rtype + ") ";
   str += vname;
   if (n==0) str += ";"; else str += ": " + s2 + ";";
   targ->write_mh(str.c_str());

   str = rtype + " cpp_" + cname + "_" + vname + "(";
   str += cname + " * obj";
   if (n==0) str += ");"; else str += ", " + alist + ");";
   targ->write_m(str.c_str());
   str = "- ";
   if (rtype != "void") str += "(" + rtype + ") ";
   str += vname;
   if (n==0) str += "\n{"; else str += ": " + s2 + "\n{";
   if (rtype != "void") str += " return "; else str += " ";
   str += "cpp_" + cname + "_" + vname + "(self";
   if (n==0) str += ");"; else str += ", " + s1 + ");";
   if (rtype == "void") str += "  return self;";
   str += " }\n";
   targ->write_m(str.c_str());

   str = "extern \"C\" " + rtype + " cpp_" + cname + "_" + vname + "(" + cname + " * obj";
   if (n>0) str += ", ";
   str += alist + ")\n{";
   if (rtype != "void") str += " return "; else str += " ";
   str += "obj->" + vname + "(" + s1 + "); }\n";
   targ->write_cc(str.c_str());
}

void writeobjcArg2VL(writeobjc_t *targ, string rtype, string cname, string vname,
                   string arg1Type, string arg1Name, string arg2Type, string arg2Name)
{  string str;
   str = "- ";
   if (rtype != "void") str = str + "(" + rtype + ") ";
   str += vname + ": (" + arg1Type + ") " + arg1Name + ", ...;";
   targ->write_mh(str.c_str());

   str = rtype + " cpp_" + cname + "_" + vname + "(";
   str += cname + " * obj, " + arg1Type + " " + arg1Name + ", va_list * ap);";
   targ->write_m(str.c_str());
   str = "- ";
   if (rtype != "void") str = str + "(" + rtype + ") ";
   str += vname + ": (" + arg1Type + ") " + arg1Name + ", ...\n";
   str += "{ ";
   if (rtype != "void") str += rtype + " rtnvalue;\n";
   str += "  va_list ap;\n  va_start(ap, " + arg1Name + ");\n";
   if (rtype != "void") str += "  rtnvalue = "; else str += "  ";
   str += "cpp_" + cname + "_" + vname + "(self, " + arg1Name + ", &ap);\n";
   str += "  va_end(ap);\n";
   if (rtype != "void") str += "  return rtnvalue;\n";
   str += "}\n";
   targ->write_m(str.c_str());

   str = "extern \"C\" " + rtype + " cpp_" + cname + "_" + vname + "(" + cname + " * obj, ";
   str += arg1Type + " " + arg1Name + ", va_list * ap)\n";
   str += "{ objc_t buffer;\n  buffer.ap = ap;\n";
   if (rtype != "void") str += "  return "; else str += "  ";
   str += "obj->" + vname + "(" + arg1Name + ", buffer);\n}\n";
   targ->write_cc(str.c_str());
}

//template <class T>
//void unpack(unpack_t *targ, eco_string desc, is_array ia, T *arg,  int ncopies)
//{for (int i=0; i<ncopies; i++) unpack(targ,desc,arg[i]);}

/* what to do about member functions */
//
// need to pass-in
// - return type
// - nbr of inputs
//
template<class C, class T>
void writeobjc(writeobjc_t *targ, eco_string desc, C& c, T arg, char *rtntype, char *arglist)
{
   char  cn[50];   cn[0]  = '\0';
   char  vn[50];   vn[0]  = '\0';
   char  cstr[50]; cstr[0] = '\0';
   int   n;
   char  *cp;
   strcpy(cstr, desc.c_str());
   cp = strchr(cstr,'.');
   n = strlen(cstr)-strlen(cp);
   strncpy(cn, cstr, n);
   cn[n] = '\0';
   cp++;
   strcpy(vn, cp);

   string cname  = cn;
   string vname  = vn;
   string rtype  = rtntype;
   string alist  = arglist;
   int    totArg = CountArg(alist);
   string arg1, arg2, arg1Type, arg1Name, arg2Type, arg2Name, s1, s2, str;
   string fname  = toCapStr(vname);

   alist = Trim(alist);
   rtype = Trim(rtype);
   if (objcClass)
   {
      cout << "Translating function: " << rtntype << " " << vname << "(" << arglist << ")\n";
      if ( (rtype.find_last_of("<>;(") != string::npos) ||
           (alist.find_last_of("<>;(") != string::npos) ||
           (fname.find("CPP_") != string::npos)
         )
      {  cerr << "** warning: C++ function detected, no ObjC translation.\n\n"; }
      else
      {
         if (!endVarDef)
         {  endVarDef = 1; str = "}\n- init;"; targ->write_mh(str.c_str()); }
         if (totArg==2)
         {  arg1 = getHeadArg(alist); arg1Type = getArgType(arg1); arg1Name = getArgName(arg1);
            arg2 = getTailArg(alist); arg2Type = getArgType(arg2); arg2Name = getArgName(arg2);
            if (arg2Type.find("objc_t") != string::npos)    // n args using va_list
            {  writeobjcArg2VL(targ, rtype, cname, vname, arg1Type, arg1Name, arg2Type, arg2Name); }
            else                                            // 2 args only
            {  writeobjcArgN(targ, rtype, cname, vname, alist); }
         }
         else
         {  writeobjcArgN(targ, rtype, cname, vname, alist); }
      }
   }
   else  prtObjcClassErr(targ->className);
}

void prtObjcClassErr(string c)
{  if (errObjCMsg==0)
   { errObjCMsg = 1;
     cout << "Error: " << c << " class did not derived from objc_obj.\n";
   }
}

void writeobjc(writeobjc_t *targ, eco_string desc, objc_obj& objcobj)
{  objcClass = 1;
   cout << "Starts parsing C++ class to ObjC ...\n";
}

//template<class C, class T>
//void unpack(unpack_t *targ, eco_string desc, C& c,  T arg) {}

#ifndef IS_STATIC
#define IS_STATIC
class is_static {};
#endif

inline void writeobjc(writeobjc_t *targ, eco_string desc, is_static s)
{
#ifdef STATIC_DBG
  cerr << "Static member "<<desc<<" not packed" << endl;
#endif
}

/*
inline void unpack(unpack_t *targ, eco_string desc, is_static s)
{
#ifdef STATIC_DBG
  cerr << "Static member "<<desc<<" not unpacked" << endl;
#endif
}
*/

#pragma omit writeobjc eco_string
#pragma omit writeobjc eco_strstream
#pragma omit writeobjc xdr_writeobjc
//#pragma omit unpack eco_string
//#pragma omit unpack eco_strstream
//#pragma omit unpack xdr_pack
#endif
