#include <yuuki/compiler/feasy/parser.h>
#include <yuuki/compiler/feasy/token/token_util.h>
#include <yuuki/compiler/feasy/token/operator_util.h>
#include <yuuki/compiler/feasy/syntax/generic_type.h>
#include <yuuki/compiler/diagnostics/diagnostic_builder.h>
namespace yuuki::compiler::feasy{
    using namespace yuuki::compiler::diagnostics;
    using namespace yuuki::compiler::feasy::token;
    using namespace yuuki::compiler::feasy::syntax;
    Parser::Parser(const std::shared_ptr <SyntaxContext> &context,
                   const std::shared_ptr <DiagnosticStream> &diagStream) {
        _context = context;
        _diagnosticStream = diagStream;
        _tokenIndex = 0;
    }

    void Parser::parse() {
        // the recovery lambda for main parse section
        auto recover = [&]() -> void {
            move({
                TokenType::kw_import,
                TokenType::kw_namespace,
                // generate modifiers movers
                #define MODIFIER(X) TokenType::kw_##X,
                #include <yuuki/compiler/feasy/token/tokens.inc>
                TokenType::inline_comment,
                TokenType::interline_comment,
                TokenType::semi
            });
        };
        while (_tokenIndex < _context->tokens.size()){
            switch (_context->tokens[_tokenIndex]->type){
                // we are going to handle the main situations in the global syntax unit
                case TokenType::kw_import:
                    //parseImportDirective();
                    break;
                case TokenType::kw_namespace:
                    //parseNamespaceDeclaration();
                    break;
                // we are going to handle modifiers which may be added to tokens.inc in the future design!
                // so we use marcos to generate modifier list, codes are generated like:
                //     case TokenType::kw_public:
                #define MODIFIER(X) case TokenType::kw_##X:
                #include <yuuki/compiler/feasy/token/tokens.inc>
                // the modifiers in the global environment only results lead to a global namespaced class
                // so we can merge the situation with the kw_class
                case TokenType::kw_class:
                {
                    //_context->syntaxTree->add(parseClass());
                    if(_context->tokens[_tokenIndex])
                    break;
                }
                case TokenType::identifier: {
                    // we just can't decide what happened here, we should firstly parse the full name
                    // since we may meet 'some.situation.like.this'
                    auto firstIdIndex = _tokenIndex;
                    auto name = parseName();

                    // we are going to find out what situation should be:
                    //      some.libs; -> import some.libs;
                    //
                    //      when next line is like import some.other.libs; or namespace some.ns{
                    //
                    //      some {     -> class some {
                    //
                    //      some.lib { -> namespace  {
                    switch (_context->tokens[_tokenIndex]->type){
                        case TokenType::semi: {
                            // we met the first situation, think it's a import directive and push an error
                            auto import = std::make_shared<ImportDirective>(std::move(name),
                                                                            SyntaxNode::invalidTokenIndex,
                                                                            _tokenIndex);
                            _context->syntaxTree->add(import);
                            _diagnosticStream << DiagnosticBuilder::
                            error(CompileError::ImportExpected, _context->syntaxID)
                                    .before(firstIdIndex)
                                    .message("import keyword expected")
                                    .suggestion("insert 'import' here")
                                    .build();
                            break;
                        }
                        case TokenType::l_brace:{
                            // it's a start of a block
                            if(name->getType() == SyntaxType::IdentifierName){
                                // tend to think it's a class
                                // TODO: make a class
                            } else{
                                // name is a Qualified name, must be a namespace.
                                // TODO: make a namespace
                            }

                        }
                        default:
                            // unexpected situation
                            recover();
                            // TODO: push error 'UnexpectedTokens'
                            break;
                    }
                    break;
                }
                // we are going to handle comments
                case TokenType::inline_comment:
                case TokenType::interline_comment:
                    // just jump over the comments for now
                // then the other special situations
                case TokenType::semi:
                    // we think a single semi is a null directive
                    // which would not be constructed into parse tree
                    _tokenIndex++;
                    break;
                case TokenType::eof:
                    // ends the parsing when met the end of the token list
                    return;
                default:
                    // we met a unexpected token, until we met one of:
                    //      import, namespace, //, /**/, ;, modifiers
                    // we should just jump over them and throw an error
                    std::size_t startIndex = _tokenIndex;
                    recover();
                    // we have moved into a position we can handle we should just push the error happened
                    // since ErrorBuilder::error().ranges() is not implemented yet, just do nothing here
                    // TODO: push error 'UnexpectedTokens'
                    break;
            }
        }
    }

    std::shared_ptr<Name> Parser::parseName() {
        // call to this function should ensure that token _context->tokens[_tokenIndex] is a identifier!!
        auto currName = parseIdentifier();
        // since call won't move over the identifier, we should move here
        auto nextJudgeTokenIndex = getNextNotComment();
        if(getTokenType(getNextNotComment()) == TokenType::op_period){
            // record the current token index (the index BEFORE period)
            auto originalTokIndex = _tokenIndex;
            // _tokenIndex is now set to
            _tokenIndex = nextJudgeTokenIndex;
            // move to next non-comment token position
            nextJudgeTokenIndex = getNextNotComment();
            if(getTokenType(nextJudgeTokenIndex) == TokenType::identifier) {
                _tokenIndex = nextJudgeTokenIndex;
                // continue to parse right part of name
                auto rightName = parseName();
                return std::make_shared<QualifiedName>(std::move(currName), _tokenIndex, rightName);
            } else{
                // since the next usable token is not a identifier, roll back.
                _tokenIndex = originalTokIndex;
            }
        }
        return currName;
    }

