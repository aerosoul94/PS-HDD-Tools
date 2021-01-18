#ifndef AES_XTS_STRATEGY_HPP
#define AES_XTS_STRATEGY_HPP

#include <vector>

#include "crypto_strategy.hpp"
#include "aes/aes_xts.h"

namespace crypto {

/**
 * @brief A crypto strategy that uses AES-XTS.
 */
class AesXtsStrategy : public CryptoStrategy
{
public:
  AesXtsStrategy(char* key, char* tweak);

  void decrypt(char* data, uint32_t sector, uint32_t length) override;

private:
  aes_xts_ctxt_t xts_ctxt;
  const uint32_t kSectorSize = 0x200;
};

} /* namespace crypto */

#endif /* AES_XTS_STRATEGY_HPP */
