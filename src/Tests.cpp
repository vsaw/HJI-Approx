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
 * Tests.cpp
 *
 *  Created on: 2012-07-28
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#include <iostream>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Game.h"
#include "Tests.h"
#include "Debug.h"
#include "SquareDomainRoundHole.h"

using namespace std;
using namespace bsc;

#ifdef DEBUG
void profile_ValueFunction_eval() {
	cout << "==== profile_ValueFunction_eval" << endl;

	clock_t startClock = clock();

	int n = 20;
	SquareDomain *sq = new SquareDomain(0, 1, n);
	ValueFunction *v = new ValueFunction(sq);

	State x[STATE_SPACE_DIMENSION];

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				for (int l = 0; l < n; l++) {
					x[0] = ((decimal) i) / n;
					x[1] = ((decimal) j) / n;
					x[2] = ((decimal) k) / n;
					x[3] = ((decimal) l) / n;

					v->eval(x);
				}
			}
		}
	}

	clock_t endClock = clock();

	cout << endClock - startClock << endl;

	delete v;
	delete sq;
}

void profile_Game_computeValueFunction() {
	cout << "==== profile_Game_computeValueFunction" << endl;

	int n = 10;
	SquareDomain *sq = new SquareDomain(-2, 2, n);
	Game *g = new Game();

	g->setDomain(sq);
	g->setControlResolution(8);
	g->setVelocity(Pursuer, 2);
	g->setTimeStepSize(4 / (2 * n));

	for (int i = 0; i < 5; i++) {

		clock_t startClock = clock();
		ValueFunction *vfunc = g->computeValueFunction(1e-5);
		clock_t endClock = clock();
		cout << endClock - startClock << endl;
		delete vfunc;

	}

	delete sq;
	delete g;
}

void test_Game_computeTinyValueFunction() {
	cout << "==== test_Game_computeTinyValueFunction" << endl;

	unsigned int n = 2;
	SquareDomain *sq = new SquareDomain(-2, 2, n);
	Game *g = new Game();

	g->setDomain(sq);
	g->setControlResolution(6);
	g->setVelocity(Pursuer, 2);
	g->setTimeStepSize(4 / (2 * n));
	g->setMaxIterations(5);

	for(unsigned int iteration = 0; iteration < g->getMaxIterations(); iteration++) {
		ValueFunction *vfunc = g->computeValueFunction(1e-3);

		for(int i = 0; i < sq->getMaximalGridIndex()[0]; i++) {
			for(int j = 0; j < sq->getMaximalGridIndex()[1]; j++) {
				for(int k = 0; k < sq->getMaximalGridIndex()[2]; k++) {
					for(int l = 0; l < sq->getMaximalGridIndex()[3]; l++) {
						GridPointIndex index[] = {i,j,k,l};
						ASSERT_EQUALS(0, vfunc->getValue(index), 0.1,
							"0 != %f\n", vfunc->getValue(index));
					}
				}
			}
		}

#if 0
		// Dump the vfunc for manual debugging
		cout << iteration;
		cout << ": ";
		vfunc->print();
		cout << endl;
#endif

		delete vfunc;
	}

	delete sq;
	delete g;
}

void test_GameStateDomain_clonePoint() {
	cout << "==== test_GameStateDomain_clonePoint()" << endl;

	State p1[] = { 0.0, 0.1, 0.2, 0.3 };
	State *clone = GameStateDomain::cloneState(p1);

	if (!GameStateDomain::equals(p1, clone))
		cout << "not Equal!" << endl;

	p1[0] = 2;
	if (p1[0] == clone[0])
		cout << "same reference!" << endl;

	delete[] clone;
}

