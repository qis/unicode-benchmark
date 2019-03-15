#include "common.hpp"
#include <random>

const u8string src = []() {
  std::random_device rd;
  std::uniform_int_distribution<u8string::value_type> dist(u8"0"[0], u8"9"[0]);
  u8string src = u8"unicode_test _ ぁあぃいぅうぇえぉおかがきぎ";
  src[13] = dist(rd);
  return src;
}();

const u32string dst = []() {
  std::u32string dst = U"unicode_test _ ぁあぃいぅうぇえぉおかがきぎ";
  dst[13] = static_cast<char32_t>(src[13]);
  return dst;
}();
