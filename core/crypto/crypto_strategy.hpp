#ifndef CRYPTO_STRATEGY_HPP
#define CRYPTO_STRATEGY_HPP

/// Strategy for handling data using cryptography
class CryptoStrategy
{
public:
  //virtual ~CryptoStrategy() = 0;
  virtual void decrypt(char* data, uint32_t sector, uint32_t length) = 0;
  //virutal void encrypt(char* data, uint32_t sector, uint32_t length) = 0;
};

#endif /* CRYPTO_STRATEGY_HPP */
