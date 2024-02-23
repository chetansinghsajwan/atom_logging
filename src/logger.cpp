export module atom.logging:logger;
import :log_msg;
import :log_target;
import atom.core;

namespace atom::logging
{
    export class logger
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        logger() {}

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        virtual ~logger() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// returns the name of the logger.
        /// ----------------------------------------------------------------------------------------
        virtual auto get_name() const -> string_view = 0;

        /// ----------------------------------------------------------------------------------------
        /// calls `log(log_level::trace, msg, fwd(args)...)`.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        auto log_trace(format_string<arg_types...> msg, arg_types&&... args)
        {
            log(log_level::trace, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls `log(log_level::debug, msg, fwd(args)...)`.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        auto log_debug(format_string<arg_types...> msg, arg_types&&... args)
        {
            log(log_level::debug, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls `log(log_level::info, msg, fwd(args)...)`.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        auto log_info(format_string<arg_types...> msg, arg_types&&... args)
        {
            log(log_level::info, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls `log(log_level::warn, msg, fwd(args)...)`.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        auto log_warn(format_string<arg_types...> msg, arg_types&&... args)
        {
            log(log_level::warn, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls `log(log_level::error, msg, fwd(args)...)`.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        auto log_error(format_string<arg_types...> msg, arg_types&&... args)
        {
            log(log_level::error, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls `log(log_level::fatal, msg, fwd(args)...)`.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        auto log_fatal(format_string<arg_types...> msg, arg_types&&... args)
        {
            log(log_level::fatal, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// logs message if level is high enough to pass the filter level and then flushes if
        /// level passes the flush filter level.
        ///
        /// # parameters
        ///
        /// - `lvl`: log_level to log at.
        /// - `msg`: log message containing format of the msg.
        /// - `args...`: arguments used with `msg` to construct the formatted message.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        auto log(log_level lvl, format_string<arg_types...> msg, arg_types&&... args)
        {
            if (check_log_level(lvl))
            {
                string formatted_msg = string::format(msg, fwd(args)...);
                log_msg log_msg{
                    .msg = formatted_msg,
                    .logger_name = get_name(),
                    .lvl = lvl,
                    .time = time::now(),
                };

                log(log_msg);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// logs the log_msg object.
        /// ----------------------------------------------------------------------------------------
        virtual auto log(log_msg& log_msg) -> void = 0;

        /// ----------------------------------------------------------------------------------------
        /// flushes the logs of this logger.
        /// ----------------------------------------------------------------------------------------
        virtual auto flush() -> void = 0;

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if the message should be passed for logging.
        /// ----------------------------------------------------------------------------------------
        virtual auto check_log_level(log_level lvl) const -> bool = 0;
    };
}
