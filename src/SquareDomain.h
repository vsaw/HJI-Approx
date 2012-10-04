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
 * SquareDomain.h
 *
 *  Created on: 2011-07-08
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#ifndef SQUAREDOMAIN_H_
#define SQUAREDOMAIN_H_

#include "GameStateDomain.h"

#include <fstream>
#include <string>

using namespace std;

namespace bsc {

/**
 * Represents a standard bounded symmetrical square domain for pursuit evasion
 * games in the euclidian plane.
 *
 * Formally it would be a [a; b]^4 subset of the R^4.
 */
class SquareDomain: public bsc::GameStateDomain {

protected:

	/**
	 * The upper bound of the domain.
	 */
	decimal max;

	/**
	 * The maximal grid index for this domain.
	 */
	GridPointIndex maximalGridIndex[STATE_SPACE_DIMENSION];

	/**
	 * The lower bound of the domain.
	 */
	decimal min;

	/**
	 * The amount of segments of the domain in one dimension.
	 *
	 * Both dimensions will have equally "segments" segments. Please note that if one
	 * dimension has "segments" segments, it has "segments + 1" grid points.
	 */
	int segments;

	/**
	 * The width of one segment. Computed out of max - min / segments.
	 */
	decimal width;

	/**
	 * Round the given value up the match the upper border of the current
	 * segment.
	 *
	 * Because of the symmetry we can use this function to round to the next
	 * cell point in either dimension.
	 *
	 * @param x The value to round.
	 *
	 * @returns The next greater grid cell point.
	 */
	decimal ceilToCell(decimal x);

	/**
	 * Returns the cell index of the upper bound.
	 *
	 * @param x The value to round.
	 *
	 * @returns The index whose point is greater than or equal to x.
	 */
	int ceilToCellIndex(decimal x);

#if !OPTIMIZE_CONVEX_DECOMPOSITION_NO_RECURSION
	/**
	 * Constructs a convex decomposition of the given state and stores it in
	 * the given LinearDecomposition.
	 *
	 * See Cristianes and Falcones fast method of decomposition utilizing
	 * projections to the sides of the 4-dimensional cube for details on how
	 * this works.
	 *
	 * @param x The state to build the decomposition of.
	 * @param i The dimension to build a projection of.
	 * @param start The index to use in the LinearCombination.
	 * @param ret The LinearCombination to use.
	 */
	void convexDecomposition(State *x, GridPointIndex *index, int i, int start,
			LinearCombination *ret);
#endif

	/**
	 * Round the given value up the match the lower border of the current
	 * segment.
	 *
	 * Because of the symmetry we can use this function to round to the next
	 * floor point in either dimension.
	 *
	 * @param x The value to round.
	 *
	 * @returns The next smaller grid cell point.
	 */
	decimal floorToCell(decimal x);

	/**
	 * Returns the cell index of the lower bound.
	 *
	 * @param x The value to round.
	 *
	 * @returns The index whose point is less than or equal to x.
	 */
	int floorToCellIndex(decimal x);

	/**
	 * Round the given value to the nearest border of the current segment.
	 *
	 * Because of the symmetry we can use this function to round to the nearest
	 * point in either dimension.
	 *
	 * @param x The value to round.
	 *
	 * @returns The nearest grid cell point.
	 */
	decimal roundToCell(decimal x);

public:

	/**
	 * Represents a standard bounded symmetrical square domain for pursuit
	 * evasion games in the euclidian plane.
	 *
	 * Formally it would be a [min_pos; max_pos]^4 subset of the R^4. It has an
	 * underlying grid with a given number of nodes in each dimension.
	 *
	 * Please note that: #nodes = #segments + 1
	 *
	 * @param min_pos The lower bound.
	 * @param max_pos The upper bound.
	 * @param nodes The number of nodes in each dimension.
	 */
	SquareDomain(decimal min_pos, decimal max_pos, unsigned int nodes);

	/**
	 * Deletes the domain an all of it's resources.
	 *
	 * Does not delete decompositions or other objects associated to the domain.
	 */
	virtual ~SquareDomain();

	/**
	 * Returns a convex combination of a feasible point.
	 *
	 * The convex combination is being built using the vertices of the cube
	 * containing "x".
	 *
	 * @param x The point to decompose.
	 *
	 * @returns A LinearCombination such that lc->eval() equals x. And x is
	 * 		contained in the convex hull of the points of the linear
	 * 		combination.
	 */
	LinearCombination *convexDecomposition(State *x);

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
	virtual GridPointIndex *getGridIndex(State *x);

	/**
	 * Returns the maximal valid GridPointIndex.
	 *
	 * This means that valid grid cell indices range from
	 * [0,0,0,0] to [a,b,c,d].
	 *
	 * Do not unset the value returned from this function!
	 *
	 * @returns The maximal valid grid index.
	 */
	virtual GridPointIndex *getMaximalGridIndex();

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
	decimal getMaximalResolutionWidth();

	/**
	 * Returns the point for the given index.
	 *
	 * The index parameter will be be checked for feasibility. Therefore this
	 * function will return a point even if the given index is not valid. It is
	 * the responsibility of the caller to ensure that the given index is valid.
	 *
	 * @param index The index of the point.
	 *
	 * @returns The point with the given index.
	 */
	virtual State *getPoint(GridPointIndex *index);

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
	 * Determines if the given state is located on the discrete grid of the
	 * domain.
	 *
	 * In this implementation it is considered a grid point if its position
	 * differs from the nearest GridPoint by at most 1% of the cell width.
	 *
	 * @param x The state.
	 *
	 * @returns True if it is part of the discrete grid; otherwise false.
	 */
	bool isGridPoint(State *x);

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
	 * 		SquareDomain [min, max]^STATE_SPACE_DIMENSION
	 *
	 * @returns A string representation of the GameStateDomain.
	 */
	virtual string toString();
};

}

#endif /* SQUAREDOMAIN_H_ */
