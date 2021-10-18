/**
 * Title: TestDataBuffer
 * Description: TestDataBuffer contains unit tests for the CDataBuffer class.
 * The purpose of these tests is to excercise as many methods of the CDataBuffer
 * class as possible so that the class behaviour is correct and the class
 * can be regression tested as part of an automated test suite.
 *
 * Copyright: Copyright (c) 2008 Ashkel Software
 * @author Ari Edinburg
 * @version 1.0
 * $Revision: 1.1 $<br>
 * $Date: 2006/12/19 02:38:06 $
 */

#include "stdafx.h"
#include <UnitTest++.h>
#include <databuffer.h>
#include <dataqueue.h>
#include <log4cpp\Category.hh>
#include <log4cpp/BasicConfigurator.hh>
#include <log4cpp/PropertyConfigurator.hh>
#pragma comment(lib, "Ws2_32.lib")

/**
 * Test_DataBuffer_addData is a unit test that adds the specified number of elements
 * into the buffer and verfies that the AddData method adds these elements. The test
 * is based on the size of the elements in the data buffer as returned by the Size
 * method.
 */
TEST (Test_DataBuffer_addData)
{
	int theMaxSize = 10000;
	BYTE* aPtr = NULL;
	BYTE* ptheBytes = NULL;
	CDataBuffer theData;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestDataBuffer"));
	logger->info ("Testing - Test_DataBuffer_AddData");

	ptheBytes = new BYTE [theMaxSize];
	aPtr = ptheBytes;
	for (int i = 0; i < theMaxSize; i++)
	{
		*aPtr = i;
		aPtr++;
	} // for
	theData.addData (ptheBytes, theMaxSize);
	CHECK_EQUAL (theMaxSize, theData.size ());

	theData.reset();
	bool bHostByteOrder = true;

	theData.setHost( !bHostByteOrder );
	theData.addWord( 10000 );

	CDataBuffer theOtherData;
	theOtherData.setHost( bHostByteOrder );
	theOtherData.addWord( 10000 );
	CHECK( theData.isHostOrder() != theOtherData.isHostOrder());
	CHECK( theData.getData()[0] != theOtherData.getData()[0]);
	CHECK( theData.getData()[1] != theOtherData.getData()[1]);

	CHECK_EQUAL( theData.getData()[0], theOtherData.getData()[1]);
	CHECK_EQUAL( theData.getData()[1], theOtherData.getData()[0]);
} // TEST (Test_DataBuffer_addData)

/**
 * Test_DataBuffer_addByte is a unit test that adds the specified number of elements
 * into the buffer and verfies that the AddByte method adds these elements. The test
 * is based on the size of the elements in the data buffer as returned by the Size
 * method.
 */
TEST (Test_DataBuffer_addByte)
{
	int theMaxSize = 10000;
	CDataBuffer theData;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestDataBuffer"));
	logger->info ("Testing - Test_DataBuffer_AddByte");

	for (int i = 0; i < theMaxSize; i++)
	{
		theData.addByte (i);
	} // for
	CHECK_EQUAL (theMaxSize, theData.size ());
} // TEST (Test_DataBuffer_addData)

/**
 * Test_DataBuffer_getData is a unit test that retrieves the elements in the
 * data buffer instance and checks that each element put in matches each
 * element returned.
 */
