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

    std::size_t ExplicitCastExpression::getLeftParenIndex() {
        return _leftParenIndex;
    }

    std::size_t ExplicitCastExpression::getRightParenIndex() {
        return _rightParenIndex;
    }

    std::shared_ptr<Type> ExplicitCastExpression::getTargetType() {
        return _targetType;
    }

    std::shared_ptr<Expression> ExplicitCastExpression::getOperand() {
        return _operand;
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
            s << rang::fg::gray   << " " << getType()
              << rang::fg::yellow << "<" << this << "> "
              << rang::fg::green  << "'" << toString() << "' "
              << rang::fg::reset  << std::endl;
        } else {
            s << " " << getType()
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

    std::shared_ptr<Expression> IndexExpression::getIndexedExpr() {
        return _indexedExpr;
    }

    std::shared_ptr<Expression> IndexExpression::getIndex() {
        return _index;
    }

    std::size_t IndexExpression::getLSquareIndex() {
        return _lSquareIndex;
    }

    std::size_t IndexExpression::getRSquareIndex() {
        return _rSquareIndex;
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

    std::size_t PostfixExpression::getOpIndex() {
        return _opIndex;
    }

    std::shared_ptr<Expression> PostfixExpression::getOperand() {
        return _operand;
    }

    CallExpression::CallExpression(std::size_t lParenIndex,const std::shared_ptr<Expression>& method, std::size_t rParenIndex) {
        _lParenIndex = lParenIndex;
        _method = method;
        _rParenIndex = rParenIndex;
    }

    void CallExpression::add(const std::shared_ptr<Expression> &child) {
        _arguments.push_back(child);
    }

    void CallExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_method,_arguments.empty());
        for (std::size_t i = 0; i < _arguments.size(); ++i) {
            syntaxWalker(_arguments[i], i == _arguments.size() - 1);
        }
    }

    void CallExpression::writeCurrentInfo(std::ostream &ostream) {
        Expression::writeCurrentInfo(ostream);
    }

    SyntaxType CallExpression::getType() {
        return SyntaxType::CallExpression;
    }

    std::size_t CallExpression::start() {
        if(_method->getType() != SyntaxType::NullExpression)
            return _method->start();
        return _lParenIndex;
    }

    std::size_t CallExpression::end() {
        if(_rParenIndex!=invalidTokenIndex)
            return _rParenIndex;
        if(!_arguments.empty()){
            for(std::size_t i = 0; i <= _arguments.size(); i++){
                if(_arguments[_arguments.size() - i]->getType() != SyntaxType::NullExpression)
                    return _arguments[_arguments.size() - i]->end();
            }
        }
        return _lParenIndex;
    }

    bool CallExpression::hasChild() {
        return true;
    }

    void CallExpression::analyseType() {

    }

    std::string CallExpression::toString() {
        std::string result = _method->toString() + "(";
        for (std::size_t i = 0; i < _arguments.size(); ++i) {
            result += _arguments[i]->toString();
            if (i != _arguments.size() - 1)
                result += ", ";
        }
        return result+")";
    }

    void CallExpression::setRParenIndex(std::size_t rParenIndex) {
        _rParenIndex = rParenIndex;
    }

    std::shared_ptr<Expression> CallExpression::getMethod() {
        return _method;
    }

    std::size_t CallExpression::getLParenIndex() {
        return _lParenIndex;
    }

    std::size_t CallExpression::getRParenIndex() {
        return _rParenIndex;
    }

    void CallExpression::forEachArgument(const std::function<void(std::weak_ptr<SyntaxNode>,bool)> &syntaxWalker) {
        for (std::size_t i = 0; i < _arguments.size(); ++i) {
            syntaxWalker(_arguments[i], i == _arguments.size() - 1);
        }
    }

    ThisExpression::ThisExpression(std::size_t thisIndex) {
        _thisIndex = thisIndex;
    }

    void ThisExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
    }

    void ThisExpression::writeCurrentInfo(std::ostream &ostream) {
        Expression::writeCurrentInfo(ostream);
    }

    SyntaxType ThisExpression::getType() {
        return SyntaxType::ThisExpression;
    }

    std::size_t ThisExpression::start() {
        return _thisIndex;
    }

    std::size_t ThisExpression::end() {
        return _thisIndex;
    }

    bool ThisExpression::hasChild() {
        return false;
    }

    void ThisExpression::analyseType() {

    }

    std::string ThisExpression::toString() {
        return "this";
    }

    StringLiteralExpression::StringLiteralExpression(std::size_t stringIndex, const std::string_view &rawCode) {
        _stringIndex = stringIndex;
        _rawCode = rawCode;
    }

    void
    StringLiteralExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
    }

    void StringLiteralExpression::writeCurrentInfo(std::ostream &ostream) {
        Expression::writeCurrentInfo(ostream);
    }

    SyntaxType StringLiteralExpression::getType() {
        return SyntaxType::StringLiteralExpression;
    }

    std::size_t StringLiteralExpression::start() {
        return _stringIndex;
    }

    std::size_t StringLiteralExpression::end() {
        return _stringIndex;
    }

    bool StringLiteralExpression::hasChild() {
        return false;
    }

    void StringLiteralExpression::analyseType() {

    }

    std::string StringLiteralExpression::toString() {
        return "\"" + (std::string) _rawCode + "\"";
    }

    NumericLiteralExpression::NumericLiteralExpression(std::size_t numericIndex, const std::string_view &rawCode) {
        _numericIndex = numericIndex;
        _rawCode = rawCode;
    }

    void
    NumericLiteralExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
    }

    void NumericLiteralExpression::writeCurrentInfo(std::ostream &ostream) {
        Expression::writeCurrentInfo(ostream);
    }

    SyntaxType NumericLiteralExpression::getType() {
        return SyntaxType::NumericLiteralExpression;
    }

    std::size_t NumericLiteralExpression::start() {
        return _numericIndex;
    }

    std::size_t NumericLiteralExpression::end() {
        return _numericIndex;
    }

    bool NumericLiteralExpression::hasChild() {
        return false;
    }

    void NumericLiteralExpression::analyseType() {

    }

    std::string NumericLiteralExpression::toString() {
        return (std::string) _rawCode;
    }

    GenericCallExpression::GenericCallExpression(std::size_t lParenIndex, const std::shared_ptr<Expression> &method,
                                                 const std::shared_ptr<GenericTypeList> &genericArgList,
                                                 std::size_t rParenIndex) {
        _lParenIndex = lParenIndex;
        _method = method;
        _genericArgList = genericArgList;
        _rParenIndex = rParenIndex;
    }

    void GenericCallExpression::setRParenIndex(std::size_t rParenIndex) {
        _rParenIndex = rParenIndex;
    }

    void GenericCallExpression::add(const std::shared_ptr<Expression> &child) {
        _arguments.push_back(child);
    }

    void GenericCallExpression::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_method, false);
        syntaxWalker(_genericArgList,_arguments.empty());
        for (std::size_t i = 0; i < _arguments.size(); ++i) {
            syntaxWalker(_arguments[i], i == _arguments.size() - 1);
        }
    }

    void GenericCallExpression::writeCurrentInfo(std::ostream &ostream) {
        Expression::writeCurrentInfo(ostream);
    }

    SyntaxType GenericCallExpression::getType() {
        return SyntaxType::GenericCallExpression;
    }

    std::size_t GenericCallExpression::start() {
        if(_method->getType() != SyntaxType::NullExpression)
            return _method->start();
        return _genericArgList->start();
    }

    std::size_t GenericCallExpression::end() {
        if(_rParenIndex!=invalidTokenIndex)
            return _rParenIndex;
        if(!_arguments.empty()){
            for(std::size_t i = 0; i <= _arguments.size(); i++){
                if(_arguments[_arguments.size() - i]->getType() != SyntaxType::NullExpression)
                    return _arguments[_arguments.size() - i]->end();
            }
        }
        return _lParenIndex;
    }

    bool GenericCallExpression::hasChild() {
        return true;
    }

    void GenericCallExpression::analyseType() {

    }

    std::string GenericCallExpression::toString() {
        std::string result = _method->toString() + _genericArgList->toString() + "(";
        for (std::size_t i = 0; i < _arguments.size(); ++i) {
            result += _arguments[i]->toString();
            if (i != _arguments.size() - 1)
                result += ", ";
        }
        return result+")";
    }

    std::shared_ptr<Expression> GenericCallExpression::getMethod() {
        return _method;
    }

    std::shared_ptr<GenericTypeList> GenericCallExpression::getGenericArgList() {
        return _genericArgList;
    }

    std::size_t GenericCallExpression::getLParenIndex() {
        return _lParenIndex;
    }

    std::size_t GenericCallExpression::getRParenIndex() {
        return _rParenIndex;
    }

    void GenericCallExpression::forEachArgument(const std::function<void(std::weak_ptr<Expression>, bool)> &syntaxWalker) {
        for (std::size_t i = 0; i < _arguments.size(); ++i) {
            syntaxWalker(_arguments[i], i == _arguments.size() - 1);
        }
    }
}