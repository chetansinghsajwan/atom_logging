export module atom.logging:logger_manager_impl;
import :logger;
import :logger;
import :log_target;
import atom.core;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// implementation logic for `logger_manager`.
    /// --------------------------------------------------------------------------------------------
    export class logger_manager_impl
    {
    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        struct creation_options
        {
            string_view name;
            initializer_list<log_target*> targets;
            bool register_logger = true;
            bool try_register = true;
            bool force_register = false;
            string_view key;
        };

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        struct registration_options
        {
            logger* logger;
            string_view key;
            bool force_register = false;
        };

        /// ----------------------------------------------------------------------------------------
        /// error when logger registration fails.
        /// ----------------------------------------------------------------------------------------
        class registration_error: public error
        {
        public:
            constexpr registration_error(string_view msg)
                : error(msg)
                , key()
            {}

            constexpr registration_error(string_view msg, string_view key)
                : error(msg)
                , key(key)
            {}

        public:
            string_view key;
        };

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        ///
        /// creates and sets default logger.
        /// ----------------------------------------------------------------------------------------
        logger_manager_impl() {}

        /// ----------------------------------------------------------------------------------------
        /// # virtual destructor
        /// ----------------------------------------------------------------------------------------
        virtual ~logger_manager_impl() {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        virtual auto create_logger(const creation_options& options)
            -> result<logger*, registration_error> = 0;

        /// ----------------------------------------------------------------------------------------
        /// registers `logger` with its name as the key.
        /// ----------------------------------------------------------------------------------------
        virtual auto register_logger(const registration_options& options)
            -> result<void, registration_error> = 0;

        /// ----------------------------------------------------------------------------------------
        /// unregisters the logger registered with the `key`.
        /// ----------------------------------------------------------------------------------------
        virtual auto unregister_logger(string_view key) -> logger* = 0;

        /// ----------------------------------------------------------------------------------------
        /// get the logger registered with the `key`.
        /// ----------------------------------------------------------------------------------------
        virtual auto get_logger(string_view key) const -> logger* = 0;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        virtual auto get_or_create_logger(const creation_options& options) -> logger* = 0;

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if a logger is registered with `key`.
        /// ----------------------------------------------------------------------------------------
        virtual auto has_logger(string_view key) const -> bool = 0;

        /// ----------------------------------------------------------------------------------------
        /// gets default_logger if already created before, else creates it and returns it.
        /// ----------------------------------------------------------------------------------------
        virtual auto init_default_logger() -> logger* = 0;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        virtual auto set_default_logger(logger* logger_) -> logger* = 0;
    };
}
