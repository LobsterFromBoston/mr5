#pragma once
#include <string_view>
#include <type_traits>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <string>
#include "../mem/memory.h"
#include "offsets.hpp"
#include "math.h"

namespace attr_detail {
	inline bool plausible_ptr(uint64_t p) {
		return p > 0x10000 && p < 0x00007FFFFFFFFFFF;
	}

	inline bool plausible_key(const std::string& s) {
		if (s.empty() || s.size() > 128) return false;
		for (unsigned char c : s)
			if (c < 0x20 || c > 0x7E) return false;
		return true;
	}

	inline uint64_t find_record(uint64_t instance, std::string_view wanted, bool verbose = false) {
		uint64_t componentMap = memory->read<uint64_t>(instance + Offsets::Instance::ComponentMap);
		if (!plausible_ptr(componentMap)) return 0;

		uint64_t start = memory->read<uint64_t>(componentMap);
		uint64_t end = memory->read<uint64_t>(componentMap + 0x8);
		if (!plausible_ptr(start) || end <= start || (end - start) > 0x4000) return 0;

		for (uint64_t slot = start; slot < end; slot += Offsets::AttributesMap::ComponentEntrySize) {
			uint64_t entry = memory->read<uint64_t>(slot);
			if (!plausible_ptr(entry)) continue;

			uint64_t listing = memory->read<uint64_t>(entry + Offsets::AttributesMap::Attributes);
			uint32_t count = memory->read<uint32_t>(entry + Offsets::AttributesMap::Length);
			if (!plausible_ptr(listing) || count == 0 || count > 512) continue;

			uint64_t firstKey = memory->read<uint64_t>(listing + Offsets::Attribute::Key);
			if (!plausible_ptr(firstKey)) continue;
			if (!plausible_key(memory->read_string(firstKey + Offsets::Attribute::KeyString))) continue;

			if (verbose) {
				uint32_t tag = memory->read<uint32_t>(slot + Offsets::AttributesMap::ComponentTagOffset);
				printf("[attr] attribute entry found, tag=0x%x count=%u\n", tag, count);
			}

			for (uint32_t i = 0; i < count; i++) {
				uint64_t record = listing + (uint64_t)i * Offsets::Attribute::Size;
				uint64_t keyPtr = memory->read<uint64_t>(record + Offsets::Attribute::Key);
				if (!plausible_ptr(keyPtr)) continue;

				std::string key = memory->read_string(keyPtr + Offsets::Attribute::KeyString);
				if (!plausible_key(key)) continue;

				if (verbose) printf("[attr]   #%u key='%s'\n", i, key.c_str());
				if (key == wanted) return record;
			}
		}
		return 0;
	}
}

namespace rbx
{
	class instance_t;

	struct addressable_t
	{
		std::uint64_t address;

		addressable_t() : address(0) {}
		addressable_t(std::uint64_t address) : address(address) {}
	};

	struct nameable_t : public addressable_t
	{
		using addressable_t::addressable_t;

		template <typename T>
		T get_value_as()
		{
			return memory->read<T>(this->address + Offsets::Misc::Value);
		}
		std::string get_string_value();
		std::string get_name();
		std::string get_class_name();
		//std::string get_attribute(std::string_view name_to_search);
		template<typename T>
		T get_attribute(std::string_view name_to_search)
		{
			static_assert(std::is_trivially_copyable_v<T>,
				"use the std::string specialization for strings");

			uint64_t record = attr_detail::find_record(this->address, name_to_search);
			if (!record) return T{};

			return memory->read<T>(record + Offsets::Attribute::Value);
		}
	};

	struct interface_t
	{
		template <typename T>
		std::vector<T> get_children();

		std::vector<rbx::instance_t> get_children();
		rbx::instance_t find_first_child(std::string_view str);
		rbx::instance_t find_first_child_of_class(std::string_view str);
	};

	struct instance_t : public nameable_t, public interface_t
	{
		using nameable_t::nameable_t;
	};
}

template <typename T>
std::vector<T> rbx::interface_t::get_children()
{
	rbx::instance_t* base = static_cast<rbx::instance_t*>(this);

	std::uint64_t start = memory->read<uint64_t>(base->address + Offsets::Instance::ChildrenStart);
	std::uint64_t end = memory->read<uint64_t>(start + Offsets::Instance::ChildrenEnd);

	std::vector<T> children;

	for (std::uint64_t instance = memory->read<std::uint64_t>(start); instance < end; instance += sizeof(std::shared_ptr<void*>))
	{
		children.emplace_back(memory->read<uint64_t>(instance));
	}
	return children;
}

template<>
inline std::string rbx::nameable_t::get_attribute<std::string>(std::string_view name) {
	uint64_t record = attr_detail::find_record(this->address, name, /*verbose*/ true);
	if (!record) return "unknown";
	return memory->read_string(record + Offsets::Attribute::Value);
}


rbx::instance_t resolve(rbx::instance_t root, const std::vector<std::string>& path);