#pragma once
// import atom.core;

namespace Atom
{
    template <typename... TArgs>
    using LogStr = FmtString<TArgs...>;

    template <typename T>
    concept RLogArg = requires(T arg) { requires RConvertibleTo<T, StringView>; };
}
