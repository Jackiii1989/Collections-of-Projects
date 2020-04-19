#include"SortingFunctions.h"

int * linearSort(int * p, int n, int ValMax)
{
	// good implementation of counting sort
	//http://www.sourcetricks.com/2013/03/counting-sort.html#.WhMx-jco_IU

		// init
	int count = 0;
	int* coardline = new int[ValMax];
	memset(coardline, 0, ValMax * sizeof(int));

	// save the values in the memory asceding order
	for (int a = 0; a < n; a++) {
		coardline[p[a]]++;
	}


	//save sorted values  in the array p
	for (int a = 0; a < ValMax; a++) {
		//while (coardline[a]--)
		for (int b = 0; b < coardline[a]; b++)
			p[count++] = a;
	}

	delete[] coardline; coardline = 0;
	return p;
}

template<class T>
void swap1(T & x, T & y);

void QuickSort(int * A, int left, int right)
{
	// initalize array
	int i = left, j = right;
	int pivot = A[(left + right) / 2];

	// partition
	while (i <= j)
	{
		while (A[i] < pivot) i++;
		while (A[j] > pivot) j--;
		if (i <= j) {
			swap1(A[i], A[j]);
			i++; j--;
		}
	};

	if (left < j) QuickSort(A, left, j);
	if (left < i) QuickSort(A, i, right);

}

template<class T>
void swap1(T & x, T & y)
{
	T temp;
	temp = x;
	x = y;
	y = temp;
}

