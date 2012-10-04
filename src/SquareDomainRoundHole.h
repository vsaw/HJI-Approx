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
 * SquareDomainRoundHole.h
 *
 *  Created on: 2012-08-02
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#ifndef SQUAREDOMAINROUNDHOLE_H_
#define SQUAREDOMAINROUNDHOLE_H_

#include "SquareDomain.h"

namespace bsc {

/**
 * Represents a standard square domain with a round hole for pursuit evasion
 * games in the euclidian plane.
 *
 * Formally it would be a [a; b]^4 \ B_r(hole_x, hole_y)^4 subset of the R^4.
 *
 * Please note, that the square domains with holes does not necessarily have a
 * particular symmetry. The hole must not be placed in the center. Also
 * the domain may not be connected if the hole is placed near the border of
 * the domain.
 */
class SquareDomainRoundHole: public bsc::SquareDomain {
private:
	/**
	 * The x position of the center point of the hole.
	 */
	decimal hole_x;

	/**
	 * The y position of the center point of the hole.
	 */
	decimal hole_y;

	/**
	 * The radius of the hole.
	 */
	decimal hole_radius;

public:
	/**
	 * Creates a new SquareDomain with a round hole.
	 *
	 * @param min_pos The lower bound for the complete domain.
	 * @param max_pos The upper bound for the complete domain.
	 * @param hole_x_pos The x position of the center of the hole.
	 * @param hole_y_pos The y position of the center of the hole.
	 * @param radius The radius of the hole.
	 * @param segments The amount of segments for each dimension.
	 *
	 * Please note, that the square domains with holes does not necessarily have
	 * a particular symmetry. The hole must not be placed in the center. Also
	 * the domain may not be connected if the hole is placed near the border of
	 * the domain.
	 */
	SquareDomainRoundHole(decimal min_pos, decimal max_pos, decimal hole_x_pos,
			decimal hole_y_pos, decimal radius, int segments);

	/**
	 * Destroys this instance of the SquareDomainRoundHole.
	 */
	virtual ~SquareDomainRoundHole();

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
	 * Persist the Domain to GnuPlot.
	 *
	 * This will write the GnuPlot commands needed to draw our domain into the
	 * out stream. This will just adjust the x- and y-axis according to the size
	 * of the domain.
	 *
	 * @param outStream The stream to write the GnuPlot-Commands to.
	 */
	virtual void toGnuPlot(ofstream *outStream);

	/**
	 * Returns a string representation of the Domain.
	 *
	 * The string returned will be:
	 * 		SquareDomain [min, max]^STATE_SPACE_DIMENSION \ B_radius(hole_center_x, hole_center_y)^STATE_SPACE_DIMENSION
	 *
	 * @returns A string representation of the Domain.
	 */
	virtual string toString();
};

} /* namespace bsc */
#endif /* SQUAREDOMAINROUNDHOLE_H_ */
