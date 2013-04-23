/*
 * populator/grid3d.cpp --
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

#include <string>
#include <iostream>
#include "grid3d.hpp"

namespace populator {

namespace tags {
	const static std::string north = "north";
	const static std::string south = "south";
	const static std::string east = "east";
	const static std::string west = "west";
	const static std::string top = "top";
	const static std::string bottom = "bottom";

	const static std::string boundary = "boundary";
	const static std::string inner = "inner";

	const static std::string x = "x";
	const static std::string y = "y";
	const static std::string z = "z";
}

namespace props {
	const static std::string x = "x";
	const static std::string y = "y";
	const static std::string z = "z";
}

const double Grid3d::square = 1.0;

void Grid3d::doPopulate(Network& network) {
	IndexTensor xIndices(boost::extents[params.xSize][params.ySize][params.zSize]);
	IndexTensor yIndices(boost::extents[params.xSize][params.ySize][params.zSize]);
	IndexTensor zIndices(boost::extents[params.xSize][params.ySize][params.zSize]);

	for (unsigned x = 0; x < params.xSize; ++x) {
		for (unsigned y = 0; y < params.ySize; ++y) {
			for (unsigned z = 0; z < params.zSize; ++z) {

				if (x > 0) {
					xIndices[x][y][z] = addXContact(network, x, y, z);
				}

				if (y > 0) {
					yIndices[x][y][z] = addYContact(network, x, y, z);
				}

				if (z > 0) {
					zIndices[x][y][z] = addZContact(network, x, y, z);
				}

				if (x > 0 && y > 0) {
					addZCircuit(network, x, y, z, xIndices, yIndices);
				}

				if (x > 0 && z > 0) {
					addYCircuit(network, x, y, z, zIndices, xIndices);
				}

				if (y > 0 && z > 0) {
					addXCircuit(network, x, y, z, yIndices, zIndices);
				}

			}
		}
	}
}

Grid3d::index_type Grid3d::addXContact(Network& network, unsigned x, unsigned y, unsigned z) {
	const index_type index = addContact(network);
	Contact& c = network.contact(index);

	c.addTag(tags::x);
	c.setProp(props::x, x - 1);
	c.setProp(props::y, y);
	c.setProp(props::z, z);

	setContactTags(c, x, y, z, true, false, false);

	c.hNormal = Point(
		0.0,
		z == 0 ? 1.0 : z == params.zSize - 1 ? -1.0 : 0.0,
		y == 0 ? -1.0 : y == params.ySize - 1 ? 1.0 : 0.0
	);

	return index;
}

Grid3d::index_type Grid3d::addYContact(Network& network, unsigned x, unsigned y, unsigned z) {
	const index_type index = addContact(network);
	Contact& c = network.contact(index);

	c.addTag(tags::y);
	c.setProp(props::x, x);
	c.setProp(props::y, y - 1);
	c.setProp(props::z, z);

	setContactTags(c, x, y, z, false, true, false);

	c.hNormal = Point(
		z == 0 ? -1.0 : z == params.zSize - 1 ? 1.0 : 0.0,
		0.0,
		x == 0 ? 1.0 : x == params.xSize - 1 ? -1.0 : 0.0
	);

	return index;
}

Grid3d::index_type Grid3d::addZContact(Network& network, unsigned x, unsigned y, unsigned z) {
	const index_type index = addContact(network);
	Contact& c = network.contact(index);

	c.addTag(tags::z);
	c.setProp(props::x, x);
	c.setProp(props::y, y);
	c.setProp(props::z, z - 1);

	setContactTags(c, x, y, z, false, false, true);

	c.hNormal = Point(
		y == 0 ? 1.0 : y == params.ySize - 1 ? -1.0 : 0.0,
		x == 0 ? -1.0 : x == params.xSize - 1 ? 1.0 : 0.0,
		0.0
	);

	return index;
}

Grid3d::index_type Grid3d::addContact(Network& network) {
	return network.addContact(Contact(params.betaRng(), params.tauRng(), params.vRng()));
}

void Grid3d::setContactTags(Contact& c, unsigned x, unsigned y, unsigned z, bool isX, bool isY, bool isZ) {

	if (isY || isZ) {
		if (x == 0) {
			c.addTag(tags::west);
			c.addTag(tags::boundary);
		}
		else if (x == params.xSize - 1) {
			c.addTag(tags::east);
			c.addTag(tags::boundary);
		}
	}

	if (isX || isZ) {
		if (y == 0) {
			c.addTag(tags::south);
			c.addTag(tags::boundary);
		} else if (y == params.ySize - 1) {
			c.addTag(tags::north);
			c.addTag(tags::boundary);
		}
	}

	if (isX || isY) {
		if (z == 0) {
			c.addTag(tags::bottom);
			c.addTag(tags::boundary);
		} else if (z == params.zSize - 1) {
			c.addTag(tags::top);
			c.addTag(tags::boundary);
		}
	}

	if (!c.hasTag(tags::boundary)) {
		c.addTag(tags::inner);
	}
}

void Grid3d::addContactRef(Circuit& c, index_type index, const double gain, const double weight) {
	c.addContactRef(ContactRef(index, gain, weight));
}

void Grid3d::addXCircuit(Network& network, unsigned x, unsigned y, unsigned z, const IndexTensor& yIndices, const IndexTensor& zIndices) {
	Circuit c(square);

	addContactRef(c, yIndices[x][y - 1][z - 1], 1.0, 1.0);
	addContactRef(c, zIndices[x][y][z - 1], 1.0, 1.0);
	addContactRef(c, yIndices[x][y - 1][z], 1.0, -1.0);
	addContactRef(c, zIndices[x][y - 1][z - 1], 1.0, -1.0);

	c.addTag(tags::x);
	c.setProp(props::x, x);
	c.setProp(props::y, y - 1);
	c.setProp(props::z, z - 1);

	if (x == 0) {
		c.addTag(tags::west);
		c.addTag(tags::boundary);
	} else if (x == params.xSize - 1) {
		c.addTag(tags::east);
		c.addTag(tags::boundary);
	}

	if (!c.hasTag(tags::boundary)) {
		c.addTag(tags::inner);
	}

	network.addCircuit(c);

}

void Grid3d::addYCircuit(Network& network, unsigned x, unsigned y, unsigned z, const IndexTensor& zIndices, const IndexTensor& xIndices) {
	Circuit c(square);

	addContactRef(c, zIndices[x - 1][y][z - 1], 1.0, 1.0);
	addContactRef(c, xIndices[x - 1][y][z], 1.0, 1.0);
	addContactRef(c, zIndices[x][y][z - 1], 1.0, -1.0);
	addContactRef(c, xIndices[x - 1][y][z - 1], 1.0, -1.0);

	c.addTag(tags::y);
	c.setProp(props::x, x - 1);
	c.setProp(props::y, y);
	c.setProp(props::z, z - 1);

	if (y == 0) {
		c.addTag(tags::south);
		c.addTag(tags::boundary);
	} else if (y == params.ySize - 1) {
		c.addTag(tags::north);
		c.addTag(tags::boundary);
	}

	if (!c.hasTag(tags::boundary)) {
		c.addTag(tags::inner);
	}

	network.addCircuit(c);
}

void Grid3d::addZCircuit(Network& network, unsigned x, unsigned y, unsigned z, const IndexTensor& xIndices, const IndexTensor& yIndices) {
	Circuit c(square);

	addContactRef(c, xIndices[x - 1][y - 1][z], 1.0, 1.0);
	addContactRef(c, yIndices[x][y - 1][z], 1.0, 1.0);
	addContactRef(c, xIndices[x - 1][y][z], 1.0, -1.0);
	addContactRef(c, yIndices[x - 1][y - 1][z], 1.0, -1.0);

	c.addTag(tags::z);
	c.setProp(props::x, x - 1);
	c.setProp(props::y, y - 1);
	c.setProp(props::z, z);

	if (z == 0) {
		c.addTag(tags::bottom);
		c.addTag(tags::boundary);
	} else if (z == params.zSize - 1) {
		c.addTag(tags::top);
		c.addTag(tags::boundary);
	}

	if (!c.hasTag(tags::boundary)) {
		c.addTag(tags::inner);
	}

	network.addCircuit(c);
}

}	//	namespace populator
