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
 * Game.js
 *
 *  Created on: 2013-09-13
 *      Author: Valentin Sawadski <valentin@sawadski.eu>
 */

var Definitions = require('./Definitions.js');
var ValueFunction = require('./ValueFunction.js');

/**
 * The game class.
 * 
 * This class is used to set various parameters of the game, like the velocity
 * of the players, and make calculation of the value of the game.
 */
var Game = function() {

	/**
	 * If true, players are allowed to stand still. Otherwise they have to make
	 * a move.
	 * 
	 * By default players are allowed to stand still at one position.
	 */
	var allowStandingStill = true;
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
	var controlResolution = 50;
	/**
	 * The domain this game takes place.
	 */
	var domain = undefined;
	/**
	 * The maximal amount of valid iterations in the computation of the value
	 * function.
	 * 
	 * If computation of the value function does not converge within
	 * maxIterations. The iteration will be stopped and considered unsuccessful.
	 */
	var maxIterations = Definitions.MAX_ITERATIONS;
	/**
	 * The step size.
	 * 
	 * Smaller values will lead to a better approximation of the value function
	 * but also to higher computational costs.
	 * 
	 * Please note that to ensure convergence of the approximation scheme we
	 * need to pick the step size smaller than the diameters of the grid cells.
	 */
	var timeStepSize;
	/**
	 * The velocity of the evader.
	 * 
	 * Default value is 1
	 */
	var velocityE = 1;
	/**
	 * The velocity of the pursuer.
	 * 
	 * Default value is 1
	 */
	var velocityP = 1;

	/**
	 * This will calculate the value function.
	 * 
	 * This computation will disregard the initial state and compute really the
	 * complete value for all possible states! The returning value function will
	 * be defined at each possible states.
	 * 
	 * Use with care as it will result in massive computational effort.
	 * 
	 * @param tol
	 *            The tolerance for the convergence.
	 * 
	 * @returns The value function for this game.
	 * 
	 * @see ValueFunction
	 */
	this.computeValueFunction = function(tol) {
		if (tol <= 0) {
			return null;
		}

		var changed = true;
		var iterationCounter = 0;

		/*
		 * In this implementation we have two value functions. The reason being,
		 * if I only have one, I immediately feed back the new values into the
		 * same iteration loop. I believe this breaks the fixed point iteration.
		 */
		var vfunc = new ValueFunction(domain);
		var vfuncNext = new ValueFunction(domain);
		var vfuncTemp;
		var maxIndex = domain.getMaximalGridIndex();
		var i = 0;
		var index;

		while (changed && iterationCounter < maxIterations) {
			iterationCounter++;
			changed = false;

			// Loop through the grid points of the value function as mentioned
			// on page 195 of the paper!
			for (i = 0; i <= maxIndex[0] / 2; i++) {
				for (var j = 0; j <= maxIndex[1] / 2; j++) {
					for (var k = 0; k <= maxIndex[2]; k++) {
						for (var l = 0; l <= maxIndex[3]; l++) {

							// TODO: Speed up!
							// I have the feeling I can speed things up here if
							// I handle the terminal states more precisely!
							// but I have more important things to fix right
							// now!
							index = [ i, j, k, l ];

							// If the index is not feasible for the given
							// domain, skip the execution of the loop.
							if (!domain.isFeasibleIndex(index)) {
								continue;
							}

							if (vfunc.getValue(index) == undefined) {
								vfunc.setValue(index,
										Definitions.VALUE_FUNCTION_INITIAL);
							}
							var oldValue = vfunc.getValue(index);

							// Before we compute the new Value check if the
							// state is terminal, as this saves us a lot of
							// trouble and computing power.
							var newValue = 0;
							if (!this.isTerminalIndex(index)) {
								newValue = this.computeValueOnGridIndex(vfunc,
										index);
							}

							// Check if the value of the function at this
							// gridindex changed above tol. If it did not, we
							// can leave the outer while-loop.
							if (!changed) {
								changed = Math.abs(oldValue - newValue) > tol;
							}

							vfuncNext.setValue(index, newValue);

							// XXX: This Symmetry does only work in the standard
							// square domain.
							// TODO: Symmetry in ValueFunction
							// There must be some kind of way to figure out how
							// to push the symmetry to the value function!
							index = [ i, maxIndex[1] - j, k, maxIndex[3] - l ];
							vfuncNext.setValue(index, newValue);
						}
					}
				}
			}

			// XXX: This does only work in the standard square domain.
			// Copy the last line as suggested by Cristiane and Falcone.
			// Remember that we stepped out of the i-loop therefore i has the
			// value maxIndex[0] / 2 + 1
			for (var j = 0; j <= maxIndex[1]; j++) {
				for (var k = 0; k <= maxIndex[2]; k++) {
					for (var l = 0; l <= maxIndex[3]; l++) {
						index = [ i - 1, j, k, l ];

						// If the index is not feasible for the given domain,
						// skip the execution of the loop.
						if (!domain.isFeasible(index)) {
							continue;
						}

						var oldVal = vfuncNext.getValue(index);

						index = [ i, maxIndex[1] - j, maxIndex[2] - k,
								maxIndex[3] - l ];

						vfuncNext.setValue(index, oldVal);
					}
				}
			}

			/*
			 * Change the value functions as we finished our iteration.
			 * 
			 * So in the next iteration the updated values will be our baseline
			 * and the second function will be updated accordingly.
			 */
			vfuncTemp = vfunc;
			vfunc = vfuncNext;
			vfuncNext = vfuncTemp;
		}

		// XXX: Copy the rest of the symmetry in the value function!
		for (i = 0; i <= maxIndex[0] / 2; i++) {
			for (var j = 0; j <= maxIndex[1]; j++) {
				for (var k = 0; k <= maxIndex[2]; k++) {
					for (var l = 0; l <= maxIndex[3]; l++) {
						index = [ i, j, k, l ];

						// If the index is not feasible for the given domain,
						// skip the execution of the loop.
						if (!domain.isFeasible(index)) {
							continue;
						}

						var oldVal = vfuncNext.getValue(index);

						index = [ maxIndex[0] - i, maxIndex[1] - j,
								maxIndex[2] - k, maxIndex[3] - l ];
						vfuncNext.setValue(index, oldVal);
					}
				}
			}
		}

		vfuncNext.setIterations(iterationCounter);
		return vfuncNext;
	};

	/**
	 * Computes the value of the ValueFunction at given State which is located
	 * on the grid of the GameStateDomain.
	 * 
	 * This value is used for the internal calculation of the value function.
	 * Basically it corresponds to the first line of fully discrete
	 * approximation scheme for the constrained case by Cristiane & Falcone on
	 * Page 182 of the paper.
	 * 
	 * @param vfunc
	 *            The value function to approximate.
	 * @param x
	 *            The point to approximate
	 * 
	 * @returns The value.
	 */
	this.computeValueOnGrid = function(vfunc, x) {
		// The value at point x.
		var value = -Infinity;

		// Some variables I extracted from the loop to check if this would
		// reduce the memory footprint.
		var newState = [];
		var blankoState = [];
		var newValue = 0;

		// Loop through all the control options of P
		for (var evaderControl = allowStandingStill ? 0 : 1; evaderControl <= controlResolution; evaderControl++) {

			// Check if the current control option is admissible
			// if this control is admissible the pursuer variables will already
			// be stored in newState. As for the evader, newstate will just
			// contain his original position as if he stood still.
			if (this.isAdmissibleControl(x, blankoState, Game.Evader,
					evaderControl)) {

				// This is the value the evader picks. We did not yet maximize
				// the pursuers controls so this might be different from the
				// actual value.
				var evaderValue = Infinity;

				// New check all the options E has
				for (var pursuerControl = 1; pursuerControl <= controlResolution; pursuerControl++) {

					// Check if E control is admissible
					// Since the two players can't interfere with each others
					// decisions and we store the valid control changes of
					// Pursuer and evader in newState we have a valid new state
					// if the current control of the Evader is also valid.
					// Therefore it is not necessary anymore to reevaluate the
					// kinematic equations.
					if (this.isAdmissibleControl(blankoState, newState,
							Game.Pursuer, pursuerControl)) {

						// Evaluate the value of the value function according to
						// the first line of the approximation scheme.
						newValue = beta * vfunc.eval(newState) + 1 - beta;

						// ve = min { ve, computeValueOnGrid( state after
						// control change ) )
						// if ve has a new value make set ce's control to the
						// current
						if (evaderValue > newValue) {
							evaderValue = newValue;
						}
					}
				}

				// After Checking every option of the evader set the value
				// v = max { v, ve }
				// If the value has changed set the control of pursuer.
				if (value < evaderValue) {
					value = evaderValue;
				}
			}
		}

		return value;
	};

	/**
	 * Computes the value of the ValueFunction at given State which is located
	 * on the grid of the GameStateDomain.
	 * 
	 * This value is used for the internal calculation of the value function.
	 * Basically it corresponds to the first line of fully discrete
	 * approximation scheme for the constrained case by Cristiane & Falcone on
	 * Page 182 of the paper.
	 * 
	 * @param vfunc
	 *            The value function to approximate.
	 * @param index
	 *            The index of the point to approximate.
	 * 
	 * @returns The value.
	 */
	this.computeValueOnGridIndex = function(vfunc, index) {
		return this.computeValueOnGrid(vfunc, domain.getPoint(index));
	};

	/**
	 * Evaluate the kinematic equation
	 * 
	 * This computes the new state of the game as it evolves from the given
	 * state and the the given controls of the players. It is not being returned
	 * but stored in an input parameter for less memory consumption
	 * 
	 * @param x
	 *            The current state of the game.
	 * @param newState
	 *            The variable to stor the new state in.
	 * @param p
	 *            The control of the pursuer.
	 * @param e
	 *            The control of the evader.
	 * @param timeStepSize
	 *            The size of the timestep.
	 */
	this.evalKE = function(x, newState, p, e, step) {
		var res = controlResolution;
		var cont, dir;

		if (p != 0) {
			cont = p;
			dir = 2 * Math.PI * (cont / res);
			newState[0] = x[0] + step * velocityP * Math.cos(dir);
			newState[1] = x[1] + step * velocityP * Math.sin(dir);
		} else {
			newState[0] = x[0];
			newState[1] = x[1];
		}

		if (e != 0) {
			cont = e;
			dir = 2 * Math.PI * (cont / res);
			newState[2] = x[2] + step * velocityE * Math.cos(dir);
			newState[3] = x[3] + step * velocityE * Math.sin(dir);
		} else {
			newState[2] = x[2];
			newState[3] = x[3];
		}
	};

	/**
	 * Returns true if players are allowed to stand still.
	 * 
	 * If not standing still is not a valid move and the will have to move. In
	 * Cristiane & Falcones Paper this corresponds to the "+ 1" move option.
	 * 
	 * @returns True if players are allowed to stand still; false otherwise.
	 * 
	 * @see Game::getControlResolution()
	 * @see Game:.setControlResolution( unsigned int n )
	 */
	this.getAllowStandingStill = function() {
		return allowStandingStill;
	};

	/**
	 * Return the control resolution of the game.
	 * 
	 * Basically this variable describes the amount of discretizations we will
	 * make on the unit circle.
	 * 
	 * A higher value will allow the players to adjust their trajectory more
	 * precisely but will also result in a higher computational effort.
	 * 
	 * @returns The amount of discretizations we make on the unit circle.
	 * 
	 * @see Game::getAllowStandingStill()
	 * @see Game::setAllowStandingStill( bool b )
	 * @see Game::setControlResolution( unsigned int n )
	 */
	this.getControlResolution = function() {
		return controlResolution;
	};

	/**
	 * Returns the GameStateDomain for this game.
	 * 
	 * @returns GameStateDomain
	 * 
	 * @see GameStateDomain
	 */
	this.getDomain = function() {
		return domain;
	};

	/**
	 * Returns the maximal amount of valid iterations in the computation of the
	 * value function.
	 * 
	 * If computation of the value function does not converge within
	 * maxIterations. The iteration will be stopped and considered unsuccessful.
	 * 
	 * @returns The maximal amount of valid iterations.
	 */
	this.getMaxIterations = function() {
		return maxIterations;
	}

	/**
	 * Returns the step size for this game.
	 * 
	 * Smaller values will lead to a better approximation of the value function
	 * but also to higher computational costs.
	 * 
	 * Please not that to ensure convergence of the approximation scheme we need
	 * to pick the step size smaller than the diameters of the grid cells.
	 * 
	 * @returns The current step size.
	 */
	this.getTimeStepSize = function() {
		return timeStepSize;
	};

	/**
	 * Get the velocity for player P.
	 * 
	 * @param p
	 *            The player.
	 * 
	 * @return The velocity of the player.
	 */
	this.getVelocity = function(p) {
		if (p === Game.Evader) {
			return velocityE;
		} else if (p === Game.Pursuer) {
			return velocityP;
		}
	}

	/**
	 * Determines if a given Point is inside the terminal set.
	 * 
	 * A state is determined terminal if it the two players are placed on
	 * adjacent grid cells. Adjacency is determined by the index of the grid
	 * cells. If it differs by at max 1 then the state shall be regarded as
	 * terminal.
	 * 
	 * @param index
	 *            The index of the state.
	 * 
	 * @returns True if the state is terminal, false otherwise.
	 */
	this.isTerminalIndex = function(index) {
		return (Math.abs(index[0] - index[2]) <= 1)
				&& (Math.abs(index[1] - index[3]) <= 1);
	};

	/**
	 * Checks if the given control is admissible for the player at the current
	 * state of the game.
	 * 
	 * If it is feasible the new state after the control change will be stored
	 * in the newState variable.
	 * 
	 * @param x
	 *            The current state
	 * @param newState
	 *            The new state after the player has made a move.
	 * @param p
	 *            The player who wants to move.
	 * @param c
	 *            The direction he wants to move to.
	 * 
	 * @returns True if it is a valid move; otherwise false.
	 * 
	 * @see GameStateDomain::isFeasible
	 */
	this.isAdmissibleControl = function(x, newState, p, c) {
		if (p === Game.Pursuer) {
			this.evalKE(x, newState, c, 0, timeStepSize);
		} else {
			this.evalKE(x, newState, 0, c, timeStepSize);
		}

		return domain.isFeasible(newState, p);
	};

	/**
	 * Set if players are allowed to stand still during the game.
	 * 
	 * This corresponds to Cristiane & Falcones "+ 1" direction.
	 * 
	 * @param b
	 *            The value if they are allowed or not.
	 * 
	 * @see Game::getAllowStandingStill()
	 * @see Game::getControlResolution()
	 * @see Game::setControlResolution( unsigned int n )
	 */
	this.setAllowStandingStill = function(b) {
		allowStandingStill = b;
	};

	/**
	 * Set the resolution of the control options.
	 * 
	 * Setting this to a higher value will allow the players to adjust their
	 * trajectory more precisely but will also result in a higher computational
	 * effort. Therefore don't set this value much higher than 50.
	 * 
	 * Basically this variable describes the amount of discretizations we will
	 * make on the unit circle.
	 * 
	 * @param n
	 *            The new resolution.
	 * 
	 * @see Game::getAllowStandingStill()
	 * @see Game::setAllowStandingStill( bool b )
	 * @see Game::getControlResolution()
	 */
	this.setControlResolution = function(res) {
		controlResolution = res;
	};

	/**
	 * Sets the GameStateDomain for this game.
	 * 
	 * This will also automatically set the time step size to maxResolutionWidth /
	 * 2 of this domain. However the time step size can still be adjusted
	 * individually afterwards.
	 * 
	 * @param d
	 *            A pointer to the new domain.
	 * 
	 * @see GameStateDomain
	 * @see GameStateDomain::getMaximalResolutionWidth()
	 * @see Game::setTimeStepSize( step )
	 */
	this.setDomain = function(d) {
		domain = d;
	};

	/**
	 * Sets the maximal amount of valid iterations for the computation of the
	 * value function.
	 * 
	 * If computation of the value function does not converge within
	 * maxIterations. The iteration will be stopped and considered unsuccessful.
	 * 
	 * @param n
	 *            The new amount of maximal iterations.
	 */
	this.setMaxIterations = function(n) {
		maxIterations = n;
	};

	/**
	 * Sets the step size of the game to a new value.
	 * 
	 * Smaller values will lead to a better approximation of the value function
	 * but also to higher computational costs.
	 * 
	 * Please not that to ensure convergence of the approximation scheme we need
	 * to pick the step size smaller than the diameters of the grid cells.
	 * 
	 * This function will do nothing if the new step size is less than or equal
	 * to 0.
	 * 
	 * @param s
	 *            The new step size. Must be greater than 0.
	 */
	this.setTimeStepSize = function(s) {
		if (s > 0) {
			timeStepSize = s;
			beta = Math.exp(-timeStepSize);
		}
	};

	/**
	 * Set the velocity of the player.
	 * 
	 * This function will do nothing if the new velocity is less than 0.
	 * 
	 * @param p
	 *            The player.
	 * @param v
	 *            The new velocity. Must be greater or equal 0.
	 */
	this.setVelocity = function(p, v) {
		if (p === Game.Evader) {
			velocityE = v;
		} else if (p === Game.Pursuer) {
			velocityP = v;
		}
	};

	this.setTimeStepSize(0.1);
};

/**
 * This value designates a Pursuer.
 */
Game.Pursuer = new Object();
/**
 * This value designates an Evader.
 */
Game.Evader = new Object();

module.exports = Game;
