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
 * TODO: Document
 */
function Game() {

	/**
	 * Evalutes the Kinematic equation of a Game that describe how a Game
	 * evolves.
	 *
	 * This will not check if the new state is valid but just return the new
	 * state.
	 *
	 * @param state
	 *            The current state.
	 * @param controls
	 *            The controls of all the players.
	 * @param stepsize
	 *            The amount of time for how long the controls are valid.
	 *
	 * @returns The new state.
	 */
	this.evalKE = function(state, controls, stepsize) {
		throw "To be implemented by subclasses.";
	};

	/**
	 * Determines if a state is terminal
	 *
	 * @param state
	 *            The state to check.
	 *
	 * @returns true if it is a terminal state where the game has ended.
	 */
	this.isTerminal = function(state) {
		throw "To be implemented by subclasses.";
	};
}
