export module atom.logging:multi_log_target;
import :log_target;
import :log_msg;
import atom.core;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// --- doc_template
    /// --------------------------------------------------------------------------------------------
    /// _multi_log_target_template is used to store multiple log_target objects.
    ///
    /// @tparam tlockable type used to ensure thread safety.
    ///
    /// @requires tlockable rlockable<tlockable>.
    ///
    /// @thread_safety safe
    /// --------------------------------------------------------------------------------------------
    template <typename tlockable>
    class _multi_log_target_template: public log_target
    {
        static_assert(rlockable<tlockable>);

    private:
        using _container_type = dynamic_array<log_target_ptr>;

    public:
	using elem_type = log_target_ptr;
        using iter_type = typename _container_type::iter_type;
        using iter_end_type = typename _container_type::iter_end_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// default constructor.
        /// ----------------------------------------------------------------------------------------
        _multi_log_target_template() {}

        /// ----------------------------------------------------------------------------------------
        /// constructs with log_target objects.
        ///
        /// @param[in] target rrange of log_target objects to add.
        ///     if {targets} contains null objects, this doesn't adds them.
        ///
        /// @time_complexity @copy_from _add_targets(range).
        /// ----------------------------------------------------------------------------------------
        template <typename trange>
        _multi_log_target_template(const trange& targets)
            requires(rrange_of<trange, log_target_ptr>)
        {
            _add_targets(mov(targets));
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// log_target interface
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        /// writes to all stored log_target objects.
        /// ----------------------------------------------------------------------------------------
        virtual auto write(const log_msg& log_msg) -> void override final
        {
            lock_guard guard(_lock);
            for (auto& target : _targets)
            {
                target->write(log_msg);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// flushes all stored log_target objects.
        /// ----------------------------------------------------------------------------------------
        virtual auto flush() -> void override final
        {
            lock_guard guard(_lock);
            for (auto& target : _targets)
            {
                target->flush();
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// log_target manipulation functions
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        /// adds log_target object.
        ///
        /// @param[in] target {log_target} object to add.
        ///     if {target} is null, this doesn't adds it.
        /// @returns `true` if added, else `false`.
        ///
        /// @time_complexity @copy_from _add_target(log_target_ptr target).
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        auto add_target(log_target_ptr target) -> bool
        {
            if (target == nullptr)
                return false;

            lock_guard guard(_lock);
            _add_target(target);
        }

        /// ----------------------------------------------------------------------------------------
        /// adds log_target objects.
        ///
        /// @param[in] targets log_target objects to add.
        ///     if {targets} contains null objects, this doesn't adds them.
        /// @returns count of log_target objects added.
        ///
        /// @time_complexity @copy_from _add_targets(range).
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        template <typename trange>
            requires rrange_of<trange, log_target_ptr>
        usize add_targets(const trange& targets)
        {
            if (targets.iter().equals(targets.iter_end()))
                return 0;

            lock_guard guard(_lock);
            _add_targets(targets);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes log_target object if found.
        ///
        /// @param[in] target log_target object to remove.
        ///     if {target} is null, this doesn't searches it.
        /// @returns `true` if found and removed, else `false`.
        ///
        /// @time_complexity @copy_from _remove_target(log_target_ptr target)
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        auto remove_target(log_target_ptr target) -> bool
        {
            if (target == nullptr)
                return false;

            lock_guard guard(_lock);
            return _remove_target(target);
        }

        /// ----------------------------------------------------------------------------------------
        /// removes log_target objects if found.
        ///
        /// @param[in] targets log_target objects to remove.
        ///     if {targets} contains null objects, this doesn't searches them.
        /// @returns count of log_target objects removed.
        ///
        /// @time_complexity @copy_from _remove_targets(const trange& range)
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        template <typename trange>
            requires rrange_of<trange, log_target_ptr>
        auto remove_targets(const trange& targets) -> usize
        {
            if (targets.iter().equals(targets.iter_end()))
                return 0;

            lock_guard guard(_lock);
            return _remove_targets(targets);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if this has log_target object.
        ///
        /// @param[in] target log_target object to search for.
        ///     if {target} is null, this doesn't searches it.
        /// @returns `true` if found, else `false`.
        ///
        /// @time_complexity @copy_from _has_target(log_target_ptr target)
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        auto has_target(log_target_ptr target) const -> bool
        {
            if (target == nullptr)
                return false;

            lock_guard guard(_lock);
            return _has_target(target);
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if this has log_target objects.
        ///
        /// @param[in] target log_target objects to search for.
        ///     if {targets} contains null objects, this doesn't searches them.
        /// @returns count of log_target objects found.
        ///
        /// @time_complexity @copy_from _has_target(log_target_ptr target)
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        template <typename trange>
            requires rrange_of<trange, log_target_ptr>
        auto has_targets(const trange& targets) const -> usize
        {
            if (targets.iter().equals(targets.iter_end()))
                return 0;

            lock_guard guard(_lock);
            return _has_targets(targets);
        }

        /// ----------------------------------------------------------------------------------------
        /// reserves memory for {capacity} log_target objects.
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        auto reserve(usize capacity)
        {
            lock_guard guard(_lock);
            _reserve(capacity);
        }

        /// ----------------------------------------------------------------------------------------
        /// count of log_target objects this has.
        ///
        /// @thread_safety safe
        /// ----------------------------------------------------------------------------------------
        auto count() const -> usize
        {
            lock_guard guard(_lock);
            return _targets.count();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        /// iterator to the first log_target.
        ///
        /// @thread_safety none
        ///
        /// # to do
        ///
        /// - make thread_safe.
        /// ----------------------------------------------------------------------------------------
        auto iter() const -> iter_type
        {
            return _targets.iter();
        }

        /// ----------------------------------------------------------------------------------------
        /// iterator to the last log_target.
        ///
        /// @thread_safety none
        ///
        /// # to do
        ///
        /// - make thread_safe.
        /// ----------------------------------------------------------------------------------------
        auto iter_end() const -> iter_end_type
        {
            return _targets.iter_end();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// implementation functions
        ////
        //// these functions doesn't perform any checks at runtime and assumes that all thread 
        //// safety steps have been taken.
        //// 
        ////////////////////////////////////////////////////////////////////////////////////////////

    protected:
        /// ----------------------------------------------------------------------------------------
        /// adds the log_target object.
        ///
        /// @param[in] target {log_target} object to add.
        ///     if {target} is null, this doesn't adds it.
        ///
        /// @time_complexity @copy_from ${_container_type}::emplace_back(log_target& target)
        ///
        /// @thread_safety none
        /// ----------------------------------------------------------------------------------------
        auto _add_target(log_target_ptr target) -> bool
        {
            contracts::debug_expects(target != nullptr);

            _targets.emplace_back(mov(target));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// adds the log_target objects.
        ///
        /// @param[in] target {log_target} object to add.
        ///     if {targets} contains null objects, this doesn't adds them.
        /// @returns count of log_target objects added.
        ///
        /// @time_complexity @copy_from ${_container_type}::emplace_back(rrange_of<log_target_ptr> targets)
        ///
        /// @thread_safety none
        /// ----------------------------------------------------------------------------------------
        template <typename trange>
            requires rrange_of<trange, log_target_ptr>
        auto _add_targets(const trange& targets) -> usize
        {
            usize count = 0;
            for (log_target_ptr target : targets)
            {
                if (target != nullptr)
                {
                    _targets.emplace_back(mov(target));
                    count++;
                }
            }

            return count;
        }

        /// ----------------------------------------------------------------------------------------
        /// removes the log_target object.
        ///
        /// @param[in] target log_target object to remove.
        ///     if {target} is null, this doesn't searches it.
        /// @returns `true` if found and removed, else `false`.
        ///
        /// @time_complexity @copy_from ${_container_type}::remove(log_target& target)
        /// ----------------------------------------------------------------------------------------
        auto _remove_target(log_target_ptr target) -> bool
        {
            contracts::debug_expects(target != nullptr);

            for (auto it = _targets.iter(); not it.is_eq(_targets.iter_end()); it.next())
            {
                if (it.value() == target)
                {
                    _targets.remove_at(it);
                    return true;
                }
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        /// removes log_target objects.
        ///
        /// @param[in] it rrange to beginning of the range to remove.
        /// @param[in] end rrange to end of the range to remove.
        ///     if range {[it, end]} contains null objects, this doesn't searches them.
        /// @returns count of log_target objects removed.
        ///
        /// @time_complexity @copy_from ${_container_type}::remove(log_target& target)
        /// ----------------------------------------------------------------------------------------
        template <typename trange>
            requires rrange_of<trange, log_target_ptr>
        auto _remove_targets(const trange& targets) -> usize
        {
            usize count = 0;
            for (auto target : targets)
            {
                if (target == nullptr)
                    continue;

                for (auto it = _targets.iter(); it.is_eq(_targets.iter_end()); it.next())
                {
                    if (it.value() == target)
                    {
                        _targets.remove_at(it);
                        count++;
                        break;
                    }
                }
            }

            return count;
        }

        /// ----------------------------------------------------------------------------------------
        /// searches for log_target object.
        ///
        /// @param[in] target log_target object to search for.
        ///     if {target} is null, doesn't searches it.
        /// @returns `true` if found, else `false`.
        ///
        /// @time_complexity linear
        /// ----------------------------------------------------------------------------------------
        auto _has_target(const log_target_ptr& target) const -> bool
        {
            contracts::debug_expects(target != nullptr);

            return _targets.contains(target);
        }

        /// ----------------------------------------------------------------------------------------
        /// search log_target objects.
        ///
        /// @param[in] it rrange to beginning of range to search for.
        /// @param[in] end rrange to end of range to search for.
        ///     if range {[it, end]} contains null objects, this doesn't searches them.
        /// @returns count of log_target objects found.
        ///
        /// @time_complexity exponential
        /// ----------------------------------------------------------------------------------------
        template <typename trange>
            requires rrange_of<trange, log_target_ptr>
        auto _has_targets(const trange& targets) -> usize
        {
            return _targets.count_any(targets);
        }

        /// ----------------------------------------------------------------------------------------
        /// reserves memory for {capacity} log_target objects.
        /// ----------------------------------------------------------------------------------------
        auto _reserve(usize capacity)
        {
            _targets.reserve(capacity);
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// _container_type to store log_target objects.
        /// ----------------------------------------------------------------------------------------
        _container_type _targets;

        /// ----------------------------------------------------------------------------------------
        /// lockable object for thread safety.
        /// ----------------------------------------------------------------------------------------
        tlockable _lock;
    };

    export using multi_log_target_st = _multi_log_target_template<null_lockable>;
    export using multi_log_target_mt = _multi_log_target_template<simple_mutex>;
}
