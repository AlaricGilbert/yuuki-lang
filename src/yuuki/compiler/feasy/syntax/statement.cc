#include <yuuki/compiler/feasy/syntax/statement.h>
#include <rang/rang.h>
namespace yuuki::compiler::feasy::syntax{
    IfStatement::IfStatement(std::size_t ifTokenIndex,
                             const std::shared_ptr<Expression> &condition, 
                             const std::shared_ptr<Statement>& ifClause,
                             const std::shared_ptr<Statement>& elseClause){
        _if = ifClause;
        _else = elseClause;
        _condition = condition;
        _ifTokenIndex = ifTokenIndex;
    }

    void IfStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_condition, false);
        if(_else == nullptr){
            syntaxWalker(_if, true);
        } else {
            syntaxWalker(_if, false);
            syntaxWalker(_else, true);
        }
    }

    void IfStatement::writeCurrentInfo(std::ostream &s) {
       if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "IfStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::blue     << "condition: " 
              << rang::fg::green    << "'" << _condition->toString() << "'"
              << rang::fg::reset    << std::endl;
        } else{
            s << "IfStatement "
              << "<" << this << "> "
              << "condition: "
              << "'" << _condition->toString() << "'"
              << std::endl;
        }
    }

    SyntaxType IfStatement::getType() {
        return SyntaxType::IfStatement;
    }

    bool IfStatement::hasChild() {
        return true;
    }

    std::size_t IfStatement::start() {
        return _ifTokenIndex;
    }

    std::size_t IfStatement::end() {
        if(_else!=nullptr)
            return _else->end();
        return _if ->end();
    }

    ForStatement::ForStatement(std::size_t forTokenIndex,
                               const std::shared_ptr<Statement>& init,
                               const std::shared_ptr<Statement>& condition,
                               const std::shared_ptr<Statement>& post,
                               const std::shared_ptr<Statement>& body){
        _forTokenIndex = forTokenIndex;
        _init = init;
        _post = post;
        _condition = condition;
        _body = body; 
    }

    void ForStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_init, false);
        syntaxWalker(_condition, false);
        syntaxWalker(_post, false);
        syntaxWalker(_body, true);
    }

    void ForStatement::writeCurrentInfo(std::ostream &s) {
       if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "ForStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::reset    << std::endl;
        } else{
            s << "ForStatement "
              << "<" << this << "> "
              << std::endl;
        }
    }

    SyntaxType ForStatement::getType() {
        return SyntaxType::ForStatement;
    }

    bool ForStatement::hasChild() {
        return true;
    }

    std::size_t ForStatement::start() {
        return _forTokenIndex;
    }

    std::size_t ForStatement::end() {
        return _body->end();
    }

    WhileStatement::WhileStatement(std::size_t whileTokenIndex,
                             const std::shared_ptr<Expression>& condition, 
                             const std::shared_ptr<Statement>& body){
        _whileTokenIndex = whileTokenIndex;
        _condition = condition;
        _body = body;
    }

    void WhileStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_condition, false);
        syntaxWalker(_body, true);
    }

    void WhileStatement::writeCurrentInfo(std::ostream &s) {
       if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "WhileStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::blue     << "condition: " 
              << rang::fg::green    << "'" << _condition->toString() << "'"
              << rang::fg::reset    << std::endl;
        } else{
            s << "WhileStatement "
              << "<" << this << "> "
              << "condition: "
              << "'" << _condition->toString() << "'"
              << std::endl;
        }
    }

    SyntaxType WhileStatement::getType() {
        return SyntaxType::WhileStatement;
    }

    bool WhileStatement::hasChild() {
        return true;
    }

    std::size_t WhileStatement::start() {
        return _whileTokenIndex;
    }

    std::size_t WhileStatement::end() {
        return _body->end();
    }

    DoWhileStatement::DoWhileStatement(std::size_t doTokenIndex,
                                       std::size_t semiTokenIndex,
                             const std::shared_ptr<Expression>& condition, 
                             const std::shared_ptr<Statement>& body){
        _doTokenIndex = doTokenIndex;
        _semiTokenIndex = semiTokenIndex;
        _condition = condition;
        _body = body;
    }

    void DoWhileStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_body, false);
        syntaxWalker(_condition, true);
    }

    void DoWhileStatement::writeCurrentInfo(std::ostream &s) {
       if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "DoWhileStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::blue     << "condition: " 
              << rang::fg::green    << "'" << _condition->toString() << "'"
              << rang::fg::reset    << std::endl;
        } else{
            s << "DoWhileStatement "
              << "<" << this << "> "
              << "condition: "
              << "'" << _condition->toString() << "'"
              << std::endl;
        }
    }

    SyntaxType DoWhileStatement::getType() {
        return SyntaxType::DoWhileStatement;
    }

    bool DoWhileStatement::hasChild() {
        return true;
    }

    std::size_t DoWhileStatement::start() {
        return _doTokenIndex;
    }

    std::size_t DoWhileStatement::end() {
        return _semiTokenIndex;
    }

    NopStatement::NopStatement(std::size_t semiTokenIndex){
        _semiTokenIndex = semiTokenIndex;
    }

    void NopStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
    }

    void NopStatement::writeCurrentInfo(std::ostream &s) {
       if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "NopStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::reset    << std::endl;
        } else{
            s << "NopStatement "
              << "<" << this << "> "
              << std::endl;
        }
    }

    SyntaxType NopStatement::getType() {
        return SyntaxType::NopStatement;
    }

    bool NopStatement::hasChild() {
        return false;
    }

    std::size_t NopStatement::start() {
        return _semiTokenIndex;
    }

    std::size_t NopStatement::end() {
        return _semiTokenIndex;
    }

    BreakStatement::BreakStatement(std::size_t breakTokenIndex,std::size_t semiTokenIndex){
        _breakTokenIndex = breakTokenIndex;
        _semiTokenIndex = semiTokenIndex;
    }

    void BreakStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
    }

    void BreakStatement::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "BreakStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::reset    << std::endl;
        } else{
            s << "BreakStatement "
              << "<" << this << "> "
              << std::endl;
        }
    }

    SyntaxType BreakStatement::getType() {
        return SyntaxType::BreakStatement;
    }

    bool BreakStatement::hasChild() {
        return false;
    }

    std::size_t BreakStatement::start() {
        return _breakTokenIndex;
    }

    std::size_t BreakStatement::end() {
        return _semiTokenIndex == invalidTokenIndex?_breakTokenIndex:_semiTokenIndex;
    }

    ContinueStatement::ContinueStatement(std::size_t continueTokenIndex,std::size_t semiTokenIndex){
        _continueTokenIndex = continueTokenIndex;
        _semiTokenIndex = semiTokenIndex;
    }

    void ContinueStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {

    }

    void ContinueStatement::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "ContinueStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::reset    << std::endl;
        } else{
            s << "ContinueStatement "
              << "<" << this << "> "
              << std::endl;
        }
    }

    SyntaxType ContinueStatement::getType() {
        return SyntaxType::ContinueStatement;
    }

    bool ContinueStatement::hasChild() {
        return false;
    }

    std::size_t ContinueStatement::start() {
        return _continueTokenIndex;
    }

    std::size_t ContinueStatement::end() {
        return _semiTokenIndex==invalidTokenIndex?_continueTokenIndex:_semiTokenIndex;
    }

    ReturnStatement::ReturnStatement(std::size_t returnTokenIndex,std::size_t semiTokenIndex){
        _returnTokenIndex = returnTokenIndex;
        _semiTokenIndex = semiTokenIndex;
        _value = nullptr;
    }

    void ReturnStatement::setValue(const std::shared_ptr<Expression> &value) {
        _value = value;
    }

    void ReturnStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        if(_value != nullptr)
            syntaxWalker(_value, true);
    }

    void ReturnStatement::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "ReturnStatement "
              << rang::fg::yellow   << "<" << this << "> ";
            if(_value!= nullptr)
                s << rang::fg::green << "value: '" << _value->toString() << "'";
            s  << rang::fg::reset    << std::endl;
        } else{
            s << "ReturnStatement "
              << "<" << this << "> ";
            if(_value!= nullptr)
                s << rang::fg::green << "value: '" << _value->toString() << "'";
            s  << std::endl;
        }
    }

    SyntaxType ReturnStatement::getType() {
        return SyntaxType::ReturnStatement;
    }

    bool ReturnStatement::hasChild() {
        return _value != nullptr;
    }

    std::size_t ReturnStatement::start() {
        return _returnTokenIndex;
    }

    std::size_t ReturnStatement::end() {
        if(_semiTokenIndex != invalidTokenIndex)
            return _semiTokenIndex;
        if(_value != nullptr)
            return _value->end();
        return _returnTokenIndex;
    }

    GotoStatement::GotoStatement(std::size_t gotoTokenIndex,
                                 const std::shared_ptr<Name>& labelName,
                                 std::size_t semiTokenIndex){
        _gotoTokenIndex = gotoTokenIndex;
        _labelName = labelName;
        _semiTokenIndex = semiTokenIndex;
    }

    void GotoStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_labelName, true);
    }

    void GotoStatement::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "GotoStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::green    << "goto label: <" << _labelName->toString() << ">"
              << rang::fg::reset    << std::endl;
        } else{
            s << "GotoStatement "
              << "<" << this << "> "
              << "goto label: <" << _labelName->toString() << ">"
              << std::endl;
        }
    }

    SyntaxType GotoStatement::getType() {
        return SyntaxType::GotoStatement;
    }

    bool GotoStatement::hasChild() {
        return true;
    }

    std::size_t GotoStatement::start() {
        return _gotoTokenIndex;
    }

    std::size_t GotoStatement::end() {
        if(_semiTokenIndex != invalidTokenIndex)
            return _semiTokenIndex;
        if(_labelName->end() != invalidTokenIndex)
            return _labelName->end();
        return _gotoTokenIndex;
    }

    LabelStatement::LabelStatement(const std::shared_ptr<Name>& labelName, std::size_t colonTokenIndex){
        _labelName = labelName;
    }

    void LabelStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_labelName, true);
    }

    void LabelStatement::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "LabelStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::green    << "label: <" << _labelName->toString() << ">"
              << rang::fg::reset    << std::endl;
        } else{
            s << "LabelStatement "
              << "<" << this << "> "
              << "label: <" << _labelName->toString() << ">"
              << std::endl;
        }
    }

    SyntaxType LabelStatement::getType() {
        return SyntaxType::LabelStatement;
    }

    bool LabelStatement::hasChild() {
        return true;
    }

    std::size_t LabelStatement::start() {
        return _labelName->start();
    }

    std::size_t LabelStatement::end() {
        return _colonTokenIndex;
    }

    void BlockStatement::add(const std::shared_ptr<Statement> &child) {
        _children.push_back(child);
    }

    void BlockStatement::add(const std::shared_ptr<Expression> &child) {
        _children.push_back(child);
    }
    void BlockStatement::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "BlockStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::reset    << std::endl;
        } else{
            s << "BlockStatement "
              << "<" << this << "> " << std::endl;
        }
    }

    void BlockStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        for(size_t id = 0; id < _children.size();id++){
            syntaxWalker(_children[id], id == _children.size() - 1);
        }
    }

    SyntaxType BlockStatement::getType() {
        return SyntaxType::BlockStatement;
    }

    bool BlockStatement::hasChild() {
        return !_children.empty();
    }

    std::size_t BlockStatement::start() {
        return _lBraceTokenIndex;
    }

    std::size_t BlockStatement::end() {
        if(_rBraceTokenIndex!=invalidTokenIndex)
            return _rBraceTokenIndex;
        if(_children.empty())
            return _lBraceTokenIndex;
        return _children.back()->end();
    }

    BlockStatement::BlockStatement(std::size_t lBraceTokenIndex) {
        _lBraceTokenIndex = lBraceTokenIndex;
        _rBraceTokenIndex = invalidTokenIndex;
    }

    void BlockStatement::setRBraceTokenIndex(std::size_t rBraceTokenIndex) {
        _rBraceTokenIndex = rBraceTokenIndex;
    }

    SwitchStatement::SwitchStatement(std::size_t switchTokenIndex,
                                     const std::shared_ptr<Expression>& value,
                                     const std::shared_ptr<BlockStatement>& cases){
        _switchTokenIndex = switchTokenIndex;
        _value = value;
        _cases = cases;
    }

    void SwitchStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        if(_cases != nullptr){
            if(_value!= nullptr) {
                syntaxWalker(_value, false);
            }
            syntaxWalker(_cases, true);
        } else{
            if(_value!= nullptr) {
                syntaxWalker(_value, true);
            }
        }
    }

    void SwitchStatement::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "SwitchStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::blue     << "switch "
              << rang::fg::green    << _value->toString()
              << rang::fg::reset    << std::endl;
        } else{
            s << "SwitchStatement "
              << "<" << this << "> "
              << "switch "
              << _value->toString()
              << std::endl;
        }
    }

    SyntaxType SwitchStatement::getType() {
        return SyntaxType::SwitchStatement;
    }

    bool SwitchStatement::hasChild() {
        return (_value != nullptr)||(_cases != nullptr);
    }

    std::size_t SwitchStatement::start() {
        return _switchTokenIndex;
    }

    std::size_t SwitchStatement::end() {
        if(_cases != nullptr)
            return _cases->end();
        if(_value!= nullptr)
            return _value->end();
        return _switchTokenIndex;
    }

    CaseStatement::CaseStatement(std::size_t caseTokenIndex,std::size_t colonTokenIndex,
                                 const std::shared_ptr<Expression>& value,
                                 const std::shared_ptr<BlockStatement>& caseBlock) {
        _caseTokenIndex = caseTokenIndex;
        _caseBlock = caseBlock;
        _value = value;
        _colonTokenIndex = colonTokenIndex;
    }

    void CaseStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        if(_caseBlock!= nullptr){
            if(_value!= nullptr) {
                syntaxWalker(_value, false);
            }
            syntaxWalker(_caseBlock, true);
        } else{
            if(_value!= nullptr) {
                syntaxWalker(_value, true);
            }
        }
    }

    void CaseStatement::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "CaseStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::blue     << "case :"
              << rang::fg::green    << _value->toString()
              << rang::fg::reset    << std::endl;
        } else{
            s << "CaseStatement "
              << "<" << this << "> "
              << "case :"    << _value->toString()
              << std::endl;
        }
    }

    SyntaxType CaseStatement::getType() {
        return SyntaxType::CaseStatement;
    }

    bool CaseStatement::hasChild() {
        return (_value!= nullptr)||(_caseBlock!= nullptr);
    }

    std::size_t CaseStatement::start() {
        return _caseTokenIndex;
    }

    std::size_t CaseStatement::end() {
        if(_caseBlock!= nullptr)
            return _caseBlock->end();
        if(_colonTokenIndex!=invalidTokenIndex)
            return _colonTokenIndex;
        if(_value!= nullptr)
            return _value->end();
        return _caseTokenIndex;
    }

    DefaultStatement::DefaultStatement(std::size_t defaultTokenIndex,
                                                std::size_t colonTokenIndex,
                                                const std::shared_ptr<BlockStatement>& defaultBlock){
        _defaultTokenIndex = defaultTokenIndex;
        _colonTokenIndex = colonTokenIndex;
        _defaultBlock = defaultBlock;
    }

    void DefaultStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        if(_defaultBlock!= nullptr)
            syntaxWalker(_defaultBlock, true);
    }

    void DefaultStatement::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "DefaultStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::reset    << std::endl;
        } else{
            s << "DefaultStatement "
              << "<" << this << "> "
              << std::endl;
        }
    }

    SyntaxType DefaultStatement::getType() {
        return SyntaxType::DefaultStatement;
    }

    bool DefaultStatement::hasChild() {
        return _defaultBlock!= nullptr;
    }

    std::size_t DefaultStatement::start() {
        return _defaultTokenIndex;
    }

    std::size_t DefaultStatement::end() {
        if(_defaultBlock!= nullptr)
            return _defaultBlock->end();
        if(_colonTokenIndex!= invalidTokenIndex)
            return _colonTokenIndex;
        return _defaultTokenIndex;
    }


    VariableDeclarationStatement::VariableDeclarationStatement(const std::shared_ptr<Type> &type) {
        _type = type;
        _semiTokenIndex = invalidTokenIndex;
    }

    void VariableDeclarationStatement::setSemiTokenIndex(std::size_t semiTokenIndex){
        _semiTokenIndex = semiTokenIndex;
    }

    void
    VariableDeclarationStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_type, _varDecls.empty());
        for (std::size_t i = 0; i < _varDecls.size(); i++){
            syntaxWalker(_varDecls[i], i == _varDecls.size() - 1);
        }
    }

    void VariableDeclarationStatement::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "VariableDeclarationStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::blue     << "type: "
              << rang::fg::green    << "'" << _type->toString() << "'"
              << rang::fg::reset    << std::endl;
        } else{
            s << "VariableDeclarationStatement "
              << "<" << this << "> "
              << "condition: "
              << "'" << _type->toString() << "'"
              << std::endl;
        }
    }

    void VariableDeclarationStatement::add(const std::shared_ptr<Expression> &varDecl) {
        _varDecls.push_back(varDecl);
    }

    SyntaxType VariableDeclarationStatement::getType() {
        return SyntaxType::VariableDeclarationList;
    }

    bool VariableDeclarationStatement::hasChild() {
        return true;
    }

    std::size_t VariableDeclarationStatement::start() {
        return _type->start();
    }

    std::size_t VariableDeclarationStatement::end() {
        if(_semiTokenIndex!=invalidTokenIndex)
            return _semiTokenIndex;
        if(_varDecls.empty())
            return _type->end();
        return _varDecls.back()->end();
    }

    ExpressionStatement::ExpressionStatement(const std::shared_ptr<Expression> &expression) {
        _expression = expression;
        _semiTokenIndex = invalidTokenIndex;
    }

    void ExpressionStatement::setSemiTokenIndex(std::size_t semiTokenIndex) {
        _semiTokenIndex = semiTokenIndex;
    }

    void ExpressionStatement::forEachChild(const std::function<void(std::weak_ptr<SyntaxNode>, bool)> &syntaxWalker) {
        syntaxWalker(_expression, true);
    }

    void ExpressionStatement::writeCurrentInfo(std::ostream &s) {
        if(s.rdbuf() == std::cout.rdbuf()){
            s << rang::fg::magenta  << "ExpressionStatement "
              << rang::fg::yellow   << "<" << this << "> "
              << rang::fg::blue     << "expr: "
              << rang::fg::green    << "'" << _expression->toString() << "'"
              << rang::fg::reset    << std::endl;
        } else{
            s << "ExpressionStatement "
              << "<" << this << "> "
              << "expr: "
              << "'" << _expression->toString() << "'"
              << std::endl;
        }
    }

    SyntaxType ExpressionStatement::getType() {
        return SyntaxType::ExpressionStatement;
    }

    bool ExpressionStatement::hasChild() {
        return true;
    }

    std::size_t ExpressionStatement::start() {
        return _expression->start();
    }

    std::size_t ExpressionStatement::end() {
        if(_semiTokenIndex == invalidTokenIndex)
            return _expression->end();
        return _semiTokenIndex;
    }
}