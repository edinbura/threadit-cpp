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

#if !defined (UINT_DATAITEM_H)
#define UINT_DATAITEM_H

#include <memory>
#include <dataitem.h>

class CUintDataItem : public CDataItem
{
  // Attributes
  protected:
		UINT* m_ptheItem;

  public:
    CUintDataItem::CUintDataItem ()
    {
			m_ptheItem = NULL;
      // Initialise the parameters to the default values.
    } // CUintDataItem

    CUintDataItem::~CUintDataItem ()
    {
      if (m_ptheItem != NULL)
			{
				delete m_ptheItem;
			} // if
    } // CUintDataItem

		void setDataItem (UINT* ptheItem)
		{
			m_ptheItem = ptheItem;
		} //

		void getDataItem (UINT*& ptheItem)
		{
			ptheItem = m_ptheItem;
		} //

}; // CUintDataItem

#endif // !defined (UINT_DATAITEM_H)