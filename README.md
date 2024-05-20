Deception is a simple environment programming language that is a cross
between forth, APL, and other programming languages.

This script has come about due to issues I've been encountering while bringing
up a simple system on my i960 development board. While I have access to C/C++,
it is a very old version of it. I have to use gcc 3.4.6 so no modern C++
features...  I also tried to port the i960 backend to gcc 11.3.0 but ran into
nearly impossible to debug code generation problems. I have no idea how gcc
works and it is a very daunting task to try and fix all of it. 

Clang/LLVM is no better, the table gen language requires that you understand
everything about a given processor before you can write a backend. It also
doesn't have a quick way to test the backend in parts.

Usually, one would write a forth intepreter but that too became quite a
nightmare because there are ambiguities in the concept when it comes to
executing words and getting them from the user. It isn't that we read line by
line but what happens when you encounter an error. If it is in the middle of a
character stream then it can becomes trivial to not only hose your system but
possible to cause damage. I hate the ambiguity when it comes to error handling
and that it actually consumes the majority of your time when writing a forth
implementation.

I actually started the bones of a forth interpreter in i960 assembly (which is
actually a joy to write in because it is mostly-RISC) but I encountered burnout
because I couldn't see the point as it would just be a boring load/store
wrapper over a frankly interesting architecture. 

I have been bouncing the idea around in my head for years to have a very simple
compositional language that I can easily define for a given purpose but not
lock me in either. Combined with the fact that I find writing simulators for
instruction sets of my own design, I came up with what I call Deception.


Deception is a variable length composable programming language that is designed
to allow me to build and test ideas quickly. It is a cross between a character
parser, forth dictionary, and problem solving platform.

The core of Deception is the action table. Each action table is comprised of
256 entries. Each entry contains an action to perform when that given ascii
code is encountered. Multiple action tables can be constructed and linked
together using actions in different tables. Each action entry can also contain
other flags and information depending on what you want to do with it. 

The ascii codes are actually very powerful and I can reuse the concepts behind
different codes to eliminate all forms of ambiguity and the need for escape
codes (another bug bear of mine).

At it's core the language works like this:

- Read a character in (doesn't matter from where) as an 8-bit ascii code
- Grab the action entry from the current action table corresponding to the ascii code
- If the pointer to code contains a valid address then invoke it. If there is no operation associated with that entry then just continue (it is not an error state)
- Repeat

The set of actions that are being performed are defined inside of the
interpreter itself. All you need to boot strap a system is a small loop that
reads a stream of characters in rom and carry out actions based off of the set
of actions you have defined. You don't even need a C/C++ compiler, if you have
an assembler this idea works perfectly as well. 

Think about it this way, you build up your system from tables of actions (like
the vocabularies in forth but not insane). It is important to remember that
you only use the codes you want. It allows for more logical reasoning about
actions. 

It is also important to note that you do not need to implement things like
escape code processing within the interpreter. That is encoded by your
interaction with the system.

This repo contains both examples of Deception in action and a skeleton
interpreter written in C.

The name deception comes from the lyrics of "The Thing That Should Not Be" by Metallica. 
I was trying to come up with a name for this language and that word just stuck.

