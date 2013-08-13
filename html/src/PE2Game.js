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
 * Evaluates the game state and calculates how it evolves and when it is
 * finished.
 */
var PE2Game = function() {

	/**
	 * If true, players are allowed to stand still. Otherwise they have to make
	 * a move.
	 * 
	 * By default players are allowed to stand still at one position.
	 */
	this.allowStandingStill = true;
	/**
	 * This is the capture radius. If the players are closer than this, capture
	 * occured.
	 */
	this.captureRadius = 1e-3;
	/**
	 * The amount of directions the players can make.
	 * 
	 * Setting this to a higher value will allow the players to adjust their
	 * trajectory more precisely but will also result in a higher computational
	 * effort. Therefore don't set this value much higher than 50.
	 * 
	 * Basically this variable describes the amount of discretizations we will
	 * make on the unit circle.
	 * 
	 * Default value is 50.
	 */
	this.controlResolution = 50;

	this.evaderVelocity;

	this.evalKE = function(state, controls, stepsize) {
		if (stepsize != null && !Array.isArray(stepsize)) {
			var tmp = stepsize;
			stepsize = [ tmp, tmp ];
		}

		var newState = [];

		var dir = 2 * Math.PI * (controls[0] / this.controlResolution);
		newState[0] = state[0] + stepsize[0] * Math.cos(dir);
		newState[1] = state[1] + stepsize[0] * Math.sin(dir);

		dir = 2 * Math.PI * (controls[1] / this.controlResolution);
		newState[2] = state[2] + stepsize[1] * Math.cos(dir);
		newState[3] = state[3] + stepsize[1] * Math.sin(dir);

		return newState;
	};

	this.isTerminal = function(state) {
		var p = state.slice(0, 2);
		var e = state.slice(2, 5);
		return State.equals(p, e, this.captureRadius);
	};
};

PE2Game.prototype = Game;