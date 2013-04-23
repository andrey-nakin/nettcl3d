/*
 * calc/point.hpp --
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

#ifndef CALC_POINT_HPP_
#define CALC_POINT_HPP_

#include <vector>
#include <iostream>

class Point {

public:

	double x, y, z;

	Point() : x(0.0), y(0.0), z(0.0) {}
	Point(const double x, const double y, const double z) : x(x), y(y), z(z) {}

	Point& operator+=(const Point& p) {
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}

	Point& operator-=(const Point& p) {
		x -= p.x;
		y -= p.y;
		z -= p.z;
		return *this;
	}

	Point& operator/=(const double v) {
		x /= v;
		y /= v;
		z /= v;
		return *this;
	}

	std::vector<double> toVector() const {
		std::vector<double> result(3);
		result[0] = x;
		result[1] = y;
		result[2] = z;
		return result;
	}

	static Point fromVector(const std::vector<double>& v) {
		Point p;
		if (v.size() > 0) {
			p.x = v[0];
		}
		if (v.size() > 1) {
			p.y = v[1];
		}
		if (v.size() > 2) {
			p.z = v[2];
		}
		return p;
	}

};

Point operator+(const Point& a, const Point& b);
Point operator-(const Point& a, const Point& b);
double operator*(const Point& a, const Point& b);
std::ostream& operator<<(std::ostream&, const Point&);

#endif /* CALC_POINT_HPP_ */
