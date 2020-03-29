#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <yuuki/compiler/feasy/syntax/syntax_ostream_writer.h>
#include <yuuki/compiler/feasy/syntax_context_manager.h>
#include <yuuki/compiler/diagnostics/diagnostic_stream.h>
#include <yuuki/compiler/feasy/lexer.h>
#include <yuuki/compiler/feasy/parser.h>
#include <yuuki/compiler/feasy/formatter.h>

#define YUUKI_CMD_TOOL_VERSION "0.0.1.0"
using namespace yuuki::compiler::feasy;
using namespace yuuki::compiler::feasy::token;
using namespace yuuki::compiler::diagnostics;
struct {
    std::vector<std::string> input;
    std::vector<std::string> output;
    bool showSyntax = false;
    bool format = false;
} options;

typedef std::uint64_t hash_t;
constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;
hash_t hash(char const* str)
{
    hash_t ret{basis};
    while(*str){
        ret ^= *str;
        ret *= prime;
        str++;
    }
    return ret;
}
constexpr hash_t hash_(char const* str, hash_t last_value = basis)
{
    return *str ? hash_(str+1, (*str ^ last_value) * prime) : last_value;
}
int main(int argc, char const *argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (*argv[i] == '-') {
            switch (hash(argv[i] + 1)) {
                case hash_("s"):
                case hash_("-showSyntax"):
                    options.showSyntax = true;
                    break;
                case hash_("f"):
                case hash_("-format"):
                    options.format = true;
                    break;
                case hash_("o"):
                case hash_("-out"):
                    while ((++i < argc) && (*argv[i] != '-'))
                        options.output.emplace_back(argv[i]);
                    break;
                case hash_("h"):
                case hash_("-help"):
                    std::cout << "usage: yuuki [input files ...] [options]" << std::endl
                              << "    -f, --format:" << std::endl
                              << "        format the given input yuuki source files." << std::endl
                              << "    -h, --help:" << std::endl
                              << "        show this help info." << std::endl
                              << "    -o, --out:" << std::endl
                              << "        specify output file name." << std::endl
                              << "    -s, --syntax:" << std::endl
                              << "        show the syntax tree of the input source file." << std::endl
                              << "    -v, --version:" << std::endl
                              << "        show version info." << std::endl;
                    return 0;
                case hash_("v"):
                case hash_("-version"):
                    std::cout << "Yuuki Command Line Tool Version " << YUUKI_CMD_TOOL_VERSION << std::endl;
                    return 0;
                default:
                    std::cout << "Unknown switch \"" << argv[i] << "\"" << std::endl
                              << "You can get help with: yuuki --help" << std::endl;
                    return 0;
            }
        } else {
            options.input.emplace_back(argv[i]);
        }

    }
    auto sm = std::make_shared<SyntaxContextManager>();
    auto d = std::make_shared<DiagnosticStream>(sm);
    for (int i = 0; i < options.input.size(); i++) {
        std::string ifname = options.input[i];
        std::ifstream infile(ifname);
        std::string str((std::istreambuf_iterator<char>(infile)),
                        std::istreambuf_iterator<char>());
        auto context = sm->create(str);
        auto lexer = Lexer(context, d);
        auto parser = Parser(context, d);
        lexer.lex();
        parser.parse();
        if (options.format) {
            Formatter f = Formatter(context);
            if (i < options.output.size()) {
                std::ofstream outfile(options.output[i]);
                outfile << f.format();
            } else {
                std::cout << f.format();
            }
        }
        if (options.showSyntax) {
            std::cout << *(context->syntaxTree.get());
        }
    }
    return 0;
}
