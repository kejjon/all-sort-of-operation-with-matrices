#ifndef _MATRIX_H_
#define _MATRIX_H_
 
#include <iostream>
#include <cassert>
#include <climits>
#include <vector>
#include <string>
#include <stack>
#include <queue>
using namespace std;

//=============================================================================
// Matrix template class
//   Contains a set of matrix manipulation functions.  The template is designed
// so that the values of the matrix can be of any type that allows basic
// arithmetic.
//=============================================================================
template< class TYPE = int >
class Matrix
{
protected:
	// Matrix data.
	unsigned rows;
	unsigned columns;
	// Storage for matrix data.
	vector<vector< TYPE > > matrix;
	// Order sub-index for rows.
	//   Used as: matrix[ order[ row ] ][ column ].
	unsigned * order;
	//-------------------------------------------------------------
	// Return the number of leading zeros in the given row.
	//-------------------------------------------------------------
	unsigned getLeadingZeros(unsigned row) const
	{
		TYPE const ZERO = static_cast< TYPE >(0);
		unsigned column = 0;
		while (ZERO == matrix[row][column])
			++column;
		return column;
	}
	//-------------------------------------------------------------
	// Allocate memory for matrix data.
	//-------------------------------------------------------------
	void allocate(unsigned rowNumber, unsigned columnNumber)
	{
		// Allocate order integers.
		order = new unsigned[rowNumber];
		// Setup matrix sizes.
		matrix.resize(rowNumber);
		for (unsigned row = 0; row < rowNumber; ++row)
			matrix[row].resize(columnNumber);
	}
	//-------------------------------------------------------------
	// Free memory used for matrix data.
	//-------------------------------------------------------------
	void deallocate(unsigned rowNumber,unsigned columnNumber)
	{
		// Free memory used for storing order (if there is any).
		if (0 != rowNumber)
			delete[] order;
	}
public:
	// Used for matrix concatenation.
	typedef enum
	{
		TO_RIGHT,
		TO_BOTTOM
	} Position;
	//-------------------------------------------------------------
	// Return the number of rows in this matrix.
	//-------------------------------------------------------------
	unsigned getRows() const
	{
		return rows;
	}
	//-------------------------------------------------------------
	// Return the number of columns in this matrix.
	//-------------------------------------------------------------
	unsigned getColumns() const
	{
		return columns;
	}
	//-------------------------------------------------------------
	// Get an element of the matrix.
	//-------------------------------------------------------------
	TYPE get(unsigned row, unsigned column) const
	{
		assert(row < rows);
		assert(column < columns);
		return matrix[row][column];
	}
	//-------------------------------------------------------------
	// Set an element in the matrix.
	//-------------------------------------------------------------
	void put ( unsigned row, unsigned column, TYPE const & value)
	{
		assert(row < rows);
		assert(column < columns);
		matrix[row][column] = value;
	}
	//-------------------------------------------------------------
	// Return part of the matrix.
	// NOTE: The end points are the last elements copied.  They can
	// be equal to the first element when wanting just a single row
	// or column.  However, the span of the total matrix is
	// ( 0, rows - 1, 0, columns - 1 ).
	//-------------------------------------------------------------
	Matrix getSubMatrix(unsigned startRow, unsigned endRow, unsigned startColumn, unsigned endColumn, unsigned const * newOrder = NULL)
	{
		Matrix subMatrix(endRow - startRow + 1, endColumn - startColumn + 1);
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
	//-------------------------------------------------------------
	// Return a single column from the matrix.
	//-------------------------------------------------------------
	Matrix getColumn( unsigned column)
	{
		return getSubMatrix(0, rows - 1, column, column);
	}
	//-------------------------------------------------------------
	// Return a single row from the matrix.
	//-------------------------------------------------------------
	Matrix getRow( unsigned row)
	{
		return getSubMatrix(row, row, 0, columns - 1);
	}
	//-------------------------------------------------------------
	// Reorder the matrix so the rows with the most zeros are at
	// the end, and those with the least at the beginning.
	//
	// NOTE: The matrix data itself is not manipulated, just the
	// 'order' sub-indexes.
	//-------------------------------------------------------------
	void reorder()
	{
		unsigned * zeros = new unsigned[rows];
		for (unsigned row = 0; row < rows; ++row)
		{
			order[row] = row;
			zeros[row] = getLeadingZeros(row);
		}

		for (unsigned row = 0; row < (rows - 1); ++row)
		{
			unsigned swapRow = row;
			for (unsigned subRow = row + 1; subRow < rows; ++subRow)
			{
				if (zeros[order[subRow]] < zeros[order[swapRow]])
					swapRow = subRow;
			}

			unsigned hold = order[row];
			order[row] = order[swapRow];
			order[swapRow] = hold;
		}
		delete zeros;
	}
	//-------------------------------------------------------------
	// Divide a row by given value.  An elementary row operation.
	//-------------------------------------------------------------
	void divideRow(unsigned row, TYPE const & divisor)
	{
		for (unsigned column = 0; column < columns; ++column)
			matrix[row][column] /= divisor;
	}
	//-------------------------------------------------------------
	// Modify a row by adding a scaled row. An elementary row
	// operation.
	//-------------------------------------------------------------
	void rowOperation(unsigned row, unsigned addRow, TYPE const & scale)
	{
		for (unsigned column = 0; column < columns; ++column)
			matrix[row][column] += matrix[addRow][column] * scale;
	}
	//-------------------------------------------------------------
	// Place matrix in reduced row echelon form.
	//-------------------------------------------------------------
	virtual void reducedRowEchelon() //virtual
	{
		TYPE const ZERO = static_cast< TYPE >(0);
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
				// Subtract this row from all subsequent rows.
				for (unsigned subRowIndex = (rowIndex + 1); subRowIndex < rows; ++subRowIndex)
				{
					unsigned subRow = order[subRowIndex];
					if (ZERO != matrix[subRow][column])
						rowOperation(subRow,row,-matrix[subRow][column]);
				}
			}
		}
		// Back substitute all lower rows.
		for (unsigned rowIndex = (rows - 1); rowIndex > 0; --rowIndex)
		{
			unsigned row = order[rowIndex];
			unsigned column = getLeadingZeros(row);
			for (unsigned subRowIndex = 0; subRowIndex < rowIndex; ++subRowIndex)
			{
				unsigned subRow = order[subRowIndex];
				rowOperation(subRow, row,-matrix[subRow][column]);
			}
		}
	} // reducedRowEchelon
	
	  //-------------------------------------------------------------
	  // Reverse the order of the matrix entries using stacks and queues
	  //-------------------------------------------------------------
	void reverse()
	{
		stack <int> st;
		queue <int> q;
		for (unsigned i = 0; i < rows; i++) {
			for (unsigned j = 0; j < columns; j++) {
				q.push(matrix[i][j]);
			}
		}
		while (!q.empty())
		{
			st.push(q.front());
			q.pop();
		}
		while (!st.empty())
		{
			q.push(st.top());
			st.pop();
		}
		while (!q.empty())
		{
			for (unsigned i = 0; i < rows; i++) {
				for (unsigned j = 0; j < columns; j++) {
					matrix[i][j] = q.front();
					q.pop();
				}
			}
		  
		}
	}
	  //-------------------------------------------------------------
	  // Calculate a dot product between this and an other matrix.
	  //-------------------------------------------------------------
	TYPE dotProduct( Matrix const & otherMatrix) const
	{
		// The number of columns of the first matrix should be same as the number of rows of the second matrix.

		TYPE result = static_cast< TYPE >(0);
		for (unsigned row = 0; row < rows; ++row)
			for (unsigned column = 0; column < columns; ++column)
			{
				result += matrix[row][column]* otherMatrix.matrix[row][column];
			}
		return result;
	} // dotProduct
	  //-------------------------------------------------------------
	  // Return the transpose of the matrix.
	  //-------------------------------------------------------------
	Matrix const getTranspose() const 
	{
		Matrix result(columns, rows);
		// Transpose the matrix by filling the result's rows will
		// these columns, and vica versa.
		for (unsigned row = 0; row < rows; ++row)
			for (unsigned column = 0; column < columns; ++column)
				result.matrix[column][row] = matrix[row][column];
		return result;
	} // transpose
	  //-------------------------------------------------------------
	  // Transpose the matrix.
	  //-------------------------------------------------------------
	void transpose()
	{
		*this = getTranspose();
	}
	
	  //=======================================================================
	  // Operators.
	  //=======================================================================

	  //-------------------------------------------------------------
	  // Add by an other matrix.
	  //-------------------------------------------------------------
	Matrix const operator +(Matrix const & otherMatrix) const
	{
		assert(otherMatrix.rows == rows);
		assert(otherMatrix.columns == columns);

		Matrix result(rows, columns);

		for (unsigned row = 0; row < rows; ++row)
			for (unsigned column = 0; column < columns; ++column)
				result.matrix[row][column] =
				matrix[row][column]
				+ otherMatrix.matrix[row][column];

		return result;
	}

	//-------------------------------------------------------------
	// Add self by an other matrix.
	//-------------------------------------------------------------
	Matrix const & operator += ( Matrix const & otherMatrix)
	{
		*this = *this + otherMatrix;
		return *this;
	}

	//-------------------------------------------------------------
	// Subtract by an other matrix.
	//-------------------------------------------------------------
	Matrix const operator - (Matrix const & otherMatrix) const
	{
		assert(otherMatrix.rows == rows);
		assert(otherMatrix.columns == columns);

		Matrix result(rows, columns);

		for (unsigned row = 0; row < rows; ++row)
			for (unsigned column = 0; column < columns; ++column)
				result.matrix[row][column] =
				matrix[row][column]
				- otherMatrix.matrix[row][column];

		return result;
	}

	//-------------------------------------------------------------
	// Subtract self by an other matrix.
	//-------------------------------------------------------------
	Matrix const & operator -= (Matrix const & otherMatrix)
	{
		*this = *this - otherMatrix;
		return *this;
	}
	//-------------------------------------------------------------
	// Matrix multiplication.
	//-------------------------------------------------------------
	Matrix const operator * ( Matrix const & otherMatrix) const
	{
		TYPE const ZERO = static_cast< TYPE >(0);

		assert(otherMatrix.rows == columns);

		Matrix result(rows, otherMatrix.columns);

		for (unsigned row = 0; row < rows; ++row)
			for (unsigned column = 0; column < otherMatrix.columns; ++column)
			{
				result.matrix[row][column] = ZERO;

				for (unsigned index = 0; index < columns; ++index)
					result.matrix[row][column] +=
					matrix[row][index]
					* otherMatrix.matrix[index][column];
			}

		return result;
	}

	//-------------------------------------------------------------
	// Multiply self by matrix.
	//-------------------------------------------------------------
	Matrix const & operator *= (Matrix const & otherMatrix)
	{
		*this = *this * otherMatrix;
		return *this;
	}

	//-------------------------------------------------------------
	// Multiply by scalar constant.
	//-------------------------------------------------------------
	Matrix const operator *(TYPE const & scalar) const
	{
		Matrix result(rows, columns);

		for (unsigned row = 0; row < rows; ++row)
			for (unsigned column = 0; column < columns; ++column)
				result.matrix[row][column] = matrix[row][column] * scalar;

		return result;
	}

	//-------------------------------------------------------------
	// Multiply self by scalar constant.
	//-------------------------------------------------------------
	Matrix const & operator *=
		(
			TYPE const & scalar
			)
	{
		*this = *this * scalar;
		return *this;
	}
	//-------------------------------------------------------------
	// Copy matrix.
	//-------------------------------------------------------------
	Matrix & operator =(Matrix const & otherMatrix)
	{
		if (this == &otherMatrix)
			return *this;

		// Release memory currently in use.
		deallocate(rows, columns);

		rows = otherMatrix.rows;
		columns = otherMatrix.columns;
		allocate(rows, columns);

		for (unsigned row = 0; row < rows; ++row)
			for (unsigned column = 0; column < columns; ++column)
				matrix[row][column] =
				otherMatrix.matrix[row][column];

		return *this;
	}

	//-------------------------------------------------------------
	// Copy matrix data from array.
	// Although matrix data is two dimensional, this copy function
	// assumes the previous row is immediately followed by the next
	// row's data.
	//
	// Example for 3x2 matrix:
	//     int const data[ 3 * 2 ] =
	//     {
	//       1, 2, 3,
	//       4, 5, 6
	//     };
	//    Matrix< int > matrix( 3, 2 );
	//    matrix = data;
	//-------------------------------------------------------------
	Matrix & operator =
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

	//-----------------------------------------------------------------------
	// Return true if this matrix is the same of parameter.
	//-----------------------------------------------------------------------
	bool operator ==
		(
			Matrix const & value
			) const
	{
		bool isEqual = true;
		for (unsigned row = 0; row < rows; ++row)
			for (unsigned column = 0; column < columns; ++column)
				if (matrix[row][column] != value.matrix[row][column])
					isEqual = false;

		return isEqual;
	}

	//-----------------------------------------------------------------------
	// Return true if this matrix is NOT the same of parameter.
	//-----------------------------------------------------------------------
	bool operator !=
		(
			Matrix const & value
			) const
	{
		return !(*this == value);
	}

	//-------------------------------------------------------------
	// Constructor for empty matrix.
	// Only useful if matrix is being assigned (i.e. copied) from
	// somewhere else sometime after construction.
	//-------------------------------------------------------------
	Matrix(): rows(0), columns(0)
	{
		allocate(0, 0);
	}
	//-------------------------------------------------------------
	// Constructor using rows and columns.
	//-------------------------------------------------------------
	Matrix(unsigned rowsParameter, unsigned columnsParameter)
	{
		rows = rowsParameter;
		columns = columnsParameter;

		TYPE const ZERO = static_cast< TYPE >(0);

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

	//-------------------------------------------------------------
	// This constructor will allow the creation of a matrix based off
	// an other matrix.  It can copy the matrix entirely, or omitted a
	// row/column.
	//-------------------------------------------------------------
	Matrix( Matrix const & copyMatrix, unsigned omittedRow = INT_MAX, unsigned omittedColumn = INT_MAX)
	{
		// Start with the number of rows/columns from matrix to be copied.
		rows = copyMatrix.getRows();
		columns = copyMatrix.getColumns();

		// If a row is omitted, then there is one less row.
		if (INT_MAX != omittedRow)
			rows--;

		// If a column is omitted, then there is one less column.
		if (INT_MAX != omittedColumn)
			columns--;

		// Allocate memory for new matrix.
		allocate(rows, columns);

		unsigned rowIndex = 0;
		for (unsigned row = 0; row < rows; ++row)
		{
			// If this row is to be skipped...
			if (rowIndex == omittedRow)
				rowIndex++;

			// Set default order.
			order[row] = row;

			unsigned columnIndex = 0;
			for (unsigned column = 0; column < columns; ++column)
			{
				// If this column is to be skipped...
				if (columnIndex == omittedColumn)
					columnIndex++;

				matrix[row][column] = copyMatrix.matrix[rowIndex][columnIndex];

				columnIndex++;
			}

			++rowIndex;
		}

	}

	//-------------------------------------------------------------
	// Constructor to concatenate two matrices.  Concatenation
	// can be done to the right, or to the bottom.
	//   A = [B | C]
	//-------------------------------------------------------------
	Matrix( Matrix const & copyMatrixA, Matrix const & copyMatrixB, Position position = TO_RIGHT)
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
	//-------------------------------------------------------------
	// Destructor.
	//-------------------------------------------------------------
	~Matrix()
	{
		// Release memory.
		deallocate(rows, columns);
	}

};
//-----------------------------------------------------------------------------
// Stream operator used to convert matrix class to a string.
//-----------------------------------------------------------------------------
template< class TYPE >
std::ostream & operator<<
(
	// Stream data to place string.
	std::ostream & stream,

	// A matrix.
	Matrix< TYPE > const & matrix
	)
{
	for (unsigned row = 0; row < matrix.getRows(); ++row)
	{
		for (unsigned column = 0; column < matrix.getColumns(); ++column)
			stream << "\t" << matrix.get(row, column);

		stream << std::endl;
	}

	return stream;
}

#endif // _MATRIX
