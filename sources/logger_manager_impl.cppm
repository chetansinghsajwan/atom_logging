export module atom.logging:logger_manager_impl;

import atom.core;
import :logger;
import :log_typearget;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// implementation logic for `logger_manager`.
    /// --------------------------------------------------------------------------------------------
    export class logger_manager_impl
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// options used during logger creation.
        /// ----------------------------------------------------------------------------------------
        struct creation_options
        {
            string_view name;                      // name of the logger.
            initializer_list<log_typearget*> targets; // targets to add into logger.
            bool register_logger = true;           // should the logger be registered.
            bool try_register = true;              // if registration fails, do not return error.
            bool force_register = false;           // force register the logger.
            string_view key;                       // key used to register the logger.
        };

        /// ----------------------------------------------------------------------------------------
        /// options used during registering logger.
        /// ----------------------------------------------------------------------------------------
        struct registration_options
        {
            class logger* logger;              // logger to register.
            string_view key;             // key used to register the logger.
            bool force_register = false; // force register the logger.
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
            string_view key; // key used during registration.
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
        /// creates a logger with name `options.name` and returns it.
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
        /// gets the logger registered with key `options.key`, else creates one and returns it.
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
        /// sets the default logger and returns the same. the returned value is then catched inside
        /// `logger_manager`. this is useful when you want to avoid setting the defualt logger null.
        /// ----------------------------------------------------------------------------------------
        virtual auto set_default_logger(class logger* logger) -> class logger* = 0;
    };
}
