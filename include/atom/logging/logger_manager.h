#pragma once
#include "atom/logging/logger.h"
#include "atom/logging/log_target.h"
#include "atom/logging/logger_manager_impl.h"
#include "atom/logging/default_logger_manager_impl.h"
#include "atom.core.h"

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// provides registration and creation abstraction layer.
    /// --------------------------------------------------------------------------------------------
    class logger_manager
    {
    public:
        using creation_options = logger_manager_impl::creation_options;
        using registration_options = logger_manager_impl::registration_options;
        using registration_error = logger_manager_impl::registration_error;

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
            ATOM_EXPECTS(not is_initialized(), "`logger_manager` is already initialized.");

            _initialize(new default_logger_manager_impl_st());
        }

        /// ----------------------------------------------------------------------------------------
        /// initializes the underlying implementation with the specified implementation.
        /// ----------------------------------------------------------------------------------------
        static auto initialize_with_impl(logger_manager_impl* impl) -> void
        {
            ATOM_EXPECTS(impl != nullptr, "cannot initialize `logger_manager` with null"
                                          "implementation instance");

            _initialize(impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// clears up any resources.
        /// ----------------------------------------------------------------------------------------
        static auto finalize() -> void
        {
            ATOM_EXPECTS(is_initialized(), "`logger_manager` is not initialized yet.");

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
        static auto create_logger(const creation_options& options)
            -> result<logger*, registration_error>
        {
            ATOM_DEBUG_EXPECTS(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->create_logger(options);
        }

        /// ----------------------------------------------------------------------------------------
        /// gets the registered logger with key `name` or creates one and returns it.
        /// ----------------------------------------------------------------------------------------
        static auto get_or_create_logger(const creation_options& options) -> logger*
        {
            ATOM_DEBUG_EXPECTS(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->get_or_create_logger(options);
        }

        /// ----------------------------------------------------------------------------------------
        /// registers `logger` with its name as the key.
        /// ----------------------------------------------------------------------------------------
        static auto register_logger(const registration_options& options)
            -> result<void, registration_error>
        {
            ATOM_DEBUG_EXPECTS(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->register_logger(options);
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters the logger registered with the `key`.
        /// ----------------------------------------------------------------------------------------
        static auto unregister_logger(string_view key) -> logger*
        {
            ATOM_DEBUG_EXPECTS(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->unregister_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// get the logger registered with the `key`.
        /// ----------------------------------------------------------------------------------------
        static auto get_logger(string_view key) -> logger*
        {
            ATOM_DEBUG_EXPECTS(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->get_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if a logger is registered with `key`.
        /// ----------------------------------------------------------------------------------------
        static auto has_logger(string_view key) -> bool
        {
            ATOM_DEBUG_EXPECTS(is_initialized(), "`logger_manager` is not initialized yet.");

            return _impl->has_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the default logger.
        /// ----------------------------------------------------------------------------------------
        static auto set_default_logger(logger* logger) -> void
        {
            ATOM_DEBUG_EXPECTS(is_initialized(), "`logger_manager` is not initialized yet.");

            _default_logger = _impl->set_default_logger(logger);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the default logger.
        /// ----------------------------------------------------------------------------------------
        static auto get_default_logger() -> logger*
        {
            ATOM_DEBUG_EXPECTS(is_initialized(), "`logger_manager` is not initialized yet.");

            return _default_logger;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if the default logger is not null.
        /// ----------------------------------------------------------------------------------------
        static auto has_default_logger() -> bool
        {
            ATOM_DEBUG_EXPECTS(is_initialized(), "`logger_manager` is not initialized yet.");

            return _default_logger != nullptr;
        }

    private:
        static auto _initialize(logger_manager_impl* impl) -> void
        {
            _impl = impl;
            _default_logger = _impl->init_default_logger();
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
