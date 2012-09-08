//
// File: BppODiscreteDistributionFormatFormat.cpp
// Created by: Laurent Guéguen
// Created on: lundi 3 septembre 2012, à 14h 48
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 16, 2004)

  This software is a computer program whose purpose is to provide classes
  for phylogenetic data analysis.

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

#include "BppODiscreteDistributionFormat.h"

#include "../Io/FileTools.h"
#include "../Text/TextTools.h"
#include "../Text/StringTokenizer.h"
#include "../Text/KeyvalTools.h"

#include "../Numeric/Prob.all"
#include "BppOParametrizableFormat.h"

using namespace bpp;

// From the STL:
#include <iomanip>

using namespace std;


DiscreteDistribution* BppODiscreteDistributionFormat::read(const std::string& distDescription,
                                                           std::map<std::string, std::string>& unparsedParameterValues,
                                                           bool verbose)
{
  string distName;
  DiscreteDistribution* rDist = 0;
  map<string, string> args;
  KeyvalTools::parseProcedure(distDescription, distName, args);

  if ((distName == "InvariantMixed") || (distName=="Invariant"))
    {
      // We have to parse the nested distribution first:
      string nestedDistDescription = args["dist"];
      if (TextTools::isEmpty(nestedDistDescription))
        throw Exception("PhylogeneticsApplicationTools::getDistributionDefaultInstance. Missing argument 'dist' for distribution 'Invariant'.");
      if (verbose)
        ApplicationTools::displayResult("Invariant Mixed distribution", distName );
      map<string, string> unparsedParameterValuesNested;
      DiscreteDistribution* nestedDistribution = read(nestedDistDescription,
                                                      unparsedParameterValuesNested,
                                                      verbose);

      // Now we create the Invariant rate distribution:
      rDist = new InvariantMixedDiscreteDistribution(nestedDistribution, 0.1, 0.000001);

      // Then we update the parameter set:
      for (map<string, string>::iterator it = unparsedParameterValuesNested.begin();
           it != unparsedParameterValuesNested.end(); it++)
        unparsedParameterValues["Invariant." + it->first] = it->second;
      
      if (args.find("p") != args.end())
        unparsedParameterValues["Invariant.p"] = args["p"];
    }
  else if (distName == "Constant")
    {
      if (args.find("value") == args.end())
        throw Exception("Missing argument 'value' in Constant distribution");
      rDist = new ConstantDistribution(TextTools::to<double>(args["value"]));
      unparsedParameterValues["Constant.value"] = args["value"];
    }
  else if (distName == "Simple")
    {
      if (args.find("values") == args.end())
        throw Exception("Missing argument 'values' in Simple distribution");
      if (args.find("probas") == args.end())
        throw Exception("Missing argument 'probas' in Simple distribution");
      vector<double> probas, values;

      string rf = args["values"];
      StringTokenizer strtok(rf.substr(1, rf.length() - 2), ",");
      while (strtok.hasMoreToken())
        values.push_back(TextTools::toDouble(strtok.nextToken()));

      rf = args["probas"];
      StringTokenizer strtok2(rf.substr(1, rf.length() - 2), ",");
      while (strtok2.hasMoreToken())
        probas.push_back(TextTools::toDouble(strtok2.nextToken()));

      rDist = new SimpleDiscreteDistribution(values, probas);
      vector<string> v = rDist->getParameters().getParameterNames();

      for (unsigned int i = 0; i < v.size(); i++)
        {
          unparsedParameterValues[v[i]] = TextTools::toString(rDist->getParameterValue(rDist->getParameterNameWithoutNamespace(v[i])));
        }
    }
  else if (distName == "Mixture")
    {
      if (args.find("probas") == args.end())
        throw Exception("Missing argument 'probas' in Mixture distribution");
      vector<double> probas;
      vector<DiscreteDistribution*> v_pdd;
      DiscreteDistribution* pdd;
      string rf = args["probas"];
      StringTokenizer strtok2(rf.substr(1, rf.length() - 2), ",");
      while (strtok2.hasMoreToken())
        probas.push_back(TextTools::toDouble(strtok2.nextToken()));

      vector<string> v_nestedDistrDescr;

      unsigned int nbd = 0;
      while (args.find("dist" + TextTools::toString(++nbd)) != args.end())
        v_nestedDistrDescr.push_back(args["dist" + TextTools::toString(nbd)]);

      if (v_nestedDistrDescr.size() != probas.size())
        throw Exception("Number of distributions (keyword 'dist" + TextTools::toString(probas.size()) + "') do not fit the number of probabilities");

      map<string, string> unparsedParameterValuesNested;

      for (unsigned i = 0; i < v_nestedDistrDescr.size(); i++)
        {
          unparsedParameterValuesNested.clear();
          pdd = read(v_nestedDistrDescr[i], unparsedParameterValuesNested, false);

          for (map<string, string>::iterator it = unparsedParameterValuesNested.begin(); it != unparsedParameterValuesNested.end(); it++)
            {
              unparsedParameterValues[distName + "." + TextTools::toString(i + 1) + "_" + it->first] = it->second;
            }
          v_pdd.push_back(pdd);
        }
      rDist = new MixtureOfDiscreteDistributions(v_pdd, probas);
    }
  else
    {
      if (args.find("n") == args.end())
        throw Exception("Missing argument 'n' (number of classes) in " + distName
                        + " distribution");
      unsigned int nbClasses = TextTools::to<unsigned int>(args["n"]);

      if (distName == "Gamma")
        {
          rDist = new GammaDiscreteDistribution(nbClasses, 1, 1);
          if (args.find("alpha") != args.end())
            unparsedParameterValues["Gamma.alpha"] = args["alpha"];
          if (args.find("beta") != args.end())
            unparsedParameterValues["Gamma.beta"] = args["beta"];
        }
      else if (distName == "Gaussian")
        {
          rDist = new GaussianDiscreteDistribution(nbClasses, 0, 1);
          if (args.find("mu") != args.end())
            unparsedParameterValues["Gaussian.mu"] = args["mu"];
          if (args.find("sigma") != args.end())
            unparsedParameterValues["Gaussian.sigma"] = args["sigma"];
        }
      else if (distName == "Beta")
        {
          rDist = new BetaDiscreteDistribution(nbClasses, 1, 1);
          if (args.find("alpha") != args.end())
            unparsedParameterValues["Beta.alpha"] = args["alpha"];
          if (args.find("beta") != args.end())
            unparsedParameterValues["Beta.beta"] = args["beta"];
        }
      else if (distName == "Exponential")
        {
          rDist = new ExponentialDiscreteDistribution(nbClasses, 1);
          if (args.find("lambda") != args.end())
            unparsedParameterValues["Exponential.lambda"] = args["lambda"];
          if (args.find("median") != args.end())
            rDist->setMedian(true);
        }
      else if (distName == "TruncExponential")
        {
          rDist = new TruncatedExponentialDiscreteDistribution(nbClasses, 1, 0);

          if (args.find("median") != args.end())
            rDist->setMedian(true);
          if (args.find("lambda") != args.end())
            unparsedParameterValues["TruncExponential.lambda"] = args["lambda"];
          if (args.find("tp") != args.end())
            unparsedParameterValues["TruncExponential.tp"] = args["tp"];
        }
      else if (distName == "Uniform")
        {
          if (args.find("begin") == args.end())
            throw Exception("Missing argument 'begin' in Uniform distribution");
          if (args.find("end") == args.end())
            throw Exception("Missing argument 'end' in Uniform distribution");
          rDist = new UniformDiscreteDistribution(nbClasses, TextTools::to<double>(args["begin"]),
                                                  TextTools::to<double>(args["end"]));
        }
      else
        {
          throw Exception("Unknown distribution: " + distName + ".");
        }
    }
  if (verbose)
    {
      ApplicationTools::displayResult("Distribution", distName);
      ApplicationTools::displayResult("Number of classes", TextTools::toString(rDist->getNumberOfCategories()));
    }

  return rDist;
}



