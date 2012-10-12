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
    // test request too many elements
    // --------   
	void test_too_many_elements () 
	{
        	
        	  A x;
        	  const difference_type s = 50;
        	  try
        	  {
        	  	x.allocate(s);
        	  	CPPUNIT_ASSERT(false);
        	  }
        	  
        	  catch (std::invalid_argument)
        	  {
        	  	CPPUNIT_ASSERT(true);
        	  }     
    	}
    	
    // --------
    // test request overfill for an int allocator
    // --------   
    
    	void test_overfill_int () //should also fail for double case
    	{
    		A x;
    		try
    		{
    			const pointer p = x.allocate(25);
    			x.destroy(p);
       		}
       		
       		catch (std::invalid_argument)
       		{
       			CPPUNIT_ASSERT(true);
       		}

    	}  
    	
    // --------
    // test request overfill for a double allocator
    // --------  
    	void test_overfill_double () //should NOT fail for the int case
    	{
    		A x;
    		try
    		{
    			const pointer p = x.allocate(12);
    			x.destroy(p);
			CPPUNIT_ASSERT(true); //for int
       		}
       		
       		catch (std::invalid_argument)
       		{
       			CPPUNIT_ASSERT(true); //for double
       		}

    	} 

    // --------
    // test request greater than required minimum size left after allocation for an int allocator
    // --------    
    	void test_greater_than_minSpaceRequired_int () //should merge just for int, and fail for double
    	{
    		A x;
		try
		{
		
			const pointer p = x.allocate(22);
			if (x.view(x.a[0]) == -92) //case for int
			{
				if (x.view(x.a[96]) == -92)
				{	
					x.destroy(p);
					CPPUNIT_ASSERT(true);
				}
				else
				{
					x.destroy(p);
					CPPUNIT_ASSERT(false);
				}	
			}
    			
    			
			else
			{	
				x.destroy(p);
				CPPUNIT_ASSERT(false); 
			}
		}
		
		catch(std::invalid_argument)
		{
			CPPUNIT_ASSERT(true); //we want to fail for a double allocator
			
		}
	}
	
    // --------
    // test request greater than required minimum size left after allocation for a double allocator
    // --------    
    	void test_greater_than_minSpaceRequired_double () //should merge just for double
    	{
    		A x;
		const pointer p = x.allocate(11);
		if (x.view(x.a[0]) == -92) //case for double
		{
			if (x.view(x.a[96]) == -92)
			{	
				x.destroy(p);
				CPPUNIT_ASSERT(true);
			}
			else
			{
				x.destroy(p);
				CPPUNIT_ASSERT(false);
			}	
		}
    			
    			
		else
		{	
			x.destroy(p);
			CPPUNIT_ASSERT(true); //for int
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
        const pointer         b1 = x.allocate(s);
        const pointer         b2 = x.allocate(s);
        const pointer         b3 = x.allocate(s);
       
	x.destroy(b1);
	x.deallocate(b1,s);
	
	x.destroy(b2);   
	x.deallocate(b2,s);
	
	x.destroy(b3);
        x.deallocate(b3,s);

        CPPUNIT_ASSERT(x.view(x.a[0]) == x.view(x.a[96]));
    }

    // -----
    // suite
    // -----

    CPPUNIT_TEST_SUITE(TestAllocator);
    CPPUNIT_TEST(test_allocate_1); 
    CPPUNIT_TEST(test_construct_one);
    CPPUNIT_TEST(test_one);
    CPPUNIT_TEST(test_three);
    CPPUNIT_TEST(test_ten);
    CPPUNIT_TEST(test_twoBlocks);
    CPPUNIT_TEST(test_coalesce);
    CPPUNIT_TEST(test_overfill_int);
    CPPUNIT_TEST(test_too_many_elements);
    CPPUNIT_TEST(test_overfill_double);
    CPPUNIT_TEST(test_greater_than_minSpaceRequired_int);
    CPPUNIT_TEST(test_greater_than_minSpaceRequired_double);
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

