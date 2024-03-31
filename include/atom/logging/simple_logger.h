#pragma once
#include "atom/logging/logger.h"
#include "atom/logging/log_target.h"
#include "atom/logging/log_msg.h"

#include <initializer_list>

namespace atom::logging
{
    template <bool st>
    class _simple_logger_impl
    {
        using this_type = _simple_logger_impl;
        using atomic_log_level_type = typeutils::conditional_t<st, log_level, atomic<log_level>>;
        using lockable_type = typeutils::conditional_t<st, null_lockable, simple_mutex>;

    public:
        explicit _simple_logger_impl(string name)
            : _name(move(name))
            , _targets()
            , _log_level(log_level::info)
            , _flush_level(log_level::info)
        {}

        template <typename range_type>
        _simple_logger_impl(string name, range_type&& targets)
            requires(is_range_of<typename typeinfo<range_type>::pure_t::value_t, log_target*>)
            : _name(move(name))
            , _targets(forward<range_type>(targets))
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

            for (log_target* target : _targets)
            {
                target->write(log_msg);
            }

            if (check_flush_level(log_msg.lvl))
            {
                for (log_target* target : _targets)
                {
                    target->flush();
                }
            }
        }

        auto flush() -> void
        {
            lock_guard guard(_lock);

            for (log_target* target : _targets)
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

        auto add_target(log_target* target) -> void
        {
            lock_guard guard(_lock);
            _targets.emplace_back(target);
        }

        template <typename range_type>
        auto add_targets(range_type&& targets) -> void
            requires is_range_of<typename typeinfo<range_type>::pure_t::value_t, log_target*>
        {
            lock_guard guard(_lock);
            _targets.insert_range_back(forward<range_type>(targets));
        }

        auto remove_target(log_target* target) -> void
        {
            lock_guard guard(_lock);
            _targets.remove_one_find(target);
        }

        auto has_target(log_target* target) const -> bool
        {
            lock_guard guard(_lock);
            for (log_target* value : _targets)
            {
                if (value == target)
                    return true;
            }

            return false;
        }

        auto get_targets() const -> array_view<log_target*>
        {
            return _targets;
        }

    private:
        const string _name;
        atomic_log_level_type _log_level;
        atomic_log_level_type _flush_level;
        dynamic_array<log_target*> _targets;
        lockable_type _lock;
    };

    using _simple_logger_impl_st = _simple_logger_impl<true>;
    using _simple_logger_impl_mt = _simple_logger_impl<false>;

    /// --------------------------------------------------------------------------------------------
    /// basic logger type.
    /// --------------------------------------------------------------------------------------------
    template <typename impl_type>
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
        /// constructs and adds `log_target` objects.
        ///
        /// @param[in] name name of this logger.
        /// @param[in] targets `log_target` objects to add.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        simple_logger(string name, range_type&& targets)
            requires(is_range_of<typename typeinfo<range_type>::pure_t::value_t, log_target*>)
            : _impl(move(name), forward<range_type>(targets))
        {}

        /// ----------------------------------------------------------------------------------------
        /// constructs and adds `log_target` objects.
        ///
        /// @param[in] name name of this logger.
        /// @param[in] targets `log_target` objects to add.
        /// ----------------------------------------------------------------------------------------
        simple_logger(string name, initializer_list<log_target*> targets)
            : _impl(move(name), range_from(targets))
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
        /// writes to all log_target objects.
        /// ----------------------------------------------------------------------------------------
        auto log(log_msg& log_msg) -> void override
        {
            _impl.log(log_msg);
        }

        /// ----------------------------------------------------------------------------------------
        /// flushes all log_target objects.
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
        auto add_target(log_target* target) -> void
        {
            return _impl.add_target(target);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        auto add_targets(range_type&& targets) -> void
            requires is_range_of<typename typeinfo<range_type>::pure_t::value_t, log_target*>
        {
            return _impl.add_targets(forward<range_type>(targets));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto add_targets(std::initializer_list<log_target*> targets) -> void
        {
            return _impl.add_targets(range_from(targets));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto remove_target(log_target* target) -> void
        {
            return _impl.remove_target(target);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto get_targets() const -> array_view<log_target*>
        {
            return _impl.get_targets();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto has_target(log_target* target) const -> bool
        {
            return _impl.has_target(target);
        }

    private:
        impl_type _impl;
    };

    using simple_logger_st = simple_logger<_simple_logger_impl_st>;
    using simple_logger_mt = simple_logger<_simple_logger_impl_mt>;
}
