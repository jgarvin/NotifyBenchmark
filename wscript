#!/bin/env python

def options(opt):
    opt.load('compiler_c compiler_cxx')

def configure(cnf):
    cnf.load('compiler_c compiler_cxx')
    cnf.check(features='cxx cxxprogram', lib=['pthread'], uselib_store="PTHREAD")
    print("configure!")

def build(bld):
    print("build!")
    bld(features='cxx cxxprogram', source='src/PipeTest.cpp', target='app', use=['PTHREAD'])

