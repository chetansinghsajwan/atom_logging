export module atom_logging:simple_logger;

import std;
import atom_core;
import :logger;
import :log_typearget;
import :log_msg;

namespace atom::logging
{
    template <bool st>
    class _simple_logger_impl
    {
        using this_type = _simple_logger_impl;
        using atomic_log_level_type =
            type_utils::conditional_type<st, log_level, atomic<log_level>>;
        using lockable_type = type_utils::conditional_type<st, null_lockable, simple_mutex>;

    public:
        explicit _simple_logger_impl(string name)
            : _name{ move(name) }
            , _typeargets{}
            , _log_level{ log_level::info }
            , _flush_level{ log_level::info }
        {}

        template <typename range_type>
        _simple_logger_impl(string name, range_type&& targets)
            requires(ranges::const_range_concept<
                        typename type_info<range_type>::pure_type::value_type, log_typearget*>)
            : _name(move(name))
            , _typeargets(forward<range_type>(targets))
        {}

        _simple_logger_impl(const this_type& that) = delete;
        _simple_logger_impl& operator=(const this_type&& that) = delete;

        _simple_logger_impl(this_type&& that) = delete;
        _simple_logger_impl& operator=(this_type&& that) = delete;

        ~_simple_logger_impl() {}

    public:
        auto get_name() const -> string_view
        {
            return _name;
        }

        auto log(log_msg& log_msg) -> void
        {
            lock_guard guard(_lock);

            for (log_typearget* target : _typeargets)
            {
                target->write(log_msg);
            }

            if (check_flush_level(log_msg.lvl))
            {
                for (log_typearget* target : _typeargets)
                {
                    target->flush();
                }
            }
        }

        auto flush() -> void
        {
            lock_guard guard(_lock);

            for (log_typearget* target : _typeargets)
            {
                target->flush();
            }
        }

        auto set_log_level(log_level lvl)
        {
            _log_level = lvl;
        }

        auto get_log_level(log_level lvl) -> log_level
        {
            return _log_level;
        }

        auto check_log_level(log_level lvl) const -> bool
        {
            return lvl != log_level::off && lvl >= _log_level;
        }

        auto set_flush_level(log_level lvl)
        {
            _flush_level = lvl;
        }

        auto get_flush_level() const -> log_level
        {
            return _flush_level;
        }

        auto check_flush_level(log_level lvl) const -> bool
        {
            return lvl != log_level::off && lvl >= _flush_level;
        }

        auto add_typearget(log_typearget* target) -> void
        {
            lock_guard guard(_lock);
            _typeargets.emplace_last(target);
        }

        template <typename range_type>
        auto add_typeargets(range_type&& targets) -> void
            requires ranges::const_range_concept<
                typename type_info<range_type>::pure_type::value_type, log_typearget*>
        {
            lock_guard guard(_lock);
            _typeargets.insert_range_last(std::forward<range_type>(targets));
        }

        auto remove_typearget(log_typearget* target) -> void
        {
            lock_guard guard(_lock);
            _typeargets.remove_one_find(target);
        }

        auto has_typearget(log_typearget* target) const -> bool
        {
            lock_guard guard(_lock);
            for (log_typearget* value : _typeargets)
            {
                if (value == target)
                    return true;
            }

            return false;
        }

        auto get_typeargets() const -> array_view<log_typearget*>
        {
            return _typeargets;
        }

    private:
        const string _name;
        atomic_log_level_type _log_level;
        atomic_log_level_type _flush_level;
        dynamic_array<log_typearget*> _typeargets;
        lockable_type _lock;
    };

    using _simple_logger_impl_st = _simple_logger_impl<true>;
    using _simple_logger_impl_mt = _simple_logger_impl<false>;

    /// --------------------------------------------------------------------------------------------
    /// basic logger type.
    /// --------------------------------------------------------------------------------------------
    export template <typename impl_type>
    class simple_logger: public logger
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor.
        ///
        /// @param[in] name name of this logger.
        /// ----------------------------------------------------------------------------------------
        explicit simple_logger(string name)
            : _impl(move(name))
        {}

