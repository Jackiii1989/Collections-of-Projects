 #include <algorithm>
#include <cmath>
#include <cassert>
#include <functional>
#include <thread>
#include <numeric>
#include<utility>
#include "Means.h"

using namespace std;

bool ComputeMeans(Numbers const &numbers, Means &averages)
{
  if (numbers.empty()) {
    return false;
  }
  double sum = 0;
  double logSum = 0;
  double reciprocalSum = 0;
  double quadSum = 0;
  for (auto nr : numbers) {
    sum += nr;
    logSum += log(nr);
    reciprocalSum += 1.0 / nr;
    quadSum += nr * nr;
  }
  double size = (double)numbers.size();
  averages.arithmeticMean = sum / size;
  averages.geometricMean = exp(logSum / size);
  averages.harmonicMean = size / reciprocalSum;
  averages.quadraticMean = sqrt(quadSum / size);
  return true;
}


void CalcMeansPara( Means &averages, int  val) {
	
	averages.arithmeticMean += val;
	averages.geometricMean += log(val);
	averages.harmonicMean += 1.0 / val;
	averages.quadraticMean += val*val;
	
}

void CalcMeansPara2(Means &averages, Means averages2) {

	averages.arithmeticMean += averages2.arithmeticMean;
	averages.geometricMean += averages2.geometricMean;
	averages.harmonicMean +=  averages2.harmonicMean;
	averages.quadraticMean += averages2.quadraticMean;

}


template<class InputIt, class BinaryOperation>
Means MyAccumulate(InputIt first, InputIt last, Means init,
	BinaryOperation op)
{
	for (; first != last; ++first) {
		op(init, *first); // std::move since C++20
	}
	return init;
}

bool ParallelComputeMeans(Numbers const &numbers, Means &averages, int nrThreads)
{
	if (nrThreads == 0)
		nrThreads = thread::hardware_concurrency();
	size_t size = numbers.size();
	size_t nrElems = size / nrThreads;
	auto begin =  numbers.begin();
	vector<Means> results(nrThreads);
	vector<thread> threads;
	for (size_t i = 0; i < nrThreads; i++) {
		auto end = begin + nrElems;
		threads.emplace_back([i, begin, end, &results]{
			// i = 0;
			results[i] = (MyAccumulate(begin, end, results[i], CalcMeansPara));
		
		});
		begin = end;
	}
	Means result;
	result = MyAccumulate(begin, numbers.end(), result, CalcMeansPara);
	for (auto &t : threads) {
		t.join();
	}
	averages = MyAccumulate(results.begin(), results.end(), result, CalcMeansPara2);

	averages.arithmeticMean = averages.arithmeticMean / size;
	averages.geometricMean = exp(averages.geometricMean / size);
	averages.harmonicMean = size / averages.harmonicMean;
	averages.quadraticMean = sqrt(averages.quadraticMean / size);

	return true;
}


