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
 * ValueFunction.h
 *
 *  Created on: 2011-08-08
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#ifndef VALUEFUNCTION_H_
#define VALUEFUNCTION_H_

#include <string>

#include "Definitions.h"
#include "GameStateDomain.h"

namespace bsc {

/**
 * The Value Function.
 *
 * This function can be used to compute the value of the game. Basically it
 * linearly interpolates the value. It does so by storing value of the
 * game at the vertices of the grid. And then uses the convex-decomposition of
 * the GameDomain to determine the the grid it is in.
 *
 * Please note that a new ValueFunction is set to VALUE_UNDEFINED at every point
 * of the GameStateDomain, unless you want to provide a different default value.
 * Use the Game class to compute the value function and set the values of the
 * game for certain states.
 *
 * @see Game
 * @see GameStateDomain
 * @see VALUE_UNDEFINED
 */
class ValueFunction {

private:

#if OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY
#if STATE_SPACE_DIMENSION != 4
#error "Only for the 4-dimensional state space"
#endif
	/**
	 * A four dimensional array that contains the values of the value function.
	 */
	decimal ****values;
#else
	/**
	 * This is array containing the values of the function at a given GridIndex.
	 * While the GridIndex is 4-dimensional the value is a 1-dimensional array.
	 *
	 * We can do this without a problem since there exists a bijective map
	 * from the int^4 to int.
	 *
	 * See the following scheme from the two-dimensional plane for example
	 *
	 * 		6----7----8		Corner Number 0 has GridPointIndex {0, 0}
	 * 		|    |    |		Corner Number 1 has GridPointIndex {1, 0}
	 * 		|    |    |		...
	 * 		3----4----5		Corner Number 7 has GridPointIndex {1, 2}
	 * 		|    |    |		Corner Number 8 has GridPointIndex {2, 2}
	 * 		|    |    |
	 * 		0----1----2
	 *
	 * To convert from GridIndex to Array Index and vice-versa use the
	 * toGridIndex(int arrayIndex) and toArrayIndex(GridPointIndex index)
	 * functions.
	 *
	 * @see ValueFunction::toArrayIndex(GridPointIndex index)
	 */
	decimal *values;

	/** The length of the values array. */
	unsigned long size;
#endif /* VALUE_FUNCTION_4DIM_ARRAY */

	/**
	 * The amount of iterations it took to compute the value function.
	 */
	int iterations;

	/** The Domain of the ValueFunction. */
	GameStateDomain *domain;

	/**
	 * Initializes the properties of the ValueFunction
	 *
	 * @param domain The domain of the value function.
	 * @param defaultValue The default value the value function will return.
	 */
	void initialize(GameStateDomain *domain);

#if !VALUE_FUNCTION_4DIM_ARRAY
	/**
	 * Converts an GridPointIndex to the position in the value array.
	 *
	 * @param GridPointIndex The index.
	 *
	 * @returns The position of the GridCell in the values array.
	 */
	unsigned long toArrayIndex(GridPointIndex *index);
#endif /* !VALUE_FUNCTION_4DIM_ARRAY */

public:
	/**
	 * Creates a new ValueFunction for the given domain.
	 *
	 * @param domain The domain of the value function.
	 *
	 * @see ValueFunction ( GameStateDomain, decimal )
	 */
	ValueFunction(GameStateDomain *domain);

	/**
	 * Creates a new ValueFunction for the given domain with the values obtained
	 * from the given file.
	 *
	 * The file can be created by calling the ValueFunction::persist function to
	 * persist a previously computed value function. This constructor can then
	 * load it again so that additional plots or trajectories can be computed
	 * without having to recompute the whole value function.
	 *
	 * @param domain The domain of the value function.
	 * @param inFile The file to load the values from.
	 */
	ValueFunction(GameStateDomain *domain, std::string inFile);

	virtual ~ValueFunction();

	/**
	 * Evaluation the ValueFunction for the given state.
	 *
	 * This returns the value which might be VALUE_UNDEFINED.
	 *
	 * @param x The state.
	 *
	 * @returns The value of the game.
	 */
	decimal eval(State *x);

	/**
	 * Evaluation the ValueFunction for the given state.
	 *
	 * Since the original plots of the value function were transformed by
	 *
	 * 		T: R^4 -> R,	(xy_P, xy_E) |-> -ln(1 - vfunc(xy_P, xy_E))
	 *
	 * this function will also do the transformation  if the "timeValue"
	 * parameter is true. If it is false, the unaltered values will be returned.
	 *
	 * @param x The state.
	 *
	 * @returns The time value of the game.
	 */
	decimal eval(State *x, bool timeValue);

	/**
	 * Returns the domain of the game.
	 *
	 * @returns The domain of the value function.
	 */
	GameStateDomain *getDomain();

	/**
	 * Get the amount of iterations performed to calculate the value function.
	 *
	 * @returns The amount of iterations performed to calculate the value
	 * 		function.
	 */
	int getIterations();

	/**
	 * Returns the value of the ValueFunction at a given GridPointIndex.
	 *
	 * @param index The index.
	 *
	 * @returns The value of the function at the index.
	 */
#if OPTIMIZE_VALUE_FUNCTION_INLINE_GETTER_SETTER
	decimal getValue(GridPointIndex *index) {
#if !OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY
#error "Inlining of ValueFunction::getValue is only possible when OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY is enabled"
#endif
#if STATE_SPACE_DIMENSION != 4
#error "Only for the 4-dimensional state space"
#endif

		return values[index[0]][index[1]][index[2]][index[3]];
	}
#else
	decimal getValue(GridPointIndex *index);
#endif
	/**
	 * Saves the value function into given file.
	 *
	 * @param filename The name of the file to save the value function to.
	 */
	void persist(std::string filename);

