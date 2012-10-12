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
    // test_three
    // --------

    void test_three () {
        A x;
        const difference_type s = 1;
        const value_type v = 2;
        const value_type u = 4;
        const value_type y = 6;
        const pointer p = x.allocate(s);
        const pointer q = x.allocate(s);
        const pointer r = x.allocate(s);

        x.construct(p, v);
        x.construct(q, u);
        x.construct(r, y);

        CPPUNIT_ASSERT(*p == v);
        CPPUNIT_ASSERT(*q == u);
        x.destroy(p);
        x.destroy(r);        
        x.destroy(q);
        x.deallocate(p, s);
        x.deallocate(r, s);
        x.deallocate(q, s);}

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


    // --------
    // test two blocks
    // --------

    void test_twoBlocks() {
        A x;
        const value_type v = 4;
        const difference_type s = 5;
        const difference_type t = 1;
        const pointer b1 = x.allocate(s);
              pointer e1 = b1 + s;
              pointer p1 = b1;
        const pointer b2 = x.allocate(t);
              pointer e2 = b2 + t;
              pointer p2 = b2;
        
        try{
            while(p1 != e1){
                x.construct(p1,v);
                ++p1;
            }
        }catch(...){
            while (b1 != p1) {
                --p1;
                x.destroy(p1);}
            x.deallocate(b1, s);
            throw;
        }
        CPPUNIT_ASSERT(std::count(b1, e1, v) == s);
        try{
            while(p2 != e2){
                x.construct(p2,v);
                ++p2;
            }
        }catch(...){
            while (b2 != p2) {
                --p2;
                x.destroy(p2);}
            x.deallocate(b2, t);
            throw;
        }
        CPPUNIT_ASSERT(std::count(b2, e2, v) == t);
        
        while (b1 != e1) {
            --e1;
            x.destroy(e1);}
        x.deallocate(b1, s);
        
        while (b2 != e2) {
            --e2;
            x.destroy(e2);}
        x.deallocate(b2, t);
    }    

    // -------
    // test_coallesce
    // -------
    
    void test_coalesce () {
        A x;
        const difference_type s = 1;
        const value_type      v = 2;
        const pointer         b1 = x.allocate(s);
	//cout << "First allocation: " << endl;
	//cout << "First sentinel is: " << x.view(x.a[0]) << endl;
	//cout << "Second sentinel is: " << x.view(x.a[8]) << endl;
        const pointer         b2 = x.allocate(s);
	//cout << "Second allocation: " << endl;
	//cout << "First sentinel is: " << x.view(x.a[12]) << endl;
	//cout << "Second sentinel is: " << x.view(x.a[20]) << endl;
        const pointer         b3 = x.allocate(s);
	//cout << "Third allocation: " << endl;
	//cout << "First sentinel is: " << x.view(x.a[24]) << endl;
	//cout << "Second sentinel is: " << x.view(x.a[32]) << endl;
       
	x.destroy(b1);        
	x.deallocate(b1,s);
	//cout << "First deallocation was valid!" << endl;
	//cout << "First sentinel is: " << x.view(x.a[0]) << endl;;
	//cout << "Second sentinel is: " << x.view(x.a[8]) << endl;;
	//cout << "Last sentinel is: " << x.view(x.a[96]) << endl;
	
	x.destroy(b2);        
	x.deallocate(b2,s);
	//cout << "Second deallocation was valid!" << endl;
	//cout << "First sentinel is: " << x.view(x.a[0]) << endl;;
	//cout << "Second sentinel is: " << x.view(x.a[20]) << endl;;
	//cout << "Next sentinel is: " << x.view(x.a[24]) << endl;
	
	x.destroy(b3);
        x.deallocate(b3,s);
	//cout << "‏Third deallocation was valid!" << endl;
	//cout << "First sentinel is: " << x.view(x.a[0]) << endl;
	//cout << "Second sentinel is: " << x.view(x.a[96]) << endl;
        CPPUNIT_ASSERT(x.view(x.a[0]) == x.view(x.a[96]));
    }

    // -----
    // suite
    // -----

    CPPUNIT_TEST_SUITE(TestAllocator);
    CPPUNIT_TEST(test_allocate_1); //WORKS FINE, UNCOMMENT LATER
    CPPUNIT_TEST(test_allocate_2); //WORKS FINE, UNCOMMENT LATER
    CPPUNIT_TEST(test_construct_one);
    CPPUNIT_TEST(test_one);
    CPPUNIT_TEST(test_three);
    CPPUNIT_TEST(test_ten);
    CPPUNIT_TEST(test_twoBlocks);
    CPPUNIT_TEST(test_coalesce);
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
  	tr.addTest(TestAllocator< Allocator<double, 100> >::suite()); // uncomment!

    	tr.run();

    	cout << "Done." << endl;
    	return 0;}

