#ifndef YUUKI_CLASS_DECLARATION_H
#define YUUKI_CLASS_DECLARATION_H
#include <yuuki/compiler/feasy/syntax/block_statement.h>
#include <yuuki/compiler/feasy/syntax/import_directive.h>
#include <yuuki/compiler/feasy/syntax/inherit_declaration.h>
#include <yuuki/compiler/feasy/syntax/generic.h>
#include <yuuki/compiler/feasy/syntax/modifier_list.h>
#include <yuuki/compiler/feasy/syntax/type.h>
#include <string>
#include <memory>
#include "method_declaration.h"

namespace yuuki::compiler::feasy::syntax{
    class FieldDeclaration:public SyntaxNode, public ISyntaxList<Expression>{
    public:
        void add(const std::shared_ptr<Expression> &declExpr) override ;
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        bool hasChild() override ;
        SyntaxType getType() override ;

    private:
        std::vector<std::shared_ptr<Expression>> _fieldDeclExprs;
    };
    class ClassDeclaration:public SyntaxNode,
                           public ISyntaxList<MethodDeclaration>,
                           public ISyntaxList<FieldDeclaration>,
                           public ISyntaxList<ClassDeclaration>{
    public:
        explicit ClassDeclaration(const std::shared_ptr<ModifierList> &mod,
                                  const std::shared_ptr<Name> &name,
                                  const std::shared_ptr<GenericDeclaration>& genericInfos,
                                  const std::shared_ptr<InheritDeclaration>& inheritInfos);
        void add(const std::shared_ptr<MethodDeclaration> &method) override ;
        void add(const std::shared_ptr<FieldDeclaration> &field) override ;
        void add(const std::shared_ptr<ClassDeclaration> &child) override ;
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        bool hasChild() override ;
        void setClassTokenIndex(std::size_t classTokenIndex);
        void setSemiTokenIndex(std::size_t semiTokenIndex);
        std::size_t start() override;
        std::size_t end() override;
        SyntaxType getType() override ;
    private:
        std::size_t _classTokenIndex;
        std::size_t _semiTokenIndex;
        std::shared_ptr<ModifierList> _mod;
        std::shared_ptr<Name> _name;
        std::shared_ptr<InheritDeclaration> _inheritInfos;
        std::shared_ptr<GenericDeclaration> _genericInfos;
        std::vector<std::shared_ptr<SyntaxNode>> _members;
    };
}
#endif //YUUKI_CLASS_DECLARATION_H