void test_GameStateDomain_equals() {
	printf("==== test_GameStateDomain_equals()\n");

	State p1[] = { 0.0, 0.1, 0.2, 0.3 };
	State p2[] = { 0.0, 0.1, 0.2, 0.3 };
	State p3[] = { 0.0, 0.01, 0.25, 0.2 };
	State p4[] = { 0.0, 0.01, 0.25, 0.19 };

	ASSERT(GameStateDomain::equals(p1, p1), "p1 != p1\n");
	ASSERT(GameStateDomain::equals(p1, p2), "p1 != p2\n");
	ASSERT(GameStateDomain::equals(p1, p3, 0.1), "p1 != p3\n");
	ASSERT(!GameStateDomain::equals(p1, p3), "p1 == p3\n");
	ASSERT(!GameStateDomain::equals(p1, p4, 0.1), "p1 == p4\n");
}

void test_GameStateDomain_eval() {
	printf("==== test_GameStateDomain_eval()\n");

	LinearCombination *lc = new LinearCombination();

	// Build Linear Combination.
	for (int i = 0; i < LINEAR_COMBINATON_LENGTH; i++) {
		lc->setFactor(i, pow(2, -LINEAR_COMBINATON_LENGTH));

		State *dp = new State[STATE_SPACE_DIMENSION];
		int p = i / 2;
		for (int j = 0; j < STATE_SPACE_DIMENSION; j++) {
			if (j == p)
				dp[j] = pow(-1, i);
			else
				dp[j] = 0;
		}
		lc->setPoint(i, dp);
	}

	// Build Expected result
	State expected[] = { 0.0, 0.0, 0.0, 0.0 };

	State *actual = lc->eval();
	ASSERT(GameStateDomain::equals(expected, actual, 1e-9), "Not Zero\n");

	delete[] actual;
	delete lc;
}

void test_LinearCombination_creation() {
	cout << "==== test_LinearCombination_creation()" << endl;

	LinearCombination *lc = new LinearCombination();

	for (int i = 0; i < LINEAR_COMBINATON_LENGTH; i++) {
		ASSERT(lc->getPoint(i) == 0, "Point %d\n", i);
		ASSERT_EQUALS(1, lc->getFactor(i), 1e-9, "Factor %d\n", i);
	}
}

void test_SquareDomainSquareHole_convexCombination() {
	cout << "==== test_SquareDomainSquareHole_convexCombination()" << endl;

	SquareDomainSquareHole *sq = new SquareDomainSquareHole(-1, 1, -0.5, 0.5,
			4);

	State p0[] = { 0.6, 0.0, 0.5, 0.0 };
	LinearCombination *lc = sq->convexDecomposition(p0);
	State *actual = lc->eval();
	ASSERT(SquareDomain::equals(p0, actual, 1e-9), "p0\n");
//	cout << "LinComb of P0: ";
//	util_print_LinearCombination(lc);
//	cout << "Actual of P0: ";
//	util_print_State(actual);
	delete[] actual;
	delete lc;

	State p1[] = { 0.1, 0.9, -0.5, -1.0 };
	lc = sq->convexDecomposition(p1);
	actual = lc->eval();
//	cout << "LinComb of P1: ";
//	util_print_LinearCombination(lc);
//	cout << "Actual of P1: ";
//	util_print_State(actual);
	ASSERT(SquareDomain::equals(p1, actual, 1e-9), "p1\n");
	delete[] actual;
	delete lc;

	State p2[] = { 1.0, 1.0, 1.0, 1.0 };
	lc = sq->convexDecomposition(p2);
	actual = lc->eval();

	//	cout << "LinComb of P2: ";
	//	util_print_LinearCombination(lc);
	//	cout << "Actual of P2: ";
	//	util_print_State(actual);

	ASSERT(SquareDomain::equals(p2, actual, 1e-9), "p2\n");
	delete[] actual;
	delete lc;

	State p3[] = { -1.0, -1.0, -1.0, -1.0 };
	lc = sq->convexDecomposition(p3);
	actual = lc->eval();
	//	cout << "LinComb of P3: ";
	//	util_print_LinearCombination(lc);
	//	cout << "Actual of P3: ";
	//	util_print_State(actual);
	ASSERT(SquareDomain::equals(p3, actual, 1e-9), "p3\n");
	delete[] actual;
	delete lc;

	State p4[] = { 0.5, 0.5, 0.5, 0.5 };
	lc = sq->convexDecomposition(p4);
	actual = lc->eval();
	//	cout << "LinComb of P4: ";
	//	util_print_LinearCombination(lc);
	//	cout << "Actual of P4: ";
	//	util_print_State(actual);
	ASSERT(SquareDomainSquareHole::equals(p4, actual, 1e-9), "p4\n");
	delete[] actual;
	delete lc;

	delete sq;

	sq = new SquareDomainSquareHole(-2, 2, -0.5, 0.5, 16);
	State p5[] = { -1.78, -1.84, -2, -1.46 };
	lc = sq->convexDecomposition(p5);
	actual = lc->eval();
//	cout << "LinComb of P5: ";
//	util_print_LinearCombination(lc);
//	cout << "Actual of P5: ";
//	util_print_State(actual);
	for (int i = 0; i < lc->getSize(); i++) {
		if (lc->getPoint(i) == NULL) {
			ASSERT_NULL(lc->getIndex(i), "Index not null %d", i);
		} else {
			GridPointIndex *index = sq->getGridIndex(lc->getPoint(i));
			for (int d = 0; d < STATE_SPACE_DIMENSION; d++) {
				ASSERT_EQUALS(index[d], lc->getIndex(i)[d], 1e-9,
						"Index number %d Component %d: %d <> %d",
						i, d, index[d], lc->getIndex(i)[d]);
			}
			delete[] index;
		}
	}
	ASSERT(SquareDomainSquareHole::equals(p5, actual, 1e-9), "p5\n");
	delete[] actual;
	delete lc;
	delete sq;
}