TEST (Test_DataBuffer_getData)
{
	int theVal = 0;
	int theMaxSize = 10000;
	int theRetBytes = 0;
	BYTE* aPtr = NULL;
	BYTE* ptheBytes = NULL;
	BYTE* ptheRetBytes = NULL;
	CDataBuffer theData;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestDataBuffer"));
	logger->info ("Testing - Test_DataBuffer_GetData");

	// Create a sequence of bytes to store.
	ptheBytes = new BYTE [theMaxSize];
	aPtr = ptheBytes;
	for (int i = 0; i < theMaxSize; i++)
	{
		*aPtr = i;
		aPtr++;
	} // for
	theData.addData (ptheBytes, theMaxSize);
	CHECK_EQUAL (theMaxSize, theData.size ());
	std::vector<BYTE> theByteVector = theData.getData();
	CHECK_EQUAL (theByteVector.size(), theData.size ());
	theByteVector.clear();
	CHECK_EQUAL (theByteVector.size(), 0);

	// Get the data from the buffer.
	theData.getData (&ptheRetBytes, &theRetBytes);
	// Check that the number of bytes returned is the same as that
	// in the buffer.
	CHECK_EQUAL (theRetBytes, theData.size ());
	// Make sure what went in is what comes out.
	aPtr = ptheRetBytes;
	for (int i = 0; i < theMaxSize; i++)
	{
		theVal = *aPtr;
		aPtr++;
		CHECK_EQUAL (theVal, (BYTE)i);
	} // for
	// Free the allocated memory.
	delete [] ptheBytes;
	delete [] ptheRetBytes;
} // TEST (Test_DataBuffer_getData)

/**
 * Test_DataBuffer_getByte is a unit test that retrieve an element in the
 * data buffer instance and checks that each element put in matches each
 * element returned via the GetByte method.
 */
TEST (Test_DataBuffer_getByte)
{
	int theVal = 0;
	int theMaxSize = 10000;
	bool isValue = false;
	BYTE aByte = 0;
	CDataBuffer theData;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestDataBuffer"));
	logger->info ("Testing - Test_DataBuffer_getByte");

	// Create a sequence of bytes to store.
	for (int i = 0; i < theMaxSize; i++)
	{
		theData.addByte (i);
	} // for
	// Retrieve the bytes one at a time.
	for (int i = 0; i < theMaxSize; i++)
	{
		isValue = theData.getByte (aByte, i);
		CHECK (isValue);
		CHECK_EQUAL (aByte, (BYTE)i);
	} // for
	isValue = theData.getByte (aByte, theMaxSize);
	CHECK (!isValue);
} // TEST (Test_DataBuffer_getByte)

/**
 * Test_DataBuffer_OperatorPlus is a unit test that checks that
 * two data buffers can be concatenated correctly. The test
 * checks that the sequence of bytes returned matches that expected
 * sequence when two separate buffers are concatenated together.
 */
TEST (Test_DataBuffer_OperatorPlus)
{
	int theVal = 0;
	int theCount = 0;
	int theMaxSize = 10000;
	bool isValue = false;
	BYTE aByte = 0;
	CDataBuffer theData1;
	CDataBuffer theData2;
	CDataBuffer theData3;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestDataBuffer"));
	logger->info ("Testing - Test_DataBuffer_OperatorPlus");

	// Create a sequence of bytes to store.
	for (int i = 0; i < theMaxSize; i++)
	{
		theData1.addByte (i);
		theData2.addByte (i);
	} // for
	// Now concatenate the data.
	theData3 = theData1 + theData2;
	CHECK_EQUAL (2 * theMaxSize, theData3.size ());
	// Retrieve the bytes one at a time.
	for (int i = 0; i < theMaxSize; i++)
	{
		isValue = theData3.getByte (aByte, i);
		CHECK (isValue);
		CHECK_EQUAL (aByte, (BYTE)i);
	} // for
	for (int i = theMaxSize; i < 2 * theMaxSize; i++)
	{
		isValue = theData3.getByte (aByte, i);
		CHECK (isValue);
		CHECK_EQUAL (aByte, (BYTE)theCount);
		theCount++;
	} // for
	isValue = theData3.getByte (aByte, 2 * theMaxSize);
	CHECK (!isValue);
} // TEST (Test_DataBuffer_OperatorPlus)

/**
 * Test_DataBuffer_OperatorEquals is a unit test that checks that
 * a data buffer can be assigned correctly to another data buffer.
 * The test checks that the elements of two buffers concatenated
 * and then assigned to the first element are correct.
 */
