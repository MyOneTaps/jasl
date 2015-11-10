//
//  test.cpp
//  jasl
//
//  Created by Ben Jones 
//  Copyright (c) 2015 Ben Jones. All rights reserved.
//

#include "../SimpleTest.hpp"
#include "../CommandInterpretor.hpp"
#include "../GlobalCache.hpp"
#include "../SharedVarCache.hpp"

#include <sstream>
#include <vector>

namespace ll = jasl;
using namespace simpletest;

void testTypes()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("integer 5 -> a;", cache);
    ci.parseAndInterpretSingleCommand("decimal 1.1 -> b;", cache);
    ci.parseAndInterpretSingleCommand("boolean true -> c;", cache);
    ci.parseAndInterpretSingleCommand("string \"hello\" -> d;", cache);
    ci.parseAndInterpretSingleCommand("list [some big list] -> e;", cache);
    ci.parseAndInterpretSingleCommand("type(a) -> atype;", cache);
    ci.parseAndInterpretSingleCommand("type(b) -> btype;", cache);
    ci.parseAndInterpretSingleCommand("type(c) -> ctype;", cache);
    ci.parseAndInterpretSingleCommand("type(d) -> dtype;", cache);
    ci.parseAndInterpretSingleCommand("type(e) -> etype;", cache);
    ASSERT_EQUAL("integer", *cache->getString("atype"), "testType::a is integer");
    ASSERT_EQUAL("decimal", *cache->getString("btype"), "testType::b is decimal");
    ASSERT_EQUAL("boolean", *cache->getString("ctype"), "testType::c is boolean");
    ASSERT_EQUAL("string", *cache->getString("dtype"), "testType::d is string");
    ASSERT_EQUAL("list", *cache->getString("etype"), "testType::e is list");
}

void testMath()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("integer (10 + 20) -> result;", cache);
    ASSERT_EQUAL(30, *cache->getInt("result"), "testMath::result is 30");
    ci.parseAndInterpretSingleCommand("integer 2 -> x;", cache);
    ci.parseAndInterpretSingleCommand("integer (result * x) -> result;", cache);
    ASSERT_EQUAL(60, *cache->getInt("result"), "testMath::result is 60");
    ci.parseAndInterpretSingleCommand("decimal (result - 2.5) -> resultDouble;", cache);
    ASSERT_EQUAL(57.5, *cache->getDouble("resultDouble"), "testMath::resultDouble is 57.5");
}

void testVarNewSyntax()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("integer 1->i;", cache);
    ci.parseAndInterpretSingleCommand("decimal 1.1->j;", cache);
    ci.parseAndInterpretSingleCommand("boolean true->k;", cache);
    ASSERT_EQUAL(1, *cache->getInt("i"), "testVarNewSyntax::i is 1");
    ASSERT_EQUAL(1.1, *cache->getDouble("j"), "testVarNewSyntax::j is 1.1");
    ASSERT_EQUAL(true, *cache->getBool("k"), "testVarNewSyntax::k is true");
}

void testVarNewSyntaxFromString()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("integer \"1\" -> i;", cache);
    ci.parseAndInterpretSingleCommand("decimal \"1.1\" -> j;", cache);
    ci.parseAndInterpretSingleCommand("string \"2\" -> sint;", cache);
    ci.parseAndInterpretSingleCommand("string \"2.2\" -> sdouble;", cache);
    ci.parseAndInterpretSingleCommand("integer sint -> k;", cache);
    ci.parseAndInterpretSingleCommand("decimal sdouble -> l;", cache);
    ASSERT_EQUAL(1, *cache->getInt("i"), "testVarNewSyntaxFromString A");
    ASSERT_EQUAL(1.1, *cache->getDouble("j"), "testVarNewSyntaxFromString B");
    ASSERT_EQUAL(2, *cache->getInt("k"), "testVarNewSyntaxFromString C");
    ASSERT_EQUAL(2.2, *cache->getDouble("l"), "testVarNewSyntaxFromString D");
}

void testEchoLiterals()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("echo \"Hello, world!\";", cache, ss);
    ASSERT_EQUAL("Hello, world!", ss.str(), "testEcho: Hello, world!");
}

void testEchoNLLiterals()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("echo_nl \"Hello, world!\";", cache, ss);
    ASSERT_EQUAL("Hello, world!\n", ss.str(), "testEchoNL: Hello, world!");
}

