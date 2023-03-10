#!/usr/bin/env python3

import unittest
from subprocess import Popen, call, PIPE
import subprocess
import random


def runVM (input_file, argv=[]):
    proc = Popen (["cs4974", input_file] + argv, stdout=PIPE)
    proc.wait ()
    out, err = proc.communicate ()
    return str (out, 'utf-8')

class TestVM (unittest.TestCase):

    def testAdd (self):
        expected = "52\n"
        out = runVM ("../example/add.casm");
        self.assertEqual (out, expected);

    def testMov (self):
        expected = "Hello World\n"
        out = runVM ("../example/mov.casm");
        self.assertEqual (out, expected);

    def testCall (self):
        expected = ("inside myFunc\n"
                    "back to main\n"
                    "33\n")
        out = runVM ("../example/call.casm");
        self.assertEqual (out, expected);
    
    def testNestedCall (self):
        expected = ("inside outerFunc\n"
                    "inside innerFunc\n"
                    "back to main\n")
        out = runVM ("../example/nested_func.casm");
        self.assertEqual (out, expected);

    def testBasicAdder (self):
        expected = ("12\n")
        out = runVM ("../example/basic_adder.casm");
        self.assertEqual (out, expected);

    def testExample (self):
        expected = ("80\n"
                    "CS 4974 scripting design and implementation\n")
        out = runVM ("../example/example.casm");
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
        out = runVM ("../example/arithmetic.casm");
        self.assertEqual (out, expected);

    def testArithmetic (self):
        expected = ("3\n"
                    "7\n"
                    "14\n"
                    "1\n"
                    "8\n"
                    "4\n")
        out = runVM ("../example/bitwise.casm");
        self.assertEqual (out, expected);

    def testBranch (self):
        expected = ("0\n"
                    "1\n"
                    "2\n"
                    "3\n"
                    "4\n")
        out = runVM ("../example/branch.casm");
        self.assertEqual (out, expected);

    def testJump (self):
        expected = ("3\n"
                    "1\n"
                    "2\n")
        out = runVM ("../example/jump.casm");
        self.assertEqual (out, expected);

    def testCmdLineArgs (self):
        expected = ("2\n"
                    "Hello\n"
                    "World\n")
        out = runVM ("../example/cmd_args.casm", ["Hello", "World"]);
        self.assertEqual (out, expected);
       
    def testFibonacci (self):
        expected = ("0 1 1 2 3 5 8 13 21 34 55 89 144 \n")
        out = runVM ("../example/fibonacci.casm", ["12"]);
        self.assertEqual (out, expected);

    def testInsertionSort (self):
        unsorted_arr = [str (random.randint (0, 1000)) for _ in range (30)]
        sorted_arr = sorted ([int (i) for i in unsorted_arr])

        expected = "Before: "
        for i in reversed (range (len (unsorted_arr))):
          expected += str (unsorted_arr[i]) + " "
        expected += "\nAfter:  "
        for i in range (len (sorted_arr)):
          expected += str (sorted_arr[i]) + " "
        expected += "\n"

        out = runVM ("../example/sort.casm", unsorted_arr);
        self.assertEqual (out, expected);

    def testGlobal (self):
        expected = ("42\n")
        out = runVM ("../example/global.casm");
        self.assertEqual (out, expected);
         
    def testPassByRef (self):
        expected = ("7\n"
                    "3\n"
                    "7\n"
                    "7\n"
                    "42\n")
        out = runVM ("../example/pass_by_ref.casm");
        self.assertEqual (out, expected);

    def testPop (self):
        expected = ("2\n"
                    "1\n"
                    "hello world\n")
        out = runVM ("../example/pop.casm");
        self.assertEqual (out, expected);

    def testConcat (self):
        expected = ("Hello \n"
                    "World\n"
                    "Hello World\n")
        out = runVM ("../example/concat.casm");
        self.assertEqual (out, expected);

    def testStringProcess (self):
        expected = ("Hello World!\n"
                    "Hello \n")
        out = runVM ("../example/str_process.casm");
        self.assertEqual (out, expected);


if __name__ == '__main__':
    unittest.main ()
