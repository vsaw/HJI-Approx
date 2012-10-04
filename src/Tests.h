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
 * Tests.h
 *
 *  Created on: 2012-07-28
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#ifndef TESTS_H_
#define TESTS_H_

#include <iostream>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "LinearCombination.h"
#include "GameStateDomain.h"
#include "SquareDomain.h"
#include "SquareDomainSquareHole.h"
#include "ValueFunction.h"
#include "Game.h"
#include "Debug.h"

using namespace std;
using namespace bsc;

void profile_ValueFunction_eval();

void profile_Game_computeValueFunction();

void test_GameStateDomain_clonePoint();

void test_GameStateDomain_equals();

void test_GameStateDomain_eval();

void test_LinearCombination_creation();

void test_SquareDomainSquareHole_convexCombination();

void test_SquareDomainSquareHole_getGridIndex();

void test_SquareDomain_convexCombination_index();

void test_SquareDomainSquareHole_isFeasible();

void test_SquareDomainRoundHole_isFeasible();

void test_SquareDomain_convexCombination();

void test_SquareDomain_getGridIndex();

void test_SqaureDomain_getMaximalGridIndex();

void test_SquareDomain_getPoint();

void test_SquareDomain_isGridPoint();

void test_SquareDomain_isTerminal();

void test_ValueFunction_getValue();

void test_ValueFunction_getValue_altered();

void test_ValueFunction_eval();

void test_ValueFunction_toArrayIndex();

#endif /* TESTS_H_ */
