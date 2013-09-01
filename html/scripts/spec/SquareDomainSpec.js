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
 * SquareDomainSpec.js
 * 
 *  Created on: 2013-09-01
 *      Author: Valentin Sawadski <valentin@sawadski.eu>
 */

var LinearCombination = require('../LinearCombination.js');
var Definitions = require('../Definitions.js');
var SquareDomain = require('../SquareDomain.js');
var GameStateDomain = require('../GameStateDomain.js');

describe("SquareDomain", function() {

	it("gets indexes of convex decompositions", function() {
		var sq = new SquareDomain(-2, 2, 10);
		var s = [ 0.1, 0.9, -0.5, -1.0 ];

		var lc = sq.convexDecomposition(s);
		for ( var i = 0; i < Definitions.LINEAR_COMBINATON_LENGTH; i++) {
			var corner = lc.getPoint(i);
			if (corner != null) {
				var cornerIndex = sq.getGridIndex(corner);
				for ( var j = 0; j < Definitions.STATE_SPACE_DIMENSION; j++) {
					expect(cornerIndex[j]).toBeCloseTo(lc.getIndex(i)[j], 3);
				}
			}
		}
	});

	it("can convex decomposition points", function() {
		var checkConvexDecomposition = function(p, sq) {
			var lc = sq.convexDecomposition(p);
			var actual = lc.eval();
			// expect(p).toEqual(actual);
			expect(GameStateDomain.equals(p, actual, 1e-6)).toBe(true);
		};
		var sq = new SquareDomain(-1, 1, 2);

		checkConvexDecomposition([ 0.0, 0.0, 0.0, 0.0 ], sq);
		checkConvexDecomposition([ 0.1, 0.9, -0.5, -1.0 ], sq);
		checkConvexDecomposition([ 1.0, 1.0, 1.0, 1.0 ], sq);
		checkConvexDecomposition([ -1.0, -1.0, -1.0, -1.0 ], sq);
		checkConvexDecomposition([ 0.5, 0.5, 0.5, 0.5 ], sq);
	});
});
