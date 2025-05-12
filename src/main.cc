#include "environment.h"
#include <lexer.h>
#include <memory>
#include <parser.h>
#include <evaluator.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


struct FileData {
  char* sourceCode;
  size_t fileSize;
};

void PrintParserErrors(std::vector<std::string> errs) {
  for (const std::string& err : errs) {
    std::cerr << err << "\n";
  }
}

std::shared_ptr<Evaluator> NewEval() {
  GCollector& gCollector = GCollector::getGCollector();
  Boolean* TRUE = new Boolean(true);
  Boolean* FALSE = new Boolean(false);
  Null* NULL_T = new Null();
  std::unordered_map<std::string, BuiltIn*> builtInFuncs = GetBuiltIns();

  auto evaluator = std::make_shared<Evaluator>(gCollector, TRUE, FALSE, NULL_T, builtInFuncs);

  return evaluator;
}

FileData ReadFile(char* fileName) {
  int fd = open(fileName, O_RDONLY);
  if (fd == -1) {
    std::cerr << "file open error\n";
    return (FileData){.sourceCode = nullptr, .fileSize = 0};
  }

  struct stat sb;
  if (fstat(fd, &sb) == -1) {
    std::cerr << "could not get file size\n";
    return (FileData){.sourceCode = nullptr, .fileSize = 0};
  }

  size_t fileSize = sb.st_size;

  // MEMORY MAPPING FILE
  char* mapped = static_cast<char*>(mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0));

  // CLOSING FILE
  if (close(fd) == -1) {
    std::cerr << "error closing file\n";
    return (FileData){.sourceCode = nullptr, .fileSize = 0};
  }

  return (FileData){.sourceCode = mapped, .fileSize = fileSize};
}

void RunRepl(std::shared_ptr<Evaluator> evaluator, std::shared_ptr<Environment<Object*>> env) {
  std::cout << "McScript v3.0 Programming Language\n";
  std::cout << "Enter commands: (type 'exit' to terminate)\n";

  while (true) {
    if (evaluator->GetNumObjects() > 10) {
      evaluator->CollectGarbage();
    }
    std::cout << ">> ";
    std::string input;
    std::getline(std::cin, input);

    if (input.compare("exit") == 0) {
      break;
    }

    auto l = std::make_shared<Lexer>(input.c_str());
    auto p = std::make_shared<Parser>(l);
    std::shared_ptr<Program> program = p->ParseProgram();

    if (p->GetErrors().size() > 0) {
      PrintParserErrors(p->GetErrors());
    }
    

    Object* obj = evaluator->Eval(program, env);
    if (obj != nullptr && obj->Type() == ObjectType::ERROR_OBJ) {
      std::cout << obj->Inspect() << "\n";
    }
  }

}

int main(int argc, char** argv) {
  auto env = std::make_shared<Environment<Object*>>();
  std::shared_ptr<Evaluator> evaluator = NewEval();

  if (argc == 1) {
    RunRepl(evaluator, env);
  }

  else if (argc > 2) {
    std::cerr << "ERROR: too many arguments\n";
    return -1;
  }
  else {
    FileData fileData = ReadFile(argv[1]);
    if (fileData.sourceCode == nullptr) {
      return 1;
    }

    auto l = std::make_shared<Lexer>(fileData.sourceCode);
    auto p = std::make_shared<Parser>(l);
    std::shared_ptr<Program> program = p->ParseProgram();

    Object* obj = evaluator->Eval(program, env);
    if (obj != nullptr && obj->Type() == ObjectType::ERROR_OBJ) {
      std::cerr << obj->Inspect() << "\n";
    }

    if (munmap(fileData.sourceCode, fileData.fileSize) == -1) {
      std::cerr << "error unmapping file\n";
      return 2;
    }
  }

  evaluator->FinalCleanup();
  return 0;
}
