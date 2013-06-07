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

var g;

module("PE2Game", {
	setup : function() {
		// prepare something for all following tests
		g = new PE2Game();
	},
	teardown : function() {
		// clean up after each test
	}
});

test("capture", function() {
	g.captureRadius = -1;
	ok(!g.isTerminal([ 0, 0, 0, 0 ]));
	
	g.captureRadius = 0;
	ok(g.isTerminal([ 0, 0, 0, 0 ]));

	g.captureRadius = 1;
	ok(g.isTerminal([ 0, 0, 0, 0 ]));
	ok(g.isTerminal([ 1, 0, 0, 0 ]));
	ok(g.isTerminal([ 1, 0, 0, 0 ]));
	ok(g.isTerminal([ 0, 0, 1, 1 ]));

	// No capture
	ok(!g.isTerminal([ 1.1, 0, 0, 0 ]));
	ok(!g.isTerminal([ 1.1, 1, 0, 0 ]));

	// Garbage data
	ok(!g.isTerminal(null));
	ok(!g.isTerminal([ 1.1, 1, 0, 0, 0 ]));
	ok(!g.isTerminal([ 1.1, 1, 0 ]));
});

delete g;