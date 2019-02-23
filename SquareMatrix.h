#ifndef _SQUAREMATRIX_H_
#define _SQUAREMATRIX_H_

#include "Matrix.h"
#include <iostream>
#include <cassert>
#include <climits>
#include <vector>
#include <string>

using namespace std;
template<class TYPE > class SquareMatrix :public Matrix<int>
{
private:
	string size;
	string name;
public:
	SquareMatrix() : Matrix() //default constructor
	{
		allocate(0, 0);
	}
	//Constructor using rows and columns
	SquareMatrix(unsigned rowsParameter, unsigned columnsParameter, string Name, string size) : Matrix(rowsParameter, columnsParameter)
	{
		rows = rowsParameter;
		columns = columnsParameter;
		name = Name;
		this->size =size;
		if (rows != columns) {
			cout << "Invalid matrix size.The number of rows and columns should be equal! " << endl;
		}
		else {
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
	}
	// Constructor used for concentration. Used only for the Inverse.
	SquareMatrix(SquareMatrix const & copyMatrixA, SquareMatrix const & copyMatrixB, Position position = TO_RIGHT) :Matrix(copyMatrixA, copyMatrixB, TO_RIGHT)
	{
		unsigned rowOffset = 0;
		unsigned columnOffset = 0;

		if (TO_RIGHT == position)
			columnOffset = copyMatrixA.columns;
		else
			rowOffset = copyMatrixA.rows;

		rows = copyMatrixA.rows + rowOffset;
		columns = copyMatrixA.columns + columnOffset;

		// Allocate memory for new matrix.
		allocate(rows, columns);

		for (unsigned row = 0; row < copyMatrixA.rows; ++row)
			for (unsigned column = 0; column < copyMatrixA.columns; ++column)
				matrix[row][column] = copyMatrixA.matrix[row][column];

		for (unsigned row = 0; row < copyMatrixB.rows; ++row)
			for (unsigned column = 0; column < copyMatrixB.columns; ++column)
				matrix[row + rowOffset][column + columnOffset] =
				copyMatrixB.matrix[row][column];
	}
	//Constructor to create a matrix out of another one. Ommitting rows or columns
	SquareMatrix(SquareMatrix const & copyMatrix, unsigned omittedRow = INT_MAX, unsigned omittedColumn = INT_MAX) :Matrix(copyMatrix, INT_MAX, INT_MAX)
	{
		rows = copyMatrix.getRows();
		columns = copyMatrix.getColumns();
		if (INT_MAX != omittedRow)
			rows--;
		if (INT_MAX != omittedColumn)
			columns--;
		// Allocate memory for new matrix.
		allocate(rows, columns);
		unsigned rowIndex = 0;
		for (unsigned row = 0; row < rows; ++row)
		{
			if (rowIndex == omittedRow)
				rowIndex++;

			order[row] = row;
			unsigned columnIndex = 0;
			for (unsigned column = 0; column < columns; ++column)
			{
				if (columnIndex == omittedColumn)
					columnIndex++;

				matrix[row][column] = copyMatrix.matrix[rowIndex][columnIndex];

				columnIndex++;
			}
			++rowIndex;
		}

	}
	SquareMatrix getSubMatrix(unsigned startRow, unsigned endRow, unsigned startColumn, unsigned endColumn, unsigned const * newOrder = NULL)
	{
		SquareMatrix subMatrix(endRow - startRow + 1, endColumn - startColumn + 1,"","");
		for (unsigned row = startRow; row <= endRow; ++row)
		{
			unsigned subRow;
			if (NULL == newOrder)
				subRow = row;
			else
				subRow = newOrder[row];
			for (unsigned column = startColumn; column <= endColumn; ++column)
				subMatrix.matrix[row - startRow][column - startColumn] =
				matrix[subRow][column];
		}
		return subMatrix;
	}
	SquareMatrix & operator =
		(
			TYPE const * data
			)
	{
		unsigned index = 0;

		for (unsigned row = 0; row < rows; ++row)
			for (unsigned column = 0; column < columns; ++column)
				matrix[row][column] = data[index++];

		return *this;
	}

	//Virtual function which is changed and simplified in the SquareMatrix CLass
	void reducedRowEchelon() 
	{
			TYPE const ZERO = static_cast<TYPE>(0);
			for (unsigned IndexR = 0; IndexR < rows; ++IndexR)
			{
				reorder();
				unsigned row = order[IndexR];
				unsigned column = getLeadingZeros(row);
				TYPE divisor = matrix[row][column];
				if (ZERO != divisor)
				{
					divideRow(row, divisor);
					for (unsigned subRowIndex = (IndexR + 1); subRowIndex < rows; ++subRowIndex)
					{
						unsigned subRow = order[subRowIndex];
						if (ZERO != matrix[subRow][column])
							rowOperation(subRow, row, -matrix[subRow][column]);
					}
				}
			}
			for (unsigned IndexR = (rows - 1); IndexR > 0; --IndexR)
			{
				unsigned row = order[IndexR];
				unsigned column = getLeadingZeros(row);
				for (unsigned subRowIndex = 0; subRowIndex < IndexR; ++subRowIndex)
				{
					unsigned subRow = order[subRowIndex];
					rowOperation(subRow, row, -matrix[subRow][column]);
				}
			}
		
	} // reducedRowEchelon

	// Return the determinant of the matrix.
	// Recursive function.
	//-------------------------------------------------------------
	TYPE determinant() const 
	{
		TYPE result = static_cast<TYPE>(0);

		//Error will appear if not a Square matrix
		assert(rows == columns);

		if (rows > 2)
		{
			int sign = 1;
			for (unsigned column = 0; column < columns; ++column)
			{
				TYPE subDeterminant;

				SquareMatrix subMatrix = SquareMatrix(*this, 0, column);

				subDeterminant = subMatrix.determinant();
				subDeterminant *= matrix[0][column];

				if (sign > 0)
					result += subDeterminant;
				else
					result -= subDeterminant;

				sign = -sign;
			}
		}
		else
		{
			result = (matrix[0][0] * matrix[1][1])
				- (matrix[0][1] * matrix[1][0]);
		}

		return result;

	} // determinant

	  //-------------------------------------------------------------
	  // Proform LU decomposition.
	  // This will create matrices L and U such that A=LxU
	  //-------------------------------------------------------------
	void LU_Decomposition(SquareMatrix & upper, SquareMatrix & lower) const 
	{
		//Error will appear if not A Square Matrix (Assertion failed)
		assert(rows == columns);

		TYPE const ZERO = static_cast<TYPE>(0);

		upper = *this;
		lower = *this;

		for (unsigned row = 0; row < rows; ++row)
			for (unsigned column = 0; column < columns; ++column)
				lower.matrix[row][column] = ZERO;

		for (unsigned row = 0; row < rows; ++row)
		{
			TYPE value = upper.matrix[row][row];
			if (ZERO != value)
			{
				upper.divideRow(row, value);
				lower.matrix[row][row] = value;
			}

			for (unsigned subRow = row + 1; subRow < rows; ++subRow)
			{
				TYPE value = upper.matrix[subRow][row];
				upper.rowOperation(subRow, row, -value);
				lower.matrix[subRow][row] = value;
			}
		}
	}
	SquareMatrix const getInverse() const //Square Matrix
	{
		// Concatenate the identity matrix onto this matrix.
		SquareMatrix inverseMatrix
		(
			*this,
			IdentityMatrix< TYPE >(rows, columns, "",""),
			TO_RIGHT
		);

		// Row reduce this matrix.  This will result in the identity
		// matrix on the left, and the inverse matrix on the right.
		inverseMatrix.reducedRowEchelon();
		// Copy the inverse matrix data back to this matrix.
		SquareMatrix result
		(
			inverseMatrix.getSubMatrix
			(
				0,
				rows - 1,
				columns,
				columns + columns - 1,
				inverseMatrix.order
			)
		);
		if ((*this)*result != IdentityMatrix< TYPE >(rows, columns, "", ""))
		{
			cout << "Inverse does not exist. The matrix was not inverted: " << endl;
			return *this;
		}
		else {
			return result;
		}
		
	} // invert
  //-------------------------------------------------------------
  // Invert this matrix.
  //-------------------------------------------------------------
	void invert()
	{
		*this = getInverse();
	} // invert
	
	string getname() const {
		return this->name;
	}
	string getsize() const
	{
		return this->size;
	}
	  //=============================================================================
	  // Class for identity matrix.
	  //=============================================================================

	template< class TYPE >
	class IdentityMatrix : public SquareMatrix< TYPE >
	{
	public:
		IdentityMatrix(unsigned rowsParameter, unsigned columnsParameter, string l,string p) : SquareMatrix< TYPE >(rowsParameter, columnsParameter, l, p)
		{
			TYPE const ZERO = static_cast<TYPE>(0);
			TYPE const ONE = static_cast<TYPE>(1);

			for (unsigned row = 0; row < Matrix< TYPE >::rows; ++row)
			{
				for (unsigned column = 0; column < SquareMatrix< TYPE >::columns; ++column)
					if (row == column)
						SquareMatrix< TYPE >::matrix[row][column] = ONE;
					else
						SquareMatrix< TYPE >::matrix[row][column] = ZERO;
			}
		}
	};
};
#endif //SquareMatrix