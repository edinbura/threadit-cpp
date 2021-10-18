/**
 * Title: CDataBuffer
 * Description: class CDataBuffer is provided to supported reading in a sequence
 * of bytes from input/output (I/O) devices. As each sequence of bytes of
 * however long are read in they can be stored in a CDataBuffer instance. The class
 * supports concatention of buffers so as a number of buffers are read in the can
 * be concatenated into a single buffer.
 *
 * Copyright: Copyright (c) 2008 Ashkel Software
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */

// Includes
#include "stdafx.h"
#include <new>
#include <winsock2.h>
#include "DataBuffer.h"

/**
 * Method CDataBuffer is the constructor for the instance. This is currently an
 * empty implementation.
 */
CDataBuffer::CDataBuffer ()
{
	m_thePos = 0;
	m_isHostOrder = false;
}	 // constructor CDataBuffer

CDataBuffer::CDataBuffer (bool isHostOrder)
{
	m_thePos = 0;
	m_isHostOrder = isHostOrder;
}	 // constructor CDataBuffer

void CDataBuffer::setHost (bool isHostOrder)
{
	m_isHostOrder = isHostOrder;
}	 // setHost


/**
 * Method CDataBuffer is the copy constructor for the instance.
 */
CDataBuffer::CDataBuffer (const CDataBuffer &theBuffer)
{
	m_thePos = theBuffer.m_thePos;
	m_isHostOrder = theBuffer.m_isHostOrder;
	m_theData.resize(theBuffer.m_theData.size());
	std::copy(theBuffer.m_theData.begin(), theBuffer.m_theData.end(), m_theData.begin());
}	 // constructor CDataBuffer

/**
 * Method ~CDataBuffer is the destructor for the instance. This implementation will
 * release all bytes stored in the instance.
 */
CDataBuffer::~CDataBuffer ()
{
	// Free all elements of the list.
	m_theData.clear ();
} // destructor ~CDataBuffer

/**
 * Method addData is called to add an array or sequence of bytes into the
 * databuffer instance.
 * ptheBytes is the array or pointer to the sequence of bytes to add.
 * theBytes is the number of bytes to added from the sequence.
 * The method returns true if at least one byte is added.
 */
bool CDataBuffer::addData (const BYTE* ptheBytes, int theBytes)
{
	bool isAdded = false;

	// Add the number of bytes specified into the list.
	if ((ptheBytes != NULL) && (theBytes > 0))
	{
		for (int i = 0; i < theBytes; i++)
		{
			m_theData.push_back (*ptheBytes);
			ptheBytes++;
			isAdded = true;
		} // for
	} // if
	return isAdded;
} // addData

/**
 * Method addByte is called to add a single byte into the
 * databuffer instance.
 * ptheByte is the byte to add.
 */
void CDataBuffer::addByte (BYTE theByte)
{
	m_theData.push_back (theByte);
} // addByte
 
/**
 * Method GetData is called to retrieve the sequence of all the bytes
 * currently stored in the data buffer.
 * ptheBytes returns a copy of the sequence of bytes in the data buffer.
 * The recipient is responsible for freeing the memory allocated to return
 * the copy of the data buffer contents. Use delete [] to delete the array.
 * theBytes returns the number of bytes returned.
 * Method GetData returns true if there are bytes to return.
 */
bool CDataBuffer::getData (BYTE** ptheBytes, int* theBytes)
{
	int theSize = 0;
	bool isSuccess = false;
	BYTE* ptheByteSeq = NULL;
	BYTE* aPtr = NULL;

	// Determine the number of bytes we need to copy.
	theSize = (int)m_theData.size ();
	// Allocate memory for the byte sequence.
	ptheByteSeq = new BYTE [theSize];
	// Get a pointer to the memory.
	aPtr = ptheByteSeq;
	// Copy the bytes from the data buffer into the allocated memory.
	for (int i = 0; i < theSize; i++)
	{
		*aPtr = m_theData.at (i);
		aPtr++;
	} // for
	// Determine if bytes were copied.
	isSuccess = (theSize > 0);
	// Return the result.
	*ptheBytes = ptheByteSeq;
	*theBytes = theSize;
	return isSuccess;
} // getData

/**
 * Method getByte is called to return a byte at the specified position
 * in the data buffer.
 * theByte returns the byte at the given position in the buffer.
 * thePosition specifies the location of the byte to return.
 * Method getByte returns true if there is a byte at the specified
 * position to return.
 */
bool CDataBuffer::getByte (BYTE& theByte, int thePosition) const
{
	bool isSuccess = false;

	if ((thePosition >= 0) && (thePosition < (int)m_theData.size ()))
	{
		theByte = m_theData.at (thePosition);
		isSuccess = true;
	} // if
	return isSuccess;;
} // getByte

