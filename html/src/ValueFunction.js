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
 * @author Valentin Sawadski
 * 
 * @see Game
 * @see GameStateDomain
 * @see VALUE_UNDEFINED
 */
function ValueFunction(domain) {

	/**
	 * The Domain of the ValueFunction.
	 */
	this.domain = domain;
	/**
	 * The amount of iterations it took to compute the value function.
	 */
	this.iterations = undefined;
	/**
	 * This is array containing the values of the function at a given GridIndex.
	 * While the GridIndex is n-dimensional the value is a 1-dimensional array.
	 * 
	 * We can do this without a problem since there exists a bijective map from
	 * the int^n to int.
	 * 
	 * See the following scheme from the two-dimensional plane for example
	 * 
	 * <tt>
	 * 		6----7----8		Corner Number 0 has GridPointIndex {0, 0}
	 * 		|    |    |		Corner Number 1 has GridPointIndex {1, 0}
	 * 		|    |    |		...
	 * 		3----4----5		Corner Number 7 has GridPointIndex {1, 2}
	 * 		|    |    |		Corner Number 8 has GridPointIndex {2, 2}
	 * 		|    |    |
	 * 		0----1----2
	 * </tt>
	 * 
	 * To convert from GridIndex to Array Index and vice-versa use the
	 * toGridIndex(arrayIndex) and toArrayIndex(index) functions.
	 * 
	 * @see ValueFunction.toArrayIndex(index)
	 */
	this.values = [];

	/**
	 * Determines deep equality for a given ValueFunction.
	 *
	 * @param vfunc
	 *            The object to determine equality with.
	 *
	 * @return true if they are equal, false otherwise.
	 */
	this.equals = function(vfunc) {
		if (vfunc instanceof ValueFunction) {
			if (this === vfunc) {
				return true;
			}

			if (this.domain == null && vfunc.domain != null) {
				return false;
			} else {
				if (!this.domain.equals(vfunc.domain)) {
					return false;
				}
			}

			return JSON.stringify(this.values) === JSON.stringify(vfunc.values);
		}
		return false;
	};

	/**
	 * Evaluation the ValueFunction for the given state.
	 * 
	 * @param x
	 *            The state as an Array.
	 * @param timeValue
	 *            true if the transformation should be performed.
	 * 
	 * @returns The time value of the game.
	 * 
	 * Since the original plots of the value function were transformed by
	 * 
	 * T: R^4 -> R, (xy_P, xy_E) |-> -ln(1 - vfunc(xy_P, xy_E))
	 * 
	 * this function will also do the transformation if the "timeValue"
	 * parameter is true. If it is false, the unaltered values will be returned.
	 */
	this.eval = function(x, timeValue) {
		if (arguments.length == 1) {
			timeValue = false;
		}

		var lc = this.domain.convexDecomposition(x);
		var ret = 0;
		for ( var i = 0; i < lc.points.length; i++) {
			ret += lc.factors[i]
					* this.getValue(this.domain.getGridIndex(lc.points[i]));
		}

		if (timeValue) {
			ret = -Math.log(1 - ret);
		}

		return ret;
	};

	/**
	 * Converts an GridPointIndex to the position in the value array.
	 * 
	 * @param index
	 *            The index.
	 * 
	 * @returns The position of the GridCell in the values array.
	 */
	this.toArrayIndex = function(index) {
		var ret = 0;
		var multiple = 1;
		var maxIndex = this.domain.getMaximalGridIndex(index.length);
		for ( var i = index.length - 1; i >= 0; i--) {
			ret += index[i] * multiple;
			multiple *= (maxIndex[i] + 1);
		}
		return ret;
	};

	/**
	 * Returns the value of the ValueFunction at a given GridPointIndex.
	 * 
	 * @param index
	 *            The index as an array.
	 * 
	 * @returns The value of the function at the index.
	 */
	this.getValue = function(index) {
		return this.values[this.toArrayIndex(index)];
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
		this.values[this.toArrayIndex(index)] = value;
	};
}
