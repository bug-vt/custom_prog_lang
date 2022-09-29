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
        expected = "52"
        out = runVM ("../example/add.assembly");
        self.assertEqual (out, expected);

    def testMov (self):
        expected = "Hello World"
        out = runVM ("../example/mov.assembly");
        self.assertEqual (out, expected);

    def testCall (self):
        expected = ("inside myFunc\n"
                    "back to main\n"
                    "33")
        out = runVM ("../example/call.assembly");
        self.assertEqual (out, expected);
        
if __name__ == '__main__':
    unittest.main ()
