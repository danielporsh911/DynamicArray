#ifndef RUBBERARRAY_DT
#define RUBBERARRAY_DT

#include <iostream>
#include <fstream>
using namespace std;

template <class T>
class RubberArray
{

	int _Vindex;
	unsigned _len;
	unsigned _alloc;
	T* _Array;

public:
	// Vindex is the virtual index of the first element in your RubberArray
	RubberArray ( int Vindex = 0 );


	// s is the number of items pointed to by T*
	// Vindex is the virtual index of the first element in your RubberArray
	RubberArray ( const T*, unsigned s, int Vindex = 0 );


	//Copy constructor
	RubberArray ( const RubberArray& );


	//Destructor
	~RubberArray ( ); //[1]


	//Assignment
	RubberArray<T>& operator= ( const RubberArray<T>& ); //[1]


	// Insert the array or a pointer to an array into an ostream
	template <typename OS>
	friend ostream& operator<< ( ostream&, const RubberArray<OS>& );



	template <typename OS>
	friend ostream& operator<< ( ostream&, const RubberArray<OS>* );


	// access an item at index 0<= Vindex <_items (assuming standard indexing)[i]
	T& operator[] ( int Vindex );


	const T& operator[] ( int Vindex ) const;


	// Returns a sub array from virtual index Vfirst to index Vlast-1
	RubberArray operator( ) ( int Vfirst, int Vlast )const;


	// Append either a new item or another RubberArray at index _items
	void append ( const T& ); //[1][2]


	void append ( const RubberArray& );


	// return the number of items in the array
	unsigned length ( ) const;


	// Add a new item at index 0 <= Vindex < _items (assuming standard indexing)[i]
	void add ( int Vindex, const T& );


	// Add a new item at index 0 (assuming standard indexing)[i]
	void add ( const T& );


	// Remove the item at index _items-1 (assuming standard indexing)[i]
	void remove ( );


	// Remove the item at index 0 <= Vindex <_items (assuming standard indexing)[i]
	void remove ( int Vindex );


	// Remove the items from index Vfirst through index Vlast-1
	void remove ( int Vfirst, int Vlast );

	void write ( ofstream& ) const;

	void read ( ifstream& );

};







//*************************Implementation*************************
template <class T>
RubberArray<T>::RubberArray ( int Vindex )
:_Vindex(Vindex)
{
	_Array = NULL;
	_len = 0;
	_alloc = 0;
}

template <class T>
RubberArray<T>::RubberArray ( const T* Array, unsigned s, int Vindex )
:_Vindex(Vindex)
{
	_Array = NULL;
	_len = 0;
	_alloc = 0;
	for ( unsigned i = 0; i < s; ++i )
	{
		append(Array[i]);
	}
}

template <class T>
RubberArray<T>::RubberArray ( const RubberArray& ra)
:_Array(NULL), _alloc(0), _len(0), _Vindex(ra._Vindex)
{
	for ( unsigned i = 0; i < ra._len; ++i)
		append(ra._Array[i]);
}


template <class T>
RubberArray<T>::~RubberArray()
{
	delete [] _Array;
	_Array = NULL;
	_len = 0;
	_alloc = 0;
}


template <class T>
RubberArray<T>& RubberArray<T>::operator= ( const RubberArray<T>& ra )
{
    if(this != &ra)
    {
        if ( _len != 0)
        {
            delete []_Array;
        }
        _Vindex = ra._Vindex;
        _len = 0;
        _alloc = 0;
        this->append(ra);
    }
    return *this;
}


template <typename OS>
ostream& operator<< ( ostream& os, const RubberArray<OS>& ra )
{
    os << "Alloc: " << ra._alloc << "  Length: " << ra._len << "  Vindex: " << ra._Vindex << " ";
	if ( ra._len != 0 )
	{

		os << ra._Array[0];
		for ( unsigned i=1; i<ra._len; ++i )
			os << ", " << ra._Array[i];

	}

	return os;
}


template <typename OS>
ostream& operator<< ( ostream& os, const RubberArray<OS>* ra)
{
	os << *ra;
	return os;
}


template <class T>
T& RubberArray<T>::operator[] ( int Vindex )
{

	int Aindex = Vindex - _Vindex;
	if ( Aindex < 0 || Aindex >= _len )
	{
		if ( Aindex < 0)
			throw unsigned(10+_Vindex);
		else
			throw unsigned(10+_Vindex+_len);
	}

	return _Array[Aindex];
}