    std::shared_ptr<ClassDeclaration> Parser::parseClass() {
        std::shared_ptr<ModifierList> modifiers = std::make_shared<ModifierList>();
        std::shared_ptr<Name> name;
        // parse the modifiers
        fillModifierList(modifiers);
        // jump to class token
        _tokenIndex = getNextNotComment();
        std::size_t classTokenIndex = _tokenIndex;
        _tokenIndex = getNextNotComment();
        name = parseName();
        std::size_t nextJudgeTokenIndex = getNextNotComment();
        std::shared_ptr<GenericTypeList> genDecl = std::make_shared<GenericTypeList>();
        std::shared_ptr<InheritTypeList> inheritDecl = std::make_shared<InheritTypeList>();
        if(getTokenType(nextJudgeTokenIndex)== TokenType::op_less){
            _tokenIndex = nextJudgeTokenIndex;
            fillGenericTypeList(genDecl);
            nextJudgeTokenIndex = getNextNotComment();
        } else{
            //genDecl = std::make_shared<GenericDeclaration>(SyntaxNode::invalidTokenIndex);
        }
        if(getTokenType(nextJudgeTokenIndex)==TokenType::op_colon){
            _tokenIndex = nextJudgeTokenIndex;
            nextJudgeTokenIndex = getNextNotComment();
            parseNextInherit:
            if(getTokenType(nextJudgeTokenIndex)==TokenType::identifier){
                _tokenIndex = nextJudgeTokenIndex;
                inheritDecl->add(parseType());
                nextJudgeTokenIndex = getNextNotComment();
                if(getTokenType(nextJudgeTokenIndex)==TokenType::comma) {
                    _tokenIndex = nextJudgeTokenIndex;
                    nextJudgeTokenIndex = getNextNotComment();
                    goto parseNextInherit;
                }
                else if (getTokenType(nextJudgeTokenIndex)==TokenType::identifier){
                    _diagnosticStream << DiagnosticBuilder::error(CompileError::CommaExpected, _context->syntaxID)
                            .before(nextJudgeTokenIndex)
                            .message("',' expected")
                            .suggestion("insert ',' here")
                            .build();
                    goto parseNextInherit;
                }
            }
        }
        auto classDecl = std::make_shared<ClassDeclaration>(modifiers,name,genDecl,inheritDecl);
        auto errorHandle = [&]()->void{                       // we met a unexpected token, until we met one of:
            //
            // we should just jump over them and throw an error
            std::size_t startIndex = _tokenIndex;
            move({
                    TokenType::kw_class,
                    // generate modifiers movers
                    #define MODIFIER(X) TokenType::kw_##X,
                    #include <yuuki/compiler/feasy/token/tokens.inc>
                 });
            // we have moved into a position we can handle we should just push the error happened
            // since ErrorBuilder::error().ranges() is not implemented yet, just do nothing here
            // TODO: push error 'UnexpectedTokens
        };
        _tokenIndex = getNextNotComment();
        if(getCurrentTokenType()== TokenType::l_brace){
            classDecl->setLBraceIndex(_tokenIndex);
            parseNextMember:
            _tokenIndex = getNextNotComment();
            std::size_t memberStart = _tokenIndex;
            skipOverAModifierList();
            switch (getCurrentTokenType())
            {
                case TokenType::kw_class:
                    _tokenIndex = memberStart;
                    classDecl->add(parseClass());
                    break;
                case TokenType::identifier: {
                    if((!skipOverATypeDeclaration())||(getTokenType(getNextNotComment()))!=TokenType::identifier){
                        errorHandle();
                        goto parseNextMember;
                    }
                    _tokenIndex = getNextNotComment();
                    switch (getTokenType(getNextNotComment())){
                        case TokenType::op_equal:
                        case TokenType::semi:
                            _tokenIndex = memberStart;
                            //classDecl->add(parse)
                        case TokenType::op_less:
                        case TokenType::l_brace:
                            _tokenIndex = memberStart;
                            classDecl->add(parseMethodDeclaration());
                        default:
                            errorHandle();
                            goto parseNextMember;
                    }

                }
                case TokenType::eof:
                    break;
                case TokenType::r_brace:
                    classDecl->setRBraceIndex(_tokenIndex);
                    break;
                default:
                    errorHandle();
                    goto parseNextMember;
            }
        } else {
            classDecl->setLBraceIndex(SyntaxNode::invalidTokenIndex);
        }
        return classDecl;
    }

    std::shared_ptr<IdentifierName> Parser::parseIdentifier() {
        // call to this function should ensure that token _context->tokens[_tokenIndex] is a identifier!!
        return std::make_shared<IdentifierName>(
                (std::string) _context->tokens[_tokenIndex]->rawCode, _tokenIndex);
    }

    std::shared_ptr<Type> Parser::parseType(const std::list<TokenType>& errorRecover) {
        // a valid type should start with a name like
        //     Generic<G.T.S>
        //        ^                         valid!
        // the judge token in this situation should be next non-comment token (marked by '^')
        //     Generic /*interline comment*/ <G.T.S>
        //                                   ^
        // but in some error cases a type could start just like
        //     Generic<T1, [ ]>
        //                 ^                identifier expected,   parse as Generic<T1, unknown[]>
        //     Generic<T1, <SomeArgument>>
        //                 ^                identifier expected,   parse as Generic<T1, unknown<SomeArgument>>
        // the judge token in this situation should be current token (marked by '^')
        std::shared_ptr<Type> type;
        std::size_t nextJudgeTokenIndex = _tokenIndex;
        if(getCurrentTokenType() == TokenType::identifier) {
            type = std::make_shared<TrivialType>(parseName());
            nextJudgeTokenIndex = getNextNotComment();
        } else{
            // TODO:we should make a identifier expected error here
            if(!errorRecover.empty())
                move(errorRecover);
            type = std::make_shared<UnknownType>();
        }
        switch (getTokenType(nextJudgeTokenIndex)){
            case TokenType::op_less:{
                // move token to the '<' position
                _tokenIndex = nextJudgeTokenIndex;
                type = std::make_shared<GenericType>(type);
                if(!fillGenericTypeList((std::static_pointer_cast<GenericType>(type))->getGenericList())){
                    if(!errorRecover.empty())
                        move(errorRecover);
                    return type;
                }
                nextJudgeTokenIndex = getNextNotComment();
                if(getTokenType(nextJudgeTokenIndex) != TokenType::l_square)
                    return type;
                break;
            }
            case TokenType::l_square:
                break;
            default:
                return type;
        }

        parseNextArray:
        _tokenIndex = nextJudgeTokenIndex;
        type = std::make_shared<ArrayType>(type,_tokenIndex);
        nextJudgeTokenIndex = getNextNotComment();
        if(getTokenType(nextJudgeTokenIndex)!=TokenType::r_square){
            _diagnosticStream << DiagnosticBuilder::error(CompileError::IdentifierExpected, _context->syntaxID)
                    .before(nextJudgeTokenIndex)
                    .message("']' expected")
                    .suggestion("insert ']' here")
                    .build();
            if(!errorRecover.empty())
                move(errorRecover);
            return type;
        }
        _tokenIndex = nextJudgeTokenIndex;
        std::static_pointer_cast<ArrayType>(type)->setRSquareIndex(_tokenIndex);   // []

        nextJudgeTokenIndex = getNextNotComment();
        if(getTokenType(nextJudgeTokenIndex) == TokenType::l_square){
            goto parseNextArray;
        }
        return type;
    }

