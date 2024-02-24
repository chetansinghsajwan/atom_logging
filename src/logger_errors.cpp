export module atom.logging:logging_errors;
import :logger;
import atom.core;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// error when logger registration fails.
    /// --------------------------------------------------------------------------------------------
    export class logger_registration_error: public error
    {
    public:
        constexpr logger_registration_error(string_view msg, string_view key)
            : error({ (const char*)msg.data().to_unwrapped(), msg.count().to_unwrapped() })
            , key(key)
        {}

    public:
        string_view key;
    };
}
