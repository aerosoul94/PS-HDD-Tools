#ifndef AES_CBC_STRATEGY_HPP
#define AES_CBC_STRATEGY_HPP

#include <vector>

#include "crypto_strategy.hpp"
#include "aes/aes.h"

namespace crypto {

/**
 * @brief A crypto strategy that uses AES-CBC.
 */
class AesCbcStrategy : public CryptoStrategy
{
public:
  AesCbcStrategy(std::vector<char>& keyData);

  void decrypt(char* data, uint32_t sector, uint32_t length) override;

private:
  aes_context aes;
  const uint32_t kSectorSize = 0x200;
};

} /* namespace crypto */

#endif /* AES_CBC_STRATEGY_HPP */
