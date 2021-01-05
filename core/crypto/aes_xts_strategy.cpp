#include "aes_xts_strategy.hpp"

#include <utilities/endian.hpp>

namespace crypto {

AesXtsStrategy::AesXtsStrategy(char* key, char* tweak)
{
  aes_xts_init(&xts_ctxt, AES_DECRYPT, 
    reinterpret_cast<uint8_t*>(key), 
    reinterpret_cast<uint8_t*>(tweak), 
    128
  );
}

void AesXtsStrategy::decrypt(char* data, uint32_t sector, uint32_t length)
{
  for (uint32_t i = 0; i < length; i += kSectorSize) {
    // Don't decrypt if data is not encrypted
    if (*reinterpret_cast<uint64_t*>(data + i) != 0) {
      aes_xts_crypt(&xts_ctxt, sector + i / kSectorSize, kSectorSize, 
        reinterpret_cast<uint8_t*>(data) + i, 
        reinterpret_cast<uint8_t*>(data) + i);
    }
  }
}

} /* namespace crypto */
