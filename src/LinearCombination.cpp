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
 * LinearCombination.cpp
 *
 *  Created on: 2012-07-26
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#include <math.h>
#include <string.h>

#include "LinearCombination.h"
#include "Debug.h"

namespace bsc {

LinearCombination::LinearCombination() {
	for (int i = 0; i < LINEAR_COMBINATON_LENGTH; i++) {
		factors[i] = 1;
		points[i] = 0;
		indexes[i] = 0;
	}
}

LinearCombination::~LinearCombination() {
	for (int i = 0; i < LINEAR_COMBINATON_LENGTH; i++) {
		delete points[i];
		delete indexes[i];
	}
}

#if !OPTIMIZE_LINEAR_COMBINATION_CHECKS
void LinearCombination::checkBounds(int i) {
	ASSERT(i >= 0, "%s:%d", __FILE__, __LINE__);
	ASSERT(i < LINEAR_COMBINATON_LENGTH, "%s:%d", __FILE__, __LINE__);

	if (i < 0 || i >= LINEAR_COMBINATON_LENGTH)
	throw EXCEPTION_INDEX_OUT_OF_RANGE;
}
#endif

State *LinearCombination::eval() {
	State *s = new State[STATE_SPACE_DIMENSION];
	for (int i = 0; i < STATE_SPACE_DIMENSION; i++)
		s[i] = 0;

	for (int i = 0; i < LINEAR_COMBINATON_LENGTH; i++) {
		State *next = getPoint(i);
		if (next != 0) {
			for (int j = 0; j < STATE_SPACE_DIMENSION; j++) {
				s[j] = s[j] + getFactor(i) * next[j];
			}
		}
	}

	return s;
}

#if !OPTIMIZE_LINEAR_COMBINATION_INLINE_GETTER_SETTER
decimal LinearCombination::getFactor(int i) {
#if !OPTIMIZE_LINEAR_COMBINATION_CHECKS
	checkBounds(i);
#endif
	return factors[i];
}

GridPointIndex *LinearCombination::getIndex(int i) {
#if !OPTIMIZE_LINEAR_COMBINATION_CHECKS
	checkBounds(i);
#endif
	return indexes[i];
}

State *LinearCombination::getPoint(int i) {
#if !OPTIMIZE_LINEAR_COMBINATION_CHECKS
	checkBounds(i);
#endif
	return points[i];
}

void LinearCombination::setFactor(int i, decimal f) {
#if !OPTIMIZE_LINEAR_COMBINATION_CHECKS
	checkBounds(i);
#endif
	factors[i] = f;
}

void LinearCombination::setIndex(int i, GridPointIndex *x) {
#if !OPTIMIZE_LINEAR_COMBINATION_CHECKS
	checkBounds(i);
#endif
	indexes[i] = x;
}

void LinearCombination::setPoint(int i, State *s) {
#if !OPTIMIZE_LINEAR_COMBINATION_CHECKS
	checkBounds(i);
#endif
	points[i] = s;
}
#endif

}
