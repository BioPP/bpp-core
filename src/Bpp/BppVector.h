//
// File: BppVector.h
// Authors:
//   Julien Dutheil
//   Francois Gindraud (2017)
// Created: 2008-04-07 15:14:00
// Last modified: 2017-06-27
//

/*
  Copyright or © or Copr. CNRS, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide utilitary
  classes. This file belongs to the Bio++ Project.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_BPPVECTOR_H
#define BPP_BPPVECTOR_H

#include <utility>
#include <vector>

#include "Clonable.h"

namespace bpp
{
  /** @brief The BppVector object class.
   * This class extends the std::vector class to support the Clonable interface.
   * @deprecated Do not use in new code anymore.
   */
  template <typename T>
  class BppVector : public Clonable, public std::vector<T>
  {
  public:
    template <typename... Args>
    BppVector(Args&&... args)
      : std::vector<T>(std::forward<Args>(args)...)
    {
    }

    BppVector<T>* clone() const { return new BppVector<T>(*this); }

    const std::vector<T>& toSTL() const { return *this; }
    std::vector<T>& toSTL() { return *this; }
  };
} // namespace bpp
#endif // BPP_BPPVECTOR_H
