// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/VectorTools.h>
#include <Bpp/Numeric/Random/RandomTools.h>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>

using namespace bpp;
using namespace std;

int main() {
  //Create vector:
  vector<string> pop;
  pop.push_back("A");
  pop.push_back("B");
  pop.push_back("C");
  pop.push_back("D");
  pop.push_back("E");
  unsigned int n = 10000;

  cout << "-*- Check without replacement -*-" << endl;
  for (unsigned int k = 1; k < 5; ++k) {
    map<string, unsigned int> counts;
    for (unsigned int i = 0; i < n; ++i) {
      vector<string> sample(k);
      RandomTools::getSample(pop, sample, false);
      for (size_t j = 0; j < sample.size(); ++j) {
        counts[sample[j]]++;
      }
    }
    for (map<string, unsigned int>::iterator it = counts.begin(); it != counts.end(); ++it) {
      double fobs = static_cast<double>(it->second) / static_cast<double>(n);
      double fexp = static_cast<double>(k) / 5.;
      cout << it->first << "\t" << it->second << "\t" << fobs << "\t" << fexp << endl;
      if (abs(fobs - fexp) > 0.1)
        return 1;
    }
    cout << "---------------------------------------" << endl;
  }

  cout << "-*- Check with replacement -*-" << endl;
  for (unsigned int k = 1; k < 5; ++k) {
    map<string, unsigned int> counts;
    for (unsigned int i = 0; i < n; ++i) {
      vector<string> sample(k);
      RandomTools::getSample(pop, sample, true);
      for (size_t j = 0; j < sample.size(); ++j) {
        counts[sample[j]]++;
      }
    }
    for (map<string, unsigned int>::iterator it = counts.begin(); it != counts.end(); ++it) {
      double fobs = static_cast<double>(it->second) / static_cast<double>(n);
      double fexp = static_cast<double>(k) / 5.;
      cout << it->first << "\t" << it->second << "\t" << fobs << "\t" << fexp << endl;
      if (abs(fobs - fexp) > 0.1)
        return 1;
    }
    cout << "---------------------------------------" << endl;
  }

  cout << "-*- Check with replacement and weights -*-" << endl;
  vector<double> weights;
  weights.push_back(2);
  weights.push_back(3);
  weights.push_back(8);
  weights.push_back(2);
  weights.push_back(1);
  double sumw = VectorTools::sum(weights);
  vector<double> fexp = weights / sumw;
  for (unsigned int k = 1; k < 5; ++k) {
    map<string, unsigned int> counts;
    for (unsigned int i = 0; i < n; ++i) {
      vector<string> sample(k);
      RandomTools::getSample(pop, weights, sample, true);
      for (size_t j = 0; j < sample.size(); ++j) {
        counts[sample[j]]++;
      }
    }
    for (size_t i = 0; i < pop.size(); ++i) {
      double fobs = static_cast<double>(counts[pop[i]]) / static_cast<double>(n*k);
      cout << pop[i] << "\t" << counts[pop[i]] << "\t" << fobs << "\t" << fexp[i] << endl;
      if (abs(fobs - fexp[i]) > 0.1)
        return 1;
    }
    cout << "---------------------------------------" << endl;
  }



  return 0;
}

