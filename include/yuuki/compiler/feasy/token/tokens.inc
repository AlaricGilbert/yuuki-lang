// Hack codes by using marcos to generate some repeated codes and prevent typos
// This file mostly defines the special tokens in the yuuki language.
// We can add SPECIAL_TOK_CLASSIFY(params) to get new special token types.
#ifndef TOKEN
#define TOKEN(X)
#endif

/* 
 * PUNCTUATOR:
 *   punctuator is a special of token, mostly used for split purposes
 */
#ifndef PUNCTUATOR
#define PUNCTUATOR(X,Y) TOKEN(X)
#endif

/* 
 * OPERATOR:
 *   operator is a special token, mostly used to indicate the operation
 *   of identifier elements
 */
#ifndef OPERATOR
#define OPERATOR(X,Y) TOKEN(op_ ## X)
#endif

/* 
 * KEYWORD:
 *   keyword is a special token, mostly reserved to represent syntax
 */
#ifndef KEYWORD
#define KEYWORD(X) TOKEN(kw_ ## X)
#endif
           
/* 
 * MODIFIER:
 *   modifier ia a special type of keyword that should only be used 
 *   for modification purpose for class, field or method
 */
#ifndef MODIFIER
#define MODIFIER(X) KEYWORD(X)
#endif


// compiler-internal tokens
TOKEN(unknown)              // unknown token for yuuki
TOKEN(eof)                  // end of file
TOKEN(inline_comment)       // in-line comment
TOKEN(interline_comment)    // inter-line comment


// yuuki language user input tokens
TOKEN(identifier)           // identifier like: _some_variable
TOKEN(numeric_const)        // numeric const like: 01237
TOKEN(string_const)         // string const
TOKEN(true_const)           // bool const that has value 'true'
TOKEN(false_const)          // bool const that has value 'false'


// operator definations
PUNCTUATOR(l_paren,       "(")
PUNCTUATOR(r_paren,       ")")
PUNCTUATOR(l_square,      "[")
PUNCTUATOR(r_square,      "]")
PUNCTUATOR(l_brace,       "{")
PUNCTUATOR(r_brace,       "}")
PUNCTUATOR(comma,         ",")
PUNCTUATOR(semi,          ";")

// operator definations
OPERATOR(amp,           "&")
OPERATOR(caret,         "^")
OPERATOR(colon,         ":")
OPERATOR(question,      "?")
OPERATOR(exclaim,       "!")
OPERATOR(tilde,         "~")
OPERATOR(equal,         "=")
OPERATOR(less,          "<")
OPERATOR(greater,       ">")
OPERATOR(modulus,       "%")
OPERATOR(period,        ".")
OPERATOR(plus,          "+")
OPERATOR(minus,         "-")
OPERATOR(multi,         "*")
OPERATOR(pipe,          "|")
OPERATOR(slash,         "/")
OPERATOR(ampamp,        "&&")
OPERATOR(ampeq,         "&=")
OPERATOR(careteq,       "^=")
OPERATOR(equaleq,       "==")
OPERATOR(exclaimeq,     "!=")
OPERATOR(greatereq,     ">=")
OPERATOR(greatergreater,">>")
OPERATOR(lesseq,        "<=")
OPERATOR(lessless,      "<<")
OPERATOR(minuseq,       "-=")
OPERATOR(minusminus,    "--")
OPERATOR(moduluseq,     "%=")
OPERATOR(multieq,       "*=")
OPERATOR(pluseq,        "+=")
OPERATOR(plusplus,      "++")
OPERATOR(pipeeq,        "|=")
OPERATOR(pipepipe,      "||")
OPERATOR(slasheq,       "/=")


// process control keywords
KEYWORD(if)
KEYWORD(else)
KEYWORD(while)
KEYWORD(continue)
KEYWORD(break)
KEYWORD(do)
KEYWORD(for)
KEYWORD(foreach)
KEYWORD(switch)
KEYWORD(case)
KEYWORD(default)
KEYWORD(goto)
KEYWORD(return)
// try-catch-throw statement keywords
KEYWORD(try)
KEYWORD(catch)
KEYWORD(throw)
// declaration limiter
KEYWORD(class)
KEYWORD(interface)
KEYWORD(enum)
// other keywords
KEYWORD(new)
KEYWORD(base)
KEYWORD(this)
KEYWORD(import)
KEYWORD(namespace)
KEYWORD(operator)

// Modifiers
MODIFIER(public)
MODIFIER(private)
MODIFIER(protected)
MODIFIER(sealed)
MODIFIER(naive)     // 蛤蛤
MODIFIER(static)
MODIFIER(override)
MODIFIER(virtual)


#undef TOKEN
#undef PUNCTUATOR
#undef KEYWORD
#undef MODIFIER
#undef OPERATOR