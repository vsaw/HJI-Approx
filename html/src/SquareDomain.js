/* Copyright (c) 2013, Valentin Sawadski, http://vsaw.eu/.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * Represents a standard bounded symmetrical square domain for pursuit evasion
 * games in the euclidian plane.
 * 
 * @param minArg
 *            The lower bound.
 * @param maxArg
 *            The upper bound.
 * @param nodesArg
 *            The number of nodes in each dimension.
 * 
 * Formally it would be a [min_pos; max_pos]^n subset of the R^n. It has an
 * underlying grid with a given number of nodes in each dimension.
 * 
 * The implementation is designed to work with different dimensions. In fact
 * most of the functions take the dimension from the point or index parameter
 * that is passed to them. Only the getMaximalGridIndex function depends on an
 * explicit dimension parameter passed to it.
 * 
 * Please note that: #nodes = #segments + 1
 */
function SquareDomain(minArg, maxArg, nodesArg) {

	/**
	 * The lower bound of the domain.
	 */
	this.min;
	/**
	 * The upper bound of the domain.
	 */
	this.max;
	/**
	 * The amount of segments of the domain in one dimension.
	 * 
	 * Both dimensions will have equally "segments" segments. Please note that
	 * if one dimension has "segments" segments, it has "segments + 1" grid
	 * points.
	 */
	this.segments;

	if (minArg > maxArg) {
		throw "minArg must be larger than maxArg";
	}

	if (nodesArg <= 0) {
		throw "nodesArg must be at least 1";
	}

	this.min = minArg;
	this.max = maxArg;
	this.segments = nodesArg - 1;

	/**
	 * Determines deep equality for a given SquareDomain.
	 *
	 * @param vfunc
	 *            The object to determine equality with.
	 *
	 * @return true if they are equal, false otherwise.
	 */
	this.equals = function(obj) {
		if (this === obj) {
			return true;
		}

		if (obj instanceof SquareDomain) {
			var sameMin = this.min == obj.min;
			var sameMax = this.max == obj.max;
			var sameSegments = this.segments == obj.segments;
			return sameMax && sameMin && sameSegments;
		}

		return false;
	};

	/**
	 * Returns the Cell Index of the nearest corner for state x.
	 * 
	 * @param x
	 *            The state to find his nearest grid point.
	 * 
	 * @returns The index of the nearest grid point.
	 * 
	 * The point x must be feasible. Then this will return the nearest valid
	 * grid point. However feasibility is assumed and not checked.
	 */
	this.getGridIndex = function(x) {
		var width = (this.max - this.min) / this.segments;
		var ret = new Array(x.length);

		for ( var i = 0; i < x.length; i++) {
			ret[i] = Math.round((x[i] - this.min) / width);
		}

		return ret;
	};

	/**
	 * Returns the maximal valid GridPointIndex.
	 * 
	 * @param d
	 *            The number of dimensions as an integer.
	 * 
	 * @returns The maximal valid grid index.
	 * 
	 * This means that valid grid cell indices range from [0,0,0,0] to
	 * [a,b,c,d].
	 */
	this.getMaximalGridIndex = function(d) {
		var ret = new Array(d);

		for ( var i = 0; i < d; i++) {
			ret[i] = this.segments;
		}

		return ret;
	};

	/**
	 * Returns the point for the given index.
	 * 
	 * @param index
	 *            The index of the point.
	 * 
	 * @returns The point with the given index.
	 * 
	 * The index parameter will be be checked for feasibility. Therefore this
	 * function will return a point even if the given index is not valid. It is
	 * the responsibility of the caller to ensure that the given index is valid.
	 */
	this.getPoint = function(index) {
		var width = (this.max - this.min) / this.segments;
		var s = new Array(index.length);

		for ( var i = 0; i < index.length; i++) {
			s[i] = this.min + (index[i] * width);
		}

		return s;
	};

	/**
	 * Round the given value up the match the lower border of the current
	 * segment.
	 * 
	 * @param x
	 *            The value to round.
	 * 
	 * @returns The next smaller grid cell point.
	 * 
	 * Because of the symmetry we can use this function to round to the next
	 * floor point in either dimension.
	 */
	this.floorToCell = function(x) {
		var width = (this.max - this.min) / this.segments;
		var cell = Math.floor((x - this.min) / width);
		return this.min + width * cell;
	};

	/**
	 * Round the given value up the match the upper border of the current
	 * segment.
	 * 
	 * @param x
	 *            The value to round.
	 * 
	 * @returns The next greater grid cell point.
	 * 
	 * Because of the symmetry we can use this function to round to the next
	 * cell point in either dimension.
	 */
	this.ceilToCell = function(x) {
		var width = (this.max - this.min) / this.segments;
		var cell = Math.ceil((x - this.min) / width);
		return this.min + width * cell;
	};

	/**
	 * Returns a convex combination of a feasible point.
	 * 
	 * @param x
	 *            The point to decompose.
	 * 
	 * @returns A LinearCombination such that lc.eval() equals x. And x is
	 *          contained in the convex hull of the points of the linear
	 *          combination.
	 * 
	 * Please note that feasibility is assumed and not checked.
	 * 
	 * The convex combination is being built using the vertices of the cube
	 * containing "x". The LinearCombination is packed before being returned.
	 */
	this.convexDecomposition = function(x) {
		// Initialize the linear combination with clones of x and empty indices
		var ret = new LinearCombination();
		for ( var i = 0; i < Math.pow(2, x.length); i++) {
			ret.points.push(x.slice(0));
			ret.factors.push(1);
		}

		/*
		 * d is the direction (or since we are projecting alongside the axis of
		 * our coordinate system) the dimension we are currently working on.
		 */
		for ( var d = 0; d < x.length; d++) {

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
			var numChilds = Math.pow(2, x.length - 1 - d);

			/*
			 * For each dimension we loop through every point in the convex
			 * decomposition, to make sure we get all the corners of every
			 * point.
			 */
			while (i < Math.pow(2, x.length)) {

				/*
				 * First check if the current index is null. that might happen
				 * if there is a dimension d such that the given point is
				 * already on the border of d. (e.g. on the side of the cube).
				 * 
				 * In that case we set the "ceil part" of the decomposition to
				 * null since we don't need that points for the decomposition.
				 */
				if (ret.points[i] != null) {
					// calculate floor and ceil values.
					var f = this.floorToCell(ret.points[i][d]);
					var c = this.ceilToCell(ret.points[i][d]);
					var width = (this.max - this.min) / this.segments;
					var lambda = 1 - (ret.points[i][d] - f) / width;

					for ( var j = 0; j < numChilds; j++) {
						/* set the factor, point and grid index. */
						ret.factors[i] = ret.factors[i] * lambda;
						ret.points[i][d] = f;

						i++;
					}

					for ( var j = 0; j < numChilds; j++) {
						if (f != c) {
							/* set the factor, point and grid index. */
							ret.factors[i] = ret.factors[i] * (1 - lambda);
							ret.points[i][d] = c;
						} else {
							if (ret.points[i] != null) {
								/*
								 * delete all the points and indices since they
								 * would be either duplicates of floor or if we
								 * in fact move to the next ceiling point, the
								 * factor of the point would be 0 therefore it
								 * would not matter anyways.
								 */
								ret.points[i] = null;
								ret.factors[i] = 0;
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

		/*
		 * The return values are packed to make it easier and faster to consume
		 * for others.
		 */
		ret.pack();
		return ret;
	};

	/**
	 * Determines if the given state is located on the discrete grid of the
	 * domain.
	 * 
	 * @param x
	 *            The state.
	 * 
	 * @returns True if it is part of the discrete grid; otherwise false.
	 * 
	 * In this implementation it is considered a grid point if its position
	 * differs from the nearest GridPoint by at most 1% of the cell width.
	 */
	this.isGridPoint = function(x) {
		var width = (this.max - this.min) / this.segments;
		for ( var i = 0; i < x.length; i++) {
			// For performance reasons we do the feasibility check on the fly.
			if (x[i] < this.min || x[i] > this.max) {
				return false;
			}

			// Compute the unrounded GridPointIndex.
			var index = (x[i] - this.min) / width;
			// Then check the distance from the unrounded index to the rounded.
			// If this differ by more than 1% width we're not an GridPoint.
			var dist = Math.abs(index - Math.round(index));
			if (dist > 0.01) {
				return false;
			}
		}
		return true;
	};

	/**
	 * Determines if a given point is feasible.
	 * 
	 * @param x
	 *            The point to determine feasibility.
	 * 
	 * @returns true if it is feasible; false otherwise.
	 * 
	 * Points are regarded as feasible as long as they are contained in the
	 * domain.
	 */
	this.isFeasible = function(x) {
		for ( var i = 0; i < x.length; i++) {
			if (x[i] < this.min || x[i] > this.max) {
				return false;
			}
		}
		return true;
	};
}
