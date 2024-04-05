// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/NumConstants.h>

#include <iostream>
#include <limits>

using namespace bpp;
using namespace std;

int main()
{
  cout << "Min double\t:" << numeric_limits<double>::min() << endl;
  cout << "Max double\t:" << numeric_limits<double>::max() << endl;
  cout << "bpp::NumConstants::INF()\t:" << NumConstants::INF() << endl;
  cout << "bpp::NumConstants::MINF()\t:" << NumConstants::MINF() << endl;
  cout << "bpp::NumConstants::PINF()\t:" << NumConstants::PINF() << endl;
  cout << "bpp::NumConstants::NaNF()\t:" << NumConstants::NaN() << endl;
  if (numeric_limits<double>::min() < NumConstants::MINF())
    return 1;
  if (numeric_limits<double>::max() > NumConstants::PINF())
    return 1;
  cout << "Compiler implements infinity\t:" << (numeric_limits<double>::has_infinity ? "yes" : "no") << endl;
  cout << "Compiler implements quiet nan\t:" << (numeric_limits<double>::has_quiet_NaN ? "yes" : "no") << endl;
  cout << "Compiler implements signaling nan\t:" << (numeric_limits<double>::has_signaling_NaN ? "yes" : "no") << endl;
  return 0;
}