    std::shared_ptr<MethodDeclaration> Parser::parseMethodDeclaration() {
        // we are going to parse a method declaration: 
        // calls to this function should ensure :
        //                    |                 |              |
        // -----------------  |  -------------  |              |  -------------------------
        // | Modifier List |  |  | Type Decl |  |  identifier  |  | (Optional)GenericDecl | ---->PartII
        // -----------------  |  -------------  |              |  -------------------------
        //        ^           |  ^              |              |
        //        |           |  |              |              |
        //  parsed by parent  | we are here;    | the id is    |  <-... after this we cannot 
        //  funtions that     | a valid type    | also ensure  |  ensure any token relation
        //  called this       | is ensured.     |              |  
        //
        //                    -----------------------       / BlockStatement: method body
        //  a<----Part I   (  | (Optional)ParamList |  )  -   
        //                    -----------------------       \ SemiToken ';' : just declaraion (for interfaces)
        //
        //

        // from graph upward, we can directly parse a type definition and a identifier
        // for method variable establish.
        auto returnType = parseType();
        _tokenIndex = getNextNotComment(); // move to next non-comment token, just the place of the identifier
        auto methodName = parseName();

        // get next non-comment token, which is the key choise for whether a generic arg or directly param decl
        std::size_t nextJudgeTokenIndex = getNextNotComment();
        // from the graph upward, we only have two situations should be parsed here:
        //    s1.    public void methodGeneric<T1,T2,T3>(...)
        //                                    ^
        //                          nextJudgeTokenIndex is here
        //    s2.    public void methodTrivial(...)
        //                                    ^
        //                       nextJudgeTokenIndex is here
        //
        if(getTokenType(nextJudgeTokenIndex)==TokenType::op_less){
            
        }
        nextJudgeTokenIndex = getNextNotComment();
        if(getTokenType(nextJudgeTokenIndex)==TokenType::identifier){

        }
        return nullptr;
    }

    bool Parser::fillGenericTypeList(const std::shared_ptr<GenericTypeList>& list){
        // generic list should start with '<'
        list->setStartOpIndex(_tokenIndex);
        auto nextJudgeTokenIndex = getNextNotComment();

        parseNextTypeArg:
        switch (getTokenType(nextJudgeTokenIndex)){
            // for situations like  Generic< >
            //                              ^    identifier expected
            case TokenType::op_greatergreater:
                _tokenIndex = nextJudgeTokenIndex;
                splitCurrentMultiCharOperator();
            case TokenType::op_greater:{
                _tokenIndex = nextJudgeTokenIndex;
                if(!list->hasChild()) {
                    _diagnosticStream << DiagnosticBuilder::error(CompileError::IdentifierExpected, _context->syntaxID)
                            .before(nextJudgeTokenIndex)
                            .message("identifier expected")
                            .build();
                }
                list->setEndOpIndex(nextJudgeTokenIndex);
                return false;
            }
            case TokenType::eof:{
                _tokenIndex = nextJudgeTokenIndex;
                _diagnosticStream << DiagnosticBuilder::error(CompileError::GreaterExpected,_context->syntaxID)
                        .before(nextJudgeTokenIndex)
                        .message("'>' expected")
                        .suggestion("add '>' here")
                        .build();
                return false;
            }
            case TokenType::identifier:
                _tokenIndex = nextJudgeTokenIndex;
                list->add(parseType());
                nextJudgeTokenIndex = getNextNotComment();
                switch (getTokenType(nextJudgeTokenIndex))
                {
                    case TokenType::comma:
                        _tokenIndex = nextJudgeTokenIndex;
                        nextJudgeTokenIndex = getNextNotComment();
                        goto parseNextTypeArg;
                    case TokenType::op_greatergreater:
                        _tokenIndex = nextJudgeTokenIndex;
                        splitCurrentMultiCharOperator();
                    case TokenType::op_greater:
                        _tokenIndex = nextJudgeTokenIndex;
                        list->setEndOpIndex(_tokenIndex);
                        return true;
                    default:
                        _diagnosticStream << DiagnosticBuilder::error(CompileError::GreaterExpected,_context->syntaxID)
                                .before(nextJudgeTokenIndex)
                                .message("',' expected")
                                .suggestion("add ',' here")
                                .build();
                        return false;
                }

            default:
                _diagnosticStream << DiagnosticBuilder::error(CompileError::IdentifierExpected, _context->syntaxID)
                        .before(nextJudgeTokenIndex)
                        .message("identifier expected")
                        .build();
                return false;
        }
    }

    bool Parser::fillModifierList(const std::shared_ptr<ModifierList> &list){
        std::size_t nextJudgeTokenIndex = _tokenIndex;
        while (_context->tokens[nextJudgeTokenIndex]->is(TokenType::modifiers)){
            list->add(std::make_shared<ModifierMark>(getTokenType(nextJudgeTokenIndex), nextJudgeTokenIndex));
            _tokenIndex = nextJudgeTokenIndex;
            nextJudgeTokenIndex = getNextNotComment();
        }
        return true;
    }

    bool Parser::fillParamList(const std::shared_ptr<ParamList> &list){
        return true;
    }

    void Parser::splitCurrentMultiCharOperator() {
        std::string_view code = _context->tokens[_tokenIndex]->rawCode;
        std::size_t offset = _context->tokens[_tokenIndex]->offset;
        _context->tokens.erase(_context->tokens.begin() + _tokenIndex);
        for(std::size_t i = 0; i < code.size();i++) {
            auto tok = std::make_unique<Token>();
            tok->rawCode = code.substr( i, 1);
            tok->offset = offset + i;
            tok->type = TokenUtil::getType(code[i]);
            _context->tokens.insert(_context->tokens.begin() + _tokenIndex + i, std::move(tok));
        }
    }

