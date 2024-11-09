//
// Created by jujur on 30/10/24.
//

#include "main.h"

void fatalf(const char* fmt, ...) {
    // printf and exit with error
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    exit(1);
}