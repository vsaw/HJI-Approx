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
var GameSolver = function() {

	/**
	 * Dampening factor computed from the time step size in order to make our
	 * fixed point iteration a contraction which will converge because of the
	 * banach fixed point theorem.
	 */
	this.beta;
	/**
	 * The GameStateDomain for this game.
	 * 
	 * This domain is used in various placed all over the class and defined
	 * where the game takes place.
	 */
	this.domain;
	/**
	 * The Game that describes its dynamics and terminal states.
	 */
	this.game;
	/**
	 * The maximal amount of valid iterations in the computation of the value
	 * function.
	 * 
	 * If computation of the value function does not converge within
	 * maxIterations. The iteration will be stopped and considered unsuccessful.
	 */
	this.maxIterations;
	/**
	 * The maximal amount of valid iterations in the computation of the
	 * trajectories.
	 * 
	 * If computation of the trajectories does not converge within maxTimeSteps,
	 * the value of the game will be considered infinite and catch does not
	 * occur.
	 */
	this.maxTimeSteps;
	/**
	 * The step size.
	 * 
	 * Smaller values will lead to a better approximation of the value function
	 * but also to higher computational costs.
	 * 
	 * Please note that to ensure convergence of the approximation scheme we
	 * need to pick the step size smaller than the diameters of the grid cells.
	 */
	this.timeStepSize;

	/**
	 * This will calculate the value function.
	 * 
	 * @param tol
	 *            The tolerance for the convergence.
	 * 
	 * @returns The value function for this game.
	 * 
	 * This computation will disregard the initial state and compute really the
	 * complete value for all possible states! The returning value function will
	 * be defined at each possible states.
	 * 
	 * Use with care as it will result in massive computational effort.
	 * 
	 * @see ValueFunction
	 */
	this.computeValueFunction = function(tolerance) {
		throw "To be implemented by subclasses.";
	};

	/**
	 * Computes the optimal trajectories for the game at State x.
	 * 
	 * @param vfunc
	 *            The value function of the game.
	 * @param x
	 *            The state to plot the trajectories.
	 * 
	 * @returns An array with all the states that lead to the end of the Game or
	 *          the state that can be reached in the respective amount of steps
	 *          if no terminal state could be reached.
	 */
	this.computeTrajectories = function(valueFunction, state) {
		throw "To be implemented by subclasses.";
	};
};
