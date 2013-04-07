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

module("SquareDomain", {
	setup : function() {
		// prepare something for all following tests
	},
	teardown : function() {
		// clean up after each test
	}
});

test("convex combination", function() {
	var sq = new SquareDomain(-1, 1, 2);

	var p0 = [ 0.0, 0.0, 0.0, 0.0 ];
	var lc = sq.convexDecomposition(p0);
	var actual = lc.eval();
	equal(lc.factors
			.reduce(function(previousValue, currentValue, index, array) {
				return previousValue + currentValue;
			}), 1);
	ok(State.equals(p0, actual, 1e-9));

	var p1 = [ 0.1, 0.9, -0.5, -1.0 ];
	lc = sq.convexDecomposition(p1);
	actual = lc.eval();
	equal(lc.factors
			.reduce(function(previousValue, currentValue, index, array) {
				return previousValue + currentValue;
			}), 1);
	ok(State.equals(p1, actual, 1e-9));

	var p2 = [ 1.0, 1.0, 1.0, 1.0 ];
	lc = sq.convexDecomposition(p2);
	actual = lc.eval();
	ok(State.equals(p2, actual, 1e-9));

	var p3 = [ -1.0, -1.0, -1.0, -1.0 ];
	lc = sq.convexDecomposition(p3);
	actual = lc.eval();
	ok(State.equals(p3, actual, 1e-9));

	var p4 = [ 0.5, 0.5, 0.5, 0.5 ];
	lc = sq.convexDecomposition(p4);
	actual = lc.eval();
	ok(State.equals(p4, actual, 1e-9));

	var p5 = [ 0.0, 0.0 ];
	var lc = sq.convexDecomposition(p5);
	var actual = lc.eval();
	equal(lc.factors
			.reduce(function(previousValue, currentValue, index, array) {
				return previousValue + currentValue;
			}), 1);
	ok(State.equals(p5, actual, 1e-9));

	var p6 = [ 0.0, 0.0, 1.3 ];
	var lc = sq.convexDecomposition(p6);
	var actual = lc.eval();
	ok(State.equals(p6, actual, 1e-9));

	var sq = new SquareDomain(-1, 1, 3);
	var p6 = [ 0.0, 0.0, 1.3 ];
	var lc = sq.convexDecomposition(p6);
	var actual = lc.eval();
	ok(State.equals(p6, actual, 1e-9));
});

test("get grid index", function() {
	var points = [ [ 0.0, 0.0, 0.0, 0.0 ], [ 0.0, 0.0, 0.1, 0.0 ],
			[ 0.0, 0.0, 0.1, 0.1 ], [ 1.0, 0.5, 0.0, 1.0 ],
			[ -1.0, -1.0, -0.5, -0.2 ], [ -1.0, -1.0, -1.0, -1.0 ] ];
	var indexes = [ [ 10, 10, 10, 10 ], [ 10, 10, 11, 10 ], [ 10, 10, 11, 11 ],
			[ 20, 15, 10, 20 ], [ 0, 0, 5, 8 ], [ 0, 0, 0, 0 ] ];
	var sq = new SquareDomain(-1, 1, 21);

	for ( var i = 0; i < points.length; i++) {
		var ret = sq.getGridIndex(points[i]);
		deepEqual(ret, indexes[i]);
	}
});

test("max grid index", function() {
	var param = [ 10, 20, 2 ];

	for ( var outer = 0; outer < param.length; outer++) {
		var sq = new SquareDomain(0, 1, param[outer]);
		var ind = sq.getMaximalGridIndex(4);

		for ( var i = 0; i < ind.length; i++) {
			ok(Math.abs(param[outer] - 1 - ind[i]) < 1e-9, outer * 10 + i);
		}
	}
});

test("get point", function() {
	var sq = new SquareDomain(0, 1, 11);

	var c0 = [ 0, 0, 0, 0, 0 ];
	var p0 = [ 0.0, 0.0, 0.0, 0.0, 0.0 ];
	var s0 = sq.getPoint(c0);
	deepEqual(p0, s0);

	var p1 = [ 0.5, 0.4, 0.3, 0.0 ];
	var c1 = [ 5, 4, 3, 0 ];
	var s1 = sq.getPoint(c1);
	ok(State.equals(p1, s1, 1e-9));

	var p2 = [ 0.1, 0.1, 0.1, 0.0, 0.1 ];
	var c2 = [ 1, 1, 1, 0, 1 ];
	var s2 = sq.getPoint(c2);
	ok(State.equals(p2, s2, 1e-9));

	var p3 = [ 0.1 ];
	var c3 = [ 1 ];
	var s3 = sq.getPoint(c3);
	ok(State.equals(p3, s3, 1e-9));

	var sq = new SquareDomain(-1, 1, 11);
	var c4 = [ 0, 0, 0, 0 ];
	var p4 = [ -1.0, -1.0, -1.0, -1.0 ];
	var s4 = sq.getPoint(c4);
	deepEqual(p4, s4);

	var p5 = [ -0.8, -0.8, -0.8, -1.0 ];
	var c5 = [ 1, 1, 1, 0 ];
	var s5 = sq.getPoint(c5);
	ok(State.equals(p5, s5, 1e-9));
});

test("is grid point", function() {
	var onGrid = [ [ 0.0001, 0.0, 0.0, 0.0 ], [ 0.0, 0.0, 0.1, 0.0 ],
			[ 0.0, 0.0001, 0.1, 0.1 ], [ 0.0, 0.5, 1.0, 1.0 ],
			[ 1.0, 0.99999, 1.0, 1.0 ], [ 1.0, 0.99999, 1.0 ] ];

	var offGrid = [ [ 0.02, 0.0, 0.0, 0.0 ], [ 0.0, 0.0, -0.1, 0.0 ],
			[ 0.0, 0.0, 0.1, 0.87 ], [ 0.0, 0.55, 1.0, 1.0 ],
			[ 1.0, 1.0, 1.1, 1.0 ], [ 1.0, 1.0, 1.1 ] ];

	var sq = new SquareDomain(0, 1, 11);

	for ( var i = 0; i < onGrid.length; i++) {
		ok(sq.isGridPoint(onGrid[i]), i);
	}

	for ( var i = 0; i < offGrid.length; i++) {
		ok(!sq.isGridPoint(offGrid[i]), i);
	}
});

test("is feasible", function() {
	var onGrid = [ [ 0.0001, 0.0, 0.0, 0.0 ], [ 0.0, 0.0, 0.1, 0.0 ],
			[ 0.0, 0.0001, 0.1, 0.1 ], [ 0.0, 0.5, 1.0, 1.0 ],
			[ 1.0, 0.99999, 1.0, 1.0 ], [ 1.0, 0.99999, 1.0 ] ];

	var offGrid = [ [ -0.02, 0.0, 0.0, 0.0 ], [ 0.0, 0.0, -0.1, 0.0 ],
			[ 0.0, 0.0, 0.1, -0.87 ], [ 0.0, -0.55, 1.0, 1.0 ],
			[ 1.0, 1.0, 1.1, 1.0 ], [ 1.0, 1.0, 1.1 ] ];

	var sq = new SquareDomain(0, 1, 11);

	for ( var i = 0; i < onGrid.length; i++) {
		ok(sq.isFeasible(onGrid[i]), i);
	}

	for ( var i = 0; i < offGrid.length; i++) {
		ok(!sq.isFeasible(offGrid[i]), i);
	}
});
