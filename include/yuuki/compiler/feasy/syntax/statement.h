#ifndef YUUKI_STATEMENT_H
#define YUUKI_STATEMENT_H
#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/expression.h>
namespace yuuki::compiler::feasy::syntax{


    class Statement:public SyntaxNode{
    public:
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override = 0;
        void writeCurrentInfo(std::ostream& ostream) override = 0;
        SyntaxType getType() override = 0;
        bool hasChild() override = 0;
        std::size_t start() override = 0;
        std::size_t end() override = 0;
    };

    class IfStatement:public Statement{
    public:
        IfStatement(std::size_t ifTokenIndex,
                    const std::shared_ptr<Expression> &condition,
                    const std::shared_ptr<Statement>& ifClause,
                    const std::shared_ptr<Statement>& elseClause = nullptr);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;
    private:
        std::shared_ptr<Statement> _if;
        std::shared_ptr<Statement> _else;
        std::shared_ptr<Expression> _condition;
        std::size_t _ifTokenIndex;
    };

    class ForStatement:public Statement{
    public:
        ForStatement(std::size_t forTokenIndex,
                     const std::shared_ptr<Statement>& init,
                     const std::shared_ptr<Statement>& condition,
                     const std::shared_ptr<Statement>& post,
                     const std::shared_ptr<Statement>& body);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;
        private:
        std::size_t _forTokenIndex;
        std::shared_ptr<Statement> _init;
        std::shared_ptr<Statement> _condition;
        std::shared_ptr<Statement> _post;
        std::shared_ptr<Statement> _body;
    };

    class WhileStatement:public Statement{
    public:
        WhileStatement(std::size_t whileTokenIndex,
                    const std::shared_ptr<Expression>& condition,
                    const std::shared_ptr<Statement>& body);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;
    private:
        std::size_t _whileTokenIndex;
        std::shared_ptr<Expression> _condition;
        std::shared_ptr<Statement> _body;
    };

    class DoWhileStatement:public Statement{
    public:
        DoWhileStatement(std::size_t doTokenIndex,
                         std::size_t semiTokenIndex,
                    const std::shared_ptr<Expression>& condition,
                    const std::shared_ptr<Statement>& body);    
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;
    private:
        std::size_t _doTokenIndex;
        std::size_t _semiTokenIndex;
        std::shared_ptr<Expression> _condition;
        std::shared_ptr<Statement> _body;
    };

    class NopStatement:public Statement{
    public:
        explicit NopStatement(std::size_t semiTokenIndex);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;
    private:
        std::size_t _semiTokenIndex;
    };

    class BreakStatement:public Statement{
    public:
        explicit BreakStatement(std::size_t breakTokenIndex,std::size_t semiTokenIndex = invalidTokenIndex);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;

    private:
        std::size_t _breakTokenIndex;
        std::size_t _semiTokenIndex;
    };

    class ContinueStatement:public Statement{
    public:
        explicit ContinueStatement(std::size_t continueTokenIndex,std::size_t semiTokenIndex = invalidTokenIndex);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;

    private:
        std::size_t _continueTokenIndex;
        std::size_t _semiTokenIndex;
    };

    class ReturnStatement:public Statement{
    public:
        explicit ReturnStatement(std::size_t returnTokenIndex, std::size_t semiTokenIndex=invalidTokenIndex);
        void setValue(const std::shared_ptr<Expression> &value);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;

    private:
        std::size_t _returnTokenIndex;
        std::size_t _semiTokenIndex;
        std::shared_ptr<Expression> _value;
    };

    class GotoStatement:public Statement{
    public:
        GotoStatement(std::size_t gotoTokenIndex,
                const std::shared_ptr<Name>& labelName,
                std::size_t semiTokenIndex = invalidTokenIndex);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;

    private:
        std::size_t _gotoTokenIndex;
        std::size_t _semiTokenIndex;
        std::shared_ptr<Name> _labelName;
    };

    class LabelStatement:public Statement{
    public:
        LabelStatement(const std::shared_ptr<Name>& labelName, std::size_t colonTokenIndex);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;

    private:
        std::shared_ptr<Name> _labelName;
        std::size_t _colonTokenIndex;
    };

}
#endif //YUUKI_STATEMENT_H