        /// ----------------------------------------------------------------------------------------
        /// constructs and adds `log_typearget` objects.
        ///
        /// @param[in] name name of this logger.
        /// @param[in] targets `log_typearget` objects to add.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        simple_logger(string name, range_type&& targets)
            requires(
                ranges::const_range_concept<typename type_info<range_type>::pure_type::value_type,
                    log_typearget*>)
            : _impl(move(name), forward<range_type>(targets))
        {}

        /// ----------------------------------------------------------------------------------------
        /// constructs and adds `log_typearget` objects.
        ///
        /// @param[in] name name of this logger.
        /// @param[in] targets `log_typearget` objects to add.
        /// ----------------------------------------------------------------------------------------
        simple_logger(string name, initializer_list<log_typearget*> targets)
            : _impl{ move(name), targets }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        ~simple_logger() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// get the name of this logger.
        /// ----------------------------------------------------------------------------------------
        auto get_name() const -> string_view override final
        {
            return _impl.get_name();
        }

        /// ----------------------------------------------------------------------------------------
        /// writes to all log_typearget objects.
        /// ----------------------------------------------------------------------------------------
        auto log(log_msg& log_msg) -> void override
        {
            _impl.log(log_msg);
        }

        /// ----------------------------------------------------------------------------------------
        /// flushes all log_typearget objects.
        /// ----------------------------------------------------------------------------------------
        auto flush() -> void override
        {
            _impl.flush();
        }

        /// ----------------------------------------------------------------------------------------
        /// sets log level.
        /// ----------------------------------------------------------------------------------------
        auto set_log_level(log_level lvl)
        {
            _impl.set_log_level(lvl);
        }

        /// ----------------------------------------------------------------------------------------
        /// gets log level.
        /// ----------------------------------------------------------------------------------------
        auto get_log_level(log_level lvl) -> log_level
        {
            return _impl.get_log_level();
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if `lvl` is high enough to pass log level filter.
        /// ----------------------------------------------------------------------------------------
        auto check_log_level(log_level lvl) const -> bool override final
        {
            return _impl.check_log_level(lvl);
        }

        /// ----------------------------------------------------------------------------------------
        /// sets flush level.
        /// ----------------------------------------------------------------------------------------
        auto set_flush_level(log_level lvl)
        {
            _impl.set_flush_level(lvl);
        }

        /// ----------------------------------------------------------------------------------------
        /// gets flush level.
        /// ----------------------------------------------------------------------------------------
        auto get_flush_level() const -> log_level
        {
            return _impl.get_flush_level();
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if `lvl` is high enough to pass flush level filter.
        /// ----------------------------------------------------------------------------------------
        auto check_flush_level(log_level lvl) const -> bool
        {
            return _impl.check_flush_level(lvl);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto add_typearget(log_typearget* target) -> void
        {
            return _impl.add_typearget(target);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        auto add_typeargets(range_type&& targets) -> void
            requires ranges::const_range_concept<
                typename type_info<range_type>::pure_type::value_type, log_typearget*>
        {
            return _impl.add_typeargets(forward<range_type>(targets));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto add_typeargets(std::initializer_list<log_typearget*> targets) -> void
        {
            return _impl.add_typeargets(targets);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto remove_typearget(log_typearget* target) -> void
        {
            return _impl.remove_typearget(target);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto get_typeargets() const -> array_view<log_typearget*>
        {
            return _impl.get_typeargets();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto has_typearget(log_typearget* target) const -> bool
        {
            return _impl.has_typearget(target);
        }

    private:
        impl_type _impl;
    };

    export using simple_logger_st = simple_logger<_simple_logger_impl_st>;
    export using simple_logger_mt = simple_logger<_simple_logger_impl_mt>;
}
