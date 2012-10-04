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
 * ValueFunction.cpp
 *
 *  Created on: 2011-08-08
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

#include "ValueFunction.h"
#include "Debug.h"
#if DEBUG
#include "Tests.h"
#endif

#include <iostream>
#include <fstream>
#include <string>

namespace bsc {

ValueFunction::ValueFunction(GameStateDomain *domain) {
	initialize(domain);
}

ValueFunction::ValueFunction(GameStateDomain *domain, std::string inFile) {
	this->domain = domain;
	iterations = -1;

	std::ifstream inFileStream(inFile.c_str());

#if OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY
#if STATE_SPACE_DIMENSION != 4
#error "Only for the 4-dimensional state space"
#endif

	GridPointIndex *index = domain->getMaximalGridIndex();
	int tmpSize = -1;
	for (int i = 0; i < STATE_SPACE_DIMENSION; i++) {
		inFileStream >> tmpSize;
		ASSERT_EQUALS(index[i] + 1, tmpSize, 1e-1,
				"Different size in dimension %d. Expected %d, Actual %d",
				i, index[i], tmpSize);
	}

	values = new decimal***[index[0] + 1];
	for (int i0 = 0; i0 < index[0] + 1; i0++) {
		values[i0] = new decimal**[index[1] + 1];

		for (int i1 = 0; i1 < index[1] + 1; i1++) {
			values[i0][i1] = new decimal*[index[2] + 1];

			for (int i2 = 0; i2 < index[2] + 1; i2++) {
				values[i0][i1][i2] = new decimal[index[3] + 1];

				for (int i3 = 0; i3 < index[3] + 1; i3++) {
					inFileStream >> values[i0][i1][i2][i3];
				}
			}
		}
	}

#else
	inFileStream >> size;
	values = new decimal[size];
	for (unsigned long i = 0; i < size; i++)
		inFileStream >> values[i];
#endif /* OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY */
	inFileStream.close();
}

ValueFunction::~ValueFunction() {
	delete[] values;
}

decimal ValueFunction::eval(State *x) {
	return eval(x, false);
}

decimal ValueFunction::eval(State *x, bool timeValue) {
	ASSERT(getDomain()->isFeasible(x), "%s:%d\n", __FILE__, __LINE__);

	LinearCombination *lc = getDomain()->convexDecomposition(x);

#if DEBUG
	util_print_State(x);
	util_print_LinearCombination(lc);
	for(int i = 0; i < lc->getSize(); i++) {
		if(lc->getIndex(i) != NULL) {
			printf("%f \t* %f, %f, %f, %f: \t %d, %d, %d, %d", lc->getFactor(i), lc->getPoint(i)[0], lc->getPoint(i)[1], lc->getPoint(i)[2], lc->getPoint(i)[3], lc->getIndex(i)[0], lc->getIndex(i)[1], lc->getIndex(i)[2], lc->getIndex(i)[3]);
		}
	}
#endif

	decimal ret = 0;
	for (int i = 0; i < lc->getSize(); i++) {
		if (lc->getPoint(i) != NULL) {
			ret += lc->getFactor(i) * getValue(lc->getIndex(i));
		}
	}
	delete lc;

	if(timeValue) {
		ret = -log(1 - ret);
	}

	return ret;
}

GameStateDomain *ValueFunction::getDomain() {
	return domain;
}

int ValueFunction::getIterations() {
	return iterations;
}

#if !OPTIMIZE_VALUE_FUNCTION_INLINE_GETTER_SETTER
decimal ValueFunction::getValue(GridPointIndex *index) {
#if OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY
#if STATE_SPACE_DIMENSION != 4
#error "Only for the 4-dimensional state space"
#endif

	return values[index[0]][index[1]][index[2]][index[3]];

#else
	return values[toArrayIndex(index)];
#endif /* OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY */
}
#endif

void ValueFunction::initialize(GameStateDomain *domain) {
	this->domain = domain;
	iterations = -1;

#if OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY
#if STATE_SPACE_DIMENSION != 4
#error "Only for the 4-dimensional state space"
#endif

	GridPointIndex *index = domain->getMaximalGridIndex();
	GridPointIndex cur[STATE_SPACE_DIMENSION];
	values = new decimal***[index[0] + 1];
	for (int i0 = 0; i0 < index[0] + 1; i0++) {
		values[i0] = new decimal**[index[1] + 1];
		cur[0] = i0;

		for (int i1 = 0; i1 < index[1] + 1; i1++) {
			values[i0][i1] = new decimal*[index[2] + 1];
			cur[1] = i1;

			for (int i2 = 0; i2 < index[2] + 1; i2++) {
				values[i0][i1][i2] = new decimal[index[3] + 1];
				cur[2] = i2;

				for (int i3 = 0; i3 < index[3] + 1; i3++) {
					cur[3] = i3;
					if(domain->isFeasible(cur))
						values[i0][i1][i2][i3] = VALUE_FUNCTION_INITIAL;
					else
						values[i0][i1][i2][i3] = VALUE_UNDEFINED;
				}
			}
		}
	}

#else
	// Initialize Value Tensor.
	GridPointIndex *index = domain->getMaximalGridIndex();
	size = toArrayIndex(index) + 1;
	values = new decimal[size];
	for (unsigned long i = 0; i < size; i++)
		values[i] = VALUE_FUNCTION_INITIAL;
#endif /* OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY */
}

void ValueFunction::persist(std::string filename) {
	std::ofstream outFile(filename.c_str());

#if OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY
#if STATE_SPACE_DIMENSION != 4
#error "Only for the 4-dimensional state space"
#endif

	GridPointIndex *index = domain->getMaximalGridIndex();
	outFile << index[0] + 1 << std::endl;
	outFile << index[1] + 1 << std::endl;
	outFile << index[2] + 1 << std::endl;
	outFile << index[3] + 1 << std::endl;

	for (int i0 = 0; i0 < index[0] + 1; i0++) {
		for (int i1 = 0; i1 < index[1] + 1; i1++) {
			for (int i2 = 0; i2 < index[2]+ 1 ; i2++) {
				for (int i3 = 0; i3 < index[3] + 1; i3++) {
					outFile << values[i0][i1][i2][i3] << std::endl;
				}
			}
		}
	}

#else
	outFile << size << std::endl;
	for (unsigned long i = 0; i < size; i++)
		outFile << values[i] << std::endl;
#endif /* OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY */

	outFile.close();
}

void ValueFunction::print() {
#if OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY
#if STATE_SPACE_DIMENSION != 4
#error "Only for the 4-dimensional state space"
#endif

	GridPointIndex *index = domain->getMaximalGridIndex();
	for (int i0 = 0; i0 < index[0] + 1; i0++) {
		for (int i1 = 0; i1 < index[1] + 1; i1++) {
			for (int i2 = 0; i2 < index[2] + 1; i2++) {
				for (int i3 = 0; i3 < index[3] + 1; i3++) {
					std::cout << values[i0][i1][i2][i3] << ";";
				}
			}
		}
	}

#else
	for (unsigned long i = 0; i < size - 1; i++)
	std::cout << values[i] << ";";

	std::cout << values[size - 1] << std::endl;
#endif
}

void ValueFunction::setIterations(unsigned int iterations) {
	this->iterations = iterations;
}

#if !OPTIMIZE_VALUE_FUNCTION_INLINE_GETTER_SETTER
void ValueFunction::setValue(GridPointIndex *index, decimal value) {
#if OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY
#if STATE_SPACE_DIMENSION != 4
#error "Only for the 4-dimensional state space"
#endif

	values[index[0]][index[1]][index[2]][index[3]] = value;

#else
	values[toArrayIndex(index)] = value;
#endif /* OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY */
}
#endif

#if !OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY
unsigned long ValueFunction::toArrayIndex(GridPointIndex *index) {
	unsigned long ret = 0;
	unsigned long multiple = 1;
	GridPointIndex *maxIndex = domain->getMaximalGridIndex();
	for (int i = STATE_SPACE_DIMENSION - 1; i >= 0; i--) {
		ret += index[i] * multiple;
		multiple *= (maxIndex[i] + 1);
	}
	return ret;
}
#endif /* OPTIMIZE_VALUE_FUNCTION_4DIM_ARRAY */

void ValueFunction::toGnuPlot(std::ofstream *commandStream,
		std::ofstream *dataStream, std::string dataFileName, Player p,
		State *x, bool timePlot) {
#if STATE_SPACE_DIMENSION != 4
// XXX: Only for the 4-dim Domain!
#error "Only for the 4-dim Domain!"
#endif

	// Set the style and stuff
	domain->toGnuPlot(commandStream);
	*commandStream << "set style data lines" << std::endl;
	*commandStream << "set contour base" << std::endl;
	*commandStream << "splot '" << dataFileName << "'" << std::endl;

	// Output the values to the datafile.
	int innerBound, outerBound, index1, index2;
	if (p == Evader) {
		index1 = 0;
		index2 = 1;
	} else {
		index1 = 2;
		index2 = 3;
	}

	GridPointIndex *maxIndex = domain->getMaximalGridIndex();
	innerBound = maxIndex[index1];
	outerBound = maxIndex[index2];

	GridPointIndex *index = domain->getGridIndex(x);
	for (int i = 0; i <= outerBound; i++) {
		for (int j = 0; j <= innerBound; j++) {

			index[index1] = j;
			index[index2] = i;

			State *coord = domain->getPoint(index);
			if (p == Pursuer) {
				coord[0] = x[0];
				coord[1] = x[1];
			} else {
				coord[2] = x[2];
				coord[3] = x[3];
			}

			*dataStream << coord[index1] << "\t" << coord[index2] << "\t"
					<< eval(coord, timePlot);
			*dataStream << std::endl;

			delete[] coord;
		}
		*dataStream << std::endl;
	}
}

void ValueFunction::toGnuPlot(std::string commandFile, std::string dataFile,
		Player p, State *x, bool timePlot) {
	std::ofstream commandStream(commandFile.c_str());
	std::ofstream dataStream(dataFile.c_str());
	toGnuPlot(&commandStream, &dataStream, dataFile, p, x, timePlot);
	commandStream.close();
	dataStream.close();
}

}
