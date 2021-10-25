/*-------------------------------------------------------------------------*/
/* Copyright (C) 2021 by Ashkel Software                                   */
/* ari@ashkel.com.au                                                       */
/*                                                                         */
/* This file is part of the threadit library.                              */
/*                                                                         */
/* The threadit library is free software; you can redistribute it and/or   */
/* modify it under the terms of The Code Project Open License (CPOL) 1.02  */
/*                                                                         */
/* The threadit library is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the CPOL       */
/* License for more details.                                               */
/*                                                                         */
/* You should have received a copy of the CPOL License along with this     */
/* software.                                                               */
/*-------------------------------------------------------------------------*/

#if !defined (DATAITEM_H)
#define DATAITEM_H

#include <memory>
class CDataItem;

/** WorkOutput is the a queued protected by a critical section. This queue
 * is used to transfer generic items around. */
typedef std::shared_ptr <CDataItem> DataItemPtr;

class CDataItem
{
  public:
    CDataItem::CDataItem ()
    {
    } // CUintDataItem

    virtual CDataItem::~CDataItem ()
    {
    } // ~CUintDataItem


}; // CUintDataItem

#endif // !defined (DATAITEM_H)