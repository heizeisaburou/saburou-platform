#include <saburou/platform/v2.hpp>
#include <saburou/platform/v2/os/linux.hpp> // distro_info

#include <saburou/platform/v2/bytes/byte_swap.hpp>
#include <saburou/platform/v2/bytes/endian.hpp>

#include <format>
#include <iostream>

namespace os = saburou::platform::v2::os;

int main() {
    auto type = os::type();
    std::cout << std::format("type\n  {:r} -> {}\n", type, type); // r == repr (python based)
    auto name = os::to_name(type);
    std::cout << std::format("(type_)name\n  {}\n", name);


    std::cout << "\n";
    auto family = os::family();
    std::cout << std::format("family\n  {:r} -> {}\n", family, family);
    auto family_name = os::to_family_name(family);
    std::cout << std::format("family_name\n  {}\n", family_name);


    std::cout << "\n";
    auto info = os::info();
    std::cout << "(os_)info\n";
    std::cout << std::format("  [repr]  {:r}\n", info);
    std::cout << std::format("[normal]  {:s}\n", info);


    std::cout << "\n";
    auto distro_info = os::linux::distro_info();
    std::cout << "distro_info\n";
    std::cout << std::format("  [repr]  {:r}\n", distro_info);
    std::cout << std::format("[normal]  {}\n", distro_info); // same as :s


    namespace endian = saburou::platform::v2::bytes::endian;
    using saburou::platform::v2::bytes::byte_swap;

    std::cout << std::format("\nEndian: Little? {} | Big? {}\n", endian::is_little, endian::is_big);
    uint16_t val = 0xAAFF; // 10101010 11111111
    uint16_t swapped = byte_swap(val);
    std::cout << std::format("Original: {:016b} ({:#06x})\n", val, val);
    std::cout << std::format("Swapped:  {:016b} ({:#06x})\n", swapped, swapped);
    if (val == byte_swap(swapped)) std::cout << "Check: Involution OK\n";
    std::cout << std::format("to_little changes? {} | to_big changes? {}\n", endian::to_little(val) != val,
                             endian::to_big(val) != val);
    bool identity_ok = endian::is_little ? (endian::to_little(val) == val) : (endian::to_big(val) == val);
    if (identity_ok) std::cout << "Check: Native Identity OK (No-op verified)\n";
}
