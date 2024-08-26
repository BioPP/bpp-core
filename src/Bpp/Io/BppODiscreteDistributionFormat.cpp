// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../Io/FileTools.h"
#include "../Numeric/AutoParameter.h"
#include "../Numeric/Prob/BetaDiscreteDistribution.h"
#include "../Numeric/Prob/ConstantDistribution.h"
#include "../Numeric/Prob/DiscreteDistribution.h"
#include "../Numeric/Prob/ExponentialDiscreteDistribution.h"
#include "../Numeric/Prob/GammaDiscreteDistribution.h"
#include "../Numeric/Prob/GaussianDiscreteDistribution.h"
#include "../Numeric/Prob/InvariantMixedDiscreteDistribution.h"
#include "../Numeric/Prob/MixtureOfDiscreteDistributions.h"
#include "../Numeric/Prob/SimpleDiscreteDistribution.h"
#include "../Numeric/Prob/TruncatedExponentialDiscreteDistribution.h"
#include "../Numeric/Prob/UniformDiscreteDistribution.h"
#include "../Text/KeyvalTools.h"
#include "../Text/StringTokenizer.h"
#include "../Text/TextTools.h"
#include "BppODiscreteDistributionFormat.h"
#include "BppOParametrizableFormat.h"

using namespace bpp;

// From the STL:
#include <iomanip>

using namespace std;


