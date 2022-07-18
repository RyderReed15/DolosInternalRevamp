#pragma once

#ifndef HASH_H
#define HASH_H

#include <type_traits>

//Reconstructed from std::hash<basic_string> in constexpr form | same result as std::hash
constexpr size_t const_hash(const char* szString) {

    size_t value = std::_FNV_offset_basis;
    while (*szString != '\0') {
        value ^= static_cast<size_t>(*szString);
        value *= std::_FNV_prime;
        szString++;
    }
    return value;
}
//Use length without \\0 or else will give different result than std::hash without size
constexpr size_t const_hash(const char* szString, size_t uSize) {

    size_t value = std::_FNV_offset_basis;
    for (size_t i = 0; i < uSize; ++i) {
        value ^= static_cast<size_t>(szString[i]);
        value *= std::_FNV_prime;
    }
    return value;
}

#endif // !HASH_H
