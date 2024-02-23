export module atom.logging:logger_factory;
import :logger;
import :simple_logger;
import atom.core;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// this type is responsible for abstracting logger creation default logic.
    /// --------------------------------------------------------------------------------------------
    export class logger_factory
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// sets new instance.
        /// ----------------------------------------------------------------------------------------
        static auto set_instance(logger_factory* instance) -> void
        {
            _instance = instance;
        }

        /// ----------------------------------------------------------------------------------------
        /// gets current instance. by default it is set to `logger_factory` instance.
        /// ----------------------------------------------------------------------------------------
        static auto get_instance() -> logger_factory*
        {
            return _instance;
        }

    private:
        static logger_factory* _instance;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        logger_factory() {}

        /// ----------------------------------------------------------------------------------------
        /// # virtual destructor
        /// ----------------------------------------------------------------------------------------
        virtual ~logger_factory() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// creates `simple_logger_st` with `name`.
        /// ----------------------------------------------------------------------------------------
        virtual auto create_logger(string_view name) -> logger*
        {
            return new simple_logger_st(name);
        }
    };

    logger_factory* logger_factory::_instance = new logger_factory();
}
