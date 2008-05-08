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
#ifndef INCLUDED_PXXMLXPATHLOADERIMP_H_
#define INCLUDED_PXXMLXPATHLOADERIMP_H_

#include <exception>
#include <libxml/parser.h>
#include <libxml/xpath.h>

class PXXMLException : public std::exception
  {
public:
  PXXMLException(const char *inError, const char *inHint = 0) throw()
    : mError(inError), mHint(inHint) {}
  virtual ~PXXMLException() throw() {}
  
  const char *GetErrorString() const { return mError; }
  const char *GetHint() const { return mHint ? mHint : "n/a"; }
  static void Throw(const char *inError, const char *inHint = 0)
    { throw PXXMLException(inError, inHint); }

protected:
  const char *mError, *mHint;
  };

class StPXXPathContextNode
  {
  xmlNodePtr mNodeSave;
  xmlNodePtr * mNodeHandle;
  
public:
  StPXXPathContextNode(xmlNodePtr *inNodeSave, xmlNodePtr inNewNode)
    : mNodeHandle(inNodeSave)
    {
    mNodeSave = *mNodeHandle;
    *inNodeSave = inNewNode;
    }
  ~StPXXPathContextNode()
    {
    *mNodeHandle = mNodeSave;
    }
  };

class PXXMLXPathLoaderImp
  {
public:
  PXXMLXPathLoaderImp(const char *inFile);
  virtual ~PXXMLXPathLoaderImp();
  
  virtual xmlXPathObjectPtr Eval(const char *xpath);
  virtual xmlXPathObjectPtr EvalUnique(const char *xpath);
  
  virtual void GetAddress(const char *xpath, int idx, int mincnt,
                          const char *debugpath, struct in_addr *outAddr);
  virtual int GetInteger(const char *xpath, int idx, int mincnt);
  virtual xmlChar * CopyString(const char *xpath, int idx, int mincnt);

protected:
  xmlDocPtr mDoc;
  xmlXPathContextPtr mCx;
  };

#endif /* INCLUDED_PXXMLXPATHLOADERIMP_H_ */
