/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

/*
  This file includes the relevant epilogue files for any descriptors
  used so far
*/
/** \file
\brief Epilogue header for Classdesc

This file needs to be included after all .cd files have been processed
*/ 

#ifndef CLASSDESC_EPILOGUE_H
#define CLASSDESC_EPILOGUE_H
// satisfy linkage requirements for classdesc_epilogue_not_included check
namespace
{
  int classdesc_epilogue_not_included() {return 1;}
}

#include "typeName_epilogue.h"

#ifdef CLASSDESC_PACK_BASE_H
#include "pack_epilogue.h"
#endif

#ifdef CLASSDESC_XML_COMMON_H
#include "xml_pack_epilogue.h"
#endif

#ifdef CLASSDESC_XSD_GENERATE_BASE_H
#include "xsd_generate-allCDs.h"
#endif

#ifdef CLASSDESC_DUMP_BASE_H
#include "dump_epilogue.h"
#endif

#ifdef CLASSDESC_JAVACLASS_BASE_H
#include "javaClass_epilogue.h"
#endif

#ifdef CLASSDESC_JSON_PACK_BASE_H
#include "json_pack_epilogue.h"
#endif

#ifdef CLASSDESC_RANDOM_INIT_BASE_H
#include "random_init_epilogue.h"
#endif

#ifdef CLASSDESC_PYTHON_BASE_H
#include "python_epilogue.h"
#endif

#ifdef CLASSDESC_RESTPROCESS_BASE_H
#include "RESTProcess_epilogue.h"
#endif

#endif
