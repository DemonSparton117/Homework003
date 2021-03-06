/*
 * LinearHashTable.h
 *
 *  Created on: 2011-12-16
 *      Author: morin
 */

#ifndef LINEARHASHTABLE_H_
#define LINEARHASHTABLE_H_
#include <climits>
#include <iostream>
using namespace std;
#include "array.h"
namespace ods {

//a change here

extern unsigned int tab[4][256];

template<class T>
class LinearHashTable {

	static const int w = 32;
	static const int r = 8;
	array<T> t; //THIS IS THE BACKING ARRAY
	array<T> t2; //THIS IS THE SECOND BACKING ARRAY
	int n;   // number of values in T
	int q;   // number of non-null entries in T (n + # of del)
	int d;   // t.length = 2^d /2 = 2^(d-1)
	T null, del;
	void resize();//check
	int hash(T x) { /*TODO: change things so that the size of the backing arrays depends on d at all times
	then the size of each backing array will be half of 2^d so that the total size of our hash
	table will be 2^d*/
		int tester = (unsigned) 1 + (x % ((1 << d) - 1));
		return (unsigned) 1 + (x % ((1 << d) - 1)); //TODO: make this a double hash function
	}

	/*the below function will determine where to put elements in the second backing array
	 * whenever something is hashed to the second backing array*/
	int adjust(int num, int length) {
		if(num < length) {
			return num;
		}
		return num - length;
	}
	// Sample code for the book only -- never use this
	/*
	int idealHash(T x) {
		return tab[hashCode(x) >> w-d];
	}
	*/


public:
	// FIXME: get rid of default constructor
	LinearHashTable();
	LinearHashTable(T null, T del); // this constructor allows you to set null and del to whatever you want that is of the template class
	/*This is the constructor we want to use because it is the one that allows us to set the null and del variables
	 * which will allow the program to properly run*/
	virtual ~LinearHashTable();
	bool add(T x); //check
	bool addSlow(T x); // check
	T remove(T x); // check
	T find(T x); //check
	int size() { return n; }
	void clear();
	// FIXME: yuck
	void setNull(T null) { this->null = null; t.fill(null), t2.fill(null); }
	void setDel(T del) { this->del = del; }
};

/*
template<>
LinearHashTable<int>::LinearHashTable() : t(2, INT_MIN) {
	null = INT_MIN;
	del = INT_MIN + 1;
	n = 0;
	q = 0;
	d = 1;
}
*/

/**
 * FIXME: Dangerous - leaves null and del uninitialized
 */
template<class T>
LinearHashTable<T>::LinearHashTable() : t(1), t2(1) {
/*
	this->null = null;
	this->del = del;
*/
	n = 0;
	q = 0;
	d = 1;
}


template<class T>
LinearHashTable<T>::LinearHashTable(T null, T del) : t(1, null), t2(1, null) {
	this->null = null;
	this->del = del;
	n = 0;
	q = 0;
	d = 1;
}

template<class T>
LinearHashTable<T>::~LinearHashTable() {
}

template<class T>
void LinearHashTable<T>::resize() { //FIXME: I am pretty sure the problem lies here somewhere
	//FIXME:need to adjust this method if it does not work
	// will also need to adjust the add, find, and most other functions
	cout << endl << endl << "!!Resizing!!" << endl << endl;
	d = 1;
	while ((1<<d) < 3*n) d++;
	array<T> tnew((1<<d) / 2, null);
	array<T> t2new((1<<d) / 2, null);
	q = n;
	T data;
	int index = 0;
	// insert everything into tnew and re-hashes everything
	for (int k = 0; k < t.length + t2.length; k++) {

		if(k < t.length) {
			index = k;
			data = t[index];
		}
		else {
			index = adjust(k, t.length);
			data = t2[index];
		}
		//the above piece figures out what piece of data we are going to put in the new hash table

		if (data != null && data != del) {
			index = hash(data);
			if(index <= tnew.length) {
				while (tnew[index] != null)
					index = (index == tnew.length-1) ? 0 : index + 1; //increment i (but for the new (re-sized) hash table)
				tnew[index] = data;
			}
			else {
				index = adjust(index, tnew.length);
				while (t2new[index] != null)
					index = (index == t2new.length-1) ? 0 : index + 1; //increment i (but for the new (re-sized) hash table)
								t2new[index] = data;
			}
		}
	}
	t = tnew;
	t2 = t2new;
}

template<class T>
void LinearHashTable<T>::clear() {
	n = 0;
	q = 0;
	d = 1;
	array<T> tnew(2, null);
	t = tnew;
}

template<class T>	//TODO: edit this function to consider the two backing arrays
bool LinearHashTable<T>::add(T x) {

	if (find(x) != null) return false; //(returning false because you already have the item you are trying to add)

	if (2*(q+1) > 1<<d) resize();   // max 50% occupancy (because it will resize once it is half full)

	int index = hash(x);
	int i = index;

	if(i >= t.length) {
		i = adjust(index, t.length);
		while (t2[i] != null && t2[i] != del)
			i = (i == t2.length-1) ? 0 : i + 1; // increment i

		if (t2[i] == null) q++;

		n++;
		t2[i] = x;
	}//if(i >= t.length)

	else {
		while (t[i] != null && t[i] != del)
			i = (i == t.length-1) ? 0 : i + 1; // increment i

		if (t[i] == null) q++;

		n++;
		t[i] = x;
	}//else
	return true;
}

template<class T> //TODO: edit this function to consider the two backing arrays
T LinearHashTable<T>::find(T x) {
	int index = hash(x);
	int i = index;
	if(i >= t.length) {
		i = adjust(index, t.length);
		while (t2[i] != null) {

			if (t2[i] != del && t2[i] == x) return t2[i];

			i = (i == t2.length-1) ? 0 : i + 1; // increment i
			if(t.length -1 == 0) {
				break;
			}
		}//while
	} //if(i >= t.length)
	else {
		while (t[i] != null) {

			if (t[i] != del && t[i] == x) return t[i];

			i = (i == t.length-1) ? 0 : i + 1; // increment i unless it is too big
			if(t.length -1 == 0) {
							break;
						}
		}//while
	}//else
	return null;
}

template<class T> //TODO: edit this function to consider the two backing arrays
T LinearHashTable<T>::remove(T x) {
	int index = hash(x);
	if(index < t.length) {
		while (t[index] != null) {
			T y = t[index];
			if (y != del && x == y) {
				t[index] = del;
				n--;
				if (8*n < t.length) resize(); // min 12.5% occupancy
				return y;
			}
			index = (index == t.length-1) ? 0 : index + 1;  // increment i
		}
	}//if index is small enough
	else {
		index = adjust(index, t.length);
		while (t2[index] != null) {
					T y = t2[index];
					if (y != del && x == y) {
						t2[index] = del;
						n--;
						if (8*n < t2.length) resize(); // min 12.5% occupancy
						return y;
					}
					index = (index == t2.length-1) ? 0 : index + 1;  // increment i
				}
	}

	return null;
}

template<class T>
bool LinearHashTable<T>::addSlow(T x) {
	if ((q+1) > t.length) resize();   // max 50% occupancy
	int index = hash(x);
	if(index < t.length) {
		while (t[index] != null) {
			if (t[index] != del && x == t[index]) return false;
			index = (index == t.length-1) ? 0 : index + 1; // increment i
		}
		t[index] = x;
	}

	else {
		index = adjust(index, t.length);
		while (t2[index] != null) {
					if (t2[index] != del && x == t[index]) return false;
					index = (index == t2.length-1) ? 0 : index + 1; // increment i
				}
				t2[index] = x;
	}

	n++; q++;
	return true;
}


} /* namespace ods */
#endif /* LINEARHASHTABLE_H_ */
