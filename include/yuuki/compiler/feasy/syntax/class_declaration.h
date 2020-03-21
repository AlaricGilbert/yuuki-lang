#ifndef YUUKI_CLASS_DECLARATION_H
#define YUUKI_CLASS_DECLARATION_H
#include <yuuki/compiler/feasy/syntax/block_statement.h>
#include <yuuki/compiler/feasy/syntax/import_directive.h>
#include <yuuki/compiler/feasy/syntax/inherit_type_list.h>
#include <yuuki/compiler/feasy/syntax/generic.h>
#include <yuuki/compiler/feasy/syntax/modifier_list.h>
#include <yuuki/compiler/feasy/syntax/type.h>
#include <string>
#include <memory>
#include "method_declaration.h"
#include "field_declaration.h"

namespace yuuki::compiler::feasy::syntax{

    class ClassDeclaration:public SyntaxNode,
                           public ISyntaxList<MethodDeclaration>,
                           public ISyntaxList<FieldDeclaration>,
                           public ISyntaxList<ClassDeclaration>{
    public:
        explicit ClassDeclaration(const std::shared_ptr<ModifierList> &mod,
                                  const std::shared_ptr<Name> &name,
                                  const std::shared_ptr<GenericTypeList>& genericInfos,
                                  const std::shared_ptr<InheritTypeList>& inheritInfos);
        void add(const std::shared_ptr<MethodDeclaration> &method) override ;
        void add(const std::shared_ptr<FieldDeclaration> &field) override ;
        void add(const std::shared_ptr<ClassDeclaration> &child) override ;
        void forEachChild(const std::function<void (std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream& ostream) override;
        bool hasChild() override ;
        std::size_t start() override;
        std::size_t end() override;
        SyntaxType getType() override ;
        void setClassTokenIndex(std::size_t classTokenIndex);
        void setLBraceIndex(std::size_t lBraceIndex);
        void setRBraceIndex(std::size_t rBraceIndex);
    private:
        std::size_t _classTokenIndex;
        std::size_t _lBraceIndex;
        std::size_t _rBraceIndex;
        std::shared_ptr<ModifierList> _mod;
        std::shared_ptr<Name> _name;
        std::shared_ptr<InheritTypeList> _inheritInfos;
        std::shared_ptr<GenericTypeList> _genericInfos;
        std::vector<std::shared_ptr<SyntaxNode>> _members;
    };
}
#endif //YUUKI_CLASS_DECLARATION_H
