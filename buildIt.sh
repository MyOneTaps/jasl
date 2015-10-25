#!/bin/bash 
# set build flags
compiler=clang++
cflags="-std=c++11 -O3 -ffast-math -funroll-loops -Wno-ctor-dtor-privacy"
boostIncPath=/usr/local/include

# these objects will exist after compilation of everything
objects="GlobalCache.o \
         ScopedVarCache.o \
         Command.o \
         AppendCommand.o \
         ArgsCommand.o \
         EchoNLCommand.o \
         EchoCommand.o \
         ExecCommand.o \
         IfCommand.o \
         RepeatCommand.o \
         WhileCommand.o \
         ForCommand.o \
         BlockCommand.o \
         StartCommand.o \
         CallCommand.o \
         ReturnableCommand.o \
         MathExpression.o \
         ComparisonExpression.o \
         CommandInterpretor.o"

function build_GlobalCache {
    echo "Building GlobalCache..."
    $compiler $cflags -I$boostIncPath -c GlobalCache.cpp -o GlobalCache.o 
}

function build_ScopedVarCache {
    echo "Building ScopedVarCache..."
    $compiler $cflags -I$boostIncPath -c ScopedVarCache.cpp -o ScopedVarCache.o 
}

function build_Command {
    echo "Building Command..."
    $compiler $cflags -I$boostIncPath -c commands/Command.cpp -o Command.o 
}

function build_AppendCommand {
    echo "Building AppendCommand..."
    $compiler $cflags -I$boostIncPath -c commands/AppendCommand.cpp -o AppendCommand.o 
}

function build_ArgsCommand {
    echo "Building ArgsCommand..."
    $compiler $cflags -I$boostIncPath -c commands/ArgsCommand.cpp -o ArgsCommand.o 
}

function build_CallCommand {
    echo "Building CallCommand..."
    $compiler $cflags -I$boostIncPath -c commands/CallCommand.cpp -o CallCommand.o 
}

function build_EchoCommand {
    echo "Building EchoCommand..."
    $compiler $cflags -I$boostIncPath -c commands/EchoCommand.cpp -o EchoCommand.o 
}

function build_EchoNLCommand {
    echo "Building EchoNLCommand..."
    $compiler $cflags -I$boostIncPath -c commands/EchoNLCommand.cpp -o EchoNLCommand.o 
}

function build_ExecCommand {
    echo "Building ExecCommand..."
    $compiler $cflags -I$boostIncPath -c commands/ExecCommand.cpp -o ExecCommand.o 
}

function build_ReturnableCommand {
    echo "Building ReturnableCommand..."
    $compiler $cflags -I$boostIncPath -c commands/ReturnableCommand.cpp -o ReturnableCommand.o 
}

function build_IfCommand {
    echo "Building IfCommand..."
    $compiler $cflags -I$boostIncPath -c commands/IfCommand.cpp -o IfCommand.o 
}

function build_RepeatCommand {
    echo "Building RepeatCommand..."
    $compiler $cflags -I$boostIncPath -c commands/RepeatCommand.cpp -o RepeatCommand.o 
}

function build_WhileCommand {
    echo "Building WhileCommand..."
    $compiler $cflags -I$boostIncPath -c commands/WhileCommand.cpp -o WhileCommand.o 
}

function build_ForCommand {
    echo "Building ForCommand..."
    $compiler $cflags -I$boostIncPath -c commands/ForCommand.cpp -o ForCommand.o 
}

function build_StartCommand {
    echo "Building StartCommand..."
    $compiler $cflags -I$boostIncPath -c commands/StartCommand.cpp -o StartCommand.o 
}

function build_BlockCommand {
    echo "Building BlockCommand..."
    $compiler $cflags -I$boostIncPath -c commands/BlockCommand.cpp -o BlockCommand.o 
}

function build_MathExpression {
    echo "Building MathExpression..."
    $compiler $cflags -I$boostIncPath -c commands/expressions/MathExpression.cpp -o MathExpression.o 
}

function build_ComparisonExpression {
    echo "Building ComparisonExpression..."
    $compiler $cflags -I$boostIncPath -c commands/expressions/ComparisonExpression.cpp -o ComparisonExpression.o 
}

function build_CommandInterpretor {
    echo "Building CommandInterpretor..."
    $compiler $cflags -I$boostIncPath -c CommandInterpretor.cpp -o CommandInterpretor.o
}

function build_tests {
    echo "Building tests..."
    $compiler $cflags -I$boostIncPath $objects test.cpp -o test
}

function build_jasl {
    echo "Building jasl..."
    $compiler $cflags -I$boostIncPath $objects jasl.cpp -o jasl
}

function build_clean {
    echo "Removing compiled items..."
    rm *.o
    rm test
    rm jasl
}

function build_all {
    build_GlobalCache
    build_ScopedVarCache
    build_Command
    build_AppendCommand
    build_ArgsCommand
    build_CallCommand
    build_EchoCommand
    build_EchoNLCommand
    build_ExecCommand
    build_ReturnableCommand
    build_IfCommand
    build_RepeatCommand
    build_WhileCommand
    build_ForCommand
    build_StartCommand
    build_BlockCommand
    build_MathExpression
    build_ComparisonExpression
    build_CommandInterpretor
    build_tests
    build_jasl
}

# control which parts we need to be built.
# For example "./sh buildIt.sh all" will build everything
# while "./sh buildIt.sh tests" will attempt to build the tests
build_$1
