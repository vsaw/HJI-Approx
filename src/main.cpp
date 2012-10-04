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
 * main.cpp
 *
 *  Created on: 2011-07-26
 *      Author: Valentin Sawadski <valentin.sawadski@mytum.de>
 */

/*! \mainpage Implementation of Fully-Discrete Schemes for the Value Function of Pursuit-Evasion Games with State Constraints.
 *
 * This is the in depth documentation of the code for the Bachelors Thesis of
 * Valentin Sawadski.
 *
 * If compiled. The experiments can be started with the following command:
 *
 * \code
 * BscThesis x y z
 * \endcode
 *
 * where x, y and z specify the number of the experiment to be started.
 *
 * The following experiments are implemented:
 * - 0 0 1: [-2,2]^4, n = 14, n_c = 8 + 1, Vp = 2, Ve = 1, eps = 1e-3
 * - 0 0 2: [-2,2]^4, n = 20, n_c = 20 + 1, Vp = 1, Ve = 1, eps = 1e-3
 * - 0 0 3: [-2,2]^4 \ [-0.5, 0.5]^4, n = 20, n_c = 20 + 1, Vp = 2, Ve = 1, eps = 1e-5
 * - 0 0 4: [-2,2]^4, n = 20, n_c = 21 + 1, Vp = 2, Ve = 1, eps = 1e-3
 * - 0 0 5: [-2,2]^4, n = 24, n_c = 48 + 1, Vp = 2, Ve = 1, eps = 1e-3
 * - 0 0 6: [-2,2]^4, n = 14, n_c = 20 + 1, Vp = 1, Ve = 1.25, eps = 1e-3
 * - 0 0 7: [-2,2]^4 \ Ball(Center=0, rad=0.5), n = 16, n_c = 20 + 1, Vp = 2, Ve = 1, eps = 1e-5
 * - 1 1 0: [-2,2]^4, n = 50, n_c = 48 + 1, Vp = 2, Ve = 1, eps = 1e-3
 * - 1 1 1: [-2,2]^4, n = 50, n_c = 48 + 1, Vp = 2, Ve = 1, eps = 1e-3
 * - 1 2 0: [-2,2]^4, n = 26, n_c = 36 + 1, Vp = 2, Ve = 1, eps = 1e-5
 * - 1 3 0: [-2,2]^4 \ [-0.53, 0.53]^4, n = 50, n_c = 48 + 1, Vp = 2, Ve = 1, eps = 1e-4
 * - 1 4 0: [-2,2]^4 \ Ball(Center=0, rad=7 * delta x), n = 50, n_c = 48 + 1, Vp = 2, Ve = 1, eps = 1e-4
 * - 2 5 0: [-2,2]^4, n = 50, n_c = 36, Vp = 1, Ve = 1, eps = 1e-3
 * - 2 6 0: [-2,2]^4 \ Ball(Center=0, rad=7 * delta x), n = 50, n_c = 48 + 1, Vp = 1, Ve = 1, eps = 1e-4
 * - 2 7 0: [-2,2]^4, n = 50, n_c = 48 + 1, Vp = 1, Ve = 1.25, eps = 1e-3
 * - 2 8 0: [-2,2]^4, n = 50, n_c = 36, Vp = 1, Ve = 1.25, eps = 1e-3
 */

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
#include "Tests.h"
#include "SquareDomainRoundHole.h"

using namespace std;
using namespace bsc;

bool util_file_exists(string file) {
	FILE* fp = NULL;

	//will not work if you do not have read permissions
	//to the file, but if you don't have read, it
	//may as well not exist to begin with.
	fp = fopen(file.c_str(), "rb");
	if (fp != NULL) {
		fclose(fp);
		return true;
	}

	return false;
}