unique_ptr<DiscreteDistributionInterface> BppODiscreteDistributionFormat::readDiscreteDistribution(
    const std::string& distDescription,
    bool parseArguments)
{
  unparsedArguments_.clear();
  string distName;
  unique_ptr<DiscreteDistributionInterface> rDist;
  map<string, string> args;
  KeyvalTools::parseProcedure(distDescription, distName, args);

  if ((distName == "InvariantMixed") || (distName == "Invariant"))
  {
    // We have to parse the nested distribution first:
    string nestedDistDescription = args["dist"];
    if (TextTools::isEmpty(nestedDistDescription))
      throw Exception("BppODiscreteDistributionFormat::read. Missing argument 'dist' for distribution 'Invariant'.");
    if (verbose_)
      ApplicationTools::displayResult("Invariant Mixed distribution", distName );
    BppODiscreteDistributionFormat nestedReader(verbose_);
    auto nestedDistribution = nestedReader.readDiscreteDistribution(nestedDistDescription, true);
    map<string, string> unparsedArgumentsNested(nestedReader.getUnparsedArguments());

    // Now we create the Invariant rate distribution:
    rDist = make_unique<InvariantMixedDiscreteDistribution>(std::move(nestedDistribution), 0.1, 0.000001);

    // Then we update the parameter set:
    for (auto& it : unparsedArgumentsNested)
    {
      unparsedArguments_["Invariant." + it.first] = it.second;
    }

    if (args.find("p") != args.end())
      unparsedArguments_["Invariant.p"] = args["p"];
  }
  else if (distName == "Constant")
  {
    if (args.find("value") == args.end())
      throw Exception("Missing argument 'value' in Constant distribution");
    rDist = make_unique<ConstantDistribution>(TextTools::to<double>(args["value"]));
    unparsedArguments_["Constant.value"] = args["value"];
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

    std::map<size_t, std::vector<double>> ranges;

    if (args.find("ranges") != args.end())
    {
      string rr = args["ranges"];
      StringTokenizer strtok3(rr.substr(1, rr.length() - 2), ",");
      string desc;
      double deb, fin;
      unsigned int num;
      size_t po, pf, ppv;
      while (strtok3.hasMoreToken())
      {
        desc = strtok3.nextToken();
        po = desc.find("[");
        ppv = desc.find(";");
        pf = desc.find("]");
        num = (unsigned int)(TextTools::toInt(desc.substr(1, po - 1)));
        deb = TextTools::toDouble(desc.substr(po + 1, ppv - po - 1));
        fin = TextTools::toDouble(desc.substr(ppv + 1, pf - ppv - 1));
        vector<double> vd;
        vd.push_back(deb);
        vd.push_back(fin);
        ranges[num] = vd;
      }
    }
    if (ranges.size() == 0)
      rDist = make_unique<SimpleDiscreteDistribution>(values, probas);
    else
      rDist = make_unique<SimpleDiscreteDistribution>(values, ranges, probas);

    vector<string> v = rDist->getParameters().getParameterNames();

    for (auto i : v)
    {
      unparsedArguments_[i] = TextTools::toString(rDist->getParameterValue(rDist->getParameterNameWithoutNamespace(i)));
    }
  }
  else if (distName == "Mixture")
  {
    if (args.find("probas") == args.end())
      throw Exception("Missing argument 'probas' in Mixture distribution");
    vector<double> probas;
    vector<unique_ptr<DiscreteDistributionInterface>> v_pdd;
    unique_ptr<DiscreteDistributionInterface> pdd;
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

    BppODiscreteDistributionFormat nestedReader;

    for (unsigned i = 0; i < v_nestedDistrDescr.size(); ++i)
    {
      pdd = nestedReader.readDiscreteDistribution(v_nestedDistrDescr[i], true);
      map<string, string> unparsedArgumentsNested(nestedReader.getUnparsedArguments());

      for (auto& it : unparsedArgumentsNested)
      {
        unparsedArguments_[distName + "." + TextTools::toString(i + 1) + "_" + it.first] = it.second;
      }
      v_pdd.push_back(std::move(pdd));
    }
    rDist = make_unique<MixtureOfDiscreteDistributions>(v_pdd, probas);
  }
  else
  {
    if (args.find("n") == args.end())
      throw Exception("Missing argument 'n' (number of classes) in " + distName
          + " distribution");
    unsigned int nbClasses = TextTools::to<unsigned int>(args["n"]);

    if (distName == "Gamma")
    {
      double offset = 0;

      if (args.find("offset") != args.end())
        try
        {
          offset = TextTools::toDouble(args["offset"]);
        }
        catch (Exception& e)
        {}

      if (args.find("ParamOffset") != args.end())
        rDist.reset(new GammaDiscreteDistribution(nbClasses, 1, 1, true, offset));
      else
        rDist.reset(new GammaDiscreteDistribution(nbClasses, 1, 1, false, offset));

      if (args.find("alpha") != args.end())
        unparsedArguments_["Gamma.alpha"] = args["alpha"];
      if (args.find("beta") != args.end())
        unparsedArguments_["Gamma.beta"] = args["beta"];
      if (args.find("offset") != args.end())
        unparsedArguments_["Gamma.offset"] = args["offset"];
    }
    else if (distName == "Gaussian")
    {
      rDist.reset(new GaussianDiscreteDistribution(nbClasses, 0, 1));
      if (args.find("mu") != args.end())
        unparsedArguments_["Gaussian.mu"] = args["mu"];
      if (args.find("sigma") != args.end())
        unparsedArguments_["Gaussian.sigma"] = args["sigma"];
    }
    else if (distName == "Beta")
    {
      rDist.reset(new BetaDiscreteDistribution(nbClasses, 1, 1));
      if (args.find("alpha") != args.end())
        unparsedArguments_["Beta.alpha"] = args["alpha"];
      if (args.find("beta") != args.end())
        unparsedArguments_["Beta.beta"] = args["beta"];
    }
    else if (distName == "Exponential")
    {
      rDist.reset(new ExponentialDiscreteDistribution(nbClasses, 1));
      if (args.find("lambda") != args.end())
        unparsedArguments_["Exponential.lambda"] = args["lambda"];
      if (args.find("median") != args.end())
        rDist->setMedian(true);
    }
    else if (distName == "TruncExponential")
    {
      rDist.reset(new TruncatedExponentialDiscreteDistribution(nbClasses, 1, 0));

      if (args.find("median") != args.end())
        rDist->setMedian(true);
      if (args.find("lambda") != args.end())
        unparsedArguments_["TruncExponential.lambda"] = args["lambda"];
      if (args.find("tp") != args.end())
        unparsedArguments_["TruncExponential.tp"] = args["tp"];
    }
    else if (distName == "Uniform")
    {
      if (args.find("begin") == args.end())
        throw Exception("Missing argument 'begin' in Uniform distribution");
      if (args.find("end") == args.end())
        throw Exception("Missing argument 'end' in Uniform distribution");
      rDist.reset(new UniformDiscreteDistribution(
          nbClasses,
          TextTools::to<double>(args["begin"]),
          TextTools::to<double>(args["end"])));
    }
    else
    {
      throw Exception("Unknown distribution: " + distName + ".");
    }
  }
  if (verbose_)
  {
    ApplicationTools::displayResult("Distribution", distName);
    ApplicationTools::displayResult("Number of classes", TextTools::toString(rDist->getNumberOfCategories()));
  }

  if (parseArguments)
    initialize_(*rDist);

  return rDist;
}


