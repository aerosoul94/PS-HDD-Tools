#include "aes_cbc_strategy.hpp"
#include "aes/aes.h"
#include <utilities/endian.hpp>

namespace crypto {

AesCbcStrategy::AesCbcStrategy(std::vector<char>& keyData)
{
    aes_setkey_dec(&aes, reinterpret_cast<uint8_t*>(keyData.data()), 192);
}

void AesCbcStrategy::decrypt(char* data, uint32_t sector, uint32_t length)
{
    // to avoid the warning
    sector = sector;
    for (uint32_t i = 0; i < length; i += kSectorSize) {
        if (*reinterpret_cast<uint64_t*>(data + i) != 0) {
            uint8_t iv[0x10] = { 0 };
            aes_crypt_cbc(&aes, AES_DECRYPT, kSectorSize, iv,
                reinterpret_cast<uint8_t*>(data + i),
                reinterpret_cast<uint8_t*>(data + i));
        }
    }
}

} /* namespace crypto */
