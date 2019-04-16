#pragma once

#include <string>
#include <map>
#include <memory>
#include <sstream>
#include <functional>
#include <time.h>

#define KXVER 3
#include "k.h"

#define Q(x, e) if(x) { krr((S)e); R 0; }
#define kip(x) kp((S)x.c_str())              // k char array from std::string
#define kts(x) ktj(-KP, pu(x))               // k timestamp from unix nanoseconds since 2000.01.01
#define kis(x) ks((S)x.c_str())              // k symbol from std:string

inline J pu(I u) {R 8.64e13 * (u / 8.64e4 - 10957);} // from integer unix nanos to long int q nanos
inline I up(J f) { return (f / 8.64e13 + 10957) * 8.64e4; } // integer unix nanos from long int q nanos/timestamp
Z K identity(){
  K id = ka(101);
  id->g = 0;
  R id;}

Z K createDictionary(std::map<std::string, K> map){
  K keys = ktn(KS, map.size());
  K vals = ktn(0, 0);
  I i = 0;
  for (auto it = map.begin(); it != map.end(); it++, i++) {
    kS(keys)[i] = ss((S)it->first.c_str());
    jk(&vals, it->second);
  }
  R xD(keys, vals);}

template<typename ... Args>
std::string stringFormat(const std::string& format, Args ... args ){
  size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format.c_str(), args ...);
  return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

static std::string formatTime(const char *str, time_t time, int adjusted){
  static char buffer[4096];
  struct tm *timeinfo = gmtime(&time);
  if (adjusted) timeinfo->tm_hour -= 1;
  strftime(buffer, sizeof(buffer), str, timeinfo);
  return stringFormat("%s", buffer);
}

static std::string getString(K x){
  if (abs(x->t) != KC)
    throw std::runtime_error("type");
  if (x->t == -KC)
    return x->s;
  std::stringstream ss;
  DO(xn, ss << kc(kC(x)[i])->g);
  R ss.str();
}