void BppODiscreteDistributionFormat::writeDiscreteDistribution(
    const DiscreteDistributionInterface& dist,
    OutputStream& out,
    std::map<std::string, std::string>& globalAliases,
    std::vector<std::string>& writtenNames) const
{
  bool comma = false;

  out << dist.getName() + "(";

  auto* invar = dynamic_cast<const InvariantMixedDiscreteDistribution*>(&dist);
  if (invar)
  {
    auto& pd = invar->variableSubDistribution();
    out << "dist=";
    writeDiscreteDistribution(pd, out, globalAliases, writtenNames);
    comma = true;
  }
  else
  {
    try
    {
      auto& mix = dynamic_cast<const MixtureOfDiscreteDistributions&>(dist);
      size_t nd = mix.getNumberOfDistributions();
      for (size_t i = 0; i < nd; ++i)
      {
        if (comma)
          out << ",";
        out << "dist" + TextTools::toString(i + 1) + "=";
        writeDiscreteDistribution(mix.nDistribution(i), out, globalAliases, writtenNames);
        comma = true;
      }
      out << ",probas=(";
      for (size_t i = 0; i < nd; ++i)
      {
        out << mix.getNProbability(i);
        if (i != nd - 1)
          out << ",";
      }
      out << ")";
      for (size_t i = 1; i < nd; ++i)
      {
        writtenNames.push_back(mix.getNamespace() + "theta" + TextTools::toString(i));
      }
    }
    catch (bad_cast&)
    {}
  }

  if (dynamic_cast<const BetaDiscreteDistribution*>(&dist) ||
      dynamic_cast<const ExponentialDiscreteDistribution*>(&dist) ||
      dynamic_cast<const GammaDiscreteDistribution*>(&dist) ||
      dynamic_cast<const GaussianDiscreteDistribution*>(&dist) ||
      dynamic_cast<const TruncatedExponentialDiscreteDistribution*>(&dist) ||
      dynamic_cast<const UniformDiscreteDistribution*>(&dist))
  {
    if (comma)
      out << ",";
    out << "n="  << dist.getNumberOfCategories();
    comma = true;
  }

  try
  {
    auto& pc = dynamic_cast<const ConstantDistribution&>(dist);
    if (dist.getNumberOfParameters() == 0)
    {
      if (comma)
        out << ",";
      out << "value="  << pc.getLowerBound();
      comma = true;
    }
  }
  catch (bad_cast&)
  {}

  try
  {
    auto& ps = dynamic_cast<const SimpleDiscreteDistribution&>(dist);
    size_t nd = ps.getNumberOfCategories();
    if (comma)
      out << ",";
    out << "values=(";
    for (size_t i = 0; i < nd; ++i)
    {
      out << ps.getCategory(i);
      if (i != nd - 1)
        out << ",";
    }
    out << "),probas=(";
    for (size_t i = 0; i < nd; ++i)
    {
      out << ps.getProbability(i);
      if (i != nd - 1)
        out << ",";
    }
    out << ")";

    auto range = ps.getRanges();
    if (range.size() != 0)
    {
      out << ",ranges=(";
      auto it(range.begin());
      while (it != range.end())
      {
        out << "V" << TextTools::toString(it->first);
        out << "[" << TextTools::toString(it->second[0]) << ";" << TextTools::toString(it->second[1]) << "]";
        it++;
        if (it != range.end())
          out << ",";
      }
      out << ")";
    }

    for (size_t i = 1; i < nd; ++i)
    {
      writtenNames.push_back(ps.getNamespace() + "theta" + TextTools::toString(i));
    }
    for (size_t i = 1; i < nd + 1; ++i)
    {
      writtenNames.push_back(ps.getNamespace() + "V" + TextTools::toString(i));
    }
    comma = true;
  }
  catch (bad_cast&)
  {}

  // Writing the parameters
  BppOParametrizableFormat bOP;
  bOP.write(dynamic_cast<const ParameterAliasable&>(dist), out, globalAliases, dist.getIndependentParameters().getParameterNames(), writtenNames, true, comma);
  out << ")";
}

void BppODiscreteDistributionFormat::initialize_(
    DiscreteDistributionInterface& rDist)
{
  ParameterList pl = rDist.getIndependentParameters();

  for (size_t i = 0; i < pl.size(); ++i)
  {
    AutoParameter ap(pl[i]);
    ap.setMessageHandler(ApplicationTools::warning);
    pl.setParameter(i, ap);
  }

  for (size_t i = 0; i < pl.size(); ++i)
  {
    const string pName = pl[i].getName();
    double value = ApplicationTools::getDoubleParameter(pName, unparsedArguments_, pl[i].getValue());
    pl[i].setValue(value);
    if (verbose_)
      ApplicationTools::displayResult("Parameter found", pName + "=" + TextTools::toString(pl[i].getValue()));
  }

  rDist.matchParametersValues(pl);
  if (verbose_)
  {
    for (size_t c = 0; c < rDist.getNumberOfCategories(); ++c)
    {
      ApplicationTools::displayResult("- Category " + TextTools::toString(c)
          + " (Pr = " + TextTools::toString(rDist.getProbability(c)) + ") rate",
          TextTools::toString(rDist.getCategory(c)));
    }
  }
}
