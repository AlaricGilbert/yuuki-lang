#ifndef YUUKI_GENERIC_H
#define YUUKI_GENERIC_H
#include <yuuki/compiler/feasy/syntax/syntax_node.h>
#include <yuuki/compiler/feasy/syntax/name.h>
#include <yuuki/compiler/feasy/syntax/type.h>
namespace yuuki::compiler::feasy::syntax {
    class GenericTypeList : public SyntaxNode, public ISyntaxList<Type> {
    public:
        explicit GenericTypeList(std::size_t startLessOp,std::size_t endGreaterOp = invalidTokenIndex);
        GenericTypeList():_start(invalidTokenIndex),_end(invalidTokenIndex){}
        void forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) override;
        void writeCurrentInfo(std::ostream &ostream) override;
        void add(const std::shared_ptr<Type> &types) override;
        void setStartOpIndex(std::size_t startLessOp);
        void setEndOpIndex(std::size_t endGreaterOp);
        bool hasChild() override;
        std::size_t start() override;
        std::size_t end() override;
        SyntaxType getType() override;
        std::string toString();

    private:
        std::vector<std::shared_ptr<Type>> _genericTypes;
        std::size_t _start;
        std::size_t _end;
    };
}
#endif //YUUKI_GENERIC_H
