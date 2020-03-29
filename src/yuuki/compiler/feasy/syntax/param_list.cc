#include <yuuki/compiler/feasy/syntax/param_list.h>
#include <rang/rang.h>

namespace yuuki::compiler::feasy::syntax{


    ParamDeclaration::ParamDeclaration(const std::shared_ptr<Type> &type, const std::shared_ptr<Name> &name) {
        _name = name;
        _type = type;
        _defaultValue = nullptr;
        _equalIndex = invalidTokenIndex;
    }

    void ParamDeclaration::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_name, false);
        if(_defaultValue == nullptr) {
            syntaxWalker(_type, true);
        } else{
            syntaxWalker(_type, false);
            syntaxWalker(_defaultValue, true);
        }
    }

    void ParamDeclaration::writeCurrentInfo(std::ostream &s) {
        if (s.rdbuf() == std::cout.rdbuf()) {
            s << rang::fg::gray << "ParamDeclaration "
              << rang::fg::yellow << "<" << this << "> "
              << rang::fg::cyan << "'" << _type->toString() << " " << _name->toString();
            if (_defaultValue != nullptr)
                s << " = " << _defaultValue->toString();

            s << "'"
              << rang::fg::reset << std::endl;
        } else {
            s << "ParamDeclaration "
              << "<" << this << "> "
              << "'" << _type->toString() << " " << _name->toString();
            if (_defaultValue != nullptr)
                s << " = " << _defaultValue->toString();
            s << "'" << std::endl;
        }
    }

    SyntaxType ParamDeclaration::getType() {
        return SyntaxType::ParamDeclaration;
    }

    bool ParamDeclaration::hasChild() {
        return true;
    }

    std::weak_ptr<Name> ParamDeclaration::getParamName() {
        return _name;
    }

    std::weak_ptr<Type> ParamDeclaration::getParamType() {
        return _type;
    }

    void ParamDeclaration::setDefaultValue(const std::shared_ptr<Expression> &defaultValue) {
        _defaultValue = defaultValue;
    }

    std::size_t ParamDeclaration::start() {
        return _type->start();
    }

    std::size_t ParamDeclaration::end() {
        if(_defaultValue!= nullptr)
            return _defaultValue->end();
        return _name->end();
    }

    void ParamDeclaration::setEqualTokenIndex(std::size_t equalIndex) {
        _equalIndex = equalIndex;
    }

    std::weak_ptr<Expression> ParamDeclaration::getDefaultValue() {
        return _defaultValue;
    }

    std::size_t ParamDeclaration::getEqualIndex() {
        return _equalIndex;
    }

    void ParamList::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        for(std::size_t i = 0; i < _params.size(); i++){
            syntaxWalker(_params[i],i == _params.size() - 1);
        }
    }

    void ParamList::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::gray     << "ParamList "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::reset    << std::endl;
        } else{
            s << "ParamList "
              << "<" << this << "> " << std::endl;
        }
    }

    SyntaxType ParamList::getType() {
        return SyntaxType::ParamList;
    }

    bool ParamList::hasChild() {
        return !_params.empty();
    }

    void ParamList::add(const std::shared_ptr<ParamDeclaration> &param) {
        _params.push_back(param);
    }
    std::size_t ParamList::start() {
        if(_params.empty())
            return invalidTokenIndex;
        return _params.front()->start();
    }

    std::size_t ParamList::end() {
        if(_params.empty())
            return invalidTokenIndex;
        return _params.back()->end();
    }
}