# CppUnit to Gtest

## What is CppUnit?
Cppunit is a C++ testing framework inspired 
by the similarly named Java testing framework [Junit](https://junit.org/).

In the early 2000s it was used extensively and was seen 
as a great testing framework. In modern times however people
often prefer other frameworks such as [catch2](https://github.com/catchorg/Catch2) or [gtest](https://github.com/google/googletest).

CppUnit has been significantly forked and fractured. 
The most up-to-date version I can see is [here](https://freedesktop.org/wiki/Software/cppunit/)
though the [source forge fork](https://sourceforge.net/projects/cppunit/) is often used.

## Why not Cppunit?

CppUnit does not share many similarities with Junit
(partially due to C++ having less functionality available) 
so is not as easy to use. 

This project was started because of CppUnit's lack of memory management
which lead to my leak sanitizer giving a lot of false positives. 

There are ways around this but having one or more leaks 
per file means a large blacklist was challenging.

Gtest is good! Gtest gives me all the tools I need out of the box:
 - Tracking of time each test has taken and displaying it in output
   + Cppunit has a clocker plugin which needs to be explicitly added in the main section of the program
 - Easily readable test output
   + Each failing test gives an error to say it has failed, a reasoning as to why it has failed and what the problem lines where
   + Test output is green for passing tests and red for failing ones (if the terminal supports it or if added explicitly as I do for gitlab CI)
   + Cppunit gives "pass" or "fail" by default
 - Catches obscure errors
   + GTest catches things that Cppunit does not like odd internal windows exceptions that cannot be caught with a `try{` block and instead need a `__try`
   + CppUnit does not catch all exceptions and instead terminates the program if a custom uncatchable exception is thrown
- Junit output
  + Despite Cppunit being a port of Junit the xml output file does not match the Junit one, as Junit has become the norm for CI pipelines this means that an output-er needs to be custom written
  + GTest comes with this ability built in

## Why not rewrite all my tests instead of using this?
You should! 

This aims to allow for incremental adoption of GTest rather than force its usage everywhere.
A wiser approach might be to automate some changes using a python script.

[//]: # (TODO: Write said python script and add it to the repo)

## Features

We don't aim to add every feature of CppUnit but if we are missing something please write an example test case in an issue!

We currently support all features shown in the CppUnit repo's examples and no more.

