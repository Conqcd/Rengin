#pragma once
#include <xhash>

namespace Rengin {
class UUID {
private:
    uint64_t m_UUID;
public:
    UUID();
    UUID(uint64_t id);
    UUID(const UUID&) = default;
    ~UUID() = default;

    operator uint64_t() const {return m_UUID;}
};
} // namespace Rengin

namespace std
{
    template<>
    struct hash<Rengin::UUID>
    {
        std::size_t operator()(const Rengin::UUID& uuid) const
        {
            return hash<uint64_t>()(static_cast<uint64_t>(uuid));
        }
    };
}
