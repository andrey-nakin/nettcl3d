/*
 * tracer/flux_section.hpp --
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

#ifndef TRACER_FLUX_SECTION_HPP_
#define TRACER_FLUX_SECTION_HPP_

#include <functional>
#include <boost/optional.hpp>
#include <boost/multi_array.hpp>
#include "null.hpp"

namespace tracer {

	class FluxSection : public Null {
	public:

		typedef unsigned Position;

		struct Params {

			typedef enum {X, Y, Z} Plane;

			std::string fileName;
			Plane plane;
			Position position;

			Params() : fileName("flux.section") {}
		};

		FluxSection(const Params& params) : params(params) {}

	private:

		const Params params;

		virtual void doAfterRun(const Network& network) {
			boost::optional<Position> mina, maxa, minb, maxb;
			const std::string plane = getPlaneProperty(params.plane), 
				aprop = getAProperty(params.plane), bprop = getBProperty(params.plane);
			boost::multi_array<boost::optional<Position>, 2> indices;

			// determine ranges of (a, b) coordinates on plane
			for (Network::circuit_const_iterator first = network.circuitBegin(), i = first ,last = network.circuitEnd();
					i != last; ++i) {

				if (i->hasTag(plane) && i->getTypedProp<Position>(plane) == params.position) {
					const Position a = i->getTypedProp<Position>(aprop);
					check(mina, a, std::less<Position>());
					check(maxa, a, std::greater<Position>());

					const Position b = i->getTypedProp<Position>(bprop);
					check(minb, b, std::less<Position>());
					check(maxb, b, std::greater<Position>());

					indices[a][b] = std::distance(first, i);
				}
			}
			
			// check if at least one circuit exists on plane
			if (!mina.is_initialized() || params.fileName.empty()) {
				return;
			}

			std::ofstream f(params.fileName);
			f << "# plane " << plane << ", position " << params.position << '\n';
			for (Position a = mina.get(); a <= maxa.get(); ++a) {
				for (Position b = minb.get(); b <= maxb.get(); ++b) {
					if (indices[a][b].is_initialized()) {
						f << a << 't' << b << 't' << network.flux(indices[a][b].get()) << '\n';
					}
				}
				f << '\n';
			}
		}

		static std::string getPlaneProperty(Params::Plane plane) {
			switch (plane) {
				case Params::X:
					return "x";
				case Params::Y:
					return "y";
				case Params::Z:
					return "z";
			}
			return "";
		}

		static std::string getAProperty(Params::Plane plane) {
			switch (plane) {
				case Params::X:
					return "y";
				case Params::Y:
					return "z";
				case Params::Z:
					return "x";
			}
			return "";
		}

		static std::string getBProperty(Params::Plane plane) {
			switch (plane) {
				case Params::X:
					return "z";
				case Params::Y:
					return "x";
				case Params::Z:
					return "y";
			}
			return "";
		}

		template <typename Comparator>
		static void check(boost::optional<Position>& accum, Position value, Comparator& comparator) {
			if (accum.is_initialized()) {
				if (comparator(value, accum.get())) {
					accum = value;
				}
			} else {
				accum = value;
			}
		}
 
	};

}

#endif /* TRACER_FLUX_SECTION_HPP_ */
