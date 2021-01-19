#ifndef MULTI_LAYER_STRATEGY_HPP
#define MULTI_LAYER_STRATEGY_HPP

#include <vector>

#include "crypto_strategy.hpp"

namespace crypto {

/**
 * @brief Handles multiple layers of crypto.
 */
class MultiLayerStrategy : public CryptoStrategy
{
public:
  MultiLayerStrategy();

  void decrypt(char* data, uint32_t sector, uint32_t length) override;

  /**
   * @brief Add a layer of a cryptographic handler.
   * 
   * @param layer The crypto layer. 
   */
  void addLayer(CryptoStrategy* layer);

private:
  std::vector<CryptoStrategy*> layers;
};

} /* namespace crypto */

#endif /* MULTI_LAYER_STRATEGY_HPP */
