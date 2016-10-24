//
//  test.cpp
//  jasl
//
//  Created by Ben Jones 
//  Copyright (c) 2015-2016 Ben Jones. All rights reserved.
//

#include "../SimpleTest.hpp"
#include "../CommandInterpretor.hpp"
#include "../caching/GlobalCache.hpp"
#include "../caching/SharedVarCache.hpp"

#include <sstream>
#include <vector>

namespace ll = jasl;
using namespace simpletest;

void testTypes()
{
    auto cache = std::make_shared<ll::CacheStack>();
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("int 5 -> a;", cache);
    ci.parseAndInterpretSingleCommand("real 1.1 -> b;", cache);
    ci.parseAndInterpretSingleCommand("bool true -> c;", cache);
    ci.parseAndInterpretSingleCommand("string \"hello\" -> d;", cache);
    ci.parseAndInterpretSingleCommand("list [some big list] -> e;", cache);
    ci.parseAndInterpretSingleCommand("array:int(5) -> f;", cache);
    ci.parseAndInterpretSingleCommand("array:real(5) -> g;", cache);
    ci.parseAndInterpretSingleCommand("type a -> atype;", cache);
    ci.parseAndInterpretSingleCommand("type b -> btype;", cache);
    ci.parseAndInterpretSingleCommand("type c -> ctype;", cache);
    ci.parseAndInterpretSingleCommand("type d -> dtype;", cache);
    ci.parseAndInterpretSingleCommand("type e -> etype;", cache);
    ci.parseAndInterpretSingleCommand("type f -> ftype;", cache);
    ci.parseAndInterpretSingleCommand("type g -> gtype;", cache);
    ASSERT_EQUAL("int", *cache->getVar<std::string>("atype", ll::Type::String), "testType::a is int");
    ASSERT_EQUAL("real", *cache->getVar<std::string>("btype", ll::Type::String), "testType::b is real");
    ASSERT_EQUAL("bool", *cache->getVar<std::string>("ctype", ll::Type::String), "testType::c is bool");
    ASSERT_EQUAL("string", *cache->getVar<std::string>("dtype", ll::Type::String), "testType::d is string");
    ASSERT_EQUAL("list", *cache->getVar<std::string>("etype", ll::Type::String), "testType::e is list");
    ASSERT_EQUAL("array:int", *cache->getVar<std::string>("ftype", ll::Type::String), "testType::f is array:int");
    ASSERT_EQUAL("array:real", *cache->getVar<std::string>("gtype", ll::Type::String), "testType::g is array:real");
}

void testMath()
{
    auto cache = std::make_shared<ll::CacheStack>();
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("int (10 + 20) -> result;", cache);
    ASSERT_EQUAL(30, *cache->getVar<int64_t>("result", ll::Type::Int), "testMath::result is 30");
    ci.parseAndInterpretSingleCommand("int 2 -> x;", cache);
    ci.parseAndInterpretSingleCommand("int (result * x) -> result;", cache);
    ASSERT_EQUAL(60, *cache->getVar<int64_t>("result", ll::Type::Int), "testMath::result is 60");
    ci.parseAndInterpretSingleCommand("real (result - 2.5) -> resultReal;", cache);
    ASSERT_EQUAL(57.5, *cache->getVar<double>("resultReal", ll::Type::Real), "testMath::resultReal is 57.5");
}

void testVarNewSyntax()
{
    auto cache = std::make_shared<ll::CacheStack>();
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("int 1->i;", cache);
    ci.parseAndInterpretSingleCommand("real 1.1->j;", cache);
    ci.parseAndInterpretSingleCommand("bool true->k;", cache);
    ASSERT_EQUAL(1, *cache->getVar<int64_t>("i", ll::Type::Int), "testVarNewSyntax::i is 1");
    ASSERT_EQUAL(1.1, *cache->getVar<double>("j", ll::Type::Real), "testVarNewSyntax::j is 1.1");
    ASSERT_EQUAL(true, *cache->getVar<bool>("k", ll::Type::Bool), "testVarNewSyntax::k is true");
}

