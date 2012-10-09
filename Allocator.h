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
	//Useful in getting setting sentinel values
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
				p = p + sizeof(int) + sizeof(T)*(beginningSentinel* -1)				
				endingSentinel = view(*p);
				if (beginningSentinel != endingSentinel)
					return false;
			}

			else
			{	
				p = p + sizeof(int) + sizeof(T)* beginningSentinel				
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
        Allocator () {
            // <your code>
            assert(valid());}

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
            // <your code>
            assert(valid());
            return 0;} // replace!

        // ---------
        // construct
        // ---------

        /**
* O(1) in space
* O(1) in time
* <your documentation>
*/
        void construct (pointer p, const_reference v) {
            // new (p) T(v); // uncomment!
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
            // <your code>
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
            // p->~T(); // uncomment!
            assert(valid());}};

#endif // Allocator_h
