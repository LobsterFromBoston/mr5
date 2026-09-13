#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <iostream>

#include "../mem/memory.h"
#include "offsets.hpp"

namespace rbx
{
    class instance_t;

    struct addressable_t
    {
        std::uint64_t address{};

        addressable_t() = default;
        explicit addressable_t(std::uint64_t addr)
            : address(addr)
        {
        }
    };

    struct nameable_t : public addressable_t
    {
        using addressable_t::addressable_t;

        template <typename T>
        T get_value_as()
        {
            if (!address)
                return T{};

            return memory->read<T>(
                address + Offsets::Misc::Value
            );
        }

        std::string get_string_value();
        std::string get_name();
        std::string get_class_name();

        /*
         * Generic attribute reader.
         *
         * Used for things such as:
         *   speed
         *
         * where the attribute value is numeric.
         */
        template <typename T>
        T get_attribute(std::string_view attribute_name)
        {
            if (!address)
                return T{};

            const std::uint64_t component =
                memory->read<std::uint64_t>(
                    address + Offsets::Instance::ComponentMap
                );

            if (!component)
                return T{};

            const std::uint64_t start =
                memory->read<std::uint64_t>(component);

            const std::uint64_t end =
                memory->read<std::uint64_t>(component + 0x8);

            if (!start || !end || end <= start)
                return T{};

            for (std::uint64_t index = 0;
                 index < (end - start);
                 index += 0x10)
            {
                const std::uint64_t entry =
                    memory->read<std::uint64_t>(start + index);

                if (!entry)
                    continue;

                /*
                 * Do NOT use:
                 *
                 *     entry += 0x10
                 *
                 * because that modifies entry.
                 */
                const std::uint64_t listing =
                    memory->read<std::uint64_t>(entry + 0x10);

                if (!listing)
                    continue;

                for (int step = 0;
                     step < static_cast<int>(Offsets::Attribute::Size * 32);
                     step += static_cast<int>(Offsets::Attribute::Size))
                {
                    const std::uint64_t name_ptr =
                        memory->read<std::uint64_t>(
                            listing +
                            step +
                            Offsets::Attribute::Key
                        );

                    if (!name_ptr)
                        break;

                    const std::string current_name =
                        memory->read_string(name_ptr);

                    if (current_name.empty())
                        continue;

                    if (current_name == attribute_name)
                    {
                        const std::uint64_t value_address =
                            listing +
                            step +
                            Offsets::Attribute::Value;

                        return memory->read<T>(value_address);
                    }
                }
            }

            return T{};
        }

        /*
         * String attribute reader.
         *
         * Used for:
         *   Day_2
         *   Day_3
         *   direction
         */
        template <>
        std::string get_attribute<std::string>(
            std::string_view attribute_name);
    };


    struct interface_t : public addressable_t
    {
        using addressable_t::addressable_t;

        std::vector<instance_t> get_children();

        instance_t find_first_child(
            std::string_view name
        );

        instance_t find_first_child_of_class(
            std::string_view class_name
        );
    };


    class instance_t : public interface_t, public nameable_t
    {
    public:
        instance_t() = default;

        explicit instance_t(std::uint64_t addr)
            : addressable_t(addr)
        {
        }

        operator bool() const
        {
            return address != 0;
        }
    };
}


/*
 * String attribute specialization.
 *
 * This is intentionally outside the class definition because
 * explicit template specializations need to be defined this way.
 */
template <>
inline std::string rbx::nameable_t::get_attribute<std::string>(
    std::string_view attribute_name)
{
    if (!address)
        return "unknown";

    const std::uint64_t component =
        memory->read<std::uint64_t>(
            address + Offsets::Instance::ComponentMap
        );

    if (!component)
        return "unknown";

    const std::uint64_t start =
        memory->read<std::uint64_t>(component);

    const std::uint64_t end =
        memory->read<std::uint64_t>(component + 0x8);

    if (!start || !end || end <= start)
        return "unknown";

    for (std::uint64_t index = 0;
         index < (end - start);
         index += 0x10)
    {
        const std::uint64_t entry =
            memory->read<std::uint64_t>(start + index);

        if (!entry)
            continue;

        const std::uint64_t listing =
            memory->read<std::uint64_t>(entry + 0x10);

        if (!listing)
            continue;

        for (int step = 0;
             step < static_cast<int>(Offsets::Attribute::Size * 32);
             step += static_cast<int>(Offsets::Attribute::Size))
        {
            const std::uint64_t name_ptr =
                memory->read<std::uint64_t>(
                    listing +
                    step +
                    Offsets::Attribute::Key
                );

            if (!name_ptr)
                break;

            const std::string current_name =
                memory->read_string(name_ptr);

            if (current_name.empty())
                continue;

            if (current_name == attribute_name)
            {
                const std::uint64_t value_address =
                    listing +
                    step +
                    Offsets::Attribute::Value;

                return memory->read_string(value_address);
            }
        }
    }

    return "unknown";
}


rbx::instance_t resolve(
    rbx::instance_t root,
    const std::vector<std::string>& path
);
