export module atom.logging:default_logger_manager_impl;
import :logger;
import :logger;
import :log_target;
import :logger_manager_impl;
import :console_log_target;
import :simple_logger;
import atom.core;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <bool st>
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
        ///
        /// ----------------------------------------------------------------------------------------
        auto create_logger(const creation_options& options)
            -> result<logger*, registration_error> override
        {
            if (not options.register_logger)
                return _create_logger(options.name, options.targets);

            string_view options_key = options.key.is_empty() ? options.name : options.key;

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
        ///
        /// ----------------------------------------------------------------------------------------
        virtual auto get_or_create_logger(const creation_options& options) -> logger* override
        {
            string_view options_key = options.key.is_empty() ? options.name : options.key;
            logger* logger_ = _get_logger(options_key);

            if (logger_ == nullptr)
            {
                logger_ = _create_logger(options.name, options.targets);
                _register_logger(logger_, options_key);
            }

            return logger_;
        }

        /// ----------------------------------------------------------------------------------------
        /// registers `logger` with its name as the key.
        /// ----------------------------------------------------------------------------------------
        auto register_logger(const registration_options& options)
            -> result<void, registration_error> override
        {
            if (options.logger == nullptr)
                return registration_error("cannot register null logger.");

            string_view options_key =
                options.key.is_empty() ? options.logger->get_name() : options.key;

            if (options_key.is_empty())
                return registration_error("cannot register logger for empty key.");

            if (options.force_register)
            {
                _register_logger_forced(options.logger, options_key);
                return success();
            }

            bool registered = _register_logger(options.logger, options_key);
            if (not registered)
            {
                return registration_error(
                    "a logger with this key is already registered.", options_key);
            }

            return success();
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters the logger registered with the `key`.
        /// ----------------------------------------------------------------------------------------
        virtual auto unregister_logger(string_view key) -> logger* override
        {
            if (key.is_empty())
                return nullptr;

            return _unregister_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// get the logger registered with the `key`.
        /// ----------------------------------------------------------------------------------------
        virtual auto get_logger(string_view key) const -> logger* override
        {
            if (key.is_empty())
                return nullptr;

            return _get_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if a logger is registered with `key`.
        /// ----------------------------------------------------------------------------------------
        virtual auto has_logger(string_view key) const -> bool override
        {
            if (key.is_empty())
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
            logger->add_target(new console_log_target());
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

    using default_logger_manager_impl_st = default_logger_manager_impl<true>;
    using default_logger_manager_impl_mt = default_logger_manager_impl<false>;
}
