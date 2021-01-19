#include "multi_layer_strategy.hpp"

namespace crypto {

MultiLayerStrategy::MultiLayerStrategy() {}

void MultiLayerStrategy::decrypt(char* data, uint32_t sector, uint32_t length)
{
  for (auto layer : layers)
    layer->decrypt(data, sector, length);
}

void MultiLayerStrategy::addLayer(CryptoStrategy* layer)
{
  layers.push_back(layer);
}

} /* namespace crypto */
