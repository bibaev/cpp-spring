#include <vector>
#include <functional>

namespace utils {
    template<typename>
    struct event;

    template<class R, class... ARGSS>
    struct event<R(ARGSS...)> {
        using handler_t = std::function<R(ARGSS...)>;
        event() = default;
        event(event const& other)
            : handlers_(other.handlers_) {
            
        }

        event operator=(event const& other);
        event operator=(event && other);

        ~event() = default;
        void add(handler_t const & handler);
        void add(handler_t && handler);

        void fire(ARGSS...);
    private:
        std::vector<handler_t> handlers_;
    };

    template <class R, class ... ARGSS>
    event<R(ARGSS...)> event<R(ARGSS...)>::operator=(event const& other) {
        handlers_ = other.handlers_;
        return *this;
    }

    template <class R, class ... ARGSS>
    void event<R(ARGSS...)>::add(std::function<R(ARGSS...)> const& handler) {
        handlers_.push_back(handler);
    }

    template <class R, class ... ARGSS>
    void event<R(ARGSS...)>::add(std::function<R(ARGSS...)>&& handler) {
        handlers_.emplace_back(handler);
    }

    template <class R, class ... ARGSS>
    void event<R(ARGSS...)>::fire(ARGSS... args) {
        for(auto const & handler : handlers_) {
            handler(args...);
        }
    }
}; // utils 
