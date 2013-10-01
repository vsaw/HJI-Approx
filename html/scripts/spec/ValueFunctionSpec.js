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
 * varERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * ValueFunctionSpec.js
 * 
 *  Created on: 2013-09-09
 *      Author: Valentin Sawadski <valentin@sawadski.eu>
 */

var Definitions = require('../Definitions.js');
var SquareDomain = require('../SquareDomain.js');
var ValueFunction = require('../ValueFunction.js');

describe("ValueFunctionSpec", function() {

	it("can get the default value", function() {
		var n = 10;
		var sq = new SquareDomain(0, 1, n + 1);
		var v = new ValueFunction(sq);

		// New ValueFunctions must be equal to the zero function.
		for (var i0 = 0; i0 <= n; i0++) {
			for (var i1 = 0; i1 <= n; i1++) {
				for (var i2 = 0; i2 <= n; i2++) {
					for (var i3 = 0; i3 <= n; i3++) {
						expect(v.getValue([ i0, i1, i2, i3 ])).toBe(undefined);
					}
				}
			}
		}
	});

	it("remembers the value if it has been changed",
			function() {
				var n = 10;
				var sq = new SquareDomain(0, 1, n);
				var index = [ 0, 0, 0, 0 ];
				var v = new ValueFunction(sq);

				var alteredPovars = [];

				var setAlteredPovars = function(index) {
					v.setValue(index, alteredPovars.length);
					alteredPovars[alteredPovars.length] = index;
				};

				var getAlteredPovars = function(index) {
					for (var i = 0; i < alteredPovars.length; i++) {
						var found = true;
						for (var j = 0; j < index.length; j++) {
							found &= index[j] == alteredPovars[i][j];
						}
						if (found) {
							return i;
						}
					}
					return undefined;
				};

				// Change a single value and see if it's still working
				// out as expected.
				setAlteredPovars([ 0, 0, 0, 0 ]);
				setAlteredPovars([ 0, 0, 5, 8 ]);
				setAlteredPovars([ 2, 5, 10, 0 ]);
				setAlteredPovars([ 4, 10, 8, 10 ]);
				setAlteredPovars([ 10, 1, 3, 1 ]);
				setAlteredPovars([ 10, 10, 10, 10 ]);

				// New ValueFunctions must be equal to the zero
				// function.
				for (var i0 = 0; i0 <= n; i0++) {
					index[0] = i0;
					for (var i1 = 0; i1 <= n; i1++) {
						index[1] = i1;
						for (var i2 = 0; i2 <= n; i2++) {
							index[2] = i2;
							for (var i3 = 0; i3 <= n; i3++) {
								index[3] = i3;

								expect(v.getValue(index)).toBe(
										getAlteredPovars(index));
							}
						}
					}
				}
			});

	it("can eval points",
			function() {

				var n = 10;
				var sq = new SquareDomain(0, 1, n);
				var index = [ 0, 0, 0, 0 ];
				var v = new ValueFunction(sq);

				// New ValueFunctions must be equal to the zero function.
				for (var i0 = 0; i0 <= n; i0++) {
					index[0] = i0;
					for (var i1 = 0; i1 <= n; i1++) {
						index[1] = i1;
						for (var i2 = 0; i2 <= n; i2++) {
							index[2] = i2;
							for (var i3 = 0; i3 <= n; i3++) {
								index[3] = i3;

								v.setValue(index, 0);
							}
						}
					}
				}

				for (var i = 0; i < 4; i++) {
					index[i] = 0;
				}

				var p = [ [ 0.5, 0.5, 0.5, 0.5 ], [ 0, 0, 0, 0 ],
						[ 0.01, 0.01, 0, 0 ] ];

				expect(v.eval(p[0])).toBeCloseTo(0, 3);

				v.setValue(index, 1);
				expect(v.eval(p[1])).toBeCloseTo(1, 3);
				expect(v.eval(p[0])).toBeCloseTo(0, 3);
				expect(v.eval(p[2])).toBeCloseTo(0.828, 3);
			});

	it("remembers the domain", function() {
		var n = 10;
		var sq = new SquareDomain(0, 1, n);
		var v = new ValueFunction(sq);

		expect(v.getDomain()).toBe(sq);
	});

	it("equals himself no domain", function() {
		var sq = new SquareDomain(0, 1, 10);
		var vfunc1 = new ValueFunction(sq);

		expect(vfunc1.equals(vfunc1)).toBe(true);
	});

	it("equals himself", function() {
		var vfunc1 = new ValueFunction(null);

		expect(vfunc1.equals(vfunc1)).toBe(true);
	});

	it("empty functions are equal no domain", function() {
		var vfunc1 = new ValueFunction(null);
		var vfunc2 = new ValueFunction(null);

		expect(vfunc1.equals(vfunc2)).toBe(true);
	});

	it("empty functions are not equal", function() {
		var sq = new SquareDomain(0, 1, 10);
		var vfunc1 = new ValueFunction(sq);
		var vfunc2 = new ValueFunction(null);

		expect(vfunc1.equals(vfunc2)).toBe(false);
	});

	it("non empty functions are equal", function() {
		var vfunc1 = new ValueFunction(null);
		vfunc1.setValue(1, 2);
		var vfunc2 = new ValueFunction(null);
		vfunc2.setValue(1, 2);

		expect(vfunc1.equals(vfunc2)).toBe(true);
	});

	it("non empty functions are equal", function() {
		var vfunc1 = new ValueFunction(null);
		vfunc1.setValue(1, 2);
		var vfunc2 = new ValueFunction(null);

		expect(vfunc1.equals(vfunc2)).toBe(false);
	});
});