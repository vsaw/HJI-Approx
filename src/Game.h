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
 * Game.h
 *
 *  Created on: 2011-08-08
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#ifndef GAME_H_
#define GAME_H_

#include "Definitions.h"
#include "GameStateDomain.h"
#include "ValueFunction.h"

#include <string>

namespace bsc {

/**
 * The game class.
 *
 * This class is used to set various parameters of the game, like the velocity
 * of the players, and make calculation of the value of the game.
 */
class Game {

private:

	/**
	 * If true, players are allowed to stand still. Otherwise they have to make
	 * a move.
	 *
	 * By default players are allowed to stand still at one position.
	 */
	bool allowStandingStill;

	/**
	 * Dampening factor computed from the time step size in order to make our
	 * fixed point iteration a contraction which will converge because of the
	 * banach fixed point theorem.
	 */
	decimal beta;

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
	unsigned int controlResolution;

	/**
	 * The GameStateDomain for this game.
	 *
	 * This domain is used in various placed all over the class and defined
	 * where the game takes place.
	 *
	 * @see GameStateDomain
	 */
	GameStateDomain *domain;

	/**
	 * The maximal amount of valid iterations in the computation of the value
	 * function.
	 *
	 * If computation of the value function does not converge within
	 * maxIterations. The iteration will be stopped and considered unsuccessful.
	 */
	unsigned int maxIterations;

	/**
	 * The maximal amount of valid iterations in the computation of the
	 * trajectories.
	 *
	 * If computation of the trajectories does not converge within maxTimeSteps,
	 * the value of the game will be considered infinite and catch does not
	 * occur.
	 */
	unsigned int maxTimeSteps;

	/**
	 * The step size.
	 *
	 * Smaller values will lead to a better approximation of the value function
	 * but also to higher computational costs.
	 *
	 * Please note that to ensure convergence of the approximation scheme we
	 * need to pick the step size smaller than the diameters of the grid cells.
	 */
	decimal timeStepSize;

	/**
	 * The velocity of the evader.
	 *
	 * Default value is 1
	 */
	decimal velocityE;

	/**
	 * The velocity of the pursuer.
	 *
	 * Default value is 1
	 */
	decimal velocityP;

	/**
	 * Computes the value of the ValueFunction at given State which is located
	 * on the grid of the GameStateDomain.
	 *
	 * This value is used for the internal calculation of the value function.
	 * Basically it corresponds to the first line of fully discrete
	 * approximation scheme for the constrained case by Cristiane & Falcone on
	 * Page 182 of the paper.
	 *
	 * @param vfunc The value function to approximate.
	 * @param x The point to approximate
	 *
	 * @returns The value.
	 */
	decimal computeValueOnGrid(ValueFunction *vfunc, State *x);

	/**
	 * Computes the value of the ValueFunction at given State which is located
	 * on the grid of the GameStateDomain.
	 *
	 * This value is used for the internal calculation of the value function.
	 * Basically it corresponds to the first line of fully discrete
	 * approximation scheme for the constrained case by Cristiane & Falcone on
	 * Page 182 of the paper.
	 *
	 * @param vfunc The value function to approximate.
	 * @param index The index of the point to approximate.
	 *
	 * @returns The value.
	 */
	decimal computeValueOnGrid(ValueFunction *vfunc, GridPointIndex *index);

	/**
	 * Checks if the given control is admissible for the player at the current
	 * state of the game.
	 *
	 * @param x The current state
	 * @param p The player who wants to move.
	 * @param c The direction he wants to move to.
	 *
	 * @returns True if it is a valid move; otherwise false.
	 *
	 * @see GameStateDomain::isFeasible
	 */
	bool isAdmissibleControl(State *x, Player p, Control c);

	/**
	 * Checks if the given control is admissible for the player at the current
	 * state of the game.
	 *
	 * If it is feasible the new state after the control change will be stored
	 * in the newState variable.
	 *
	 * @param x The current state
	 * @param newState The new state after the player has made a move.
	 * @param p The player who wants to move.
	 * @param c The direction he wants to move to.
	 *
	 * @returns True if it is a valid move; otherwise false.
	 *
	 * @see GameStateDomain::isFeasible
	 */
	bool isAdmissibleControl(State *x, State *newState, Player p, Control c);

	/**
	 * Updates the values of the given GridPointIndex with the given variables.
	 *
	 * @param index The index to update.
	 * @param coord0 First value of the index.
	 * @param coord1 Second value of the index.
	 * @param coord2 Third value of the index.
	 * @param coord3 Fourth value of the index.
	 */
	void updateGridPointIndex(GridPointIndex *index, int coord0, int coord1,
			int coord2, int coord3);

