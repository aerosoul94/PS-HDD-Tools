#include "aes_xts_swapped_strategy.hpp"

#include <utilities/endian.hpp>

namespace crypto {

AesXtsSwappedStrategy::AesXtsSwappedStrategy(char* key, char* tweak)
{
  aes_xts_init(&xts_ctxt, AES_DECRYPT, 
    reinterpret_cast<uint8_t*>(key), 
    reinterpret_cast<uint8_t*>(tweak), 
    128
  );
}

void AesXtsSwappedStrategy::decrypt(char* data, uint32_t sector, uint32_t length)
{
  swapWords(data, length);
  for (uint32_t i = 0; i < length; i += kSectorSize) {
    // Don't decrypt if data is not encrypted
    if (*reinterpret_cast<uint64_t*>(data + i) != 0) {
      aes_xts_crypt(&xts_ctxt, sector + i / kSectorSize, kSectorSize, 
        reinterpret_cast<uint8_t*>(data) + i, 
        reinterpret_cast<uint8_t*>(data) + i);
    }
  }
}

void AesXtsSwappedStrategy::swapWords(char* data, uint32_t length)
{
  for (uint32_t i = 0; i < length / 2; i++)
    ((uint16_t*)data)[i] = swap16(((uint16_t*)data)[i]);
}

} /* namespace crypto */
