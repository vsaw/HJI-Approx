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
 * SquareDomainSquareHole.h
 *
 *  Created on: 2012-05-13
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#ifndef SQUAREDOMAINSQUAREHOLE_H_
#define SQUAREDOMAINSQUAREHOLE_H_

#include "SquareDomain.h"

namespace bsc {

/**
 * Represents a standard square domain with a square hole for pursuit evasion
 * games in the euclidian plane.
 *
 * Formally it would be a [a; b]^4 \ [c; d]^4 subset of the R^4, where
 * a < c < d < b.
 *
 * Please note, that the square domains with holes does not necessarily have a
 * particular symmetry. The hole must not be placed in the center. To have the
 * domain symmetrical with the hole placed in the center, please make sure that
 * (a == -b) && (c == -d) holds true.
 */
class SquareDomainSquareHole: public bsc::SquareDomain {

private:
	/**
	 * The lower bound of the hole.
	 */
	decimal hole_min;

	/**
	 * The upper bound for the hole.
	 */
	decimal hole_max;

public:
	/**
	 * Creates a new SquareDomain with a square hole.
	 *
	 * @param min_pos The lower bound for the complete domain.
	 * @param max_pos The upper bound for the complete domain.
	 * @param hole_min The lower bound for the hole in the domain.
	 * @param hole_max The upper bound for the hole in the domain.
	 * @param segments The amount of segments for each dimension.
	 *
	 * Please note, that the square domains with holes does not necessarily have a
	 * particular symmetry. The hole must not be placed in the center. To have the
	 * domain symmetrical with the hole placed in the center, please make sure
	 * that the following holds true:
	 *
	 * 		(min_pos == -max_pos) && (hole_min == -hole_max)
	 */
	SquareDomainSquareHole(decimal min_pos, decimal max_pos, decimal hole_min,
			decimal hole_max, int segments);

	/**
	 * Destroys this instance of the SquareDomainSquareHole.
	 */
	virtual ~SquareDomainSquareHole();

	/**
	 * Determines if a state is feasible.
	 *
	 * @param x The state to check.
	 *
	 * @returns True if the state is contained in the domain and feasible and
	 * 		false otherwise.
	 */
	virtual bool isFeasible(State *x);

	/**
	 * Determines if a state is feasible for the given player.
	 *
	 * @param x The state to check.
	 * @param p The player to check his position.
	 *
	 * @returns True if the player is contained in the domain and feasible and
	 * 		false otherwise.
	 */
	virtual bool isFeasible(State *x, Player p);

	/**
	 * Determines if a given GridPointIndex is feasible.
	 *
	 * Terminal GridPointIndexes are regarded as feasible as long as they are
	 * contained in the domain.
	 *
	 * @param x The GridPointIndex to determine feasibility.
	 *
	 * @returns True if it is feasible; false otherwise.
	 */
	virtual bool isFeasible(GridPointIndex *x);

	/**
	 * Determines if a GridPointIndex is feasible for the given player.
	 *
	 * @param x The GridPointIndex to check.
	 * @param p The player to check his position.
	 *
	 * @returns True if the player is contained in the domain and feasible and
	 * 		false otherwise.
	 */
	virtual bool isFeasible(GridPointIndex *x, Player p);

	/**
	 * Persist the SquareDomain to GnuPlot.
	 *
	 * This will write the GnuPlot commands needed to draw our domain into the
	 * out stream. This will just adjust the x- and y-axis according to the size
	 * of the domain.
	 *
	 * @param outStream The stream to write the GnuPlot-Commands to.
	 */
	virtual void toGnuPlot(std::ofstream *outStream);

	/**
	 * Returns a string representation of the SquareDomain.
	 *
	 * The string returned will be:
	 * 		SquareDomain [min, max]^STATE_SPACE_DIMENSION \ [hole_min, hole_max]^STATE_SPACE_DIMENSION
	 *
	 * @returns A string representation of the GameStateDomain.
	 */
	virtual string toString();
};

}

#endif /* SQUAREDOMAINSQUAREHOLE_H_ */
