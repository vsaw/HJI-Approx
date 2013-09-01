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
 * GameStateDomainSpec.js
 *
 *  Created on: 2013-08-27
 *      Author: Valentin Sawadski <valentin@sawadski.eu>
 */

var GameStateDomain = require('../GameStateDomain.js');

describe("GameStateDomain", function() {

	it("can clone points", function() {
		var p1 = [ 0.0, 0.1, 0.2, 0.3 ];
		var clone = GameStateDomain.cloneState(p1);

		expect(clone).not.toBe(p1);
		expect(clone).toEqual(p1);
	});

	it("can determine equal points", function() {
		var p1 = [ 0.0, 0.1, 0.2, 0.3 ];
		var p2 = [ 0.0, 0.1, 0.2, 0.3 ];
		var p3 = [ 0.0, 0.01, 0.25, 0.2 ];
		var p4 = [ 0.0, 0.01, 0.25, 0.19 ];

		expect(GameStateDomain.equals(p1, p1)).toBe(true);
		expect(GameStateDomain.equals(p1, p2)).toBe(true);
		expect(GameStateDomain.equals(p1, p3, 0.1)).toBe(true);
		expect(GameStateDomain.equals(p1, p3)).toBe(false);
		expect(GameStateDomain.equals(p1, p4, 0.1)).toBe(false);
	});
});
