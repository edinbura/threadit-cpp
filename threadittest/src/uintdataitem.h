
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