void testVarNewSyntaxFromString()
{
    auto cache = std::make_shared<ll::CacheStack>();
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("int \"1\" -> i;", cache);
    ci.parseAndInterpretSingleCommand("real \"1.1\" -> j;", cache);
    ci.parseAndInterpretSingleCommand("string \"2\" -> sint;", cache);
    ci.parseAndInterpretSingleCommand("string \"2.2\" -> sdouble;", cache);
    ci.parseAndInterpretSingleCommand("int sint -> k;", cache);
    ci.parseAndInterpretSingleCommand("real sdouble -> l;", cache);
    ASSERT_EQUAL(1, *cache->getVar<int64_t>("i", ll::Type::Int), "testVarNewSyntaxFromString A");
    ASSERT_EQUAL(1.1, *cache->getVar<double>("j", ll::Type::Real), "testVarNewSyntaxFromString B");
    ASSERT_EQUAL(2, *cache->getVar<int64_t>("k", ll::Type::Int), "testVarNewSyntaxFromString C");
    ASSERT_EQUAL(2.2, *cache->getVar<double>("l", ll::Type::Real), "testVarNewSyntaxFromString D");
}

void testprLiterals()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("pr \"Hello, world!\";", cache, ss);
    ASSERT_EQUAL("Hello, world!", ss.str(), "testpr: Hello, world!");
}

void testprNLLiterals()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("prn \"Hello, world!\";", cache, ss);
    ASSERT_EQUAL("Hello, world!\n", ss.str(), "testprNL: Hello, world!");
}

void testprSymbols()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("int 10 -> b;", cache);
    ci.parseAndInterpretSingleCommand("real 1.1 -> c;", cache);
    ci.parseAndInterpretSingleCommand("bool true -> d;", cache);
    ci.parseAndInterpretSingleCommand("pr b;", cache, ss);
    ci.parseAndInterpretSingleCommand("pr c;", cache, ss);
    ci.parseAndInterpretSingleCommand("prn d;", cache, ss);
    ASSERT_EQUAL("101.11\n", ss.str(), "testprSymbols");
}

void testprPrimitives()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("prn 2;", cache, ss);
    ci.parseAndInterpretSingleCommand("prn 5.67;", cache, ss);
    ci.parseAndInterpretSingleCommand("prn 3.14;", cache, ss);
    ASSERT_EQUAL("2\n5.67\n3.14\n", ss.str(), "testprPrimitives");
}

void testprMath()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("prn (5 + 2) * 5.6;", cache, ss);
    ASSERT_EQUAL("39.2\n", ss.str(), "testprMath");
}

void testprString()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string \"Hello, world!\" -> hello;", cache, ss);
    ci.parseAndInterpretSingleCommand("prn hello;", cache, ss);
    ASSERT_EQUAL("Hello, world!\n", ss.str(), "testprString");
}

void testStringWithLiteral()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string \"Hello, world!\" -> hello;", cache, ss);
    ASSERT_EQUAL("Hello, world!", *cache->getVar<std::string>("hello", ll::Type::String), "testStringWithLiteral");
}

void testStringWithNumbers()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string 5 -> numberFive;", cache, ss);
    ASSERT_EQUAL("5", *cache->getVar<std::string>("numberFive", ll::Type::String), "testStringWithNumbers int");
    ci.parseAndInterpretSingleCommand("string 5.5 -> numberDecimal;", cache, ss);
    ASSERT_EQUAL("5.5", *cache->getVar<std::string>("numberDecimal", ll::Type::String), "testStringWithNumbers real");
}

void testStringWithMath()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string (5 + 2) * 5.6 -> numberMath;", cache, ss);
    ci.parseAndInterpretSingleCommand("string (5 + 2) * 5 -> numberMathB;", cache, ss);
    ASSERT_EQUAL("39.2", *cache->getVar<std::string>("numberMath", ll::Type::String), "testStringWithMath A");
    ASSERT_EQUAL("35", *cache->getVar<std::string>("numberMathB", ll::Type::String), "testStringWithMath B");
}

void testStringFromList()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string [hello there] -> listString;", cache, ss);
    ci.parseAndInterpretSingleCommand("list [another list] -> listB;", cache);
    ci.parseAndInterpretSingleCommand("string listB -> listStringB;", cache, ss);
    ASSERT_EQUAL("hello there", *cache->getVar<std::string>("listString", ll::Type::String), "testStringFromList A");
    ASSERT_EQUAL("another list", *cache->getVar<std::string>("listStringB", ll::Type::String), "testStringFromList B");
}

void testAppendLiteral()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string \"Hello\" -> s;", cache, ss);
    ci.parseAndInterpretSingleCommand("append (s, \", world!\") -> s;", cache, ss);
    ASSERT_EQUAL("Hello, world!", *cache->getVar<std::string>("s", ll::Type::String), "testAppendLiteral");
}

