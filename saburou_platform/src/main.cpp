#include <saburou/platform/v1.hpp>
#include <saburou/platform/v1/os/linux.hpp> // distro_info

#include <format>
#include <iostream>

int main() {

    auto type = saburou::platform::v1::os::type();
    std::cout << std::format("type\n  {:r} -> {}\n", type, type);  // r == repr (python based)
    auto name = saburou::platform::v1::os::to_name(type);
    std::cout << std::format("(type_)name\n  {}\n", name);

    std::cout << "\n";
    auto family = saburou::platform::v1::os::family();
    std::cout << std::format("family\n  {:r} -> {}\n", family, family);
    auto family_name = saburou::platform::v1::os::to_family_name(family);
    std::cout << std::format("family_name\n  {}\n", family_name);

    std::cout << "\n";
    auto info = saburou::platform::v1::os::info();
    std::cout << "(os_)info\n";
    std::cout << std::format("  [repr]  {:r}\n", info);
    std::cout << std::format("[normal]  {:s}\n", info);

    std::cout << "\n";
    auto distro_info = saburou::platform::v1::os::linux::distro_info();
    std::cout << "distro_info\n";
    std::cout << std::format("  [repr]  {:r}\n", distro_info);
    std::cout << std::format("[normal]  {}\n", distro_info); // same as :s
}
