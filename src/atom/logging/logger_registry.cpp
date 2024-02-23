export module atom.logging:logger_registry;
import :null_logger;
import :logger;
import :logger_factory;
import atom.core;

namespace atom::logging
{
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
    /// # to do
    /// - add thread safety.
    /// - add string like template parameter support.
    /// --------------------------------------------------------------------------------------------
    export class logger_registry
    {
    public:
        using container_type = unordered_map<string, logger_ptr>;
        using iter_type = typename container_type::const_iterator;
        using iter_end_type = typename container_type::const_iterator;

    public:
        logger_registry()
        {
            set_default_logger(get_logger_factory().create_logger("default_logger"));
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// registers logger with its name.
        ///
        /// # parameters
        /// - `logger`: logger to register.
        /// ----------------------------------------------------------------------------------------
        auto register_logger(logger_ptr logger) -> result<void, logger_registration_error>
        {
            contracts::debug_expects(logger != nullptr, "cannot register null logger.");

            string_view key = logger->name();
            contracts::debug_expects(not key.is_empty(), "cannot register logger with null key.");

            if (_has_logger(key))
            {
                return logger_registration_error("logger already registered.", key);
            }

            _register_logger(logger, string(key));
            return success();
        }

        /// ----------------------------------------------------------------------------------------
        /// registers logger with specified key. this key then can be used to retrieve the
        /// logger back.
        ///
        /// if a logger with this key is already registered throws exception.
        /// use unregister_logger of force_register_logger to register logger in that case.
        ///
        /// # parameters
        /// - `logger`: logger to register.
        /// - `key`: string used as key to register logger.
        /// ----------------------------------------------------------------------------------------
        auto register_logger(logger_ptr logger, string_view key)
            -> result<void, logger_registration_error>
        {
            contracts::debug_expects(logger != nullptr, "cannot register null logger.");
            contracts::debug_expects(not key.is_empty(), "cannot register logger with null key.");

            if (_has_logger(key))
            {
                return logger_registration_error("logger already registered.", key);
            }

            _register_logger(logger, string(key));
            return success();
        }

        /// ----------------------------------------------------------------------------------------
        /// overload for void register_logger(logger_ptr logger, string_view key) for performance.
        ///
        /// # parameters
        /// - `key`: rvalue reference to string, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        auto register_logger(logger_ptr logger, string&& key)
            -> result<void, logger_registration_error>
        {
            contracts::debug_expects(logger != nullptr, "cannot register null logger.");
            contracts::debug_expects(not key.is_empty(), "cannot register logger with null key.");

            if (_has_logger(key))
            {
                return logger_registration_error("logger already registered.", key);
            }

            _register_logger(logger, move(key));
            return success();
        }

        /// ----------------------------------------------------------------------------------------
        /// force registers logger with its name. if a logger with key {key} is already
        /// registered, then unregisters it and registers this.
        /// ----------------------------------------------------------------------------------------
        auto force_register_logger(logger_ptr logger)
        {
            contracts::debug_expects(logger != nullptr, "cannot register null logger.");

            string_view key = logger->name();
            contracts::debug_expects(not key.is_empty(), "cannot register logger with null key.");

            _force_register_logger(logger, string(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// force registers logger with specified key. if a logger with key {key} is already
        /// registered, then unregisters it and registers this.
        ///
        /// # parameters
        /// - `logger`: logger to register.
        /// - `key`: string used as key to register logger.
        /// ----------------------------------------------------------------------------------------
        auto force_register_logger(logger_ptr logger, string_view key)
        {
            contracts::debug_expects(logger != nullptr, "cannot register null logger.");
            contracts::debug_expects(not key.is_empty(), "cannot register logger with null key.");

            _force_register_logger(logger, string(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// overload for void force_register_logger(logger_ptr logger, string_view key) for performance.
        ///
        /// # parameters
        /// - `key`: rvalue reference to string, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        auto force_register_logger(logger_ptr logger, string&& key)
        {
            contracts::debug_expects(logger != nullptr, "cannot register null logger.");
            contracts::debug_expects(not key.is_empty(), "cannot register logger with null key.");

            _force_register_logger(logger, move(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// tries to register logger with its name.
        ///
        /// # parameters
        /// - `logger`: logger to register.
        /// ----------------------------------------------------------------------------------------
        auto try_register_logger(logger_ptr logger) -> bool
        {
            if (logger == nullptr)
                return false;

            string_view key = logger->name();
            if (key.is_empty())
                return false;

            if (_has_logger(key))
                return false;

            _register_logger(logger, string(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// tries to register logger with specified key.
        ///
        /// # parameters
        /// - `logger`: logger to register.
        /// - `key`: string used as key to register logger.
        /// ----------------------------------------------------------------------------------------
        auto try_register_logger(logger_ptr logger, string_view key) -> bool
        {
            if (logger == nullptr)
                return false;

            if (key.is_empty())
                return false;

            if (_has_logger(key))
                return false;

            _register_logger(logger, string(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// overload for bool try_register_logger(logger_ptr logger, string_view key) for
        /// performance.
        ///
        /// # parameters
        /// - `key`: rvalue reference to string, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        auto try_register_logger(logger_ptr logger, string&& key) -> bool
        {
            if (logger == nullptr)
                return false;

            if (key.is_empty())
                return false;

            if (_has_logger(key))
                return false;

            _register_logger(logger, move(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters the logger registered with key {key}.
        /// ----------------------------------------------------------------------------------------
        auto unregister_logger(string_view key) -> bool
        {
            contracts::debug_expects(not key.is_empty(), "cannot access logger with null key.");

            return _unregister_logger(key) != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters all loggers.
        /// ----------------------------------------------------------------------------------------
        auto unregister_all_loggers()
        {
            _loggers.clear();
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters and returns the logger registered with key {key}.
        ///
        /// # parameters
        /// - `key`: key used to register the logger.
        ///
        /// # retuns
        /// logger registered with key {key}. if no logger was registered returns nullptr.
        /// ----------------------------------------------------------------------------------------
        auto unregister_and_get_logger(string_view key) -> logger_ptr
        {
            contracts::debug_expects(not key.is_empty(), "cannot access logger with null key.");

            return _unregister_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// get the logger registered with key {key}.
        /// ----------------------------------------------------------------------------------------
        auto get_logger(string_view key) const -> logger_ptr
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

        /// ----------------------------------------------------------------------------------------
        /// checks if a logger for key {key} is registered.
        /// ----------------------------------------------------------------------------------------
        auto has_logger(string_view key) const -> bool
        {
            return _has_logger(key);
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// get the default logger
        /// in case there is no default logger assigned, returns a fake logger but never
        /// returns nullptr.
        ///
        /// default_logger is used to log global logs or when categorization is not necessary.
        /// ----------------------------------------------------------------------------------------
        auto get_default_logger() const -> logger_ptr
        {
            return _default_logger;
        }

        /// ----------------------------------------------------------------------------------------
        /// set the default logger.
        ///
        /// # parameters
        /// - `logger`: logger to set as default logger. if null sets a fake logger instead.
        /// ----------------------------------------------------------------------------------------
        auto set_default_logger(logger_ptr logger) -> void
        {
            if (logger == nullptr)
            {
                logger = null_logger::instance;
            }

            _default_logger = logger;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        /// # retuns
        /// iter_type to first key-logger pair.
        /// ----------------------------------------------------------------------------------------
        auto iter() const -> iter_type
        {
            return _loggers.begin();
        }

        /// ----------------------------------------------------------------------------------------
        /// # retuns
        /// iter_type to last key-logger pair.
        /// ----------------------------------------------------------------------------------------
        auto iter_end() const -> iter_type
        {
            return _loggers.cend();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// implementation functions
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    protected:
        auto _register_logger(logger_ptr logger, string key) -> void
        {
            contracts::debug_expects(logger != nullptr);
            contracts::debug_expects(!key.is_empty());

            _loggers.insert({ move(key), move(logger) });
        }

        auto _force_register_logger(logger_ptr logger, string key) -> void
        {
            contracts::debug_expects(logger != nullptr);
            contracts::debug_expects(!key.is_empty());

            _loggers.insert_or_assign(move(key), move(logger));
        }

        auto _unregister_logger(string_view key) -> logger_ptr
        {
            auto it = _get_logger_iter(key);
            if (it == iter_end())
            {
                return nullptr;
            }

            logger_ptr logger = move(const_cast<logger_ptr&>(it->second));
            _loggers.erase(it);
            return logger;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto _has_logger(string_view key) const -> bool
        {
            for (auto pair : _loggers)
            {
                if (pair.first == key)
                {
                    return true;
                }
            }

            return false;
        }

        auto _get_logger_iter(string_view key) const -> iter_type
        {
            auto it = iter();
            for (; it != iter_end(); it++)
            {
                if (it->first == key)
                {
                    break;
                }
            }

            return it;
        }

    protected:
        container_type _loggers;
        logger_ptr _default_logger;
    };

    export inline auto get_registry() -> logger_registry&
    {
        static logger_registry s_instance;
        return s_instance;
    }
}
