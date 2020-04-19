#ifndef AVERAGE_H
#define AVERAGE_H

#include <vector>

typedef std::vector<int> Numbers;

struct Means {
  double arithmeticMean;
  double geometricMean;
  double harmonicMean;
  double quadraticMean;

  Means(double a, double b, double c, double d) :
	  arithmeticMean(a),
	  geometricMean(b),
	  harmonicMean(c),
	  quadraticMean(d)
  {};
  

  Means() :
	  arithmeticMean {},
	  geometricMean{},
	  harmonicMean{},
	  quadraticMean{}
  {};

  Means& operator=(Means a)
  {
	  arithmeticMean = a.arithmeticMean;
	  geometricMean = a.geometricMean;
	  harmonicMean = a.harmonicMean;
	  quadraticMean = a.quadraticMean;
	  return *this;
  }


};

bool ComputeMeans(Numbers const &numbers, Means &averages);

bool ParallelComputeMeans(Numbers const &numbers, Means &averages, int nrThreads = 0);

#endif