    std::shared_ptr<Expression> Parser::parseExpression(const std::list<TokenType>& endTokens,
                                                                std::size_t parentPrecedence) {
        std::size_t nextJudgeTokenIndex = getFirstNotComment();
        TokenType nextType = getTokenType(nextJudgeTokenIndex);
        std::shared_ptr<Expression> left;
        // we met a unary operator here, we can ensure that a unary expression can be directly parsed
        // since primary expressions had been directly handled with special codes, and primary expressions
        // are parsed locally so we can ensure that when we met a unary operator, unary has the highest
        // privilege
        if (OperatorUtil::isUnary(nextType) && OperatorUtil::unary <= parentPrecedence) {
            _tokenIndex = nextJudgeTokenIndex;
            _tokenIndex = getNextNotComment();
            left = parseExpression(endTokens, OperatorUtil::unary);
            left = std::make_shared<UnaryExpression>(nextType, nextJudgeTokenIndex, left);
        } else{
            _tokenIndex = nextJudgeTokenIndex;
            switch (nextType){
                case TokenType::l_paren: {
                    std::size_t lParenIndex = _tokenIndex;
                    _tokenIndex = getNextNotComment();
                    if (skipOverATypeDeclaration()) {
                        _tokenIndex = getNextNotComment();
                        if (getCurrentTokenType() == TokenType::r_paren) {
                            _tokenIndex = getNextNotComment();
                            if ((getCurrentTokenType() == TokenType::identifier) ||
                                    (getCurrentTokenType() == TokenType::l_paren)) {
                                // we ensured that in this situation, we met a explicit type convert expression

                                // reset token pos to start parse of skipped type
                                _tokenIndex = lParenIndex;
                                _tokenIndex = getNextNotComment();
                                auto type = parseType();
                                std::size_t rParenIndex = getNextNotComment();
                                _tokenIndex = rParenIndex;
                                _tokenIndex = getNextNotComment();
                                auto rExpr = parseExpression(endTokens, OperatorUtil::primary);
                                left = std::make_shared<ExplicitCastExpression>(lParenIndex, rParenIndex, type, rExpr);
                                break;
                            }
                        }
                    }
                    _tokenIndex = lParenIndex;
                    _tokenIndex = getNextNotComment();
                    auto newEndTokens = endTokens;
                    newEndTokens.push_back(TokenType::r_paren);
                    auto inner = parseExpression(newEndTokens);
                    nextJudgeTokenIndex = getNextNotComment();
                    nextType = getTokenType(nextJudgeTokenIndex);
                    if (nextType == TokenType::r_paren) {
                        left = std::make_shared<ParenthesesExpression>(lParenIndex, nextJudgeTokenIndex, inner);
                        _tokenIndex = nextJudgeTokenIndex;
                    } else {
                        left = std::make_shared<ParenthesesExpression>(lParenIndex, SyntaxNode::invalidTokenIndex,
                                                                       inner);
                    }
                    break;
                }
                case TokenType::identifier: {
                    left = std::make_shared<NameExpression>(parseName());
                    break;
                }
                case TokenType::kw_this:{
                    left = std::make_shared<ThisExpression>(_tokenIndex);
                    break;
                }
                case TokenType::string_const:{
                    left = std::make_shared<StringLiteralExpression>(_tokenIndex,
                            _context->tokens[_tokenIndex]->rawCode);
                    break;
                }
                case TokenType::numeric_const:{
                    left = std::make_shared<NumericLiteralExpression>(_tokenIndex,
                                                                     _context->tokens[_tokenIndex]->rawCode);
                    break;
                }
                case TokenType::kw_new: {
                    // TODO: handle with new expr;
                    auto type = parseType();
                    break;
                }
                default:
                    left = std::make_unique<NullExpression>();

                    if(OperatorUtil::getBinaryOperatorPrecedence(nextType)==OperatorUtil::initial){
                        std::size_t startIndex = _tokenIndex;
                        // we have moved into a position we can handle we should just push the error happened
                        // since ErrorBuilder::error().ranges() is not implemented yet, just do nothing here
                        // TODO: push error 'UnexpectedTokens'
                        // unexpected tokens, move to endTokens;
                        move(endTokens);
                    }
                    break;
            }
        }
        while (true){
            nextJudgeTokenIndex = getNextNotComment();
            nextType = getTokenType(nextJudgeTokenIndex);
            // we met the end of the expression
            if(std::find(endTokens.begin(),endTokens.end(),nextType)!=endTokens.end())
                break;
            parsePrimary:

            switch (nextType){
                case TokenType::op_plusplus:
                case TokenType::op_minusminus:{
                    _tokenIndex = nextJudgeTokenIndex;
                    left = std::make_shared<PostfixExpression>(nextType,_tokenIndex,left);
                    break;
                }
                case TokenType::l_square: {
                    _tokenIndex = nextJudgeTokenIndex;
                    std::size_t lSquare = _tokenIndex;
                    _tokenIndex = getNextNotComment();
                    auto newEndTokens = endTokens;
                    newEndTokens.push_back(TokenType::r_square);
                    auto index = parseExpression(newEndTokens);
                    std::size_t rSquare = getNextNotComment();
                    if(getTokenType(rSquare)==TokenType::r_square){
                        left = std::make_shared<IndexExpression>(left,index,lSquare,rSquare);
                        _tokenIndex = rSquare;
                        nextJudgeTokenIndex = getNextNotComment();
                        nextType = getTokenType(nextJudgeTokenIndex);
                        if(nextType == TokenType::l_square)
                            goto parsePrimary;
                    } else{
                        left = std::make_shared<IndexExpression>(left,index,lSquare);
                        move(endTokens);
                        // TODO: push ] expected
                    }
                    break;
                }
                case TokenType::l_paren: {
                    // we are going to handle trivial method calls here
                    // make a copy of end tokens
                    auto newEndTokens = endTokens;
                    newEndTokens.push_back(TokenType::r_paren);
                    _tokenIndex = nextJudgeTokenIndex;
                    auto callExpr = std::make_shared<CallExpression>(_tokenIndex,left);
                    parseNextParam:
                    _tokenIndex = getNextNotComment();
                    if(getCurrentTokenType() != TokenType::r_paren) {
                        auto para = parseExpression(newEndTokens);
                        callExpr->add(para);
                        nextJudgeTokenIndex = getNextNotComment();
                        switch (getTokenType(nextJudgeTokenIndex)) {
                            case TokenType::comma:
                                _tokenIndex = nextJudgeTokenIndex;
                                goto parseNextParam;
                            case TokenType::r_paren:
                                _tokenIndex = nextJudgeTokenIndex;
                                callExpr->setRParenIndex(_tokenIndex);
                                break;
                            default:
                                //TODO: push ')' expected.
                                move(endTokens);
                        }
                    } else{
                        callExpr->setRParenIndex(_tokenIndex);
                    }
                    left = callExpr;
                    break;
                }
                default:
                    break;
            }
            std::size_t tokenBeforeArgTest = _tokenIndex;
            // for context sensitive situations like
            //    obj.method<Type>()
            //              ^ should be parsed in primary precedence as '('.
            if(nextType == TokenType::op_less){
                _tokenIndex = nextJudgeTokenIndex;
                std::size_t lessTokenIndex = nextJudgeTokenIndex;
                if(skipOverAGenericArgument()){
                    _tokenIndex = getNextNotComment();
                    if(getCurrentTokenType() == TokenType::l_paren){
                        // matches the situation
                        auto newEndTokens = endTokens;
                        newEndTokens.push_back(TokenType::r_paren);
                        _tokenIndex = lessTokenIndex;
                        auto genericArg = std::make_shared<GenericTypeList>();
                        fillGenericTypeList(genericArg);
                        // move to '('
                        _tokenIndex = getNextNotComment();

                        auto callExpr = std::make_shared<GenericCallExpression>(_tokenIndex,left,genericArg);
                        parseNextParam_:
                        _tokenIndex = getNextNotComment();
                        auto para = parseExpression(newEndTokens);
                        callExpr->add(para);
                        nextJudgeTokenIndex = getNextNotComment();
                        switch (getTokenType(nextJudgeTokenIndex)){
                            case TokenType::comma:
                                _tokenIndex = nextJudgeTokenIndex;
                                goto parseNextParam_;
                            case TokenType::r_paren:
                                _tokenIndex = nextJudgeTokenIndex;
                                callExpr->setRParenIndex(_tokenIndex);
                                break;
                            default:
                                //TODO: push ')' expected.
                                move(endTokens);
                        }
                        left = callExpr;
                    } else{
                        _tokenIndex = tokenBeforeArgTest;
                        nextJudgeTokenIndex = getNextNotComment();
                        nextType = getTokenType(nextJudgeTokenIndex);
                    }
                } else{
                    _tokenIndex = tokenBeforeArgTest;
                    nextJudgeTokenIndex = getNextNotComment();
                    nextType = getTokenType(nextJudgeTokenIndex);
                }
            }

            // we are now going to handle with binary expressions
            auto precedence = OperatorUtil::getBinaryOperatorPrecedence(nextType);
            if(precedence == OperatorUtil::initial)
                break;
            if(precedence >= parentPrecedence && precedence != OperatorUtil::assignment)
                break;
            _tokenIndex = nextJudgeTokenIndex;
            _tokenIndex = getNextNotComment();
            auto right = parseExpression(endTokens, precedence);
            left = std::make_shared<BinaryExpression>(left,nextType,nextJudgeTokenIndex,right);
        }
        return left;
    }

