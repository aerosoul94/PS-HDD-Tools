#ifndef AES_CBC_SWAPPED_STRATEGY_HPP
#define AES_CBC_SWAPPED_STRATEGY_HPP

#include <vector>

#include "crypto_strategy.hpp"
#include "aes/aes.h"

namespace crypto {

/// AES-CBC-192 with swapped halfwords
class AesCbcSwappedStrategy : public CryptoStrategy
{
public:
  AesCbcSwappedStrategy(std::vector<char>& keyData);

  void decrypt(char* data, uint32_t sector, uint32_t length) override;

private:
  void swapWords(char* data, uint32_t length);
  
  aes_context aes;
  const uint32_t kSectorSize = 0x200;
};

} /* namespace crypto */

#endif /* AES_CBC_SWAPPED_STRATEGY_HPP */
