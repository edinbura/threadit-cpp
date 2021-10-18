

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