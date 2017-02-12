//
// File: StreamSequenceIterator.h
// Created by: Julien Dutheil
// Created on: Tue Feb 26 14:27 2013
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

#ifndef _STREAMSEQUENCEITERATOR_H_
#define _STREAMSEQUENCEITERATOR_H_

#include "../SequenceIterator.h"
#include "ISequenceStream.h"

// From the STL:
#include <istream>

namespace bpp
{

/**
 * @brief A sequence iterator based on a sequence stream.
 *
 * This iterator uses a bpp::BasicSequence object for storing sequences.
 */
class StreamSequenceIterator:
  public virtual SequenceIterator
{
  private:
    const Alphabet* alphabet_;
    const ISequenceStream* seqStream_;
    std::istream* stream_;
    BasicSequence* nextSeq_;

	public:
		StreamSequenceIterator(ISequenceStream& seqStream, std::istream& stream, const Alphabet* alphabet);

		virtual ~StreamSequenceIterator() {}

  private: //Recopy is forbidden
    StreamSequenceIterator(const StreamSequenceIterator& ssi):
      alphabet_(ssi.alphabet_),
      seqStream_(ssi.seqStream_),
      stream_(ssi.stream_),
      nextSeq_(0) {}

    StreamSequenceIterator& operator=(const StreamSequenceIterator& ssi)
    {
      alphabet_  = ssi.alphabet_;
      seqStream_ = ssi.seqStream_;
      stream_    = ssi.stream_;
      nextSeq_   = 0;
      return *this;
    }
	
	public:
		virtual Sequence* nextSequence();
    
    virtual bool hasMoreSequences() const { return nextSeq_ != 0; }
};

/**
 * @brief A sequence iterator based on a sequence stream.
 *
 * This iterator uses a bpp::SequenceWithQuality object for storing sequences.
 */
class StreamSequenceWithQualityIterator:
  public virtual SequenceWithQualityIterator
{
  private:
    const Alphabet* alphabet_;
    const ISequenceStream* seqStream_;
    std::istream* stream_;
    SequenceWithQuality* nextSeq_;

	public:
		StreamSequenceWithQualityIterator(ISequenceStream& seqStream, std::istream& stream, const Alphabet* alphabet);

		virtual ~StreamSequenceWithQualityIterator() {}

  private: //Recopy is forbidden
    StreamSequenceWithQualityIterator(const StreamSequenceWithQualityIterator& ssi):
      alphabet_(ssi.alphabet_),
      seqStream_(ssi.seqStream_),
      stream_(ssi.stream_),
      nextSeq_(0) {}

    StreamSequenceWithQualityIterator& operator=(const StreamSequenceWithQualityIterator& ssi)
    {
      alphabet_  = ssi.alphabet_;
      seqStream_ = ssi.seqStream_;
      stream_    = ssi.stream_;
      nextSeq_   = 0;
      return *this;
    }
	
	public:
		virtual SequenceWithQuality* nextSequence();
    
    virtual bool hasMoreSequences() const { return nextSeq_ != 0; }
};


} //end of namespace bpp.

#endif	//_SEQUENCEITERATOR_H_

