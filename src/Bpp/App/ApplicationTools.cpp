// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "ApplicationTools.h"

using namespace bpp;

// From the STL:
#include <cmath>

using namespace std;

/******************************************************************************/

std::shared_ptr<OutputStream> ApplicationTools::error(new StdErr());
std::shared_ptr<OutputStream> ApplicationTools::message(new StdOut());
std::shared_ptr<OutputStream> ApplicationTools::warning(new StdOut());
time_t ApplicationTools::startTime;
size_t ApplicationTools::terminalWidth = 80;
float ApplicationTools::terminalSplit = 0.5;
bool ApplicationTools::interactive = true;
int ApplicationTools::warningLevel = 0;


/******************************************************************************/

vector<string> ApplicationTools::matchingParameters(const string& pattern, const map<string, string>& params)
{
  vector<string> retv;

  for (const auto& it : params)
  {
    StringTokenizer stj(pattern, "*", true, false);
    size_t pos1, pos2;
    string parn = it.first;
    bool flag(true);
    string g = stj.nextToken();
    pos1 = parn.find(g);
    if (pos1 != 0)
      flag = false;
    pos1 += g.length();
    while (flag && stj.hasMoreToken())
    {
      g = stj.nextToken();
      pos2 = parn.find(g, pos1);
      if (pos2 == string::npos)
      {
        flag = false;
        break;
      }
      pos1 = pos2 + g.length();
    }
    if (flag &&
        ((g.length() == 0) || (pos1 == parn.length()) || (parn.rfind(g) == parn.length() - g.length())))
      retv.push_back(parn);
  }

  return retv;
}

vector<string> ApplicationTools::matchingParameters(const string& pattern, vector<string>& params)
{
  vector<string> retv;

  for (size_t i = 0; i < params.size(); i++)
  {
    StringTokenizer stj(pattern, "*", true, false);
    size_t pos1, pos2;
    string parn = params.at(i);
    bool flag(true);
    string g = stj.nextToken();
    pos1 = parn.find(g);
    if (pos1 != 0)
      flag = false;
    pos1 += g.length();
    while (flag && stj.hasMoreToken())
    {
      g = stj.nextToken();
      pos2 = parn.find(g, pos1);
      if (pos2 == string::npos)
      {
        flag = false;
        break;
      }
      pos1 = pos2 + g.length();
    }
    if (flag &&
        ((g.length() == 0) || (pos1 == parn.length()) || (parn.rfind(g) == parn.length() - g.length())))
      retv.push_back(parn);
  }

  return retv;
}

/******************************************************************************/

string ApplicationTools::getAFilePath(
  const string& parameter,
  const map<string, string>& params,
  bool isRequired,
  bool mustExist,
  const string& suffix,
  bool suffixIsOptional,
  const string& defaultPath,
  int warn)
{
  string filePath = getStringParameter(parameter, params, defaultPath, suffix, suffixIsOptional, warn);
  if (filePath == "")
    filePath = "none";
  if (filePath == "none" && isRequired)
  {
    throw Exception("You must specify a file for this parameter: " + parameter + (suffixIsOptional ? "" : suffix));
  }
  if (filePath == "none")
    return filePath;
  if (mustExist && !FileTools::fileExists(filePath))
  {
    throw Exception("File does not exists: " + filePath);
  }
  return filePath;
}

/******************************************************************************/

double ApplicationTools::getDoubleParameter(
  const string& parameterName,
  const map<string, string>& params,
  double defaultValue,
  const string& suffix,
  bool suffixIsOptional,
  int warn)
{
  double dParam = defaultValue;
  if (parameterExists(parameterName + suffix, params))
  {
    dParam = TextTools::toDouble(params.at(parameterName + suffix));
  }
  else if (suffixIsOptional && parameterExists(parameterName, params))
  {
    dParam = TextTools::toDouble(params.at(parameterName));
  }
  else if (warn <= warningLevel)
  {
    displayWarning("Parameter " + parameterName + suffix + " not specified. Default used instead: " + TextTools::toString(defaultValue));
  }
  return dParam;
}

/******************************************************************************/

int ApplicationTools::getIntParameter(
  const string& parameterName,
  const map<string, string>& params,
  int defaultValue,
  const string& suffix,
  bool suffixIsOptional,
  int warn)
{
  int iParam = defaultValue;
  if (parameterExists(parameterName + suffix, params))
  {
    iParam = TextTools::toInt(params.at(parameterName + suffix));
  }
  else if (suffixIsOptional && parameterExists(parameterName, params))
  {
    iParam = TextTools::toInt(params.at(parameterName));
  }
  else if (warn <= warningLevel)
  {
    displayWarning("Parameter " + parameterName + suffix + " not specified. Default used instead: " + TextTools::toString(defaultValue));
  }
  return iParam;
}

/******************************************************************************/