    std::shared_ptr<VariableDeclarationStatement> Parser::parseVariableDeclarationStatement(){
        auto stmt = std::make_shared<VariableDeclarationStatement>(parseType());

        parseNextVar:
        _tokenIndex = getNextNotComment();
        auto expr = parseExpression({TokenType::semi,TokenType::comma,TokenType::r_brace});
        _tokenIndex = getNextNotComment();
        switch (getCurrentTokenType()){
            case TokenType::semi:
                stmt->add(expr);
                stmt->setSemiTokenIndex(_tokenIndex);
                break;
            case TokenType::comma:
                stmt->add(expr);
                if(getTokenType(getNextNotComment())==TokenType::identifier)
                    goto parseNextVar;
                _diagnosticStream << DiagnosticBuilder::error(CompileError::CommaExpected,_context->syntaxID)
                        .at(_tokenIndex)
                        .message("semicolon expected")
                        .suggestion("but meet ',' here")
                        .build();
                break;
            case TokenType::identifier:
                _diagnosticStream << DiagnosticBuilder::error(CompileError::CommaExpected,_context->syntaxID)
                        .after(expr->end())
                        .message("comma expected")
                        .suggestion("add ',' here")
                        .build();
                stmt->add(expr);
                _tokenIndex = expr->end();
                goto parseNextVar;
            default:
                _diagnosticStream << DiagnosticBuilder::error(CompileError::CommaExpected,_context->syntaxID)
                        .after(expr->end())
                        .message("semicolon expected")
                        .suggestion("add ';' here")
                        .build();
                break;
        }
        return stmt;
    }

    std::shared_ptr<BlockStatement> Parser::parseBlockStatement() {
        auto block = std::make_shared<BlockStatement>(_tokenIndex);
        _tokenIndex = getNextNotComment();
        parseNextStmt:
        switch (getCurrentTokenType()){
            case TokenType::r_brace:
                block->setRBraceTokenIndex(_tokenIndex);
                break;
            case TokenType::eof:
                _diagnosticStream << DiagnosticBuilder::error(CompileError::RBraceExpected,_context->syntaxID)
                    .before(_tokenIndex)
                    .message("'}' expected")
                    .suggestion("add '}' here")
                    .build();
                break;
            default:{
                auto stmt = parseStatement();
                auto type = getTokenType(stmt->end());
                if(type == TokenType::r_brace||type == TokenType::eof||type == TokenType::semi)
                    _tokenIndex = getNextNotComment();
                block->add(stmt);
                goto parseNextStmt;
            }
        }
        return block;
    }

    std::shared_ptr<Statement> Parser::parseStatement(){
        switch (getCurrentTokenType()) {
            case TokenType::kw_if:
                return parseIfStatement();
            case TokenType::kw_while:
                return parseWhileStatement();
            case TokenType::kw_break:
                return parseBreakStatement();
            case TokenType::kw_continue:
                return parseContinueStatement();
            case TokenType::kw_return:
                return parseReturnStatement();
            case TokenType::kw_goto:
                return parseGotoStatement();
            case TokenType::kw_switch:
                return parseSwitchStatement();
            case TokenType::kw_case:
                return parseCaseStatement();
            case TokenType::kw_default:
                return parseDefaultStatement();
            case TokenType::kw_for:
                return parseForStatement();
            case TokenType::l_brace:
                return parseBlockStatement();
            case TokenType::identifier: {
                // label:
                if (getTokenType(getNextNotComment()) == TokenType::op_colon)
                    return parseLabelStatement();
                std::size_t stmtStartTokenIndex = _tokenIndex;
                if (getTokenType(getNextNotComment()) == TokenType::identifier || skipOverAGenericArgument()) {
                    if (getTokenType(getNextNotComment()) == TokenType::identifier) {
                        _tokenIndex = stmtStartTokenIndex;
                        return parseVariableDeclarationStatement();
                    }
                }
                _tokenIndex = stmtStartTokenIndex;
                return parseExpressionStatement();
            }
            case TokenType::l_paren:
                return parseExpressionStatement();
            case TokenType::semi:
                return std::make_shared<NopStatement>(_tokenIndex);
            case TokenType::r_brace:
            case TokenType::eof:
                _diagnosticStream << DiagnosticBuilder::error(CompileError::CommaExpected, _context->syntaxID)
                        .before(_tokenIndex)
                        .message("statement expected")
                        .build();
                return std::make_shared<NopStatement>(_tokenIndex);
            default:
                std::size_t _startTokenIndex = _tokenIndex;
                move({TokenType::kw_if,
                      TokenType::kw_while,
                      TokenType::kw_break,
                      TokenType::r_brace,
                      TokenType::kw_continue,
                      TokenType::kw_return,
                      TokenType::kw_goto,
                      TokenType::semi,
                      TokenType::l_brace,
                      TokenType::kw_for,
                      TokenType::kw_switch,
                      TokenType::kw_case,
                      TokenType::kw_default,
                      TokenType::identifier,
                      TokenType::l_paren});
                // TODO: push expected tokens error;
                return parseStatement();
        }
    }

