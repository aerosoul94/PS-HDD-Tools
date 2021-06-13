#ifndef CRYPTO_STRATEGY_HPP
#define CRYPTO_STRATEGY_HPP

#include <stdint.h>

namespace crypto {

/**
 * @brief A strategy for post/pre processing data using cryptography.
 */
class CryptoStrategy
{
public:
  //virtual ~CryptoStrategy() = 0;

  /**
   * @brief Decrypt ciphertext data.
   * 
   * @param data Data to decrypt.
   * @param sector Sector index for crypto algorithms that require a sector.
   * @param length The length of the data that needs to be decrypted.
   */
  virtual void decrypt(char* data, uint32_t sector, uint32_t length) = 0;

  /**
   * @brief Encrypt plaintext data.
   */
  //virutal void encrypt(char* data, uint32_t sector, uint32_t length) = 0;
};

} /* namespace crypto */

#endif /* CRYPTO_STRATEGY_HPP */
