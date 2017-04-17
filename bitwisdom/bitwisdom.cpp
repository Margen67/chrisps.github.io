
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
using jnum_t = double;
constexpr jnum_t operator ""_jschar(char c) {
    return static_cast<jnum_t>(c);
}

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
static biggest_t gcd( biggest_t x, biggest_t y )
{
    if( x < y )
        std::swap( x, y );

    while( y > biggest_t(0) )
    {
        biggest_t f = x % y;
        x = y;
        y = f;
    }
    return x;
}  

static biggest_t gcd(Values_t vals) {
    biggest_t curr = vals[0];
    for(ix_t i = 1; i < vals.size(); ++i) {
        curr = gcd(curr, vals[i]);
    }
    return curr;
}


static HTMLTextAreaElement* texin = nullptr;
static HTMLButtonElement* butt = nullptr;
static HTMLTextAreaElement* texout = nullptr;

static void writeOut(String&& s) {
    String* initialValue = texout->get_value();
    if(initialValue != nullptr) {
        texout->set_value(initialValue->concat(String("\n"), s));
    }
    else
        texout->set_value(s);
}

static void writeOut(std::string s) {
    writeOut(String(s.data()));
}

static Values_t valarray;

static biggest_t parseVal(String& value, jnum_t radix) {
    
    return static_cast<uint64_t>(parseInt(value, radix));
}

static biggest_t parseInputValue(String& splitval) {
    jnum_t splitlen = splitval.get_length();
    const jnum_t char0 = splitlen > 0 ? splitval.charCodeAt(0) : 0;
    const jnum_t char1 = splitlen > 1 ? splitval.charCodeAt(1) : 0;
    jnum_t radix = 10;

    if(char0 == '0'_jschar && (char1 == 'x'_jschar || char1 == 'X'_jschar)) {
        radix = 16;
        splitval = splitval.replace(char1 == 'X'_jschar ? "0X": "0x", "");
    } else if(char0 == '0'_jschar && char1 == 'b'_jschar) {
        radix = 2;
        splitval = splitval.replace(char1 == 'B'_jschar ? "0B" : "0b", "");
    }
    return parseVal(splitval, radix);
}

static void writeTrivialAttributes(Values_t vals) {
    auto unified = unify(vals);
    auto commonOf = common(vals);
    char buffer[20];
    char buff2[20];

    writeOut(String("Unified: ").concat(
        String(itoa(uint32_t(unified), buffer, 16)), String("\nCommon: "),  
        String(itoa(uint32_t(commonOf), buff2, 16))));
}

static void submit_cb() {
    String* value = texin->get_value();
    auto& values = *value->split(",");
    valarray.clear();
    ix_t arraylen = values.get_length();
    for(ix_t i = 0; i < arraylen; ++i) {
        String& splitval = (*values[i]);
        valarray.push_back(parseInputValue(splitval));
    }
    writeTrivialAttributes(valarray);

    auto listgcd = gcd(valarray);
    writeOut(std::string("Greatest common divisor: " + listgcd.str()));

}

void loadCallback()
{
    HTMLElement* body = document.get_body();
    auto inpform = document.getElementById("inputform");
    texin = (decltype(texin))document.getElementById("range_input");
    butt = (decltype(butt))document.getElementById("submit_button");
    auto dtypebutton = document.getElementById("datatype");
    texout = (decltype(texout))document.getElementById("output_area");
    butt->set_onclick(cheerp::Callback(submit_cb));
}

void webMain()
{
    document.addEventListener("DOMContentLoaded", cheerp::Callback(loadCallback));
    
}