	/**
	 * Copies the first state to the second variable.
	 *
	 * This will create a deep copy and not just set "to" to "from".
	 * Every component will be copied.
	 *
	 * @param from The source.
	 * @param to The target.
	 */
	void updateState(State *from, State *to);

protected:
	/**
	 * Evaluate the kinematic equation
	 *
	 * This computes the new state of the game as it evolves from the given
	 * state and the the given controls of the players. It is not being returned
	 * but stored in an input parameter for less memory consumption
	 *
	 * @param x The current state of the game.
	 * @param newState The variable to stor the new state in.
	 * @param p The control of the pursuer.
	 * @param e The control of the evader.
	 * @param timeStepSize The size of the timestep.
	 */
	virtual void evalKE(State *x, State *newState, Control p, Control e,
			decimal stepSize);

public:

	/**
	 * Creates a new game.
	 *
	 * This game has to be initialized at a later point by calling the various
	 * setter available.
	 *
	 * @see Game::setControlResolution( unsigned int n )
	 * @see Game::setDomain( GameSitateDomain *d )
	 * @see Game::setVelocity( Player p, decimal v )
	 */
	Game();
	virtual ~Game();

	/**
	 * This will calculate the value function.
	 *
	 * This computation will disregard the initial state and compute really the
	 * complete value for all possible states! The returning value function will
	 * be defined at each possible states.
	 *
	 * Use with care as it will result in massive computational effort.
	 *
	 * @param tol The tolerance for the convergence.
	 *
	 * @returns The value function for this game.
	 *
	 * @see ValueFunction
	 */
	ValueFunction *computeValueFunction(decimal tol);

	/**
	 * Returns true if players are allowed to stand still.
	 *
	 * If not standing still is not a valid move and the will have to move.
	 * In Cristiane & Falcones Paper this corresponds to the "+ 1" move option.
	 *
	 * @returns True if players are allowed to stand still; false otherwise.
	 *
	 * @see Game::getControlResolution()
	 * @see Game:.setControlResolution( unsigned int n )
	 */
	bool getAllowStandingStill();

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
	unsigned int getControlResolution();

	/**
	 * Returns the GameStateDomain for this game.
	 *
	 * @returns GameStateDomain
	 *
	 * @see GameStateDomain
	 */
	GameStateDomain *getDomain();

	/**
	 * Returns the maximal amount of valid iterations in the computation of the
	 * value function.
	 *
	 * If computation of the value function does not converge within
	 * maxIterations. The iteration will be stopped and considered unsuccessful.
	 *
	 * @returns The maximal amount of valid iterations.
	 */
	unsigned int getMaxIterations();

	/**
	 * Get the maximal amount of valid iterations in the computation of the
	 * trajectories.
	 *
	 * If computation of the trajectories does not converge within maxTimeSteps,
	 * the value of the game will be considered infinite and catch does not
	 * occur.
	 *
	 * @returns The maximal amount.
	 */
	unsigned int getMaxTimeSteps();

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
	decimal getTimeStepSize();

	/**
	 * Get the velocity for player P.
	 *
	 * @param p The player.
	 *
	 * @return The velocity of the player.
	 */
	decimal getVelocity(Player p);

	/**
	 * Plot of the optimal trajectories for the game at State x.
	 *
	 * This plot will be just a data file that needs to be loaded into GNUplot
	 * separately.
	 *
	 * @param vfunc The value function of the game.
	 * @param x The state to plot the trajectories.
	 * @param outFile The name of file to store the plots.
	 *
	 * @returns The amount of timesteps until capture or maxTimeSteps if capture
	 * 		did not occur within maxTimeSteps.
	 */
	unsigned int plotTrajectories(ValueFunction *vfunc, State *x,
			std::string outFile);

	/**
	 * Plot of the optimal trajectories for the game at State x.
	 *
	 * This plot will be just a data file that needs to be loaded into GNUplot
	 * separately.
	 *
	 * @param vfunc The value function of the game.
	 * @param x The state to plot the trajectories.
	 * @param outFile The name of file to store the plots.
	 * @param commandFile The name of the file that stores the GNUPlot commands.
	 *
	 * @returns The amount of timesteps until capture or maxTimeSteps if capture
	 * 		did not occur within maxTimeSteps.
	 */
	unsigned int plotTrajectories(ValueFunction *vfunc, State *x,
			std::string outFile, std::string commandFile);

