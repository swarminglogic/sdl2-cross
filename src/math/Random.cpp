#include <math/Random.h>

std::mt19937 Random::device_ = std::mt19937(std::random_device{}());
std::uniform_real_distribution<float> Random::dist_(0.0f, 1.0f);
unsigned int Random::seed_(0);
