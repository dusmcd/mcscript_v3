#ifndef MCSCRIPT_V3_EVALUATOR_H
#define MCSCRIPT_V3_EVALUATOR_H

#include <object.h>
#include <ast.h>
#include <memory>
#include <gcollector.h>
#include <environment.h>


class Evaluator {
  public:
    Evaluator(
      ::GCollector& gCollector,
       Boolean* TRUE,
       Boolean* FALSE,
       Null* NULL_T,
       std::unordered_map<std::string, BuiltIn*> builtInFuncs)
     : gCollector_(gCollector), TRUE_(TRUE), FALSE_(FALSE), NULL_T_(NULL_T) {
      builtInFuncs_ = builtInFuncs;
    }

    ~Evaluator();

    ::Object* Eval(std::shared_ptr<::Node> node, std::shared_ptr<Environment<Object*>> env);

    inline void TrackObject(Object* obj) {
      gCollector_.TrackObject(obj);
    }

    inline void CollectGarbage() {
      gCollector_.Collect();
    }

    inline void FinalCleanup() {
      gCollector_.CollectAll();
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

    inline size_t GetNumObjects() const {
      return gCollector_.GetNumObjects();
    }

  
  private:
    // garbage collector
    ::GCollector& gCollector_;
    Boolean* TRUE_;
    Boolean* FALSE_;
    Null* NULL_T_;
    std::unordered_map<std::string, BuiltIn*> builtInFuncs_;

    // methods
    
    // helpers
    std::string GetTypeName_(std::shared_ptr<::Node> node);
    Boolean* NativeBooleanToBooleanObj_(bool input);
    bool IsTruthy_(Object* condition);
    Error* NewError_(std::string message);
    std::string GetInfixErrorMsg_(const char* format, Object* left, std::string op, Object* right);
    std::string GetPrefixErrorMsg_(const char* format, std::string op, Object* right);
    bool IsError_(Object* obj);
    Object* NewObject_(Object* obj);

    // evals
    Object* EvalPrefixExpression_(std::string op, Object* right);
    Object* EvalBangExpression_(Object* right);
    Object* EvalMinusExpression_(Object* right);
    Object* EvalInfixExpression_(std::string op, Object* left, Object* right);
    Object* EvalIntegerInfixExpression_(std::string op, Object* left, Object* right);
    Object* EvalStringInfixExpression_(std::string op, Object* left, Object* right);
    Object* EvalIfExpression_(std::shared_ptr<IfExpression> ie, std::shared_ptr<Environment<Object*>> env);
    Object* EvalProgram_(std::shared_ptr<Program> program, std::shared_ptr<Environment<Object*>> env);
    Object* EvalBlockStatement_(std::shared_ptr<BlockStatement> block, std::shared_ptr<Environment<Object*>> env);
    Object* EvalIdentifier_(std::string name, std::shared_ptr<Environment<Object*>> env);
    std::vector<Object*> EvalParameters_(std::shared_ptr<Environment<Object*>> env, std::vector<std::shared_ptr<Expression>> params);
    Object* EvalFunctionCall_(Object* function, std::vector<Object*> args, std::shared_ptr<Environment<Object*>> outerEnv);

    Object* EvalBuiltInFuncCall_(BuiltIn* function, std::vector<Object*> args);
};







#endif // MCSCRIPT_V3_EVALUATOR_H