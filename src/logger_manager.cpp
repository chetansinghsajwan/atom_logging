export module atom.logging:logger_manager;
import :logger;
import :log_target;
import :logger_manager_impl;
import :logging_errors;
import atom.core;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// provides registration and creation abstraction layer.
    /// --------------------------------------------------------------------------------------------
    export class logger_manager
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        logger_manager() = delete;

        /// ----------------------------------------------------------------------------------------
        /// # virtual destructor
        /// ----------------------------------------------------------------------------------------
        ~logger_manager() = delete;

    public:
        /// ----------------------------------------------------------------------------------------
        /// initializes the underlying implementation with `default_logger_manager_impl_st`.
        /// ----------------------------------------------------------------------------------------
        static auto initialize() -> void
        {
            contracts::expects(not is_initialized(), "`logger_manager` is already initialized.");

            _initialize(new default_logger_manager_impl_st());
        }

        /// ----------------------------------------------------------------------------------------
        /// initializes the underlying implementation with the specified implementation.
        /// ----------------------------------------------------------------------------------------
        static auto initialize_with_impl(logger_manager_impl* impl) -> void
        {
            contracts::expects(impl != nullptr, "cannot initialize `logger_manager` with null"
                                                "implementation instance");

            _initialize(impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// clears up any resources.
        /// ----------------------------------------------------------------------------------------
        static auto finalize() -> void
        {
            contracts::expects(is_initialized(), "`logger_manager` is not initialized yet.");

            _finalize();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if a `logger_mananger_impl` is set.
        /// ----------------------------------------------------------------------------------------
        static auto is_initialized() -> bool
        {
            return _impl != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the underlying implementation object.
        /// ----------------------------------------------------------------------------------------
        static auto get_impl() -> logger_manager_impl*
        {
            return _impl;
        }

        /// ----------------------------------------------------------------------------------------
        /// creates a `logger` object with `name` and registers with `name` as key.
        /// ----------------------------------------------------------------------------------------
        static auto create_logger(string_view name, initializer_list<log_target*> targets)
            -> logger*
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->create_logger(name, targets);
        }

        /// ----------------------------------------------------------------------------------------
        /// creates a `logger` object with `name` but doesn't registers it.
        /// ----------------------------------------------------------------------------------------
        static auto create_unregistered_logger(
            string_view name, initializer_list<log_target*> targets) -> logger*
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->create_logger(name, targets);
        }

        /// ----------------------------------------------------------------------------------------
        /// registers `logger` with its name as the key.
        /// ----------------------------------------------------------------------------------------
        static auto register_logger(logger* logger) -> result<void, logger_registration_error>
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->register_logger(logger);
        }

        /// ----------------------------------------------------------------------------------------
        /// registers `logger` with the specified `key`.
        /// ----------------------------------------------------------------------------------------
        static auto register_with_key_logger(logger* logger, string_view key)
            -> result<void, logger_registration_error>
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->register_logger_with_key(logger, key);
        }

        /// ----------------------------------------------------------------------------------------
        /// registers `logger` with its name as the key. if a logger with the same key is already
        /// registered, then unregisters it and registers this.
        /// ----------------------------------------------------------------------------------------
        static auto register_forced_logger(logger* logger)
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->register_logger_forced(logger);
        }

        /// ----------------------------------------------------------------------------------------
        /// registers `logger` with the specified `key`. if a logger with the same key is already
        /// registered, then unregisters it and registers this.
        /// ----------------------------------------------------------------------------------------
        static auto register_with_key_forced_logger(logger* logger, string_view key)
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->register_logger_with_key_forced(logger, key);
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters the logger registered with the `key`.
        /// ----------------------------------------------------------------------------------------
        static auto unregister_logger(string_view key) -> bool
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->unregister_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters all loggers.
        /// ----------------------------------------------------------------------------------------
        static auto unregister_all_loggers() -> void
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            _impl->unregister_all_loggers();
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters and returns the logger registered with `key`.
        /// ----------------------------------------------------------------------------------------
        static auto unregister_and_get_logger(string_view key) -> logger*
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->unregister_and_get_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// get the logger registered with the `key`.
        /// ----------------------------------------------------------------------------------------
        static auto get_logger(string_view key) -> logger*
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->get_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// gets the registered logger with key `name` or creates and registers one and returns it.
        /// ----------------------------------------------------------------------------------------
        static auto get_or_create_logger(string_view name, initializer_list<log_target*> targets)
            -> logger*
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->get_or_create_logger(name, targets);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if a logger is registered with `key`.
        /// ----------------------------------------------------------------------------------------
        static auto has_logger(string_view key) -> bool
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->has_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the default logger.
        /// ----------------------------------------------------------------------------------------
        static auto set_default_logger(logger* logger) -> void
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            _default_logger = logger;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the default logger.
        /// ----------------------------------------------------------------------------------------
        static auto get_default_logger() -> logger*
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            return _default_logger;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if the default logger is not null.
        /// ----------------------------------------------------------------------------------------
        static auto has_default_logger() -> bool
        {
            contracts::debug_expects(is_initialized(), "`logger_manager` is not initialized yet.");

            return _default_logger != nullptr;
        }

    private:
        static auto _initialize(logger_manager_impl* impl) -> void
        {
            _impl = impl;
            _default_logger = _impl->get_or_create_default_logger();
        }

        static auto _finalize() -> void
        {
            delete _impl;
        }

    private:
        static inline logger_manager_impl* _impl = nullptr;
        static inline logger* _default_logger = nullptr;
    };
}