void testEchoSymbols()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("integer 10 -> b;", cache);
    ci.parseAndInterpretSingleCommand("decimal 1.1 -> c;", cache);
    ci.parseAndInterpretSingleCommand("boolean true -> d;", cache);
    ci.parseAndInterpretSingleCommand("echo b;", cache, ss);
    ci.parseAndInterpretSingleCommand("echo c;", cache, ss);
    ci.parseAndInterpretSingleCommand("echo_nl d;", cache, ss);
    ASSERT_EQUAL("101.11\n", ss.str(), "testEchoSymbols");
}

void testEchoPrimitives()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("echo_nl 2;", cache, ss);
    ci.parseAndInterpretSingleCommand("echo_nl 5.67;", cache, ss);
    ci.parseAndInterpretSingleCommand("echo_nl 3.14;", cache, ss);
    ASSERT_EQUAL("2\n5.67\n3.14\n", ss.str(), "testEchoPrimitives");
}

void testEchoMath()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("echo_nl (5 + 2) * 5.6;", cache, ss);
    ASSERT_EQUAL("39.2\n", ss.str(), "testEchoMath");
}

void testEchoString()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string \"Hello, world!\" -> hello;", cache, ss);
    ci.parseAndInterpretSingleCommand("echo_nl hello;", cache, ss);
    ASSERT_EQUAL("Hello, world!\n", ss.str(), "testEchoString");
}

void testStringWithLiteral()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string \"Hello, world!\" -> hello;", cache, ss);
    ASSERT_EQUAL("Hello, world!", *cache->getString("hello"), "testStringWithLiteral");
}

void testStringWithNumbers()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string 5 -> numberFive;", cache, ss);
    ASSERT_EQUAL("5", *cache->getString("numberFive"), "testStringWithNumbers integer");
    ci.parseAndInterpretSingleCommand("string 5.5 -> numberDecimal;", cache, ss);
    ASSERT_EQUAL("5.5", *cache->getString("numberDecimal"), "testStringWithNumbers decimal");
}

void testStringWithMath()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string (5 + 2) * 5.6 -> numberMath;", cache, ss);
    ci.parseAndInterpretSingleCommand("string (5 + 2) * 5 -> numberMathB;", cache, ss);
    ASSERT_EQUAL("39.2", *cache->getString("numberMath"), "testStringWithMath A");
    ASSERT_EQUAL("35", *cache->getString("numberMathB"), "testStringWithMath B");
}

void testStringFromList()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string [hello there] -> listString;", cache, ss);
    ci.parseAndInterpretSingleCommand("list [another list] -> listB;", cache);
    ci.parseAndInterpretSingleCommand("string listB -> listStringB;", cache, ss);
    ASSERT_EQUAL("hello there", *cache->getString("listString"), "testStringFromList A");
    ASSERT_EQUAL("another list", *cache->getString("listStringB"), "testStringFromList B");
}

void testAppendLiteral()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string \"Hello\" -> s;", cache, ss);
    ci.parseAndInterpretSingleCommand("append (s, \", world!\") -> s;", cache, ss);
    ASSERT_EQUAL("Hello, world!", *cache->getString("s"), "testAppendLiteral");
}

void testAppendString()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string \"Hello\" -> s;", cache, ss);
    ci.parseAndInterpretSingleCommand("string \", world!\" -> b;", cache, ss);
    ci.parseAndInterpretSingleCommand("append (s, b) -> s;", cache, ss);
    ASSERT_EQUAL("Hello, world!", *cache->getString("s"), "testAppendString");
}

void testReverseString()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string \"Hello\" -> s;", cache, ss);
    ci.parseAndInterpretSingleCommand("string_reverse s;", cache, ss);
    ASSERT_EQUAL("olleH", *cache->getString("s"), "testReverseString");
}

void testStringLength()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string_length \"Hello\" -> s;", cache, ss);
    ci.parseAndInterpretSingleCommand("string \"Hello, world!\" -> sb;", cache, ss);
    ci.parseAndInterpretSingleCommand("string_length sb -> sc;", cache, ss);
    ASSERT_EQUAL(5, *cache->getInt("s"), "testStringLength A");
    ASSERT_EQUAL(13, *cache->getInt("sc"), "testStringLength B");
}

void testIfCommand()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("if(1 < 2) { echo \"Hello, world!\"; integer 5 -> x; }", cache, ss);
    ASSERT_EQUAL("Hello, world!", ss.str(), "testIfCommand: Hello, world!");
    ASSERT_EQUAL(5, *cache->getInt("x"), "testIfCommand: x is 5");
}

