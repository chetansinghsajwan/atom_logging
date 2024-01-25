export module atom.logging:logger;
import :core;
import :log_msg;
import :log_target;
import atom.core;

namespace atom::logging
{
    export class logger
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// get the name of the logger.
        /// ----------------------------------------------------------------------------------------
        virtual auto name() const -> string_view = 0;

        /// ----------------------------------------------------------------------------------------
        /// calls log(log_level::trace, msg, fwd(args)...).
        /// ----------------------------------------------------------------------------------------
        template <rlog_arg... arg_types>
        auto log_trace(log_str<arg_types...> msg, arg_types&&... args)
        {
            log(log_level::trace, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls log(log_level::debug, msg, fwd(args)...).
        /// ----------------------------------------------------------------------------------------
        template <rlog_arg... arg_types>
        auto log_debug(log_str<arg_types...> msg, arg_types&&... args)
        {
            log(log_level::debug, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls log(log_level::info, msg, fwd(args)...).
        /// ----------------------------------------------------------------------------------------
        template <rlog_arg... arg_types>
        auto log_info(log_str<arg_types...> msg, arg_types&&... args)
        {
            log(log_level::info, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls log(log_level::warn, msg, fwd(args)...).
        /// ----------------------------------------------------------------------------------------
        template <rlog_arg... arg_types>
        auto log_warn(log_str<arg_types...> msg, arg_types&&... args)
        {
            log(log_level::warn, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls log(log_level::error, msg, fwd(args)...).
        /// ----------------------------------------------------------------------------------------
        template <rlog_arg... arg_types>
        auto log_error(log_str<arg_types...> msg, arg_types&&... args)
        {
            log(log_level::error, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls log(log_level::fatal, msg, fwd(args)...).
        /// ----------------------------------------------------------------------------------------
        template <rlog_arg... arg_types>
        auto log_fatal(log_str<arg_types...> msg, arg_types&&... args)
        {
            log(log_level::fatal, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// logs message if level is high enough to pass the filter level. flushes if level passes
        /// flush filter level.
        ///
        /// @param[in] lvl log_level to log at.
        /// @param[in] msg log message containing format of the msg.
        /// @param[in] args... arguments used with {msg} to construct the formatted message.
        /// ----------------------------------------------------------------------------------------
        template <rlog_arg... arg_types>
        auto log(log_level lvl, log_str<arg_types...> msg, arg_types&&... args)
        {
            if (check_log_level(lvl))
            {
                string formatted_msg = string::format(msg, fwd(args)...);
                log_msg log_msg{
                    .msg = formatted_msg,
                    .logger_name = name(),
                    .lvl = lvl,
                    .time = time::now(),
                };

                log(log_msg);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// pure virtual function.
        ///
        /// logs the log_msg object.
        /// ----------------------------------------------------------------------------------------
        virtual auto log(log_msg& log_msg) -> void = 0;

        /// ----------------------------------------------------------------------------------------
        /// pure virtual function.
        ///
        /// flushes the logs of this logger.
        /// ----------------------------------------------------------------------------------------
        virtual auto flush() -> void = 0;

        /// ----------------------------------------------------------------------------------------
        /// pure virtual function.
        ///
        /// check if the message should be passed for logging.
        /// ----------------------------------------------------------------------------------------
        virtual auto check_log_level(log_level lvl) const -> bool = 0;
    };

    /// --------------------------------------------------------------------------------------------
    /// ptr type used by this logging api to manage a logger. it's recommended to use this
    /// type to store logger instance. this keeps compatibility with this logging api.
    /// --------------------------------------------------------------------------------------------
    using logger_ptr = shared_ptr<logger>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename tlogger, typename... arg_types>
    logger_ptr make_logger(arg_types&&... args)
        requires rderived_from<tlogger, logger>
    {
        return make_shared<tlogger>(forward<arg_types>(args)...);
    }
}
