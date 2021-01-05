#ifndef AES_XTS_SWAPPED_STRATEGY_HPP
#define AES_XTS_SWAPPED_STRATEGY_HPP

#include <vector>

#include "crypto_strategy.hpp"
#include "aes/aes_xts.h"

namespace crypto {

/// AES-XTS with swapped halfwords
class AesXtsSwappedStrategy : public CryptoStrategy
{
public:
  AesXtsSwappedStrategy(char*, char*);

  void decrypt(char* data, uint32_t sector, uint32_t length) override;

private:
  void swapWords(char* data, uint32_t length);
  
  aes_xts_ctxt_t xts_ctxt;
  const uint32_t kSectorSize = 0x200;
};

} /* namespace crypto */

#endif /* AES_XTS_SWAPPED_STRATEGY_HPP */