TEST (Test_DataBuffer_OperatorEquals)
{
	int theVal = 0;
	int theCount = 0;
	int theMaxSize = 10000;
	bool isValue = false;
	BYTE aByte = 0;
	CDataBuffer theData1;
	CDataBuffer theData2;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestDataBuffer"));
	logger->info ("Testing - Test_DataBuffer_OperatorEquals");

	// Create a sequence of bytes to store.
	for (int i = 0; i < theMaxSize; i++)
	{
		theData1.addByte (i);
		theData2.addByte (i);
	} // for
	// Now concatenate the data.
	theData1 = theData1 + theData2;
	CHECK_EQUAL (2 * theMaxSize, theData1.size ());
	// Retrieve the bytes one at a time.
	for (int i = 0; i < theMaxSize; i++)
	{
		isValue = theData1.getByte (aByte, i);
		CHECK (isValue);
		CHECK_EQUAL (aByte, (BYTE)i);
	} // for
	for (int i = theMaxSize; i < 2 * theMaxSize; i++)
	{
		isValue = theData1.getByte (aByte, i);
		CHECK (isValue);
		CHECK_EQUAL (aByte, (BYTE)theCount);
		theCount++;
	} // for
	isValue = theData1.getByte (aByte, 2 * theMaxSize);
	CHECK (!isValue);
} // TEST (Test_DataBuffer_OperatorEquals)

/**
 * Test_DataBuffer_reset is a unit test that adds the specified number of elements
 * into the buffer and verfies that the AddData method adds these elements. The
 * data buffer is reset and the test repeated. The test is based on the size of the
 * elements in the data buffer as returned by the Size	method.
 */
TEST (Test_DataBuffer_reset)
{
	int theMaxSize = 10000;
	BYTE* aPtr = NULL;
	BYTE* ptheBytes = NULL;
	CDataBuffer theData;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestDataBuffer"));
	logger->info ("Testing - Test_DataBuffer_reset");

	ptheBytes = new BYTE [theMaxSize];
	aPtr = ptheBytes;
	for (int i = 0; i < theMaxSize; i++)
	{
		*aPtr = i;
		aPtr++;
	} // for
	theData.addData (ptheBytes, theMaxSize);
	CHECK_EQUAL (theMaxSize, theData.size ());
	// Now test the reset capability of the class.
	theData.reset ();
	CHECK_EQUAL (0, theData.size ());
	// Add the data in again and confirm.
	theData.addData (ptheBytes, theMaxSize);
	CHECK_EQUAL (theMaxSize, theData.size ());
} // TEST (Test_DataBuffer_reset)

/**
 * Test_DataBuffer_popByte is a unit test that adds the specified number of elements
 * into the buffer and verfies that the popByt method removes these elements.
 */
TEST (Test_DataBuffer_popByte)
{
	int theMaxSize = 250;
	bool isByte = false;
	BYTE aByte = 0;
	int theErrorCount = 0;
	CDataBuffer theData;

	log4cpp::Category *logger = &(log4cpp::Category::getInstance ("TestDataBuffer"));
	logger->info ("Testing - Test_DataBuffer_popByte");

	// Fill the data buffer with a given number of bytes.
	for (int i = 0; i < theMaxSize; i++)
	{
		theData.addByte (i);
	} // for
	CHECK_EQUAL (theMaxSize, theData.size ());
	// Now test the popByte capability of the class.
	for (int i = 0; i < theMaxSize; i++)
	{
		isByte = theData.popByte (aByte);
		if (!isByte)
		{
			theErrorCount++;
		} // if
	} // for
	CHECK_EQUAL (theErrorCount, 0);
	CHECK_EQUAL (aByte, theMaxSize - 1);
	CHECK_EQUAL (theData.size (), 0);

	// Fill the data buffer with a given number of bytes.
	for (int i = 0; i < theMaxSize; i++)
	{
		theData.addByte (i);
	} // for
	CHECK_EQUAL (theMaxSize, theData.size ());
	// Now test the popByte capability of the class.
	for (int i = 0; i < theMaxSize + 10; i++)
	{
		isByte = theData.popByte (aByte);
		if (!isByte)
		{
			theErrorCount++;
		} // if
	} // for
	CHECK_EQUAL (theErrorCount, 10);
	CHECK_EQUAL (theData.size (), 0);
} // TEST (Test_DataBuffer_popByte)
