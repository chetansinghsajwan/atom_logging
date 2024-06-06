export module atom.logging:log_typeargets.console_log_typearget;

import atom.core;
import :log_msg;
import :log_typeargets.basic_log_typearget;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// console_log_typearget logs to console using stdout and stderr.
    /// --------------------------------------------------------------------------------------------
    export class console_log_typearget: public basic_log_typearget
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// default constructs the console_log_typearget.
        ///
        /// error_log_level is set to log_level::error.
        /// ----------------------------------------------------------------------------------------
        console_log_typearget()
            : _stdout(stdout)
            , _stderr(stderr)
            , _err_log_level(log_level::error)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// retuns the log_level specified for error logging.
        /// ----------------------------------------------------------------------------------------
        auto get_err_log_level() const -> log_level
        {
            return _err_log_level;
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the log_level specified for error logging.
        ///
        /// logs of same or above level are written to stderr.
        /// ----------------------------------------------------------------------------------------
        auto set_err_log_level(log_level lvl)
        {
            _err_log_level = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if log_level should be written to stderr.
        ///
        /// @returns `true` if {lvl >= get_err_log_level()}, else `false`.
        /// ----------------------------------------------------------------------------------------
        auto should_log_as_error(log_level lvl) const -> bool
        {
            return lvl >= _err_log_level;
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// writes the formatted message to stdout or stderr based on log level.
        ///
        /// if {should_log_as_error(log_msg.lvl) == true}, writes the message to stderr else to stdout.
        /// ----------------------------------------------------------------------------------------
        virtual auto _write(const log_msg& log_msg, string_view formatted_msg)
            -> void override final
        {
            FILE* file = _stdout;

            if (should_log_as_error(log_msg.lvl))
            {
                file = _stderr;
            }

            ::fwrite(formatted_msg.get_data(), sizeof(char), formatted_msg.get_count(), file);
        }

        /// ----------------------------------------------------------------------------------------
        /// flushes both stdout and stderr.
        /// ----------------------------------------------------------------------------------------
        virtual auto _flush() -> void override final
        {
            fflush(_stdout);
            fflush(_stderr);
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// file* to stdout.
        /// ----------------------------------------------------------------------------------------
        FILE* _stdout;

        /// ----------------------------------------------------------------------------------------
        /// file* to stderr.
        /// ----------------------------------------------------------------------------------------
        FILE* _stderr;

        /// ----------------------------------------------------------------------------------------
        /// error_log_level used to check if the message should be logged to stderr or stdout.
        /// ----------------------------------------------------------------------------------------
        log_level _err_log_level;
    };
}
