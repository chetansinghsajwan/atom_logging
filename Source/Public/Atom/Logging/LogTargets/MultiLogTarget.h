#pragma once
#include "Atom/CoreAll.h"

namespace Atom::Logging::Private
{
    /// --------------------------------------------------------------------------------------------
    /// --- DOC_TEMPLATE
    /// --------------------------------------------------------------------------------------------
    /// MultiLogTargetTemplate is used to store multiple LogTarget objects.
    ///
    /// @TPARAM TLockable Type used to ensure thread safety.
    ///
    /// @REQUIRES TLockable RLockable<TLockable>.
    ///
    /// @THREAD_SAFETY SAFE
    /// --------------------------------------------------------------------------------------------
    template <typename TLockable>
        requires RLockable<TLockable>
    class MultiLogTargetTemplate: public LogTarget
    {
        using _TContainer = DynArr<LogTargetPtr>;

    public:
        using TElem = LogTargetPtr;
        using TIter = typename _TContainer::TIter;
        using TIterEnd = typename _TContainer::TIterEnd;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Default constructor.
        /// ----------------------------------------------------------------------------------------
        MultiLogTargetTemplate() {}

        /// ----------------------------------------------------------------------------------------
        /// Constructs with LogTarget objects.
        ///
        /// @PARAM[IN] target RRange of LogTarget objects to add.
        ///     If {targets} contains null objects, this doesn't adds them.
        ///
        /// @TIME_COMPLEXITY @COPY_FROM _AddTargets(range).
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        MultiLogTargetTemplate(const TRange& targets)
            requires(RRangeOf<TRange, LogTargetPtr>)
        {
            _AddTargets(mov(targets));
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// LogTarget Interface
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        /// Writes to all stored LogTarget objects.
        /// ----------------------------------------------------------------------------------------
        virtual auto Write(const LogMsg& logMsg) -> void override final
        {
            LockGuard guard(_lock);
            for (auto& target : _targets)
            {
                target->Write(logMsg);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Flushes all stored LogTarget objects.
        /// ----------------------------------------------------------------------------------------
        virtual auto Flush() -> void override final
        {
            LockGuard guard(_lock);
            for (auto& target : _targets)
            {
                target->Flush();
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// LogTarget manipulation functions
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        /// Adds LogTarget object.
        ///
        /// @PARAM[IN] target {LogTarget} object to add.
        ///     If {target} is null, this doesn't adds it.
        /// @RETURNS `true` if added, else `false`.
        ///
        /// @TIME_COMPLEXITY @COPY_FROM _AddTarget(LogTargetPtr target).
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        auto AddTarget(LogTargetPtr target) -> bool
        {
            if (target == nullptr)
                return false;

            LockGuard guard(_lock);
            _AddTarget(target);
        }

        /// ----------------------------------------------------------------------------------------
        /// Adds LogTarget objects.
        ///
        /// @PARAM[IN] targets LogTarget objects to add.
        ///     If {targets} contains null objects, this doesn't adds them.
        /// @RETURNS Count of LogTarget objects added.
        ///
        /// @TIME_COMPLEXITY @COPY_FROM _AddTargets(range).
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
            requires RRangeOf<TRange, LogTargetPtr>
        usize AddTargets(const TRange& targets)
        {
            if (targets.iter().equals(targets.iterEnd()))
                return 0;

            LockGuard guard(_lock);
            _AddTargets(targets);
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes LogTarget object if found.
        ///
        /// @PARAM[IN] target LogTarget object to remove.
        ///     If {target} is null, this doesn't searches it.
        /// @RETURNS `true` if found and removed, else `false`.
        ///
        /// @TIME_COMPLEXITY @COPY_FROM _RemoveTarget(LogTargetPtr target)
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        auto RemoveTarget(LogTargetPtr target) -> bool
        {
            if (target == nullptr)
                return false;

            LockGuard guard(_lock);
            return _RemoveTarget(target);
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes LogTarget objects if found.
        ///
        /// @PARAM[IN] targets LogTarget objects to remove.
        ///     If {targets} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects removed.
        ///
        /// @TIME_COMPLEXITY @COPY_FROM _RemoveTargets(const TRange& range)
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
            requires RRangeOf<TRange, LogTargetPtr>
        auto RemoveTargets(const TRange& targets) -> usize
        {
            if (targets.iter().equals(targets.iterEnd()))
                return 0;

            LockGuard guard(_lock);
            return _RemoveTargets(targets);
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if this has LogTarget object.
        ///
        /// @PARAM[IN] target LogTarget object to search for.
        ///     If {target} is null, this doesn't searches it.
        /// @RETURNS `true` if found, else `false`.
        ///
        /// @TIME_COMPLEXITY @COPY_FROM _HasTarget(LogTargetPtr target)
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        auto HasTarget(LogTargetPtr target) const -> bool
        {
            if (target == nullptr)
                return false;

            LockGuard guard(_lock);
            return _HasTarget(target);
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if this has LogTarget objects.
        ///
        /// @PARAM[IN] target LogTarget objects to search for.
        ///     If {targets} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects found.
        ///
        /// @TIME_COMPLEXITY @COPY_FROM _HasTarget(LogTargetPtr target)
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
            requires RRangeOf<TRange, LogTargetPtr>
        auto HasTargets(const TRange& targets) const -> usize
        {
            if (targets.iter().equals(targets.iterEnd()))
                return 0;

            LockGuard guard(_lock);
            return _HasTargets(targets);
        }

        /// ----------------------------------------------------------------------------------------
        /// Reserves memory for {capacity} LogTarget objects.
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        auto Reserve(usize capacity)
        {
            LockGuard guard(_lock);
            _Reserve(capacity);
        }

        /// ----------------------------------------------------------------------------------------
        /// Count of LogTarget objects this has.
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        auto count() const -> usize
        {
            LockGuard guard(_lock);
            return _targets.count();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        /// Iterator to the first LogTarget.
        ///
        /// @THREAD_SAFETY NONE
        ///
        /// @TODO Make ThreadSafe.
        /// ----------------------------------------------------------------------------------------
        auto iter() const -> TIter
        {
            return _targets.iter();
        }

        /// ----------------------------------------------------------------------------------------
        /// Iterator to the last LogTarget.
        ///
        /// @THREAD_SAFETY NONE
        ///
        /// @TODO Make ThreadSafe.
        /// ----------------------------------------------------------------------------------------
        auto iterEnd() const -> TIterEnd
        {
            return _targets.iterEnd();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Implementation Functions
        ////
        //// These functions doesn't perform any checks at runtime and assumes that all thread safety
        //// steps have been taken.
        //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Adds the LogTarget object.
        ///
        /// @PARAM[IN] target {LogTarget} object to add.
        ///     If {target} is null, this doesn't adds it.
        ///
        /// @TIME_COMPLEXITY @COPY_FROM ${_TContainer}::emplaceBack(LogTarget& target)
        ///
        /// @THREAD_SAFETY NONE
        /// ----------------------------------------------------------------------------------------
        auto _AddTarget(LogTargetPtr target) -> bool
        {
            debug_expects(target != nullptr);

            _targets.emplaceBack(mov(target));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Adds the LogTarget objects.
        ///
        /// @PARAM[IN] target {LogTarget} object to add.
        ///     If {targets} contains null objects, this doesn't adds them.
        /// @RETURNS Count of LogTarget objects added.
        ///
        /// @TIME_COMPLEXITY @COPY_FROM ${_TContainer}::emplaceBack(RRangeOf<LogTargetPtr> targets)
        ///
        /// @THREAD_SAFETY NONE
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
            requires RRangeOf<TRange, LogTargetPtr>
        auto _AddTargets(const TRange& targets) -> usize
        {
            usize count = 0;
            for (LogTargetPtr target : targets)
            {
                if (target != nullptr)
                {
                    _targets.emplaceBack(mov(target));
                    count++;
                }
            }

            return count;
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes the LogTarget object.
        ///
        /// @PARAM[IN] target LogTarget object to remove.
        ///     If {target} is null, this doesn't searches it.
        /// @RETURNS `true` if found and removed, else `false`.
        ///
        /// @TIME_COMPLEXITY @COPY_FROM ${_TContainer}::Remove(LogTarget& target)
        /// ----------------------------------------------------------------------------------------
        auto _RemoveTarget(LogTargetPtr target) -> bool
        {
            debug_expects(target != nullptr);

            for (auto it = _targets.iter(); not it.equals(_targets.iterEnd()); it.next())
            {
                if (it.value() == target)
                {
                    _targets.removeAt(it);
                    return true;
                }
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes LogTarget objects.
        ///
        /// @PARAM[IN] it RRange to beginning of the range to remove.
        /// @PARAM[IN] end RRange to end of the range to remove.
        ///     If range {[it, end]} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects removed.
        ///
        /// @TIME_COMPLEXITY @COPY_FROM ${_TContainer}::Remove(LogTarget& target)
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
            requires RRangeOf<TRange, LogTargetPtr>
        auto _RemoveTargets(const TRange& targets) -> usize
        {
            usize count = 0;
            for (auto target : targets)
            {
                if (target == nullptr)
                    continue;

                for (auto it = _targets.iter(); it.equals(_targets.iterEnd()); it.next())
                {
                    if (it.value() == target)
                    {
                        _targets.removeAt(it);
                        count++;
                        break;
                    }
                }
            }

            return count;
        }

        /// ----------------------------------------------------------------------------------------
        /// Searches for LogTarget object.
        ///
        /// @PARAM[IN] target LogTarget object to search for.
        ///     If {target} is null, doesn't searches it.
        /// @RETURNS `true` if found, else `false`.
        ///
        /// @TIME_COMPLEXITY Linear
        /// ----------------------------------------------------------------------------------------
        auto _HasTarget(const LogTargetPtr& target) const -> bool
        {
            debug_expects(target != nullptr);

            return _targets.contains(target);
        }

        /// ----------------------------------------------------------------------------------------
        /// Search LogTarget objects.
        ///
        /// @PARAM[IN] it RRange to beginning of range to search for.
        /// @PARAM[IN] end RRange to end of range to search for.
        ///     If range {[it, end]} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects found.
        ///
        /// @TIME_COMPLEXITY Exponential
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
            requires RRangeOf<TRange, LogTargetPtr>
        auto _HasTargets(const TRange& targets) -> usize
        {
            return _targets.countAny(targets);
        }

        /// ----------------------------------------------------------------------------------------
        /// Reserves memory for {capacity} LogTarget objects.
        /// ----------------------------------------------------------------------------------------
        auto _Reserve(usize capacity)
        {
            _targets.reserve(capacity);
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// _TContainer to store LogTarget objects.
        /// ----------------------------------------------------------------------------------------
        _TContainer _targets;

        /// ----------------------------------------------------------------------------------------
        /// Lockable object for thread safety.
        /// ----------------------------------------------------------------------------------------
        TLockable _lock;
    };
}

namespace Atom::Logging
{
    using MultiLogTargetST = Private::MultiLogTargetTemplate<NullLockable>;
    using MultiLogTargetMT = Private::MultiLogTargetTemplate<SimpleMutex>;

    static_assert(RFwdRangeOf<MultiLogTargetST, LogTargetPtr>,
        "{MultiLogTargetST} does not meet {RFwdRange} requirements.");

    static_assert(RFwdRangeOf<MultiLogTargetMT, LogTargetPtr>,
        "{MultiLogTargetMT} does not meet {RFwdRange} requirements.");
}
