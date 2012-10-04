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
 * SquareDomain.cpp
 *
 *  Created on: 2011-07-08
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#include "SquareDomain.h"
#include <math.h>
#include "Debug.h"

namespace bsc {

SquareDomain::SquareDomain(decimal min_pos, decimal max_pos,
		unsigned int num_nodes) {
	ASSERT(min_pos < max_pos, "%s:%d\n", __FILE__, __LINE__);
	ASSERT(num_nodes > 1, "%s:%d\n", __FILE__, __LINE__);

	min = min_pos;
	max = max_pos;
	segments = num_nodes - 1;
	width = (max - min) / segments;

	// Initialize the maximal grid index.
	for (int i = 0; i < STATE_SPACE_DIMENSION; i++)
		maximalGridIndex[i] = this->segments;
}

SquareDomain::~SquareDomain() {
}

decimal SquareDomain::ceilToCell(decimal x) {
	return min + width * ceilToCellIndex(x);
}

int SquareDomain::ceilToCellIndex(decimal x) {
	State cell = (x - min) / width;
	return ceil(cell);
}

#if OPTIMIZE_CONVEX_DECOMPOSITION_NO_RECURSION
LinearCombination *SquareDomain::convexDecomposition(State *x) {
	ASSERT(isFeasible(x), "%s:%d\n", __FILE__, __LINE__);

	// Initialize the linear combination with clones of x and empty indices
	LinearCombination *ret = new LinearCombination();
	for (int i = 0; i < LINEAR_COMBINATON_LENGTH; i++) {
		ret->setPoint(i, GameStateDomain::cloneState(x));
		ret->setIndex(i, new int[STATE_SPACE_DIMENSION]);
	}

	/* d is the direction (or since we are projecting alongside the axis of our
	 * coordinate system) the dimension we are currently working on. */
	for (int d = 0; d < STATE_SPACE_DIMENSION; d++) {

		/* i is the index of the point in the linear combination that we are
		 * currenly adjusting. */
		int i = 0;
		/* Now, since we are "forking" from a certain point in the tree this
		 * variable contains the number of child nodes that this point can have
		 * up in this step of the convex decomposition. */
		int numChilds = pow(2, STATE_SPACE_DIMENSION - 1 - d);

		/* For each dimension we loop through every point in the convex
		 * decomposition, to make sure we get all the corners of every point. */
		while (i < LINEAR_COMBINATON_LENGTH) {

			/* First check if the current index is null. that might happen if
			 * there is a dimension d such that the given point is already on
			 * the border of d. (e.g. on the side of the cube).
			 *
			 * In that case we set the "ceil part" of the decomposition to null
			 * since we don't need that points for the decomposition. */
			if (ret->getPoint(i) != NULL) {
				// calculate floor and ceil values.
				decimal f = floorToCell(ret->getPoint(i)[d]);
				decimal c = ceilToCell(ret->getPoint(i)[d]);
				int fIndex = floorToCellIndex(ret->getPoint(i)[d]);
				int cIndex = ceilToCellIndex(ret->getPoint(i)[d]);
				decimal lambda = 1 - (ret->getPoint(i)[d] - f) / width;

				for (int j = 0; j < numChilds; j++) {
					/* set the factor, point and grid index. */
					ret->setFactor(i, ret->getFactor(i) * lambda);
					ret->getPoint(i)[d] = f;
					ret->getIndex(i)[d] = fIndex;

					i++;
				}

				for (int j = 0; j < numChilds; j++) {
					if (f != c) {
						/* set the factor, point and grid index. */
						ret->setFactor(i, ret->getFactor(i) * (1 - lambda));
						ret->getPoint(i)[d] = c;
						ret->getIndex(i)[d] = cIndex;
					} else {
						if (ret->getPoint(i) != NULL) {

							/* delete all the points and indices since they would be
							 * either duplicates of floor or if we in fact move to
							 * the next ceiling point, the factor of the point would
							 * be 0 therefore it would not matter anyways. */
							delete[] ret->getIndex(i);
							delete[] ret->getPoint(i);

							ret->setIndex(i, NULL);
							ret->setPoint(i, NULL);
						}
					}

					i++;
				}
			} else {
				/* if the floor value is NULL the ceil value will also be NULL
				 * therefore we can skip the ceil thingy as well. */
				i += 2 * numChilds;
			}
		}
	}

	return ret;
}
#else
LinearCombination *SquareDomain::convexDecomposition(State *x) {
	ASSERT(isFeasible(x), "%s:%d\n", __FILE__, __LINE__);

	LinearCombination *ret = new LinearCombination();

	// The state passed to this function will be altered therefore we provide
	// a clone to preserve the original state passed to us.
	convexDecomposition(GameStateDomain::cloneState(x),
			getGridIndex(x), 0, 0, ret);
	return ret;
}

void SquareDomain::convexDecomposition(State *s, GridPointIndex *index, int i,
		int start, LinearCombination *ret) {
	/* While the implemtation by Cristiani & Falcone suggest a binary tree
	 * structure I decided to implement this tree using an array implementation.
	 * (Remember your GAD lecture by Prof. Seidl).
	 * This code uses "i" as current the depth of the tree.
	 *
	 * This enabled me to implement the decomposition in an effictient tail
	 * recursive fashion. And it also makes the evaluation of the linear
	 * combination very very fast: O( 2^STATE_SPATE_DIMENSION) )
	 * (Evaluation of a full-tree-structure would take twice as long)
	 */

	if (i >= STATE_SPACE_DIMENSION)
	return;

	decimal f, c;

	f = floorToCell(s[i]);
	int fIndex = floorToCellIndex(s[i]);
	c = ceilToCell(s[i]);
	int cIndex = ceilToCellIndex(s[i]);
	if (f == c) {
		// Does C++ recognize this as tail recursive?
		// Or do I have to put the return in the if-clause?
		if (i < STATE_SPACE_DIMENSION - 1) {
			// If we have still directions left just step down in the recursion.
			convexDecomposition(s, index, i + 1, start, ret);
		} else {
			// In this case, the last dimension is located on a grid-point.
			// Therefore just apply the point and do not alter the factor.
			// Since the other factor would be 0 anyway.
			ret->setPoint(start, s);
			ret->setIndex(start, index);
		}

		return;
	}

	decimal lambda = 1 - (s[i] - f) / width;
	int someIndex = start + pow(2, (STATE_SPACE_DIMENSION - 1 - i));

	// Adjust the factors according to our lambda in the given direction.
	// But only alter the leafs of our children.
	for (int j = start; j <= someIndex - 1; j++) {
		ret->setFactor(j, ret->getFactor(j) * lambda);
	}

	for (int j = someIndex;
			j <= start + pow(2, (STATE_SPACE_DIMENSION - i)) - 1; j++) {
		ret->setFactor(j, ret->getFactor(j) * (1 - lambda));
	}

	// Apply floor and ceil values for the given direction.
	s[i] = f;
	index[i] = fIndex;
	State *clone = GameStateDomain::cloneState(s);
	GridPointIndex *indexClone = GameStateDomain::cloneIndex(index);
	clone[i] = c;
	indexClone[i] = cIndex;

	if (i < STATE_SPACE_DIMENSION - 1) {
		convexDecomposition(s, index, i + 1, start, ret);
		convexDecomposition(clone, indexClone, i + 1, someIndex, ret);
	} else {
		ret->setPoint(start, s);
		ret->setPoint(start + 1, clone);
		ret->setIndex(start, index);
		ret->setIndex(start + 1, indexClone);
	}
}
#endif

decimal SquareDomain::floorToCell(decimal x) {
	return min + width * floorToCellIndex(x);
}

int SquareDomain::floorToCellIndex(decimal x) {
	State cell = (x - min) / width;
	return floor(cell);
}

GridPointIndex *SquareDomain::getGridIndex(State *x) {
	GridPointIndex *index = new int[STATE_SPACE_DIMENSION];

	for (int i = 0; i < STATE_SPACE_DIMENSION; i++) {
		index[i] = round((x[i] - min) / width);
	}

	return index;
}

GridPointIndex *SquareDomain::getMaximalGridIndex() {
	return maximalGridIndex;
}

decimal SquareDomain::getMaximalResolutionWidth() {
	return width;
}

State *SquareDomain::getPoint(GridPointIndex *index) {
	State *s = new State[STATE_SPACE_DIMENSION];

	for (int i = 0; i < STATE_SPACE_DIMENSION; i++)
		s[i] = min + (index[i] * width);

	return s;
}

bool SquareDomain::isFeasible(State *state) {
	ASSERT(state != NULL, "%s:%d\n", __FILE__, __LINE__);

	if (state == NULL)
		return false;

	for (int i = 0; i < STATE_SPACE_DIMENSION; i++) {
		if (state[i] < min || state[i] > max)
			return false;
	}
	return true;
}

bool SquareDomain::isFeasible(State *x, Player p) {
#if STATE_SPACE_DIMENSION != 4
// XXX: Only for the 4-dim Domain!
#error "Only for the 4-dim Domain!"
#endif

	ASSERT(x != NULL, "%s:%d\n", __FILE__, __LINE__);

	if (p == Pursuer)
		return (x[0] >= min && x[0] <= max) && (x[1] >= min && x[1] <= max);

	return (x[2] >= min && x[2] <= max) && (x[3] >= min && x[3] <= max);
}

bool SquareDomain::isFeasible(GridPointIndex *x) {
	ASSERT(x != NULL, "%s:%d\n", __FILE__, __LINE__);

	if (x == NULL)
		return false;

	for (int i = 0; i < STATE_SPACE_DIMENSION; i++) {
		if (x[i] < 0 || x[i] > maximalGridIndex[i])
			return false;
	}
	return true;
}

bool SquareDomain::isFeasible(GridPointIndex *x, Player p) {
#if STATE_SPACE_DIMENSION != 4
// XXX: Only for the 4-dim Domain!
#error "Only for the 4-dim Domain!"
#endif

	ASSERT(x == NULL, "%s:%d\n", __FILE__, __LINE__);

	if (p == Pursuer)
		return (x[0] >= 0 && x[0] <= maximalGridIndex[0])
				&& (x[1] >= 0 && x[1] <= maximalGridIndex[1]);

	return (x[2] >= 0 && x[2] <= maximalGridIndex[2])
			&& (x[3] >= 0 && x[3] <= maximalGridIndex[3]);
}

bool SquareDomain::isGridPoint(State *x) {
	ASSERT(isFeasible(x), "%s:%d\n", __FILE__, __LINE__);

	for (int i = 0; i < STATE_SPACE_DIMENSION; i++) {
		// For performance reasons we do the feasibility check on the fly.
		if (x[i] < min || x[i] > max)
			return false;

		// Compute the unrounded GridPointIndex.
		State index = (x[i] - min) / width;
		// Then check the distance from the unrounded index to the rounded.
		// If this differ by more than 1% width we're not an GridPoint.
		State dist = fabs(index - round(index));
		if (dist > 0.01)
			return false;
	}
	return true;
}

decimal SquareDomain::roundToCell(decimal x) {
	return floorToCell(x + width / 2);
}

void SquareDomain::toGnuPlot(std::ofstream *outStream) {
	*outStream << "set xrange [" << min << ":" << max << "]" << std::endl;
	*outStream << "set yrange [" << min << ":" << max << "]" << std::endl;
	*outStream << "set view equal xy" << std::endl;
	*outStream << "set size square" << std::endl;

	// Build the x and y axis tics.
	*outStream << "set xtics " << min << "," << width << "," << max
			<< " format '%.2f'" << std::endl;
	*outStream << "set ytics " << min << "," << width << "," << max
			<< " format '%.2f'" << std::endl;
}

string SquareDomain::toString() {
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "SquareDomain [%f, %f]^%d", min, max, STATE_SPACE_DIMENSION);
	return buffer;
}

}
