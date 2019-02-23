#ifndef _DIAGONALMATRIX_H_
#define _DIAGONALMATRIX_H_

#include "Matrix.h"
#include "SquareMatrix.h"
#include <iostream>
#include <cassert>
#include <climits>
#include <vector>
#include <string>
#include <cmath>

using namespace std;
template < class TYPE > class DiagonalMatrix : public SquareMatrix<int>
{	
private:
	string Invertibility; //I or NI
public:
	DiagonalMatrix() : SquareMatrix() //default constructor
	{
		allocate(0, 0);
	}
	//Constructor using rows and columns
	DiagonalMatrix(unsigned rowsParameter, unsigned columnsParameter, string name, string size, string Invertibility) : SquareMatrix(rowsParameter, columnsParameter, name, size) {
		this->Invertibility = Invertibility;
		if (rows != columns) {
			cout << "Invalid matrix size.The number of rows and columns should be equal! " << endl;
		}
		TYPE const ZERO = static_cast<TYPE>(0);

		// Allocate memory for new matrix.
		allocate(rows, columns);

		// Fill matrix with zero.
		for (unsigned row = 0; row < rows; ++row)
		{
			order[row] = row;

			for (unsigned column = 0; column < columns; ++column)
				matrix[row][column] = ZERO;
		}
	}
	DiagonalMatrix & operator = (TYPE const * data)
	{
		unsigned index = 0;

		for (unsigned row = 0; row < rows; ++row)
			for (unsigned column = 0; column < columns; ++column)
				matrix[row][column] = data[index++];
			return *this;
	}
	void reducedRowEchelon() //virtual
	{
		TYPE const ZERO = static_cast<TYPE>(0);
		// For each row...
		for (unsigned rowIndex = 0; rowIndex < rows; ++rowIndex)
		{
			// Reorder the rows.
			reorder();
			unsigned row = order[rowIndex];
			// Divide row down so first term is 1.
			unsigned column = getLeadingZeros(row);
			TYPE divisor = matrix[row][column];
			if (ZERO != divisor)
			{
				divideRow(row, divisor);
			}
		} // reducedRowEcholon
	}
	void getDiagonal()
	{
		for (int i = 0; i < rows; i++) {
			cout << matrix[i][i] << " ";
		}
		cout << endl;
	}
	DiagonalMatrix Diagonalization(SquareMatrix <int>& smatrix)
	{

		matrix[0][0]=smatrix.get(0, 0);
		matrix[1][1] = smatrix.get(1,1);
		matrix[2][2]= smatrix.get(2,2);
		return *this;
	}
	DiagonalMatrix Powers_of_DMatrix(unsigned exp) {
		for (int i = 0; i < matrix.size(); i++) {
			matrix[i][i] = pow(matrix[i][i], exp);
		}
		return *this;
	}
	void getInvertibility() {
		if (Invertibility == "NI")
			cout << "DiagonalMatrix is not invertible " << endl;
		else if (Invertibility == "I")
			cout << "Diagonal matrix is invertible" << endl;
		else cout << "Invalid entry. Please insert NI(non-invertible) or I(invertible)!" << endl;
	}
};
#endif //DiagonalMatrix
