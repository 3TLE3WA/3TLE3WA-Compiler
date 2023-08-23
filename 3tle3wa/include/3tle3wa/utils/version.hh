#pragma once

#define MAJOR 1
#define MINOR 0

#define TOSTR_(x) #x
#define TOSTR(x) TOSTR_(x)

#define VERSION TOSTR(MAJOR) "." TOSTR(MINOR)