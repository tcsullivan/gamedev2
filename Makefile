##
# @file Makefile
# Script to build source files
#
# Copyright (C) 2019 Clyne Sullivan
# Copyright (C) 2019 Belle-Isle, Andrew <drumsetmonkey@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.	If not, see <https://www.gnu.org/licenses/>.
#

#
# VARIABLES
#

CC  = gcc
CXX = g++

EXEC = main
EXECDIR = .

SRCDIR = src
OUTDIR = out
SRCEXT = cpp
OBJEXT = o
DEPEXT = d

LIBDIR = lib
LIBS   = -L$(LIBDIR) -lSDL2 -lpthread -lentityx -lluajit -ldl -lGLEW -lGL \
         -lSDL2_image -lsoil -lfreetype -lopenal -lalut

CXXFLAGS = -ggdb -std=c++17 -Wall -Wextra -Werror -pedantic \
           -Wno-class-memaccess -Wno-implicit-fallthrough -Wno-unused-parameter

CXXINCS = -I$(SRCDIR) \
          -I$(LIBDIR)/entityx \
          -I$(LIBDIR)/luajit/src \
          -I$(LIBDIR)/sol2/include \
          -I$(LIBDIR)/soil \
          -I$(LIBDIR)/cereal/include \
	  -I$(LIBDIR)/freetype

CXXSRC := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
CXXOBJ := $(patsubst $(SRCDIR)/%,$(OUTDIR)/%,$(CXXSRC:.$(SRCEXT)=.$(OBJEXT)))

#
# COMPILE STUFF
#

all: resources $(EXEC)

resources: directories

directories: 
	@mkdir -p $(EXECDIR)
	@mkdir -p $(OUTDIR)

clean:
	@echo "  CLEAN"
	@$(RM) -rf $(OUTDIR)

cleanall: clean
	@$(RM) -f lib/libentityx.a lib/libluajit.a lib/libsoil.a

cleaner: clean

$(EXEC): lib/libentityx.a lib/libluajit.a lib/libsoil.a $(CXXOBJ)
	@echo "  CXX   " $(EXEC)
	@$(CXX) -o $(EXECDIR)/$(EXEC) $^ $(LIBS)

$(OUTDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@echo "  CXX   " $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(CXXINCS) -c -o $@ $<
	@$(CXX) $(CXXFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(OUTDIR)/$*.$(DEPEXT)
	@cp -f $(OUTDIR)/$*.$(DEPEXT) $(OUTDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(OUTDIR)/$*.$(OBJEXT):|' < $(OUTDIR)/$*.$(DEPEXT).tmp > $(OUTDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(OUTDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(OUTDIR)/$*.$(DEPEXT)
	@rm -f $(OUTDIR)/$*.$(DEPEXT).tmp

lib/libentityx.a:
	@cmake lib/entityx -DENTITYX_BUILD_SHARED=FALSE
	@make -Clib/entityx -j4 entityx
	@cp lib/entityx/libentityx.a lib/libentityx.a

lib/libluajit.a:
	@make -Clib/luajit -j4
	@cp lib/luajit/src/libluajit.a lib/libluajit.a

lib/libsoil.a:
	@make -Clib/soil -j4
	@cp lib/soil/libsoil.a lib/libsoil.a

.PHONY: all remake clean cleaner cleanall resources

