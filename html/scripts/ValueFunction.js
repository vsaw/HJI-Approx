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
 * ValueFunction.js
 *
 *  Created on: 2013-09-09
 *      Author: Valentin Sawadski <valentin@sawadski.eu>
 */

var Definitions = require('./Definitions.js');

/**
 * The Value Function.
 * 
 * This function can be used to compute the value of the game. Basically it
 * linearly interpolates the value. It does so by storing value of the game at
 * the vertices of the grid. And then uses the convex-decomposition of the
 * GameDomain to determine the the grid it is in.
 * 
 * Please note that a new ValueFunction is set to VALUE_UNDEFINED at every point
 * of the GameStateDomain, unless you want to provide a different default value.
 * Use the Game class to compute the value function and set the values of the
 * game for certain states.
 * 
 * @see Game
 * @see GameStateDomain
 */
var ValueFunction = function(dom) {

	/**
	 * The Domain of the ValueFunction.
	 */
	var domain = dom;
	/**
	 * This is array containing the values of the function at a given GridIndex.
	 * While the GridIndex is 4-dimensional the value is a 1-dimensional array.
	 * 
	 * We can do this without a problem since there exists a bijective map from
	 * the int^4 to int.
	 * 
	 * See the following scheme from the two-dimensional plane for example
	 * 
	 * <pre>
	 * 		6----7----8 Corner Number 0 has GridPointIndex {0, 0}
	 * 		|    |    | Corner Number 1 has GridPointIndex {1, 0}
	 * 		|    |    | ...
	 * 		3----4----5 Corner Number 7 has GridPointIndex {1, 2}
	 * 		|    |    | Corner Number 8 has GridPointIndex {2, 2}
	 * 		|    |    |
	 * 		0----1----2
	 * </pre>
	 * 
	 * To convert from GridIndex to Array Index and vice-versa use the
	 * toGridIndex(int arrayIndex) and toArrayIndex(GridPointIndex index)
	 * functions.
	 * 
	 * @see ValueFunction::toArrayIndex(GridPointIndex index)
	 */
	var values = [];

	/**
	 * Determines if the SquareDomain is equal to "x"
	 * 
	 * @param x
	 *            The object to compare it to.
	 * 
	 * @returns True if it is equal to "x", false otherwise.
	 * 
	 * They are regarded as equal if "x" has an equal domain and the same
	 * values.
	 */
	this.equals = function(x) {
		if (this === x) {
			return true;
		}

		if (x instanceof ValueFunction) {
			if (domain != null) {
				if (!domain.equals(x.getDomain())) {
					return false;
				}
			} else {
				if (x.getDomain() != null) {
					return null;
				}
			}

			for (var i = 0; i < values.length; i++) {
				if (x.getValue(i) != values[i]) {
					return false;
				}
			}
			return true;
		}
		return false;
	};

	/**
	 * Evaluation the ValueFunction for the given state.
	 * 
	 * @param x
	 *            The state.
	 * 
	 * @returns The time value of the game.
	 * 
	 * Since the original plots of the value function were transformed by
	 * 
	 * T: R^4 -> R, (xy_P, xy_E) |-> -ln(1 - vfunc(xy_P, xy_E))
	 * 
	 * this function will also do the transformation if the "timeValue"
	 * parameter is true. If it is false, the unaltered values will be returned.
	 * 
	 */
	this.eval = function(x, timeValue) {
		var lc = this.getDomain().convexDecomposition(x);

		var ret = 0;
		for (var i = 0; i < lc.getSize(); i++) {
			if (lc.getPoint(i) != undefined) {
				ret += lc.getFactor(i) * this.getValue(lc.getIndex(i));
			}
		}

		if (timeValue) {
			ret = -log(1 - ret);
		}

		return ret;
	};

	/**
	 * Returns the domain of the game.
	 * 
	 * @returns The domain of the value function.
	 */
	this.getDomain = function() {
		return domain;
	};

	/**
	 * Gets the amount of iterations needed to compute the Value Function.
	 * 
	 * @returns The amount of iterations needed to compute the Value Function.
	 */
	this.getIterations = function() {
		return iterations;
	};

	/**
	 * Returns the value of the ValueFunction at a given GridPointIndex.
	 * 
	 * @param index
	 *            The index.
	 * 
	 * @returns The value of the function at the index.
	 */
	this.getValue = function(index) {
		return values[index];
	};

	/**
	 * Sets the amount of iterations needed to compute the Value Function.
	 * 
	 * @param n
	 *            The amount of iterations used to compute the value function
	 */
	this.setIterations = function(n) {
		iterations = n;
	};

	/**
	 * Sets the value of the function at the given index.
	 * 
	 * @param index
	 *            The index
	 * @param value
	 *            The new Value.
	 * 
	 * The values set here are used to evaluate the value of the function. The
	 * value of a state is linearly approximated by the values of its
	 * surrounding GridCells.
	 */
	this.setValue = function(index, value) {
		values[index] = value;
	};
};

module.exports = ValueFunction;