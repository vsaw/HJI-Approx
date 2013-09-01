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
 * Definitions.h
 *
 *  Created on: 2011-07-08
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

/**
 * Compiler switch to enable the usage of a 4-dimensional array for the internal
 * storage of the value function.
 * 
 * This will be faster than the original implementation, but is also less
 * tested.
 */
module.exports.OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY = 1;

/**
 * Turn on/off optimizations for symmetric game domains.
 * 
 * If turned on the value function must have the following symmetries:
 * 
 * I v(xp, yp, xe, ye) = v(-xp, -yp, -xe, -ye) II v(xp, yp, xe, ye) = v(-xp, yp,
 * -xe, ye) III v(xp, yp, xe, ye) = v( xp, -yp, xe, -ye)
 */
module.exports.OPTIMIZE_SYMMETRY = 1;

/**
 * Enable/disable robustness checks on the LinearCombination class.
 * 
 * If "0" robustness checks will be performed, that might slow the computation
 * down.
 */
module.exports.OPTIMIZE_LINEAR_COMBINATION_CHECKS = 1;

/**
 * Enable/disable inlining of getter and setters of the value function.
 */
module.exports.OPTIMIZE_LINEAR_COMBINATION_INLINE_GETTER_SETTER = 1;

/**
 * Enable/disable inlining of the static cloneState function in the
 * GameStateDomain.
 */
module.exports.OPTIMIZE_INLINE_GAME_STATE_DOMAIN_CLONE_STATE = 1;

/**
 * Enabling this will compute the convex decomposition of points without
 * recursion.
 */
module.exports.OPTIMIZE_CONVEX_DECOMPOSITION_NO_RECURSION = 1;

/**
 * Enabling this will inline the getter/setter of the value function for
 * performance enhancements.
 */
module.exports.OPTIMIZE_VALUE_FUNCTION_INLINE_GETTER_SETTER = 1;

/**
 * The Dimension of the State Space.
 * 
 * Right now games have to the place in the 2-dimensional euclidian plane. But
 * support for different kind of games can easily be implemented afterwards.
 */
module.exports.STATE_SPACE_DIMENSION = 4;

/**
 * The Length of an linear combination as provided by the GameStateDomain.
 * 
 * Beware that every combination has a length of LINEAR_COMBINATION_LENGTH but
 * not every value of that combination is set. There might be NULL values and
 * therefore shorter combination might be possible.
 */
module.exports.LINEAR_COMBINATON_LENGTH = 16;

/**
 * The maximal amount of iterations that are allowed for the computation of the
 * value function.
 * 
 * The iteration will be aborted if it reaches this limit and did not converge
 * before.
 */
module.exports.MAX_ITERATIONS = 150;

/**
 * The maximal amount of steps that are allowed for the computation of the
 * trajectories.
 * 
 * The calculation will be aborted if it reaches this limit and capture did not
 * occure before.
 */
module.exports.MAX_TIMESTEPS = 2000;

module.exports.EXCEPTION_DIFFERENT_DIMENSION = 1001;
module.exports.EXCEPTION_INDEX_OUT_OF_RANGE = 1002;
module.exports.EXCEPTION_INVALID_POINT = 1003;

/**
 * The default value the value function will return for any point that is not
 * feasible.
 */
module.exports.VALUE_UNDEFINED = -1;

/**
 * The default value for the value function upon initialization.
 */
module.exports.VALUE_FUNCTION_INITIAL = 1;
