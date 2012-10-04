/*
 * Copyright (c) 2011-2012, Valentin Sawadski
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the <ORGANIZATION> nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * SquareDomainSquareHole.cpp
 *
 *  Created on: 2012-05-13
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#include "SquareDomainSquareHole.h"
#include "Debug.h"

namespace bsc {

SquareDomainSquareHole::SquareDomainSquareHole(decimal min_pos, decimal max_pos,
		decimal hole_min_pos, decimal hole_max_pos, int segments) :
		SquareDomain(min_pos, max_pos, segments) {

	ASSERT(min_pos < hole_min_pos, "%s:%d\n", __FILE__, __LINE__);
	ASSERT(hole_min_pos < hole_max_pos, "%s:%d\n", __FILE__, __LINE__);
	ASSERT(hole_max_pos < max_pos, "%s:%d\n", __FILE__, __LINE__);

	hole_min = roundToCell(hole_min_pos);
	hole_max = roundToCell(hole_max_pos);
}

SquareDomainSquareHole::~SquareDomainSquareHole() {
}

bool SquareDomainSquareHole::isFeasible(State *x) {
	if (isFeasible(x, Pursuer))
		return isFeasible(x, Evader);

	return false;
}

bool SquareDomainSquareHole::isFeasible(State *x, Player p) {
	if (x == NULL)
		return false;

	int start = (p == Pursuer) ? 0 : (STATE_SPACE_DIMENSION / 2);
	int finish =
			(p == Pursuer) ? (STATE_SPACE_DIMENSION / 2) : STATE_SPACE_DIMENSION;

	// Okay we assume at the beginning that we are in fact inside the hole
	// because once we find one coordinate that is outisde the hole we know we
	// are clear.
	bool onceInside = true;
	for (int i = start; i < finish; i++) {
		bool outside = x[i] < min || x[i] > max;
		bool inHole = hole_min < x[i] && x[i] < hole_max;

		// If we are outside of the domain we can return immediately
		if (outside)
			return false;

		// If every coordinate is inside the hole we are in the hole
		// if we find a coordinate that is outside this will turn to false
		// and it will remain false.
		if (onceInside)
			onceInside &= inHole;
	}

	return !onceInside;
}

bool SquareDomainSquareHole::isFeasible(GridPointIndex *x) {
	if (x != NULL) {
		State *s = getPoint(x);
		bool ret = isFeasible(s);
		delete[] s;
		return ret;
	}
	return false;
}

bool SquareDomainSquareHole::isFeasible(GridPointIndex *x, Player p) {
	if (x != NULL) {
		State *s = getPoint(x);
		bool ret = isFeasible(s, p);
		delete[] s;
		return ret;
	}
	return false;
}

void SquareDomainSquareHole::toGnuPlot(std::ofstream *outStream) {
	// Plot the regular stuff of the SquareDomain first.
	SquareDomain::toGnuPlot(outStream);

	// Now plot the hole
	*outStream << "set arrow from " << hole_min << "," << hole_min << " to "
			<< hole_min << "," << hole_max << " nohead" << std::endl;
	*outStream << "set arrow from " << hole_min << "," << hole_min << " to "
			<< hole_max << "," << hole_min << " nohead" << std::endl;
	*outStream << "set arrow from " << hole_max << "," << hole_max << " to "
			<< hole_min << "," << hole_max << " nohead" << std::endl;
	*outStream << "set arrow from " << hole_max << "," << hole_max << " to "
			<< hole_max << "," << hole_min << " nohead" << std::endl;
}

string SquareDomainSquareHole::toString() {
	char buffer[256];
	snprintf(buffer, sizeof(buffer),
			"SquareDomainSquareHole [%f, %f]^%d \\ [%f, %f]^%d", min, max,
			STATE_SPACE_DIMENSION, hole_min, hole_max, STATE_SPACE_DIMENSION);
	return buffer;
}

}
