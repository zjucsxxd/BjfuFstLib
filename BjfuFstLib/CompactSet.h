#pragma once
namespace bjfufst{

	//sequenced compact dynamic set.
	//ONLY support insert and read.
	template <class T>
	class CompactSet
	{
	public:
		CompactSet();
		virtual ~CompactSet();

		//get size of array
		size_t Size();

		//insert an element at tail of array
		size_t Insert(const T& elem);

		//get an element from position
		const T& GetAt(size_t pos);

		//array-style operator,get an element from position
		const T& operator[](size_t pos);

		//find an elem and return its pos.
		//return size of array if not found.
		size_t Find(const T& elem);

	private:

		//length of array
		size_t arr_length;

		//element pointers
		T * * pElems_arr;
	};



	template < class T >
	CompactSet<T>::CompactSet()
		:arr_length(0), pElems_arr(nullptr)
	{

	}

	template <class T>
	CompactSet<T>::~CompactSet()
	{
		for (int i = 0; i < arr_length; i++)
		{
			delete pElems_arr[i];
		}
		delete[]pElems_arr;
	}


	template <class T>
	const T& CompactSet<T>::GetAt(size_t pos)
	{
		if (pos < 0 || pos >= arr_length)
		{
			throw std::out_of_range("OOPS, CompactSet OUT OF RANGE error.");
		}
		return *(pElems_arr[pos]);
	}

	template <class T>
	size_t CompactSet<T>::Insert(const T& elem)
	{
		if (pElems_arr == nullptr)//if empty, insert one single element and return initial location, that is zero.
		{
			arr_length = 1;
			pElems_arr = new  T*[arr_length];
			pElems_arr[0] = new T(elem);
			return 0;
		}
		else if ((size_t pos=Find(elem))!=arr_length)//if elem already exists in array, do no modification and return position of elem.
		{
			return pos;
		} 
		else//if not empty, copy whole ptr array and append one elem.
		{
			
			size_t old_size = arr_length;
			T** old_array = pElems_arr;//"ptr reserving technology"

			pElems_arr = new T*[++arr_length];//new array which is 1 elem larger.
			for (size_t i = 0; i < old_size; i++)//move pointers from old array to new one.
			{
				pElems_arr[i] = old_array[i];
			}
			delete[]old_array;
			pElems_arr[arr_length - 1] = new T(elem);
			return arr_length - 1;
		}
	}

	template <class T>
	size_t CompactSet<T>::Size()
	{
		return arr_length;
	}


	template <class T>
	const T& bjfufst::CompactSet<T>::operator[](size_t pos)
	{
		return GetAt(pos);
	}

	template <class T>
	size_t bjfufst::CompactSet<T>::Find(const T& elem)
	{
		for (size_t i = 0; i < arr_length;i++)
		{
			if (*pElems_arr[i]==elem)
			{
				return i;
			}
		}
		return i;
	}
}

//#include "CompactArray.cpp"
	