void testRepeatCommand()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    std::string const commands("integer 0 -> x; repeat 5 times { echo \"Hello\"; integer (x + 1) -> x; }");
    auto functions = ci.parseStringCollection(commands, cache);
    for(auto &f : functions) {
        (void)ci.interpretFunc(f, cache, ss);
    }
    ASSERT_EQUAL("HelloHelloHelloHelloHello", ss.str(), "testRepeatCommand::repeat hello");
    ASSERT_EQUAL(5, *cache->getInt("x"), "testRepeatCommand::x is 5");
}

void testWhileCommand()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    std::string const commands("integer 5 -> x; while(x > 0) { integer (x - 1) -> x; }");
    auto functions = ci.parseStringCollection(commands);
    for(auto &f : functions) {
        (void)ci.interpretFunc(f, cache, ss);
    }
    ASSERT_EQUAL(0, *cache->getInt("x"), "testWhileCommand::x is 0");
}

void testStartCommand()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    std::string const command("start { echo \"Hello\"; integer 21 -> x; }");
    ci.parseAndInterpretSingleCommand(command, cache, ss);
    ASSERT_EQUAL("Hello", ss.str(), "testStartCommand::print hello");
    ASSERT_EQUAL(21, *cache->getInt("x"), "testStartCommand::x is 21");
}

void testCallReturnableCommand()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    std::string commands("start { echo \"Starting..\"; integer 0 -> x; call foo (x) -> x; call bar (x) -> x; integer (x - 5) -> x;}");
    commands.append("returnable integer bar(x) -> x { echo \"..and Goodbye!\"; integer (x + 1) -> x; return x;}");
    commands.append("returnable integer foo(x) -> x { echo \"Hello\"; integer 20 -> x; return x;}");
    auto functions = ci.parseStringCollection(commands);
    for(auto &f : functions) {
        if(f.name == "start") {
            (void)ci.interpretFunc(f, cache, ss);
            break;
        }
    }
    ASSERT_EQUAL("Starting..Hello..and Goodbye!", ss.str(), "testCallReturnableCommand::print-outs");
    ASSERT_EQUAL(16, *cache->getInt("x"), "testCallReturnableCommand::x is 16");
}

void testCallBlockCommand()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    std::string commands("start { echo \"Starting..\"; call foo (); call bar ();}");
    commands.append("block bar() { echo \"..and Goodbye!\"; }");
    commands.append("block foo() { echo \"Hello\"; }");
    auto functions = ci.parseStringCollection(commands);
    for(auto &f : functions) {
        if(f.name == "start") {
            (void)ci.interpretFunc(f, cache, ss);
            break;
        }
    }
    ASSERT_EQUAL("Starting..Hello..and Goodbye!", ss.str(), "testCallBlockCommand::print-outs");
}

void testArgsCommand()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ll::GlobalCache::args.push_back("argument1");
    ll::GlobalCache::args.push_back("argument2");
    ci.parseAndInterpretSingleCommand("args 0 -> arga;", cache, ss);
    ci.parseAndInterpretSingleCommand("args 1 -> argb;", cache, ss);
    ASSERT_EQUAL("argument1", *cache->getString("arga"), "testArgsCommand A");
    ASSERT_EQUAL("argument2", *cache->getString("argb"), "testArgsCommand B");
}

void testList()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("list [hello there] -> s;", cache, ss);

    std::string tokenA;
    ll::VarExtractor::tryAnyCast(tokenA, cache->getListToken("s", 0));
    ASSERT_EQUAL("hello", tokenA, "testList tokenA");

    std::string tokenB;
    ll::VarExtractor::tryAnyCast(tokenB, cache->getListToken("s", 1));
    ASSERT_EQUAL("there", tokenB, "testList tokenB");

}

void testListGetTokenRaw()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("get_token (0, [hello there]) -> s;", cache, ss);
    ci.parseAndInterpretSingleCommand("integer 1 -> i;", cache);
    ci.parseAndInterpretSingleCommand("get_token (i, [hello there]) -> b;", cache, ss);
    ASSERT_EQUAL("hello", *cache->getString("s"), "testListGetTokenRaw A");
    ASSERT_EQUAL("there", *cache->getString("b"), "testListGetTokenRaw B");
}

void testListGetTokenSymbol()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("list [hello there] -> lst;", cache, ss);
    ci.parseAndInterpretSingleCommand("get_token (1, lst) -> s;", cache, ss);
    ci.parseAndInterpretSingleCommand("integer 0 -> i;", cache);
    ci.parseAndInterpretSingleCommand("get_token (i, lst) -> b;", cache, ss);
    ASSERT_EQUAL("there", *cache->getString("s"), "testListGetTokenSymbol A");
    ASSERT_EQUAL("hello", *cache->getString("b"), "testListGetTokenSymbol B");
}