void testAppendString()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string \"Hello\" -> s;", cache, ss);
    ci.parseAndInterpretSingleCommand("string \", world!\" -> b;", cache, ss);
    ci.parseAndInterpretSingleCommand("append (s, b) -> s;", cache, ss);
    ASSERT_EQUAL("Hello, world!", *cache->getVar<std::string>("s", ll::Type::String), "testAppendString");
}

void testReverseString()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("string \"Hello\" -> s;", cache, ss);
    ci.parseAndInterpretSingleCommand("string_reverse s;", cache, ss);
    ASSERT_EQUAL("olleH", *cache->getVar<std::string>("s", ll::Type::String), "testReverseString");
}

void testStringLength()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("length \"Hello\" -> s;", cache, ss);
    ci.parseAndInterpretSingleCommand("string \"Hello, world!\" -> sb;", cache, ss);
    ci.parseAndInterpretSingleCommand("length sb -> sc;", cache, ss);
    ASSERT_EQUAL(5, *cache->getVar<int64_t>("s", ll::Type::Int), "testStringLength A");
    ASSERT_EQUAL(13, *cache->getVar<int64_t>("sc", ll::Type::Int), "testStringLength B");
}

void testIfCommand()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("if(1 < 2) { pr \"Hello, world!\"; int 5 -> x; }", cache, ss);
    ASSERT_EQUAL("Hello, world!", ss.str(), "testIfCommand: Hello, world!");
    ASSERT_EQUAL(5, *cache->getVar<int64_t>("x", ll::Type::Int), "testIfCommand: x is 5");
}

void testRepeatCommand()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    std::string const commands("int 0 -> x; repeat 5 times { pr \"Hello\"; int (x + 1) -> x; }");
    auto functions = ci.parseStringCollection(commands, cache);
    for(auto &f : functions) {
        (void)ci.interpretFunc(f, cache, ss);
    }
    ASSERT_EQUAL("HelloHelloHelloHelloHello", ss.str(), "testRepeatCommand::repeat hello");
    ASSERT_EQUAL(5, *cache->getVar<int64_t>("x", ll::Type::Int), "testRepeatCommand::x is 5");
}

void testWhileCommand()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    std::string const commands("int 5 -> x; while(x > 0) { int (x - 1) -> x; }");
    auto functions = ci.parseStringCollection(commands);
    for(auto &f : functions) {
        (void)ci.interpretFunc(f, cache, ss);
    }
    ASSERT_EQUAL(0, *cache->getVar<int64_t>("x", ll::Type::Int), "testWhileCommand::x is 0");
}

void testStartCommand()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    std::string const command("start { pr \"Hello\"; int 21 -> x; }");
    ci.parseAndInterpretSingleCommand(command, cache, ss);
    ASSERT_EQUAL("Hello", ss.str(), "testStartCommand::print hello");
    ASSERT_EQUAL(21, *cache->getVar<int64_t>("x", ll::Type::Int), "testStartCommand::x is 21");
}

void testCallReturnableCommand()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    std::string commands("start { pr \"Starting..\"; int 0 -> x; call foo (x) -> x; call bar (x) -> x; int (x - 5) -> x;}");
    commands.append("fn:int bar(x) -> x { pr \"..and Goodbye!\"; int (x + 1) -> x; }");
    commands.append("fn:int foo(x) -> x { pr \"Hello\"; int 20 -> x;}");
    auto functions = ci.parseStringCollection(commands);
    for(auto &f : functions) {
        if(f.name == "start") {
            (void)ci.interpretFunc(f, cache, ss);
            break;
        }
    }
    ASSERT_EQUAL("Starting..Hello..and Goodbye!", ss.str(), "testCallReturnableCommand::print-outs");
    ASSERT_EQUAL(16, *cache->getVar<int64_t>("x", ll::Type::Int), "testCallReturnableCommand::x is 16");
}

void testCallBlockCommand()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    std::string commands("start { pr \"Starting..\"; call foo (); call bar ();}");
    commands.append("block bar() { pr \"..and Goodbye!\"; }");
    commands.append("block foo() { pr \"Hello\"; }");
    auto functions = ci.parseStringCollection(commands);
    for(auto &f : functions) {
        if(f.name == "start") {
            (void)ci.interpretFunc(f, cache, ss);
            break;
        }
    }
    ASSERT_EQUAL("Starting..Hello..and Goodbye!", ss.str(), "testCallBlockCommand::print-outs");
}

