#include "Matrix.h"
#include "SquareMatrix.h"
#include "DiagnalMatrix.h"
#include "Algorithms_DataCollection.h"

#include <iostream>
#include <cassert>
#include <climits>
#include <vector>
#include <string>

using namespace std;

vector<SquareMatrix<int>*> Determinant_Collection;
vector<int> sampleVector;


void SolveLinear_Systems(SquareMatrix<int> & A, Matrix<int> & b) {
	cout << "The system of equation is: " << endl;
	cout << A.get(0, 0) << "*X1 + " << A.get(0, 1) << "*X2 + " << A.get(0, 2) << "*X3 =" << b.get(0, 0) << endl;
	cout << A.get(1, 0) << "*X1 + " << A.get(1, 1) << "*X2 + " << A.get(1, 2) << "*X3 =" << b.get(1, 0) << endl;
	cout << A.get(2, 0) << "*X1 + " << A.get(2, 1) << "*X2 + " << A.get(2, 2) << "*X3 =" << b.get(2, 0) << endl;
	A.invert();
	A *= b;
	cout << endl;
	cout << "The soulution of the system of equation is:" << endl;
	cout << "X1= " << A.get(0, 0) << " ,";
	cout << "X2= " << A.get(1, 0) << " ,";
	cout << "X3= " << A.get(2, 0);
	cout << endl;
}
void Matrix_to_1DVector(Matrix<int> & sampleMatrix, int size)
{
	sampleVector.empty(); 
	cout << "The entries of the matrix in order stored in a vector are: " << endl;
	for(int i=0; i<size*size; i++){
	 sampleVector.push_back(sampleMatrix.get(i/size, i % size));
	 cout << sampleVector[i] << " ";
		}
	cout << endl;
};
void Binary_Implementation(Matrix<int> & m, int matrix_size, int key) {

	cout << "The Matrix used for finding the entry "<<key<<" is: \n" << m;
	Matrix_to_1DVector(m, matrix_size);
	Quick_Sort(sampleVector);
	if (binary_search(sampleVector, key) != -1) {
		cout << "Element found at index(count starts from 0): " << binary_search(sampleVector, key) << endl;
	}
	else
		cout << "The Element was not found! " << endl;
}

