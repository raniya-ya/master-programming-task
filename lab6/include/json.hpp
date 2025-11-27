/* Json object.
 * @file
 * @date 2018-08-09
 * @author Anonymous
 */

#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <map>
#include <string>
#include <vector>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/adapted/std_pair.hpp>

namespace x3 = boost::spirit::x3;

//определение типов данных для джисон 
//value - это основной тип, который может хранить ЛЮБОЕ JSON-значение
namespace json::types
{
    struct array;
    struct object;

    using value = x3::variant<
        std::nullptr_t,
        std::string,
        bool,
        int,
        float,
        x3::forward_ast<array>,
        x3::forward_ast<object>
    >;

    struct array : std::vector<value> {};
    struct object : std::map<std::string, value> {};

    using json = x3::variant<
        object,
        array
    >;
}

namespace json::parser
{
    const auto sfloat_ = x3::real_parser<float, x3::strict_real_policies<float>>();

    // Объявления правил 
    x3::rule<class value_class, json::types::value> const value("value");
    x3::rule<class array_class, json::types::array> const array("array");
    x3::rule<class object_class, json::types::object> const object("object");
    x3::rule<class json_class, json::types::json> const json("json");
    x3::rule<class key_value_class, std::pair<std::string, json::types::value>> const key_value("key_value");
    x3::rule<class quoted_string_class, std::string> const quoted_string("quoted_string");

    // Простые парсеры  

    // Парсер для null
    const auto nullable = x3::lit("null") >> x3::attr(nullptr);

    // Парсер для true false
    const auto boolean = x3::bool_;

    const auto number = sfloat_ | x3::int_;

    // Определения правил 

    const auto quoted_string_def = x3::lexeme[
        '"' >> *( (x3::char_ - '"' - '\\') | (x3::lit('\\') > x3::char_) ) >> '"'
    ];

    const auto value_def = 
          quoted_string    
        | number           
        | boolean         
        | nullable        
        | array           
        | object;         

    const auto key_value_def = quoted_string >> ':' >> value;

    const auto array_def = '[' >> -(value % ',') >> ']';

    const auto object_def = '{' >> -(key_value % ',') >> '}';

    const auto json_def = object | array;

    // Связываем объявления правил с их определениями
    BOOST_SPIRIT_DEFINE(value, array, object, key_value, json, quoted_string)
}

#endif // __JSON_HPP__