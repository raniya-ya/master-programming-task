/* Variant classes.
 * @file
 * @date 2018-08-12
 * @author Anonymous
 */
#ifndef __VARIANT_HPP__
#define __VARIANT_HPP__

#include <map>
#include <memory>
#include <variant>
#include <vector>
#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <string>
#include <type_traits>

//{ number
using number = std::variant<int, float>;
//}

//{ array
using array = std::vector<number>;
//}

//{ recursive_array
struct recursive_array : public std::vector<std::variant<number, std::shared_ptr<recursive_array>>>
{
    using base = std::vector<std::variant<number, std::shared_ptr<recursive_array>>>;
    using base::base;  
};
//}

//{ recursive_array2
struct recursive_array2 : public std::vector<std::variant<number, boost::recursive_wrapper<recursive_array2>>>
{
    using base = std::vector<std::variant<number, boost::recursive_wrapper<recursive_array2>>>;
    using base::base;
};
//}

//{ variant_decorator
template<typename... Types>
struct variant_decorator : public std::variant<Types...>
{
    using base = std::variant<Types...>;
    using base::base;

    template<typename T>
    const T& as() const
    {
        if constexpr (std::disjunction_v<std::is_same<T, Types>...>)
        {
            return std::get<T>(*this);
        }
        else if constexpr (std::disjunction_v<std::is_same<boost::recursive_wrapper<T>, Types>...>)
        {
            return std::get<boost::recursive_wrapper<T>>(*this).get();
        }
        else
        {
            static_assert(sizeof(T) == 0, "Type not supported");
        }
    }

    template<typename T>
    T& as()
    {
        if constexpr (std::disjunction_v<std::is_same<T, Types>...>)
        {
            return std::get<T>(*this);
        }
        else if constexpr (std::disjunction_v<std::is_same<boost::recursive_wrapper<T>, Types>...>)
        {
            return std::get<boost::recursive_wrapper<T>>(*this).get();
        }
        else
        {
            static_assert(sizeof(T) == 0, "Type not supported");
        }
    }
};
//}

//{ recursive_map
using variant_value = variant_decorator<
    std::string,
    float,
    int,
    bool,
    boost::recursive_wrapper<struct recursive_map>
>;

struct recursive_map : public std::map<std::string, variant_value>
{
    using base = std::map<std::string, variant_value>;
    using base::base;

    variant_value& operator[](const std::string& key)
    {
        return base::operator[](key);
    }

    const variant_value& operator[](const std::string& key) const
    {
        return base::at(key);
    }
};
//}

#endif // __VARIANT_HPP__