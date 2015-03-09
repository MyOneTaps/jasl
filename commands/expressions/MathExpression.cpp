#include "MathExpression.hpp"
#include "../../VarExtractor.hpp"

namespace lightlang {

    MathExpression::MathExpression()
    : errorMessage("")
    , resultIsInteger(true)
    {

    }

    double
    MathExpression::evaluate() 
    {

        // Note, for convenience, all math operations are done using doubles
        // The client of this function is responsible for casting to
        // an integer if integer precision is required
        double vleft;
        double vright;
        auto valA = VarExtractor::trySingleDoubleExtraction(left);

        // extracting a double didn't work. Maybe an integer
        if(!valA) {
            auto valC = VarExtractor::trySingleIntExtraction(left);
            if(valC) {
                vleft = *valC;
            } else {
                errorMessage = "Can't parse left of expression";
            }
        } else {
            resultIsInteger = false;
            vleft = *valA;
        }

        auto valB = VarExtractor::trySingleDoubleExtraction(right);

        // extracting a double didn't work. Maybe an integer
        if(!valB) {
            auto valD = VarExtractor::trySingleIntExtraction(right);
            if(valD) {
                vright = *valD;
            } else {
                errorMessage = "Can't parse right of expression";
            }
        } else {
            resultIsInteger = false;
            vright = *valB;
        }

        if (symbolOperator == "+" || symbolOperator == "add") {
            return vleft + vright;
        } else if (symbolOperator == "-" || symbolOperator == "sub") {
            return vleft - vright;
        } else if (symbolOperator == "/" || symbolOperator == "div") {
            return vleft / vright;
        } else if (symbolOperator == "*" || symbolOperator == "mult") {
            return vleft * vright;
        } else {
            errorMessage = "Unknown binary operand";
        }
    }
}