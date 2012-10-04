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
 * Debug.h
 *
 *  Created on: 2012-05-28
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>
#include <stdio.h>

#include "Definitions.h"
#include "ValueFunction.h"

using namespace std;
using namespace bsc;

#define ASSERT_EQUALS(expected, actual, eps, message, ...) ASSERT(fabs(expected - actual) <= eps, message, ##__VA_ARGS__)

#define ASSERT_NOT_NULL(ptr, message, ...) ASSERT(ptr != NULL, message, ##__VA_ARGS__)

#define ASSERT_NULL(ptr, message, ...) ASSERT(ptr == NULL, message, ##__VA_ARGS__)

#ifdef DEBUG
#define ASSERT(cond, message, ...) if(!(cond)) { printf(message, ##__VA_ARGS__); }
#else /* DEBUG */
#define ASSERT(cond, message, ...)
#endif /* DEBUG */

void assert_equals_valueAtGridIndex(int expected, int actual,
		GridPointIndex *index);

void assert_ValueFuntion_isSymmetrical(ValueFunction *vfunc);

void util_print_LinearCombination(LinearCombination *lc);

void util_print_State(State *s);
#endif /* DEBUG_H_ */
