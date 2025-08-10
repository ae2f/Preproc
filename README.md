# ae2f::Preproc
The cmake utilities for enabling template meta programming in C with macro-function generating and including.  
Basically it uses `stdin` and `stdout` for processing, and you will need to pipeline those outputs.  
    > Which means you will making new files.  

It is written in standard ANSI C, which is the lowest version covered by cmake.  
The sources now covers from ANSI C to C23.

# ae2f_PreProc_C_COMPILER
When you define this value on cmake, 't will be expected to valid C compiler for configuration step.

# Requirements
- A C compiler >= 90 (aka ANSI C)
- cmake >= 3.20
- A functional OS which supports standard library of C and pipelining.
    - Unix based systems are recommended.
    - Windows is tested.
