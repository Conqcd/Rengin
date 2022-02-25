#include "repch.hpp"
#include "UUID.hpp"
#include <random>

static std::unordered_map<Rengin::UUID,std::string> m_Maps;

static void Add2Map()
{
    m_Maps[Rengin::UUID()] = "me";
}

namespace Rengin
{

static std::random_device s_RandomDevice;
static std::mt19937_64 s_Engine(s_RandomDevice());
static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

UUID::UUID()
    : m_UUID(s_UniformDistribution(s_Engine))
{
}

UUID::UUID(uint64_t id)
    : m_UUID(id)
{
}

} // namespace Rengin
