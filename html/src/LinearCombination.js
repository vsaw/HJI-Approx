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
 * Represents a linear combination of a point x such that
 * 
 * x = factor0 * point0 + factor1 * point1 + ... + factorN * pointN
 * 
 * Please note that this class will mostly be used for convex combinations.
 * Although the factors are not scanned to ensure a convex combination.
 */
function LinearCombination() {

	/**
	 * Contains all the points of this combination.
	 */
	this.points = [];
	/**
	 * Contains all the factors of this combination.
	 */
	this.factors = [];

	/**
	 * Evaluates the linear combination and returns the target it is pointing
	 * to.
	 * 
	 * @returns The target of the linear combination.
	 * 
	 * This will evaluate the following equation and return the variable x
	 * 
	 * x = factor0 * point0 + factor1 * point1 + ... + factorN * pointN
	 */
	this.eval = function() {
		var ret = 0;
		if (this.points.length > 0 && Array.isArray(this.points[0])) {
			ret = [];
			ret.length = this.points[0].length;
			for ( var i = 0; i < ret.length; i++) {
				ret[i] = 0;
			}
		}

		for ( var i = 0; i < Math.min(this.points.length, this.factors.length); i++) {
			if (Array.isArray(ret)) {
				for ( var j = 0; j < ret.length; j++) {
					if (this.points[i] != null) {
						ret[j] += this.factors[i] * this.points[i][j];
					}
				}
			} else {
				ret += this.factors[i] * this.points[i];
			}
		}

		return ret;
	};

	/**
	 * Packs the LinearCombination to the minimum viable necessary points.
	 * 
	 * This will strip the i-th entry from the LinearCombination if either the
	 * i-th factor is 0 or the i-th point is null.
	 */
	this.pack = function() {
		var points = [];
		var factors = [];

		for ( var i = 0; i < Math.min(this.points.length, this.factors.length); i++) {
			if (this.points[i] != null && this.factors[i] != 0) {
				points.push(this.points[i]);
				factors.push(this.factors[i]);
			}
		}

		this.points = points;
		this.factors = factors;
	};
}
