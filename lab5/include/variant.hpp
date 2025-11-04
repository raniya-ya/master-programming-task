#ifndef __VARIANT_HPP__
#define __VARIANT_HPP__

#include <map>
#include <memory>
#include <variant>
#include <vector>

#include <boost/variant.hpp>

//{ number
using number = std::variant<int, float>;
//}

//{ array
using array = std::vector<number>;
//}

//{ recursive_array
struct recursive_array;

using recursive_array_base = std::variant<
    number,
    std::shared_ptr<recursive_array>
>;

struct recursive_array : public std::vector<recursive_array_base>
{
    using std::vector<recursive_array_base>::vector;
};
//}

//{ recursive_array2
struct recursive_array2;

using recursive_array2_base = std::variant<
    number,
    boost::recursive_wrapper<recursive_array2>
>;

struct recursive_array2 : public std::vector<recursive_array2_base>
{
    using std::vector<recursive_array2_base>::vector;
};
//}

//{ recursive_map
struct recursive_map;

// Сначала объявляем variant_decorator как шаблон
template<typename... Types>
struct variant_decorator;

// Затем объявляем recursive_map_value_base с forward declaration
using recursive_map_value_base = variant_decorator<
    int, 
    float, 
    bool, 
    std::string,
    boost::recursive_wrapper<recursive_map>
>;

// Теперь определяем variant_decorator
template<typename... Types>
struct variant_decorator : public std::variant<Types...>
{
    using std::variant<Types...>::variant;

    template<typename T>
    const T& as() const
    {
        return std::get<T>(*this);
    }

    template<typename T>
    T& as()
    {
        return std::get<T>(*this);
    }
};

// Теперь определяем recursive_map
struct recursive_map : public std::map<std::string, recursive_map_value_base>
{
    using std::map<std::string, recursive_map_value_base>::map;
};

// Специализация as() для recursive_map (с правильным синтаксисом)
template<>
template<>
inline const recursive_map& variant_decorator<int, float, bool, std::string, boost::recursive_wrapper<recursive_map>>::as<recursive_map>() const
{
    // Извлекаем из variant boost::recursive_wrapper и возвращаем ссылку на recursive_map
    return std::get<boost::recursive_wrapper<recursive_map>>(*this).get();
}

template<>
template<>
inline recursive_map& variant_decorator<int, float, bool, std::string, boost::recursive_wrapper<recursive_map>>::as<recursive_map>()
{
    return std::get<boost::recursive_wrapper<recursive_map>>(*this).get();
}
//}

#endif // __VARIANT_HPP__
