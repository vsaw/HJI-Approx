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

var LinearCombination = function() {

	var factors = [];
	var points = [];

	for ( var i = 0; i < Definitions.LINEAR_COMBINATON_LENGTH; i++) {
		factors[i] = 1;
		points[i] = null;
	}

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

	this.getFactor = function(index) {
		return factors[index];
	};

	this.getPoint = function(index) {
		return points[index];
	};

	this.setFactor = function(index, factor) {
		factors[index] = factor;
	};

	this.setPoint = function(index, point) {
		points[index] = point;
	};
};

module.exports = LinearCombination;