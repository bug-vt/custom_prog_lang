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
    return str (out, 'utf-8'), proc.returncode


class TestVM (unittest.TestCase):

    def testAdd (self):
        expected = "6\n"
        out = runProg ("../example/add.src")[0];
        self.assertEqual (out, expected);

    def testArithmetic (self):
        expected = "129\n"
        out = runProg ("../example/arithmetic.src")[0];
        self.assertEqual (out, expected);

    def testComparison (self):
        expected = ("1\n"
                    "0\n"
                    "1\n")
        out = runProg ("../example/comparison.src")[0];
        self.assertEqual (out, expected);
    
    def testGrouping (self):
        expected = ("-174\n")
        out = runProg ("../example/grouping.src")[0];
        self.assertEqual (out, expected);

    def testScope (self):
        expected = ("pi in outer scope: 44\n"
                    "No local pi inside inner scope yet. Changing outer pi to: 99\n"
                    "pi in inner scope: 32\n"
                    "pi back in outer scope: 99\n")
        out = runProg ("../example/scope.src")[0];
        self.assertEqual (out, expected);

    def testString (self):
        expected = ("Hello World!\n")
        out = runProg ("../example/string.src")[0];
        self.assertEqual (out, expected);

    def testIfThenElse (self):
        expected = ("inside then branch\n"
                    "42\n")
        out = runProg ("../example/if_then_else.src")[0];
        self.assertEqual (out, expected);

    def testNestedControlFlow (self):
        expected = ("inside else\n"
                    "  inside if\n"
                    "    inside else if\n")
        out = runProg ("../example/nested_control_flow.src")[0];
        self.assertEqual (out, expected);

    def testLogical (self):
        expected = ("hi\n"
                    "cs4974\n"
                    "0\n"
                    "0\n")
        out = runProg ("../example/logical.src")[0];
        self.assertEqual (out, expected);

    def testWhileLoop (self):
        expected = ("0\n"
                    "1\n"
                    "2\n"
                    "3\n"
                    "4\n")
        out = runProg ("../example/while.src")[0];
        self.assertEqual (out, expected);

    def testGoto (self):
        expected = ("1\n"
                    "3\n"
                    "4\n")
        out = runProg ("../example/goto.src")[0];
        self.assertEqual (out, expected);

    def testNestedWhile (self):
        expected = ("0\n"
                    "-1\n"
                    "3\n"
                    "0\n"
                    "-1\n"
                    "5\n"
                    "0\n"
                    "-1\n"
                    "9\n")
        out = runProg ("../example/nested_while.src")[0];
        self.assertEqual (out, expected);

    def testForLoop (self):
        expected = ("1\n"
                    "2\n"
                    "4\n"
                    "8\n"
                    "32\n")
        out = runProg ("../example/for.src")[0];
        self.assertEqual (out, expected);

    def testFuncCall (self):
        expected = ("7\n"
                    "hello world\n")
        out = runProg ("../example/call.src")[0];
        self.assertEqual (out, expected);

    def testReturn (self):
        expected = ("4\n")
        out = runProg ("../example/return.src")[0];
        self.assertEqual (out, expected);

    def testFib (self):
        expected = ("0\n"
                    "1\n"
                    "1\n"
                    "2\n"
                    "3\n"
                    "5\n"
                    "8\n"
                    "13\n"
                    "21\n"
                    "34\n")
        out = runProg ("../example/fib.src")[0];
        self.assertEqual (out, expected);

    def testArray (self):
        expected = ("hello 123 0.001000 world! cs4974 \n")
        out = runProg ("../example/array.src")[0];
        self.assertEqual (out, expected);

    def testRef (self):
        expected = ("42\n"
                    "37\n")
        out = runProg ("../example/ref.src")[0];
        self.assertEqual (out, expected);

    def testArrayRef (self):
        expected = ("before:\n"
                    "0\n"
                    "1\n"
                    "2\n"
                    "3\n"
                    "4\n"
                    "\n"
                    "after:\n"
                    "10\n"
                    "11\n"
                    "12\n"
                    "13\n"
                    "14\n")
        out = runProg ("../example/array_ref.src")[0];
        self.assertEqual (out, expected);

    def testRandInt (self):
        expected = ("true\n")
        out = runProg ("../example/rand.src")[0];
        self.assertEqual (out, expected);

    def testExit (self):
        expected = ("before exit\n")
        out, exit_code = runProg ("../example/exit.src");
        self.assertEqual (out, expected);
        self.assertEqual (exit_code, 2);


if __name__ == '__main__':
    unittest.main ()
