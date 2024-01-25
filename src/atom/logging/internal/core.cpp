export module atom.logging:core;
import atom.core;

namespace atom
{
    export template <typename... arg_types>
    using log_str = format_string<arg_types...>;

    export template <typename t>
    concept rlog_arg = requires(t arg) { requires rconvertible_to<t, string_view>; };
}