int main() {
	//Matrix base
	int const mdata1[2 * 3] = { 1, 5, 7
						      -2, -7,-5 };
	int const mdata2[3 * 2] = {1, 2,
	                           3, 4,
	                           5, 6};
	int const mdata3[4 * 5] = { 1, -5, 4 ,0 ,0,
						        0, 1, 0, 1, 0,
						        0, 0, 3, 0, 0,
						        0, 0, 0 ,2, 0 };
	int const mdata4[4 * 5] = { 2, 1, 3, 2, -1,
		                        0, -3, 1, 3, -2,
		                        1, 4, 4, 7, 5,
		                       4, 9, 11, 8, 10 };
	Matrix<int> matrix1(2, 3); matrix1 = mdata1;
	Matrix<int> matrix2(3, 2); matrix2 = mdata2;
	Matrix<int> matrix3(4, 5); matrix3 = mdata3;
	Matrix<int> matrix4(4, 5); matrix4 = mdata4;
	
	//Squarematrix child2
	int const sdata1[3*3]= { 1, 1, 1,
		                    0, 1, 1,
		                    0, 0, 1};
	int const sdata2[3 * 3] = { 1, 2, 3,
						        4, 5, 6,
						        7, 8, 9 };
	SquareMatrix<int> smatrix1(3, 3, "SquareMatrix_1", "3X3"); smatrix1 = sdata1;
	SquareMatrix<int> smatrix2(3, 3, "SquareMatrix_2", "3X3"); smatrix2 = sdata2;

	//DiagonalMatrix child3
	int const ddata1[4 * 4] = { 1, 0, 0, 0,
								0, 5, 0, 0,
								0, 0, 7, 0,
		                        0, 0, 0, 0 };
	int const ddata2[5 * 5] = { 5, 0, 0, 0, 0,
		                        0, 6, 0, 0, 0,
		                        0, 0, 8, 0, 0,
		                        0, 0, 0, 4, 0,
	                            0, 0, 0, 0, 1};
	DiagonalMatrix<int> dmatrix1(4, 4, "DiagonalMatrix_1", "4X4", "NI"); dmatrix1 = ddata1;
	DiagonalMatrix<int> dmatrix2(5, 5, "DiagonalMatrix_2", "5X5", "I"); dmatrix2 = ddata2;
	 
	//Collection of SquareMatrices
	int const sdata3[2 * 2] = { 2, 4, 5, 8 };
	int const sdata4[4 * 4] = { 1, 3, 0, 1, 7, 3, 3, 5, 8, 3, 0, 4, 8, 0, 0, 4 };
	int const sdata5[5 * 5] = { 2, 3, 3, 1, 0, 4, 3, -3, 2, -1, -1, -3, 0, -4, -3, 2 ,8, 4, 5 ,0, 0, 1, 4, 0, 1};

	SquareMatrix<int>* smatrix11 = new SquareMatrix<int>(3, 3, "SquareMatrix_11", "3X3"); *smatrix11 = sdata1;
	SquareMatrix<int>* smatrix22 = new SquareMatrix<int>(3, 3, "SquareMatrix_22", "3X3"); *smatrix22 = sdata2;
	SquareMatrix<int>* smatrix3= new SquareMatrix<int>(2, 2, "smatrix_3", "2X2"); *smatrix3 = sdata3;
	SquareMatrix<int>* smatrix4 = new SquareMatrix<int>(4, 4, "smatrix_4", "4X4"); *smatrix4 = sdata4;
	SquareMatrix<int>* smatrix5 = new SquareMatrix<int>(5, 5, "smatrix_5", "5X5"); *smatrix5 = sdata5;
	
	Determinant_Collection.push_back(smatrix11);
	Determinant_Collection.push_back(smatrix22);
	Determinant_Collection.push_back(smatrix3);
	Determinant_Collection.push_back(smatrix4);
	Determinant_Collection.push_back(smatrix5);


	cout << "__________________________________________________________________________" << endl << endl;
	cout << "^^^^ MATRIX OPERATIONS AND PROBLEM SOLVING USING MATRICES ^^^^" << endl << endl;

	cout << "Choose one of the options below to show: \n";
	cout << "1. Matrix Operations\n";
	cout << "2. Square Matrix Properties\n";
	cout << "3. Diagonal Matrix Properties \n";
	cout << "4. Binary Search example \n";
	cout << "5. Collection of SquareMatrices ordered by their Determinant value\n";
	cout << "6. Solution of a linear system of equations using Matrixes\n";
	cout << " Input number: ";
	int response;
	while (cin >> response)
	{
		if (response == 1)
		{
			cout << "The matrices used for this operations: " << endl;
			cout << "Matrix_1\n" << matrix1 << endl;
			cout << "Matrix_2\n" << matrix2 << endl;
			cout << "Matrix_3\n" << matrix3 << endl;
			cout << "Matrix_4\n" << matrix4 << endl;

			cout << "Multiplication\n Matrix_1 * Matrix_2 = \n";
			cout << matrix1*matrix2 << endl;
			cout << "Matrix_2*Matrix_1 = \n" << endl << matrix2*matrix1 << endl;
			cout << "Addition\n Matrix_3 + Matrix_4 = \n";
			cout << matrix3 + matrix4 << endl;
			cout << "Substration\n Matrix_4 - Matrix_3 =\n";
			cout << matrix4 - matrix3 << endl;
			cout << " The transpose of Matrix1 is:\n" << matrix1.getTranspose() << endl;
			cout << " The reverse order of Matrix2 is:\n"; matrix2.reverse();
			cout << matrix2 << endl;
			cout << "The reduced echelon form of Matrix_3 is:\n"; matrix3.reducedRowEchelon();
			cout << matrix3 << endl;
			cout << "The reduced echelon form of Matrix_4 is:\n"; matrix4.reducedRowEchelon();
			cout << matrix4 << endl;

		}
		else if (response == 2) {
			cout << "The matrices used for this operations: " << endl;
			cout << "SquareMatrix_1\n" << smatrix1 << endl;
			cout << "SquareMatrix_2\n" << smatrix2 << endl;
			cout << "Finding the inverse of " << smatrix1.getname() << endl; cout << smatrix1.getInverse() << endl;
			cout << "Finding the inverse of " << smatrix2.getname() << endl; cout << smatrix2.getInverse() << endl;

			cout << "The determinant of SquareMatrix_1 and 2 are(respectively): " << smatrix1.determinant() << " and " << smatrix2.determinant() << endl;
			SquareMatrix<int> upper(3, 3, "Upper", "3X3");
			SquareMatrix<int> lower(3, 3, "lower", "3X3");
			cout << "The LU decomposition of SquareMatrix_2 such that L*U equals this matrix: " << endl;
			smatrix2.LU_Decomposition(upper, lower);
			cout << " U :\n" << upper << endl;
			cout << " L :\n" << lower << endl;
			cout << "The Dotproduct of L and U is: " << upper.dotProduct(lower) << endl;
			cout << "The reduced echelon form of " << smatrix1.getname() << " is:\n"; smatrix1.reducedRowEchelon();
			cout << smatrix1 << endl;

		}
		else if (response == 3)
		{
			cout << "The matrices used for this operations: " << endl;
			cout << "DiagonalMatrix_1\n" << dmatrix1 << endl;
			dmatrix1.getInvertibility();
			cout << "DiagonalMatrix_2\n" << dmatrix2 << endl;
			dmatrix2.getInvertibility();
			cout << "DiagonalMatrix_1 to the power 3 is: \n";
			cout << dmatrix1.Powers_of_DMatrix(3) << endl;
			DiagonalMatrix<int> sample_dmatrix(3, 3, "DiagonalMatrix", "3X3", "NI");
			int const sample[3 * 3] = { 4,0,0,7,1,0, 6,4,9 };
			SquareMatrix<int> sample_matrix(3, 3, "DiagonalMatrix_1", "4X4"); sample_matrix = sample;
			cout << "Diagonalization example" << endl << "SquareMatrix used: \n" << sample_matrix;
			cout << "The Diagonal Matrix of SqaureMatrix is: \n";
			cout << sample_dmatrix.Diagonalization(sample_matrix) << endl;
			cout << "The eigenvalues of The SquareMatrix: "; sample_dmatrix.getDiagonal();
			cout << "The reduced echelon form of DiagonalMatrix_2 is:\n"; dmatrix2.reducedRowEchelon();
			cout << dmatrix2 << endl;
		}
		else if (response == 4)
		{
			Binary_Implementation(smatrix2, 3, 5);
		}
		else if (response == 5)
		{
			mergeSort(Determinant_Collection);
			output_sorting(Determinant_Collection);
		}
		else if (response == 6) {
			cout << "Example: " << endl;
			SquareMatrix<int> smatrix(3, 3, "SquareMatrix_1", "3X3"); smatrix = sdata1;
			Matrix<int> b1(3, 1);  b1.put(0, 0, 1); b1.put(1, 0, 2);  b1.put(2, 0, 3);
			SolveLinear_Systems(smatrix, b1);
		}
		else cout << "Invalid entry" << endl;

		cin.clear();
		cout << "--------------------------------------------------------------------" << endl;
		cout << "Choose one of the options below to show: \n";
		cout << "1. Matrix Operations\n";
		cout << "2. Square Matrix Properties\n";
		cout << "3. Diagonal Matrix Properties \n";
		cout << "4. Binary Search example \n";
		cout << "5. Collection of SquareMatrices ordered by their Determinant value\n";
		cout << "6. Solution of a linear system of equations using Matrixes\n";
		cout << " Input number: ";

	}
	return 0;
}