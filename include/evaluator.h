#ifndef MCSCRIPT_V3_EVALUATOR_H
#define MCSCRIPT_V3_EVALUATOR_H

#include <object.h>
#include <ast.h>
#include <memory>
#include <gcollector.h>


class Evaluator {
  public:
    Evaluator(::GCollector& gCollector, Boolean* TRUE, Boolean* FALSE, Null* NULL_T)
     : gCollector_(gCollector), TRUE_(TRUE), FALSE_(FALSE), NULL_T_(NULL_T) {
      //empty
    }

    ~Evaluator();

    ::Object* Eval(std::shared_ptr<::Node> node);

    inline void TrackObject(Object* obj) {
      gCollector_.TrackObject(obj);
    }

    inline void CollectGarbage() {
      gCollector_.Collect();
    }

    inline Boolean* TRUE() {
      return TRUE_;
    }

    inline Boolean* FALSE() {
      return FALSE_;
    }

    inline Null* NULL_T() {
      return NULL_T_;
    }

  
  private:
    // garbage collector
    ::GCollector& gCollector_;
    Boolean* TRUE_;
    Boolean* FALSE_;
    Null* NULL_T_;

    // methods
    
    // helpers
    std::string GetTypeName_(std::shared_ptr<::Node> node);
    Boolean* NativeBooleanToBooleanObj_(bool input);
    bool IsTruthy_(Object* condition);

    // evals
    Object* EvalPrefixExpression_(std::string op, Object* right);
    Object* EvalBangExpression_(Object* right);
    Object* EvalMinusExpression_(Object* right);
    Object* EvalInfixExpression_(std::string op, Object* left, Object* right);
    Object* EvalIntegerInfixExpression_(std::string op, Object* left, Object* right);
    Object* EvalIfExpression_(std::shared_ptr<IfExpression> ie);
    Object* EvalProgram_(std::shared_ptr<Program> program);
    Object* EvalBlockStatement_(std::shared_ptr<BlockStatement> block);
};







#endif // MCSCRIPT_V3_EVALUATOR_H