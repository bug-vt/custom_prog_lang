#!/usr/bin/env python3

import unittest
from subprocess import Popen, call, PIPE
import subprocess
import random


def runProg (input_file, argv=[]):
    exit_code = call (["compiler", input_file])
    if (exit_code > 0):
        exit (1)

    proc = Popen (["cs4974", "out.casm"] + argv, stdout=PIPE)
    proc.wait ()
    out, err = proc.communicate ()
    return str (out, 'utf-8')

class TestVM (unittest.TestCase):

    def testAdd (self):
        expected = "6\n"
        out = runProg ("../example/add.src");
        self.assertEqual (out, expected);

    def testArithmetic (self):
        expected = "129\n"
        out = runProg ("../example/arithmetic.src");
        self.assertEqual (out, expected);

    def testComparison (self):
        expected = ("1\n"
                    "0\n"
                    "1\n")
        out = runProg ("../example/comparison.src");
        self.assertEqual (out, expected);
    
    def testGrouping (self):
        expected = ("-174\n")
        out = runProg ("../example/grouping.src");
        self.assertEqual (out, expected);

    def testScope (self):
        expected = ("39\n"
                    "32\n"
                    "99\n")
        out = runProg ("../example/scope.src");
        self.assertEqual (out, expected);

    def testString (self):
        expected = ("Hello World!\n")
        out = runProg ("../example/string.src");
        self.assertEqual (out, expected);

    def testIfThenElse (self):
        expected = ("inside then branch\n"
                    "42\n")
        out = runProg ("../example/if_then_else.src");
        self.assertEqual (out, expected);

    def testNestedControlFlow (self):
        expected = ("hi\n"
                    "water\n"
                    "windy\n")
        out = runProg ("../example/nested_control_flow.src");
        self.assertEqual (out, expected);

    def testLogical (self):
        expected = ("hi\n"
                    "cs4974\n"
                    "0\n"
                    "0\n")
        out = runProg ("../example/logical.src");
        self.assertEqual (out, expected);

if __name__ == '__main__':
    unittest.main ()
