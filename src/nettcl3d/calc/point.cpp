/*
 * calc/point.cpp --
 *
 * This file is part of nettcl3d application.
 *
 * Copyright (c) 2012 Andrey V. Nakin <andrey.nakin@gmail.com>
 * All rights reserved.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */

#include "point.hpp"

Point operator+(const Point& a, const Point& b) {
	return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point operator-(const Point& a, const Point& b) {
	return Point(a.x - b.x, a.y - b.y, a.z - b.z);
}

double operator*(const Point& a, const Point& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
