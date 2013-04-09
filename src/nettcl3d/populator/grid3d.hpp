/*
 * populator/grid3d.hpp --
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

#ifndef GRID3D_HPP_
#define GRID3D_HPP_

#include <math.h>
#include <boost/multi_array.hpp>
#include "../calc/abstract_populator.hpp"
#include "../calc/abstract_rng.hpp"
#include "../util/statistics.hpp"

namespace populator {

	class Grid3d : public AbstractPopulator {
	public:

		struct Params {
			unsigned xSize, ySize, zSize;
			AbstractRng& betaRng;
			AbstractRng& tauRng;
			AbstractRng& vRng;

			Params(
					unsigned xSize,
					unsigned ySize,
					unsigned zSize,
					AbstractRng& betaRng,
					AbstractRng& tauRng,
					AbstractRng& vRng) :
				xSize(xSize), ySize(ySize), zSize(zSize),
				betaRng(betaRng), tauRng(tauRng), vRng(vRng) {}
		};

		Grid3d(const Params& params) : params(params){
		}

	private:

		typedef std::size_t index_type;
		typedef boost::multi_array<index_type, 3> IndexTensor;

		static const double square;
		const Params params;

		Grid3d(const Grid3d& src) : params(src.params){
		}

		virtual void doPopulate(Network& network);

		virtual phlib::Cloneable* doClone() const {
			return new Grid3d(*this);
		}

		index_type addXContact(Network& network, unsigned x, unsigned y, unsigned z);
		index_type addYContact(Network& network, unsigned x, unsigned y, unsigned z);
		index_type addZContact(Network& network, unsigned x, unsigned y, unsigned z);
		index_type addContact(Network& network);
		void setContactTags(Contact& c, unsigned x, unsigned y, unsigned z, bool isX, bool isY, bool isZ);

		static void addContactRef(Circuit& c, index_type index, const double gain, const double weight);
		void addXCircuit(Network& network, unsigned x, unsigned y, unsigned z, const IndexTensor& yIndices, const IndexTensor& zIndices);
		void addYCircuit(Network& network, unsigned x, unsigned y, unsigned z, const IndexTensor& zIndices, const IndexTensor& xIndices);
		void addZCircuit(Network& network, unsigned x, unsigned y, unsigned z, const IndexTensor& xIndices, const IndexTensor& yIndices);

	};
}

#endif /* GRID3D_HPP_ */
