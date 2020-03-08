#ifndef YUUKI_SYNTAX_NODE_H
#define YUUKI_SYNTAX_NODE_H

#include <functional>
#include <vector>
#include <iostream>
#include <memory>
namespace yuuki::compiler::feasy::syntax{
    enum class SyntaxType{
        SyntaxUnit,
        ImportDirective,

        ModifierMark,
        ModifierBag,

        BlockStatement,

        NamespaceDeclaration,
        ClassDeclaration,
        GenericDeclaration,
        InheritDeclaration,
        MethodDeclaration,
        FieldDeclaration,
        ParamDeclaration,
        ParamList,

        IdentifierName,
        QualifiedName,

        TrivialType,
        ArrayType,
        GenericType,

        BinaryExpression,
        UnaryExpression,
        NameExpression,

        InlineComment,
        InterlineComment
    };
    template <class _TListElem>
    class ISyntaxContext;
    class SyntaxNode{
    public:
        virtual void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>,bool)>& syntaxWalker) = 0;
        virtual void writeCurrentInfo(std::ostream& ostream) = 0;
        virtual SyntaxType getType() = 0;
        virtual bool hasChild() = 0;
        virtual ~SyntaxNode() = default;
    };

    template <typename _TListElem>
    class ISyntaxList{
    public:
        virtual void add(const std::shared_ptr<_TListElem>& child) = 0;
        virtual ~ISyntaxList();
    };

    template<typename _TListElem>
    ISyntaxList<_TListElem>::~ISyntaxList() {
        static_assert(std::is_base_of<SyntaxNode,_TListElem>::value);
    }

}
#endif //YUUKI_SYNTAX_NODE_H
