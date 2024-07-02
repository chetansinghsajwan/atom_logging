export module atom_logging:null_logger;

import atom_core;
import :logger;
import :log_msg;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// null_logger is used in cases where it's not appropriate to return nullptr.
    ///
    /// null_logger is stateless and doesn't do anything except refusing every log request.
    /// --------------------------------------------------------------------------------------------
    export class null_logger final: public logger
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// returns the static instance of `null_logger`.
        /// ----------------------------------------------------------------------------------------
        static auto get_instance() -> null_logger*
        {
            static null_logger instance;
            return &instance;
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        null_logger()
            : logger()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        ~null_logger() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// returns name of the null_logger.
        /// ----------------------------------------------------------------------------------------
        virtual auto get_name() const -> string_view override final
        {
            return "null_logger";
        }

        /// ----------------------------------------------------------------------------------------
        /// does nothing.
        /// ----------------------------------------------------------------------------------------
        virtual auto log(log_msg& log_msg) -> void override final {}

        /// ----------------------------------------------------------------------------------------
        /// does nothing.
        /// ----------------------------------------------------------------------------------------
        virtual auto flush() -> void override final {}

        /// ----------------------------------------------------------------------------------------
        /// always returns false to avoid constructing log msg.
        /// ----------------------------------------------------------------------------------------
        virtual auto check_log_level(log_level lvl) const -> bool override final
        {
            return false;
        }
    };
}