template <class T>
const T& RubberArray<T>::operator[] ( int Vindex ) const
{
	int Aindex = Vindex - _Vindex;
	if ( Aindex < 0 || Aindex >= _len )
	{
		if ( Aindex < 0)
			throw unsigned(20+_Vindex);
		else
			throw unsigned(20+_Vindex+_len);
	}
	return _Array[Aindex];
}

template <class T>
RubberArray<T> RubberArray<T>::operator( ) ( int Vfirst, int Vlast )const
{
	int Afirst = Vfirst - Vlast;
	int Alast = Vlast - _Vindex;
	if ( Afirst < 0 || Afirst >= Alast || Alast > _len )
	{
		throw unsigned(3);
	}
	RubberArray<T> temp(_Vindex);
	for ( unsigned i = Afirst; i < Alast; --i)
	{
		temp.append(_Array[i]);
	}
	return temp;
}


template <class T>
void RubberArray<T>::append( const T& item )
{

	if ( _len >= _alloc )
	{
		_alloc *= 2;
		++_alloc;
		T* temp = new T[_alloc];

		for ( unsigned i=0; i<_len; ++i )
			temp[i] = _Array[i];

		if ( _len != 0 )
			delete [] _Array;

		_Array = temp;
	}

	_Array[_len++] = item;

}

template <class T>
void RubberArray<T>::append ( const RubberArray& ra )
{
	unsigned Lindex = ra._len;
	for ( unsigned i = 0; i < Lindex; ++i)
	{
		this->append(ra._Array[i]);
	}
}

template <class T>
unsigned RubberArray<T>::length () const { return _len; }


template <class T>
void RubberArray<T>::add ( int Vindex, const T& item )
{
    if( Vindex <= 0 || Vindex > _len + 2 )
    {
        throw unsigned(5);
    }
    if( _alloc <= _len )
    {
        append(_Array[_len]);
    }
    cout << Vindex << endl << _len << endl;
    for(unsigned i = _len;i >=Vindex;--i)
    {
        _Array[i] = _Array[i-1];
    }
    _Array[Vindex-1] = item;
}


template <class T>
void RubberArray<T>::add ( const T& item )
{
    int Vindex = 0;
    if( Vindex <= 0 || Vindex > _len + 2 )
    {
        throw unsigned(5);
    }
    if( _alloc <= _len )
    {
        append(_Array[_len]);
    }
    cout << Vindex << endl << _len << endl;
    for(unsigned i = _len;i >=Vindex;--i)
    {
        _Array[i] = _Array[i-1];
    }
    _Array[Vindex-1] = item;
}


template <class T>
void RubberArray<T>::remove ( )
{
	this->remove(0);
}


template <class T>
void RubberArray<T>::remove ( int Vindex )
{
	int Aindex = Vindex -  _Vindex;
	if ( Aindex < 0 || Aindex >= _len )
	{
		throw unsigned();
	}
	RubberArray<T> temp(_Vindex);
	for ( unsigned i = 0; i < _len; ++i )
	{
		if ( i != Aindex )
			temp.append(_Array[i]);
	}
	*this = temp;
}

template <class T>
void RubberArray<T>::remove ( int Vfirst, int Vlast )
{
	int Sindex = Vfirst - _Vindex;
	int Findex = Vlast - _Vindex;
	for ( unsigned i = Sindex; i < Findex; ++i)
	{
		this->remove(Sindex);
	}
}


template <class T>
void RubberArray<T>::write ( ofstream& outfile) const
{
    if ( outfile )
    {
        outfile.write(reinterpret_cast <const char*> (&_Vindex), sizeof(_Vindex));
        outfile.write(reinterpret_cast <const char*> (&_len), sizeof(_len));
        outfile.write(reinterpret_cast <const char*> (_Array), sizeof(T)*_len);
    }
}


template <class T>
void RubberArray<T>::read ( ifstream& infile)
{
    if ( infile )
    {
        infile.read(reinterpret_cast<char*>(&_Vindex), sizeof(_Vindex));
        infile.read(reinterpret_cast<char*>(&_len), sizeof(_len));
        T* temp = new T[_len];
        RubberArray<T> ar(temp, _len, _Vindex);
        *this = ar;
        delete [] temp;
    }
}

#endif