void experiment_0_0_1() {
	printf("==== experiment_0_0_1()\n");

	decimal eps = 1e-3;

	SquareDomain *sq = new SquareDomain(-2, 2, 14);
	Game *g = new Game();

	g->setDomain(sq);
	g->setControlResolution(8);
	g->setVelocity(Pursuer, 2);

	cout << sq->toString() << endl;
	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << eps << endl;
	printf("max Iterations: %d\n", g->getMaxIterations());
	printf("max Time Steps for Trajectory Plots: %d\n", g->getMaxTimeSteps());

	ValueFunction *vfunc;
	if (util_file_exists("experiment_0_0_1.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_0_0_1.vfunc");
	} else {
		vfunc = g->computeValueFunction(eps);
		cout << "  finished with " << vfunc->getIterations() << " iterations"
				<< endl;
		vfunc->persist("experiment_0_0_1.vfunc");
	}

#if DEBUG
	assert_ValueFuntion_isSymmetrical(vfunc);
#endif

	State x[] = { 0.0, 0.0, 0.0, 0.0 };
	if (!util_file_exists("experiment_0_0_1.cmd")
			|| !util_file_exists("experiment_0_0_1.dat")) {
		vfunc->toGnuPlot("experiment_0_0_1.cmd", "experiment_0_0_1.dat",
				Pursuer, x, true);
	}

	x[0] = -2;
	x[1] = -2;
	printf("Plotting trajectory for state:\n");
	util_print_State(x);
	unsigned int timeSteps = -1;
	if (!util_file_exists("experiment_0_0_1.traj1")) {
		timeSteps = g->plotTrajectories(vfunc, x, "experiment_0_0_1.traj1",
				"experiment_0_0_1.traj1.cmd");
		printf("Time Steps till capture or abort: %d\n", timeSteps);
	}

	x[0] = -1.8;
	x[1] = -1.8;
	x[2] = 0.5;
	x[3] = -1.6;
	printf("Plotting trajectory for state:\n");
	util_print_State(x);
	if (!util_file_exists("experiment_0_0_1.traj2")) {
		timeSteps = g->plotTrajectories(vfunc, x, "experiment_0_0_1.traj2",
				"experiment_0_0_1.traj2.cmd");
		printf("Time Steps till capture or abort: %d\n", timeSteps);
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_0_0_2() {
	cout << "==== experiment_0_0_2()" << endl;

	decimal eps = 1e-3;

	SquareDomain *sq = new SquareDomain(-2, 2, 20);
	Game *g = new Game();

	g->setDomain(sq);
	g->setControlResolution(20);

	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << eps << endl;

	ValueFunction *vfunc;
	if (util_file_exists("experiment_0_0_2.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_0_0_2.vfunc");
	} else {
		vfunc = g->computeValueFunction(eps);
		cout << "  finished with " << vfunc->getIterations() << " iterations"
				<< endl;
		vfunc->persist("experiment_0_0_2.vfunc");
	}

	State x[] = { 0.0, 0.0, 0.0, 0.0 };
	if (!util_file_exists("experiment_0_0_2.cmd")
			|| !util_file_exists("experiment_0_0_2.dat")) {
		vfunc->toGnuPlot("experiment_0_0_2.cmd", "experiment_0_0_2.dat",
				Pursuer, x, true);
	}

	x[0] = -2;
	x[1] = -2;
	if (!util_file_exists("experiment_0_0_2.traj1")) {
		g->plotTrajectories(vfunc, x, "experiment_0_0_2.traj1",
				"experiment_0_0_2.traj1.cmd");
	}

	x[0] = -1.8;
	x[1] = -1.8;
	x[2] = 0.5;
	x[3] = -1.6;
	if (!util_file_exists("experiment_0_0_2.traj2")) {
		g->plotTrajectories(vfunc, x, "experiment_0_0_2.traj2",
				"experiment_0_0_2.traj2.cmd");
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_0_0_3() {
	printf("==== experiment_0_0_3()\n");

	decimal eps = 1e-5;

	SquareDomainSquareHole *sq = new SquareDomainSquareHole(-2, 2, -0.5, 0.5,
			16);
	Game *g = new Game();
	g->setDomain(sq);
	g->setControlResolution(20);
	g->setVelocity(Pursuer, 2);

	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << eps << endl;

	ValueFunction *vfunc;
	if (util_file_exists("experiment_0_0_3.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_0_0_3.vfunc");
	} else {
		vfunc = g->computeValueFunction(eps);
		cout << "  finished with " << vfunc->getIterations() << " iterations"
				<< endl;
		vfunc->persist("experiment_0_0_3.vfunc");
	}

#if DEBUG
	assert_ValueFuntion_isSymmetrical(vfunc);
#endif

	State x[] = { -1.5, -1.5, -1.0, -1.0 };
	if (!util_file_exists("experiment_0_0_3.cmd")
			|| !util_file_exists("experiment_0_0_3.dat")) {
		vfunc->toGnuPlot("experiment_0_0_3.cmd", "experiment_0_0_3.dat",
				Pursuer, x, true);
	}

	x[0] = -2;
	x[1] = -2;
	if (!util_file_exists("experiment_0_0_3.traj1")) {
		g->plotTrajectories(vfunc, x, "experiment_0_0_3.traj1",
				"experiment_0_0_3.traj1.cmd");
	}

	x[0] = -1.8;
	x[1] = -1.8;
	x[2] = 0.5;
	x[3] = -1.6;
	if (!util_file_exists("experiment_0_0_3.cmd1")
			|| !util_file_exists("experiment_0_0_3.dat1")) {
		vfunc->toGnuPlot("experiment_0_0_3.cmd1", "experiment_0_0_3.dat1",
				Pursuer, x, true);
	}

	x[0] = -1.8;
	x[1] = -1.8;
	x[2] = 0.5;
	x[3] = -1.6;
	if (!util_file_exists("experiment_0_0_3.traj2")) {
		g->plotTrajectories(vfunc, x, "experiment_0_0_3.traj2",
				"experiment_0_0_3.traj2.cmd");
	}

	x[0] = -1.9;
	x[1] = -1.9;
	x[2] = 1.9;
	x[3] = 1.9;
	if (!util_file_exists("experiment_0_0_3.traj3")) {
		g->plotTrajectories(vfunc, x, "experiment_0_0_3.traj3",
				"experiment_0_0_3.traj3.cmd");
	}

	if (!util_file_exists("experiment_0_0_3.cmd3")
			|| !util_file_exists("experiment_0_0_3.dat3")) {
		vfunc->toGnuPlot("experiment_0_0_3.cmd3", "experiment_0_0_3.dat3",
				Pursuer, x, true);
	}

	x[0] = -1.9;
	x[1] = 0;
	x[2] = 1;
	x[3] = 0;
	if (!util_file_exists("experiment_0_0_3.traj4")) {
		g->plotTrajectories(vfunc, x, "experiment_0_0_3.traj4",
				"experiment_0_0_3.traj4.cmd");
	}

	if (!util_file_exists("experiment_0_0_3.cmd4")
			|| !util_file_exists("experiment_0_0_3.dat4")) {
		vfunc->toGnuPlot("experiment_0_0_3.cmd4", "experiment_0_0_3.dat4",
				Pursuer, x, true);
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_0_0_4() {
	cout << "==== experiment_0_0_4()" << endl;

	int n = 20;
	int numControls = 20 + 1;
	decimal eps = 1e-3;

	SquareDomain *sq = new SquareDomain(-2, 2, n);
	Game *g = new Game();
	g->setDomain(sq);
	g->setControlResolution(numControls);
	g->setVelocity(Pursuer, 2);

	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << eps << endl;

	ValueFunction *vfunc;
	if (util_file_exists("experiment_0_0_4.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_0_0_4.vfunc");
	} else {
		vfunc = g->computeValueFunction(eps);
		cout << "  finished with " << vfunc->getIterations() << " iterations"
				<< endl;
		vfunc->persist("experiment_0_0_4.vfunc");
	}

	State x[] = { 0.0, 0.0, 0.0, 0.0 };
	if (!util_file_exists("experiment_0_0_4.cmd")
			|| !util_file_exists("experiment_0_0_4.dat")) {
		vfunc->toGnuPlot("experiment_0_0_4.cmd", "experiment_0_0_4.dat",
				Pursuer, x, true);
	}

	x[0] = -2;
	x[1] = -2;
	if (!util_file_exists("experiment_0_0_4.traj1")) {
		g->plotTrajectories(vfunc, x, "experiment_0_0_4.traj1",
				"experiment_0_0_4.traj1.cmd");
	}

	x[0] = -1.8;
	x[1] = -1.8;
	x[2] = 0.5;
	x[3] = -1.6;
	if (!util_file_exists("experiment_0_0_4.traj2")) {
		g->plotTrajectories(vfunc, x, "experiment_0_0_4.traj2",
				"experiment_0_0_4.traj2.cmd");
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_0_0_5() {
	cout << "==== experiment_0_0_5()" << endl;

	SquareDomain *sq = new SquareDomain(-2, 2, 24);
	Game *g = new Game();
	g->setDomain(sq);
	g->setControlResolution(48);
	g->setVelocity(Pursuer, 2);

	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << 1e-3 << endl;

	ValueFunction *vfunc;
	if (util_file_exists("experiment_0_0_5.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_0_0_5.vfunc");
	} else {
		vfunc = g->computeValueFunction(1e-3);
		vfunc->persist("experiment_0_0_5.vfunc");
		cout << "iterations = " << vfunc->getIterations() << endl;
	}

	State x[] = { 0.0, 0.0, 0.0, 0.0 };
	if (!util_file_exists("experiment_0_0_5.cmd")
			|| !util_file_exists("experiment_0_0_5.dat")) {
		vfunc->toGnuPlot("experiment_0_0_5.cmd", "experiment_0_0_5.dat",
				Pursuer, x, true);
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_0_0_6() {
	cout << "==== experiment_0_0_6()" << endl;

	SquareDomain *sq = new SquareDomain(-2, 2, 14);
	Game *g = new Game();
	g->setDomain(sq);
	g->setControlResolution(20);
	g->setVelocity(Evader, 1.25);

	cout << sq->toString() << endl;
	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << 1e-3 << endl;
	printf("max Iterations: %d\n", g->getMaxIterations());
	printf("max Time Steps for Trajectory Plots: %d\n", g->getMaxTimeSteps());

	ValueFunction *vfunc;
	if (util_file_exists("experiment_0_0_6.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_0_0_6.vfunc");
	} else {
		vfunc = g->computeValueFunction(1e-3);
		vfunc->persist("experiment_0_0_6.vfunc");
		cout << "iterations = " << vfunc->getIterations() << endl;
	}

	State x[] = { -1.0, -1.0, 0.0, 0.0 };
	if (!util_file_exists("experiment_0_0_6.cmd")
			|| !util_file_exists("experiment_0_0_6.dat")) {
		vfunc->toGnuPlot("experiment_0_0_6.cmd", "experiment_0_0_6.dat",
				Pursuer, x, true);
	}

	x[0] = -1;
	x[1] = -1;
	x[2] = -1;
	x[3] = 1;
	printf("Plotting trajectory for state:\n");
	util_print_State(x);
	unsigned int timeSteps = -1;
	util_print_State(x);
	if (!util_file_exists("experiment_0_0_6.traj1")) {
		timeSteps = g->plotTrajectories(vfunc, x, "experiment_0_0_6.traj1",
				"experiment_0_0_6.traj1.cmd");
		printf("Time Steps till capture or abort: %d\n", timeSteps);
	}

	x[0] = -1;
	x[1] = -1;
	x[2] = -0.5;
	x[3] = -0.5;
	printf("Plotting trajectory for state:\n");
	util_print_State(x);
	if (!util_file_exists("experiment_0_0_6.traj2")) {
		timeSteps = g->plotTrajectories(vfunc, x, "experiment_0_0_6.traj2",
				"experiment_0_0_6.traj2.cmd");
		printf("Time Steps till capture or abort: %d\n", timeSteps);
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_0_0_7() {
	printf("==== experiment_0_0_7()\n");

	decimal eps = 1e-5;

	SquareDomainRoundHole *sq = new SquareDomainRoundHole(-2, 2, 0, 0, 0.5,
			16);
	Game *g = new Game();
	g->setDomain(sq);
	g->setControlResolution(20);
	g->setVelocity(Pursuer, 2);

	cout << sq->toString() << endl;
	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << 1e-3 << endl;
	printf("max Iterations: %d\n", g->getMaxIterations());
	printf("max Time Steps for Trajectory Plots: %d\n", g->getMaxTimeSteps());

	ValueFunction *vfunc;
	if (util_file_exists("experiment_0_0_7.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_0_0_7.vfunc");
	} else {
		vfunc = g->computeValueFunction(eps);
		cout << "  finished with " << vfunc->getIterations() << " iterations"
				<< endl;
		vfunc->persist("experiment_0_0_7.vfunc");
	}

#if DEBUG
	assert_ValueFuntion_isSymmetrical(vfunc);
#endif

	State x[STATE_SPACE_DIMENSION] = { -1.9, -1.9, 1.9, 1.9 };
	if (!util_file_exists("experiment_0_0_7.cmd")
			|| !util_file_exists("experiment_0_0_7.dat")) {
		vfunc->toGnuPlot("experiment_0_0_7.cmd", "experiment_0_0_7.dat",
				Pursuer, x, true);
	}

	printf("Plotting trajectory for state:\n");
	util_print_State(x);
	unsigned int timeSteps = -1;
	if (!util_file_exists("experiment_0_0_7.traj")) {
		timeSteps = g->plotTrajectories(vfunc, x, "experiment_0_0_7.traj",
				"experiment_0_0_7.traj.cmd");
		printf("Time Steps till capture or abort: %d\n", timeSteps);
	}

	x[0] = -0.6;
	x[1] = 0;
	x[2] = 1;
	x[3] = 0.4;
	if (!util_file_exists("experiment_0_0_7.cmd1")
			|| !util_file_exists("experiment_0_0_7.dat1")) {
		vfunc->toGnuPlot("experiment_0_0_7.cmd1", "experiment_0_0_7.dat1",
				Pursuer, x, true);
	}

	printf("Plotting trajectory for state:\n");
	util_print_State(x);
	if (!util_file_exists("experiment_0_0_7.traj1")) {
		timeSteps = g->plotTrajectories(vfunc, x, "experiment_0_0_7.traj1",
				"experiment_0_0_7.traj1.cmd");
		printf("Time Steps till capture or abort: %d\n", timeSteps);
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_1_1_0() {
	cout << "==== experiment_1_1_0()" << endl;

	SquareDomain *sq = new SquareDomain(-2, 2, 50);
	Game *g = new Game();
	g->setDomain(sq);
	g->setControlResolution(48);
	g->setVelocity(Pursuer, 2);

	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << 1e-3 << endl;

	ValueFunction *vfunc;
	if (util_file_exists("experiment_1_1_0.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_1_1_0.vfunc");
	} else {
		vfunc = g->computeValueFunction(1e-3);
		vfunc->persist("experiment_1_1_0.vfunc");
		cout << "iterations = " << vfunc->getIterations() << endl;
	}

	State x[] = { 0.0, 0.0, 0.0, 0.0 };
	if (!util_file_exists("experiment_1_1_0.cmd")
			|| !util_file_exists("experiment_1_1_0.dat")) {
		vfunc->toGnuPlot("experiment_1_1_0.cmd", "experiment_1_1_0.dat",
				Pursuer, x, true);
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_1_1_1() {
	cout << "==== experiment_1_1_1()" << endl;

	SquareDomain *sq = new SquareDomain(-2, 2, 50);
	Game *g = new Game();

	g->setDomain(sq);
	g->setControlResolution(48);
	g->setVelocity(Pursuer, 2);

	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << 1e-3 << endl;

	ValueFunction *vfunc;
	if (util_file_exists("experiment_1_1_1.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_1_1_1.vfunc");
	} else {
		vfunc = g->computeValueFunction(1e-3);
		vfunc->persist("experiment_1_1_1.vfunc");
		cout << "iterations = " << vfunc->getIterations() << endl;
	}

	State x[] = { -1, 0.0, 0.0, 0.0 };
	if (!util_file_exists("experiment_1_1_1.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_1_1_1.traj",
				"experiment_1_1_1.traj.cmd");
	}

	x[0] = -2;
	x[1] = -2;
	x[2] = 1;
	x[3] = 0.7;
	if (!util_file_exists("experiment_1_1_2.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_1_1_2.traj",
				"experiment_1_1_2.traj.cmd");
	}

	x[0] = -1.8;
	x[1] = -1.8;
	x[2] = 0.5;
	x[3] = -1.6;
	if (!util_file_exists("experiment_1_1_3.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_1_1_3.traj",
				"experiment_1_1_3.traj.cmd");
	}

	x[0] = -1.8;
	x[1] = -1.8;
	x[2] = 0.5;
	x[3] = -1.8;
	if (!util_file_exists("experiment_1_1_4.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_1_1_4.traj",
				"experiment_1_1_4.traj.cmd");
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_1_2_0() {
	cout << "==== experiment_1_2_0()" << endl;

	SquareDomain *sq = new SquareDomain(-2, 2, 26);
	Game *g = new Game();
	g->setDomain(sq);
	g->setControlResolution(36);
	g->setVelocity(Pursuer, 2);

	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << 1e-5 << endl;

	ValueFunction *vfunc;
	if (util_file_exists("experiment_1_2_0.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_1_2_0.vfunc");
	} else {
		vfunc = g->computeValueFunction(1e-5);
		vfunc->persist("experiment_1_2_0.vfunc");
		cout << "iterations = " << vfunc->getIterations() << endl;
	}

	State x[] = { 0.0, 0.0, 0.0, 0.0 };
	if (!util_file_exists("experiment_1_2_0.cmd")
			|| !util_file_exists("experiment_1_2_0.dat")) {
		vfunc->toGnuPlot("experiment_1_2_0.cmd", "experiment_1_2_0.dat",
				Pursuer, x, true);
	}

	x[0] = -1.8;
	x[1] = -1.8;
	x[2] = 0.5;
	x[3] = -1.6;
	printf("Plotting trajectory for state:\n");
	unsigned int timeSteps = -1;
	util_print_State(x);
	if (!util_file_exists("experiment_1_2_0.traj")) {
		timeSteps = g->plotTrajectories(vfunc, x, "experiment_1_2_0.traj",
				"experiment_1_2_0.traj.cmd");
		printf("Time Steps till capture or abort: %d\n", timeSteps);
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_1_3_0() {
	cout << "==== experiment_1_3_0()" << endl;

	SquareDomainSquareHole *sq = new SquareDomainSquareHole(-2, 2, -0.53, 0.53,
			50);
	Game *g = new Game();
	g->setDomain(sq);
	g->setControlResolution(48);
	g->setAllowStandingStill(true);
	g->setVelocity(Pursuer, 2);

	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << 1e-4 << endl;

	ValueFunction *vfunc;
	if (util_file_exists("experiment_1_3_0.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_1_3_0.vfunc");
	} else {
		vfunc = g->computeValueFunction(1e-4);
		vfunc->persist("experiment_1_3_0.vfunc");
		cout << "iterations = " << vfunc->getIterations() << endl;
	}

	State x[] = { -1.5, -1.5, 0.0, 0.0 };
	if (!util_file_exists("experiment_1_3_0.cmd")
			|| !util_file_exists("experiment_1_3_0.dat")) {
		vfunc->toGnuPlot("experiment_1_3_0.cmd", "experiment_1_3_0.dat",
				Pursuer, x, true);
	}

	x[0] = -1.9;
	x[1] = -1.9;
	x[2] = 1.9;
	x[3] = 1.9;
	if (!util_file_exists("experiment_1_3_1.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_1_3_1.traj",
				"experiment_1_3_1.traj.cmd");
	}

	x[0] = -1.9;
	x[1] = 0;
	x[2] = 1;
	x[3] = 0;
	if (!util_file_exists("experiment_1_3_2.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_1_3_2.traj",
				"experiment_1_3_2.traj.cmd");
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_1_4_0() {
	printf("==== experiment_1_4_0()\n");

	decimal eps = 1e-4;

	SquareDomainRoundHole *sq = new SquareDomainRoundHole(-2, 2, 0, 0,
			7.0 * (4.0 / 49.0), 50);
	Game *g = new Game();
	g->setDomain(sq);
	g->setControlResolution(48);
	g->setVelocity(Pursuer, 2);

	cout << sq->toString() << endl;
	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << eps << endl;
	printf("max Iterations: %d\n", g->getMaxIterations());
	printf("max Time Steps for Trajectory Plots: %d\n", g->getMaxTimeSteps());

	ValueFunction *vfunc;
	if (util_file_exists("experiment_1_4_0.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_1_4_0.vfunc");
	} else {
		vfunc = g->computeValueFunction(eps);
		cout << "  finished with " << vfunc->getIterations() << " iterations"
				<< endl;
		vfunc->persist("experiment_1_4_0.vfunc");
	}

#if DEBUG
	assert_ValueFuntion_isSymmetrical(vfunc);
#endif

	State x[STATE_SPACE_DIMENSION] = { -1.9, -1.9, 1.9, 1.9 };
	if (!util_file_exists("experiment_1_4_0.cmd")
			|| !util_file_exists("experiment_1_4_0.dat")) {
		vfunc->toGnuPlot("experiment_1_4_0.cmd", "experiment_1_4_0.dat",
				Pursuer, x, true);
	}

	printf("Plotting trajectory for state:\n");
	util_print_State(x);
	unsigned int timeSteps = -1;
	if (!util_file_exists("experiment_1_4_0.traj")) {
		timeSteps = g->plotTrajectories(vfunc, x, "experiment_1_4_0.traj",
				"experiment_1_4_0.traj.cmd");
		printf("Time Steps till capture or abort: %d\n", timeSteps);
	}

	x[0] = -0.6;
	x[1] = 0;
	x[2] = 1;
	x[3] = 0.4;
	if (!util_file_exists("experiment_1_4_0.cmd1")
			|| !util_file_exists("experiment_1_4_0.dat1")) {
		vfunc->toGnuPlot("experiment_1_4_0.cmd1", "experiment_1_4_0.dat1",
				Pursuer, x, true);
	}

	printf("Plotting trajectory for state:\n");
	util_print_State(x);
	if (!util_file_exists("experiment_1_4_0.traj1")) {
		timeSteps = g->plotTrajectories(vfunc, x, "experiment_1_4_0.traj1",
				"experiment_1_4_0.traj1.cmd");
		printf("Time Steps till capture or abort: %d\n", timeSteps);
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_2_5_0() {
	cout << "==== experiment_2_5_0()" << endl;

	SquareDomain *sq = new SquareDomain(-2, 2, 50);
	Game *g = new Game();
	g->setDomain(sq);
	g->setControlResolution(36);
	g->setAllowStandingStill(false);

	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << 1e-3 << endl;

	ValueFunction *vfunc;
	if (util_file_exists("experiment_2_5_0.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_2_5_0.vfunc");
	} else {
		vfunc = g->computeValueFunction(1e-3);
		vfunc->persist("experiment_2_5_0.vfunc");
		cout << "iterations = " << vfunc->getIterations() << endl;
	}

	State x[] = { 0.0, 0.0, 0.0, 0.0 };
	if (!util_file_exists("experiment_2_5_0_0.cmd")
			|| !util_file_exists("experiment_2_5_0_0.dat")) {
		vfunc->toGnuPlot("experiment_2_5_0_0.cmd", "experiment_2_5_0_0.dat",
				Pursuer, x, true);
	}

	x[0] = 1.15;
	x[1] = 1.15;
	if (!util_file_exists("experiment_2_5_0_1.cmd")
			|| !util_file_exists("experiment_2_5_0_1.dat")) {
		vfunc->toGnuPlot("experiment_2_5_0_1.cmd", "experiment_2_5_0_1.dat",
				Pursuer, x, true);
	}

	x[0] = 0;
	x[1] = 1;
	x[2] = 0;
	x[3] = 0;
	if (!util_file_exists("experiment_2_5_1.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_2_5_1.traj",
				"experiment_2_5_1.traj.cmd");
	}

	x[0] = 1;
	x[1] = 1.5;
	x[2] = -0.5;
	x[3] = 0;
	if (!util_file_exists("experiment_2_5_2.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_2_5_2.traj",
				"experiment_2_5_2.traj.cmd");
	}

	x[0] = 1.3;
	x[1] = 1.8;
	x[2] = 0;
	x[3] = 0;
	if (!util_file_exists("experiment_2_5_3.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_2_5_3.traj",
				"experiment_2_5_3.traj.cmd");
	}

	x[0] = -1.9;
	x[1] = -1.9;
	x[2] = -1.7;
	x[3] = -1.9;
	if (!util_file_exists("experiment_2_5_4.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_2_5_4.traj",
				"experiment_2_5_4.traj.cmd");
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_2_6_0() {
	printf("==== experiment_2_6_0()\n");

	decimal eps = 1e-4;

	SquareDomainRoundHole *sq = new SquareDomainRoundHole(-2, 2, 0, 0,
			7.0 * (4.0 / 49.0), 50);
	Game *g = new Game();
	g->setDomain(sq);
	g->setControlResolution(48);
	g->setMaxTimeSteps(200);

	cout << sq->toString() << endl;
	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << eps << endl;
	printf("max Iterations: %d\n", g->getMaxIterations());
	printf("max Time Steps for Trajectory Plots: %d\n", g->getMaxTimeSteps());

	ValueFunction *vfunc;
	if (util_file_exists("experiment_2_6_0.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_2_6_0.vfunc");
	} else {
		vfunc = g->computeValueFunction(eps);
		cout << "  finished with " << vfunc->getIterations() << " iterations"
				<< endl;
		vfunc->persist("experiment_2_6_0.vfunc");
	}

#if DEBUG
	assert_ValueFuntion_isSymmetrical(vfunc);
#endif

	State x[STATE_SPACE_DIMENSION] = { -1.8, 8, 1.2, 0 };
	if (!util_file_exists("experiment_2_6_0.cmd")
			|| !util_file_exists("experiment_2_6_0.dat")) {
		vfunc->toGnuPlot("experiment_2_6_0.cmd", "experiment_2_6_0.dat",
				Pursuer, x, true);
	}

	if (!util_file_exists("experiment_2_6_0.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_2_6_0.traj",
				"experiment_2_6_0.traj.cmd");
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_2_7_0() {
	cout << "==== experiment_2_7_0()" << endl;

	SquareDomain *sq = new SquareDomain(-2, 2, 50);
	Game *g = new Game();
	g->setDomain(sq);
	g->setControlResolution(48);
	g->setVelocity(Evader, 1.25);

	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << 1e-3 << endl;

	ValueFunction *vfunc;
	if (util_file_exists("experiment_2_7_0.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_2_7_0.vfunc");
	} else {
		vfunc = g->computeValueFunction(1e-3);
		vfunc->persist("experiment_2_7_0.vfunc");
		cout << "iterations = " << vfunc->getIterations() << endl;
	}

	State x[] = { -1.0, -1.0, 0.0, 0.0 };
	if (!util_file_exists("experiment_2_7_0.cmd")
			|| !util_file_exists("experiment_2_7_0.dat")) {
		vfunc->toGnuPlot("experiment_2_7_0.cmd", "experiment_2_7_0.dat",
				Pursuer, x, true);
	}

	x[0] = -1;
	x[1] = -1;
	x[2] = -1;
	x[3] = 1;
	printf("Plotting trajectory for state:\n");
	util_print_State(x);
	if (!util_file_exists("experiment_2_7_1.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_2_7_1.traj",
				"experiment_2_7_1.traj.cmd");
	}

	x[0] = -1;
	x[1] = -1;
	x[2] = -0.5;
	x[3] = -0.5;
	printf("Plotting trajectory for state:\n");
	util_print_State(x);
	if (!util_file_exists("experiment_2_7_2.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_2_7_2.traj",
				"experiment_2_7_2.traj.cmd");
	}

	delete vfunc;
	delete sq;
	delete g;
}

void experiment_2_8_0() {
	cout << "==== experiment_2_8_0()" << endl;

	SquareDomain *sq = new SquareDomain(-2, 2, 50);
	Game *g = new Game();
	g->setAllowStandingStill(false);
	g->setDomain(sq);
	g->setControlResolution(36);
	g->setAllowStandingStill(false);
	g->setVelocity(Evader, 1.5);

	printf("n = %d \nn_c = %d + %d\n", sq->getMaximalGridIndex()[0] + 1,
			g->getControlResolution(), g->getAllowStandingStill());
	printf("vP = %f \nvE = %f\n", g->getVelocity(Pursuer),
			g->getVelocity(Evader));
	cout << "width = " << sq->getMaximalResolutionWidth() << endl;
	cout << "timeStepSize = " << g->getTimeStepSize() << endl;
	cout << "eps = " << 1e-4 << endl;

	ValueFunction *vfunc;
	if (util_file_exists("experiment_2_8_0.vfunc")) {
		vfunc = new ValueFunction(sq, "experiment_2_8_0.vfunc");
	} else {
		vfunc = g->computeValueFunction(1e-4);
		vfunc->persist("experiment_2_8_0.vfunc");
		cout << "iterations = " << vfunc->getIterations() << endl;
	}

	State x[] = { -1.0, -1.0, 0.0, 0.0 };
	if (!util_file_exists("experiment_2_8_0.cmd")
			|| !util_file_exists("experiment_2_8_0.dat")) {
		vfunc->toGnuPlot("experiment_2_8_0.cmd", "experiment_2_8_0.dat",
				Pursuer, x, true);
	}

	x[0] = 0.5;
	x[1] = 0.5;
	x[2] = 1.5;
	x[3] = 1.5;
	if (!util_file_exists("experiment_2_8_1.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_2_8_1.traj",
				"experiment_2_8_1.traj.cmd");
	}

	x[0] = 0;
	x[1] = -0.8;
	x[2] = -0.3;
	x[3] = -1.3;
	if (!util_file_exists("experiment_2_8_2.traj")) {
		g->plotTrajectories(vfunc, x, "experiment_2_8_2.traj",
				"experiment_2_8_2.traj.cmd");
	}

	delete vfunc;
	delete sq;
	delete g;
}

bool isExperiment(char *argv[], int i, int j, int k) {
	if (0 <= i && i < 10 && 0 <= j && j < 10 && 0 <= k && k < 10) {
		char buffer[2];
		sprintf((char *) &buffer, "%d", i);
		if (strcmp(argv[1], (char *) &buffer) == 0) {
			sprintf((char *) &buffer, "%d", j);
			if (strcmp(argv[2], (char *) &buffer) == 0) {
				sprintf((char *) &buffer, "%d", k);
				return strcmp(argv[3], (char *) &buffer) == 0;
			}
		}
	}
	return false;
}

int main(int argc, char *argv[]) {

	// Only accept valid experiment codes. The have 3 digits.
	if (argc != 4) {
		printf("ERROR: Got %d Arguments while expecting %d Arguments\n", argc,
				4 - 1);
		printf("Usage: BscThesis i j k\n");
		printf("Where i, j, k are integers.");
		return 0;
	}

#if DEBUG
	// TESTS
	if (true) {
		void (*tests[])(void) = {test_GameStateDomain_clonePoint,
			test_GameStateDomain_equals,
			test_GameStateDomain_eval,
			test_LinearCombination_creation,
			test_SquareDomainSquareHole_convexCombination,
			test_SquareDomainSquareHole_getGridIndex,
			test_SquareDomainSquareHole_isFeasible,
			test_SquareDomainRoundHole_isFeasible,
			test_SquareDomain_convexCombination,
			test_SquareDomain_convexCombination_index,
			test_SquareDomain_getGridIndex,
			test_SqaureDomain_getMaximalGridIndex,
			test_SquareDomain_getPoint,
			test_SquareDomain_isGridPoint,
			test_SquareDomain_isTerminal,
			test_ValueFunction_getValue,
			test_ValueFunction_getValue_altered,
			test_ValueFunction_eval,
			test_ValueFunction_toArrayIndex};

		for (int i = 0; i < 14; i++) {
			try {
				(*tests[i])();
			} catch (int ex) {
				cout << ex << endl;
			}
		}
		cout << endl << "Test done!" << endl;
	}

	// PROFILING
	if (false) {
		profile_ValueFunction_eval();
		profile_Game_computeValueFunction();
	}
#endif /* DEBUG */

	// EXPERIMENTS
	if (true) {
		if (isExperiment(argv, 0, 0, 1))
			experiment_0_0_1();
		else if (isExperiment(argv, 0, 0, 2))
			experiment_0_0_2();
		else if (isExperiment(argv, 0, 0, 3))
			experiment_0_0_3();
		else if (isExperiment(argv, 0, 0, 4))
			experiment_0_0_4();
		else if (isExperiment(argv, 0, 0, 5))
			experiment_0_0_5();
		else if (isExperiment(argv, 0, 0, 6))
			experiment_0_0_6();
		else if (isExperiment(argv, 0, 0, 7))
			experiment_0_0_7();
		else if (isExperiment(argv, 1, 1, 0))
			experiment_1_1_0();
		else if (isExperiment(argv, 1, 1, 1))
			experiment_1_1_1();
		else if (isExperiment(argv, 1, 2, 0))
			experiment_1_2_0();
		else if (isExperiment(argv, 1, 3, 0))
			experiment_1_3_0();
		else if (isExperiment(argv, 1, 4, 0))
			experiment_1_4_0();
		else if (isExperiment(argv, 2, 5, 0))
			experiment_2_5_0();
		else if (isExperiment(argv, 2, 6, 0))
			experiment_2_6_0();
		else if (isExperiment(argv, 2, 7, 0))
			experiment_2_7_0();
		else if (isExperiment(argv, 2, 8, 0))
			experiment_2_8_0();
	}
	return 0;
}
