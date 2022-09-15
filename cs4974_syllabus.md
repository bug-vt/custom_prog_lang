# CS 4974 Scripting Language Design and Implementation

Dr. Muhammad Ali Gulzar  
Office: KW II, Suite 2224, 2202 Kraft Drive, Blacksburg, VA 24060  
Email: gulzar@cs.vt.edu  
Phone: (540) 231-0851  

## <ins> **Course Syllabus** </ins>
## 1. Credit Hours: 3
## 2. Description:
Provide an in-depth introduction to principles and implementation of scripting language. The objective is to design and implement a C-like scripting language by the end. Emphasis is given to topics of Assembler, Virtual Machine, and Compiler thorough hands-on implementation. Also, give the student opportunity to dig deeper on a specific topic on VM/Compiler of his choice through an open-ended project.

## 3. Format:
The course will be entirely project-based where the student will work on a semester-long project to build a working C-like (C subset) scripting language from the scratch. The project is broken down into 4 parts: Assembler, Virtual Machine, Compiler, and Open-ended project. The Assembler, Virtual Machine, and Compiler will be written in C.  
In the first several weeks, the student will work on the low-level scripting environment. The student will design an assembly language that mimics a subset of intel 80x86 assembly, implement an assembler for generating bytecode, then Virtual Machine that mimics a generic hardware processor to execute the bytecode. After that, the student will implement a compiler for the next several weeks. Finally, for the remaining weeks, the student will select and work on a specific topic that extends the project in an interesting way (See the open-ended project below). 

## 4. Learning outcomes:
- Obtain a full overview of scripting language construction through studying the interaction/relationship between source code, compiler, assembler, and virtual machine.
- Gain a deeper understanding of computer architecture and operating systems through hands-on implementation of an assembler that generates 80x86-like instructions and a virtual machine that can run it.
- Gain a deeper understanding of compiler theory and the inner working of programming language.
- Understand the advantages and disadvantages of scripting language. Know why execution of scripting language has a lower performance over programming languages that compile and run directly on specific hardware.
- Improve C programming and organization/management of a large-scale software project.
- More exposure to test-driven development, writing own test cases, and regression testing. Learn to write meaningful test cases that catch bugs and ensure the correctness of software.

## 5. Textbooks:
1. Crafting interpreters by Robert Nystrom
2. Modern Compiler Implementation in ML by Andrew Appel
3. Game scripting mastery by Alex Varanese

## 6. Assignments:
- Reading assigned chapters from the textbooks (See the tentative schedule below)
- Project report for each project
- Project 1: Assembler
- Project 2: Virtual Machine 
- Project 3: Compiler
- **Open-ended final project:**
  -  The open-ended project should extend the scripting system in an interesting way. Examples of possible projects include:
     -  Implementing Symbolic execution.
     -  Implementing fuzzing to test the compiler.
     -  Integrating the scripting system into a game engine or embedded application.
     -  Implementing priority-based multithreading features into VM.
     -  Optimizing compiler output with better stack utilization and use of additional virtual registers.
     -  Implementing popular language features such as classes, inheritance, garbage collection, etc.


## 7. Grading:
Grading is based on correctness and coverage of test cases for implemented features, project reports, and successful demonstration of working scripting language at the end of the semester.
### **Test cases:**
Employ test-driven development by writing a large set of small programs that each demonstrate one aspect of the assembler, virtual machine, and compiler. These will include both programs that should assemble/compile successfully and ones that should output errors.
### **Project reports:**
Document the justification and trade-offs made in each project. This should demonstrate a strong understanding of the systems that the student has built throughout the semester. 
### **End of semester Demo:**
Since the goal of this course is to build a working scripting language, the student should be able to demonstrate a program that is entirely run using the scripting language he has built. The program should demonstrate the capability, versatility, and robustness of the scripting language.


<div style="page-break-after: always;"></div>

## 8. Tentative schedule:
### Phase 1: Assembler and Virtual Machine
| Week # | Topic | Assignment | Reading |
| ------ | ----- | ---------- | ------- |
| 1 | Instruction set and Assembly framework | Project 1 start | Varanese Chapter 8
| 2 | Assembler for VM | | Varanese Chapter 9
| 3 | Bytecode and Executable format | Project 1 due <br> Project 2 start |
| 4 | VM structure interfaces | | Varanese Chapter 10
| 5 | VM execution cycle | | Varanese Chapter 11

### Phase 2: Compiler 
| Week # | Topic | Assignment | Reading |
| ------ | ----- | ---------- | ------- |
| 6 | Loader and Preprocessor | Project 2 due <br> Project 3 start | Varanese Chapter 12
| 7 | Lexical Analysis | | Appel Chapter 2 <br> Nystrom Chapter 4-5, 16
| 8 | Parsing | | Appel Chapter 3, 4 <br> Nystrom Chapter 6-8, 17
| 9 | Semantic Analysis, I-Code | | Appel Chapter 5, 7 <br> Nystrom Chapter 11
| 10 | Code Generation | | Appel Chapter 8-9 

### Phase 3: Open-ended project
| Week # | Topic | Assignment | Reading |
| ------ | ----- | ---------- | ------- |
| 11 | Topic for open-ended project | Project 3 due <br> Open-ended final project start | TBD
| 12 | Topic for open-ended project | | TBD
| 13 | Topic for open-ended project | | TBD
| 14 | Thanksgiving break | |
| 15 | Topic for open-ended project | | TBD
| 16 | Wrap-up and Demo | Open-ended final project due |


<div style="page-break-after: always;"></div>