/**
 * Method popByte is called to return the byte at the front of the
 * buffer and remove it from the buffer. This corresponds to the fir
 * first element in the buffer (FIFO).
 * theByte returns the byte at the front of the buffer.
 * Method popByte returns true if there is a byte at the front of
 * the buffer to return. The buffer size will be reduced by one if the
 * byte is popped successfully.
 */
bool CDataBuffer::popByte (BYTE& theByte)
{
	bool isSuccess = false;

	if (m_theData.size () > 0)
	{
		theByte = m_theData.at (0);
		m_theData.erase (m_theData.begin());
		isSuccess = true;
	} // if
	return isSuccess;;
} // popByte

/**
 * Method getByte is called to return a byte at the specified position
 * in the data buffer.
 * theByte returns the byte at the given position in the buffer.
 * thePosition specifies the location of the byte to return.
 * Method GetByte returns true if there is a byte at the specified
 * position to return. // Automatically advances the position.
 */
bool CDataBuffer::getByte (BYTE& theByte)
{
	bool isSuccess = false;

	if ((m_thePos >= 0) && (m_thePos < (int)m_theData.size ()))
	{
		theByte = m_theData.at (m_thePos);
		isSuccess = true;
		m_thePos++;
	} // if
	return isSuccess;;
} // getByte

/**
 * Method getByte is called to return a byte at the specified position
 * in the data buffer.
 * theByte returns the byte at the given position in the buffer.
 * thePosition specifies the location of the byte to return.
 * Method GetByte returns true if there is a byte at the specified
 * position to return. // Automatically advances the position.
 */
bool CDataBuffer::isMore () const
{
	bool isMoreBytes = false;

	if ((m_thePos >= 0) && (m_thePos < (int)m_theData.size ()))
	{
		isMoreBytes = true;
	} // if
	return isMoreBytes;;
} // isMore

/**
 * Method remove is called to remove the number of bytes specified
 * from the start of the data buffer.
 * elements specifies the number of elements to remove from the start
 * of the buffer.
 * This method is equivalent to consuming elements from the beginning
 * of the buffer.
 * Method remove returns true if the specified number of elements
 * has been removed. The method returns false if the number of elements
 * specified for removal exceed the capacity of the buffer.
 */
bool CDataBuffer::remove (UINT elements)
{
	bool isSuccess = false;

	// Erase the specified number of elements from the beginning of the vector.
	if (elements <= m_theData.size ())
	{
		m_theData.erase (m_theData.begin(), m_theData.begin() + elements);
		m_thePos = 0;
		isSuccess = true;
	} // if
	return isSuccess;
} // remove

/**
 * Method operator+= supports the concatenation of two data buffers.
 * The data sequence of the second buffer is added to the data
 * sequence of the first buffer.
 * theBuffer is the data buffer whose bytes are to be added.
 * The method returns the combination of the byte sequence in the
 * two buffers.
 */
CDataBuffer &CDataBuffer::operator+= (const CDataBuffer &theBuffer)
{
	std::vector<BYTE>::size_type oldSize = m_theData.size();
	m_theData.resize (oldSize + theBuffer.size());
	std::copy (theBuffer.m_theData.begin(), theBuffer.m_theData.end(), m_theData.begin() + oldSize);
	return *this;
} // operator+


/**
 * Method operator+ supports the concatenation of two data buffers.
 * The data sequence of the second buffer is added to the data
 * sequence of the first buffer.
 * theBuffer is the data buffer whose bytes are to be added.
 * The method returns the combination of the byte sequence in the
 * two buffers.
 */
CDataBuffer CDataBuffer::operator+ (const CDataBuffer &theBuffer)
{
	CDataBuffer aBuffer(*this);
	aBuffer += theBuffer;
	return aBuffer;
} // operator+

/**
 * Method operator= supports the assignment of the specified data
 * buffer to the target data buffer. All data bytes associated with the
 * target data buffer will be lost.
 * theBuffer is the data buffer whose bytes are to be assigned to the
 * target buffer.
 */
CDataBuffer &CDataBuffer::operator= (const CDataBuffer &theBuffer)
{
	if (this != &theBuffer)
	{
	  // Often it is necessary to first call the destructor with this idiom,
	  // however in this class the copy constructor is coded so that it will
	  // still work without calling the destructor first.
	  // Call the copy constructor using placement operator new.
	  // new(this)CDataBuffer(theBuffer); Found by SH - this causes a memory leak.
	  // SH - using the normal assignment instead of the placement new will pacify the memory validator
	  m_thePos = theBuffer.m_thePos;
	  m_isHostOrder = theBuffer.m_isHostOrder;
	  m_theData.resize(theBuffer.m_theData.size());
	  std::copy (theBuffer.m_theData.begin(), theBuffer.m_theData.end(), m_theData.begin());
	}
	return *this;
} // operator=

