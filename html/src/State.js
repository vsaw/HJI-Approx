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

/**
 * Dummy State Object.
 * 
 * To be injected with functions.
 */
var State = new Function();

/**
 * Determines equality for given points with a tolerance of eps.
 * 
 * @param x1
 *            The first state.
 * @param x2
 *            The second state.
 * @param eps
 *            The maximal distance between x1 and x2. If omitted this will be 0.
 * 
 * @returns True if the distance between x1 and x2 is less then or equal to eps
 *          measured in the infinity norm; false otherwise false.
 * 
 * Points are being regarded different in case their distance in the infinity
 * norm is greater than eps. In other words, if there exists an index i such
 * that | x1[i] - x2[i] | > eps.
 */
State.equals = function(x1, x2, eps) {

	if (arguments.length == 2) {
		eps = 0;
	}

	if (Array.isArray(x1)) {
		if (!Array.isArray(x2) || (x1.length != x2.length)) {
			return false;
		}
		for ( var i = 0; i < x1.length; i++) {
			if (Math.abs(x1[i] - x2[i]) > eps) {
				return false;
			}
		}
		return true;
	} else {
		return Math.abs(x1 - x2) <= eps;
	}
};
