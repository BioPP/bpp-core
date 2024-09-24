// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_APP_BPPAPPLICATION_H
#define BPP_APP_BPPAPPLICATION_H


#include "../Exceptions.h"

// From the STL:
#include <string>
#include <map>

namespace bpp
{
class BppApplication
{
protected:
  std::string appName_;
  mutable std::map<std::string, std::string> params_;
  bool timerStarted_;
  bool verbose_;
  int warn_;

public:
  BppApplication(int argc, char* argv[], const std::string& name, bool verbose = true, int warningLevel = 1);
  virtual ~BppApplication() {}

public:
  virtual void startTimer();
  virtual void done();

  virtual const std::map<std::string, std::string>& getParams() const { return params_; }
  virtual std::map<std::string, std::string>& getParams() { return params_; }

  virtual const std::string& getParam(const std::string& name) const
  {
    if (params_.find(name) == params_.end()) throw Exception("BppApplication::getParam(). Parameter '" + name + "' not found.");
    return params_[name];
  }

  virtual std::string& getParam(const std::string& name) { return params_[name]; }

  virtual bool isVerbose() const { return verbose_; }

  virtual int getWarningLevel() const { return warn_; }

  virtual void setWarningLevel(int warn) { warn_ = warn; }

  virtual void help(const std::string& program) const;
};
} // end of namespace bpp;
#endif // BPP_APP_BPPAPPLICATION_H
