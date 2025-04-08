#ifndef MCSCRIPT_V3_EVALUATOR_H
#define MCSCRIPT_V3_EVALUATOR_H

#include <object.h>
#include <ast.h>
#include <memory>

std::shared_ptr<::Object> Eval(std::shared_ptr<::Node> node);
std::shared_ptr<::Object> EvalStatements(std::vector<std::shared_ptr<::Statement>> stmts);



#endif // MCSCRIPT_V3_EVALUATOR_H