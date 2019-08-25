##
# @file Makefile
# Script to build source files
#
# Copyright (C) 2019 Clyne Sullivan
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

CC  = gcc
CXX = g++

LIBS = -lSDL2

CXXFLAGS = -ggdb -std=c++17 \
	-Wall -Wextra -Werror -pedantic \
	-Isrc

CXXSRCDIR = src
CXXOUTDIR = out
CXXSRC    = $(wildcard $(CXXSRCDIR)/*.cpp) $(wildcard $(CXXSRCDIR)/*/*.cpp)
CXXOBJ    = $(patsubst $(CXXSRCDIR)/%.cpp, $(CXXOUTDIR)/%.o, $(CXXSRC))

EXEC = main

all: $(EXEC)

clean:
	@echo "  CLEAN"
	@rm -f $(EXEC)
	@rm -rf out

$(EXEC): $(CXXOUTDIR) $(CXXOUTDIR)/$(CXXOBJ)
	@echo "  CXX/LD  main"
	@$(CXX) $(CXXFLAGS) -o $(EXEC) $(CXXOBJ) $(LIBS)

$(CXXOUTDIR)/%.o: $(CXXSRCDIR)/%.cpp
	@echo "  CXX    " $<
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(CXXOUTDIR):
	@mkdir $(CXXOUTDIR)