	/**
	 * Plot of the optimal trajectories for the game at State x.
	 *
	 * This plot will be just a data file that needs to be loaded into GNUplot
	 * separately.
	 *
	 * @param vfunc The value function of the game.
	 * @param x The state to plot the trajectories.
	 * @param outStream The stream to store the plots.
	 *
	 * @returns The amount of timesteps until capture or maxTimeSteps if capture
	 * 		did not occur within maxTimeSteps.
	 */
	unsigned int plotTrajectories(ValueFunction *vfunc, State *x,
			std::ofstream *outStream);

	/**
	 * Plot of the optimal trajectories for the game at State x.
	 *
	 * This plot will be just a data file that needs to be loaded into GNUplot
	 * separately.
	 *
	 * @param vfunc The value function of the game.
	 * @param x The state to plot the trajectories.
	 * @param outStream The stream to store the plots.
	 * @param outFile The name of the data output file.
	 * @param commandStream The stream to store the GNUPlot commands.
	 *
	 * @returns The amount of timesteps until capture or maxTimeSteps if capture
	 * 		did not occur within maxTimeSteps.
	 */
	unsigned int plotTrajectories(ValueFunction *vfunc, State *x,
			std::ofstream *outStream, std::string outFile,
			std::ofstream *commandStream);

	/**
	 * Set if players are allowed to stand still during the game.
	 *
	 * This corresponds to Cristiane & Falcones "+ 1" direction.
	 *
	 * @param b The value if they are allowed or not.
	 *
	 * @see Game::getAllowStandingStill()
	 * @see Game::getControlResolution()
	 * @see Game::setControlResolution( unsigned int n )
	 */
	void setAllowStandingStill(bool b);

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
	 * @param n The new resolution.
	 *
	 * @see Game::getAllowStandingStill()
	 * @see Game::setAllowStandingStill( bool b )
	 * @see Game::getControlResolution()
	 */
	void setControlResolution(unsigned int n);

	/**
	 * Sets the GameStateDomain for this game.
	 *
	 * This will also automatically set the time step size to
	 * 		maxResolutionWidth / 2
	 * of this domain. However the time step size can still be adjusted
	 * individually afterwards.
	 *
	 * @param d A pointer to the new domain.
	 *
	 * @see GameStateDomain
	 * @see GameStateDomain::getMaximalResolutionWidth()
	 * @see Game::setTimeStepSize( decimal step )
	 */
	void setDomain(GameStateDomain *d);

	/**
	 * Sets the maximal amount of valid iterations for the computation of the
	 * value function.
	 *
	 * If computation of the value function does not converge within
	 * maxIterations. The iteration will be stopped and considered unsuccessful.
	 *
	 * @param n The new amount of maximal iterations.
	 */
	void setMaxIterations(unsigned int n);

	/**
	 * Set the maximal amount of valid iterations in the computation of the
	 * trajectories.
	 *
	 * If computation of the trajectories does not converge within maxTimeSteps,
	 * the value of the game will be considered infinite and catch does not
	 * occur.
	 *
	 * @param n The maximal amount.
	 */
	void setMaxTimeSteps(unsigned int n);

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
	 * @param s The new step size. Must be greater than 0.
	 */
	void setTimeStepSize(decimal s);

	/**
	 * Set the velocity of the player.
	 *
	 * This function will do nothing if the new velocity is less than 0.
	 *
	 * @param p The player.
	 * @param v The new velocity. Must be greater or equal 0.
	 */
	void setVelocity(Player p, decimal v);

	/**
	 * Determines if a given Point is inside the terminal set.
	 *
	 * A state is determined terminal if it the two players are placed on
	 * adjacent grid cells. Adjacency is determined by the index of the grid
	 * cells. If it differs by at max 1 then the state shall be regarded as
	 * terminal.
	 *
	 * @param x The the state
	 *
	 * @returns True if the state is terminal, false otherwise.
	 */
	virtual bool isTerminal(State *x);

	/**
	 * Determines if a given Point is inside the terminal set.
	 *
	 * A state is determined terminal if it the two players are placed on
	 * adjacent grid cells. Adjacency is determined by the index of the grid
	 * cells. If it differs by at max 1 then the state shall be regarded as
	 * terminal.
	 *
	 * @param index The index of the state.
	 *
	 * @returns True if the state is terminal, false otherwise.
	 */
	virtual bool isTerminal(GridPointIndex *index);
};

}

#endif /* GAME_H_ */
