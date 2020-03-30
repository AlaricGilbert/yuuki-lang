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
        std::shared_ptr<Statement> getIf();
        std::shared_ptr<Statement> getElse();
        std::shared_ptr<Expression> getCondition();
        void setLParenIndex(std::size_t lParenIndex);
        void setRParenIndex(std::size_t rParenIndex);
        std::size_t getIfTokenIndex();
        std::size_t getLParenIndex();
        std::size_t getRParenIndex();
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
        std::size_t _elseTokenIndex;
    public:
        void setElseTokenIndex(size_t elseTokenIndex);

    public:
        size_t getElseTokenIndex() const;

    private:
        std::size_t _lParenIndex;
        std::size_t _rParenIndex;

    };

    class ForStatement:public Statement{
    public:
        explicit ForStatement(std::size_t forTokenIndex,
                     const std::shared_ptr<Statement>& init = nullptr,
                     const std::shared_ptr<Expression>& condition = nullptr,
                     const std::shared_ptr<Expression>& post = nullptr,
                     const std::shared_ptr<Statement>& body = nullptr);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;
        std::size_t getForTokenIndex();
        std::size_t getLParenIndex();
        std::size_t getRParenIndex();
        std::size_t getConditionEndSemiIndex();
        std::shared_ptr<Statement> getInit();
        std::shared_ptr<Expression> getCondition();
        std::shared_ptr<Expression> getPost();
        std::shared_ptr<Statement> getBody();
        void setLParenIndex(std::size_t lParenIndex);
        void setRParenIndex(std::size_t rParenIndex);
        void setConditionEndSemiIndex(std::size_t conditionEndSemiIndex);
    private:
        std::size_t _forTokenIndex;
        std::size_t _lParenIndex;
        std::size_t _rParenIndex;
        std::size_t _conditionEndSemiIndex;
        std::shared_ptr<Statement> _init;
        std::shared_ptr<Expression> _condition;
        std::shared_ptr<Expression> _post;
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
        std::size_t getWhileTokenIndex();
        std::shared_ptr<Expression> getCondition();
        std::shared_ptr<Statement> getBody();
        std::size_t getLParenIndex();
        std::size_t getRParenIndex();
        void setLParenIndex(std::size_t lParenIndex);
        void setRParenIndex(std::size_t rParenIndex);
    private:
        std::size_t _whileTokenIndex;
        std::shared_ptr<Expression> _condition;
        std::shared_ptr<Statement> _body;
        std::size_t _lParenIndex;
        std::size_t _rParenIndex;
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

    class BlockStatement: public ISyntaxList<Statement>,public ISyntaxList<Expression>, public Statement{
    public:
        explicit BlockStatement(std::size_t lBraceTokenIndex);
        void setRBraceTokenIndex(std::size_t lRraceTokenIndex);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;
        void add(const std::shared_ptr<Statement>& child) override ;
        void add(const std::shared_ptr<Expression>& child) override ;

    private:
        std::vector<std::shared_ptr<SyntaxNode>> _children;
        std::size_t _lBraceTokenIndex;
        std::size_t _rBraceTokenIndex;
    };

    class CaseStatement:public Statement{
    public:
        explicit CaseStatement(std::size_t caseTokenIndex,
                std::size_t colonTokenIndex = invalidTokenIndex,
                const std::shared_ptr<Expression>& value = nullptr,
                const std::shared_ptr<BlockStatement>& caseBlock = nullptr);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;std::size_t getCaseTokenIndex();
        std::size_t getColonTokenIndex();
        std::shared_ptr<BlockStatement> getCaseBlock();
        std::shared_ptr<Expression> getValue();

    private:
        std::size_t _caseTokenIndex;
        std::size_t _colonTokenIndex;
        std::shared_ptr<BlockStatement> _caseBlock;
        std::shared_ptr<Expression> _value;
    };

    class DefaultStatement:public Statement{
    public:
        explicit DefaultStatement(std::size_t defaultTokenIndex,
                                  std::size_t colonTokenIndex = invalidTokenIndex,
                                  const std::shared_ptr<BlockStatement>& defaultBlock = nullptr);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;

    private:
        std::size_t _defaultTokenIndex;
        std::size_t _colonTokenIndex;
        std::shared_ptr<BlockStatement> _defaultBlock;
    };

    class SwitchStatement:public Statement{
    public:
        explicit SwitchStatement(std::size_t switchTokenIndex,
                        const std::shared_ptr<Expression>& value = nullptr,
                        const std::shared_ptr<BlockStatement>& cases = nullptr);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;
        size_t getSwitchTokenIndex();
        size_t getLParenIndex();
        size_t getRParenIndex();
        std::shared_ptr<Expression> getValue();
        std::shared_ptr<BlockStatement> getCases() ;
        void setLParenIndex(std::size_t lParenIndex);
        void setRParenIndex(std::size_t rParenIndex);
    private:
        std::size_t _switchTokenIndex;
        std::size_t _lParenIndex;
        std::size_t _rParenIndex;
        std::shared_ptr<Expression> _value;
        std::shared_ptr<BlockStatement> _cases;
    };

    class VariableDeclarationStatement: public Statement, public ISyntaxList<Expression>{
    public:
        explicit VariableDeclarationStatement(const std::shared_ptr<Type>& type);
        void setSemiTokenIndex(std::size_t semiTokenIndex);
        void forEachVarDecl(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker);
        std::shared_ptr<Type> getVariableType();
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        void add(const std::shared_ptr<Expression>& varDecl) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;
    private:
        std::shared_ptr<Type> _type;
        std::vector<std::shared_ptr<Expression>> _varDecls;
        std::size_t _semiTokenIndex;
    };

    class ExpressionStatement: public Statement{
    public:
        explicit ExpressionStatement(const std::shared_ptr<Expression>& expression);
        void setSemiTokenIndex(std::size_t semiTokenIndex);
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        SyntaxType getType() override ;
        bool hasChild() override ;
        std::size_t start() override ;
        std::size_t end() override ;
    private:
        std::shared_ptr<Expression> _expression;
        std::size_t _semiTokenIndex;
    };
}
#endif //YUUKI_STATEMENT_H
