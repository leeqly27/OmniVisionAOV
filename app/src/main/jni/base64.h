#pragma once

#ifndef BASE64_H
#define BASE64_H

#include <string>

std::string base64_encode(const std::string &input);
std::string base64_decode(const std::string &input);
std::string base64_encode_2(unsigned char const* , unsigned int len);
std::string base64_decode_2(std::string const& s);

#endif // BASE64_H



