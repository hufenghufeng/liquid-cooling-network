#include "util.h"

string tokenizer(string& line, const string& delims) {
  string toke;

  // find the beginning position of first token
  string::size_type idx_begin = line.find_first_not_of(delims);

  if (idx_begin != string::npos) {
    string::size_type idx_end = line.find_first_of(delims, idx_begin);

    // last word
    if (idx_end == string::npos) {
      idx_end = line.length();
    }

    // extract the first token and erase it from the input string
    toke = line.substr(idx_begin, idx_end - idx_begin);
    //capitalize(toke);
    line.erase(0, idx_end - 0);
  } // end if

  return toke;
}

string capitalize(string token) {
  string return_token(token);
    for (size_t i = 0; i< token.length(); ++i) {
      return_token[i] = toupper(token[i]);
    }
    return return_token;
}

double to_double(string &str) {
  string delims = "0123456789+-E.";
  string::size_type fig_end = str.find_first_not_of(delims);
  if (fig_end == string::npos) {
    return atof(str.c_str());
  }
  else {
    double fig = atof(str.substr(0, fig_end).c_str());
    return fig;
  }
}
int getNumbits(int num) {


  int bits = 0;
  if (num<0) {
    bits++;
    num = -num;
  }

  while (num >= 1) {
    num = num / 10;
    ++bits;
  }

  return bits;
}
