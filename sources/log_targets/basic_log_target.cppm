export module atom.logging:log_typeargets.basic_log_typearget;

import atom.core;
import :log_msg;
import :log_typearget;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// base class for log_typeargets with base functionality like formatting, thread safety and
    /// level filtering.
    ///
    /// # to do
    ///
    /// - add thread safety.
    /// - make default log and flush level global.
    /// --------------------------------------------------------------------------------------------
    export class basic_log_typearget: public log_typearget
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        basic_log_typearget()
            : _log_level(log_level::debug)
            , _flush_level(log_level::info)
            , _has_written(false)
            , _always_flush(false)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// filters and formats the log_msg and passes it to write.
        /// ----------------------------------------------------------------------------------------
        virtual auto write(const log_msg& log_msg) -> void override final
        {
            if (check_log_level(log_msg.lvl))
            {
                string result = string::format("[{}] [{}] {}: {}\n", log_msg.time, log_msg.lvl,
                    log_msg.logger_name, log_msg.msg);

                _has_written = true;
                _write(log_msg, result);

                if (check_flush_level(log_msg.lvl))
                {
                    _flush();
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// flushes if `[`should_flush()`] == true`.
        /// ----------------------------------------------------------------------------------------
        virtual auto flush() -> void override final
        {
            if (should_flush())
            {
                _flush();
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// get the log level.
        /// ----------------------------------------------------------------------------------------
        auto get_log_level() const -> log_level
        {
            return _log_level;
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the log level.
        /// ----------------------------------------------------------------------------------------
        auto set_log_level(log_level lvl)
        {
            _log_level = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if we should log the message of specified level.
        /// ----------------------------------------------------------------------------------------
        auto check_log_level(log_level lvl) const -> bool
        {
            if (lvl == log_level::off)
                return false;
            if (lvl < _log_level)
                return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// gets the flush level.
        /// ----------------------------------------------------------------------------------------
        auto get_flush_level() const -> log_level
        {
            return _flush_level;
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the flush level.
        /// ----------------------------------------------------------------------------------------
        auto set_flush_level(log_level lvl)
        {
            _flush_level = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if should flush after logging the message of specified level.
        /// it also asks [`should_flush()`].
        /// ----------------------------------------------------------------------------------------
        auto check_flush_level(log_level lvl) const -> bool
        {
            if (!_has_written)
                return false;
            if (lvl == log_level::off)
                return false;
            if (lvl < _flush_level)
                return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if we should flush.
        ///
        /// # returns
        /// `true` if there has been a log since last flush, else `false`.
        /// ----------------------------------------------------------------------------------------
        auto should_flush() const -> bool
        {
            return _always_flush || _has_written;
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// write implementation.
        ///
        /// @param[in] log_msg log message object passed for logging.
        /// @param[in] formatted_msg formatted message generated from {log_msg}.
        /// ----------------------------------------------------------------------------------------
        virtual auto _write(const log_msg& log_msg, string_view formatted_msg) -> void = 0;

        /// ----------------------------------------------------------------------------------------
        /// flush implementation.
        /// ----------------------------------------------------------------------------------------
        virtual auto _flush() -> void = 0;

    protected:
        /// ----------------------------------------------------------------------------------------
        /// log level used to filter log messages.
        /// ----------------------------------------------------------------------------------------
        log_level _log_level;

        /// ----------------------------------------------------------------------------------------
        /// flush level used to check if to call flush after logging.
        /// ----------------------------------------------------------------------------------------
        log_level _flush_level;

        /// ----------------------------------------------------------------------------------------
        /// value used to check if there has been any write since last flush.
        /// ----------------------------------------------------------------------------------------
        bool _has_written;

        /// ----------------------------------------------------------------------------------------
        /// if true always calls underlying flush _flush(), even if not necessary.
        /// this doesn't override check_flush_level(log_level lvl) check. it only affects
        /// should_flush().
        /// ----------------------------------------------------------------------------------------
        bool _always_flush;
    };
}
