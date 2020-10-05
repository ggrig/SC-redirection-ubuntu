#pragma once

#include <stdio.h>
#include <string>
#include <vector>

void hexDump(const char * desc, const void * addr, size_t len);
void hexDump(const char * desc, std::vector<unsigned char> data);