void test_SquareDomainSquareHole_getGridIndex() {
	cout << "==== test_SquareDomainSquareHole_getGridIndex()" << endl;

	SquareDomainSquareHole *sq = new SquareDomainSquareHole(-1, 1, -0.5, 0.5,
			21);

	State points[3][4] = { { 1.0, 0.5, 0.0, 1.0 }, { -1.0, -1.0, -0.5, 0.5 }, {
			-1.0, -1.0, -1.0, -1.0 } };

	int index[3][4] = { { 20, 15, 10, 20 }, { 0, 0, 5, 15 }, { 0, 0, 0, 0 } };

	for (int i = 0; i < 3; i++) {
		int *ret = sq->getGridIndex(points[i]);

		ASSERT_NOT_NULL(ret, "GridPointIndex %d is null\n", i);

		for (int j = 0; j < 4; j++) {
			ASSERT(ret[j] == index[i][j], "Index %d %d\n", i, j);
		}
		delete[] ret;
	}

	delete sq;
}

void test_SquareDomainSquareHole_isFeasible() {
	printf("==== test_SquareDomainSquareHole_isFeasible\n");

	SquareDomainSquareHole *sq = new SquareDomainSquareHole(-1, 1, -0.5, 0.5,
			21);

	State feasiblePoints[][4] = { { -1, -1, -1, -1 }, { -1, -1, -1, 1 }, { -1,
			-1, 1, -1 }, { -1, -1, 1, 1 }, { -1, 1, -1, 1 }, { -1, 1, 1, 1 }, {
			-0.5, -0.5, -0.5, -0.5 }, { -0.5, -0.5, -0.5, 0.5 }, { -0.5, -0.5,
			0.51, -0.5 }, { -0.5, -0.5, 0.51, 0.511 },
			{ -0.5, 0.51, -0.5, 0.51 }, { -0.5, 0.5, 0.51, 0.5 }, { -1, -0.5,
					0.51, 0.5 }, { -0.5, -0.5, 1, 0.5 },
			{ -0.5, -0.51, -1, 0.5 }, { -0.5, -0.9, 0.5, 0.8 }, { -0.5, -0.5,
					0.6, 0.5 }, { 0, 0.55, 1, 1 } };

	for (unsigned int i = 0;
			i < sizeof(feasiblePoints) / sizeof(feasiblePoints[0]); i++) {
		ASSERT(sq->isFeasible(feasiblePoints[i]),
				"Feasible Point %d: %f %f %f %f\n",
				i, feasiblePoints[i][0], feasiblePoints[i][1], feasiblePoints[i][2], feasiblePoints[i][3]);
	}

	State unfeasiblePoints[][4] = { { 0, 0, 0, 0 }, { 0, -1.1, -1, 1 }, { -1,
			0, 1.1, -1 }, { -1, -1, 0, 1.1 }, { -1.1, 1, -1.7, 0 }, { -1.1, 1,
			1, 1 }, { -0.4, -0.4, -0.5, -0.5 }, { -0.5, -0.4, -0.4, 0.4 } };

	for (unsigned int i = 0;
			i < sizeof(unfeasiblePoints) / sizeof(unfeasiblePoints[0]); i++) {
		ASSERT(!sq->isFeasible(unfeasiblePoints[i]),
				"Unfeasible Point %d: %f %f %f %f\n",
				i, unfeasiblePoints[i][0], unfeasiblePoints[i][1], unfeasiblePoints[i][2], unfeasiblePoints[i][3]);
	}

	int feasibleIndex[][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 20 },
			{ 20, 20, 20, 20 }, { 5, 5, 5, 5 }, { 15, 15, 15, 15 }, { 14, 15,
					15, 15 }, { 0, 5, 15, 20 } };

	for (unsigned int i = 0;
			i < sizeof(feasibleIndex) / sizeof(feasibleIndex[0]); i++) {
		ASSERT(sq->isFeasible(feasibleIndex[i]),
				"Feasible Index %d: %d %d %d %d\n",
				i, feasibleIndex[i][0], feasibleIndex[i][1], feasibleIndex[i][2], feasibleIndex[i][3]);
	}

	int unfeasibleIndex[][4] = { { 6, 6, 0, 0 }, { 6, 7, 0, 20 }, { 20, 20, 6,
			14 }, { 15, 15, 14, 14 }, { 12, 14, 15, 20 } };

	for (unsigned int i = 0;
			i < sizeof(unfeasibleIndex) / sizeof(unfeasibleIndex[0]); i++) {
		ASSERT(!sq->isFeasible(unfeasibleIndex[i]),
				"Unfeasible Index %d: %d %d %d %d\n",
				i, unfeasibleIndex[i][0], unfeasibleIndex[i][1], unfeasibleIndex[i][2], unfeasibleIndex[i][3]);
	}

	delete sq;
}

