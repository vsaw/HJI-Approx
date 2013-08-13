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

module("ValueFunction", {
	setup : function() {
		// prepare something for all following tests
	},
	teardown : function() {
		// clean up after each test
	}
});

test("get value", function() {
	var n = 10;
	var sq = new SquareDomain(0, 1, n + 1);
	var v = new ValueFunction(sq);

	// New ValueFunctions must be equal to the zero function.
	for ( var i0 = 0; i0 <= n; i0++) {
		for ( var i1 = 0; i1 <= n; i1++) {
			for ( var i2 = 0; i2 <= n; i2++) {
				for ( var i3 = 0; i3 <= n; i3++) {
					equal(v.getValue([ i0, i1, i2, i3 ]), undefined);
				}
			}
		}
	}
});

test("to array index", function() {
	var n = 10;
	var sq = new SquareDomain(0, 1, n + 1);
	var v = new ValueFunction(sq);

	equal(v.toArrayIndex([ 0, 0, 0, 0 ]), 0);
	equal(v.toArrayIndex([ 0, 0, 0 ]), 0);
	equal(v.toArrayIndex([ 0 ]), 0);

	equal(v.toArrayIndex([ 1 ]), 1);
	equal(v.toArrayIndex([ 10 ]), 10);
	equal(v.toArrayIndex([ 1, 0 ]), 11);
	equal(v.toArrayIndex([ 1, 2 ]), 13);
	equal(v.toArrayIndex([ 1, 10 ]), 21);

	notEqual(v.toArrayIndex([ 2, 5, 10, 0 ]), v.toArrayIndex([ 2, 5, 9, 10 ]));
	notEqual(v.toArrayIndex([ 2, 5, 10, 0 ]), v.toArrayIndex([ 2, 6, 0, 10 ]));
	notEqual(v.toArrayIndex([ 2, 5, 9, 10 ]), v.toArrayIndex([ 2, 6, 0, 10 ]));
});

test("get value altered", function() {
	var n = 10;
	var sq = new SquareDomain(0, 1, n + 1);
	var v = new ValueFunction(sq);

	// Change a single value and see if it's still working out as expected.
	var indexx = [ [ 0, 0, 0, 0 ], [ 0, 0, 5, 8 ], [ 2, 5, 10, 0 ],
			[ 4, 10, 8, 10 ], [ 10, 1, 3, 1 ], [ 10, 10, 10, 10 ] ];

	for ( var i = 0; i < indexx.length; i++) {
		v.setValue(indexx[i], i);
	}

	// New ValueFunctions must be equal to the zero function.
	for ( var i0 = 0; i0 <= n; i0++) {
		for ( var i1 = 0; i1 <= n; i1++) {
			for ( var i2 = 0; i2 <= n; i2++) {
				for ( var i3 = 0; i3 <= n; i3++) {
					if (i0 == 0 && i1 == 0 && i2 == 0 && i3 == 0) {
						equal(v.getValue([ i0, i1, i2, i3 ]), 0);
					} else if (i0 == 0 && i1 == 0 && i2 == 5 && i3 == 8) {
						equal(v.getValue([ i0, i1, i2, i3 ]), 1);
					} else if (i0 == 2 && i1 == 5 && i2 == 10 && i3 == 0) {
						equal(v.getValue([ i0, i1, i2, i3 ]), 2);
					} else if (i0 == 4 && i1 == 10 && i2 == 8 && i3 == 10) {
						equal(v.getValue([ i0, i1, i2, i3 ]), 3);
					} else if (i0 == 10 && i1 == 1 && i2 == 3 && i3 == 1) {
						equal(v.getValue([ i0, i1, i2, i3 ]), 4);
					} else if (i0 == 10 && i1 == 10 && i2 == 10 && i3 == 10) {
						equal(v.getValue([ i0, i1, i2, i3 ]), 5);
					} else {
						equal(v.getValue([ i0, i1, i2, i3 ]), undefined, [ i0,
								i1, i2, i3 ]);
					}
				}
			}
		}
	}
});

test("eval", function() {
	var n = 10;
	var sq = new SquareDomain(0, 1, n + 1);
	var v = new ValueFunction(sq);

	// New ValueFunctions must be equal to the zero function.
	for ( var i0 = 0; i0 <= n; i0++) {
		for ( var i1 = 0; i1 <= n; i1++) {
			for ( var i2 = 0; i2 <= n; i2++) {
				for ( var i3 = 0; i3 <= n; i3++) {
					v.setValue([ i0, i1, i2, i3 ], 0);
				}
			}
		}
	}

	var p = [ [ 0.5, 0.5, 0.5, 0.5 ], [ 0, 0, 0, 0 ], [ 0.01, 0.01, 0, 0 ] ];

	equal(v.eval(p[0]), 0);
	v.setValue([ 0, 0, 0, 0 ], 1);
	equal(v.eval(p[1]), 1);
	equal(v.eval(p[0]), 0);
	equal(v.eval(p[2]), 0.81);
});

test("equals empty", function() {
	var n = 10;
	var sq = new SquareDomain(0, 1, n + 1);
	var v1 = new ValueFunction(sq);
	var v2 = new ValueFunction(sq);

	// Equals has to be symmetric
	ok(v1.equals(v2));
	ok(v2.equals(v1));

	ok(!v1.equals(null));
	ok(!v2.equals(21));
	ok(!v1.equals(undefined));
	ok(!v2.equals("bacon!"));
	ok(!v1.equals([ 1, 3, 4, 5, 6 ]));
});

test("equals non empty", function() {
	var n = 10;
	var sq = new SquareDomain(0, 1, n + 1);
	var v1 = new ValueFunction(sq);
	var v2 = new ValueFunction(sq);

	// New ValueFunctions must be equal to the zero function.
	for ( var i0 = 0; i0 <= n; i0++) {
		for ( var i1 = 0; i1 <= n; i1++) {
			for ( var i2 = 0; i2 <= n; i2++) {
				for ( var i3 = 0; i3 <= n; i3++) {
					var index = [ i0, i1, i2, i3 ];
					v1.setValue(index, i0 + i1 + i2 + i3);
					v2.setValue(index, v1.getValue(index));
				}
			}
		}
	}

	// Equals has to be symmetric
	ok(v1.equals(v2));
	ok(v2.equals(v1));
});

test("not equals one empty one non empty", function() {
	var n = 10;
	var sq = new SquareDomain(0, 1, n + 1);
	var v1 = new ValueFunction(sq);
	var v2 = new ValueFunction(sq);

	v1.setValue([ 1, 2, 3, 4 ], 1);

	// Equals has to be symmetric
	ok(!v1.equals(v2));
	ok(!v2.equals(v1));
});

test("not equals non empty", function() {
	var n = 10;
	var sq = new SquareDomain(0, 1, n + 1);
	var v1 = new ValueFunction(sq);
	var v2 = new ValueFunction(sq);

	// New ValueFunctions must be equal to the zero function.
	for ( var i0 = 0; i0 <= n; i0++) {
		for ( var i1 = 0; i1 <= n; i1++) {
			for ( var i2 = 0; i2 <= n; i2++) {
				for ( var i3 = 0; i3 <= n; i3++) {
					var index = [ i0, i1, i2, i3 ];
					v1.setValue(index, i0 + i1 + i2 + i3);
					v2.setValue(index, v1.getValue(index));
				}
			}
		}
	}

	v2.setValue([ 1, 2, 3, 4 ], 0);

	// Equals has to be symmetric
	ok(!v1.equals(v2));
	ok(!v2.equals(v1));
});
