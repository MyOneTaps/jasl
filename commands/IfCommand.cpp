#include "IfCommand.hpp"
#include "expressions/ComparisonExpression.hpp"
#include "../CommandInterpretor.hpp"
#include "../VarExtractor.hpp"
#include <vector>

namespace lightlang
{

    IfCommand::IfCommand(Function &func_, OptionalOutputStream const &output)
    : Command(func_, output)
    {
    }

    bool IfCommand::execute() 
    {
        return interpretFunctionBody();
    }

    bool IfCommand::interpretFunctionBody()
    {
        bool success = true;

        // Extract the boolean condition of the if-statement
        ComparisonExpression ce;
        if (!VarExtractor::tryExtraction<ComparisonExpression>(ce, m_func.paramA)) {
            return false;
        }

        // Basically do the if statement and then parse the functions
        // within the if block
        if (ce.evaluate()) {
            std::vector<Function> innerFuncs;
            success = VarExtractor::tryAnyCast<std::vector<Function>>(innerFuncs, m_func.paramB);
            if (success) {
                success = parseCommands(innerFuncs);
            } else {
                m_errorMessage = "IfCommand: Error interpreting if statement";
                appendToOutput(m_errorMessage);
                return false;
            }
        }
        return success;
    }

    bool IfCommand::parseCommands(std::vector<Function> &functions) 
    {
        CommandInterpretor ci;
        for(auto & f : functions) {
            (void)ci.interpretFunc(f, m_outputStream);
        }
        return true;
    }
}