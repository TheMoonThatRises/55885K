#ifndef _KRONOS_HELPERS_H_
#define _KRONOS_HELPERS_H_

#include "main.h"
#include <string>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include <cctype>

#define S1(x) #x
#define S2(x) S1(x)
#define LOCATION __FILE__ " (" S2(__LINE__) ")"

using function = std::function<void()>;
using queueFunction = std::function<void(std::vector<std::string>&)>;

#endif