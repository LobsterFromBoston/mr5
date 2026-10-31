#include "instance.h"
#include "../mem/memory.h"
#include "offsets.hpp"

std::string rbx::nameable_t::get_string_value()
{
	std::string result;

	for (int i = 0; i < 8; i++)
	{
		char c = memory->read<char>(this->address + Offsets::Misc::Value + i);

		if (c == '\0') break;
		result += c;
	}

	return result;
}

std::string rbx::nameable_t::get_name()
{
	std::uint64_t nameptr = memory->read<uint64_t>(this->address + Offsets::Instance::Name);
	if (nameptr)
	{
		return memory->read_string(nameptr);
	}
	return "unknown";
}

std::string rbx::nameable_t::get_class_name()
{
	std::uint64_t cd = memory->read<uint64_t>(this->address + Offsets::Instance::ClassDescriptor);
	std::uint64_t cn = memory->read<uint64_t>(cd + Offsets::Instance::ClassName);
	if (cn)
	{
		return memory->read_string(cn);
	}
	return "unknown";
}

std::vector<rbx::instance_t> rbx::interface_t::get_children()
{
	rbx::instance_t* base = static_cast<rbx::instance_t*>(this);
	std::uint64_t start{ memory->read<uint64_t>(base->address + Offsets::Instance::ChildrenStart) };
	std::uint64_t end{ memory->read<uint64_t>(start + Offsets::Instance::ChildrenEnd) } ;

	std::vector<rbx::instance_t> children;

	for (std::uint64_t instance = memory->read<std::uint64_t>(start); instance < end; instance += sizeof(std::shared_ptr<void*>))
	{
		children.emplace_back(memory->read<uint64_t>(instance));
	}
	return children;
}

rbx::instance_t rbx::interface_t::find_first_child(std::string_view str)
{
	std::vector<rbx::instance_t> children = this->get_children();

	for (rbx::instance_t& child : children)
	{
		if (child.get_name() == str)
		{
			return child;
		}
	}

	return {};
}

rbx::instance_t rbx::interface_t::find_first_child_of_class(std::string_view str)
{
	std::vector<rbx::instance_t> children = this->get_children();

	for (rbx::instance_t& child : children)
	{
		if (child.get_class_name() == str)
		{
			return child;
		}
	}

	return {};
}

rbx::instance_t resolve(rbx::instance_t root, const std::vector<std::string>& path) {
	rbx::instance_t current = root;
	for (const auto& name : path) {
		current = current.find_first_child(name);
		if (current.address == 0) {
			break;
		}
	}
	return current;
}