void testListSetTokenRaw()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("set_token (0, [hello there], \"goodbye\") -> s;", cache, ss);
    std::string tokenA;
    ll::VarExtractor::tryAnyCast(tokenA, cache->getListToken("s", 0));
    ASSERT_EQUAL("goodbye", tokenA, "testListSetTokenRaw A");

    ci.parseAndInterpretSingleCommand("string \"yes\" -> b;", cache, ss);
    ci.parseAndInterpretSingleCommand("set_token (1, [hello there], b) -> s;", cache, ss);
    std::string tokenB;
    ll::VarExtractor::tryAnyCast(tokenB, cache->getListToken("s", 1));
    ASSERT_EQUAL("yes", tokenB, "testListSetTokenRaw B");
}

void testListSetTokenSymbol()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("list [hello there] -> la;", cache, ss);
    ci.parseAndInterpretSingleCommand("list [what are you doing] -> lb;", cache, ss);
    ci.parseAndInterpretSingleCommand("list [hello there] -> la;", cache, ss);
    ci.parseAndInterpretSingleCommand("integer 3 -> i;", cache, ss);
    ci.parseAndInterpretSingleCommand("integer 2 -> j;", cache, ss);
    ci.parseAndInterpretSingleCommand("set_token (0, la, \"goodbye\") -> sa;", cache, ss);
    ci.parseAndInterpretSingleCommand("set_token (i, lb, \"indeed\") -> sb;", cache, ss);

    std::string tokenA;
    ll::VarExtractor::tryAnyCast(tokenA, cache->getListToken("sa", 0));
    ASSERT_EQUAL("goodbye", tokenA, "testListSetTokenSymbol A");
    std::string tokenB;
    ll::VarExtractor::tryAnyCast(tokenB, cache->getListToken("sb", 3));
    ASSERT_EQUAL("indeed", tokenB, "testListSetTokenSymbol B");
    std::string tokenC;
    ll::VarExtractor::tryAnyCast(tokenC, cache->getListToken("lb", 2));
    ASSERT_EQUAL("you", tokenC, "testListSetTokenSymbol C");
    ci.parseAndInterpretSingleCommand("set_token (j, lb, \"no\") -> lb;", cache, ss);
    std::string tokenD;
    ll::VarExtractor::tryAnyCast(tokenD,  cache->getListToken("lb", 2));
    ASSERT_EQUAL("no", tokenD, "testListSetTokenSymbol D");

}

void testListAddToken()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("list [hello there] -> la;", cache);
    ci.parseAndInterpretSingleCommand("add_token(\"again\", la);", cache);
    ci.parseAndInterpretSingleCommand("echo la;", cache, ss);
    ASSERT_EQUAL("[hello there again]", ss.str(), "testListAddToken");
}

void testListTokenIndex()
{
    auto cache = std::make_shared<ll::ScopedVarCache>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("index_of (\"are\", [hello there what are you doing]) -> i;", cache, ss);
    ci.parseAndInterpretSingleCommand("string \"you\" -> token;", cache);
    ci.parseAndInterpretSingleCommand("index_of (token, [hello there what are you doing]) -> j;", cache, ss);
    ci.parseAndInterpretSingleCommand("list [hello there what are you doing] -> L;", cache);
    ci.parseAndInterpretSingleCommand("index_of (token, L) -> p;", cache, ss);
    ci.parseAndInterpretSingleCommand("index_of (token, [hello there what are you doing]) -> q;", cache, ss);
    ASSERT_EQUAL(3, *cache->getInt("i"), "index A");
    ASSERT_EQUAL(4, *cache->getInt("j"), "index B");
    ASSERT_EQUAL(4, *cache->getInt("p"), "index C");
    ASSERT_EQUAL(4, *cache->getInt("q"), "index D");
}

int main()
{
    testTypes();
    testMath();
    testVarNewSyntax();
    testVarNewSyntaxFromString();
    testEchoLiterals();
    testEchoNLLiterals();
    testEchoSymbols();
    testEchoPrimitives(); 
    testEchoMath();   
    testEchoString();  
    testStringWithLiteral();
    testStringWithNumbers();
    testStringWithMath();
    testStringFromList();
    testAppendLiteral();
    testAppendString();
    testReverseString();
    testStringLength();
    testIfCommand();
    testRepeatCommand();
    testWhileCommand();
    testStartCommand();
    testCallReturnableCommand();
    testCallBlockCommand();
    testArgsCommand();
    testList();
    testListTokenIndex();
    testListGetTokenRaw();
    testListGetTokenSymbol();
    testListSetTokenRaw();
    testListSetTokenSymbol();
    testListAddToken();
    showResults();
    return 0;
}
