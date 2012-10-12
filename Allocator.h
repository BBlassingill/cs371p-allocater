//Fixed pointer casts and deallocate method

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
#include <iostream> //output
using namespace std;
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
        //view
	//@brief Based off of quiz 13, useful in getting sentinel values
	//@param c a reference to a character in the array
	//@return the integer representation of the sentinel
        // -----------
	int& view (char& c) const
	{
		return *reinterpret_cast<int*>(&c);
	}

	// -----------
        //view
	//@brief Based off of quiz 13, useful in getting sentinel values
	//@param c a reference to a character in the array
	//@return the integer representation of the sentinel
        // -----------
	int& view (char& c) 
	{
		return *reinterpret_cast<int*>(&c);
	}

	// -----------
        //@brief Reverse of view, useful in setting sentinal values
	//@param c a reference to a character in the array
	//@param value the value you want to store in the array
        // -----------
	void setSentinel (char& c, int value)
	{
		*(reinterpret_cast<int*>(&c)) = value;
	}

    public:
        // ----
        // data
        // ----

        char a[N];
	//char* p = a; //set a char pointer to the beginning of the array
	//illegal apparently
	
	
        // -----
        // valid
        // -----

        /**
	* O(1) in space
	* O(n) in time
	* @brief An allocator is valid if all the sentinel values match up
	* @return true if all the sentinels match up, false otherwise
	*/

        bool valid () const 
	{

        	char* maxAddress = (char*) a + N;
        	char* p = (char*) a;
		int beginningSentinel = 0;
		int endingSentinel = 0;

		while (p < maxAddress)
		{
			beginningSentinel = view(*p);
			if (beginningSentinel < 0) //ie, we found an occupied space
			{
				p = p + sizeof(int) + (beginningSentinel* -1);				
				endingSentinel = view(*p);
				if (beginningSentinel != endingSentinel)
					return false;
			}

			else
			{	
				p = p + sizeof(int) + beginningSentinel;				
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
	* @brief Constructs an Allocator object
	* @return An Allocator object
	*/
        Allocator () 
	{
            	int requiredSpace = 8 + sizeof(T);
		if (requiredSpace > N)
			throw std::invalid_argument("Create another allocator with appropriate size N.");
		int sentinel = N - 8;
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
	* @brief Allocates memory and returns a pointer to the start of the memory block
	* @param num The number of objects to be allocated
	* @return A pointer to the start of the memory block allocated
	* after allocation there must be enough space left for a valid block
	* the smallest allowable block is sizeof(T) + (2 * sizeof(int))
	* choose the first block that fits
	*/
        pointer allocate (size_type n) {
        	if (n == 0)
        		throw std::invalid_argument("Needs at least one element to allocate.");
            	int spaceNeeded = n*sizeof(T);
		assert(spaceNeeded > 0);
		int maxSpace = N - 8;
		int sentinel = 0;
		int minSpaceRequired = sizeof(T) + 8;
		int spaceLeft = 0;
		if (spaceNeeded > maxSpace)
			throw std::invalid_argument("Not enough space for requested memory.");
	    	bool spaceFound = false;
	    	bool hadtoMerge = false;
    		char* p = (char*) a;
		char* start;
		char* maxAddress = (char*) a + N;

		
		while (p < maxAddress && spaceFound == false)
		{
			sentinel = view(*p);
			if (sentinel >= spaceNeeded)
			{
				spaceFound = true;
				start = p + 4;
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
                		
                		spaceLeft = sentinel - 8 - spaceNeeded;
                		if (spaceLeft < minSpaceRequired) //should do a merge
                		{
                			spaceNeeded = sentinel; //space we're working with is now equal to a valid size
                			spaceLeft = 0;
                			hadtoMerge = true;
                		}
                		

                		setSentinel(*p, -1*spaceNeeded);
                		p = p + 4 + spaceNeeded;
                		setSentinel(*p, -1*spaceNeeded); 
\
                		
                		//we need to update the rest of the blocks if there's still space left
                		if (spaceLeft > 0 && hadtoMerge == false)
                		{
                			p = p + 4; //should get to the next block
                			setSentinel(*p, spaceLeft);
                			p = p + 4 + spaceLeft;
                			setSentinel(*p, spaceLeft);
                		}               		           
                		
                	}				
		

    		assert(valid());
            	return reinterpret_cast<pointer>(start);} // replace!

        // ---------
        // construct
        // ---------

        /**
	* O(1) in space
	* O(1) in time
	* @brief Writes value v to the address pointed at by p
	* @param p A pointer to a memory address
	* @param v The value that will be stored at p
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
	* @brief Deallocates memory at the given address p
	* @param p The memory address to be deallocated
	* after deallocation adjacent free blocks must be 	coalesced
	*/
        void deallocate (pointer p, size_type = 0) {
            	char* location = reinterpret_cast<char*>(p) - 4;
		char* startLocation = reinterpret_cast<char*>(p) - 4;
		char* max = a + N;
		char* min = a;
		if (location < min || location > (max - 8 - sizeof(T)))
                	throw std::invalid_argument("Not a valid pointer");
                int beginningSentinel = view(*location);
		bool mergeDone = false;
                if (beginningSentinel > 0)
                	throw std::invalid_argument("Pointer points to a free block. Deallocation not possible."); 
                
                //move block size and 8 spots to see if next free block is free
                location = location + 4 + beginningSentinel*-1 + 4;

		//FORWARD MERGE
		if (location >= min && location < max) //valid location
		{
			int nextSentinel = view(*location);
                	if (nextSentinel > 0) //merge
                	{
				location = location - 4 - beginningSentinel*-1 - 4; //move pointer back to the beginning
                		setSentinel(*location, beginningSentinel*-1 + nextSentinel + 4 + 4);


                		location = location + 8 + beginningSentinel*-1 + nextSentinel + 4;
                		setSentinel(*location, beginningSentinel*-1 + nextSentinel + 4 + 4);

				mergeDone = true;

				//check to see if you can merge above
				location = startLocation; //move pointer back to the beginning
				beginningSentinel = view(*location);
				location = location - 4;

				if (location >= min && location < max) //valid location
				{
					int previousSentinel = view(*location); 
					if (previousSentinel > 0) //BACKWARD MERGE
					{
						location = startLocation;
						location = location - 4 - previousSentinel - 4; //points to the beginning of the previous free block
						setSentinel(*location, beginningSentinel + previousSentinel + 4 + 4);
						location = location + 8 + beginningSentinel + previousSentinel + 4;
						setSentinel(*location, beginningSentinel + previousSentinel + 4 + 4);
						mergeDone = true;
					}
                		}
			}
		}

		//see if you can go backwards
		if (mergeDone == false)
		{

			location = startLocation; //move pointer back to the beginning
			beginningSentinel = view(*location);
			location = location - 4;		
			if (location >= min && location < max) //valid location
				{
					int previousSentinel = view(*location);

					if (previousSentinel > 0) //BACKWARD MERGE
					{
						location = startLocation;
						location = location - 4 - previousSentinel - 4; //points to the beginning of the previous free block
						setSentinel(*location, beginningSentinel*-1 + previousSentinel + 4 + 4);
						location = location + 8 + beginningSentinel*-1 + previousSentinel + 4;
						setSentinel(*location, beginningSentinel*-1 + previousSentinel + 4 + 4);
						mergeDone = true;
					}
                		}	
		}

		//if after all that, mergeDone is still false, just change sentinels
		if (mergeDone == false)
		{

			location = startLocation; //move pointer back to the beginning
			setSentinel(*location, beginningSentinel*-1);
			location = location + 4 + beginningSentinel*-1;
			setSentinel(*location, beginningSentinel*-1);			
		}	               			 
            	assert(valid());}

        // -------
        // destroy
        // -------

        /**
	* O(1) in space
	* O(1) in time
	* @brief Runs T's deconstructor at address p
	* @param p The address to be destroyed
	*/
        void destroy (pointer p) {
            p->~T(); // uncomment!
            assert(valid());}};

#endif // Allocator_h

