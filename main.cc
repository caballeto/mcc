#include <iostream>
#include <cstdio>

#include "src/Compiler.h"

#define AST_DUMP_FILE "ast_dump.txt"
#define ASSEMBY_FILE "out.s"

void Usage(char* arg) {
  std::cerr << "Usage: %s [-vcST] [-o out_file] file [file ...]" << std::endl;
  std::cerr << "       -v give verbose output of the compilation stages" << std::endl;
  std::cerr << "       -c generate object files but don't link them" << std::endl;
  std::cerr << "       -S generate assembly files but don't link them" << std::endl;
  std::cerr << "       -T dump the AST trees for each input file" << std::endl;
  std::cerr << "       -o outfile, produce the outfile executable file" << std::endl;
  exit(1);
}

std::string ChangeSuffix(const std::string& name, char expected, char change) {
  int size = name.length();

  if (size <= 2 || name[size - 1] != expected || name[size - 2] != '.') {
    std::cerr << "Error: invalid file name, can only accept '." << expected << "' file extension '"
              << name << "'." << std::endl;
    exit(1);
  }

  std::string out_file = name;
  out_file[size - 1] = change;
  return std::move(out_file);
}

std::string Assemble(const std::string& name) {
  std::string obj_file = ChangeSuffix(name, 's', 'o');
  std::string command = "as -o " + obj_file + " " + name;

  if (mcc::Compiler::fVerbose)
    std::cout << command << std::endl;

  int err = system(command.c_str());
  if (err != 0) {
    std::cerr << "Assembly of '" << name << "' failed." << std::endl;
    exit(1);
  }

  return obj_file;
}

std::string Compile(const std::string& name) {
  std::string out_file = ChangeSuffix(name, 'c', 's');
  mcc::Compiler::Compile(name, out_file);
  return std::move(out_file);
}

void Link(const std::string& out_file, const std::vector<std::string>& files) {
  std::string command = "cc -o " + out_file + " ./examples/printint.c";

  for (const auto& file : files)
    command += " " + file;
  //command += " ./examples/printint.c"; // external function hack

  if (mcc::Compiler::fVerbose)
    std::cout << command << std::endl;

  int err = system(command.c_str());
  if (err != 0) {
    std::cerr << "Linking failed." << std::endl;
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  mcc::Compiler::fDumpAst = false;
  mcc::Compiler::fKeepAsm = false;
  mcc::Compiler::fAssemble = false;
  mcc::Compiler::fVerbose = false;
  mcc::Compiler::fLink = true;

  std::string out_file = "a.out", asm_file, obj_file;
  std::vector<std::string> obj_files;

  int i;
  for (i = 1; i < argc; i++) {
    if (*argv[i] != '-')
      break;

    for (int j = 1; (*argv[i] == '-') && argv[i][j]; j++) {
      switch (argv[i][j]) {
        case 'o':
          out_file = argv[++i];	// Save & skip to next argument
          break;
        case 'T':
          mcc::Compiler::fDumpAst = true;
          break;
        case 'c':
          mcc::Compiler::fAssemble = true;
          mcc::Compiler::fKeepAsm = false;
          mcc::Compiler::fLink = false;
          break;
        case 'S':
          mcc::Compiler::fKeepAsm = true;
          mcc::Compiler::fAssemble = false;
          mcc::Compiler::fLink = false;
          break;
        case 'v':
          mcc::Compiler::fVerbose = true;
          break;
        default:
          Usage(argv[0]);
      }
    }
  }

  if (i >= argc)
    Usage(argv[0]);

  for (; i < argc; i++) {
    asm_file = Compile(argv[i]);

    if (mcc::Compiler::fLink || mcc::Compiler::fAssemble) {
      obj_files.push_back(Assemble(asm_file));
    }

    if (!mcc::Compiler::fKeepAsm) {
      remove(asm_file.c_str());
    }
  }

  if (mcc::Compiler::fLink) {
    Link(out_file, obj_files);
    if (!mcc::Compiler::fAssemble) {
      for (const auto& file : obj_files) {
        remove(file.c_str());
      }
    }
  }

  return 0;
}