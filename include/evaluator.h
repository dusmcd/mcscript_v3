#ifndef MCSCRIPT_V3_EVALUATOR_H
#define MCSCRIPT_V3_EVALUATOR_H

#include <object.h>
#include <ast.h>
#include <memory>
#include <gcollector.h>


static Boolean* TRUE = new Boolean(true);
static Boolean* FALSE = new Boolean(false);
static Null* NULL_T = new Null();

class Evaluator {
  public:
    Evaluator(::GCollector& gCollector) : gCollector_(gCollector) {
      //empty
    }

    ::Object* Eval(std::shared_ptr<::Node> node);

    inline void TrackObject(Object* obj) {
      gCollector_.TrackObject(obj);
    }

    inline void CollectGarbage() {
      gCollector_.Collect();
    }
  
  private:
    // garbage collector
    ::GCollector& gCollector_;

    // methods
    Object* EvalStatements_(std::vector<std::shared_ptr<::Statement>> stmts);
    std::string GetTypeName_(std::shared_ptr<::Node> node);

    Object* EvalPrefixExpression_(std::string op, Object* right);
    Object* EvalBangExpression_(Object* right);
    Object* EvalMinusExpression_(Object* right);
};







#endif // MCSCRIPT_V3_EVALUATOR_H