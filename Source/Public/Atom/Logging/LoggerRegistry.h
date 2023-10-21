#pragma once
#include "Atom/CoreAll.h"
#include "Atom/Logging/Logger.h"
#include "Atom/Logging/LoggerFactory.h"
#include "Atom/Logging/NullLogger.h"

namespace Atom::Logging
{    
    class LoggerRegistrationErr: public Err
    {
    public:
        constexpr LoggerRegistrationErr(StrView msg, StrView key):  
            Err(msg.data()), key{ key } {}

    public:
        StrView key;
    };

    /// --------------------------------------------------------------------------------------------
    /// LoggerRegistry is used manage loggers collectively and provides interface to register
    /// loggers with a key.
    ///
    /// # To Do
    /// - Add thread safety.
    /// - Add string like template parameter support.
    /// --------------------------------------------------------------------------------------------
    class LoggerRegistry
    {
        using TContainer = UnorderedMap<Str, LoggerPtr>;
        using TIter = typename TContainer::const_iterator;
        using TIterEnd = typename TContainer::const_iterator;

    public:
        LoggerRegistry()
        {
            SetDefaultLogger(GET_LOGGER_FACTORY().CreateLogger("DefaultLogger"));
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Registers Logger with its name.
        ///
        /// # Parameters
        /// - `logger`: Logger to register.
        /// ----------------------------------------------------------------------------------------
        auto RegisterLogger(LoggerPtr logger) -> Result<void, LoggerRegistrationErr>
        {
            debug_expects(logger != nullptr, "Cannot register null Logger.");

            StrView key = logger->Name();
            debug_expects(not key.isEmpty(), "Cannot register logger with null key.");

            if (_hasLogger(key))
            {
                return LoggerRegistrationErr{ "Logger already registered.", key };
            }

            _registerLogger(logger, Str(key));
            return Success();
        }

        /// ----------------------------------------------------------------------------------------
        /// Registers Logger with specified key. This key then can be used to retrieve the
        /// logger back.
        ///
        /// If a logger with this key is already registered throws exception.
        /// Use UnregisterLogger of ForceRegisterLogger to register logger in that case.
        ///
        /// # Parameters
        /// - `logger`: Logger to register.
        /// - `key`: Str used as key to register logger.
        /// ----------------------------------------------------------------------------------------
        auto RegisterLogger(LoggerPtr logger, StrView key) -> Result<void, LoggerRegistrationErr>
        {
            debug_expects(logger != nullptr, "Cannot register null Logger.");
            debug_expects(not key.isEmpty(), "Cannot register logger with null key.");

            if (_hasLogger(key))
            {
                return LoggerRegistrationErr{ "Logger already registered.", key };
            }

            _registerLogger(logger, Str(key));
            return Success();
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for void RegisterLogger(LoggerPtr logger, StrView key) for performance.
        ///
        /// # Parameters
        /// - `key`: RValue reference to Str, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        auto RegisterLogger(LoggerPtr logger, Str&& key) -> Result<void, LoggerRegistrationErr>
        {
            debug_expects(logger != nullptr, "Cannot register null Logger.");
            debug_expects(not key.isEmpty(), "Cannot register logger with null key.");

            if (_hasLogger(key))
            {
                return LoggerRegistrationErr{ "Logger already registered.", key };
            }

            _registerLogger(logger, mov(key));
            return Success();
        }

        /// ----------------------------------------------------------------------------------------
        /// Force registers logger with its name. If a logger with key {key} is already
        /// registered, then unregisters it and registers this.
        /// ----------------------------------------------------------------------------------------
        auto ForceRegisterLogger(LoggerPtr logger)
        {
            debug_expects(logger != nullptr, "Cannot register null Logger.");

            StrView key = logger->Name();
            debug_expects(not key.isEmpty(), "Cannot register logger with null key.");

            _forceRegisterLogger(logger, Str(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Force registers logger with specified key. If a logger with key {key} is already
        /// registered, then unregisters it and registers this.
        ///
        /// # Parameters
        /// - `logger`: Logger to register.
        /// - `key`: Str used as key to register logger.
        /// ----------------------------------------------------------------------------------------
        auto ForceRegisterLogger(LoggerPtr logger, StrView key)
        {
            debug_expects(logger != nullptr, "Cannot register null Logger.");
            debug_expects(not key.isEmpty(), "Cannot register logger with null key.");

            _forceRegisterLogger(logger, Str(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for void ForceRegisterLogger(LoggerPtr logger, StrView key) for performance.
        ///
        /// # Parameters
        /// - `key`: RValue reference to Str, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        auto ForceRegisterLogger(LoggerPtr logger, Str&& key)
        {
            debug_expects(logger != nullptr, "Cannot register null Logger.");
            debug_expects(not key.isEmpty(), "Cannot register logger with null key.");

            _forceRegisterLogger(logger, mov(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Tries to register logger with its name.
        ///
        /// # Parameters
        /// - `logger`: Logger to register.
        /// ----------------------------------------------------------------------------------------
        auto TryRegisterLogger(LoggerPtr logger) -> bool
        {
            if (logger == nullptr)
                return false;

            StrView key = logger->Name();
            if (key.isEmpty())
                return false;

            if (_hasLogger(key))
                return false;

            _registerLogger(logger, Str(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Tries to register logger with specified key.
        ///
        /// # Parameters
        /// - `logger`: Logger to register.
        /// - `key`: Str used as key to register logger.
        /// ----------------------------------------------------------------------------------------
        auto TryRegisterLogger(LoggerPtr logger, StrView key) -> bool
        {
            if (logger == nullptr)
                return false;

            if (key.isEmpty())
                return false;

            if (_hasLogger(key))
                return false;

            _registerLogger(logger, Str(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for bool TryRegisterLogger(LoggerPtr logger, StrView key) for
        /// performance.
        ///
        /// # Parameters
        /// - `key`: RValue reference to Str, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        auto TryRegisterLogger(LoggerPtr logger, Str&& key) -> bool
        {
            if (logger == nullptr)
                return false;

            if (key.isEmpty())
                return false;

            if (_hasLogger(key))
                return false;

            _registerLogger(logger, mov(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters the logger registered with key {key}.
        /// ----------------------------------------------------------------------------------------
        auto UnregisterLogger(StrView key) -> bool
        {
            debug_expects(not key.isEmpty(), "Cannot access logger with null key.");

            return _UnregisterLogger(key) != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters all loggers.
        /// ----------------------------------------------------------------------------------------
        auto UnregisterAllLoggers()
        {
            _loggers.clear();
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters and returns the logger registered with key {key}.
        ///
        /// # Parameters
        /// - `key`: Key used to register the logger.
        /// 
        /// # Retuns
        /// Logger registered with key {key}. If no logger was registered returns nullptr.
        /// ----------------------------------------------------------------------------------------
        auto UnregisterAndGetLogger(StrView key) -> LoggerPtr
        {
            debug_expects(not key.isEmpty(), "Cannot access logger with null key.");

            return _UnregisterLogger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the logger registered with key {key}.
        /// ----------------------------------------------------------------------------------------
        auto GetLogger(StrView key) const -> LoggerPtr
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
        /// Checks if a logger for key {key} is registered.
        /// ----------------------------------------------------------------------------------------
        auto HasLogger(StrView key) const -> bool
        {
            return _hasLogger(key);
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Get the default logger
        /// In case there is no default logger assigned, returns a fake logger but never
        /// returns nullptr.
        ///
        /// DefaultLogger is used to log global logs or when categorization is not necessary.
        /// ----------------------------------------------------------------------------------------
        auto GetDefaultLogger() const -> LoggerPtr
        {
            return _defaultLogger;
        }

        /// ----------------------------------------------------------------------------------------
        /// Set the default logger.
        ///
        /// # Parameters
        /// - `logger`: Logger to set as default logger. If null sets a fake logger instead.
        /// ----------------------------------------------------------------------------------------
        auto SetDefaultLogger(LoggerPtr logger) -> void
        {
            if (logger == nullptr)
            {
                logger = NullLogger::Instance;
            }

            _defaultLogger = logger;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Retuns
        /// TIter to first Key-Logger pair.
        /// ----------------------------------------------------------------------------------------
        auto iter() const -> TIter
        {
            return _loggers.begin();
        }

        /// ----------------------------------------------------------------------------------------
        /// # Retuns
        /// TIter to last Key-Logger pair.
        /// ----------------------------------------------------------------------------------------
        auto iterEnd() const -> TIter
        {
            return _loggers.cend();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Implementation functions
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    protected:
        auto _registerLogger(LoggerPtr logger, Str key) -> void
        {
            debug_expects(logger != nullptr);
            debug_expects(!key.isEmpty());

            _loggers.insert({ mov(key), mov(logger) });
        }

        auto _forceRegisterLogger(LoggerPtr logger, Str key) -> void
        {
            debug_expects(logger != nullptr);
            debug_expects(!key.isEmpty());

            _loggers.insert_or_assign(mov(key), mov(logger));
        }


        auto _UnregisterLogger(StrView key) -> LoggerPtr
        {
            auto it = _getLoggerIter(key);
            if (it == iterEnd())
            {
                return nullptr;
            }

            LoggerPtr logger = mov(const_cast<LoggerPtr&>(it->second));
            _loggers.erase(it);
            return logger;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto _hasLogger(StrView key) const -> bool
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

        auto _getLoggerIter(StrView key) const -> TIter
        {
            auto it = iter();
            for (; it != iterEnd(); it++)
            {
                if (it->first == key)
                {
                    break;
                }
            }

            return it;
        }

    protected:
        TContainer _loggers;
        LoggerPtr _defaultLogger;
    };

    inline auto GET_REGISTRY() -> LoggerRegistry&
    {
        static LoggerRegistry s_instance;
        return s_instance;
    }
}
