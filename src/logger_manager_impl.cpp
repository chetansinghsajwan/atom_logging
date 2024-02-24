export module atom.logging:logger_manager_impl;
import :logger;
import :log_target;
import :logging_errors;
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
        /// # default constructor
        ///
        /// creates and sets default logger.
        /// ----------------------------------------------------------------------------------------
        logger_manager_impl()
            : _loggers()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # virtual destructor
        /// ----------------------------------------------------------------------------------------
        virtual ~logger_manager_impl() {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto create_logger(string_view name, initializer_list<log_target*> targets) -> logger*
        {
            return nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// registers `logger` with its name as the key.
        /// ----------------------------------------------------------------------------------------
        auto register_logger(logger* logger) -> result<void, logger_registration_error>
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
        auto register_logger_with_key(logger* logger, string_view key)
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
        auto register_logger_forced(logger* logger)
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
        auto register_logger_with_key_forced(logger* logger, string_view key)
        {
            contracts::debug_expects(logger != nullptr);
            contracts::debug_expects(not key.is_empty());

            _register_logger_forced(logger, key);
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters the logger registered with the `key`.
        /// ----------------------------------------------------------------------------------------
        auto unregister_logger(string_view key) -> bool
        {
            contracts::debug_expects(not key.is_empty());

            return _unregister_logger(key) != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters all loggers.
        /// ----------------------------------------------------------------------------------------
        auto unregister_all_loggers() -> void
        {
            _loggers.clear();
        }

        /// ----------------------------------------------------------------------------------------
        /// unregisters and returns the logger registered with `key`.
        /// ----------------------------------------------------------------------------------------
        auto unregister_and_get_logger(string_view key) -> logger*
        {
            contracts::debug_expects(not key.is_empty());

            return _unregister_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// get the logger registered with the `key`.
        /// ----------------------------------------------------------------------------------------
        auto get_logger(string_view key) const -> logger*
        {
            contracts::debug_expects(not key.is_empty());

            return _get_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto get_or_create_logger(string_view name, initializer_list<log_target*> targets)
            -> logger*
        {
            return nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if a logger is registered with `key`.
        /// ----------------------------------------------------------------------------------------
        auto has_logger(string_view key) const -> bool
        {
            contracts::debug_expects(not key.is_empty());

            return _has_logger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// gets default_logger if already created before, else creates it and returns it.
        /// ----------------------------------------------------------------------------------------
        auto get_or_create_default_logger() -> logger*
        {
            if (_default_logger == nullptr)
            {
                _default_logger = get_or_create_logger("default_logger", {});
            }

            return _default_logger;
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

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <bool st>
    class default_logger_manager_impl: public logger_manager_impl
    {};

    using default_logger_manager_impl_st = default_logger_manager_impl<true>;
    using default_logger_manager_impl_mt = default_logger_manager_impl<false>;
}
