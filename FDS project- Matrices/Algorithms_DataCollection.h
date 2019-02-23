#ifndef _ALGORITHMS_h_
#define _ALGORITHMS_h_
// Header for all algorithms used to manipulate the data.
#include "Matrix.h"
#include "SquareMatrix.h"
#include "DiagnalMatrix.h"

#include <iostream>
#include <cassert>
#include <climits>
#include <vector>
#include <string>

using namespace std;
//-------------------------------------------------------------
// MergeSort Algorithm used to sort the Squarematrices in the vector by their determinant value.
//-------------------------------------------------------------
void merge(vector<SquareMatrix<int>*>& D_Collect, vector<SquareMatrix<int>*>& tmpArray, int leftPos, int rightPos, int rightEnd)
{
	int leftEnd = rightPos - 1;
	int tmpPos = leftPos;
	int numElements = rightEnd - leftPos + 1;

	while (leftPos <= leftEnd && rightPos <= rightEnd) // Main loop
		if (D_Collect[leftPos]->determinant() <= D_Collect[rightPos]->determinant())
			tmpArray[tmpPos++] = D_Collect[leftPos++];
		else
			tmpArray[tmpPos++] = D_Collect[rightPos++];

	while (leftPos <= leftEnd) // Copy rest of first half
		tmpArray[tmpPos++] = D_Collect[leftPos++];

	while (rightPos <= rightEnd) // Copy rest of right half
		tmpArray[tmpPos++] = D_Collect[rightPos++];

	for (int i = 0; i < numElements; i++, rightEnd--) // Copy tmpArray back
		D_Collect[rightEnd] = tmpArray[rightEnd];
}

void mergeSort(vector<SquareMatrix<int>*>& D_Collect, vector<SquareMatrix<int>*>& tmpArray, int left, int right)
{
	if (left < right) {
		int center = (left + right) / 2;
		mergeSort(D_Collect, tmpArray, left, center);
		mergeSort(D_Collect, tmpArray, center + 1, right);
		merge(D_Collect, tmpArray, left, center + 1, right);
	}
}
void mergeSort(vector<SquareMatrix<int>*>& D_Collect)
{
	vector<SquareMatrix<int>*> tmpArray(D_Collect.size());
	mergeSort(D_Collect, tmpArray, 0, D_Collect.size() - 1);
}
void output_sorting(vector<SquareMatrix<int>*>& a) {
	for (int i = 0; i < a.size(); i++)
	{
		cout << i + 1 << ". " << a[i]->getname() << " ---> " << a[i]->determinant() << endl;
	}
	cout << endl;
}
//-------------------------------------------------------------
//  Quick sort used to order the elements of a matrix(stored in a vector) in ascending order.
//-------------------------------------------------------------
void Quick_Sort(vector <int> & Sample_Vector, int left, int right)
{
	if (left >= right)
		return;
	int pivotValue = Sample_Vector[left], index = left;
	swap(Sample_Vector[left], Sample_Vector[right]);
	for (int i = left; i < right; i++)
	{
		if (Sample_Vector[i] < pivotValue)
		{
			swap(Sample_Vector[i], Sample_Vector[index]);
			index += 1;
		}
	}
	swap(Sample_Vector[right], Sample_Vector[index]);
	Quick_Sort(Sample_Vector, left, index - 1);
	Quick_Sort(Sample_Vector, index + 1, right);
}
void Quick_Sort(vector <int> & Sample_Vector)
{
	Quick_Sort(Sample_Vector, 0, Sample_Vector.size() - 1);
}
//-------------------------------------------------------------
// Binary Search is used find a value or entry in a matrix.
//-------------------------------------------------------------
int binary_search(vector <int> & Sample_Vector, int first, int last, int search_key)
{
	int index;

	if (first > last)
		index = -1;

	else
	{
		int mid = (first + last) / 2;

		if (search_key == Sample_Vector[mid])
			index = mid;
		else

			if (search_key < Sample_Vector[mid])
				index = binary_search(Sample_Vector, first, mid - 1, search_key);
			else
				index = binary_search(Sample_Vector, mid + 1, last, search_key);

	} // end if
	return index;
}// end binarySearch
int binary_search(vector <int> & Sample_Vector, int key) {
	return binary_search(Sample_Vector, 0, Sample_Vector.size() - 1, key);
}


#endif //Algorithms
