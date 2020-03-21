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
}