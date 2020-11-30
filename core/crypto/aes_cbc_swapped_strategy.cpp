#include "aes_cbc_swapped_strategy.hpp"
#include "aes/aes.h"
#include <utilities/endian.hpp>

AesCbcSwappedStrategy::AesCbcSwappedStrategy(std::vector<char>& keyData)
{
  aes_setkey_dec(&aes, reinterpret_cast<uint8_t*>(keyData.data()), 192);
}

void AesCbcSwappedStrategy::decrypt(char* data, uint32_t sector, uint32_t length)
{
  swapWords(data, length);
  for (uint32_t i = 0; i < length; i += kSectorSize) {
    if(*reinterpret_cast<uint64_t*>(data + i) != 0) {
      uint8_t iv[0x10] = {0};
      aes_crypt_cbc(&aes, AES_DECRYPT, kSectorSize, iv, 
        reinterpret_cast<uint8_t*>(data + i), 
        reinterpret_cast<uint8_t*>(data + i));
    }
  }
}

void AesCbcSwappedStrategy::swapWords(char* data, uint32_t length)
{
  for (uint32_t i = 0; i < length / 2; i++)
    ((uint16_t*)data)[i] = swap16(((uint16_t*)data)[i]);
}
