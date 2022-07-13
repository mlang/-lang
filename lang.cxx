#include "scanner.hxx"
#include "parser.hxx"
int main() {
  mlang::scanner scanner;
  mlang::parser parse(scanner);
  parse();
}
