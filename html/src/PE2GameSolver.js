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
var PE2GameSolver = function() {

	/**
	 * The velocity of the evader.
	 * 
	 * Default value is 1
	 */
	this.velocityEvader = 1;
	/**
	 * The velocity of the pursuer.
	 * 
	 * Default value is 1
	 */
	this.velocityPursuer = 1;
	this.maxIterations = 150;

	this.computeValueFunction = function(tolerance) {
		if (tolerance <= 0) {
			return null;
		}

		var changed = true;
		var iterationCounter = 0;

		/*
		 * In this implementation we have two value functions. The reason being,
		 * if I only have one, I immediately feed back the new values into the
		 * same iteration loop. I believe this breaks the fixed point iteration.
		 */
		var vfunc = new ValueFunction(this.domain);
		var vfuncNext = new ValueFunction(this.domain);
		var i = 0;
		var maxIndex = this.domain.getMaximalGridIndex(4);

		while (changed && iterationCounter < this.maxIterations) {
			iterationCounter++;

			// Loop through the grid points of the value function as mentioned
			// on page 195 of the paper!
			for (i = 0; i <= maxIndex[0] / 2; i++) {
				for ( var j = 0; j <= maxIndex[1] / 2; j++) {
					for ( var k = 0; k <= maxIndex[2]; k++) {
						for ( var l = 0; l <= maxIndex[3]; l++) {

							// If the index is not feasible for the given
							// domain, skip the execution of the loop.
							if (!this.domain.isFeasibleIndex([ i, j, k, l ])) {
								continue;
							}

							var oldValue = vfunc.getValue([ i, j, k, l ]);

							// Before we compute the new Value check if the
							// state is terminal, as this saves us a lot of
							// trouble and computing power.
							var newValue = 0;
							if (!this.game.isTerminal([ i, j, k, l ])) {
								newValue = this.computeValueOnGrid(vfunc, [ i,
										j, k, l ]);
							}

							// Check if the value of the function at this
							// gridindex changed above tol. If it did not, we
							// can leave the outer while-loop.
							if (!changed) {
								changed = Math.abs(oldValue - newValue) > tolerance;
							}

							vfuncNext.setValue([ i, j, k, l ], newValue);

							// XXX: This Symmetry does only work in the standard
							// square domain.
							// TODO: Symmetry in ValueFunction There must be
							// some kind of way to figure out how to push the
							// symmetry to the value function!
							vfuncNext.setValue([ i, maxIndex[1] - j, k,
									maxIndex[3] - l ], newValue);

						}
					}
				}
			}

			// XXX: This does only work in the standard square domain.
			// Copy the last line as suggested by Cristiane and Falcone.
			// Remember that we stepped out of the i-loop therefore i has the
			// value
			// maxIndex[0] / 2 + 1
			for ( var j = 0; j <= maxIndex[1]; j++) {
				for ( var k = 0; k <= maxIndex[2]; k++) {
					for ( var l = 0; l <= maxIndex[3]; l++) {
						// If the index is not feasible for the given domain,
						// skip the execution of the loop.
						if (!this.domain.isFeasibleIndex([ i - 1, j, k, l ])) {
							continue;
						}

						var oldVal = vfuncNext.getValue([ i - 1, j, k, l ]);
						vfuncNext.setValue([ i, maxIndex[1] - j,
								maxIndex[2] - k, maxIndex[3] - l ], oldVal);
					}
				}
			}

			/*
			 * Change the value functions as we finished our iteration.
			 * 
			 * So in the next iteration the updated values will be our baseline
			 * and the second function will be updated accordingly.
			 */
			var vfuncTemp = vfunc;
			vfunc = vfuncNext;
			vfuncNext = vfuncTemp;
		}

		// XXX: Copy the rest of the symmetry in the value function!
		for (i = 0; i <= maxIndex[0] / 2; i++) {
			for ( var j = 0; j <= maxIndex[1]; j++) {
				for ( var k = 0; k <= maxIndex[2]; k++) {
					for ( var l = 0; l <= maxIndex[3]; l++) {
						// If the index is not feasible for the given domain,
						// skip the execution of the loop.
						if (!this.domain.isFeasibleIndex([ i, j, k, l ])) {
							continue;
						}

						var oldVal = vfuncNext.getValue([ i, j, k, l ]);
						vfuncNext.setValue([ maxIndex[0] - i, maxIndex[1] - j,
								maxIndex[2] - k, maxIndex[3] - l ], oldVal);
					}
				}
			}
		}

		vfuncNext.iterations = iterationCounter;
		return vfuncNext;
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
	 *            The index of the point to approximate.
	 * 
	 * @returns The value.
	 */
	this.computeValueOnGrid = function(vfunc, index) {
		// The value at point x.
		var value = Number.NEGATIVE_INFINITY;

		// Some variables I extracted from the loop to check if this would
		// reduce the memory footprint.
		var newState = [];
		var blankoState = [];
		var newValue = 0;

		// Loop through all the control options of P
		var startControl = this.game.allowStandingStill ? 0 : 1;
		for ( var evaderControl = startControl; evaderControl <= this.game.controlResolution; evaderControl++) {

			// Check if the current control option is admissible if this
			// control is admissible the pursuer variables will already be
			// stored in newState. As for the evader, newstate will just
			// contain his original position as if he stood still.
			if (this.isAdmissibleControl(index, blankoState, "Evader",
					evaderControl)) {

				// This is the value the evader picks. We did not yet maximize
				// the pursuers controls so this might be different from the
				// actual value.
				var evaderValue = Number.POSITIVE_INFINITY;

				// New check all the options E has
				for ( var pursuerControl = 1; pursuerControl <= this.game.controlResolution; pursuerControl++) {

					// Check if E control is admissible
					// Since the two players can't interfere with each others
					// decisions and we store the valid control changes of
					// Pursuer and evader in newState we have a valid new state
					// if the current control of the Evader is also valid.
					// Therefore it is not necessary anymore to reevaluate the
					// kinematic equations.
					if (this.isAdmissibleControl(blankoState, newState,
							Pursuer, pursuerControl)) {

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
	 * Checks if the given control is admissible for the player at the current
	 * state of the game.
	 * 
	 * If it is feasible the new state after the control change will be stored
	 * in the newState variable.
	 * 
	 * @param index
	 *            The current state
	 * @param newState
	 *            The new state after the player has made a move.
	 * @param player
	 *            The player who wants to move.
	 * @param control
	 *            The direction he wants to move to.
	 * 
	 * @returns True if it is a valid move; otherwise false.
	 * 
	 * @see GameStateDomain::isFeasible
	 */
	this.isAdmissibleControl = function(index, newState, player, control) {
		if (player == "Pursuer") {
			newState = this.game.evalKE(index, [ control, 0 ],
					this.timeStepSize);
		} else {
			newState = this.game.evalKE(index, [ 0, control ],
					this.timeStepSize);
		}
		return this.domain.isFeasible(newState, player);
	};
};

PE2GameSolver.prototype = GameSolver;
