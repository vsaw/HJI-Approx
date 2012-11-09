HJI-Approx
==========

This can compute the Value Function and trajectories of Pursuit-Evasion Games.

Implementation of Fully-Discrete Schemes for the Value Function of Pursuit-Evasion Games with State Constraints. This is the in depth documentation of the code for the Bachelors Thesis of Valentin Sawadski <valentin@sawadski.eu>.

Compiling
---------
The project can be compiled using `make` in the main folder of the Project. All object files and binaries will then be created in the folder `obj`.

Running experiments
-------------------
If compiled in the main folder experiments can be started with the following command.

```
hji-approximation x y z
```

where `x y z` specify the number of the experiment to be started.

The following experiments are already implemented:
- 0 0 1: [-2,2]^4, n = 14, n_c = 8 + 1, Vp = 2, Ve = 1, eps = 1e-3
- 0 0 2: [-2,2]^4, n = 20, n_c = 20 + 1, Vp = 1, Ve = 1, eps = 1e-3
- 0 0 3: [-2,2]^4 \ [-0.5, 0.5]^4, n = 20, n_c = 20 + 1, Vp = 2, Ve = 1, eps = 1e-5
- 0 0 4: [-2,2]^4, n = 20, n_c = 21 + 1, Vp = 2, Ve = 1, eps = 1e-3
- 0 0 5: [-2,2]^4, n = 24, n_c = 48 + 1, Vp = 2, Ve = 1, eps = 1e-3
- 0 0 6: [-2,2]^4, n = 14, n_c = 20 + 1, Vp = 1, Ve = 1.25, eps = 1e-3
- 0 0 7: [-2,2]^4 \ Ball(Center=0, rad=0.5), n = 16, n_c = 20 + 1, Vp = 2, Ve = 1, eps = 1e-5
- 1 1 0: [-2,2]^4, n = 50, n_c = 48 + 1, Vp = 2, Ve = 1, eps = 1e-3
- 1 1 1: [-2,2]^4, n = 50, n_c = 48 + 1, Vp = 2, Ve = 1, eps = 1e-3
- 1 2 0: [-2,2]^4, n = 26, n_c = 36 + 1, Vp = 2, Ve = 1, eps = 1e-5
- 1 3 0: [-2,2]^4 \ [-0.53, 0.53]^4, n = 50, n_c = 48 + 1, Vp = 2, Ve = 1, eps = 1e-4
- 1 4 0: [-2,2]^4 \ Ball(Center=0, rad=7delta x), n = 50, n_c = 48 + 1, Vp = 2, Ve = 1, eps = 1e-4
- 2 5 0: [-2,2]^4, n = 50, n_c = 36, Vp = 1, Ve = 1, eps = 1e-3
- 2 6 0: [-2,2]^4 \ Ball(Center=0, rad=7delta x), n = 50, n_c = 48 + 1, Vp = 1, Ve = 1, eps = 1e-4
- 2 7 0: [-2,2]^4, n = 50, n_c = 48 + 1, Vp = 1, Ve = 1.25, eps = 1e-3
- 2 8 0: [-2,2]^4, n = 50, n_c = 36, Vp = 1, Ve = 1.25, eps = 1e-3

Further experiments can be easily added to the `main.c` file.

Documentation
-------------
The source code is very well documented. A complete overview of the documentation can be created by calling `doxygen` in the doc foler. Additional documentation is provided in the form of a Bachelors Thesis which contains an introduction in the mathematical background of this project.

License
-------
Copyright (c) 2011-2012, Valentin Sawadski
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
- Neither the name of the <ORGANIZATION> nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

*THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*