/* Map of types.
 * @file
 * @date 2018-08-25
 * @author Anonymous
 */
#ifndef __TYPE_MAP_HPP__
#define __TYPE_MAP_HPP__
#include <array>
#include <tuple>
namespace detail
{
    template<class T, class Tuple>
    struct type_to_index;
    template<class T, class... Ts>
    struct type_to_index<T, std::tuple<T, Ts...>>
    {
        constexpr static size_t v = 0;
    };
    template<class T, class U, class... Ts>
    struct type_to_index<T, std::tuple<U, Ts...>>
    {
        constexpr static size_t v = 1 + type_to_index<T, std::tuple<Ts...>>::v;
    };
}
template <typename Index, typename... Types>
class type_map: public std::array<Index, sizeof...(Types)>
{
public:
    using types = std::tuple<Types...>;
    template<class T>
    decltype(auto) as()
    {
        return (*this)[detail::type_to_index<T, types>::v];
    }
    template<class T>
    decltype(auto) as() const
    {
        return (*this)[detail::type_to_index<T, types>::v];
    }
};
namespace std
{
    template<typename T, typename TypeMap>
    decltype(auto) get(TypeMap& tm)
    {
        return tm.template as<T>();
    }
    template<typename T, typename TypeMap>
    decltype(auto) get(const TypeMap& tm)
    {
        return tm.template as<T>();
    }
}
#endif // __TYPE_MAP_HPP__