void BppODiscreteDistributionFormat::write(const DiscreteDistribution& dist,
                                           OutputStream& out,
                                           std::map<std::string, std::string>& globalAliases,
                                           std::vector<std::string>& writtenNames) const
{
  bool comma=false;
  
  const DiscreteDistribution* pd;

  out << dist.getName() + "(";

  const InvariantMixedDiscreteDistribution* invar = dynamic_cast<const InvariantMixedDiscreteDistribution*>(&dist);
  if (invar){
    pd = invar->getVariableSubDistribution();
    out << "dist=";
    write(*pd, out, globalAliases, writtenNames);
    comma=true;
  }
  else {
    const MixtureOfDiscreteDistributions* mix = dynamic_cast<const MixtureOfDiscreteDistributions*>(&dist);
    if (mix){
      unsigned int nd=mix->getNumberOfDistributions();
      for (unsigned int i=0;i<nd;i++){
          if (comma)
            out << ",";
          out << "dist"+TextTools::toString(i+1)+"=";
          write(*mix->getNDistribution(i),out,globalAliases,writtenNames);
          comma=true;
        }
      out << ",probas=(";
      for (unsigned int i=0;i<nd;i++){
        out << mix->getNProbability(i);
        if (i!=nd-1)
          out << ",";
      }
      out << ")";
      for (unsigned int i=1;i<nd;i++)
        writtenNames.push_back("Mixture.theta"+TextTools::toString(i));
    } 
  }
  
  if (dynamic_cast<const BetaDiscreteDistribution*>(&dist) ||
      dynamic_cast<const ExponentialDiscreteDistribution*>(&dist) ||
      dynamic_cast<const GammaDiscreteDistribution*>(&dist) ||
      dynamic_cast<const GaussianDiscreteDistribution*>(&dist) ||
      dynamic_cast<const TruncatedExponentialDiscreteDistribution*>(&dist) ||
      dynamic_cast<const UniformDiscreteDistribution*>(&dist)){
    if (comma)
      out << ",";
    out << "n="  << dist.getNumberOfCategories();
    comma=true;
  }

  const ConstantDistribution* pc=dynamic_cast<const ConstantDistribution*>(&dist);
  if (pc && dist.getNumberOfParameters()==0){
    if (comma)
      out << ",";
    out << "value="  << pc->getLowerBound();
    comma=true;
  }
    
  const SimpleDiscreteDistribution* ps=dynamic_cast<const SimpleDiscreteDistribution*>(&dist);
  if (ps && dist.getNumberOfParameters()==0){
    unsigned int nd=ps->getNumberOfCategories();
    if (comma)
      out << ",";
    out << "values=(";
    for (unsigned int i=0;i<nd;i++){
      out << ps->getCategory(i);
      if (i!=nd-1)
        out << ",";
    }
    out << "),probas=(";
    for (unsigned int i=0;i<nd;i++){
      out << ps->getProbability(i);
      if (i!=nd-1)
        out << ",";
    }
    out << ")";

    for (unsigned int i=1;i<nd;i++)
      writtenNames.push_back("Simple.theta"+TextTools::toString(i));
    for (unsigned int i=1;i<nd+1;i++)
      writtenNames.push_back("Simple.V"+TextTools::toString(i));

    comma=true;
  }
  

  // Writing the parameters 
  BppOParametrizableFormat* bOP=new BppOParametrizableFormat();

  bOP->write(dynamic_cast<const ParameterAliasable*>(&dist), out, globalAliases, dist.getIndependentParameters().getParameterNames(), writtenNames, true, comma);
  out << ")";

  delete bOP;
  
}
  
 
