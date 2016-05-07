#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <cctype>
#include <cstdlib>

using std::string;

string tokenizer(string& str, const string& de);

string capitalize(string token);

double to_double(string &str);

int getNumbits(int num);

#endif
