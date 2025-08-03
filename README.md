# Working out what you really need from header files, a proof of concept

Suppose you have a large chunk of code and you can, by some means or
other compile a module. Pass the code through the preprocesser and it's
often quite a surprise how much stuff is included.

You may have reasons for trying to slim down the include
footprint. Sometimes it just makes things clearer and helps visualise
dependencies. Maybe you want to isolate a small part of a large project
and make something testable in isolation.

Regardless of the reasons for this there are already great tools to help,
one of which is creduce https://github.com/csmith-project/creduce

That works very well, is simple to use, and it's even packaged as part
of Debian.

That program is intended for paring down programs which produce compiler
errors but if you just say that the error is that your program won't
compile - and you put the headers into a file to reduce - then this
works well. The minor irritation being the preference for working with
preprocessed files.

There's also https://github.com/include-what-you-use/include-what-you-use 
which is again, already part of Debian.

Here's an attempt to do things a slightly different way. There's a 
system called tree-sitter which parses source files and produces output 
which is well suited to language servers for editors. It can classify 
chunks of source code into trees suitable for syntax highlighting, etc.

It does not claim to be perfect - but as you'll see - this doesn't matter 
for my application.

# An example

I have a `hello.c` C file in this directory with a header file, some
parts of which are used and some are not. The aim here is to remove the
unused parts of the header but - in order to keep things simple for me -
I run a C preprocessor pass (the type which does not expand macros).

Then I split the resulting file into two chunks: `head.c` and `body.c`
where body comprises the what was in the original `hello.c`

I concatenate head.c and body.c to a file called test.c - which still
compiles, of course.

I vibe coded a parser (which I intend to revisit later). This finds
the top level entities in any C file and outputs their offsets in the
file. I run that on head.c - the file which I intend to reduce.

Starting at the end of the list of entities (corresponding to points
furthest down in head.c) I have a loop which splats spaces over each and
tries to compile. If the compile succeeds then the new head.c is kept,
otherwise it's rejected and we move on to the next entity.

So, even if the parser does a bad job in some places, the badly formed
entities will be ignored if they break the compile. If they don't then
so much the better - they were not needed anyway :-)

All of these actions are in a simple script called doit.sh and there is
a cleanup script called clean.sh

# To run the example

Just type:

    ./doit.sh

And wait - maybe for quite a while ...

# Issues

Some of the more esoteric function declarations are being missed -
functions with attributes for example. Hence the need to understand
tree-sitter a bit better, and write my own parser. I'm starting to think
that it could form the basis for some really good refactoring tools.
