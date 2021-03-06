/*
 * ChainedHashTable.h
 *
 *  Created on: 2011-11-30
 *      Author: morin
 */

#ifndef CHAINEDHASHTABLE_H_
#define CHAINEDHASHTABLE_H_
#include <climits>
#include "DLList.h"
#include "utils.h"
#include "array.h"
#include "ArrayStack.h"

using namespace std;
namespace ods {

template<class T>
class ChainedHashTable {
protected:
	typedef DLList<T> List;
	T null;
	array<List> backingArray; //THIS IS THE BACKING ARRAY
	int n;  //total number of elements in structure
	int dimension; //dimension
	int randomNum; //random number
	int loadFactor;
	static const int w = 32; //sizeof(int)*8;
	void allocTable(int m);
	void resize();
	int hash(T x) { //TODO: ensure this hash function will work properly
		return (unsigned) 1 + ((x % ((1 << dimension)) - 1)); //nice and simple (template classes must have the % operator defined)
	}

public:
	ChainedHashTable();
	virtual ~ChainedHashTable();
	void SetLocalFactor(float f); //I added this here
	bool add(T x);
	T remove(T x);
	T find(T x);
	int size() {
		return n;
	}
	int GetLongestList(); //I also added this
	void clear();
};

/**
 * FIXME:  A copy-constructor for arrays would be useful here
 */
template<class T>
void ChainedHashTable<T>::resize() {
	/*the below code is for testing to see WHEN resize is called
	 * and should not be considered part of the code. This is only
	 * here to make the Test.cpp file work better and since this
	 * assignment is about making the alterations to classes AND
	 * testing them, I assume it is a good thing to do and leave in*/
	cout << endl;
	cout << "!!it is time to resize!!" << endl;
	cout << endl;

	dimension = 1;
	while (1<<dimension <= n) dimension++;
	array<List> newTable(1<<dimension);
	for (int i = 0; i < backingArray.length; i++) {
		for (int j = 0; j < backingArray[i].size(); j++) {
			T x = backingArray[i].get(j);
			newTable[hash(x)].add(x);
		}
	}
	backingArray = newTable;
}

template<class T>
void ChainedHashTable<T>::SetLocalFactor(float f) {
	loadFactor = f;
}

/*
template<>
ChainedHashTable<int>::ChainedHashTable() : t(2)
{
	n = 0;
	d = 1;
	null = INT_MIN;
	z = rand() | 1;     // is a random odd integer
}
*/


template<class T>
ChainedHashTable<T>::ChainedHashTable() : backingArray(2) { //t(2) says we are starting with an array of size 2
	n = 0;
	dimension = 1;
	loadFactor = 1; //initially the loadFactor will be set to 1
	//TODO: the commented out code below does not work because INT_MIN does not exist
	null = INT_MIN;
	randomNum = rand() | 1;     // is a random odd integer
}



template<class T>
ChainedHashTable<T>::~ChainedHashTable() {
}


template<class T>
bool ChainedHashTable<T>::add(T x) {
	/*the below code is used in unison with the other testing code
	 * for the resize function. I needed to know when the resize
	 * function is being called and after how many add functions so
	 * I could know if everything is working properly*/
	cout << "adding " << x <<endl;
	cout << endl;

	if (find(x) != null) return false;
	if ((n+1) / loadFactor > backingArray.length) resize();
	backingArray[hash(x)].add(x);
	n++;
	return true;
}


template<class T>
T ChainedHashTable<T>::remove(T x) {
	int j = hash(x);
	for (int i = 0; i < backingArray[j].size(); i++) {
		T y = backingArray[j].get(i);
		if (x == y) {
			backingArray[j].remove(i);
			n--;
			return y;
		}
	}
	return null;
}


template<class T>
T ChainedHashTable<T>::find(T x) {
	int j = hash(x);
	for (int i = 0; i < backingArray[j].size(); i++)
		if (x == backingArray[j].get(i))
			return backingArray[j].get(i);
	return null;
}


template<class T>
void ChainedHashTable<T>::clear() {
	n = 0;
	dimension = 1;
	DLList<List> b(2);
	backingArray = b;
}

template<class T>
int ChainedHashTable<T>::GetLongestList() {
	int largest = 0;
	for (int i = 0; i < backingArray.length; i++) {
		if(backingArray[i].size() > largest) {
			largest = backingArray[i].size();
		}
	}
	return largest;
}

} /* namespace ods */
#endif /* CHAINEDHASHTABLE_H_ */