void test_SquareDomainRoundHole_isFeasible() {
	printf("==== test_SquareDomainRoundHole_isFeasible() in %s:%d\n", __FILE__,
			__LINE__);
	SquareDomainRoundHole sq(-2, 2, 0, 0, 1, 20);

	State unfeasiblePoints[][STATE_SPACE_DIMENSION] = { { -2.2, -2, 2, 2 }, {
			-2, -2, 3, 2 },
	/* Now some points that are clearly inside the hole */
	{ 0, 0, 2, 2 }, { 1, 1, 0, 0.5 },
	/* The next point is on the numerical border and rendered illegal as it's rounded cell point is inside the whole */
	{ -2, -2, cos(M_PI_4), sin(M_PI_4) }, { 1, 1, 0.9, 0 }, { 1, 1, 0, -0.9 }, {
			-2, -2, cos(M_PI_4), -sin(M_PI_4) } };

	for (unsigned int i = 0; i < sizeof(unfeasiblePoints) / sizeof(unfeasiblePoints[0]);
			i++) {
		ASSERT(!sq.isFeasible(unfeasiblePoints[i]),
				"Unfeasible Point %d: %f %f %f %f\n",
				i, unfeasiblePoints[i][0], unfeasiblePoints[i][1], unfeasiblePoints[i][2], unfeasiblePoints[i][3]);
	}

	State feasiblePoints[][STATE_SPACE_DIMENSION] = { /* Points directly at the boarder are valid. */
	{ 1.15789474, 0, 0, -1.15789474 }, { -1.15789474, 0, 0, 1.15789474 }, /* And points a little futher away from the circle are also valid. */
	{ -2, -2, (1 + 1 * sq.getMaximalResolutionWidth()) * cos(M_PI_4), (1
			+ 1 * sq.getMaximalResolutionWidth()) * sin(M_PI_4) } };

	for (unsigned int i = 0; i < sizeof(feasiblePoints) / sizeof(feasiblePoints[0]);
			i++) {
		ASSERT(sq.isFeasible(feasiblePoints[i]),
				"Feasible Point %d: %f %f %f %f\n",
				i, feasiblePoints[i][0], feasiblePoints[i][1], feasiblePoints[i][2], feasiblePoints[i][3]);
	}
}

