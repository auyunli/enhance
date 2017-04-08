#ifndef EN_COMPONENT_PARSER_H
#define EN_COMPONENT_PARSER_H

#include "enComponentSpecialize.hpp"
#include "IParser.hpp"
#include "ParserMd5.hpp"
#include "ParserObj.hpp"

class enComponentParserMd5 : public enComponentSpecialize<IParser, ParserMd5> {
public:
    enComponentParserMd5( ParserMd5 * instance ) : enComponentSpecialize<IParser, ParserMd5>( instance, enComponentType::PARSER ){}
};

class enComponentParserObj : public enComponentSpecialize<IParser, ParserObj> {
public:
    enComponentParserObj( ParserObj * instance ) : enComponentSpecialize<IParser, ParserObj>( instance, enComponentType::PARSER ){}
};

#endif
