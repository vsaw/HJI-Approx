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
 * LinearCombinationSpec.js
 * 
 *  Created on: 2013-08-27
 *      Author: Valentin Sawadski <valentin@sawadski.eu>
 */

var LinearCombination = require('../LinearCombination.js');
var Definitions = require('../Definitions.js');
var GameStateDomain = require('../GameStateDomain.js');

describe("LinearCombination", function() {
	it("initializes with no points", function() {
		var lc = new LinearCombination();
		for ( var i = 0; i < Definitions.LINEAR_COMBINATON_LENGTH; i++) {
			expect(lc.getPoint(i)).toBeNull();
			expect(lc.getFactor(i)).toBeCloseTo(1, 6);
		}
	});

	it("gets and sets factors", function() {
		var lc = new LinearCombination();

		for ( var i = 0; i < Definitions.LINEAR_COMBINATON_LENGTH; i++) {
			lc.setFactor(i, i);
		}

		for ( var i = 0; i < Definitions.LINEAR_COMBINATON_LENGTH; i++) {
			expect(lc.getFactor(i)).toBe(i);
		}
	});

	it("gets and sets points", function() {
		var lc = new LinearCombination();

		for ( var i = 0; i < Definitions.LINEAR_COMBINATON_LENGTH; i++) {
			var p = [];
			for ( var j = 0; j < Definitions.STATE_SPACE_DIMENSION; j++) {
				p[j] = i;
			}
			lc.setPoint(i, p);
		}

		for ( var i = 0; i < Definitions.LINEAR_COMBINATON_LENGTH; i++) {
			var p = [];
			for ( var j = 0; j < Definitions.STATE_SPACE_DIMENSION; j++) {
				p[j] = i;
			}
			expect(lc.getPoint(i)).toEqual(p);
		}
	});

	it("can evaluate combinations correctly", function() {
		var lc = new LinearCombination();

		// Build Linear Combination.
		for ( var i = 0; i < Definitions.LINEAR_COMBINATON_LENGTH; i++) {
			lc.setFactor(i, Math.pow(2, -1
					* Definitions.LINEAR_COMBINATON_LENGTH));

			var dp = [];
			var p = i / 2;
			for ( var j = 0; j < Definitions.STATE_SPACE_DIMENSION; j++) {
				if (j == p) {
					dp[j] = Math.pow(-1, i);
				} else {
					dp[j] = 0;
				}
			}
			lc.setPoint(i, dp);
		}

		// Build Expected result
		var expected = [ 0.0, 0.0, 0.0, 0.0 ];

		var actual = lc.eval();
		expect(GameStateDomain.equals(expected, actual, 1e-4)).toBe(true);
	});
});