void test_SquareDomain_convexCombination_index() {
	printf("==== test_SquareDomain_convexCombination_index() in %s:%d\n",
			__FILE__, __LINE__);

	SquareDomain *sq = new SquareDomain(-2, 2, 10);
	State s[] = { 0.1, 0.9, -0.5, -1.0 };

	LinearCombination *lc = sq->convexDecomposition(s);
	for (int i = 0; i < LINEAR_COMBINATON_LENGTH; i++) {
		State *corner = lc->getPoint(i);
		if (corner != NULL) {
			GridPointIndex *cornerIndex = sq->getGridIndex(corner);
			for (int j = 0; j < STATE_SPACE_DIMENSION; j++) {
				ASSERT_EQUALS(cornerIndex[j], lc->getIndex(i)[j], 1e-3,
						"%d %d: %d %d\n",
						i, j, cornerIndex[j], lc->getIndex(i)[j]);
			}
		}
	}

	delete lc;
	delete sq;
}

void test_SquareDomain_convexCombination() {
	cout << "==== test_SquareDomain_convexCombination()" << endl;

	SquareDomain *sq = new SquareDomain(-1, 1, 2);

	State p0[] = { 0.0, 0.0, 0.0, 0.0 };
	LinearCombination *lc = sq->convexDecomposition(p0);
	State *actual = lc->eval();
	ASSERT(SquareDomain::equals(p0, actual, 1e-9), "p0\n");
	delete[] actual;
	delete lc;

	State p1[] = { 0.1, 0.9, -0.5, -1.0 };
	lc = sq->convexDecomposition(p1);
	actual = lc->eval();
	//	cout << "LinComb of P1: ";
	//	util_print_LinearCombination(lc);
	//	cout << "Actual of P1: ";
	//	util_print_State(actual);
	ASSERT(SquareDomain::equals(p1, actual, 1e-9), "p1\n");
	delete[] actual;
	delete lc;

	State p2[] = { 1.0, 1.0, 1.0, 1.0 };
	lc = sq->convexDecomposition(p2);
	actual = lc->eval();
	//	cout << "LinComb of P2: ";
	//	util_print_LinearCombination(lc);
	//	cout << "Actual of P2: ";
	//	util_print_State(actual);
	ASSERT(SquareDomain::equals(p2, actual, 1e-9), "p2\n");
	delete[] actual;
	delete lc;

	State p3[] = { -1.0, -1.0, -1.0, -1.0 };
	lc = sq->convexDecomposition(p3);
	actual = lc->eval();
	//	cout << "LinComb of P3: ";
	//	util_print_LinearCombination(lc);
	//	cout << "Actual of P3: ";
	//	util_print_State(actual);
	ASSERT(SquareDomain::equals(p3, actual, 1e-9), "p3\n");
	delete[] actual;
	delete lc;

	State p4[] = { 0.5, 0.5, 0.5, 0.5 };
	lc = sq->convexDecomposition(p4);
	actual = lc->eval();
	//	cout << "LinComb of P4: ";
	//	util_print_LinearCombination(lc);
	//	cout << "Actual of P4: ";
	//	util_print_State(actual);
	ASSERT(SquareDomain::equals(p4, actual, 1e-9), "p4\n");
	delete[] actual;
	delete lc;

	delete sq;
}

