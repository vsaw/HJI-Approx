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
 * GameStateDomain.h
 *
 *  Created on: 2011-07-08
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#ifndef GAMESTATEDOMAIN_H_
#define GAMESTATEDOMAIN_H_

#include "Definitions.h"
#include "LinearCombination.h"

#include <fstream>
#include <string.h>

using namespace std;

namespace bsc {

/**
 * This class represents an abstract domain where a game can take place.
 *
 * It does not depend on any dimension or geometric shape of the domain. All of
 * this has to be implemented in its derived classes. See the SquareDomain class
 * as an example of a domain implementation.
 *
 * @see SquareDomain
 * @see SquareDomainSquareHole
 */
class GameStateDomain {

public:

	/**
	 * Clones the given state.
	 *
	 * @param x The state to clone
	 *
	 * @returns A deep clone of the given state.
	 */
#if OPTIMIZE_INLINE_GAME_STATE_DOMAIN_CLONE_STATE
	static State *cloneState(State *x) {
		State *ret = new State[STATE_SPACE_DIMENSION];
		memcpy(ret, x, STATE_SPACE_DIMENSION * sizeof(State));
		return ret;
	}
#else
	static State *cloneState(State *x);
#endif

	/**
	 * Clones the given index.
	 *
	 * @param x The index to clone.
	 *
	 * @returns A deep clone of the given index.
	 */
	static GridPointIndex *cloneIndex(GridPointIndex *x);

	/**
	 * Determines equality fo the given states
	 *
	 * @param x1 The first state.
	 * @param x2 The second state.
	 *
	 * @returns True if the states are exactly equal, false otherwise.
	 */
	static bool equals(State *x1, State *x2);

	/**
	 * Determines equality for given points with a tolerance eps.
	 *
	 * Points are being regarded different in case their distance in the
	 * infinity norm is greater than eps. In other words, if there exists an
	 * index i such that
	 *
	 * 		| x1[i] - x2[i] | > eps.
	 *
	 * 	@param x1 The first state.
	 * 	@param x2 The second state.
	 * 	@param eps The maximal distance between s1 and s2.
	 *
	 * 	@returns True if the distance between x1 and x2 is less then or equal to
	 * 		eps	measured in the infinity norm; false otherwise false.
	 */
	static bool equals(State *x1, State *x2, decimal eps);

	/**
	 * Print a string representation of a state into the output stream.
	 *
	 * @param x The state to print.
	 * @param stream The stream to print to.
	 */
	static void printState(State *x, std::ofstream *stream);

	/**
	 * Returns a convex combination of a feasible point.
	 *
	 * This will return a - not necessarily minimal - set of grid points so that
	 * x is in their convex hull. Also the factors of the combination are being
	 * determined. All this is made easily accessible by use of the
	 * LinearCombination class.
	 *
	 * @param x The point to build the decomposition.
	 *
	 * @returns A decomposition for point x.
	 *
	 * @see LinearCombination
	 */
	virtual LinearCombination *convexDecomposition(State *x) = 0;

	/**
	 * Returns the Cell Index of the nearest corner for state x.
	 *
	 * The point x must be feasible. Then this will return the nearest valid
	 * grid point.
	 *
	 * @param x The state to find his nearest grid point.
	 *
	 * @returns The index of the nearest grid point.
	 */
	virtual GridPointIndex *getGridIndex(State *x) = 0;

	/**
	 * Returns the maximal valid GridPointIndex.
	 *
	 * This means that if this function returns an index [a,b,c,d] the valid
	 * grid cell indices range from [0,0,0,0] to [a,b,c,d].
	 *
	 * @returns The maximal valid index for this domain.
	 */
	virtual GridPointIndex *getMaximalGridIndex() = 0;

	/**
	 * Returns the maximal distance between two node points alongside one
	 * dimension of the domain.
	 *
	 * On a regular square domain this will be computed as follows:
	 * 		(max - min) / #segments in each dimension
	 *
	 * @returns The maximal distance between two nodes on one dimension of the
	 * 		domain.
	 */
	virtual decimal getMaximalResolutionWidth() = 0;

	/**
	 * Returns the point for the given index.
	 *
	 * @param index The index of the point.
	 *
	 * @returns The point with the given index.
	 */
	virtual State *getPoint(GridPointIndex *index) = 0;

	/**
	 * Determines if a State is feasible for this domain.
	 *
	 * Terminal states are regarded as feasible as long as they are contained
	 * in the domain.
	 *
	 * @param x The state to determine its feasibility.
	 *
	 * @returns True if it is feasible; false otherwise.
	 */
	virtual bool isFeasible(State *x) = 0;

	/**
	 * Determines if a state is feasible for the given player.
	 *
	 * @param x The state to check.
	 * @param p The player to check his position.
	 *
	 * @returns True if the player is contained in the domain and feasible and
	 * 		false otherwise.
	 */
	virtual bool isFeasible(State *x, Player p) = 0;

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
	virtual bool isFeasible(GridPointIndex *x) = 0;

	/**
	 * Determines if a GridPointIndex is feasible for the given player.
	 *
	 * @param x The GridPointIndex to check.
	 * @param p The player to check his position.
	 *
	 * @returns True if the player is contained in the domain and feasible and
	 * 		false otherwise.
	 */
	virtual bool isFeasible(GridPointIndex *x, Player p) = 0;

	/**
	 * Determines if the given state is located on the discrete grid of the
	 * domain.
	 *
	 * @param x The state.
	 *
	 * @returns True if it is part of the discrete grid; otherwise false.
	 */
	virtual bool isGridPoint(State *x) = 0;

	/**
	 * Persist the GameStateDomain to GnuPlot.
	 *
	 * This will write the GnuPlot commands needed to draw our domain into the
	 * out stream. In the most cases this will just adjust the x- and y-axis
	 * according to the size of the domain in more complicated cases it will
	 * draw the domain according to the geometry of the domain.
	 *
	 * @param outStream The stream to write the GnuPlot-Commands to.
	 */
	virtual void toGnuPlot(std::ofstream *outStream) = 0;

	/**
	 * Returns a string representation of the GameStateDomain.
	 *
	 * It should contain the Class name and the most important parameters to
	 * describe the domain.
	 *
	 * @returns A string representation of the GameStateDomain.
	 */
	virtual string toString() = 0;
};

}

#endif /* GAMESTATEDOMAIN_H_ */
