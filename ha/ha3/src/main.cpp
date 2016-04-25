#include "fn.h"

extern int bind_tests_start();
extern int my_testt_start();
extern int function_tests_start();
using namespace fn;

int main() {
    my_testt_start();
    bind_tests_start();
    function_tests_start();
    return 0;
}