void test_SquareDomain_getGridIndex() {
	cout << "==== test_SquareDomain_getGridIndex()" << endl;

	SquareDomain *sq = new SquareDomain(-1, 1, 21);

	State points[6][4] = { { 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.1, 0.0 }, {
			0.0, 0.0, 0.1, 0.1 }, { 1.0, 0.5, 0.0, 1.0 }, { -1.0, -1.0, -0.5,
			-0.2 }, { -1.0, -1.0, -1.0, -1.0 } };

	int index[6][4] = { { 10, 10, 10, 10 }, { 10, 10, 11, 10 },
			{ 10, 10, 11, 11 }, { 20, 15, 10, 20 }, { 0, 0, 5, 8 },
			{ 0, 0, 0, 0 } };

	for (unsigned int i = 0; i < sizeof(points) / sizeof(points[0]); i++) {
		int *ret = sq->getGridIndex(points[i]);
		for (int j = 0; j < 4; j++) {
			ASSERT(ret[j] == index[i][j], "Index %d %d\n", i, j);
		}
		delete[] ret;
	}

	delete sq;
}

void test_SqaureDomain_getMaximalGridIndex() {
	cout << "==== test_SquareDomain_getMaximalGridIndex()" << endl;

	int param[] = { 10, 20, 2 };

	GridPointIndex *ind;
	SquareDomain *sq;

	for (unsigned int outer = 0; outer < (sizeof(param) / sizeof(int));
			outer++) {
		sq = new SquareDomain(0, 1, param[outer]);
		ind = sq->getMaximalGridIndex();
		for (int i = 0; i < STATE_SPACE_DIMENSION; i++)
			ASSERT_EQUALS(param[outer] - 1, ind[i], 1e-9, "Grid Index %d\n",
					outer * 10 + i);
		delete sq;
	}

}

void test_SquareDomain_getPoint() {
	cout << "==== test_SquareDomain_getPoint()" << endl;

	SquareDomain *sq = new SquareDomain(0, 1, 11);

	int c0[] = { 0, 0, 0, 0 };
	State p0[] = { 0.0, 0.0, 0.0, 0.0 };
	State *s0 = sq->getPoint(c0);
	ASSERT(GameStateDomain::equals(p0, s0), "p0 == s0\n");
	delete[] s0;

	State p1[] = { 0.5, 0.4, 0.3, 0.0 };
	int c1[] = { 5, 4, 3, 0 };
	State *s1 = sq->getPoint(c1);
	ASSERT(GameStateDomain::equals(p1, s1, 1e-9), "p1 == s1\n");
	delete[] s1;

	State p2[] = { 0.1, 0.1, 0.1, 0.0 };
	int c2[] = { 1, 1, 1, 0 };
	State *s2 = sq->getPoint(c2);
	ASSERT(GameStateDomain::equals(p2, s2, 1e-9), "p2 == s2\n");
	delete[] s2;

	delete sq;

	sq = new SquareDomain(-1, 1, 11);
	State p3[] = { -1.0, -1.0, -1.0, -1.0 };
	State *s3 = sq->getPoint(c0);
	ASSERT(GameStateDomain::equals(p3, s3), "p3 == s3\n");
	delete[] s3;

	State p4[] = { -0.8, -0.8, -0.8, -1.0 };
	State *s4 = sq->getPoint(c2);
	ASSERT(GameStateDomain::equals(p4, s4, 1e-9), "p4 == s4\n");
	delete[] s4;

	delete sq;

}

