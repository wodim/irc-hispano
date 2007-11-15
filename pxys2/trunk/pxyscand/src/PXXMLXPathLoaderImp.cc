/* Copyright (C) 2003 Stephane Thiell
 *
 * This file is part of pxyscand (from pxys)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */
#define RCSID "$Id: PXXMLXPathLoaderImp.cc,v 1.3 2005/05/18 22:31:30 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "PXXMLXPathLoaderImp.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

PXXMLXPathLoaderImp::PXXMLXPathLoaderImp(const char *inFile)
  {
  mDoc = xmlParseFile(inFile);
  if (mDoc == NULL)
    PXXMLException::Throw("XML document parsing failed");
  
  mCx = xmlXPathNewContext(mDoc);
  }

PXXMLXPathLoaderImp::~PXXMLXPathLoaderImp()
  {
  xmlXPathFreeContext(mCx);
  }

xmlXPathObjectPtr
PXXMLXPathLoaderImp::Eval(const char *xpath)
  {
  xmlXPathObjectPtr result;
  result = xmlXPathEvalExpression((xmlChar *)xpath, mCx);
  
  if (result && xmlXPathNodeSetIsEmpty(result->nodesetval))
    {
    xmlXPathFreeObject(result);
    return NULL;
    }
  return result;
  }

xmlXPathObjectPtr
PXXMLXPathLoaderImp::EvalUnique(const char *xpath)
  {
  xmlXPathObjectPtr o = this->Eval(xpath);
  
  if (!o || !o->nodesetval->nodeNr)
    PXXMLException::Throw("Missing parameter", xpath);
  if (o->nodesetval->nodeNr > 1)
    PXXMLException::Throw("Duplicate parameter", xpath);
  
  return o;
  }

void
PXXMLXPathLoaderImp::GetAddress(const char *xpath, int idx, int mincnt,
                                const char *debugpath, struct in_addr *outAddr)
  {
  xmlChar *str;
  
  if (!(str = CopyString(xpath, idx, mincnt)))
    outAddr->s_addr = INADDR_ANY;
  else
    {
    int res = inet_pton(AF_INET, (char *)str, outAddr);
    xmlFree(str);
    if (res == -1)
      PXXMLException::Throw("Bad IP address", debugpath);
    }
  }

int
PXXMLXPathLoaderImp::GetInteger(const char *xpath, int idx, int mincnt)
  {
  int result;
  xmlChar *s;

  if ((s = this->CopyString(xpath, idx, mincnt)) != NULL)
    {
    result = atoi((char *)s);
    xmlFree(s);
    }
  else
    result = 0;
  return result;
  }

xmlChar *
PXXMLXPathLoaderImp::CopyString(const char *xpath, int idx, int mincnt)
  {
  xmlXPathObjectPtr result;
  xmlNodePtr node;
  xmlChar *s;
  
  result = this->Eval(xpath);
  if (!result || !result->nodesetval->nodeNr)
    {
    if (result)
      xmlXPathFreeObject(result);
    if (mincnt > 0)
      PXXMLException::Throw("Missing parameter", xpath);
    return NULL;
    }
  if (idx == 0 && mincnt > 0 && result->nodesetval->nodeNr > mincnt)
    PXXMLException::Throw("Duplicate parameter", xpath);
  
  if (!(node = result->nodesetval->nodeTab[idx]->xmlChildrenNode))
    PXXMLException::Throw("Undefined/Empty value", xpath);

  s = xmlNodeListGetString(mDoc, node, 1);
  xmlXPathFreeObject(result);
  if (!s && mincnt > 0)
    PXXMLException::Throw("Empty parameter", xpath);
  return s;
  }

