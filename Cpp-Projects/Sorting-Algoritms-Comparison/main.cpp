#include "TimeMeas.h"
#include "SortingFunctions.h"
#include <algorithm>
#include <fstream>
#include <cassert>
#include<string>

int main() {

	// initialize data
	int const numberOftest = 20;
	__int64 timeDiff1[numberOftest] = { 0 }; // array for measured values
	__int64 timeDiff2[numberOftest] = { 0 }; // array for measured values
	__int64 timeDiff3[numberOftest] = { 0 }; // array for measured values
	__int64 start = 0;

	string NameCsv = "Output1";
	std::size_t const n = 10000;
	std::size_t const RndMxVal = 300;

	int	a[n], b[n], c[n];

	srand(time(NULL));

	//-------------------------------------------------------------------------------------------------------------
	// measuring iteration

	for (int i = 0; i < numberOftest; i++) {
		for (int i = 0; i<n; i++) {
			int RndVal = rand() % RndMxVal + 1;
			a[i] = RndVal;	
			b[i] = RndVal;
			c[i] = RndVal;
		}

	int MxVal = *max_element(begin(a), end(a))+1;


	start = ReadTSC(); // start
	linearSort(a, n, MxVal);
	timeDiff1[i] = ReadTSC() - start; // STOP,
	
	start = ReadTSC(); // start
	QuickSort(b,0,n-1);
	timeDiff2[i] = ReadTSC() - start; // STOP,
	

	start = ReadTSC(); // start
	sort(begin(c),end(c));
	timeDiff3[i] = ReadTSC() - start; // STOP,
	}

	// measuring iteration
	//--------------------------------------------------------------------------------------------------------------


	//cout << "linear Sort,"<< a<<"QuickSort,"<<b<<"STL-sort,"<<c;
	//cout << "\n--------------------------------------------------------------------------------------------\n\n";
	cout << "Times from linear Sort\n" << timeDiff1 << "Times from QuickSort\n" << timeDiff2 << "Times from STL-sort\n" << timeDiff3;

	
	PrintAsCSVFile(timeDiff1, std::string("linear Sort"), NameCsv, numberOftest);
	PrintAsCSVFile(timeDiff2, std::string("QuickSort "), NameCsv, numberOftest);
	PrintAsCSVFile(timeDiff3, std::string("STL-sort"), NameCsv, numberOftest);


	return 0;
}

// Print an array
template<typename T1, std::size_t arrSize, typename = std::enable_if_t<!std::is_same<T1, char>::value>>
std::ostream& operator <<(std::ostream& ost, const T1(&arr)[arrSize])
{
	ost << "values are:" << endl;

	for (const auto& elem : arr) {
		ost << elem << ",";
	}
	return ost << endl;
}

template<class T>
void PrintAsCSVFile(T* a, string& Message, string& Name, int size) {
	std::ofstream out(Name + ".csv", std::ios_base::app);
	assert(out.good()); // check if file could be opened/create
	out << Message << ";" << size << ";\n";
	for (int n = 0; n < size; n++) {
		out << a[n] << ";";
	}
	out << std::endl << std::endl;;
	out.close();
}
