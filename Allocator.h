//Finished allocate method and deallocate

 // ------------------------------
// projects/allocator/Allocator.h
// Copyright (C) 2011
// Glenn P. Downing
// ------------------------------

#ifndef Allocator_h
#define Allocator_h

// --------
// includes
// --------

#include <cassert> // assert
#include <new> // new
#include <stdexcept> // invalid_argument

// ---------
// Allocator
// ---------

template <typename T, int N>
class Allocator {
    public:
        // --------
        // typedefs
        // --------

        typedef T value_type;

        typedef int size_type;
        typedef std::ptrdiff_t difference_type;

        typedef value_type* pointer;
        typedef const value_type* const_pointer;

        typedef value_type& reference;
        typedef const value_type& const_reference;

    public:
        // -----------
        // operator ==
        // -----------

        friend bool operator == (const Allocator&, const Allocator&) {
            return true;}

        // -----------
        // operator !=
        // -----------

        friend bool operator != (const Allocator& lhs, const Allocator& rhs) {
            return !(lhs == rhs);}

	// -----------
        // view
	//Based off of quiz 13
	//Useful in getting sentinel values
        // -----------
	int& view (char& c)
	{
		return *reinterpret_cast<int*>(&c);
	}

	// -----------
        // reverse of view
	//Based off of quiz 13
	//Useful in setting sentinel values
        // -----------
	int& setSentinel (char& c, int value)
	{
		*(reinterpret_cast<int*>(&c)) = value;
	}

    private:
        // ----
        // data
        // ----

        char a[N];
	char* p = (char *) a; //set a char pointer to the beginning of the array
        // -----
        // valid
        // -----

        /**
* O(1) in space
* O(n) in time
* An allocator is valid if all the sentinel values match up
*/
        bool valid () const 
	{
        	char* maxAddress = (char*) a + N;
		int beginningSentinel = 0;
		int endingSentinel = 0;
		while (p < maxAdress)
		{
			beginningSentinel = view(*p);
			if (beginningSentinel < 0) //ie, we found an occupied space
			{
				p = p + sizeof(int) + sizeof(T)*(beginningSentinel* -1);				
				endingSentinel = view(*p);
				if (beginningSentinel != endingSentinel)
					return false;
			}

			else
			{	
				p = p + sizeof(int) + sizeof(T)* beginningSentinel;				
				endingSentinel = view(*p);
				if (beginningSentinel != endingSentinel)
					return false;
			}
		p = p+4; //get to the next sentinel
		}     
            	return true;
	}

    public:
        // ------------
        // constructors
        // ------------

        /**
* O(1) in space
* O(1) in time
* <your documentation>
*/
        Allocator () 
	{
            	int requiredSpace = 8 + sizeof(T);
		if (requiredSpace > N)
			throw std::invalid_argument("Create another allocator with appropriate size N.");
		int sentinel, sentinel = N - 8;
		char* p = a;
		setSentinel(*p, sentinel); //should set beginning sentintel to N - 8
		p = p + 4 + sentinel; //should set ending sentinel to N - 8
		setSentinel(*p, sentinel);			
            assert(valid());
	}

        // Default copy, destructor, and copy assignment
        // Allocator (const Allocator<T>&);
        // ~Allocator ();
        // Allocator& operator = (const Allocator&);

        // --------
        // allocate
        // --------

