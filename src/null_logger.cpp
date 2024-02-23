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
        null_logger() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// returns name of the null_logger.
        /// ----------------------------------------------------------------------------------------
        virtual auto get_name() const -> string_view override final
        {
            return _name;
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

    public:
        /// ----------------------------------------------------------------------------------------
        /// static instance of this type because this type is stateless.
        /// it's recommended to use this instead of creating one each time.
        /// ----------------------------------------------------------------------------------------
        static logger* instance;

    private:
        /// ----------------------------------------------------------------------------------------
        /// name of the null_logger.
        /// ----------------------------------------------------------------------------------------
        static inline string_view _name = "null_logger";
    };

    logger* null_logger::instance = new null_logger();
}