void testCallFunctionNil()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    std::string commands("start { pr \"Starting..\"; call foo (); call bar ();}");
    commands.append("fn:nil bar() { pr \"..and Goodbye!\"; }");
    commands.append("fn:nil foo() { pr \"Hello\"; }");
    auto functions = ci.parseStringCollection(commands);
    for(auto &f : functions) {
        if(f.name == "start") {
            (void)ci.interpretFunc(f, cache, ss);
            break;
        }
    }
    ASSERT_EQUAL("Starting..Hello..and Goodbye!", ss.str(), "testCallFunctionNil::print-outs");
}

void testArgsCommand()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ll::GlobalCache::args.push_back("argument1");
    ll::GlobalCache::args.push_back("argument2");
    ci.parseAndInterpretSingleCommand("args 0 -> arga;", cache, ss);
    ci.parseAndInterpretSingleCommand("args 1 -> argb;", cache, ss);
    ASSERT_EQUAL("argument1", *cache->getVar<std::string>("arga", ll::Type::String), "testArgsCommand A");
    ASSERT_EQUAL("argument2", *cache->getVar<std::string>("argb", ll::Type::String), "testArgsCommand B");
}

void testList()
{
    auto cache = std::make_shared<ll::CacheStack>();
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
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("get_token (0, [hello there]) -> s;", cache, ss);
    ci.parseAndInterpretSingleCommand("int 1 -> i;", cache);
    ci.parseAndInterpretSingleCommand("get_token (i, [hello there]) -> b;", cache, ss);
    ASSERT_EQUAL("hello", *cache->getVar<std::string>("s", ll::Type::String), "testListGetTokenRaw A");
    ASSERT_EQUAL("there", *cache->getVar<std::string>("b", ll::Type::String), "testListGetTokenRaw B");
}

void testListGetTokenSymbol()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("list [hello there] -> lst;", cache, ss);
    ci.parseAndInterpretSingleCommand("get_token (1, lst) -> s;", cache, ss);
    ci.parseAndInterpretSingleCommand("int 0 -> i;", cache);
    ci.parseAndInterpretSingleCommand("get_token (i, lst) -> b;", cache, ss);
    ASSERT_EQUAL("there", *cache->getVar<std::string>("s", ll::Type::String), "testListGetTokenSymbol A");
    ASSERT_EQUAL("hello", *cache->getVar<std::string>("b", ll::Type::String), "testListGetTokenSymbol B");
}

void testListSetTokenRaw()
{
    auto cache = std::make_shared<ll::CacheStack>();
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
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("list [hello there] -> la;", cache, ss);
    ci.parseAndInterpretSingleCommand("list [what are you doing] -> lb;", cache, ss);
    ci.parseAndInterpretSingleCommand("list [hello there] -> la;", cache, ss);
    ci.parseAndInterpretSingleCommand("int 3 -> i;", cache, ss);
    ci.parseAndInterpretSingleCommand("int 2 -> j;", cache, ss);
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
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("list [hello there] -> la;", cache);
    ci.parseAndInterpretSingleCommand("add_token(\"again\", la);", cache);
    ci.parseAndInterpretSingleCommand("pr la;", cache, ss);
    ASSERT_EQUAL("[hello there again]", ss.str(), "testListAddToken");
}

void testListTokenIndex()
{
    auto cache = std::make_shared<ll::CacheStack>();
    std::ostringstream ss;
    ll::CommandInterpretor ci;
    ci.parseAndInterpretSingleCommand("index_of (\"are\", [hello there what are you doing]) -> i;", cache, ss);
    ci.parseAndInterpretSingleCommand("string \"you\" -> token;", cache);
    ci.parseAndInterpretSingleCommand("index_of (token, [hello there what are you doing]) -> j;", cache, ss);
    ci.parseAndInterpretSingleCommand("list [hello there what are you doing] -> L;", cache);
    ci.parseAndInterpretSingleCommand("index_of (token, L) -> p;", cache, ss);
    ci.parseAndInterpretSingleCommand("index_of (token, [hello there what are you doing]) -> q;", cache, ss);
    ASSERT_EQUAL(3, *cache->getVar<int64_t>("i", ll::Type::Int), "index A");
    ASSERT_EQUAL(4, *cache->getVar<int64_t>("j", ll::Type::Int), "index B");
    ASSERT_EQUAL(4, *cache->getVar<int64_t>("p", ll::Type::Int), "index C");
    ASSERT_EQUAL(4, *cache->getVar<int64_t>("q", ll::Type::Int), "index D");
}

int main()
{
    testTypes();
    testMath();
    testVarNewSyntax();
    testVarNewSyntaxFromString();
    testprLiterals();
    testprNLLiterals();
    testprSymbols();
    testprPrimitives(); 
    testprMath();   
    testprString();  
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
    testCallFunctionNil();
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