        /**
* O(1) in space
* O(n) in time
* <your documentation>
* after allocation there must be enough space left for a valid block
* the smallest allowable block is sizeof(T) + (2 * sizeof(int))
* choose the first block that fits
*/
        pointer allocate (size_type n) {
            	int spaceNeeded = n*sizeof(T);
		int maxSpace = N - 8;
		int sentinel = 0;
		int minSpaceRequired = sizeof(T) + 8;
		int spaceLeft = 0;
		if (spaceNeeded > maxSpace)
			throw std::invalid_argument("Not enough space for requested memory.");
	    	bool spaceFound = false;
    		char* p = a;
		char* start = a;
		char* maxAddress = a + N;
		
		
		while (p < maxAddress && spaceFound == false)
		{
			int sentinel = view(*p);
			
			if (sentinel >= spaceNeeded)
			{
				spaceFound = true;
				start = p;
			}
			
			else if (sentinel < 0) //may need to change 8 to 4 later on
				p = p + 8 + -1*sentinel;
			else
				p = p + 8 + sentinel;	
		}
		
		//if we get here and spaceFound is false, there's no free block and we're done
		if (spaceFound == false)
                	throw std::invalid_argument("There was not a block big enough for requested memory");	
                
                else //let's get to work
                	{
                		
                		int spaceLeft = sentinel - 8 - spaceNeeded;
                		if (spaceLeft < minSpaceRequired) //should do a merge
                		{
                			spaceNeeded = sentinel; //space we're working with is now equal to a valid size
                			spaceLeft = 0;
                		}
                		
                		//update sentinels
                		setSentinel(*p, -1*spaceNeeded);
                		p = p + 4 + spaceNeeded;
                		setSentinel(*p, -1*spaceNeeded); 
                		
                		//we need to update the rest of the blocks if there's still space left
                		if (spaceLeft > 0)
                		{
                			p = p + 4; //should get to the next block
                			setSentinel(*p, spaceLeft);
                			p = p + 4 + spaceLeft;
                			setSentinel(*p, spaceLeft);
                		}               		           
                		
                	}				
		
		/*while (p < maxAddress && spaceFound == false)
		{
			int sentinel = view(*p);
			if (sentinel > 0) //ie a free block
			{
				if (sentinel > spaceNeeded) //yay, valid block found
				{
					spaceLeft = sentinel - 8 - spaceNeeded;
					if (spaceLeft < minSpaceRequired) //COME BACK HERE LATER
						spaceNeeded = sentinel;
					else //everything is fine
					{
						char* beginning = p + 4; //should set beginning to where p is pointing?
						setSentinel(*p, -1*spaceNeeded);
						p = p + 4 + spaceNeeded;
						setSentinel(*p, -1*spaceNeeded);
						p = p + 4;
						setSentinel(*p, spaceLeft);
						//p = p + spaceLeft;
						spaceFound = true;
						break; //should break out of entire loop
					}
				}
				//if nothing found, increment p
				p = p + 8 + sentinel;
				
			}
			else
			//don't do anything if the block is in use, skip to the next one
				p = p + 8 + sentinel*-1; 	
				
		}
		
		if (spaceFound != true)
                	throw std::invalid_argument("There was not a block big enough for requested memory");*/	
                		
    		assert(valid());
            	return return reinterpret_cast<pointer>(start);} // replace!

        // ---------
        // construct
        // ---------

        /**
* O(1) in space
* O(1) in time
* <your documentation>
*/
        void construct (pointer p, const_reference v) {
            new (p) T(v); // uncomment!
            assert(valid());}

        // ----------
        // deallocate
        // ----------

        /**
* O(1) in space
* O(1) in time
* <your documentation>
* after deallocation adjacent free blocks must be coalesced
*/
        void deallocate (pointer p, size_type = 0) { 
            	char* location = reinterpret_cast<char*>(p) - 4;
		if (location < a || location > a + N - 8 - sizeof(t))
                	throw std::invalid_argument("Not a valid pointer");
                int beginningsentinel = view(*location);
                
                //if sentinel is not negative, then a pointer to a free block was given	
                if (beginningsentinel > 0)
                	throw std::invalid_argument("Pointer points to a free block. Deallocation not possible."); 
                
                //move block size and 8 spots to see if next free block is free
                location = location + 4 + sentinel*-1 + 4;
                int nextSentinel = view(*location);
                
                if (nextSentinel > 0) //merge
                {
                	location = location - 4 - sentinel*-1 - 4; //move pointer back to the beginning
                	setSentinel(*location, sentinel*-1 + nextSentinel);
                	location = location + 4 + sentinel*-1;
                	setSentinel(*location, sentinel*-1 + nextSentinel);
                } 
                
                else
                {
                	location = location - 4 - sentinel*-1 - 4; //move pointer back to the beginning
                	setSentinel(*location, sentinel*-1);
                	location = location + 4 + sentinel*-1;
                	setSentinel(*location, sentinel*-1);
                }	               			 
            	assert(valid());}

        // -------
        // destroy
        // -------

        /**
* O(1) in space
* O(1) in time
* <your documentation>
*/
        void destroy (pointer p) {
            p->~T(); // uncomment!
            assert(valid());}};

#endif // Allocator_h

