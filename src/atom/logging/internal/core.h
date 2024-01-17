#pragma once
// import atom.core;

namespace atom
{
    template <typename... targs>
    using log_str = fmt_string<targs...>;

    template <typename t>
    concept rlog_arg = requires(t arg) { requires rconvertible_to<t, string_view>; };
}
