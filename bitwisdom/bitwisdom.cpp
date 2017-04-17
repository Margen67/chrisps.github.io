
#include <cheerp/client.h> //Misc client side stuff
#include <cheerp/clientlib.h> //Complete DOM/HTML5 interface
#include <type_traits>
#include <vector>
#include <cstdint>
#include <cstdlib>
//#include "uint256_t.h"
#include "bigints.hpp"
#include <string>
using namespace client;
using biggest_t = uint256_t;
using Values_t = std::vector<biggest_t>;
using ix_t = double;
static biggest_t unify(Values_t vals) {
    biggest_t result = 0;
    for(auto&& val : vals)
        result |= val;
    return result;
}

static biggest_t common(Values_t vals) {
    biggest_t result = -1;
    for(auto&& val : vals)
        result &= val;
    return result;
}
static biggest_t lowerBound(Values_t vals) {
    biggest_t value = -1;
    for(auto&& val : vals) 
        value = std::min(value, val);
    return value;
}
static Values_t operate(Values_t input, biggest_t param, biggest_t (*transformer)(biggest_t, biggest_t)) {
    Values_t result;
    result.reserve(input.size());
    for(ix_t i = 0; i < input.size(); ++i ) {
        result[i] = transformer(input[i], param);
    }
    return result;
}


static HTMLTextAreaElement* texin = nullptr;
static HTMLButtonElement* butt = nullptr;



static Values_t valarray;
biggest_t curruni = 0;
biggest_t currcomm = 0;
static void submit_cb() {
    String* value = texin->get_value();
    auto& values = *value->split(",");
    valarray.clear();
    ix_t arraylen = values.get_length();
    for(ix_t i = 0; i < arraylen; ++i) {
        String& splitval = (*values[i]);
        auto splitlen = splitval.get_length();
        decltype(parseInt(splitval,10)) parseResult;
        auto char0 = splitlen > 0 ? splitval.charCodeAt(0) : 0;
        auto char1 = splitlen > 1 ? splitval.charCodeAt(1) : 0;
        if(char0 == '0' && (char1 == 'x' || char1 == 'X'))
            parseResult = parseInt(splitval.replace(char1 == 'X' ? "0X": "0x", ""), 16);
        else if(char0 == '0' && char1 == 'b')
            parseResult = parseInt(splitval.replace(char1 == 'B' ? "0B" : "0b", ""), 1);
        else
            parseResult = parseInt(splitval, 10);
        valarray.push_back(uint64_t(parseResult));
    }
    auto unified = unify(valarray);
    auto commonOf = common(valarray);
    char buffer[20];
    char buff2[20];
    curruni = unified;
    currcomm = commonOf;
    alert(String("Unified: ").concat(
        String(itoa(uint32_t(unified), buffer, 16)), String("\nCommon: "),  
        String(itoa(uint32_t(commonOf), buff2, 16))));

}

void loadCallback()
{
    HTMLElement* body = document.get_body();
    auto inpform = document.getElementById("inputform");
    texin = (decltype(texin))document.getElementById("range_input");
    butt = (decltype(butt))document.getElementById("submit_button");
    auto dtypebutton = document.getElementById("datatype");
    butt->set_onclick(cheerp::Callback(submit_cb));
}

void webMain()
{
    document.addEventListener("DOMContentLoaded", cheerp::Callback(loadCallback));
    
}
