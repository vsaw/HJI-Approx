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

module("PE2GameSolver", {
	setup : function() {
		// prepare something for all following tests
		g = new PE2GameSolver();
	},
	teardown : function() {
		// clean up after each test
	}
});

var g;

test("null with negative tolerance", function() {
	equal(null, g.computeValueFunction(-1));
});

// test("no capture", function() {
// var sq = new SquareDomain(-2, 2, 2);
//	
// var g = new PE2Game();
// g.controlResolution = 8;
// g.captureRadius = 1e-1;
// g.
//
// var gs = new PE2GameSolver();
// gs.velocityPursuer = 2;
// gs.domain = sq;
// gs.game = g;
//
// var actualValueFunction = gs.computeValueFunction(1e-1);
// });

test("experiment 0 0 8", function() {
	var sq = new SquareDomain(-2, 2, 5);
	var expectedArray = [ 0, 0, 0.548982, 0.758593, 0.828536, 0, 0, 0.621726,
			0.817027, 0.877604, 0.662549, 0.666853, 0.748748, 0.881505,
			0.930589, 0.871548, 0.886901, 0.912161, 0.943614, 0.964081,
			0.934109, 0.95462, 0.967783, 0.972028, 0.972572, 0, 0, 0, 0.512795,
			0.671657, 0, 0, 0, 0.598567, 0.746888, 0.570931, 0.577638,
			0.630711, 0.732895, 0.842416, 0.813235, 0.837222, 0.871092,
			0.899737, 0.921564, 0.895358, 0.92682, 0.944266, 0.948774,
			0.948886, 0.393469, 0, 0, 0, 0.393469, 0.522364, 0, 0, 0, 0.522364,
			0.700087, 0.613456, 0.577638, 0.613456, 0.700087, 0.873785,
			0.852343, 0.832992, 0.852343, 0.873785, 0.917592, 0.917592,
			0.910886, 0.917592, 0.917592, 0.671657, 0.512795, 0, 0, 0,
			0.746888, 0.598567, 0, 0, 0, 0.842416, 0.732895, 0.630711,
			0.577638, 0.570931, 0.921564, 0.899737, 0.871092, 0.837222,
			0.813235, 0.948886, 0.948774, 0.944266, 0.92682, 0.895358,
			0.828536, 0.758593, 0.548982, 0, 0, 0.877604, 0.817027, 0.621726,
			0, 0, 0.930589, 0.881505, 0.748748, 0.666853, 0.662549, 0.964081,
			0.943614, 0.912161, 0.886901, 0.871548, 0.972572, 0.972028,
			0.967783, 0.95462, 0.934109, 0, 0, 0.548982, 0.740683, 0.790739, 0,
			0, 0.548982, 0.762018, 0.836322, 0, 0, 0.626824, 0.826226,
			0.886439, 0.660631, 0.677534, 0.762341, 0.883835, 0.912369,
			0.833549, 0.868214, 0.899457, 0.914079, 0.917004, 0, 0, 0,
			0.512795, 0.632121, 0, 0, 0, 0.512795, 0.675802, 0, 0, 0, 0.604317,
			0.758648, 0.58517, 0.598676, 0.646184, 0.738691, 0.835095,
			0.790376, 0.824307, 0.85143, 0.862171, 0.862857, 0.393469, 0, 0, 0,
			0.393469, 0.393469, 0, 0, 0, 0.393469, 0.524722, 0, 0, 0, 0.524722,
			0.698624, 0.625104, 0.595172, 0.625104, 0.698624, 0.819193,
			0.819193, 0.808034, 0.819193, 0.819193, 0.632121, 0.512795, 0, 0,
			0, 0.675802, 0.512795, 0, 0, 0, 0.758648, 0.604317, 0, 0, 0,
			0.835095, 0.738691, 0.646184, 0.598676, 0.58517, 0.862857,
			0.862171, 0.85143, 0.824307, 0.790376, 0.790739, 0.740683,
			0.548982, 0, 0, 0.836322, 0.762018, 0.548982, 0, 0, 0.886439,
			0.826226, 0.626824, 0, 0, 0.912369, 0.883835, 0.762341, 0.677534,
			0.660631, 0.917004, 0.914079, 0.899457, 0.868214, 0.833549,
			0.393469, 0.516456, 0.663431, 0.791378, 0.819193, 0, 0, 0.591735,
			0.780037, 0.821041, 0, 0, 0.548982, 0.770922, 0.845649, 0, 0,
			0.646184, 0.828251, 0.861952, 0.690401, 0.737988, 0.803467,
			0.848378, 0.862857, 0.393469, 0.393469, 0.511154, 0.623345,
			0.695297, 0, 0, 0, 0.56996, 0.695297, 0, 0, 0, 0.512795, 0.690257,
			0, 0, 0, 0.625104, 0.757364, 0.632121, 0.668667, 0.725501, 0.76014,
			0.768896, 0.519369, 0.493996, 0.393469, 0.493996, 0.519369,
			0.493996, 0, 0, 0, 0.493996, 0.393469, 0, 0, 0, 0.393469, 0.493996,
			0, 0, 0, 0.493996, 0.519369, 0.493996, 0.393469, 0.493996,
			0.519369, 0.768896, 0.76014, 0.725501, 0.668667, 0.632121,
			0.757364, 0.625104, 0, 0, 0, 0.690257, 0.512795, 0, 0, 0, 0.695297,
			0.56996, 0, 0, 0, 0.695297, 0.623345, 0.511154, 0.393469, 0.393469,
			0.862857, 0.848378, 0.803467, 0.737988, 0.690401, 0.861952,
			0.828251, 0.646184, 0, 0, 0.845649, 0.770922, 0.548982, 0, 0,
			0.821041, 0.780037, 0.591735, 0, 0, 0.819193, 0.791378, 0.663431,
			0.516456, 0.393469, 0.833549, 0.868214, 0.899457, 0.914079,
			0.917004, 0.660631, 0.677534, 0.762341, 0.883835, 0.912369, 0, 0,
			0.626824, 0.826226, 0.886439, 0, 0, 0.548982, 0.762018, 0.836322,
			0, 0, 0.548982, 0.740683, 0.790739, 0.790376, 0.824307, 0.85143,
			0.862171, 0.862857, 0.58517, 0.598676, 0.646184, 0.738691,
			0.835095, 0, 0, 0, 0.604317, 0.758648, 0, 0, 0, 0.512795, 0.675802,
			0, 0, 0, 0.512795, 0.632121, 0.819193, 0.819193, 0.808034,
			0.819193, 0.819193, 0.698624, 0.625104, 0.595172, 0.625104,
			0.698624, 0.524722, 0, 0, 0, 0.524722, 0.393469, 0, 0, 0, 0.393469,
			0.393469, 0, 0, 0, 0.393469, 0.862857, 0.862171, 0.85143, 0.824307,
			0.790376, 0.835095, 0.738691, 0.646184, 0.598676, 0.58517,
			0.758648, 0.604317, 0, 0, 0, 0.675802, 0.512795, 0, 0, 0, 0.632121,
			0.512795, 0, 0, 0, 0.917004, 0.914079, 0.899457, 0.868214,
			0.833549, 0.912369, 0.883835, 0.762341, 0.677534, 0.660631,
			0.886439, 0.826226, 0.626824, 0, 0, 0.836322, 0.762018, 0.548982,
			0, 0, 0.790739, 0.740683, 0.548982, 0, 0, 0.934109, 0.95462,
			0.967783, 0.972028, 0.972572, 0.871548, 0.886901, 0.912161,
			0.943614, 0.964081, 0.662549, 0.666853, 0.748748, 0.881505,
			0.930589, 0, 0, 0.621726, 0.817027, 0.877604, 0, 0, 0.548982,
			0.758593, 0.828536, 0.895358, 0.92682, 0.944266, 0.948774,
			0.948886, 0.813235, 0.837222, 0.871092, 0.899737, 0.921564,
			0.570931, 0.577638, 0.630711, 0.732895, 0.842416, 0, 0, 0,
			0.598567, 0.746888, 0, 0, 0, 0.512795, 0.671657, 0.917592,
			0.917592, 0.910886, 0.917592, 0.917592, 0.873785, 0.852343,
			0.832992, 0.852343, 0.873785, 0.700087, 0.613456, 0.577638,
			0.613456, 0.700087, 0.522364, 0, 0, 0, 0.522364, 0.393469, 0, 0, 0,
			0.393469, 0.948886, 0.948774, 0.944266, 0.92682, 0.895358,
			0.921564, 0.899737, 0.871092, 0.837222, 0.813235, 0.842416,
			0.732895, 0.630711, 0.577638, 0.570931, 0.746888, 0.598567, 0, 0,
			0, 0.671657, 0.512795, 0, 0, 0, 0.972572, 0.972028, 0.967783,
			0.95462, 0.934109, 0.964081, 0.943614, 0.912161, 0.886901,
			0.871548, 0.930589, 0.881505, 0.748748, 0.666853, 0.662549,
			0.877604, 0.817027, 0.621726, 0, 0, 0.828536, 0.758593, 0.548982,
			0, 0 ];
	var expectedValueFuncion = new ValueFunction(sq);
	expectedValueFuncion.values = expectedArray;

	var g = new PE2Game();
	g.controlResolution = 8;
	g.captureRadius = 1e-1;

	var gs = new PE2GameSolver();
	gs.velocityPursuer = 2;
	gs.domain = sq;
	gs.game = g;

	var actualValueFunction = gs.computeValueFunction(1e-1);

	// ok(expectedValueFuncion.equals(actualValueFunction));
	for ( var i = 0; i < expectedArray.length; i++) {
		equal(actualValueFunction.values[i], expectedArray[i], "" + i);
	}
});

delete g;