	/**
	 * Print the values of the value function to the standard output stream.
	 */
	void print();

	/**
	 * Sets the amount of iterations needed to compute the Value Function.
	 *
	 * @param iterations The amount of iterations used to compute the value
	 * 		function
	 */
	void setIterations(unsigned int iterations);

	/**
	 * Sets the value of the function at the given index.
	 *
	 * The values set here are used to evaluate the value of the function. The
	 * value of a state is linearly approximated by the values of its
	 * surrounding GridCells.
	 *
	 * @param index The index
	 * @param value The new Value.
	 */
#if OPTIMIZE_VALUE_FUNCTION_INLINE_GETTER_SETTER
	void setValue(GridPointIndex *index, decimal value) {
#if !OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY
#error "Inlining of ValueFunction::setValue is only possible when OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY is enabled"
#endif
#if STATE_SPACE_DIMENSION != 4
#error "Only for the 4-dimensional state space"
#endif

		values[index[0]][index[1]][index[2]][index[3]] = value;
	}
#else
	void setValue(GridPointIndex *index, decimal value);
#endif

	/**
	 * Plot the value function.
	 *
	 * This will create a GnuPlot command file to be used with GnuPlot to see a
	 * plot of the value function. To see the plot start gnuplot and type
	 * 		load "File_the_command_stream_points_to"
	 *
	 * The Value Function is defined on the GameState domain, which is in most
	 * cases a subset of R^4 and therefore does not allow a direct plot of the
	 * value function.
	 *
	 * 		vfunc: R^4 -> R,	(xy_P, xy_E) |-> vfunc(xy_P, xy_E)
	 *
	 * Therefore a means to define a fixed starting point for one player has
	 * been provided. This fixed staring point provides a partial evaluation so
	 * that only one player can move in the game state domain which makes the
	 * partially evaluated value function a function from the R^2 -> R and thus
	 * leading to a function that can be plotted using GnuPlot.
	 *
	 * In case that the Pursuer has a fixed position x_P the partial evaluation
	 * will be
	 *
	 * 		vfunc_Partial: R^2 -> R, x_E |-> vfunc(x_P, x_E)
	 *
	 * Since the original plots of the value function were transformed by
	 *
	 * 		T: R^4 -> R,	(xy_P, xy_E) |-> -ln(1 - vfunc(xy_P, xy_E))
	 *
	 * this function will also do the transformation before generating the plot
	 * values if the "timeValue" parameter is true. If it is false, the
	 * unaltered values will be plotted.
	 *
	 * Output will be separated into one file containing the commands to set
	 * up GnuPlot (e.g. style of plot, size of the domain) and a second file
	 * containing the values of the value function.
	 *
	 * In order to ensure that everything works fine the command file needs to
	 * know the file name of the data file. Therefore users of the function
	 * need to provide the file name of the data file.
	 *
	 * @param commandStream The stream to write the GnuPlot commands to.
	 * @param dataStream The stream to write the values of the value function
	 * 		to.
	 * @param dataFileName The name of the file the dataStream is pointing to.
	 * @param p The player to remain at a fixed position.
	 * @param x The position of the fixed player.
	 * @param timePlot If true the transformation with T will be performed
	 * 		before the plot.
	 */
	void toGnuPlot(std::ofstream *commandStream, std::ofstream *dataStream,
			std::string dataFileName, Player p, State *x, bool timePlot);

	/**
	 * Plot the value function.
	 *
	 * This will create a GnuPlot command file to be used with GnuPlot to see a
	 * plot of the value function. To see the plot start gnuplot and type
	 * 		load "File_the_command_stream_points_to"
	 *
	 * The Value Function is defined on the GameState domain, which is in most
	 * cases a subset of R^4 and therefore does not allow a direct plot of the
	 * value function.
	 *
	 * 		vfunc: R^4 -> R,	(xy_P, xy_E) |-> vfunc(xy_P, xy_E)
	 *
	 * Therefore a means to define a fixed starting point for one player has
	 * been provided. This fixed staring point provides a partial evaluation so
	 * that only one player can move in the game state domain which makes the
	 * partially evaluated value function a function from the R^2 -> R and thus
	 * leading to a function that can be plotted using GnuPlot.
	 *
	 * In case that the Pursuer has a fixed position x_P the partial evaluation
	 * will be
	 *
	 * 		vfunc_Partial: R^2 -> R, x_E |-> vfunc(x_P, x_E)
	 *
	 * Since the original plots of the value function were transformed by
	 *
	 * 		T: R^4 -> R,	(xy_P, xy_E) |-> -ln(1 - vfunc(xy_P, xy_E))
	 *
	 * this function will also do the transformation before generating the plot
	 * values if the "timeValue" parameter is true. If it is false, the
	 * unaltered values will be plotted.
	 *
	 * Output will be separated into one file containing the commands to set
	 * up GnuPlot (e.g. style of plot, size of the domain) and a second file
	 * containing the values of the value function. The files will be created
	 * if they don't exist and existing files will be deleted.
	 *
	 * @param commandFile The filename to store the GnuPlot commands.
	 * @param dataFile The file to store the values of the ValueFunction.
	 * @param p The player to remain at a fixed position.
	 * @param x The position of the fixed player.
	 * @param timePlot If true the transformation with T will be performed
	 * 		before the plot.
	 */
	void toGnuPlot(std::string commandFile, std::string dataFile, Player p,
			State *x, bool timePlot);
};

}

#endif /* VALUEFUNCTION_H_ */
