#pragma once
#include <string>
#include "krnlib/memory.hpp"

namespace krnlib {
using string = std::basic_string<char, std::char_traits<char>, krnlib::allocator<char>>;
using wstring = std::basic_string<wchar_t, std::char_traits<wchar_t>, krnlib::allocator<wchar_t>>;
}