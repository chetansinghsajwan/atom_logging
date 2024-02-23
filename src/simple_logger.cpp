export module atom.logging:simple_logger;
import :log_msg;
import :logger;
import :log_target;
import :multi_log_target;
import atom.core;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// --- doc_template
    /// --------------------------------------------------------------------------------------------
    /// basic logger object.
    ///
    /// @thread_safety safe
    /// --------------------------------------------------------------------------------------------
    template <bool st>
    class simple_logger_template: public logger
    {
        using multi_log_target =
            tti::conditional_type<st, multi_log_target_st, multi_log_target_mt>;
        using atomic_log_level = tti::conditional_type<st, log_level, atomic<log_level>>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// default_constructor.
        ///
        /// @param[in] name name of this logger.
        /// ----------------------------------------------------------------------------------------
        explicit simple_logger_template(string name)
            : _name(move(name))
            , targets()
        {}

        /// ----------------------------------------------------------------------------------------
        /// constructs and adds log_target objects.
        /// constructs {targets(targets)}.
        ///
        /// @param[in] name name of this logger.
        /// @param[in] targets log_target objects to add.
        /// ----------------------------------------------------------------------------------------
        template <typename trange>
        simple_logger_template(string name, const trange& targets)
            requires(rrange_of<trange, log_target_ptr>)
            : _name(move(name))
            , targets(targets)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// get the name of this logger.
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        auto name() const -> string_view override final
        {
            return _name;
        }

        /// ----------------------------------------------------------------------------------------
        /// writes to all log_target objects.
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        auto log(log_msg& log_msg) -> void override
        {
            targets.write(log_msg);
        }

        /// ----------------------------------------------------------------------------------------
        /// flushes all log_target objects.
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        auto flush() -> void override
        {
            targets.flush();
        }

        /// ----------------------------------------------------------------------------------------
        /// set log level.
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        auto set_log_level(log_level lvl)
        {
            _log_level = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// get log level.
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        auto get_log_level(log_level lvl) -> log_level
        {
            return _log_level;
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if log of level {lvl} should be logged.
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        auto check_log_level(log_level lvl) const -> bool override final
        {
            return lvl != log_level::off && lvl >= _log_level;
        }

        /// ----------------------------------------------------------------------------------------
        /// set flush level.
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        auto set_flush_level(log_level lvl)
        {
            _flush_level = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// get flush level.
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        auto get_flush_level() const -> log_level
        {
            return _flush_level;
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if log of level {lvl} should be flushed.
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        auto check_flush_level(log_level lvl) const -> bool
        {
            return lvl != log_level::off && lvl >= _flush_level;
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// the object used to manage log_target objects. accessing it is thread safe.
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        multi_log_target targets;

    protected:
        /// ----------------------------------------------------------------------------------------
        /// name of this logger.
        /// ----------------------------------------------------------------------------------------
        const string _name;

        /// ----------------------------------------------------------------------------------------
        /// log_level used to filter logs.
        /// ----------------------------------------------------------------------------------------
        atomic_log_level _log_level;

        /// ----------------------------------------------------------------------------------------
        /// log_level used to filter flush after logs.
        /// ----------------------------------------------------------------------------------------
        atomic_log_level _flush_level;
    };

    export using simple_logger_st = simple_logger_template<true>;
    export using simple_logger_mt = simple_logger_template<false>;
}
