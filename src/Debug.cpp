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
 * Debug.cpp
 *
 *  Created on: 2012-07-09
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */
#include "Debug.h"

void assert_equals_valueAtGridIndex(int expected, int actual,
		GridPointIndex *index) {
	if (expected != actual) {
		cout << "Expected: " << expected << " Actual: " << actual << " At: ";
		for (int i = 0; i < STATE_SPACE_DIMENSION; i++)
			cout << index[i] << " ";

		cout << endl;
	}
}

void assert_ValueFuntion_isSymmetrical(ValueFunction *vfunc) {
#if STATE_SPACE_DIMENSION != 4
#error "Works only in the four dimensional state space"
#endif

	if (vfunc == NULL)
		return;

	GameStateDomain *domain = vfunc->getDomain();
	GridPointIndex *maxIndex = domain->getMaximalGridIndex();
	GridPointIndex *cur = new int[STATE_SPACE_DIMENSION];
	GridPointIndex *sym = new int[STATE_SPACE_DIMENSION];

	for (int i0 = 0; i0 < maxIndex[0] + 1; i0++) {
		cur[0] = i0;
		for (int i1 = 0; i1 < maxIndex[1] + 1; i1++) {
			cur[1] = i1;
			for (int i2 = 0; i2 < maxIndex[2] + 1; i2++) {
				cur[2] = i2;
				for (int i3 = 0; i3 < maxIndex[3] + 1; i3++) {
					cur[3] = i3;

					/* Now e check all the symmtries of the value function by
					 * Cristini and Falcone mentioned on page 194 (or 18)
					 */

					// First build symmetry (20)
					for (int i = 0; i < STATE_SPACE_DIMENSION; i++)
						sym[i] = maxIndex[i] - cur[i];

					ASSERT_EQUALS(vfunc->getValue(cur), vfunc->getValue(sym),
							1e-3,
							"No Symmetry (20) at %d %d %d %d (%f) != %d %d %d %d (%f)\n",
							cur[0], cur[1], cur[2], cur[3], vfunc->getValue(cur), sym[0], sym[1], sym[2], sym[3], vfunc->getValue(sym));

					// Next Symmetry (21)
					sym[0] = maxIndex[0] - cur[0];
					sym[1] = cur[1];
					sym[2] = maxIndex[2] - cur[2];
					sym[3] = cur[3];
					ASSERT_EQUALS(vfunc->getValue(cur), vfunc->getValue(sym),
							1e-3,
							"No Symmetry (21) at %d %d %d %d (%f) != %d %d %d %d (%f)\n",
							cur[0], cur[1], cur[2], cur[3], vfunc->getValue(cur), sym[0], sym[1], sym[2], sym[3], vfunc->getValue(sym));

					// Next Symmetry (22)
					sym[0] = cur[0];
					sym[1] = maxIndex[1] - cur[1];
					sym[2] = cur[2];
					sym[3] = maxIndex[3] - cur[3];
					ASSERT_EQUALS(vfunc->getValue(cur), vfunc->getValue(sym),
							1e-3,
							"No Symmetry (22) at %d %d %d %d (%f) != %d %d %d %d (%f)\n",
							cur[0], cur[1], cur[2], cur[3], vfunc->getValue(cur), sym[0], sym[1], sym[2], sym[3], vfunc->getValue(sym));
				}
			}
		}
	}

	delete[] cur;
}

void util_print_State(State *s) {
	cout << "(";
	for (int i = 0; i < STATE_SPACE_DIMENSION - 1; i++) {
		cout << s[i] << ", ";
	}
	cout << s[STATE_SPACE_DIMENSION - 1] << ")" << endl;
}

void util_print_LinearCombination(LinearCombination *lc) {
	for (int i = 0; i < LINEAR_COMBINATON_LENGTH - 1; i++) {
		if (lc->getPoint(i) != 0) {
			cout << lc->getFactor(i) << " * ";
			util_print_State(lc->getPoint(i));
		}
	}
	if (lc->getPoint(LINEAR_COMBINATON_LENGTH - 1) != 0) {
		cout << lc->getFactor(LINEAR_COMBINATON_LENGTH - 1) << " * ";
		util_print_State(lc->getPoint(LINEAR_COMBINATON_LENGTH - 1));
	}
}