void test_SquareDomain_isGridPoint() {
	cout << "==== test_SquareDomain_isGridPoint()" << endl;

	SquareDomain *sq = new SquareDomain(0, 1, 11);

	State onGrid[5][4] = { { 0.0001, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.1, 0.0 }, {
			0.0, 0.0001, 0.1, 0.1 }, { 0.0, 0.5, 1.0, 1.0 }, { 1.0, 0.99999,
			1.0, 1.0 } };
	State offGrid[5][4] = { { 0.02, 0.0, 0.0, 0.0 }, { 0.0, 0.0, -0.1, 0.0 }, {
			0.0, 0.0, 0.1, 0.87 }, { 0.0, 0.55, 1.0, 1.0 },
			{ 1.0, 1.0, 1.1, 1.0 } };

	for (int i = 0; i < 5; i++)
		ASSERT(sq->isGridPoint(onGrid[i]), "Point On Grid %d\n", i);

	for (int i = 0; i < 5; i++)
		ASSERT(!sq->isGridPoint(offGrid[i]), "Point Off Grid %d\n", i);

	delete sq;
}

void test_SquareDomain_isTerminal() {
	/*
	 * TODO: Fix Tests!
	 */
//	cout << "==== test_SquareDomain_isTerminal()" << endl;
//
//	SquareDomain *sq = new SquareDomain(-1, 1, 20);
//
//	State terminal[3][4] = { { 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.1, 0.0 }, {
//			0.0, 0.0, 0.1, 0.1 } };
//
//	for (int i = 0; i < 3; i++)
//		ASSERT(sq->isTerminal(terminal[i]), "Point %d\n", i);
//
//	State notTerminal[2][4] =
//			{ { 0.0, 0.0, 0.2, 0.0 }, { 0.5, 0.5, 0.6, 0.7 } };
//
//	for (int i = 0; i < 2; i++)
//		ASSERT(!sq->isTerminal(notTerminal[i]), "Point %d\n", i);
//
//	delete sq;
}

void test_ValueFunction_getValue() {
	cout << "==== test_ValueFunction_getValue" << endl;

	int n = 10;
	SquareDomain *sq = new SquareDomain(0, 1, n + 1);
	int index[4] = { 0, 0, 0, 0 };
	ValueFunction *v = new ValueFunction(sq);

	// New ValueFunctions must be equal to the zero function.
	for (int i0 = 0; i0 <= n; i0++) {
		index[0] = i0;
		for (int i1 = 0; i1 <= n; i1++) {
			index[1] = i1;
			for (int i2 = 0; i2 <= n; i2++) {
				index[2] = i2;
				for (int i3 = 0; i3 <= n; i3++) {
					index[3] = i3;

					assert_equals_valueAtGridIndex(VALUE_UNDEFINED,
							v->getValue(index), index);
				}
			}
		}
	}

	delete v;
	delete sq;
}

