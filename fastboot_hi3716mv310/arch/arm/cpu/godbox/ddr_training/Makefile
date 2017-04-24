#/******************************************************************************
# *    Copyright (c) 2009-2012 by Hisi.
# *    All rights reserved.
# * ***
# *    Create by Czyong. 2012-09-07
# *
#******************************************************************************/

SRC := hi3716mv300 hi3716cv100 hi3712v100

all: $(addsuffix .build,$(SRC))

clean: $(addsuffix .clean,$(SRC))

%.build: %
	make -C $(@:.build=) TOPDIR=$(TOPDIR) CROSS_COMPILE=$(CROSS_COMPILE)

%.clean: %
	make -C $(@:.clean=) TOPDIR=$(TOPDIR) CROSS_COMPILE=$(CROSS_COMPILE) \
		clean

.PHONY: all clean
