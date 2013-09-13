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

	it("knows grid indexes", function() {
		var sq = new SquareDomain(-1, 1, 21);

		var points = [ [ 0.0, 0.0, 0.0, 0.0 ], [ 0.0, 0.0, 0.1, 0.0 ],
				[ 0.0, 0.0, 0.1, 0.1 ], [ 1.0, 0.5, 0.0, 1.0 ],
				[ -1.0, -1.0, -0.5, -0.2 ], [ -1.0, -1.0, -1.0, -1.0 ] ];

		var index = [ [ 10, 10, 10, 10 ], [ 10, 10, 11, 10 ],
				[ 10, 10, 11, 11 ], [ 20, 15, 10, 20 ], [ 0, 0, 5, 8 ],
				[ 0, 0, 0, 0 ] ];

		for ( var i = 0; i < points.length; i++) {
			var ret = sq.getGridIndex(points[i]);
			for ( var j = 0; j < 4; j++) {
				expect(ret[j]).toBe(index[i][j]);
			}
		}
	});

	it("knows the maximal grid index", function() {
		var param = [ 10, 20, 2 ];

		for ( var outer = 0; outer < param.length; outer++) {
			var sq = new SquareDomain(0, 1, param[outer]);
			var ind = sq.getMaximalGridIndex();
			for ( var i = 0; i < Definitions.STATE_SPACE_DIMENSION; i++) {
				expect(param[outer] - 1).toBeCloseTo(ind[i], 6);
			}
		}
	});

	it("knows the point", function() {
		var sq = new SquareDomain(0, 1, 11);

		var c0 = [ 0, 0, 0, 0 ];
		var p0 = [ 0.0, 0.0, 0.0, 0.0 ];
		var s0 = sq.getPoint(c0);
		expect(GameStateDomain.equals(p0, s0)).toBe(true);

		var p1 = [ 0.5, 0.4, 0.3, 0.0 ];
		var c1 = [ 5, 4, 3, 0 ];
		var s1 = sq.getPoint(c1);
		expect(GameStateDomain.equals(p1, s1, 1e-9)).toBe(true);

		var p2 = [ 0.1, 0.1, 0.1, 0.0 ];
		var c2 = [ 1, 1, 1, 0 ];
		var s2 = sq.getPoint(c2);
		expect(GameStateDomain.equals(p2, s2, 1e-9)).toBe(true);

		sq = new SquareDomain(-1, 1, 11);
		var p3 = [ -1.0, -1.0, -1.0, -1.0 ];
		var s3 = sq.getPoint(c0);
		expect(GameStateDomain.equals(p3, s3)).toBe(true);

		var p4 = [ -0.8, -0.8, -0.8, -1.0 ];
		var s4 = sq.getPoint(c2);
		expect(GameStateDomain.equals(p4, s4, 1e-9)).toBe(true);
	});

	it("knows if its a grid point", function() {
		var sq = new SquareDomain(0, 1, 11);

		var onGrid = [ [ 0.0001, 0.0, 0.0, 0.0 ], [ 0.0, 0.0, 0.1, 0.0 ],
				[ 0.0, 0.0001, 0.1, 0.1 ], [ 0.0, 0.5, 1.0, 1.0 ],
				[ 1.0, 0.99999, 1.0, 1.0 ] ];
		var offGrid = [ [ 0.02, 0.0, 0.0, 0.0 ], [ 0.0, 0.0, -0.1, 0.0 ],
				[ 0.0, 0.0, 0.1, 0.87 ], [ 0.0, 0.55, 1.0, 1.0 ],
				[ 1.0, 1.0, 1.1, 1.0 ] ];

		for ( var i = 0; i < 5; i++) {
			expect(sq.isGridPoint(onGrid[i])).toBe(true);
		}

		for ( var i = 0; i < 5; i++) {
			expect(sq.isGridPoint(offGrid[i])).toBe(false);
		}
	});

	it("is equal to itself", function() {
		var sq = new SquareDomain(0, 1, 11);

		expect(sq.equals(sq)).toBe(true);
	});

	it("equals to be recursive", function() {
		var sq = new SquareDomain(0, 1, 11);
		var sq2 = new SquareDomain(0, 1, 11);

		expect(sq.equals(sq2)).toBe(true);
		expect(sq2.equals(sq)).toBe(true);
	});

	it("equals to be recursive", function() {
		var sq = new SquareDomain(0, 1, 11);
		var sq2 = new SquareDomain(1, 1, 11);
		var sq3 = new SquareDomain(0, 2, 11);
		var sq4 = new SquareDomain(0, 1, 12);

		expect(sq.equals(sq2)).toBe(false);
		expect(sq.equals(sq3)).toBe(false);
		expect(sq.equals(sq4)).toBe(false);
	});
});
