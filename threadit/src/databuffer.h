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
#ifndef _DATA_BUFFER
#define _DATA_BUFFER

#include <windows.h>
#include <vector>
#include <string>

/**
 * Class CDataBuffer manages a sequence of bytes generally read in from an I/O
 * device. The class supports concatenation of data buffers and general
 * access to the values stored within.
 */
class CDataBuffer
{
	public:
		union CByteArrayWordUnion
		{
			BYTE a[2];
			USHORT i;
		};

		union CByteArrayDWordUnion
		{
			BYTE a[4];
			UINT i;
		};

	// Attributes
	protected :
		/** m_TheData is the sequence of bytes read in from the I/O device. */
		std::vector<BYTE> m_theData;
		UINT m_thePos;
		bool m_isHostOrder;

	// Constructors and Destructors
public :
	/**
	 * Method CDataBuffer is the constructor for the instance. This is currently an
	 * empty implementation.
	 */
	CDataBuffer ();

		CDataBuffer (bool isHost);

		/**
		* Method CDataBuffer is the copy constructor for the instance.
		*/
		CDataBuffer (const CDataBuffer &theBuffer);

	/**
	 * Method ~CDataBuffer is the destructor for the instance. This implementation will
	 * release all bytes stored in the instance.
	 */
	~CDataBuffer ();

	// Services
public :
	/**
	 * Method addData is called to add an array or sequence of bytes into the
	 * databuffer instance.
	 * ptheBytes is the array or pointer to the sequence of bytes to add.
	 * theBytes is the number of bytes to added from the sequence.
	 * The method returns true if at least one byte is added.
	 */
	bool addData (const BYTE* ptheBytes, int theBytes);

	/**
	 * Method addByte is called to add a single byte into the
	 * databuffer instance.
	 * ptheByte is the byte to add.
	 */
	void addByte (BYTE theByte);

	/**
	 * Method getData is called to retrieve the sequence of all the bytes
	 * currently stored in the data buffer.
	 * ptheBytes returns a copy of the sequence of bytes in the data buffer.
	 * The recipient is responsible for freeing the memory allocated to return
	 * the copy of the data buffer contents. Use delete [] to delete the array.
	 * theBytes returns the number of bytes returned.
	 * Method GetData returns true if there are bytes to return.
	 */
	bool getData (BYTE** ptheBytes, int* theBytes);

	/**
	 * Method getByte is called to return a byte at the specified position
	 * in the data buffer.
	 * theByte returns the byte at the given position in the buffer.
	 * thePosition specifies the location of the byte to return.
	 * Method GetByte returns true if there is a byte at the specified
	 * position to return.
	 */
	bool getByte (BYTE& theByte, int thePosition) const;

	/**
	 * Method popByte is called to return the byte at the front of the
	 * buffer and remove it from the buffer. This corresponds to the fir
	 * first element in the buffer (FIFO).
	 * theByte returns the byte at the front of the buffer.
	 * Method popByte returns true if there is a byte at the front of
	 * the buffer to return. The buffer size will be reduced by one if the
	 * byte is popped successfully.
	 */
	bool popByte (BYTE& theByte);
	/**
	 * Method getByte is called to return a byte at the specified position
	 * in the data buffer.
	 * theByte returns the byte at the given position in the buffer.
	 * thePosition specifies the location of the byte to return.
	 * Method GetByte returns true if there is a byte at the specified
	 * position to return. // Automatically advances the position.
	 */
	bool getByte (BYTE& theByte);

	/**
	 * Method getByte is called to return a byte at the specified position
	 * in the data buffer.
	 * theByte returns the byte at the given position in the buffer.
	 * thePosition specifies the location of the byte to return.
	 * Method GetByte returns true if there is a byte at the specified
	 * position to return. // Automatically advances the position.
	 */
	bool isMore () const;

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
	bool remove (UINT elements);

	/**
	 * Method operator+= supports the concatenation of two data buffers.
	 * The data sequence of the second buffer is added to the data
	 * sequence of this buffer.
	 * theBuffer is the data buffer whose bytes are to be added.
	 * The method returns the combination of the byte sequence in the
	 * two buffers.
	 */
	CDataBuffer &CDataBuffer::operator+= (const CDataBuffer &theBuffer);

	/**
	 * Method operator+ supports the concatenation of two data buffers.
	 * The data sequence of the second buffer is added to the data
	 * sequence of the first buffer.
	 * theBuffer is the data buffer whose bytes are to be added.
	 * The method returns the combination of the byte sequence in the
	 * two buffers.
	 */
	CDataBuffer operator+ (const CDataBuffer &theBuffer);

	/**
	 * Method operator= supports the assignment of the specified data
	 * buffer to the target data buffer. All data bytes associated with the
	 * target data buffer will be lost.
	 * theBuffer is the data buffer whose bytes are to be assigned to the
	 * target buffer.
	 */
	CDataBuffer &operator= (const CDataBuffer &theBuffer);

	/**
	 * Method size is called to return the number of bytes currently
	 * contained in the data buffer.
	 */
	int size () const;

	/**
	 * Method reset is called to clear the data buffer of all elements. This allows
	 * the data buffer to be re-used without having to be re-created.
	 */
	void reset ();

	// Add a method clone that will allow this to be used as if it were a queue as well.
	// Fill then take a clone and then reset for reuse.

	// Possibly add a split method. Possibley split and clear from given position.

	// possibily add a method call insert (to insert ahead of)
	void resetPos ();

	UINT getPos () const;

	void undoPos ();

		void addWord (USHORT word);

		void addDWord (UINT dword);

		bool getWord (USHORT &theWord, UINT offset);

		bool getDWord (UINT &theDWord, UINT offset);

		bool getWord (USHORT &theWord);

		bool getDWord (UINT &theDWord);

		void addString (const std::string theStr);

		UINT getString (std::string& theStr, UINT theCount);

		UINT getString (std::string& theStr, UINT theCount, UINT theOffset);

		void setHost (bool isHostOrder);

		/**
		 * @name: getData() const
		 * @description: provides a way to access the data without altering it.
		 * also avoids allocating a new BYTE[] on the heap to get at the contents.
		 * @returns: std::vector<BYTE> const&
		 * The returned reference should be used by the caller to initialise their own copy,
		 * its lifetime must not exceed the method call (i.e. don't hold onto the returned reference).
		 */
		std::vector<BYTE> const& getData() const;
		/**
		 * @name: isHostOrder() const
		 * @description: provides a way to determine the byte ordering of the data.
		 * @returns: true or false
		 * <p>true meaning host ordered (big endian)</p>
		 * <p>false meaning network ordered (little endian)</p>
		 */
		bool isHostOrder() const;
}; // class CDataBuffer

#endif	// #ifndef _DATA_BUFFER
