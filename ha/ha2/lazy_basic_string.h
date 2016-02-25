#ifndef LAZY_BASIC_STRING
#define LAZY_BASIC_STRING
#include <iosfwd>
#include <memory>

namespace std_utils {
    lazy_string
    template<class CharT, class Traits = std::char_traits<CharT>>
    class lazy_basic_string {
    public:

    private:
        std::shared_ptr<char> buffer_;
    };
}



#endif // LAZY_BASIC_STRING
