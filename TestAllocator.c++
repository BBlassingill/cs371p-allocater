// ------------------------------------
// projects/allocator/TestAllocator.c++
// Copyright (C) 2011
// Glenn P. Downing
// ------------------------------------

/**
 * To test the program:
 *     g++ -ansi -pedantic -lcppunit -ldl -Wall TestAllocator.c++ -o TestAllocator.app
 *     valgrind TestAllocator.app >& TestAllocator.out
 */

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator
#include <iostream>

#include "cppunit/extensions/HelperMacros.h" // CPPUNIT_TEST, CPPUNIT_TEST_SUITE, CPPUNIT_TEST_SUITE_END
#include "cppunit/TestFixture.h"             // TestFixture
#include "cppunit/TextTestRunner.h"          // TestRunner

#include "Allocator.h"

// -------------
// TestAllocator
// -------------
using namespace std;
template <typename A>
struct TestAllocator : CppUnit::TestFixture {
    // --------
    // typedefs
    // --------

    typedef typename A::value_type      value_type;
    typedef typename A::difference_type difference_type;
    typedef typename A::pointer         pointer;

    // --------
    // test_one
    // --------

    void test_one () {
        A x;
        const difference_type s = 1;
        const value_type      v = 2;
        const pointer         p = x.allocate(s);
        x.construct(p, v);
        CPPUNIT_ASSERT(*p == v);
        x.destroy(p);
        x.deallocate(p, s);}

    // --------
    // test_ten
    // --------

    void test_ten () {
        A x;
        const difference_type s = 10;
        const value_type      v = 2;
        const pointer         b = x.allocate(s);
              pointer         e = b + s;
              pointer         p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        CPPUNIT_ASSERT(std::count(b, e, v) == s);
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}
        
    // --------
    // test too small of allocator
    // --------   
	void test_allocate_1 () 
	{
        	A x;
        	const difference_type s = 0;
        	//bool errorReached = false;
        	try {
            		x.allocate(s);
        }
        
        catch (std::invalid_argument) {
            CPPUNIT_ASSERT(true);
        }
        }
        
    // --------
    // test not enough memory
    // --------   
	void test_allocate_2 () 
	{
        	
        	  A x;
        	  const difference_type s = 26;
        	  try
        	  {
        	  	x.allocate(s);
        	  }
        	  
        	  catch (std::invalid_argument)
        	  {
        	  	CPPUNIT_ASSERT(true);
        	  }     
    	}
    
    // --------
    // test a normal allocation
    // --------   
	void test_valid_allocation () 
	{
        	
        	  A x;
        	  const difference_type s = 5;
        	  try
        	  {
        	  	x.allocate(s);
        	  }
        	  
        	  catch (std::invalid_argument)
        	  {
        	  	CPPUNIT_ASSERT(true);
        	  }     
    	}         

    // --------
    // test construct
    // --------
    	void test_construct_one() {
      		A x;
		const difference_type s = 1;
      		const value_type v = 2;
      		const pointer p = x.allocate(s);
      		//cout << "Did I make it here?" << endl;      		
      		x.construct(p, v);
      		CPPUNIT_ASSERT(*p == v);
      		x.destroy(p);
      		x.deallocate(p, s);
    }
    

    // -----
    // suite
    // -----

    CPPUNIT_TEST_SUITE(TestAllocator);
    CPPUNIT_TEST(test_allocate_1);
    CPPUNIT_TEST(test_allocate_2);
    CPPUNIT_TEST(test_construct_one);
    //CPPUNIT_TEST(test_one);
    //CPPUNIT_TEST(test_ten);
    CPPUNIT_TEST_SUITE_END();};

// ----
// main
// ----

int main () {
    	using namespace std;
    	ios_base::sync_with_stdio(false);    // turn off synchronization with C I/O
    	cout << "TestAllocator.c++" << endl;

    	CppUnit::TextTestRunner tr;

    	//tr.addTest(TestAllocator< std::allocator<int> >::suite());
  	tr.addTest(TestAllocator< Allocator<int, 100> >::suite()); // uncomment!

    	//tr.addTest(TestAllocator< std::allocator<double> >::suite());
  	//tr.addTest(TestAllocator< Allocator<double, 100> >::suite()); // uncomment!

    	tr.run();

    	cout << "Done." << endl;
    	return 0;}

