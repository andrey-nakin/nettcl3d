/*
 * tracer/voltage.hpp --
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

#ifndef TRACER_VOLTAGE_HPP_
#define TRACER_VOLTAGE_HPP_

#include "index_tracer.hpp"

namespace tracer {

	struct VoltageWorker {

		void writeHeader(std::ostream& s, const Network::index_type index) const {
			s << "# time\tvoltage[" << index << "]\n";
		}

		void trace(std::ostream& s, const Network& network, const Network::index_type index) const {
			s << network.contact(index).voltage;
		}

		static const char* fileNameFormat() {
			return "u.%u";
		}
	};

	class Voltage : public IndexTracer<VoltageWorker> {

	public:

		Voltage(const Params& params) : IndexTracer<VoltageWorker>(params) {}

	};

}

#endif /* TRACER_VOLTAGE_HPP_ */
