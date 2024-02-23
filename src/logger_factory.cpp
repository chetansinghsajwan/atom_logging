export module atom.logging:logger_factory;
import :logger;
import :simple_logger;
import atom.core;

namespace atom::logging
{
    export class logger_factory
    {
    public:
        auto create_logger(string name) -> logger*
        {
            return make_logger<simple_logger_st>(move(name));
        }
    };

    export inline auto get_logger_factory() -> logger_factory&
    {
        static logger_factory instance;
        return instance;
    }

    export inline auto create_logger(auto&&... args) -> logger_ptr
    {
        return get_logger_factory().create_logger(fwd(args)...);
    }
}
