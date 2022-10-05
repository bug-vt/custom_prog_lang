#!/usr/bin/env python3

import unittest
from subprocess import Popen, call, PIPE
import subprocess


def runVM (input_file):
    proc = Popen (["cs4974", input_file], stdout=PIPE)
    proc.wait ()
    out, err = proc.communicate ()
    return str (out, 'utf-8')

class TestVM (unittest.TestCase):

    def testAdd (self):
        expected = "52\n"
        out = runVM ("../example/add.assembly");
        self.assertEqual (out, expected);

    def testMov (self):
        expected = "Hello World\n"
        out = runVM ("../example/mov.assembly");
        self.assertEqual (out, expected);

    def testCall (self):
        expected = ("inside myFunc\n"
                    "back to main\n"
                    "33\n")
        out = runVM ("../example/call.assembly");
        self.assertEqual (out, expected);
    
    def testNestedCall (self):
        expected = ("inside outerFunc\n"
                    "inside innerFunc\n"
                    "back to main\n")
        out = runVM ("../example/nested_func.assembly");
        self.assertEqual (out, expected);

    def testBasicAdder (self):
        expected = ("12\n")
        out = runVM ("../example/basic_adder.assembly");
        self.assertEqual (out, expected);

    def testExample (self):
        expected = ("80\n"
                    "CS 4974 scripting design and implementation\n")
        out = runVM ("../example/example.assembly");
        self.assertEqual (out, expected);

    def testArithmetic (self):
        expected = ("30\n"
                    "25\n"
                    "18.400000\n"
                    "5\n"
                    "8.400000\n"
                    "125\n"
                    "-8.400000\n"
                    "126\n"
                    "-9.400000\n")
        out = runVM ("../example/arithmetic.assembly");
        self.assertEqual (out, expected);

    def testArithmetic (self):
        expected = ("3\n"
                    "7\n"
                    "14\n"
                    "1\n"
                    "8\n"
                    "4\n")
        out = runVM ("../example/bitwise.assembly");
        self.assertEqual (out, expected);
        
if __name__ == '__main__':
    unittest.main ()
