#ifndef YUUKI_COMPILE_ERRORS_H
#define YUUKI_COMPILE_ERRORS_H
namespace yuuki::compiler::diagnostics{
    // Pre-defined compile errors for yuuki compiler.
    enum class CompileError{
        // YK1000-1999: lexical errors
        UnexpectedChars = 1000,
        StringNotClosed,
        InvalidNumericConst,
        InterlineCommentNotClosed,
        // YK2000-2999: parse errors
        UnexpectedToken = 2000,
        SemiExpected,
        CommaExpected,
        LBraceExpected,
        RBraceExpected,
        LParenExpected,
        RParenExpected,
        RSquareExpected,
        GreaterExpected,
        IdentifierExpected,
        ImportExpected,
        ColonExpected,

    };
}
#endif //YUUKI_COMPILE_ERRORS_H
