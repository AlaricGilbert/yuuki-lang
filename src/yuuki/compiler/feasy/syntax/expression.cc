#include <yuuki/compiler/feasy/syntax/expression.h>
#include <rang/rang.h>
#include <yuuki/compiler/feasy/token/token_util.h>

namespace yuuki::compiler::feasy::syntax {

    SyntaxType ExplicitCastExpression::getType() {
        return SyntaxType::ExplicitCastExpression;
    }

    bool ExplicitCastExpression::hasChild() {
        return true;
    }

    void ExplicitCastExpression::writeCurrentInfo(std::ostream &ostream) {
        Expression::writeCurrentInfo(ostream);
    }

    std::size_t ExplicitCastExpression::start() {
        return _leftParenIndex;
    }

    std::size_t ExplicitCastExpression::end() {
        if (_operand->getType() != SyntaxType::NullExpression)
            return _operand->end();
        return _rightParenIndex;
    }

    void
    ExplicitCastExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_targetType, false);
        syntaxWalker(_operand, true);
    }

    void ExplicitCastExpression::analyseType() {

    }

    std::string ExplicitCastExpression::toString() {
        return "(" + _targetType->toString() + ") " + _operand->toString();
    }

    ExplicitCastExpression::ExplicitCastExpression(std::size_t lParenIndex, std::size_t rParenIndex,
                                                   const std::shared_ptr<Type> &targetType,
                                                   const std::shared_ptr<Expression> &operand) {
        _leftParenIndex = lParenIndex;
        _rightParenIndex = rParenIndex;
        _targetType = targetType;
        _operand = operand;
    }


    NameExpression::NameExpression(const std::shared_ptr<Name> &name) {
        _name = name;
    }

    void NameExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_name, true);
    }

    void NameExpression::writeCurrentInfo(std::ostream &s) {
        Expression::writeCurrentInfo(s);
    }

    SyntaxType NameExpression::getType() {
        return SyntaxType::NameExpression;
    }

    bool NameExpression::hasChild() {
        return true;
    }

    void NameExpression::analyseType() {

    }

    std::size_t NameExpression::start() {
        return _name->start();
    }

    std::size_t NameExpression::end() {
        return _name->end();
    }

    std::string NameExpression::toString() {
        return _name->toString();
    }

    void Expression::writeCurrentInfo(std::ostream &s) {
        if (s.rdbuf() == std::cout.rdbuf()) {
            s << rang::fg::gray << getType()
              << rang::fg::yellow << "<" << this << "> "
              << rang::fg::green << "'" << toString() << "' "
              << rang::fg::reset << std::endl;
        } else {
            s << getType()
              << "<" << this << "> "
              << "'" << toString() << "' " << std::endl;
        }
    }

    ParenthesesExpression::ParenthesesExpression(std::size_t lParenIndex, std::size_t rParenIndex,
                                                 const std::shared_ptr<Expression> &innerExpr) {
        _leftParenIndex = lParenIndex;
        _rightParenIndex = rParenIndex;
        _innerExpression = innerExpr;
    }

    void ParenthesesExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_innerExpression, true);
    }

    void ParenthesesExpression::writeCurrentInfo(std::ostream &ostream) {
        Expression::writeCurrentInfo(ostream);
    }

    SyntaxType ParenthesesExpression::getType() {
        return SyntaxType::ParenthesesExpression;
    }

    std::size_t ParenthesesExpression::start() {
        return _leftParenIndex;
    }

    std::size_t ParenthesesExpression::end() {
        if(_rightParenIndex!=invalidTokenIndex)
            return _rightParenIndex;
        if(_innerExpression->getType()!=SyntaxType::NullExpression)
            return _innerExpression->end();
        return _leftParenIndex;
    }

    bool ParenthesesExpression::hasChild() {
        return true;
    }

    void ParenthesesExpression::analyseType() {

    }

    std::string ParenthesesExpression::toString() {
        return "("+_innerExpression->toString()+")";
    }

    void NullExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {

    }

    void NullExpression::writeCurrentInfo(std::ostream &ostream) {
        Expression::writeCurrentInfo(ostream);
    }

    SyntaxType NullExpression::getType() {
        return SyntaxType::NullExpression;
    }

    std::size_t NullExpression::start() {
        return invalidTokenIndex;
    }

    std::size_t NullExpression::end() {
        return invalidTokenIndex;
    }

    bool NullExpression::hasChild() {
        return false;
    }

    void NullExpression::analyseType() {
    }

    std::string NullExpression::toString() {
        return std::string();
    }

    IndexExpression::IndexExpression(const std::shared_ptr<Expression> &indexedExpr,
                                     const std::shared_ptr<Expression> &index, std::size_t lSquareIndex,
                                     std::size_t rSquareIndex) {
        _indexedExpr = indexedExpr;
        _index = index;
        _lSquareIndex = lSquareIndex;
        _rSquareIndex = rSquareIndex;
    }

    void IndexExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_indexedExpr, false);
        syntaxWalker(_index, true);
    }

    void IndexExpression::writeCurrentInfo(std::ostream &ostream) {
        Expression::writeCurrentInfo(ostream);
    }

    SyntaxType IndexExpression::getType() {
        return SyntaxType::IndexExpression;
    }

    std::size_t IndexExpression::start() {
        if (_indexedExpr->getType() != SyntaxType::NullExpression)
            return _indexedExpr->start();
        return _lSquareIndex;
    }

    std::size_t IndexExpression::end() {
        if(_rSquareIndex!=invalidTokenIndex)
            return _rSquareIndex;
        if (_index->getType() != SyntaxType::NullExpression)
            return _index->end();
        return _lSquareIndex;
    }

    bool IndexExpression::hasChild() {
        return true;
    }

    void IndexExpression::analyseType() {

    }

    std::string IndexExpression::toString() {
        return _indexedExpr->toString() + "[" + _index->toString() + "]";
    }

    PostfixExpression::PostfixExpression(token::TokenType operatorType, std::size_t opIndex,
                                         const std::shared_ptr<Expression> &operand) {
        _operatorType = operatorType;
        _opIndex = opIndex;
        _operand = operand;
    }

    token::TokenType PostfixExpression::getOperatorType() {
        return _operatorType;
    }

    void PostfixExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_operand, true);
    }

    void PostfixExpression::writeCurrentInfo(std::ostream &ostream) {
        Expression::writeCurrentInfo(ostream);
    }

    SyntaxType PostfixExpression::getType() {
        return SyntaxType::PostfixExpression;
    }

    std::size_t PostfixExpression::start() {
        if (_operand->getType() != SyntaxType::NullExpression)
            return _operand->start();
        return _opIndex;
    }

    std::size_t PostfixExpression::end() {
        return _opIndex;
    }

    bool PostfixExpression::hasChild() {
        return true;
    }

    void PostfixExpression::analyseType() {

    }

    std::string PostfixExpression::toString() {
        return _operand->toString() + token::TokenUtil::getSpell(_operatorType);
    }
}