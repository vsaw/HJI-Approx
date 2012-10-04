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
 * Game.cpp
 *
 *  Created on: 2011-08-08
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Game.h"
#include "Debug.h"

namespace bsc {

Game::Game() {
	controlResolution = 50;
	setTimeStepSize(0.1);
	velocityE = 1;
	velocityP = 1;
	maxIterations = MAX_ITERATIONS;
	allowStandingStill = true;
	maxTimeSteps = MAX_TIMESTEPS;
}

Game::~Game() {
}

ValueFunction *Game::computeValueFunction(decimal tol) {
#if STATE_SPACE_DIMENSION != 4
// XXX: Only for the 4-dim Domain!
#error "Only for the 4-dim Domain!"
#endif

	if (tol <= 0)
		return NULL;

	bool changed = true;
	unsigned int iterationCounter = 0;

	/* In this implementation we have two value functions. The reason being, if
	 * I only have one, I immediately feed back the new values into the same
	 * iteration loop. I believe this breaks the fixed point iteration. */
	ValueFunction *vfunc = new ValueFunction(domain);
	ValueFunction *vfuncNext = new ValueFunction(domain);
	ValueFunction *vfuncTemp;
	GridPointIndex *maxIndex = domain->getMaximalGridIndex();
	int i = 0;
	GridPointIndex *index = new int[STATE_SPACE_DIMENSION];
	while (changed && iterationCounter < maxIterations) {
		iterationCounter++;
		changed = false;

#if OPTIMIZE_SYMMETRY
		// Loop through the grid points of the value function as mentioned on
		// page 195 of the paper!
		for (i = 0; i <= maxIndex[0] / 2; i++) {
			for (int j = 0; j <= maxIndex[1] / 2; j++) {
				for (int k = 0; k <= maxIndex[2]; k++) {
					for (int l = 0; l <= maxIndex[3]; l++) {
#else
		for (i = 0; i <= maxIndex[0]; i++) {
			for (int j = 0; j <= maxIndex[1]; j++) {
				for (int k = 0; k <= maxIndex[2]; k++) {
					for (int l = 0; l <= maxIndex[3]; l++) {
#endif

						// TODO: Speed up!
						// I have the feeling I can speed things up here if I
						// handle the terminal states more precisely!
						// but I have more important things to fix right now!
						updateGridPointIndex(index, i, j, k, l);

						// If the index is not feasible for the given domain,
						// skip the execution of the loop.
						if (!domain->isFeasible(index))
							continue;

						decimal oldValue = vfunc->getValue(index);

						// Before we compute the new Value check if the state is
						// terminal, as this saves us a lot of trouble and
						// computing power.
						decimal newValue = 0;
						if (!isTerminal(index)) {
							newValue = computeValueOnGrid(vfunc, index);
						}

						// Check if the value of the function at this gridindex
						// changed above tol. If it did not, we can leave the
						// outer while-loop.
						if (!changed)
							changed = fabs(oldValue - newValue) > tol;

						vfuncNext->setValue(index, newValue);

#if OPTIMIZE_SYMMETRY
						// XXX: This Symmetry does only work in the standard square domain.
						// TODO: Symmetry in ValueFunction
						// There must be some kind of way to figure out how to
						// push the symmetry to the value function!
						updateGridPointIndex(index, i, maxIndex[1] - j, k,
								maxIndex[3] - l);
						vfuncNext->setValue(index, newValue);
#endif
					}
				}
			}
		}

#if OPTIMIZE_SYMMETRY
		// XXX: This does only work in the standard square domain.
		// Copy the last line as suggested by Cristiane and Falcone.
		// Remember that we stepped out of the i-loop therefore i has the value
		// maxIndex[0] / 2 + 1
		for (int j = 0; j <= maxIndex[1]; j++) {
			for (int k = 0; k <= maxIndex[2]; k++) {
				for (int l = 0; l <= maxIndex[3]; l++) {
					updateGridPointIndex(index, i - 1, j, k, l);

					// If the index is not feasible for the given domain,
					// skip the execution of the loop.
					if (!domain->isFeasible(index))
						continue;

					decimal oldVal = vfuncNext->getValue(index);

					updateGridPointIndex(index, i, maxIndex[1] - j,
							maxIndex[2] - k, maxIndex[3] - l);
					vfuncNext->setValue(index, oldVal);
				}
			}
		}
#endif

		/* Change the value functions as we finished our iteration.
		 *
		 * So in the next iteration the updated values will be our baseline and
		 * the second function will be updated accordingly. */
		vfuncTemp = vfunc;
		vfunc = vfuncNext;
		vfuncNext = vfuncTemp;
	}

#if OPTIMIZE_SYMMETRY
	// XXX: Copy the rest of the symmetry in the value function!
	for (i = 0; i <= maxIndex[0] / 2; i++) {
		for (int j = 0; j <= maxIndex[1]; j++) {
			for (int k = 0; k <= maxIndex[2]; k++) {
				for (int l = 0; l <= maxIndex[3]; l++) {
					updateGridPointIndex(index, i, j, k, l);

					// If the index is not feasible for the given domain,
					// skip the execution of the loop.
					if (!domain->isFeasible(index))
						continue;

					decimal oldVal = vfuncNext->getValue(index);

					updateGridPointIndex(index, maxIndex[0] - i,
							maxIndex[1] - j, maxIndex[2] - k, maxIndex[3] - l);
					vfuncNext->setValue(index, oldVal);
				}
			}
		}
	}
#endif

	delete[] index;

	vfuncNext->setIterations(iterationCounter);
	return vfuncNext;
}

decimal Game::computeValueOnGrid(ValueFunction *vfunc, GridPointIndex *index) {
	ASSERT(vfunc != NULL, "%s:%d", __FILE__, __LINE__);
	ASSERT(domain->isFeasible(index), "%s:%d", __FILE__, __LINE__);

	State *x = domain->getPoint(index);
	decimal ret = computeValueOnGrid(vfunc, x);
	delete[] x;
	return ret;
}

decimal Game::computeValueOnGrid(ValueFunction *vfunc, State *x) {
	ASSERT(vfunc != NULL, "%s:%d", __FILE__, __LINE__);
	ASSERT(domain->isFeasible(x), "%s:%d", __FILE__, __LINE__);

	// The value at point x.
	decimal value = -INFINITY;

	// Some variables I extracted from the loop to check if this would reduce
	// the memory footprint.
	State newState[STATE_SPACE_DIMENSION];
	State blankoState[STATE_SPACE_DIMENSION];
	decimal newValue = 0;

	// Loop through all the control options of P
	for (unsigned int evaderControl = allowStandingStill ? 0 : 1;
			evaderControl <= controlResolution; evaderControl++) {

		// Check if the current control option is admissible
		// if this control is admissible the pursuer variables will already be
		// stored in newState. As for the evader, newstate will just contain
		// his original position as if he stood still.
		if (isAdmissibleControl(x, blankoState, Evader, evaderControl)) {

			// This is the value the evader picks. We did not yet maximize the pursuers
			// controls so this might be different from the actual value.
			decimal evaderValue = INFINITY;

			// New check all the options E has
			for (unsigned int pursuerControl = 1;
					pursuerControl <= controlResolution; pursuerControl++) {

				// Check if E control is admissible
				// Since the two players can't interfere with each others
				// decisions and we store the valid control changes of Pursuer
				// and evader in newState we have a valid new state if the
				// current control of the Evader is also valid.
				// Therefore it is not necessary anymore to reevaluate the
				// kinematic equations.
				if (isAdmissibleControl(blankoState, newState, Pursuer,
						pursuerControl)) {

					// Evaluate the value of the value function according to the
					// first line of the approximation scheme.
					newValue = beta * vfunc->eval(newState) + 1 - beta;

					// ve = min { ve, computeValueOnGrid( state after control change ) )
					// if ve has a new value make set ce's control to the current
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
}

void Game::evalKE(State *x, State *newState, Control p, Control e, decimal step) {
	ASSERT(domain->isFeasible(x), "%s:%d", __FILE__, __LINE__);

	decimal res = controlResolution;
	decimal cont, dir;

	if (p != 0) {
		cont = p;
		dir = 2 * M_PI * (cont / res);
		newState[0] = x[0] + step * velocityP * cos(dir);
		newState[1] = x[1] + step * velocityP * sin(dir);
	} else {
		newState[0] = x[0];
		newState[1] = x[1];
	}

	if (e != 0) {
		cont = e;
		dir = 2 * M_PI * (cont / res);
		newState[2] = x[2] + step * velocityE * cos(dir);
		newState[3] = x[3] + step * velocityE * sin(dir);
	} else {
		newState[2] = x[2];
		newState[3] = x[3];
	}
}

bool Game::getAllowStandingStill() {
	return allowStandingStill;
}

unsigned int Game::getControlResolution() {
	return controlResolution;
}

GameStateDomain *Game::getDomain() {
	return domain;
}

unsigned int Game::getMaxIterations() {
	return maxIterations;
}

unsigned int Game::getMaxTimeSteps() {
	return maxTimeSteps;
}

decimal Game::getTimeStepSize() {
	return timeStepSize;
}

decimal Game::getVelocity(Player p) {
	if (p == Pursuer)
		return velocityP;

	return velocityE;
}

bool Game::isAdmissibleControl(State *x, Player p, Control c) {
	ASSERT(domain->isFeasible(x), "%s:%d", __FILE__, __LINE__);

	State newState[STATE_SPACE_DIMENSION];
	bool ret = isAdmissibleControl(x, newState, p, c);
	return ret;
}

bool Game::isAdmissibleControl(State *x, State *newState, Player p, Control c) {
	ASSERT(domain->isFeasible(x), "%s:%d", __FILE__, __LINE__);

	if (p == Pursuer)
		evalKE(x, newState, c, 0, timeStepSize);
	else
		evalKE(x, newState, 0, c, timeStepSize);

	return domain->isFeasible(newState, p);
}

bool Game::isTerminal(State *x) {
#if STATE_SPACE_DIMENSION != 4
// XXX: Only for the 4-dim Domain!
#error "Only for the 4-dim Domain!"
#endif
	ASSERT(getDomain()->isFeasible(x), "%s:%d\n", __FILE__, __LINE__);

	return (fabs(x[0] - x[2]) <= getDomain()->getMaximalResolutionWidth()) && (fabs(x[1] - x[3]) <= getDomain()->getMaximalResolutionWidth());
}

bool Game::isTerminal(GridPointIndex *index) {
#if STATE_SPACE_DIMENSION != 4
// XXX: Only for the 4-dim Domain!
#error "Only for the 4-dim Domain!"
#endif
	ASSERT(getDomain()->isFeasible(index), "%s:%d\n", __FILE__, __LINE__);

	return (fabs(index[0] - index[2]) <= 1) && (fabs(index[1] - index[3]) <= 1);
}

unsigned int Game::plotTrajectories(ValueFunction *vfunc, State *x,
		std::string outFile) {
	return plotTrajectories(vfunc, x, outFile, "");
}

unsigned int Game::plotTrajectories(ValueFunction *vfunc, State *x, std::string outFile,
		std::string commandFile) {
	ASSERT(domain->isFeasible(x), "%s:%d", __FILE__, __LINE__);
	ASSERT(vfunc != NULL, "%s:%d", __FILE__, __LINE__);

	std::ofstream outStream(outFile.c_str());
	unsigned int timeSteps = 0;
	if (!commandFile.empty()) {
		std::ofstream commandStream(commandFile.c_str());
		timeSteps = plotTrajectories(vfunc, x, &outStream, outFile, &commandStream);
		commandStream.close();
	} else {
		timeSteps = plotTrajectories(vfunc, x, &outStream, outFile, NULL);
	}
	outStream.close();
	return timeSteps;
}

unsigned int Game::plotTrajectories(ValueFunction *vfunc, State *x,
		std::ofstream *outStream) {
	return plotTrajectories(vfunc, x, outStream, NULL, NULL);
}

unsigned int Game::plotTrajectories(ValueFunction *vfunc, State *x,
		std::ofstream *outStream, std::string outFile,
		std::ofstream *commandStream) {
	ASSERT(domain->isFeasible(x), "%s:%d", __FILE__, __LINE__);
	ASSERT(vfunc != NULL, "%s:%d", __FILE__, __LINE__);

	// TODO: Plot the shit out of it!
	if ((commandStream != NULL) && (!outFile.empty())) {
		getDomain()->toGnuPlot(commandStream);

		*commandStream << "plot \"";
		*commandStream << outFile;
		*commandStream << "\" u 1:2 title 'Pursuer' with linespoints ,  \"";
		*commandStream << outFile;
		*commandStream << "\" u 3:4 title 'Evader' with linespoints"
				<< std::endl;
	}

	// Some variables I extracted from the loop to check if this would reduce
	// the memory footprint.
	State *candidateState = GameStateDomain::cloneState(x);
	State *pursuerPreffredState = GameStateDomain::cloneState(x);
	State *currentState = GameStateDomain::cloneState(x);
	State *pursuerBlankoState = GameStateDomain::cloneState(x);
	State *evaderPreferredState = GameStateDomain::cloneState(x);

	// Print the initial state;
	GameStateDomain::printState(x, outStream);

	// Determine the trajectories until we find a terminal state or hit our
	// iteration limit.
	unsigned int timeStep = 0;
	while (!isTerminal(currentState) && timeStep < maxTimeSteps) {
		timeStep++;

		// The value at point x.
		decimal value = -INFINITY;

		// Loop through all the control options of P
		for (unsigned int evaderControl = allowStandingStill ? 0 : 1;
				evaderControl <= controlResolution; evaderControl++) {

			// Check if the current control option is admissible
			// if this control is admissible the pursuer variables will already be
			// stored in newState. As for the evader, newstate will just contain
			// his original position as if he stood still.
			if (isAdmissibleControl(currentState, pursuerBlankoState, Evader,
					evaderControl)) {

				// This is the value the evader picks. We did not yet maximize the pursuers
				// controls so this might be different from the actual value.
				decimal pursuerValue = INFINITY;

				// New check all the options E has
				for (unsigned int pursuerControl = 1;
						pursuerControl <= controlResolution; pursuerControl++) {

					// Check if E control is admissible
					// Since the two players can't interfere with each others
					// decisions and we store the valid control changes of Pursuer
					// and evader in newState we have a valid new state if the
					// current control of the Evader is also valid.
					// Therefore it is not necessary anymore to reevaluate the
					// kinematic equations.
					if (isAdmissibleControl(pursuerBlankoState, candidateState,
							Pursuer, pursuerControl)) {

						// Evaluate the value of the value function according to the
						// first line of the approximation scheme.
						decimal newValue = vfunc->eval(candidateState);

						// ve = min { ve, computeValueOnGrid( state after control change ) )
						// if ve has a new value make set ce's control to the current
						if (pursuerValue > newValue) {
							pursuerValue = newValue;
							updateState(candidateState, pursuerPreffredState);
						}
					}
				}

				// After Checking every option of the evader set the value
				// v = max { v, ve }
				// If the value has changed set the control of pursuer.
				if (value < pursuerValue) {
					value = pursuerValue;
					updateState(pursuerPreffredState, evaderPreferredState);
				}
			}
		}

		updateState(evaderPreferredState, currentState);

		// At the end of the iteration, plot the new current state!
		GameStateDomain::printState(currentState, outStream);
	}
	delete[] candidateState;
	delete[] pursuerPreffredState;
	delete[] currentState;
	delete[] pursuerBlankoState;

	return timeStep;
}

void Game::setAllowStandingStill(bool b) {
	allowStandingStill = b;
}

void Game::setControlResolution(unsigned int n) {
	controlResolution = n;
}

void Game::setDomain(GameStateDomain *d) {
	domain = d;

	if (domain != NULL) {
		setTimeStepSize(domain->getMaximalResolutionWidth() / 2);
	}
}

void Game::setMaxIterations(unsigned int n) {
	maxIterations = n;
}

void Game::setMaxTimeSteps(unsigned int n) {
	maxTimeSteps = n;
}

void Game::setTimeStepSize(decimal s) {
	if (s > 0) {
		timeStepSize = s;
		beta = exp(-timeStepSize);
	}
}

void Game::setVelocity(Player p, decimal v) {
	if (v < 0)
		return;

	if (p == Pursuer)
		velocityP = v;
	else
		velocityE = v;
}

void Game::updateGridPointIndex(GridPointIndex *index, int i, int j, int k,
		int l) {
	index[0] = i;
	index[1] = j;
	index[2] = k;
	index[3] = l;
}

void Game::updateState(State *from, State *to) {
	for (int i = 0; i < STATE_SPACE_DIMENSION; i++)
		to[i] = from[i];
}

}
