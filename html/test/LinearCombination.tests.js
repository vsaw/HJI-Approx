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

module("LinearCombination", {
	setup : function() {
		// prepare something for all following tests
		lc = new LinearCombination();
	},
	teardown : function() {
		// clean up after each test
	}
});

/**
 * Common LinearCombination used by all tests.
 */
var lc;

test("eval empty", function() {
	equal(lc.eval(), 0);
});

test("eval empty points", function() {
	lc.factors.push(1);
	lc.factors.push(2);
	lc.factors.push(3);
	equal(lc.eval(), 0);
});

test("eval empty factors", function() {
	lc.points.push(1);
	lc.points.push(2);
	lc.points.push(3);
	equal(lc.eval(), 0);
});

test("eval 1 dimension", function() {
	lc.factors.push(1);
	lc.factors.push(2);
	lc.factors.push(3);

	lc.points.push(1);
	lc.points.push(2);
	lc.points.push(3);

	equal(lc.eval(), 1 + 2 * 2 + 3 * 3);
});

test("eval 1 dimension fail", function() {
	lc.factors.push(1);
	lc.factors.push("z");
	lc.factors.push(3);

	lc.points.push(1);
	lc.points.push(2);
	lc.points.push(3);

	ok(isNaN(lc.eval()));
});

test("eval 1 not enough points", function() {
	lc.factors.push(1);
	lc.factors.push(2);
	lc.factors.push(3);

	lc.points.push(1);
	lc.points.push(3);

	equal(lc.eval(), 1 + 2 * 3);
});

test("eval 1 dimension not enough factors", function() {
	lc.factors.push(1);
	lc.factors.push(2);

	lc.points.push(1);
	lc.points.push(2);
	lc.points.push(3);

	equal(lc.eval(), 1 + 2 * 2);
});

test("eval n dimensions", function() {
	lc.factors.push(1);
	lc.factors.push(2);
	lc.factors.push(3);

	lc.points.push([ 1, 2, 3 ]);
	lc.points.push([ 4, 5, 6 ]);
	lc.points.push([ 7, 8, 9 ]);

	deepEqual(lc.eval(), [ 1 + 2 * 4 + 3 * 7, 2 + 2 * 5 + 3 * 8,
			3 + 2 * 6 + 3 * 9 ]);
});

test("eval n dimensions with m points", function() {
	lc.factors.push(1);
	lc.factors.push(2);
	lc.factors.push(3);
	lc.factors.push(0);

	lc.points.push([ 1, 2, 3 ]);
	lc.points.push([ 4, 5, 6 ]);
	lc.points.push([ 7, 8, 9 ]);
	lc.points.push([ 7, 8, 9 ]);

	deepEqual(lc.eval(), [ 1 + 2 * 4 + 3 * 7, 2 + 2 * 5 + 3 * 8,
			3 + 2 * 6 + 3 * 9 ]);
});

test("eval expected fail", function() {
	lc.factors.push([ 1, 3 ]);
	lc.factors.push(2);
	lc.factors.push(3);

	lc.points.push([ 1, 2, 3 ]);
	lc.points.push([ 4, 5, 6 ]);
	lc.points.push([ 7, 8, 9 ]);

	deepEqual(lc.eval(), [ NaN, NaN, NaN ]);
});

test("eval expected fail", function() {
	lc.factors.push(1);
	lc.factors.push("a");
	lc.factors.push(3);

	lc.points.push([ 1, 2, 3 ]);
	lc.points.push([ 4, 5, 6 ]);
	lc.points.push([ 7, 8, 9 ]);

	deepEqual(lc.eval(), [ NaN, NaN, NaN ]);
});

test("eval expected fail", function() {
	lc.factors.push(1);
	lc.factors.push(2);
	lc.factors.push(3);

	lc.points.push([ NaN, 2, 3 ]);
	lc.points.push([ 4, 5, 6 ]);
	lc.points.push([ 7, 8, 9 ]);

	deepEqual(lc.eval(), [ NaN, 2 + 2 * 5 + 3 * 8, 3 + 2 * 6 + 3 * 9 ]);
});

test("no array in the middle", function() {
	lc.factors.push(1);
	lc.factors.push(2);
	lc.factors.push(3);

	lc.points.push([ 1, 2, 3 ]);
	lc.points.push(1);
	lc.points.push([ 7, 8, 9 ]);

	deepEqual(lc.eval(), [ NaN, NaN, NaN ]);
});

test("wrong array in the middle", function() {
	lc.factors.push(1);
	lc.factors.push(2);
	lc.factors.push(3);

	lc.points.push([ 1, 2, 3 ]);
	lc.points.push([ 1 ]);
	lc.points.push([ 7, 8, 9 ]);

	deepEqual(lc.eval(), [ 24, NaN, NaN ]);
});

delete lc;
