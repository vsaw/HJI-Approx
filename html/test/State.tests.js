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

module("State", {
	setup : function() {
		// prepare something for all following tests
	},
	teardown : function() {
		// clean up after each test
	}
});

test("correct dimensions", function() {
	var x1 = [ 1, 2, 3, 4 ];
	var x2 = [ 1, 2, 3, 4 ];
	ok(State.equals(x1, x2));
	ok(State.equals(x1, x2, 0));
	ok(State.equals(x1, x2, 0.1));
});

test("wrong dimensions", function() {
	var x1 = [ 1, 2, 3 ];
	var x2 = [ 1, 2, 3, 4 ];
	ok(!State.equals(x1, x2));
	ok(!State.equals(x1, x2, 0));
	ok(!State.equals(x1, x2, 0.1));
});

test("correct array", function() {
	var x1 = [ 1, 2, 3, 4.01 ];
	var x2 = [ 1, 2, 3, 4 ];
	ok(State.equals(x1, x2, 0.01));
	ok(State.equals(x1, x2, 0.1));
	ok(!State.equals(x1, x2, 0.001));
});

test("correct 1 dimension", function() {
	var x1 = 1.0;
	var x2 = 1.01;
	ok(State.equals(x1, x2, 0.0101));
	ok(State.equals(x1, x2, 0.1));
	ok(!State.equals(x1, x2, 0.001));
});
