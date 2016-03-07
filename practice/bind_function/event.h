#include <vector>
#include <functional>
#include <memory>

namespace utils {
    template<typename>
    struct event;

    template<class R, class... ARGSS>
    struct event<R(ARGSS...)> {
        using handler_t = std::function<R(ARGSS...)>;
        using handlers_collection_t = std::vector<std::pair<size_t, handler_t>>;
        struct event_deleter {
            event_deleter(handlers_collection_t* events, size_t id)
            : id_(id)
            , ptr_(events) {
            }

            void operator()();

        private:
            size_t id_;
            handlers_collection_t* ptr_;
        };
        event();
        event(event const& other);

        event operator=(event const& other);
        event operator=(event && other);

        ~event() = default;
        event_deleter add(handler_t const & handler);
        /*event_deleter add(handler_t && handler);*/

        void fire(ARGSS...);
    private:
        size_t last_id = 0;
        std::unique_ptr<handlers_collection_t> handlers_ptr_;
    };

    template <class R, class ... ARGSS>
    void event<R(ARGSS...)>::event_deleter::operator()() {
        for (auto it = ptr_->begin(); it != ptr_->end(); ++it) {
            if(it->first == id_) {
                ptr_->erase(it);
                break;
            }
        }
    }

    template <class R, class ... ARGSS>
    event<R(ARGSS...)>::event()
        : handlers_ptr_(new handlers_collection_t())
    {}

    template <class R, class ... ARGSS>
    event<R(ARGSS...)>::event(event const& other)
        : handlers_ptr_(other.handlers_ptr_) {
    }

    template <class R, class ... ARGSS>
    event<R(ARGSS...)> event<R(ARGSS...)>::operator=(event const& other) {
        handlers_ptr_ = other.handlers_ptr_;
        return *this;
    }

    template <class R, class ... ARGSS>
    event<R(ARGSS...)> event<R(ARGSS...)>::operator=(event&& other) {
        handlers_ptr_ = std::move(other.handlers_ptr_);
        return *this;
    }

    template <class R, class ... ARGSS>
    typename event<R(ARGSS...)>::event_deleter event<R(ARGSS...)>::add(handler_t const& handler) {
        handlers_ptr_->emplace_back(std::make_pair(++last_id, std::move(handler)));
        return event_deleter(handlers_ptr_.get(), last_id - 1);
    }

    template <class R, class ... ARGSS>
    void event<R(ARGSS...)>::fire(ARGSS... args) {
        for(auto const & pair : *handlers_ptr_) {
            pair.second(args...);
        }
    }
}; // utils 