/**
 * Method size is called to return the number of bytes currently
 * contained in the data buffer.
 */
int CDataBuffer::size () const
{
	return static_cast<int>(m_theData.size ());
} // size

/**
 * Method reset is called to clear the data buffer of all elements. This allows
 * the data buffer to be re-used without having to be re-created.
 */
void CDataBuffer::reset ()
{
	m_theData.clear ();
	m_thePos = 0;
} // reset
/**
 * Method reset is called to clear the data buffer of all elements. This allows
 * the data buffer to be re-used without having to be re-created.
 */
void CDataBuffer::resetPos ()
{
	m_thePos = 0;
} // resetPos

UINT CDataBuffer::getPos () const
{
	return m_thePos;
} // getPos

void CDataBuffer::undoPos ()
{
	if (m_thePos > 0)
	{
		m_thePos--;
	} // if
} // undoPos

/////////////////////////////////////////// NEW
void CDataBuffer::addWord (USHORT word)
{
  USHORT theBytes = word;

	if (m_isHostOrder)
	{
		theBytes = htons (word);          // fix byte order
	} // if 
  this->addByte (theBytes & 0x00ffU);
  this->addByte (theBytes >> 8U);
}

void CDataBuffer::addDWord (UINT dword)
{
  UINT theBytes = dword;
	if (m_isHostOrder)
	{
		theBytes = htonl (dword);          // fix byte order
	} // if 
  this->addByte (theBytes & 0x000000ffU);
  this->addByte (theBytes >> 8U);
  this->addByte (theBytes >> 16U);
  this->addByte (theBytes >> 24U);
}

bool CDataBuffer::getWord (USHORT &theWord, UINT offset)
{
  CByteArrayWordUnion u;
  if (!(this->getByte (u.a[0], offset++) && this->getByte (u.a[1], offset++)))
    return false;
	theWord = u.i;
	if (m_isHostOrder)
	{
		theWord = ntohs(u.i);
	} // if 
  return true;
}

bool CDataBuffer::getDWord (UINT &theDWord, UINT offset)
{
  CByteArrayDWordUnion u;
  if (!(this->getByte (u.a[0], offset++) &&
	this->getByte (u.a[1], offset++) &&
	this->getByte (u.a[2], offset++) &&
	this->getByte (u.a[3], offset++)))
    return false;
	theDWord = u.i;
	if (m_isHostOrder)
	{
    theDWord = ntohl(u.i);
	} // if 
  return true;
}

bool CDataBuffer::getWord (USHORT &theWord)
{
  CByteArrayWordUnion u;
  if (!(this->getByte (u.a[0]) && this->getByte (u.a[1])))
    return false;
	theWord = u.i;
	if (m_isHostOrder)
	{
		theWord = ntohs(u.i);
	} // if 
  return true;
}

bool CDataBuffer::getDWord (UINT &theDWord)
{
  CByteArrayDWordUnion u;
  if (!(this->getByte (u.a[0]) &&
	this->getByte (u.a[1]) &&
	this->getByte (u.a[2]) &&
	this->getByte (u.a[3])))
    return false;
	theDWord = u.i;
	if (m_isHostOrder)
	{
    theDWord = ntohl(u.i);
	} // if 
  return true;
}

void CDataBuffer::addString (const std::string theStr)
{
	for (size_t i = 0; i < theStr.length (); i++)
	{
		this->addByte (theStr[i]);
	} // for 
} // for 

UINT CDataBuffer::getString (std::string& theStr, UINT theCount)
{
	bool isChar = false;
	BYTE aByte;
	UINT i = 0;
	while ((isChar) && (i < theCount))
	{
		isChar = this->getByte (aByte);
		if (isChar)
		{
			theStr += (char)aByte;
			i++;
		} // if 
	} // for 
	return i;
} // getString 

UINT CDataBuffer::getString (std::string& theStr, UINT theCount, UINT theOffset)
{
	bool isChar = false;
	BYTE aByte;
	UINT i = 0;
	UINT thePos = theOffset;
	while ((isChar) && (i < theCount))
	{
		isChar = this->getByte (aByte, thePos);
		if (isChar)
		{
			theStr += (char)aByte;
			i++;
			thePos++;
		} // if 
	} // for 
	return i;
} // getString 


std::vector<BYTE> const& CDataBuffer::getData() const
{
	return m_theData;
} // getPos

bool CDataBuffer::isHostOrder () const
{
	return m_isHostOrder;
} // isHostOrder
