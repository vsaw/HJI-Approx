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
 * GameStateDomain.cpp
 *
 *  Created on: 2011-07-08
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#include <math.h>
#include <string.h>

#include "GameStateDomain.h"

namespace bsc {

#if !OPTIMIZE_INLINE_GAME_STATE_DOMAIN_CLONE_STATE
State *GameStateDomain::cloneState(State *s) {
	State *ret = new State[STATE_SPACE_DIMENSION];
	memcpy(ret, s, STATE_SPACE_DIMENSION * sizeof(State));
	return ret;
}
#endif

GridPointIndex * GameStateDomain::cloneIndex(GridPointIndex * x) {
	GridPointIndex * ret = new GridPointIndex[STATE_SPACE_DIMENSION];
	memcpy(ret, x, STATE_SPACE_DIMENSION * sizeof(GridPointIndex));
	return ret;
}

bool GameStateDomain::equals(State *s1, State *s2) {
	return equals(s1, s2, 0);
}

bool GameStateDomain::equals(State *s1, State *s2, decimal eps) {
	if (s1 == 0 || s2 == 0)
		return false;

	for (int i = 0; i < STATE_SPACE_DIMENSION; i++) {
		if (fabs(s1[i] - s2[i]) > eps)
			return false;
	}
	return true;
}

void GameStateDomain::printState(State *x, std::ofstream *stream) {
	for (int i = 0; i < STATE_SPACE_DIMENSION; i++) {
		*stream << x[i] << "\t";
	}
	*stream << std::endl;
}

}
