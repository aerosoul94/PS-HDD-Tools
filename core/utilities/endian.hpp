#ifndef ENDIAN_HPP
#define ENDIAN_HPP

#define swap16(val) \
  (uint16_t)(((val & 0xff00) >> 8) | ((val & 0x00ff) << 8))

#define swap32(val) \
  ((uint32_t)(((((uint32_t)val) & 0xff000000) >> 24) | \
              ((((uint32_t)val) & 0x00ff0000) >> 8 ) | \
              ((((uint32_t)val) & 0x0000ff00) << 8 ) | \
              ((((uint32_t)val) & 0x000000ff) << 24)))

#define swap64(val) \
  ((uint64_t)(((((uint64_t)val) & 0xff00000000000000ull) >> 56) | \
              ((((uint64_t)val) & 0x00ff000000000000ull) >> 40) | \
              ((((uint64_t)val) & 0x0000ff0000000000ull) >> 24) | \
              ((((uint64_t)val) & 0x000000ff00000000ull) >> 8 ) | \
              ((((uint64_t)val) & 0x00000000ff000000ull) << 8 ) | \
              ((((uint64_t)val) & 0x0000000000ff0000ull) << 24) | \
              ((((uint64_t)val) & 0x000000000000ff00ull) << 40) | \
              ((((uint64_t)val) & 0x00000000000000ffull) << 56)))

#endif /* ENDIAN_HPP */
