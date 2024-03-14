// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/Stat/ContingencyTableTest.h>
#include <Bpp/Numeric/VectorTools.h>
#include <Bpp/Numeric/Random/ContingencyTableGenerator.h>
#include <Bpp/Numeric/Random/RandomTools.h>
#include <Bpp/Numeric/Matrix/MatrixTools.h>
#include <vector>
#include <iostream>
#include <cmath>

using namespace bpp;
using namespace std;

//tbl<-rbind(c(6,12,16,20),c(9,34,28,12))
//chisq.test(tbl);
int main() {
  vector< vector<size_t> > table;
  vector<size_t> row1;
  row1.push_back(6);
  row1.push_back(12);
  row1.push_back(16);
  row1.push_back(20);
  table.push_back(row1);
  vector<size_t> row2;
  row2.push_back(9);
  row2.push_back(34);
  row2.push_back(28);
  row2.push_back(12);
  table.push_back(row2);
  ContingencyTableTest test(table);
  VectorTools::print(test.getMarginRows());
  VectorTools::print(test.getMarginColumns());
  ContingencyTableGenerator ctRand(test.getMarginRows(), test.getMarginColumns());
  RowMatrix<size_t> rtable = ctRand.rcont2();
  MatrixTools::print(rtable);

  cout << test.getStatistic() << " \t" << test.getPValue() << endl;
  if (abs(test.getPValue() - 0.01324) > 0.0001)
    return 1;

  //Now test permutations:
  ContingencyTableTest test2(table, 20000);
  cout << test2.getStatistic() << " \t" << test2.getPValue() << endl;
  if (abs(test2.getPValue() - 0.01324) > 0.01)
    return 1;

  return 0;
}