    std::shared_ptr<ExpressionStatement> Parser::parseExpressionStatement(){
        auto expr = parseExpression();
        auto stmt = std::make_shared<ExpressionStatement>(expr);
        if(getTokenType(getNextNotComment())==TokenType::semi){
            _tokenIndex = getNextNotComment();
            stmt->setSemiTokenIndex(_tokenIndex);
        } else{
            std::size_t _startTokenIndex = _tokenIndex;
            move({TokenType::kw_if,
                  TokenType::kw_while,
                  TokenType::kw_break,
                  TokenType::r_brace,
                  TokenType::kw_continue,
                  TokenType::kw_return,
                  TokenType::kw_goto,
                  TokenType::semi,
                  TokenType::l_brace,
                  TokenType::kw_for,
                  TokenType::kw_switch,
                  TokenType::kw_case,
                  TokenType::kw_default,
                  TokenType::identifier,
                  TokenType::l_paren});
            if(getCurrentTokenType()==TokenType::semi){
                // TODO: push expected tokens error;
                stmt->setSemiTokenIndex(_tokenIndex);
            } else{
                _tokenIndex = _startTokenIndex;
                _diagnosticStream << DiagnosticBuilder::error(CompileError::CommaExpected,_context->syntaxID)
                        .after(_tokenIndex)
                        .message("semicolon expected")
                        .suggestion("add ';' here")
                        .build();
            }
        }
        return stmt;
    }

    std::shared_ptr<IfStatement> Parser::parseIfStatement() {
        auto ifTokenId = _tokenIndex;
        _tokenIndex = getNextNotComment();
        if(getCurrentTokenType()!=TokenType::l_paren){
            _diagnosticStream << DiagnosticBuilder::error(CompileError::UnexpectedToken,_context->syntaxID)
                .after(ifTokenId)
                .message("'(' expected")
                .suggestion(std::string("but found") + TokenUtil::getSpell(getCurrentTokenType()))
                .build();
        } else{
            _tokenIndex = getNextNotComment();
        }
        auto condition = parseExpression({TokenType::r_paren,TokenType ::l_brace});
        _tokenIndex = getNextNotComment();
        if(getCurrentTokenType()!=TokenType::r_paren){
            _diagnosticStream << DiagnosticBuilder::error(CompileError::UnexpectedToken,_context->syntaxID)
                    .after(ifTokenId)
                    .message("')' expected")
                    .suggestion(std::string("but found") + TokenUtil::getSpell(getCurrentTokenType()))
                    .build();
        } else{
            _tokenIndex = getNextNotComment();
        }
        auto ifClause = parseStatement();
        _tokenIndex = getNextNotComment();
        if(getCurrentTokenType() == TokenType::kw_else){
            _tokenIndex = getNextNotComment();
            auto elseClause = parseStatement();
            return std::make_shared<IfStatement>(ifTokenId,condition,ifClause,elseClause);
        };
        _tokenIndex = ifClause->end();
        return std::make_shared<IfStatement>(ifTokenId,condition,ifClause);
    }


    std::shared_ptr<WhileStatement> Parser::parseWhileStatement() {
        auto whileTokenId = _tokenIndex;
        _tokenIndex = getNextNotComment();
        if(getCurrentTokenType()!=TokenType::l_paren){
            _diagnosticStream << DiagnosticBuilder::error(CompileError::UnexpectedToken,_context->syntaxID)
                    .after(whileTokenId)
                    .message("'(' expected")
                    .suggestion(std::string("but found") + TokenUtil::getSpell(getCurrentTokenType()))
                    .build();
        } else{
            _tokenIndex = getNextNotComment();
        }
        auto condition = parseExpression({TokenType::r_paren,TokenType ::l_brace});
        _tokenIndex = getNextNotComment();
        if(getCurrentTokenType()!=TokenType::r_paren){
            _diagnosticStream << DiagnosticBuilder::error(CompileError::UnexpectedToken,_context->syntaxID)
                    .after(whileTokenId)
                    .message("')' expected")
                    .suggestion(std::string("but found") + TokenUtil::getSpell(getCurrentTokenType()))
                    .build();
        } else{
            _tokenIndex = getNextNotComment();
        }
        return std::make_shared<WhileStatement>(whileTokenId,condition,parseStatement());
    }


    std::shared_ptr<ContinueStatement> Parser::parseContinueStatement() {
        std::size_t continueTokenIndex = _tokenIndex;
        _tokenIndex = getNextNotComment();
        if(getCurrentTokenType() == TokenType::semi)
            return std::make_shared<ContinueStatement>(continueTokenIndex, _tokenIndex);
        _diagnosticStream << DiagnosticBuilder::error(CompileError::SemiExpected,_context->syntaxID)
                .after(continueTokenIndex)
                .message("';' expected")
                .suggestion("add ';' here")
                .build();
        return std::make_shared<ContinueStatement>(continueTokenIndex);
    }

    std::shared_ptr<BreakStatement> Parser::parseBreakStatement() {
        std::size_t breakTokenIndex = _tokenIndex;
        _tokenIndex = getNextNotComment();
        if(getCurrentTokenType() == TokenType::semi)
            return std::make_shared<BreakStatement>(breakTokenIndex, _tokenIndex);
        _diagnosticStream << DiagnosticBuilder::error(CompileError::SemiExpected,_context->syntaxID)
                .after(breakTokenIndex)
                .message("';' expected")
                .suggestion("add ';' here")
                .build();
        return std::make_shared<BreakStatement>(breakTokenIndex);
    }

    std::shared_ptr<ReturnStatement> Parser::parseReturnStatement() {
        std::size_t returnTokenIndex = _tokenIndex;
        _tokenIndex = getNextNotComment();
        if(getCurrentTokenType() == TokenType::semi)
            return std::make_shared<ReturnStatement>(returnTokenIndex, _tokenIndex);
        auto value = parseExpression();
        _tokenIndex = getNextNotComment();
        std::shared_ptr<ReturnStatement> result;
        if(getCurrentTokenType() == TokenType::semi) {
            result = std::make_shared<ReturnStatement>(returnTokenIndex, _tokenIndex);
        }
        else {
            result = std::make_shared<ReturnStatement>(returnTokenIndex);
            _tokenIndex = value == nullptr ? returnTokenIndex : value->end();
        }
        result->setValue(value);
        return result;
    }

