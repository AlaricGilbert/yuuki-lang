#include <yuuki/compiler/feasy/syntax/method_declaration.h>
#include <yuuki/compiler/feasy/syntax/generic_type.h>
#include <sstream>
#include <rang/rang.h>
namespace yuuki::compiler::feasy::syntax {

    MethodDeclaration::MethodDeclaration(const std::shared_ptr<ModifierList>& mod,
                                         const std::shared_ptr<Type>& returnType,
                                         const std::shared_ptr<Name>& name,
                                         const std::shared_ptr<GenericTypeList>& genericInfos,
                                         const std::shared_ptr<ParamList>& params,
                                         const std::shared_ptr<BlockStatement>& body) {
        _mod = mod;
        _returnType = returnType;
        _name = name;
        _genericInfos = genericInfos;
        _params = params;
        _body = body;
    }

    void MethodDeclaration::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_name, false);
        syntaxWalker(_mod, false);
        syntaxWalker(_returnType, false);
        syntaxWalker(_genericInfos, false);
        syntaxWalker(_params, false);
        syntaxWalker(_body, true);
    }

    void MethodDeclaration::writeCurrentInfo(std::ostream &s) {
        std::ostringstream methodData;
        if(_mod->hasChild()){
            _mod->forEachChild([&methodData](const std::weak_ptr<SyntaxNode> &n, bool isLast) {
                methodData << std::static_pointer_cast<ModifierMark>(n.lock())->toString() << " ";
            });
        }
        methodData << "'" << _returnType->toString() << _name->toString();
        if(_genericInfos->hasChild()) {
            methodData << "<";
            _genericInfos->forEachChild([&methodData](const std::weak_ptr<SyntaxNode> &n, bool isLast) {
                methodData << std::static_pointer_cast<GenericType>(n.lock())->toString()
                           << (isLast ? ">" : ",");
            });
        }

        methodData << "(";
        if(_params->hasChild()) {
            _params->forEachChild([&methodData](const std::weak_ptr<SyntaxNode> &n, bool isLast) {
                auto para = std::static_pointer_cast<ParamDeclaration>(n.lock());
                methodData << para->getParamType().lock()->toString()
                           << para->getParamName().lock()->toString()
                           << (isLast ? ")'" : ", ");
            });
        }
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray << "MethodDeclaration "
              << rang::fg::yellow << "<" << this << "> "
              << rang::fg::green << methodData.str()
              << rang::fg::reset    << std::endl;
        } else{
            s << "MethodDeclaration "
              << "<" << this << "> "
              << methodData.str() << std::endl;
        }
    }

    bool MethodDeclaration::hasChild() {
        return true;
    }

    SyntaxType MethodDeclaration::getType() {
        return SyntaxType::MethodDeclaration;
    }
}