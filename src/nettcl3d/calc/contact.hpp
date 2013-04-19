/*
 * calc/contact.hpp --
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

#ifndef CONTACT_HPP_
#define CONTACT_HPP_

#include <string>
#include <set>
#include <exception>
#include "point.hpp"
#include "tagable.hpp"

struct Contact : public Tagable {

	double beta, tau, v;

	Point h, hNormal;
	double phase;
	double voltage;

	Contact() :
		beta(1.0), tau(1.0), v(40.0), phase(0.0), voltage(0.0) {}

	Contact(double const beta, double const tau, double const v) :
		beta(beta), tau(tau), v(v), phase(0.0), voltage(0.0) {}

};

#endif /* CONTACT_HPP_ */
