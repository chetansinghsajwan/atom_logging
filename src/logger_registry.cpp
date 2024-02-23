export module atom.logging:logger_registry;
import :logger;
import :logger_factory;
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

    /// --------------------------------------------------------------------------------------------
    /// logger_registry is used manage loggers collectively and provides interface to register
    /// loggers with a key.
    ///
    /// default_logger is used to log global logs or when categorization is not necessary.
    ///
    /// # to do
    ///
    /// - add thread safety.
    /// --------------------------------------------------------------------------------------------
    export class logger_registry
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// set a new instance.
        /// ----------------------------------------------------------------------------------------
        static auto set_instance(logger_registry* instance) -> void
        {
            _instance = instance;
        }

        /// ----------------------------------------------------------------------------------------
        /// get current instance. by default it is set to `logger_registry` instance.
        /// ----------------------------------------------------------------------------------------
        static auto get_instance() -> logger_registry*
        {
            return _instance;
        }

    private:
        static logger_registry* _instance;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        ///
        /// creates and sets default logger.
        /// ----------------------------------------------------------------------------------------
        logger_registry()
            : _loggers()
            , _default_logger()
        {
            _default_logger = logger_factory::get_instance()->create_logger("default_logger");
        }

        /// ----------------------------------------------------------------------------------------
        /// # virtual destructor
        /// ----------------------------------------------------------------------------------------
        virtual ~logger_registry() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// registers `logger` with its name as the key.
        /// ----------------------------------------------------------------------------------------
        auto register_(logger* logger) -> result<void, logger_registration_error>
        {
            contracts::debug_expects(logger != nullptr);

            string_view key = logger->get_name();
            contracts::debug_expects(not key.is_empty());

            if (_has_logger(key))
            {
                return logger_registration_error(
                    "a logger with this key is already registered.", key);
            }

            _register_logger(logger, key);
            return success();
        }

        /// ----------------------------------------------------------------------------------------
        /// registers `logger` with the specified `key`.
        /// ----------------------------------------------------------------------------------------
        auto register_with_key(logger* logger, string_view key)
            -> result<void, logger_registration_error>
        {
            contracts::debug_expects(logger != nullptr);
            contracts::debug_expects(not key.is_empty());

            if (_has_logger(key))
            {
                return logger_registration_error(
                    "a logger with this key is already registered.", key);
            }

            _register_logger(logger, key);
            return success();
        }

        /// ----------------------------------------------------------------------------------------
        /// registers `logger` with its name as the key. if a logger with the same key is already
        /// registered, then unregisters it and registers this.
        /// ----------------------------------------------------------------------------------------
        auto register_forced(logger* logger)
        {
            contracts::debug_expects(logger != nullptr);

            string_view key = logger->get_name();
            contracts::debug_expects(not key.is_empty());

            _register_logger_forced(logger, key);
        }

        /// ----------------------------------------------------------------------------------------
        /// registers `logger` with the specified `key`. if a logger with the same key is already
        /// registered, then unregisters it and registers this.
        /// ----------------------------------------------------------------------------------------
        auto register_with_key_forced(logger* logger, string_view key)
        {
            contracts::debug_expects(logger != nullptr);
            contracts::debug_expects(not key.is_empty());

            _register_logger_forced(logger, key);
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters the logger registered with the `key`.
        /// ----------------------------------------------------------------------------------------
        auto unregister(string_view key) -> bool
        {
            contracts::debug_expects(not key.is_empty());

            return _unregister_logger(key) != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters all loggers.
        /// ----------------------------------------------------------------------------------------
        auto unregister_all() -> void
        {
            _loggers.clear();
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters and returns the logger registered with `key`.
        /// ----------------------------------------------------------------------------------------
        auto unregister_and_get(string_view key) -> logger*
        {
            contracts::debug_expects(not key.is_empty());

            return _unregister_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// get the logger registered with the `key`.
        /// ----------------------------------------------------------------------------------------
        auto get(string_view key) const -> logger*
        {
            contracts::debug_expects(not key.is_empty());

            return _get_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if a logger is registered with `key`.
        /// ----------------------------------------------------------------------------------------
        auto has(string_view key) const -> bool
        {
            contracts::debug_expects(not key.is_empty());

            return _has_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the default logger.
        /// ----------------------------------------------------------------------------------------
        auto set_default(logger* logger) -> void
        {
            _default_logger = logger;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the default logger.
        /// ----------------------------------------------------------------------------------------
        auto get_default() const -> logger*
        {
            return _default_logger;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if the default logger is not null.
        /// ----------------------------------------------------------------------------------------
        auto has_default() const -> bool
        {
            return _default_logger != nullptr;
        }

    protected:
        auto _register_logger(logger* logger, string key) -> void
        {
            _loggers.insert({ move(key), move(logger) });
        }

        auto _register_logger_forced(logger* logger, string key) -> void
        {
            _loggers.insert_or_assign(move(key), move(logger));
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

    logger_registry* logger_registry::_instance = new logger_registry();
}
