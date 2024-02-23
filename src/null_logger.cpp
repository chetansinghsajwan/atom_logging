export module atom.logging:null_logger;
import :logger;
import :log_msg;
import atom.core;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// null_logger is used in cases where it's not appropriate to return nullptr.
    ///
    /// null_logger is stateless and doesn't do anything except refusing every log request.
    /// damn rude nigga!.
    /// --------------------------------------------------------------------------------------------
    export class null_logger final: public logger
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// default constructs null_logger.
        /// ----------------------------------------------------------------------------------------
        constexpr null_logger() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// returns name of the null_logger.
        /// ----------------------------------------------------------------------------------------
        constexpr virtual auto get_name() const -> string_view override
        {
            return _name;
        }

        /// ----------------------------------------------------------------------------------------
        /// does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr virtual auto log(log_msg& log_msg) -> void override {}

        /// ----------------------------------------------------------------------------------------
        /// does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr virtual auto flush() -> void override {}

        /// ----------------------------------------------------------------------------------------
        /// always returns false to avoid constructing log msg.
        /// ----------------------------------------------------------------------------------------
        constexpr virtual auto check_log_level(log_level lvl) const -> bool override
        {
            return false;
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// static instance of this type because this type is stateless.
        /// it's recommended to use this instead of creating one each time.
        /// ----------------------------------------------------------------------------------------
        static logger_ptr instance;

    private:
        /// ----------------------------------------------------------------------------------------
        /// name of the null_logger.
        /// ----------------------------------------------------------------------------------------
        static const string_view _name;
    };

    inline logger_ptr null_logger::instance = make_shared<null_logger>();
    inline const string_view null_logger::_name = "null_logger";
}
