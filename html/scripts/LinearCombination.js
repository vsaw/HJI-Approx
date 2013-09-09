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
 * LinearCombination.js
 *
 *  Created on: 2013-08-27
 *      Author: Valentin Sawadski <valentin@sawadski.eu>
 */

var Definitions = require('./Definitions.js');

/**
 * Represents a linear combination of a point x such that
 * 
 * x = factor0 * point0 + factor1 * point1 + ... + factorN * pointN
 * 
 * Please note that this class will mostly be used for convex combinations.
 * Although the factors are not scanned to ensure a convex combination.
 */
var LinearCombination = function() {

	/**
	 * An array containing the factors of the linear combination.
	 */
	var factors = [];
	/**
	 * An array of states being the points of the linear combination.
	 */
	var points = [];
	/**
	 * An array containing the indexes of the points stored in the points array.
	 * 
	 * The index number "i" is the same as calling the
	 * getPointIndex(getPoint(i)) but is faster in runtime.
	 */
	var indexes = [];

	for ( var i = 0; i < Definitions.LINEAR_COMBINATON_LENGTH; i++) {
		factors[i] = 1;
		points[i] = null;
	}

	/**
	 * Evaluates the linear combination and returns the target it is pointing
	 * to.
	 * 
	 * This will evaluate the following equation and return the variable x
	 * 
	 * x = factor0 * point0 + factor1 * point1 + ... + factorN * pointN
	 * 
	 * @returns The target of the linear combination.
	 */
	this.eval = function() {
		var ret = [];
		for ( var i = 0; i < Definitions.STATE_SPACE_DIMENSION; i++) {
			ret[i] = 0;
		}

		for ( var i = 0; i < Definitions.LINEAR_COMBINATON_LENGTH; i++) {
			var next = this.getPoint(i);
			if (next != null) {
				for ( var j = 0; j < Definitions.STATE_SPACE_DIMENSION; j++) {
					ret[j] = ret[j] + this.getFactor(i) * next[j];
				}
			}
		}

		return ret;
	};

	/**
	 * Returns the i-th factor.
	 * 
	 * @param i
	 *            The index. Must be 0 <= i <= LINEAR_COMBINATION_LENGTH
	 * 
	 * @returns The i-th factor.
	 * 
	 * @throws EXCEPTION_INDEX_OUT_OF_BOUNDS
	 *             i was not a valid index. Must be 0 <= i <=
	 *             LINEAR_COMBINATION_LENGTH
	 */
	this.getFactor = function(index) {
		return factors[index];
	};

	/**
	 * Returns the index of the i-th point of this linear combination.
	 * 
	 * The index number "i" is the same as calling the
	 * getPointIndex(getPoint(i)) but is faster in runtime.
	 * 
	 * @param i
	 *            The index number of the index in this linear combination.
	 * 
	 * @returns NULL if the point is NULL.
	 */
	this.getIndex = function(index) {
		return indexes[index];
	};

	/**
	 * Returns the i-th point.
	 * 
	 * The point might be null if not used in the linear combination. However it
	 * might still be possible that an point with an index larger than "i" is
	 * set.
	 * 
	 * @param i
	 *            The index. Must be 0 <= i <= LINEAR_COMBINATION_LENGTH
	 * 
	 * @returns The i-th point.
	 * 
	 * @throws EXCEPTION_INDEX_OUT_OF_BOUNDS
	 *             i was not a valid index. Must be 0 <= i <=
	 *             LINEAR_COMBINATION_LENGTH
	 */
	this.getPoint = function(index) {
		return points[index];
	};

	/**
	 * Returns the length of the linear combination.
	 * 
	 * @returns the length of the linear combination.
	 */
	this.getSize = function() {
		return Definitions.LINEAR_COMBINATON_LENGTH;
	};

	/**
	 * Sets the i-th factor of the linear combination.
	 * 
	 * @param i
	 *            The index
	 * @param f
	 *            the new value.
	 * 
	 * @throws EXCEPTION_INDEX_OUT_OF_BOUNDS
	 *             i was not a valid index. Must be 0 <= i <=
	 *             LINEAR_COMBINATION_LENGTH
	 */
	this.setFactor = function(index, factor) {
		factors[index] = factor;
	};

	/**
	 * Sets the i-th GridPointIndex of the linear combination.
	 * 
	 * @param i
	 *            The index
	 * @param x
	 *            The new GridPointIndex.
	 * 
	 * @throws EXCEPTION_INDEX_OUT_OF_BOUNDS
	 *             i was not a valid index. Must be 0 <= i <=
	 *             LINEAR_COMBINATION_LENGTH
	 */
	this.setIndex = function(index, number) {
		indexes[index] = number;
	};

	/**
	 * Sets the i-th point of the linear combination.
	 * 
	 * @param i
	 *            The index
	 * @param x
	 *            The new point.
	 * 
	 * @throws EXCEPTION_INDEX_OUT_OF_BOUNDS
	 *             i was not a valid index. Must be 0 <= i <=
	 *             LINEAR_COMBINATION_LENGTH
	 */
	this.setPoint = function(index, point) {
		points[index] = point;
	};
};

module.exports = LinearCombination;