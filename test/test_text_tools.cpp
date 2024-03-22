// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Text/TextTools.h>
#include <iostream>
#include <string>

namespace TextTools = bpp::TextTools;
using bpp::StringTokenizer;

TEST_CASE("string capitalization")
{
  CHECK(TextTools::toUpper("aBcDEF ") == "ABCDEF ");
  CHECK(TextTools::toLower("aBcDEF ") == "abcdef ");
}

TEST_CASE("string stripping")
{
  std::string base_text = "abc def";
  std::string base_text_no_spaces = "abcdef";

  std::string empty = "";
  std::string all_space = " \t\n";
  std::string no_space = base_text;
  std::string leading_space_only = "  " + base_text;
  std::string trailing_space_only = base_text + "  ";
  std::string multi_space = "  " + base_text + "  ";

  SUBCASE("Remove all whitespace")
  {
    CHECK(TextTools::removeWhiteSpaces(empty) == empty);
    CHECK(TextTools::removeWhiteSpaces(all_space) == empty);
    CHECK(TextTools::removeWhiteSpaces(no_space) == base_text_no_spaces);
    CHECK(TextTools::removeWhiteSpaces(leading_space_only) == base_text_no_spaces);
    CHECK(TextTools::removeWhiteSpaces(trailing_space_only) == base_text_no_spaces);
    CHECK(TextTools::removeWhiteSpaces(multi_space) == base_text_no_spaces);
  }
  SUBCASE("Remove leading space")
  {
    CHECK(TextTools::removeFirstWhiteSpaces(empty) == "");
    CHECK(TextTools::removeFirstWhiteSpaces(all_space) == "");
    CHECK(TextTools::removeFirstWhiteSpaces(no_space) == base_text);
    CHECK(TextTools::removeFirstWhiteSpaces(leading_space_only) == base_text);
    CHECK(TextTools::removeFirstWhiteSpaces(trailing_space_only) == trailing_space_only);
    CHECK(TextTools::removeFirstWhiteSpaces(multi_space) == trailing_space_only);
  }
  SUBCASE("Remove trailing space")
  {
    CHECK(TextTools::removeLastWhiteSpaces(empty) == "");
    CHECK(TextTools::removeLastWhiteSpaces(all_space) == "");
    CHECK(TextTools::removeLastWhiteSpaces(no_space) == base_text);
    CHECK(TextTools::removeLastWhiteSpaces(leading_space_only) == leading_space_only);
    CHECK(TextTools::removeLastWhiteSpaces(trailing_space_only) == base_text);
    CHECK(TextTools::removeLastWhiteSpaces(multi_space) == leading_space_only);
  }
  SUBCASE("Remove leading and trailing space")
  {
    CHECK(TextTools::removeSurroundingWhiteSpaces(empty) == "");
    CHECK(TextTools::removeSurroundingWhiteSpaces(all_space) == "");
    CHECK(TextTools::removeSurroundingWhiteSpaces(no_space) == base_text);
    CHECK(TextTools::removeSurroundingWhiteSpaces(leading_space_only) == base_text);
    CHECK(TextTools::removeSurroundingWhiteSpaces(trailing_space_only) == base_text);
    CHECK(TextTools::removeSurroundingWhiteSpaces(multi_space) == base_text);
  }
}

TEST_CASE("string conversion")
{
  CHECK_FALSE(TextTools::isDecimalNumber("aazz"));
  CHECK_FALSE(TextTools::isDecimalNumber("-aazz"));
  CHECK_FALSE(TextTools::isDecimalNumber("-3.45z"));
  CHECK(TextTools::isDecimalNumber("0"));
  CHECK(TextTools::isDecimalNumber("123"));
  CHECK(TextTools::isDecimalNumber("-123"));
  CHECK(TextTools::isDecimalNumber("-123.456"));
  CHECK(TextTools::isDecimalInteger("123456"));
  CHECK(TextTools::isDecimalInteger("-7890"));
  CHECK(TextTools::isDecimalNumber("-123.456e-5"));
  CHECK_FALSE(TextTools::isDecimalNumber("-123.456e-5.8"));
  CHECK(TextTools::isDecimalInteger("-123e6"));
  CHECK_FALSE(TextTools::isDecimalInteger("-123.456e5"));
  CHECK_FALSE(TextTools::isDecimalInteger("-123e-6"));
}