# Instruction Set
Instruction set will follow Complex Instruction Set Computing (CISC) methodology, similar to Intel 80x86. This is to make runtime environment for scripting system faster: doing much as work as possible in C instead of leaving implementation to slower scripting language.
## Memory
| Instruction | Opcode | Operand count | Operands |
| ----------- | ------ | ------------- | -------- |
| mov | 0 | 2 | DESTINATION, SOURCE |

## Arithmetic
| Instruction | Opcode | Operand count | Operands |
| ----------- | ------ | ------------- | -------- |
| add | 1 | 2 | DESTINATION, SOURCE |
| sub | 2 | 2 | DESTINATION, SOURCE |
| mul | 3 | 2 | DESTINATION, SOURCE |
| div | 4 | 2 | DESTINATION, SOURCE |
| mod | 5 | 2 | DESTINATION, SOURCE |
| exp | 6 | 2 | DESTINATION, SOURCE |
| neg | 7 | 1 | DESTINATION |
| inc | 8 | 1 | DESTINATION |
| dec | 9 | 1 | DESTINATION |
## Bitwise
| Instruction | Opcode | Operand count | Operands |
| ----------- | ------ | ------------- | -------- |
| and | 10 | 2 | DESTINATION, SOURCE |
| or  | 11 | 2 | DESTINATION, SOURCE |
| xor | 12 | 2 | DESTINATION, SOURCE |
| not | 13 | 1 | DESTINATION |
| shl | 14 | 2 | DESTINATION, SHIFT COUNT |
| shr | 15 | 2 | DESTINATION, SHIFT COUNT |
## String processing
| Instruction | Opcode | Operand count | Operands |
| ----------- | ------ | ------------- | -------- |
| concat  | 16 | 2 | STRING, STRING |
| getChar | 17 | 3 | DESTINATION, SOURCE, INTEGER |
| setChar | 18 | 3 | INTEGER, DESTINATION, SOURCE |
## Conditional Branching
| Instruction | Opcode | Operand count | Operands |
| ----------- | ------ | ------------- | -------- |
| jmp | 19 | 1 | LABEL | 
| je  | 20 | 3 | SOURCE, SOURCE, LABEL | 
| jne | 21 | 3 | SOURCE, SOURCE, LABEL | 
| jg |  22 | 3 | SOURCE, SOURCE, LABEL | 
| jl |  23 | 3 | SOURCE, SOURCE, LABEL | 
| jge | 24 | 3 | SOURCE, SOURCE, LABEL | 
| jle | 25 | 3 | SOURCE, SOURCE, LABEL | 
## Stack Interface
| Instruction | Opcode | Operand count | Operands |
| ----------- | ------ | ------------- | -------- |
| push | 26 | 1 | SOURCE | 
| pop  | 27 | 1 | DESTINATION | 
## Function Interface
| Instruction | Opcode | Operand count | Operands |
| ----------- | ------ | ------------- | -------- |
| call | 28 | 1 | FUNCTION | 
| ret |  29 | 0 | | 
## Miscellaneous
| Instruction | Opcode | Operand count | Operands |
| ----------- | ------ | ------------- | -------- |
| pause | 30 | 1 | INTEGER |
| exit  | 31 | 1 | INTEGER |
## Directives
    setStackSize    Size
    var             Identifier
    param           Identifier
    func            FunctionName

## Supporting Operand types
| Type code | Operand type | Usage |
| --------- | ------------ | ----- |
| 0 | Integer | 42 |
| 1 | Float | 3.14 |
| 2 | String | "Hello World!" |
| 3 | Absolute stack index | xyz OR array[3] |
| 4 | Relative stack index | array[x] |
| 5 | Instruction index | jmp label |
| 6 | Function index | call myFunc |
| 7 | Register | _retVal |
<div style="page-break-after: always;"></div>

# Scripting language Minimum Design Goal
## Data Structures
### Typeless variable: support Boolean, integer, floating-point, and string.
    var myBool = true;
    var myInt = 33;
    var myFloat = 1.1234;
    var myString = "hello world!";
### Array
    var myArray [10];
    myArray[1] = "hello world!";
## Operators
### Arithmetic
| Operator | Description |
| -------- | ----------- |
| + | Addition (Binary) |
| - | Subtraction (Binary) |
| $ | String Concatenation (Binary) |
| * | Multiplication (Binary) |
| / | Division (Binary) |
| % | Modulus (Binary) |
| ^ | Exponent (Binary) |
| ++ | Increment (Unary) |
| -- | Decrement (Unary) |
| = | Assignment (Binary) |
| += | Addition assignment (Binary) |
| -= | Subtraction assignment (Binary) |
| *= | Multiplication assignment (Binary) |
| /= | Division assignment (Binary) |
| %= | Modulus assignment (Binary) |
| ^= | Exponent assignment (Binary) |

### Bitwise
| Operator | Description |
| -------- | ----------- |
| & | And (Binary) |
| \| | Or (Binary) |
| # | Xor (Binary) |
| ~ | Not (Unary) |
| << | Shift left (Binary) |
| >> | Shift right (Binary) |

### Logical and Relational
| Operator | Description |
| -------- | ----------- |
| && | And (Binary) |
| \|\| | Or (Binary) |
| ! | Not (Unary) |
| == | Equal (Binary) |
| != | Not Equal (Binary) |
| < | Less than (Binary) |
| > | Greater than (Binary) |
| <= | Less than or equal (Binary) |
| >= | Greater than or equal (Binary) |

## Control Structures
    if (Expression)
    {
        // True
    }
    else
    {
        // false
    }

    while (Expression)
    {
        // body
        // break or continue can be used
    }

    for (Initializer; Terminating condition; Iterator)
    {
        // body
        // break or continue can be used
    }

## Functions
    func FunctionName (...)
    {
        // body
        // (optional) return ...
    }