    std::shared_ptr<GotoStatement> Parser::parseGotoStatement() {
        std::size_t gotoTokenIndex = _tokenIndex;
        _tokenIndex = getNextNotComment();
        std::shared_ptr<Name> labelName;
        if(getCurrentTokenType() == TokenType::identifier){
            labelName = parseIdentifier();
            _tokenIndex = getNextNotComment();
        } else{
            labelName = std::make_shared<IdentifierName>("unknown",SyntaxNode::invalidTokenIndex);
            _diagnosticStream << DiagnosticBuilder::error(CompileError::IdentifierExpected,_context->syntaxID)
                .after(gotoTokenIndex)
                .message("identifier expected")
                .build();
            if(getCurrentTokenType() == TokenType::semi)
                return std::make_shared<GotoStatement>(gotoTokenIndex,labelName,_tokenIndex);
            return std::make_shared<GotoStatement>(gotoTokenIndex,labelName);
        }
        if(getCurrentTokenType() == TokenType::semi)
            return std::make_shared<GotoStatement>(gotoTokenIndex,labelName,_tokenIndex);
        _diagnosticStream << DiagnosticBuilder::error(CompileError::IdentifierExpected,_context->syntaxID)
                .after(labelName->end())
                .message("semicolon expected")
                .message("insert ';' here")
                .build();
        return std::make_shared<GotoStatement>(gotoTokenIndex,labelName);
    }

    std::shared_ptr<LabelStatement> Parser::parseLabelStatement() {
        auto name = parseIdentifier();
        _tokenIndex = getNextNotComment();
        return std::make_shared<LabelStatement>(name,_tokenIndex);
    }

    std::shared_ptr<CaseStatement> Parser::parseCaseStatement() {
        std::size_t caseTokenIndex = _tokenIndex;
        std::size_t colonTokenIndex = SyntaxNode::invalidTokenIndex;
        std::shared_ptr<BlockStatement> caseBlock = nullptr;
        _tokenIndex = getNextNotComment();
        std::shared_ptr<Expression> value = parseExpression();
        _tokenIndex = getNextNotComment();
        if(getCurrentTokenType() == TokenType::op_colon){
            colonTokenIndex = _tokenIndex;
            _tokenIndex = getNextNotComment();
        } else{
            _diagnosticStream << DiagnosticBuilder::error(CompileError::ColonExpected,_context->syntaxID)
                .after(value->end())
                .message("colon excepted")
                .suggestion("insert ':' here")
                .build();
        }
        if(getCurrentTokenType() == TokenType::l_brace){
            caseBlock = parseBlockStatement();
        } else{
            if(colonTokenIndex!=SyntaxNode::invalidTokenIndex){
                _diagnosticStream << DiagnosticBuilder::error(CompileError::ColonExpected,_context->syntaxID)
                        .after(colonTokenIndex)
                        .message("'{' excepted")
                        .suggestion("insert '{' here")
                        .build();
            }
        }
        return std::make_shared<CaseStatement>(caseTokenIndex, colonTokenIndex, value, caseBlock);
    }

    std::shared_ptr<DefaultStatement> Parser::parseDefaultStatement() {
        std::size_t defaultTokenIndex = _tokenIndex;
        std::size_t colonTokenIndex = SyntaxNode::invalidTokenIndex;
        std::shared_ptr<BlockStatement> defaultBlock = nullptr;
        _tokenIndex = getNextNotComment();
        if(getCurrentTokenType() == TokenType::op_colon){
            colonTokenIndex = _tokenIndex;
            _tokenIndex = getNextNotComment();
        } else{
            _diagnosticStream << DiagnosticBuilder::error(CompileError::ColonExpected,_context->syntaxID)
                    .after(defaultTokenIndex)
                    .message("colon excepted")
                    .suggestion("insert ':' here")
                    .build();
        }
        if(getCurrentTokenType() == TokenType::l_brace){
            defaultBlock = parseBlockStatement();
        } else{
            if(colonTokenIndex!=SyntaxNode::invalidTokenIndex){
                _diagnosticStream << DiagnosticBuilder::error(CompileError::LBraceExpected,_context->syntaxID)
                        .after(colonTokenIndex)
                        .message("'{' excepted")
                        .suggestion("insert '{' here")
                        .build();
            }
        }
        return std::make_shared<DefaultStatement>(defaultTokenIndex, colonTokenIndex, defaultBlock);
    }

    std::shared_ptr<SwitchStatement> Parser::parseSwitchStatement(){
        std::size_t switchTokenIndex = _tokenIndex;
        _tokenIndex = getNextNotComment();
        auto value = parseExpression();
        if(value == nullptr){
            _diagnosticStream << DiagnosticBuilder::error(CompileError::LParenExpected,_context->syntaxID)
                .after(switchTokenIndex)
                .message("'(' expected after 'switch'")
                .build();
            return std::make_shared<SwitchStatement>(switchTokenIndex);
        }
        _tokenIndex = getNextNotComment();
        if(getTokenType(value->start())!=TokenType::l_paren){
            _diagnosticStream << DiagnosticBuilder::error(CompileError::LParenExpected,_context->syntaxID)
                    .before(value->start())
                    .message("'(' expected after 'switch'")
                    .build();
            if(getCurrentTokenType()!=TokenType::r_paren){
                _diagnosticStream << DiagnosticBuilder::error(CompileError::RParenExpected,_context->syntaxID)
                        .before(value->start())
                        .message("')' expected")
                        .build();
            } else{
                _tokenIndex = getNextNotComment();
            }
        }
        if(getCurrentTokenType()==TokenType::l_brace){
            return std::make_shared<SwitchStatement>(switchTokenIndex,value,parseBlockStatement());
        }
        _diagnosticStream << DiagnosticBuilder::error(CompileError::LBraceExpected,_context->syntaxID)
                .after(_tokenIndex)
                .message("'{' expected")
                .build();
        return std::make_shared<SwitchStatement>(switchTokenIndex,value);
    }


