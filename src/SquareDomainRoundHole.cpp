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
 * SquareDomainRoundHole.cpp
 *
 *  Created on: 2012-08-02
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#include "SquareDomainRoundHole.h"
#include "Debug.h"

namespace bsc {

SquareDomainRoundHole::SquareDomainRoundHole(State min_pos, State max_pos,
		State hole_x_pos, State hole_y_pos, State radius, int segments) :
		SquareDomain(min_pos, max_pos, segments) {
#if STATE_SPACE_DIMENSION != 4
// XXX: Only for the 4-dim Domain!
#error "Only for the 4-dim Domain!"
#endif

	ASSERT((min_pos <= hole_x_pos) && (hole_x_pos <= max_pos), "%s:%d\n",
			__FILE__, __LINE__);
	ASSERT((min_pos <= hole_y_pos) && (hole_y_pos <= max_pos), "%s:%d\n",
			__FILE__, __LINE__);
	ASSERT((0 < radius) && (radius < (max_pos - min_pos)), "%s:%d\n",
			__FILE__, __LINE__);

	hole_x = hole_x_pos;
	hole_y = hole_y_pos;
	hole_radius = radius;
}

SquareDomainRoundHole::~SquareDomainRoundHole() {
}

bool SquareDomainRoundHole::isFeasible(State *x) {
	if (isFeasible(x, Pursuer))
		return isFeasible(x, Evader);

	return false;
}

bool SquareDomainRoundHole::isFeasible(State *x, Player p) {
#if STATE_SPACE_DIMENSION != 4
// XXX: Only for the 4-dim Domain!
#error "Only for the 4-dim Domain!"
#endif

	if (x == NULL)
		return false;

	int start = (p == Pursuer) ? 0 : (STATE_SPACE_DIMENSION / 2);
	State residuum[STATE_SPACE_DIMENSION];

	bool outsideDomain = (x[start] < min || x[start] > max)
			|| (x[start + 1] < min || x[start + 1] > max);
	if (outsideDomain)
		return false;

	// First populate the residdum vector with the next grid point coordinates
	if (x[start] < hole_x) {
		residuum[start] = ceilToCell(x[start]) - hole_x;
	} else {
		residuum[start] = floorToCell(x[start]) - hole_x;
	}
	if (x[start + 1] < hole_y) {
		residuum[start + 1] = ceilToCell(x[start + 1]) - hole_y;
	} else {
		residuum[start + 1] = floorToCell(x[start + 1]) - hole_y;
	}

	// No calculate the length of that vector.
	State distanceToHoleCenter = sqrt(
			residuum[start] * residuum[start]
					+ residuum[start + 1] * residuum[start + 1]);

	// It is feasible if distance larger than hole radius.
	return distanceToHoleCenter > hole_radius;
}

bool SquareDomainRoundHole::isFeasible(GridPointIndex *x) {
	if (x != NULL) {
		State *s = getPoint(x);
		bool ret = isFeasible(s);
		delete[] s;
		return ret;
	}
	return false;
}

bool SquareDomainRoundHole::isFeasible(GridPointIndex *x, Player p) {
	if (x != NULL) {
		State *s = getPoint(x);
		bool ret = isFeasible(s, p);
		delete[] s;
		return ret;
	}
	return false;
}

void SquareDomainRoundHole::toGnuPlot(ofstream *outStream) {
	// Plot the regular stuff of the SquareDomain first.
	SquareDomain::toGnuPlot(outStream);

	// Now plot the hole
	*outStream << "set object circle at " << hole_x << "," << hole_y << " size "
			<< hole_radius << endl;
}

string SquareDomainRoundHole::toString() {
	char buffer[256];
	snprintf(buffer, sizeof(buffer),
			"SquareDomainRoundHole [%f, %f]^%d \\ B_%f((%f, %f)^%d", min, max,
			STATE_SPACE_DIMENSION, hole_radius, hole_x, hole_y,
			STATE_SPACE_DIMENSION);
	return buffer;
}

} /* namespace bsc */
