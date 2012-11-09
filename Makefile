# Copyright (c) 2011-2012, Valentin Sawadski
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# - Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
# - Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# - Neither the name of the <ORGANIZATION> nor the names of its contributors
#   may be used to endorse or promote products derived from this software
#   without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
# 
# Makefile
#
# Created on: 2012-10-04
#     Author: Valentin Sawadski <valentin@sawadski.eu>

SOURCE_DIR = src
SOURCES = $(notdir $(wildcard ${SOURCE_DIR}/*.cpp))

CFLAGS = -g -O3 -Wall #--verbose
LDFLAGS = #--verbose #No Particular LD-Flags so far.

CC       = g++
LD       = ld
AR       = ar
AS       = gcc
GASP     = gasp
NM       = nm
OBJCOPY  = objcopy
RANLIB   = ranlib
STRIP    = strip
SIZE     = size
READELF  = readelf
MAKETXT  = objdump
CP       = cp -p
RM       = rm -f
MKDIR    = mkdir -p
MV       = mv

# the file which will include dependencies
DEPEND = $(SOURCES:.cpp=.d)
# all the object files
OBJDIR = obj
OBJECTS = $(SOURCES:.cpp=.o)

$(warning SOURCES ${SOURCES})
$(warning OBJECTS ${OBJECTS})
$(warning DEPEND ${DEPEND})

all: outdir hji-approximation
	echo "All Finished"
 
hji-approximation: $(OBJECTS)
	echo "Linking $@"
	$(CC) $(addprefix ${OBJDIR}/, $(OBJECTS)) $(LDFLAGS) $(LIBS) -o ${OBJDIR}/$@

outdir:
	echo "Creating Outdir if not exists"
	$(MKDIR) ${OBJDIR}

%.o:
	echo "Compiling $@"
	$(CC) ${CFLAGS} -c -o ${OBJDIR}/$@ ${SOURCE_DIR}/$(addsuffix .cpp, $(basename $@))

# include the dependencies unless we're going to clean, then forget about them.
ifneq ($(MAKECMDGOALS), clean)
-include $(DEPEND)
endif
# dependencies file includes also considered, since some of these are our own
# (otherwise use -MM instead of -M)
%.d: outdir
	echo "Generating dependencies $@"
	$(CC) -M ${CFLAGS} ${SOURCE_DIR}/$(addsuffix .cpp, $(basename $@)) >${OBJDIR}/$@

.SILENT:

.PHONY:	clean

clean:
	$(RM) -r ${OBJDIR}

