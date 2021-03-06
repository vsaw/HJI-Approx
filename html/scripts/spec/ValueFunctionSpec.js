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

	createNonTrivialValueFunction = function() {
		var values = [ 5, 5, 5, 5, 0, 0, 0.548982, 0.758593, 0.828536, 0, 0,
				0.621726, 0.817027, 0.877604, 0.662549, 0.666853, 0.748748,
				0.881505, 0.930589, 0.871548, 0.886901, 0.912161, 0.943614,
				0.964081, 0.934109, 0.95462, 0.967783, 0.972028, 0.972572, 0,
				0, 0, 0.512795, 0.671657, 0, 0, 0, 0.598567, 0.746888,
				0.570931, 0.577638, 0.630711, 0.732895, 0.842416, 0.813235,
				0.837222, 0.871092, 0.899737, 0.921564, 0.895358, 0.92682,
				0.944266, 0.948774, 0.948886, 0.393469, 0, 0, 0, 0.393469,
				0.522364, 0, 0, 0, 0.522364, 0.700087, 0.613456, 0.577638,
				0.613456, 0.700087, 0.873785, 0.852343, 0.832992, 0.852343,
				0.873785, 0.917592, 0.917592, 0.910886, 0.917592, 0.917592,
				0.671657, 0.512795, 0, 0, 0, 0.746888, 0.598567, 0, 0, 0,
				0.842416, 0.732895, 0.630711, 0.577638, 0.570931, 0.921564,
				0.899737, 0.871092, 0.837222, 0.813235, 0.948886, 0.948774,
				0.944266, 0.92682, 0.895358, 0.828536, 0.758593, 0.548982, 0,
				0, 0.877604, 0.817027, 0.621726, 0, 0, 0.930589, 0.881505,
				0.748748, 0.666853, 0.662549, 0.964081, 0.943614, 0.912161,
				0.886901, 0.871548, 0.972572, 0.972028, 0.967783, 0.95462,
				0.934109, 0, 0, 0.548982, 0.740683, 0.790739, 0, 0, 0.548982,
				0.762018, 0.836322, 0, 0, 0.626824, 0.826226, 0.886439,
				0.660631, 0.677534, 0.762341, 0.883835, 0.912369, 0.833549,
				0.868214, 0.899457, 0.914079, 0.917004, 0, 0, 0, 0.512795,
				0.632121, 0, 0, 0, 0.512795, 0.675802, 0, 0, 0, 0.604317,
				0.758648, 0.58517, 0.598676, 0.646184, 0.738691, 0.835095,
				0.790376, 0.824307, 0.85143, 0.862171, 0.862857, 0.393469, 0,
				0, 0, 0.393469, 0.393469, 0, 0, 0, 0.393469, 0.524722, 0, 0, 0,
				0.524722, 0.698624, 0.625104, 0.595172, 0.625104, 0.698624,
				0.819193, 0.819193, 0.808034, 0.819193, 0.819193, 0.632121,
				0.512795, 0, 0, 0, 0.675802, 0.512795, 0, 0, 0, 0.758648,
				0.604317, 0, 0, 0, 0.835095, 0.738691, 0.646184, 0.598676,
				0.58517, 0.862857, 0.862171, 0.85143, 0.824307, 0.790376,
				0.790739, 0.740683, 0.548982, 0, 0, 0.836322, 0.762018,
				0.548982, 0, 0, 0.886439, 0.826226, 0.626824, 0, 0, 0.912369,
				0.883835, 0.762341, 0.677534, 0.660631, 0.917004, 0.914079,
				0.899457, 0.868214, 0.833549, 0.393469, 0.516456, 0.663431,
				0.791378, 0.819193, 0, 0, 0.591735, 0.780037, 0.821041, 0, 0,
				0.548982, 0.770922, 0.845649, 0, 0, 0.646184, 0.828251,
				0.861952, 0.690401, 0.737988, 0.803467, 0.848378, 0.862857,
				0.393469, 0.393469, 0.511154, 0.623345, 0.695297, 0, 0, 0,
				0.56996, 0.695297, 0, 0, 0, 0.512795, 0.690257, 0, 0, 0,
				0.625104, 0.757364, 0.632121, 0.668667, 0.725501, 0.76014,
				0.768896, 0.519369, 0.493996, 0.393469, 0.493996, 0.519369,
				0.493996, 0, 0, 0, 0.493996, 0.393469, 0, 0, 0, 0.393469,
				0.493996, 0, 0, 0, 0.493996, 0.519369, 0.493996, 0.393469,
				0.493996, 0.519369, 0.768896, 0.76014, 0.725501, 0.668667,
				0.632121, 0.757364, 0.625104, 0, 0, 0, 0.690257, 0.512795, 0,
				0, 0, 0.695297, 0.56996, 0, 0, 0, 0.695297, 0.623345, 0.511154,
				0.393469, 0.393469, 0.862857, 0.848378, 0.803467, 0.737988,
				0.690401, 0.861952, 0.828251, 0.646184, 0, 0, 0.845649,
				0.770922, 0.548982, 0, 0, 0.821041, 0.780037, 0.591735, 0, 0,
				0.819193, 0.791378, 0.663431, 0.516456, 0.393469, 0.833549,
				0.868214, 0.899457, 0.914079, 0.917004, 0.660631, 0.677534,
				0.762341, 0.883835, 0.912369, 0, 0, 0.626824, 0.826226,
				0.886439, 0, 0, 0.548982, 0.762018, 0.836322, 0, 0, 0.548982,
				0.740683, 0.790739, 0.790376, 0.824307, 0.85143, 0.862171,
				0.862857, 0.58517, 0.598676, 0.646184, 0.738691, 0.835095, 0,
				0, 0, 0.604317, 0.758648, 0, 0, 0, 0.512795, 0.675802, 0, 0, 0,
				0.512795, 0.632121, 0.819193, 0.819193, 0.808034, 0.819193,
				0.819193, 0.698624, 0.625104, 0.595172, 0.625104, 0.698624,
				0.524722, 0, 0, 0, 0.524722, 0.393469, 0, 0, 0, 0.393469,
				0.393469, 0, 0, 0, 0.393469, 0.862857, 0.862171, 0.85143,
				0.824307, 0.790376, 0.835095, 0.738691, 0.646184, 0.598676,
				0.58517, 0.758648, 0.604317, 0, 0, 0, 0.675802, 0.512795, 0, 0,
				0, 0.632121, 0.512795, 0, 0, 0, 0.917004, 0.914079, 0.899457,
				0.868214, 0.833549, 0.912369, 0.883835, 0.762341, 0.677534,
				0.660631, 0.886439, 0.826226, 0.626824, 0, 0, 0.836322,
				0.762018, 0.548982, 0, 0, 0.790739, 0.740683, 0.548982, 0, 0,
				0.934109, 0.95462, 0.967783, 0.972028, 0.972572, 0.871548,
				0.886901, 0.912161, 0.943614, 0.964081, 0.662549, 0.666853,
				0.748748, 0.881505, 0.930589, 0, 0, 0.621726, 0.817027,
				0.877604, 0, 0, 0.548982, 0.758593, 0.828536, 0.895358,
				0.92682, 0.944266, 0.948774, 0.948886, 0.813235, 0.837222,
				0.871092, 0.899737, 0.921564, 0.570931, 0.577638, 0.630711,
				0.732895, 0.842416, 0, 0, 0, 0.598567, 0.746888, 0, 0, 0,
				0.512795, 0.671657, 0.917592, 0.917592, 0.910886, 0.917592,
				0.917592, 0.873785, 0.852343, 0.832992, 0.852343, 0.873785,
				0.700087, 0.613456, 0.577638, 0.613456, 0.700087, 0.522364, 0,
				0, 0, 0.522364, 0.393469, 0, 0, 0, 0.393469, 0.948886,
				0.948774, 0.944266, 0.92682, 0.895358, 0.921564, 0.899737,
				0.871092, 0.837222, 0.813235, 0.842416, 0.732895, 0.630711,
				0.577638, 0.570931, 0.746888, 0.598567, 0, 0, 0, 0.671657,
				0.512795, 0, 0, 0, 0.972572, 0.972028, 0.967783, 0.95462,
				0.934109, 0.964081, 0.943614, 0.912161, 0.886901, 0.871548,
				0.930589, 0.881505, 0.748748, 0.666853, 0.662549, 0.877604,
				0.817027, 0.621726, 0, 0, 0.828536, 0.758593, 0.548982, 0, 0 ];
		var discretizationSteps = 5;
		var sq = new SquareDomain(-2, 2, discretizationSteps);
		var vfunc008 = new ValueFunction(sq);
		// The first 4 values describe the discretization of the domain
		// in each dimension so skip them!
		for (var i = 4; i < values.length; i++) {
			/*
			 * Index is a gridpoint index so the array can not be filled
			 * directly but we have to convert the arry index to the gridpoint
			 * index of the domain.
			 */
			var gridIndex = [];
			for (var j = 0; j < 4; j++) {
				gridIndex[j] = Math.floor((i - 4) / discretizationSteps ^ j)
						% discretizationSteps;
			}
			vfunc008.setValue(gridIndex, values[i]);
		}
		return vfunc008;
	};

	it("equals to self non trival", function() {
		var vfunc008 = createNonTrivialValueFunction();
		expect(vfunc008.equals(vfunc008)).toBe(true);
	});

	it("self is close to self", function() {
		var vfunc = createNonTrivialValueFunction();
		expect(vfunc.isClose(vfunc, 0, 1)).toBe(true);
	});

	it("self is close to altered self", function() {
		var vfunc = createNonTrivialValueFunction();
		var vfuncAltered = createNonTrivialValueFunction();
		// Change the value function at an arbitrary index
		var index = [ 1, 2, 3, 4 ];
		vfuncAltered.setValue(index, vfunc.getValue(index) + 0.0001);
		expect(vfunc.isClose(vfuncAltered, 0.1)).toBe(true);
	});

	it("close detects changes properly!", function() {
		var vfunc = createNonTrivialValueFunction();
		var vfuncAltered = createNonTrivialValueFunction();
		expect(vfunc.getDomain().equals(vfuncAltered.getDomain())).toBe(true);
		// Change the value function at an arbitrary index
		var index = [ 1, 2, 3, 4 ];
		vfuncAltered.setValue(index, vfunc.getValue(index) + 0.1);
		expect(vfunc.isClose(vfuncAltered, 0.01)).toBe(false);
	});
});
