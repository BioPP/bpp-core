// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/Prob/Simplex.h>
#include <Bpp/Text/TextTools.h>

#include <iostream>
#include <limits>

using namespace bpp;
using namespace std;

int main()
{
  try
  {
    vector<double> prob;
    prob.push_back(0.1);
    prob.push_back(0.2);
    prob.push_back(0.3);
    prob.push_back(0.15);
    prob.push_back(0.1);
    prob.push_back(0.05);
    prob.push_back(0.1);

    vector<Simplex*> vpsi;
    vpsi.push_back(new Simplex(prob, 1));
    vpsi.push_back(new Simplex(prob, 2));
    vpsi.push_back(new Simplex(prob, 3));

    for (size_t i = 0; i < 3; i++)
    {
      cout << "Method " << i + 1 << endl;
      for (size_t j = 0; j < prob.size() - 1; j++)
      {
        cout << vpsi[i]->getParameterValue("theta" + TextTools::toString(j + 1)) << "\t";
      }
      cout << endl;
    }


    cout << "Prob:";
    for (size_t j = 0; j < prob.size(); j++)
    {
      cout << prob[j] << "\t";
    }
    cout << endl;
    for (size_t i = 0; i < 3; i++)
    {
      for (size_t j = 0; j < prob.size() - 1; j++)
      {
        vpsi[i]->setParameterValue("theta" + TextTools::toString(j + 1),
            vpsi[i]->getParameterValue("theta" + TextTools::toString(j + 1)) + 0.1);
      }
      for (size_t j = 0; j < prob.size() - 1; j++)
      {
        vpsi[i]->setParameterValue("theta" + TextTools::toString(j + 1),
            vpsi[i]->getParameterValue("theta" + TextTools::toString(j + 1)) - 0.1);
      }
      cout << "Method " << i + 1 << endl;
      cout << "prob\t";
      for (size_t j = 0; j < prob.size(); j++)
      {
        cout << vpsi[i]->prob(j) << "\t";
      }
      cout << endl;
    }

    for (size_t i = 0; i < 3; i++)
    {
      delete vpsi[i];
    }
    return 0;
  }
  catch (Exception& ex)
  {
    cout << "failed :(" << endl;
    return 1;
  }
}
