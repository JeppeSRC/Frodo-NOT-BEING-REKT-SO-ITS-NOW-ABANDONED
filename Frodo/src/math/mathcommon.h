#pragma once
#include <fd.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <intrin.h>

#define FD_PRE_TO_RADIANS 0.01745329251994329576923690768489
#define FD_PRE_TO_DEGREES 57.295779513082320876798154814105

#define FD_TO_RADIANS(x) (x * FD_PRE_TO_RADIANS)
#define FD_TO_DEGREES(x) (x * FD_PRE_TO_DEGREES)

#define FD_PI M_PI