    std::shared_ptr<ForStatement> Parser::parseForStatement() {
        std::size_t forTokenIndex = _tokenIndex;
        _tokenIndex = getNextNotComment();
        if(getCurrentTokenType() != TokenType::l_paren){
            _diagnosticStream << DiagnosticBuilder::error(CompileError::LParenExpected,_context->syntaxID)
                    .after(forTokenIndex)
                    .message("'(' expected after 'for'")
                    .build();
            _tokenIndex = forTokenIndex;
        } else{
            _tokenIndex = getNextNotComment();
        }
        std::size_t stmtStartTokenIndex = _tokenIndex;
        std::shared_ptr<Statement> init = nullptr;
        std::shared_ptr<Expression> condition = nullptr;
        std::shared_ptr<Expression>  post = nullptr;
        std::shared_ptr<Statement> body = nullptr;
        if(getTokenType(getNextNotComment()) == TokenType::semi){
            init = std::make_shared<NopStatement>(_tokenIndex);
        } else {
            if (getTokenType(getNextNotComment()) == TokenType::identifier || skipOverAGenericArgument()) {
                if (getTokenType(getNextNotComment()) == TokenType::identifier) {
                    _tokenIndex = stmtStartTokenIndex;
                    init = parseVariableDeclarationStatement();
                } else{
                    _tokenIndex = stmtStartTokenIndex;
                    init = parseExpressionStatement();
                    _tokenIndex = getNextNotComment();
                }
            } else {
                _tokenIndex = stmtStartTokenIndex;
                init = parseExpressionStatement();
                _tokenIndex = getNextNotComment();
            }
        }
        if(getTokenType(_tokenIndex)!=TokenType::semi){
            _diagnosticStream << DiagnosticBuilder::error(CompileError::LParenExpected,_context->syntaxID)
                    .before(init->end())
                    .message("';' expected")
                    .build();
            _tokenIndex = init->end();
            goto formResult;
        } else{
            _tokenIndex = getNextNotComment();
        }

        condition = parseExpression();
        _tokenIndex = getNextNotComment();
        if(getCurrentTokenType()!=TokenType::semi){
            _diagnosticStream << DiagnosticBuilder::error(CompileError::SemiExpected,_context->syntaxID)
                    .before(condition->end())
                    .message("';' expected")
                    .build();
            _tokenIndex = condition->end();
            goto formResult;

        } else{
            _tokenIndex = getNextNotComment();
        }

        post = parseExpression();
        _tokenIndex = getNextNotComment();
        if(getCurrentTokenType()!=TokenType::r_paren){
            _diagnosticStream << DiagnosticBuilder::error(CompileError::RParenExpected,_context->syntaxID)
                    .before(condition->end())
                    .message("')' expected")
                    .build();
            _tokenIndex = condition->end();
            goto formResult;

        } else{
            _tokenIndex = getNextNotComment();
        }

        body = parseStatement();
        formResult:
        return std::make_shared<ForStatement>(forTokenIndex,init,condition,post,body);
    }


    std::shared_ptr<FieldDeclaration> Parser::parseFieldDeclaration() {
        auto modifiers = std::make_shared<ModifierList>();
        fillModifierList(modifiers);
        // move to next not-comment token
        _tokenIndex = getNextNotComment();
        return std::make_shared<FieldDeclaration>(modifiers,parseVariableDeclarationStatement());
    }

    bool Parser::skipOverATypeDeclaration() {
        // a valid type should start with a name like
        //     Generic<G.T.S>
        //        ^                         valid!
        // the judge token in this situation should be next non-comment token (marked by '^')
        //     Generic /*interline comment*/ <G.T.S>
        //                                   ^
        // but in some error cases a type could start just like
        //     Generic<T1, [ ]>
        //                 ^                identifier expected,   parse as Generic<T1, unknown[]>
        //     Generic<T1, <SomeArgument>>
        //                 ^                identifier expected,   parse as Generic<T1, unknown<SomeArgument>>
        //     Generic<T1,      +>
        //                 ↑    ^           unexpected identifier,
        //                 |                identifier expected,   parse as Generic<T1>
        // the judge token in this situation should be current token (marked by '^')
        std::size_t nextJudgeTokenIndex = _tokenIndex;
        if(getCurrentTokenType() == TokenType::identifier) {
            skipOverAName();
            nextJudgeTokenIndex = getNextNotComment();
        } else{
            return false;
        }
        switch (getTokenType(nextJudgeTokenIndex)){
            case TokenType::op_less:
                // move token to the '<' position
                _tokenIndex = nextJudgeTokenIndex;
                if (skipOverAGenericArgument()) {
                    nextJudgeTokenIndex = getNextNotComment();
                    if (getTokenType(nextJudgeTokenIndex) != TokenType::l_square)
                        return true;
                }
                return false;
            case TokenType::l_square:
                break;
            default:
                return true;
        }

        parseNextArray:
        _tokenIndex = nextJudgeTokenIndex;
        nextJudgeTokenIndex = getNextNotComment();
        if(getTokenType(nextJudgeTokenIndex)!=TokenType::r_square){
            return false;
        }
        _tokenIndex = nextJudgeTokenIndex;

        nextJudgeTokenIndex = getNextNotComment();
        if(getTokenType(nextJudgeTokenIndex) == TokenType::l_square){
            goto parseNextArray;
        }
        return true;
    }

    bool Parser::skipOverAGenericArgument() {
        auto nextJudgeTokenIndex = getNextNotComment();
        // for situations like  Generic< >
        //                              ^    identifier expected
        parseNextTypeArg:
        switch (getTokenType(nextJudgeTokenIndex)){
            case TokenType::op_greatergreater:
                _tokenIndex = nextJudgeTokenIndex;
                splitCurrentMultiCharOperator();
            case TokenType::op_greater:{
                _tokenIndex = nextJudgeTokenIndex;
                return true;
            }
            case TokenType::eof:{
                _tokenIndex = nextJudgeTokenIndex;
                return false;
            }
            case TokenType::identifier:
            case TokenType::op_less:
            case TokenType::l_square:
                _tokenIndex = nextJudgeTokenIndex;
                skipOverATypeDeclaration();
                switch (getCurrentTokenType()){
                    case TokenType::identifier:
                    case TokenType::op_greater:
                    case TokenType::r_square:
                        nextJudgeTokenIndex = getNextNotComment();
                        break;
                    default:
                        nextJudgeTokenIndex = _tokenIndex;
                        break;
                }
                if(getTokenType(nextJudgeTokenIndex) == TokenType::comma){
                    _tokenIndex = nextJudgeTokenIndex;
                    nextJudgeTokenIndex = getNextNotComment();
                }
                goto parseNextTypeArg;

            default:
                return false;
        }
    }

    bool Parser::skipOverAName() {
        // since call won't move over the identifier, we should move here
        auto nextJudgeTokenIndex = getNextNotComment();
        if(getTokenType(getNextNotComment()) == TokenType::op_period){
            // record the current token index (the index BEFORE period)
            auto originalTokIndex = _tokenIndex;
            // _tokenIndex is now set to
            _tokenIndex = nextJudgeTokenIndex;
            // move to next non-comment token position
            nextJudgeTokenIndex = getNextNotComment();
            if(getTokenType(nextJudgeTokenIndex) == TokenType::identifier) {
                _tokenIndex = nextJudgeTokenIndex;
                // continue to parse right part of name
                return skipOverAName();
            } else{
                // since the next usable token is not a identifier, roll back.
                _tokenIndex = originalTokIndex;
            }
        }
        return true;
    }

    bool Parser::skipOverAModifierList(){
        std::size_t nextJudgeTokenIndex = _tokenIndex;
        while (_context->tokens[nextJudgeTokenIndex]->is(TokenType::modifiers)){
            _tokenIndex = nextJudgeTokenIndex;
            nextJudgeTokenIndex = getNextNotComment();
        }
        return true;
    }
}