TEST_CASE("string resizing")
{
  std::string t = "hello world";
  CHECK(TextTools::resizeRight(t, 4) == "hell");
  CHECK(TextTools::resizeRight(t, 20) == t + std::string(20 - t.size(), ' '));
  CHECK(TextTools::resizeLeft(t, 4) == "orld");
  CHECK(TextTools::resizeLeft(t, 20) == std::string(20 - t.size(), ' ') + t);
}

TEST_CASE("string splitting")
{
  std::vector<std::string> r1 {"hell", "o wo", "rld"};
  CHECK(TextTools::split("hello world", 4) == r1);
  std::vector<std::string> r2 {"hell", "o wo", "rld!"};
  CHECK(TextTools::split("hello world!", 4) == r2);
}

TEST_CASE("string search")
{
  std::string t = "hello world world !";

  SUBCASE("startswith")
  {
    CHECK(TextTools::startsWith(t, "hello"));
    CHECK_FALSE(TextTools::startsWith(t, "blah"));
    CHECK(TextTools::startsWith(t, ""));
    CHECK_FALSE(TextTools::startsWith("", "blah"));
  }
  SUBCASE("endswith")
  {
    CHECK(TextTools::endsWith(t, "world !"));
    CHECK_FALSE(TextTools::endsWith(t, "blah"));
    CHECK(TextTools::endsWith(t, ""));
    CHECK_FALSE(TextTools::endsWith("", "blah"));
  }
  SUBCASE("hasSubstring")
  {
    CHECK(TextTools::hasSubstring(t, "world"));
    CHECK_FALSE(TextTools::hasSubstring(t, "blah"));
    CHECK(TextTools::hasSubstring(t, ""));
    CHECK_FALSE(TextTools::hasSubstring("", "blah"));
  }
  SUBCASE("count")
  {
    CHECK(TextTools::count(t, "world") == 2);
    CHECK(TextTools::count(t, "blah") == 0);
    CHECK(TextTools::count(t, "hell") == 1);
    CHECK(TextTools::count("", "blah") == 0);
  }
}

TEST_CASE("string replace")
{
  std::string t = "hello world world !";
  TextTools::replaceAll (t, "world", "sun");
  CHECK (t == "hello sun sun !");
  std::string t2 = "hell";
  TextTools::replaceAll (t2, "world", "sun");
  CHECK (t2 == "hell");
}

TEST_CASE("string tokenizer")
{
  SUBCASE("Case 1")
  {
    StringTokenizer st(" aaazzer  aeerd a    eer", " \t", false, false);
    CHECK(st.numberOfRemainingTokens() == 4);
    CHECK(st.nextToken() == "aaazzer");
    CHECK(st.nextToken() == "aeerd");
    CHECK(st.nextToken() == "a");
    CHECK(st.nextToken() == "eer");
  }
  SUBCASE("Case 2")
  {
    StringTokenizer st(" aaazzer  aeerd a    eer", " \t", false, true);
    CHECK(st.numberOfRemainingTokens() == 8);
    CHECK(st.nextToken() == "aaazzer");
    CHECK(st.nextToken() == "");
    CHECK(st.nextToken() == "aeerd");
    CHECK(st.nextToken() == "a");
    CHECK(st.nextToken() == "");
    CHECK(st.nextToken() == "");
    CHECK(st.nextToken() == "");
    CHECK(st.nextToken() == "eer");
  }
  SUBCASE("Case 3")
  {
    StringTokenizer st(" aaazzer  aeerd a    eer", " \t", true, false);
    CHECK(st.numberOfRemainingTokens() == 1);
    CHECK(st.nextToken() == " aaazzer  aeerd a    eer");
  }
}
