#ifndef YUUKI_EXPRESSION_H
#define YUUKI_EXPRESSION_H

#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/name.h>
#include <yuuki/compiler/feasy/syntax/type.h>
#include <yuuki/compiler/feasy/token/token_type.h>

namespace yuuki::compiler::feasy::syntax{

    class Expression:public SyntaxNode{
    public:
        virtual void analyseType() = 0;
        virtual std::string toString() = 0;
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override = 0;
        void writeCurrentInfo(std::ostream& ostream) override = 0;
        std::size_t start() override = 0;
        std::size_t end() override = 0;
        SyntaxType getType() override = 0;
        bool hasChild() override = 0;
    };

    class ExplicitCastExpression: public Expression{
    public:
        ExplicitCastExpression(std::size_t lParenIndex,std::size_t rParenIndex,const std::shared_ptr<Type>& targetType,
                const std::shared_ptr<Expression>& operand);
        SyntaxType getType() override;
        bool hasChild() override;
        void writeCurrentInfo(std::ostream &ostream) override;
        std::size_t start() override;
        std::size_t end() override;
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void analyseType() override;
        std::string toString() override ;

    private:
        std::size_t _leftParenIndex;
        std::size_t _rightParenIndex;
        std::shared_ptr<Type> _targetType;
        std::shared_ptr<Expression> _operand;
    };


    class NameExpression: public Expression{
    public:
        explicit NameExpression(const std::shared_ptr<Name>& name);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        std::size_t start() override ;
        std::size_t end() override ;
        bool hasChild() override ;
        void analyseType() override ;
        std::string toString() override ;


    private:
        std::shared_ptr<Name> _name;
    };

    class ParenthesesExpression: public Expression{
    public:
        ParenthesesExpression(std::size_t lParenIndex,std::size_t rParenIndex,const std::shared_ptr<Expression> &innerExpr);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        std::size_t start() override ;
        std::size_t end() override ;
        bool hasChild() override ;
        void analyseType() override ;
        std::string toString() override ;

    private:
        std::size_t _leftParenIndex;
        std::size_t _rightParenIndex;
        std::shared_ptr<Expression> _innerExpression;
    };

    class NullExpression:public Expression{
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        std::size_t start() override ;
        std::size_t end() override ;
        bool hasChild() override ;
        void analyseType() override ;
        std::string toString() override ;
    };

    class IndexExpression:public Expression{
    public:
        IndexExpression(const std::shared_ptr<Expression> &indexedExpr,const std::shared_ptr<Expression> &index,
                std::size_t lSquareIndex,std::size_t rSquareIndex = invalidTokenIndex);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        std::size_t start() override ;
        std::size_t end() override ;
        bool hasChild() override ;
        void analyseType() override ;
        std::string toString() override ;

    private:
        std::shared_ptr<Expression> _indexedExpr;
        std::shared_ptr<Expression> _index;
        std::size_t _lSquareIndex;
        std::size_t _rSquareIndex;
    };

    class PostfixExpression: public Expression{
    public:
        PostfixExpression(token::TokenType operatorType, std::size_t opIndex, const std::shared_ptr<Expression>& operand);
        token::TokenType getOperatorType();
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream &ostream) override;
        SyntaxType getType() override;
        std::size_t start() override;
        std::size_t end() override;
        bool hasChild() override;
        void analyseType() override;
        std::string toString() override;
    private:
        token::TokenType _operatorType;
        std::size_t _opIndex;
        std::shared_ptr<Expression> _operand;
    };

}
#endif //YUUKI_EXPRESSION_H
