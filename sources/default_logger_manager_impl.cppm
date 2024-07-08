export module atom_logging:default_logger_manager_impl;

import atom_core;
import :logger;
import :log_msg;
import :log_target;
import :logger_manager_impl;
import :simple_logger;
import :log_targets.console_log_target;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <bool st>
    class default_logger_manager_impl: public logger_manager_impl
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        ///
        /// creates and sets default logger.
        /// ----------------------------------------------------------------------------------------
        default_logger_manager_impl()
            : logger_manager_impl()
            , _loggers()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # virtual destructor
        /// ----------------------------------------------------------------------------------------
        virtual ~default_logger_manager_impl() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// creates a `simple_logger_st` instance.
        ///
        /// @param options.name name of the logger to create
        ///
        /// @param options.targets list of targets to add to the created logger.
        ///
        /// @param options.register_logger if `true`, registers the created logger.
        ///
        /// @param options.try_register if `true` and registration is not possible, returns the
        ///     created logger instead of error.
        ///
        /// @param options.force_register if `true` and another logger is registerd with this key,
        ///     unregisters that logger and registers this.
        ///
        /// @param options.key key used to register the logger. if key is empty, uses
        ///     `options.name` as the key.
        /// ----------------------------------------------------------------------------------------
        virtual auto create_logger(
            const creation_options& options) -> result<logger*, registration_error> override
        {
            if (not options.register_logger)
                return _create_logger(options.name, options.targets);

            string_view options_key = options.key | ranges::is_empty() ? options.name : options.key;

            bool already_registered = _has_logger(options_key);
            if (not already_registered)
            {
                logger* logger_ = _create_logger(options.name, options.targets);
                _register_logger(logger_, options_key);
                return logger_;
            }

            if (options.try_register)
            {
                return _create_logger(options.name, options.targets);
            }

            if (not options.force_register)
            {
                return registration_error(
                    "a logger with this key is already registered.", options_key);
            }

            logger* logger_ = _create_logger(options.name, options.targets);
            _register_logger_forced(logger_, options_key);
            return logger_;
        }

        /// ----------------------------------------------------------------------------------------
        /// if a logger with key `options.key` exists, returns it else creates a new logger returns
        /// it.
        ///
        /// @param options.name name of the logger to create
        ///
        /// @param options.targets list of targets to add to the created logger.
        ///
        /// @param options.register_logger if `true`, registers the created logger.
        ///
        /// @param options.try_register if `true` and registration is not possible, returns the
        ///     created logger instead of error.
        ///
        /// @param options.force_register if `true` and another logger is registerd with this key,
        ///     unregisters that logger and registers this.
        ///
        /// @param options.key key used to register the logger. if key is empty, uses
        ///     `options.name` as the key.
        /// ----------------------------------------------------------------------------------------
        virtual auto get_or_create_logger(const creation_options& options) -> logger* override
        {
            string_view options_key = options.key | ranges::is_empty() ? options.name : options.key;
            logger* logger_ = _get_logger(options_key);

            if (logger_ == nullptr)
            {
                logger_ = _create_logger(options.name, options.targets);
                _register_logger(logger_, options_key);
            }

            return logger_;
        }

        /// ----------------------------------------------------------------------------------------
        /// registers `logger` with its the specified key.
        ///
        /// @param options.logger logger to register.
        ///
        /// @param options.key key used to register the logger. if key is empty,
        ///     `options.logger->get_name()` is used as the key.
        ///
        /// @throws registration_error if `options.logger` is null.
        ///
        /// @throws registration_error if `options.key` is empty.
        ///
        /// @throws registration_error if `options.force_register` is `false` and another logger is
        ///     already registerd.
        /// ----------------------------------------------------------------------------------------
        virtual auto register_logger(
            const registration_options& options) -> result<void, registration_error> override
        {
            if (options.logger == nullptr)
                return registration_error("cannot register null logger.");

            string_view options_key =
                options.key | ranges::is_empty() ? options.logger->get_name() : options.key;

            if (options_key | ranges::is_empty())
                return registration_error("cannot register logger for empty key.");

            if (options.force_register)
            {
                _register_logger_forced(options.logger, options_key);
                return { create_from_void };
            }

            bool registered = _register_logger(options.logger, options_key);
            if (not registered)
            {
                return registration_error(
                    "a logger with this key is already registered.", options_key);
            }

            return { create_from_void };
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters the logger registered with the `key` and returns it.
        /// ----------------------------------------------------------------------------------------
        virtual auto unregister_logger(string_view key) -> logger* override
        {
            if (key | ranges::is_empty())
                return nullptr;

            return _unregister_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// get the logger registered with the `key`.
        /// ----------------------------------------------------------------------------------------
        virtual auto get_logger(string_view key) const -> logger* override
        {
            if (key | ranges::is_empty())
                return nullptr;

            return _get_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if a logger is registered with `key`.
        /// ----------------------------------------------------------------------------------------
        virtual auto has_logger(string_view key) const -> bool override
        {
            if (key | ranges::is_empty())
                return false;

            return _has_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// gets default_logger if already created before, else creates it and returns it.
        /// ----------------------------------------------------------------------------------------
        virtual auto init_default_logger() -> logger* override
        {
            return _create_logger("default_logger", {});
        }

        /// ----------------------------------------------------------------------------------------
        /// gets default_logger if already created before, else creates it and returns it.
        /// ----------------------------------------------------------------------------------------
        virtual auto set_default_logger(logger* logger_) -> logger* override
        {
            _default_logger = logger_;
            return _default_logger;
        }

    protected:
        auto _create_logger(string_view name, initializer_list<log_target*> targets) -> logger*
        {
            simple_logger_st* logger = new simple_logger_st(name);
            logger->set_log_level(log_level::trace);
            logger->set_flush_level(log_level::trace);

            console_log_target* console = new console_log_target();
            console->set_log_level(log_level::trace);
            console->set_flush_level(log_level::trace);

            logger->add_target(console);
            logger->add_targets(targets);

            return logger;
        }

        auto _register_logger(logger* logger, string key) -> bool
        {
            return _loggers.insert({ key, logger }).second;
        }

        auto _register_logger_forced(logger* logger, string key) -> void
        {
            _loggers.insert_or_assign(key, logger);
        }

        auto _unregister_logger(string_view key) -> logger*
        {
            for (auto it = _loggers.cbegin(); it != _loggers.cend(); it++)
            {
                if (it->first == key)
                {
                    logger* logger = it->second;
                    _loggers.erase(it);
                    return logger;
                }
            }

            return nullptr;
        }

        auto _get_logger(string_view key) const -> logger*
        {
            for (auto pair : _loggers)
            {
                if (pair.first == key)
                {
                    return pair.second;
                }
            }

            return nullptr;
        }

        auto _has_logger(string_view key) const -> bool
        {
            return _get_logger(key) != nullptr;
        }

    protected:
        unordered_map<string, logger*> _loggers;
        logger* _default_logger;
    };

    export using default_logger_manager_impl_st = default_logger_manager_impl<true>;
    export using default_logger_manager_impl_mt = default_logger_manager_impl<false>;
}
