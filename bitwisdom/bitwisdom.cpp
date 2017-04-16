
#include <cheerp/client.h> //Misc client side stuff
#include <cheerp/clientlib.h> //Complete DOM/HTML5 interface
#include <type_traits>
using namespace client;
using biggest_t = unsigned int;
using Values_t = cheerp::TypedArrayForPointerType<biggest_t>::type;

auto getInputValues() {
    return Values_t();
}

biggest_t unify(Values_t vals) {
    biggest_t result = 0;
    for(auto&& val : vals)
        result |= val;
    return result;
}

biggest_t common(Values_t vals) {
    biggest_t result = (std::make_signed<biggest_t>::type)-1;
    for(auto&& val : vals)
        result |= val;
    return result;
}

void loadCallback()
{
    HTMLElement* body = document.get_body();
    
}

void webMain()
{
    document.addEventListener("DOMContentLoaded", cheerp::Callback(loadCallback));
    
}