void test_ValueFunction_getValue_altered() {
	cout << "==== test_ValueFunction_getValue_altered" << endl;

	int n = 10;
	SquareDomain *sq = new SquareDomain(0, 1, n);
	int index[4] = { 0, 0, 0, 0 };
	ValueFunction *v = new ValueFunction(sq);

	// Change a single value and see if it's still working out as expected.
	int indexx[6][4] = { { 0, 0, 0, 0 }, { 0, 0, 5, 8 }, { 2, 5, 10, 0 }, { 4,
			10, 8, 10 }, { 10, 1, 3, 1 }, { 10, 10, 10, 10 } };

	for (int i = 0; i < 6; i++) {
		v->setValue(indexx[i], i);
	}

	// New ValueFunctions must be equal to the zero function.
	for (int i0 = 0; i0 <= n; i0++) {
		index[0] = i0;
		for (int i1 = 0; i1 <= n; i1++) {
			index[1] = i1;
			for (int i2 = 0; i2 <= n; i2++) {
				index[2] = i2;
				for (int i3 = 0; i3 <= n; i3++) {
					index[3] = i3;

					if (i0 == 0 && i1 == 0 && i2 == 0 && i3 == 0)
						assert_equals_valueAtGridIndex(0, v->getValue(index),
								index);
					else if (i0 == 0 && i1 == 0 && i2 == 5 && i3 == 8)
						assert_equals_valueAtGridIndex(1, v->getValue(index),
								index);
					else if (i0 == 2 && i1 == 5 && i2 == 10 && i3 == 0)
						assert_equals_valueAtGridIndex(2, v->getValue(index),
								index);
					else if (i0 == 4 && i1 == 10 && i2 == 8 && i3 == 10)
						assert_equals_valueAtGridIndex(3, v->getValue(index),
								index);
					else if (i0 == 10 && i1 == 1 && i2 == 3 && i3 == 1)
						assert_equals_valueAtGridIndex(4, v->getValue(index),
								index);
					else if (i0 == 10 && i1 == 10 && i2 == 10 && i3 == 10)
						assert_equals_valueAtGridIndex(5, v->getValue(index),
								index);
					else
						assert_equals_valueAtGridIndex(VALUE_UNDEFINED,
								v->getValue(index), index);
				}
			}
		}
	}

	delete v;
	delete sq;
}

void test_ValueFunction_eval() {
	cout << "==== test_ValueFunction_eval" << endl;

	int n = 10;
	SquareDomain *sq = new SquareDomain(0, 1, n);
	int index[4] = { 0, 0, 0, 0 };
	ValueFunction *v = new ValueFunction(sq);

	// New ValueFunctions must be equal to the zero function.
	for (int i0 = 0; i0 <= n; i0++) {
		index[0] = i0;
		for (int i1 = 0; i1 <= n; i1++) {
			index[1] = i1;
			for (int i2 = 0; i2 <= n; i2++) {
				index[2] = i2;
				for (int i3 = 0; i3 <= n; i3++) {
					index[3] = i3;

					v->setValue(index, 0);
				}
			}
		}
	}

	for (int i = 0; i < 4; i++)
		index[i] = 0;

	State p[3][4] = { { 0.5, 0.5, 0.5, 0.5 }, { 0, 0, 0, 0 }, { 0.01, 0.01, 0,
			0 } };

	ASSERT_EQUALS(0, v->eval(p[0]), 1e-9, "p0\n");

	v->setValue(index, 1);
	ASSERT_EQUALS(1, v->eval(p[1]), 1e-9, "p1\n");
	ASSERT_EQUALS(0, v->eval(p[0]), 1e-9, "p2\n");
	ASSERT_EQUALS(0.81, v->eval(p[2]), 1e-9, "p3\n");

	delete v;
	delete sq;
}

void test_ValueFunction_toArrayIndex() {
	cout << "==== test_ValueFunction_toArrayIndex()" << endl;

	unsigned long counter = 0;
	int n = 11;
	int maxIndex[] = { 11, 11, 11, 11 };
	int index[] = { 0, 0, 0, 0 };

	for (int i0 = 0; i0 <= n; i0++) {
		index[0] = i0;
		for (int i1 = 0; i1 <= n; i1++) {
			index[1] = i1;
			for (int i2 = 0; i2 <= n; i2++) {
				index[2] = i2;
				for (int i3 = 0; i3 <= n; i3++) {
					index[3] = i3;

					// The next few lines are the algorithm to determine the
					// array index!
					// Return value is stored in variable return.
					unsigned long ret = 0;
					unsigned long multiple = 1;
					for (int i = STATE_SPACE_DIMENSION - 1; i >= 0; i--) {
						ret += index[i] * multiple;
						multiple *= (maxIndex[i] + 1);
					}

					ASSERT_EQUALS(counter, ret, 1e-1, "Counter %lu\n", counter);

					counter++;
				}
			}
		}
	}
}
#endif /* DEBUG */
