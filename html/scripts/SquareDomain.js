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
 * SquareDomain.js
 *
 *  Created on: 2013-09-01
 *      Author: Valentin Sawadski <valentin@sawadski.eu>
 */

var Definitions = require('./Definitions.js');
var LinearCombination = require('./LinearCombination.js');
var GameStateDomain = require('./GameStateDomain.js');

/**
 * Represents a standard bounded symmetrical square domain for pursuit evasion
 * games in the euclidian plane.
 * 
 * Formally it would be a [a; b]^4 subset of the R^4.
 */
var SquareDomain = function(minPos, maxPos, numNodes) {

	/**
	 * The lower bound of the domain.
	 */
	var min = minPos;
	/**
	 * The upper bound of the domain.
	 */
	var max = maxPos;
	/**
	 * The width of one segment. Computed out of max - min / segments.
	 */
	var width = (max - min) / (numNodes - 1);

	/**
	 * Returns the cell index of the upper bound.
	 * 
	 * @param x
	 *            The value to round.
	 * 
	 * @returns The index whose point is greater than or equal to x.
	 */
	var ceilToCellIndex = function(x) {
		var cell = (x - min) / width;
		return Math.ceil(cell);
	};

	/**
	 * Round the given value up the match the upper border of the current
	 * segment.
	 * 
	 * Because of the symmetry we can use this function to round to the next
	 * cell point in either dimension.
	 * 
	 * @param x
	 *            The value to round.
	 * 
	 * @returns The next greater grid cell point.
	 */
	var ceilToCell = function(x) {
		return min + width * ceilToCellIndex(x);
	};

	/**
	 * Returns the cell index of the lower bound.
	 * 
	 * @param x
	 *            The value to round.
	 * 
	 * @returns The index whose point is less than or equal to x.
	 */
	var floorToCellIndex = function(x) {
		var cell = (x - min) / width;
		return Math.floor(cell);
	};

	/**
	 * Round the given value up the match the lower border of the current
	 * segment.
	 * 
	 * Because of the symmetry we can use this function to round to the next
	 * floor point in either dimension.
	 * 
	 * @param x
	 *            The value to round.
	 * 
	 * @returns The next smaller grid cell point.
	 */
	var floorToCell = function(x) {
		return min + width * floorToCellIndex(x);
	};

	/**
	 * Returns a convex combination of a feasible point.
	 * 
	 * The convex combination is being built using the vertices of the cube
	 * containing "x".
	 * 
	 * @param x
	 *            The point to decompose.
	 * 
	 * @returns A LinearCombination such that lc->eval() equals x. And x is
	 *          contained in the convex hull of the points of the linear
	 *          combination.
	 */
	this.convexDecomposition = function(x) {
		// Initialize the linear combination with clones of x and empty indices
		var ret = new LinearCombination();
		for ( var i = 0; i < Definitions.LINEAR_COMBINATON_LENGTH; i++) {
			ret.setPoint(i, GameStateDomain.cloneState(x));
			ret.setIndex(i, []);
		}

		/*
		 * d is the direction (or since we are projecting alongside the axis of
		 * our coordinate system) the dimension we are currently working on.
		 */
		for ( var d = 0; d < Definitions.STATE_SPACE_DIMENSION; d++) {

			/*
			 * i is the index of the point in the linear combination that we are
			 * currenly adjusting.
			 */
			var i = 0;
			/*
			 * Now, since we are "forking" from a certain point in the tree this
			 * variable contains the number of child nodes that this point can
			 * have up in this step of the convex decomposition.
			 */
			var numChilds = Math.pow(2, Definitions.STATE_SPACE_DIMENSION - 1
					- d);

			/*
			 * For each dimension we loop through every point in the convex
			 * decomposition, to make sure we get all the corners of every
			 * point.
			 */
			while (i < Definitions.LINEAR_COMBINATON_LENGTH) {

				/*
				 * First check if the current index is null. that might happen
				 * if there is a dimension d such that the given point is
				 * already on the border of d. (e.g. on the side of the cube).
				 * 
				 * In that case we set the "ceil part" of the decomposition to
				 * null since we don't need that points for the decomposition.
				 */
				if (ret.getPoint(i) != null) {
					// calculate floor and ceil values.
					var f = floorToCell(ret.getPoint(i)[d]);
					var c = ceilToCell(ret.getPoint(i)[d]);
					var fIndex = floorToCellIndex(ret.getPoint(i)[d]);
					var cIndex = ceilToCellIndex(ret.getPoint(i)[d]);
					var lambda = 1 - (ret.getPoint(i)[d] - f) / width;

					for ( var j = 0; j < numChilds; j++) {
						/* set the factor, point and grid index. */
						ret.setFactor(i, ret.getFactor(i) * lambda);
						ret.getPoint(i)[d] = f;
						ret.getIndex(i)[d] = fIndex;

						i++;
					}

					for ( var j = 0; j < numChilds; j++) {
						if (f != c) {
							/* set the factor, point and grid index. */
							ret.setFactor(i, ret.getFactor(i) * (1 - lambda));
							ret.getPoint(i)[d] = c;
							ret.getIndex(i)[d] = cIndex;
						} else {
							if (ret.getPoint(i) != null) {

								/*
								 * delete all the points and indices since they
								 * would be either duplicates of floor or if we
								 * in fact move to the next ceiling point, the
								 * factor of the point would be 0 therefore it
								 * would not matter anyways.
								 */
								ret.setIndex(i, null);
								ret.setPoint(i, null);
							}
						}

						i++;
					}
				} else {
					/*
					 * if the floor value is NULL the ceil value will also be
					 * NULL therefore we can skip the ceil thingy as well.
					 */
					i += 2 * numChilds;
				}
			}
		}

		return ret;
	};

	/**
	 * Returns the Cell Index of the nearest corner for state x.
	 * 
	 * The point x must be feasible. Then this will return the nearest valid
	 * grid point.
	 * 
	 * @param x
	 *            The state to find his nearest grid point.
	 * 
	 * @returns The index of the nearest grid point.
	 */
	this.getGridIndex = function(x) {
		var index = [];
		for ( var i = 0; i < Definitions.STATE_SPACE_DIMENSION; i++) {
			index[i] = Math.round((x[i] - min) / width);
		}
		return index;
	};
};

module.exports = SquareDomain;