bool ApplicationTools::getBooleanParameter(
  const string& parameterName,
  const map<string, string>& params,
  bool defaultValue,
  const string& suffix,
  bool suffixIsOptional,
  int warn)
{
  string sParam;
  bool bParam = defaultValue;
  if (parameterExists(parameterName + suffix, params))
  {
    sParam = params.at(parameterName + suffix);
  }
  else if (suffixIsOptional && parameterExists(parameterName, params))
  {
    sParam = params.at(parameterName);
  }
  else
  {
    if (warn <= warningLevel)
    {
      displayWarning("Parameter " + parameterName + " not specified. Default used instead: " + TextTools::toString(defaultValue));
    }
    return bParam;
  }
  if ((sParam == "true")
      || (sParam == "TRUE")
      || (sParam == "t")
      || (sParam == "T")
      || (sParam == "yes")
      || (sParam == "YES")
      || (sParam == "y")
      || (sParam == "Y")
      || (sParam == "1"))
    bParam = true;
  else if ((sParam == "false")
           || (sParam == "FALSE")
           || (sParam == "f")
           || (sParam == "F")
           || (sParam == "no")
           || (sParam == "NO")
           || (sParam == "n")
           || (sParam == "N")
           || (sParam == "0"))
    bParam = false;
  else
    throw Exception("ApplicationTools::getBooleanParameter. Wrong description:" + sParam);

  return bParam;
}

/******************************************************************************/

void ApplicationTools::displayMessage(const string& text)
{
  if (message)
    (*message << text).endLine();
}

void ApplicationTools::displayError(const string& text)
{
  if (error)
    (*error << "ERROR!!! " << text).endLine();
}

void ApplicationTools::displayWarning(const string& text)
{
  if (warning)
    (*warning << "WARNING!!! " << text).endLine();
}

void ApplicationTools::displayTask(const string& text, bool eof)
{
  if (message)
  {
    *message << TextTools::resizeRight(text, static_cast<size_t>(static_cast<float>(terminalWidth) * terminalSplit - 1), '.') << ": ";
    if (eof)
      message->endLine();
    else
      message->flush();
  }
}

void ApplicationTools::displayTaskDone()
{
  if (message)
    (*message << "Done.").endLine();
}

/******************************************************************************/

void ApplicationTools::displayGauge(size_t iter, size_t total, char symbol, const string& mes)
{
  if (!message)
    return;
  if (total == 0)
    return; // We do not display anything in that case.
  size_t width = static_cast<size_t>(static_cast<float>(terminalWidth) * terminalSplit - 2);
  string gauge = string(static_cast<size_t>((1. * static_cast<double>(iter) / static_cast<double>(total)) * static_cast<double>(width)), symbol);
  string info = mes;
  size_t step = static_cast<size_t>(ceil(1. * static_cast<double>(total) / static_cast<double>(width)));
  size_t x = iter % step;
  if (interactive)
  {
    string fill = string(width - gauge.length(), ' ');
    gauge = "[" + gauge + fill + "] " + TextTools::resizeLeft(TextTools::toString(100 * iter / total), 3) + "%";
    if (mes.size() > terminalWidth - gauge.size())
      info = TextTools::resizeRight(mes, terminalWidth - gauge.size());
    if (x == 0 || iter == total)
    {
      *message << '\r' + info + gauge; message->flush();
    }
  }
  else
  {
    if (iter == 0)
    {
      *message << "[";
      message->flush();
      return;
    }
    if (iter >= total)
    {
      size_t fill = static_cast<size_t>(static_cast<float>(terminalWidth) * terminalSplit) - (total - 1) / step - 1;
      *message << TextTools::resizeLeft("]", fill, symbol);
      message->flush();
      return;
    }
    if (x == 0)
    {
      *message << symbol; message->flush();
    }
  }
}

/******************************************************************************/

void ApplicationTools::displayUnlimitedGauge(size_t iter, const string& mes)
{
  if (!message)
    return;
  string chars = "-/-\\";
  string info = mes;
  if (interactive)
  {
    unsigned int i = iter % 4;
    *message << '\r' << info << chars[i] << " " << TextTools::toString(iter);
    message->flush();
  }
  else
  {
    if (iter == 0)
      *message << info;
    *message << "*";
    message->flush();
    return;
  }
}

/******************************************************************************/

void ApplicationTools::displayTime(const string& msg)
{
  time_t endTime;
  time(&endTime);
  if (message)
  {
    double nsec = difftime(endTime, startTime);
    double nmin = floor(nsec / 60.);
    double nhou = floor(nmin / 60.);
    double nday = floor(nhou / 24.);
    nhou = nhou - nday * 24;
    nmin = nmin - (nday * 24 + nhou) * 60;
    nsec = nsec - ((nday * 24 + nhou) * 60 + nmin) * 60;
    *message << msg << " ";
    *message << nday << "d, ";
    *message << nhou << "h, ";
    *message << nmin << "m, ";
    *message << nsec << "s.";
    message->endLine();
  }
}

/******************************************************************************/

double ApplicationTools::getTime()
{
  time_t endTime;
  time(&endTime);
  return difftime(endTime, startTime);
}

/******************************************************************************/
