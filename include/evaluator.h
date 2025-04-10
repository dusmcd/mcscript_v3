#ifndef MCSCRIPT_V3_EVALUATOR_H
#define MCSCRIPT_V3_EVALUATOR_H

#include <object.h>
#include <ast.h>
#include <memory>


static const std::shared_ptr<Boolean> TRUE = std::make_shared<Boolean>(true);
static const std::shared_ptr<Boolean> FALSE = std::make_shared<Boolean>(false);

std::shared_ptr<::Object> Eval(std::shared_ptr<::Node> node);

std::shared_ptr<::Object> EvalStatements(std::vector<std::shared_ptr<::Statement>> stmts);
std::string GetTypeName(std::shared_ptr<::Node> node);





#endif // MCSCRIPT_V3_EVALUATOR_H