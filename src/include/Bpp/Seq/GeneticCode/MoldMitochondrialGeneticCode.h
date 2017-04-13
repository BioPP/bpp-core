//
// File: MoldMitochondrialGeneticCode.h
// Created by: Julien Dutheil
// Created on: Thu Jun 13 11:52 CET 2013
//

/*
Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

This software is a computer program whose purpose is to provide classes
for sequences analysis.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/


#ifndef _MOLDMITOCHONDRIALGENETICCODE_H_
#define _MOLDMITOCHONDRIALGENETICCODE_H_

#include "GeneticCode.h"
#include "../Alphabet/NucleicAlphabet.h"

namespace bpp
{

/**
 * @brief This class implements the mold, protozoan, and coelenterate mitochondrial code
 * and the Mycoplasma/Spiroplasma code as describe on the NCBI website:
 * http://www.ncbi.nlm.nih.gov/Taxonomy/Utils/wprintgc.cgi?mode=t#SG4
 */
class MoldMitochondrialGeneticCode:
  public virtual GeneticCode
{
	public:
		MoldMitochondrialGeneticCode(const NucleicAlphabet* alphabet);

		virtual ~MoldMitochondrialGeneticCode() {}
	
    virtual MoldMitochondrialGeneticCode* clone() const {
      return new MoldMitochondrialGeneticCode(*this);
    }

  public:
    size_t getNumberOfStopCodons() const { return 2; }
 
    std::vector<int> getStopCodonsAsInt() const {
      std::vector<int> v(2);
      v[1] = 48;
      v[2] = 50;
      return v;
    }

    std::vector<std::string> getStopCodonsAsChar() const {
      std::vector<std::string> v(2);
      v[1] = "TAA";
      v[2] = "TAG";
      return v;
    }
   
    bool isStop(int state) const throw (BadIntException) {
      //Test:
      codonAlphabet_.intToChar(state); //throw exception if invalid state!
      return (state == 48 || state == 50);
    }
    
    bool isStop(const std::string& state) const throw (BadCharException) {
      int i = codonAlphabet_.charToInt(state);
      return (i == 48 || i == 50);
    }
 
    bool isAltStart(int state) const throw (BadIntException) {
      //Test:
      codonAlphabet_.intToChar(state); //throw exception if invalid state!
      return (state == 12 || state == 13 || state == 15 || state == 30 || state == 46 || state == 60 || state == 62);
    }
    
    bool isAltStart(const std::string& state) const throw (BadCharException) {
      int i = codonAlphabet_.charToInt(state);
      return (i == 12 || i == 13 || i == 15 || i == 30 || i == 46 || i == 60 || i == 62);
    }

};

} //end of namespace bpp.

#endif	//_MOLDMITOCHONDRIALGENETICCODE_H_

