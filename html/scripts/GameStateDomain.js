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
 * GameStateDomain.js
 *
 *  Created on: 2013-08-27
 *      Author: Valentin Sawadski <valentin@sawadski.eu>
 */

var Definitions = require('./Definitions.js');

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
var GameStateDomain = new Object();

/**
 * Clones the given state.
 * 
 * @param state
 *            The state to clone
 * @returns A deep clone of the given state.
 */
GameStateDomain.cloneState = function(state) {
	return state.slice(0);
};

/**
 * Determines equality for given points with a tolerance eps.
 * 
 * @param x1
 *            The first state.
 * @param x2
 *            The second state.
 * @param eps
 *            The maximal distance between s1 and s2.
 * 
 * @returns True if the distance between x1 and x2 is less then or equal to eps
 *          measured in the infinity norm; false otherwise false.
 * 
 * Points are being regarded different in case their distance in the infinity
 * norm is greater than eps. In other words, if there exists an index i such
 * that | x1[i] - x2[i] | > eps.
 */
GameStateDomain.equals = function(s1, s2, eps) {
	if (arguments.length == 2) {
		eps = 0;
	}

	for ( var i = 0; i < Definitions.STATE_SPACE_DIMENSION; i++) {
		if (isNaN(s1[i]) || isNaN(s2[i])) {
			return false;
		}

		if (Math.abs(s1[i] - s2[i]) > eps) {
			return false;
		}
	}
	return true;
};

module.exports = GameStateDomain;
