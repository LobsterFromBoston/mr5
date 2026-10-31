/*
	MR5
	Blah blah blah blah blah blah blah


	Copyleft© bostonlobster3141592653589793238
	- Steal this all you want! I honestly don't care.
*/

#include <iostream>
#include <thread>
#include <sstream>
#include <string>
#include <chrono>
#include <unordered_map>

#include "mem/memory.h"
#include "roblox/roblox.hpp"
#include "roblox/instance.h"
#include "roblox/offsets.hpp"
#include "roblox/twisted/hierarchy.h"

#include "others/discord.hpp"
#include "others/config.hpp"
#include "others/process.hpp"

Twisted::LastRoll prev;

int main()
{
	printf("welcome to memory roller 5000\n\n");

	Config cfg;
	if (!cfg.load("config.json")) {
		printf("failed to load config.json\n");
		std::cin.get();
		return 1;
	}

	int rerolls = 0;
	std::string lastsuccess = "";

	while (true)
	{
		system("cls");
		printf("memory roller 5000\n\n");
		printf("%-20s: %s\n", "rolling version", cfg.version.c_str());
		printf("%-20s: %d / %zu\n", "using vip", (rerolls % cfg.priv_links.size()) + 1, cfg.priv_links.size());
		printf("%-20s: %d seconds\n", "timeout", cfg.timeout);
		printf("%-20s: %d seconds\n", "stay period", cfg.success_wait);

		if (cfg.version == "1.18" || cfg.version == "1.19") {
			printf("%-20s: cape=%d\n", "announce minimum", (int)cfg.cape);
		}
		else {
			printf("%-20s: stp=%d, vtp=%d, srh=%d, cape=%d\n", "minimum sets", (int)cfg.stp, (int)cfg.vtp, (int)cfg.srh, (int)cfg.cape);
		}

		printf("\n%-20s: %d\n\n", "attempt", rerolls);

		std::string hash = Offsets::ClientVersion;
		fs::path roblox_path = fs::current_path() / "roblox" / ("version-" + hash);
		fs::path exe_path = roblox_path / "RobloxPlayerBeta.exe";

		if (!fs::exists(roblox_path) || !fs::exists(exe_path)) {
			printf("downloading roblox version %s\n", hash.c_str());
			std::string cmd = "bootstrapper.bat " + hash;

			if (!fs::exists("bootstrapper.bat")) {
				printf("bootstrapper.bat is missing from the folder!\n");
				std::cin.get();
				return 1;
			}

			int result = system(cmd.c_str());

			if (result == 0) {
				printf("installer completed\n");
				if (!fs::exists(exe_path)) {
					printf("installer finished but robloxplayerbeta is still missing\n");
					std::cin.get();
					return 1;
				}
			}
			else {
				printf("installation failed with error code: %d\n", result);
				std::cin.get();
				return 1;
			}
		}

		if (!cfg.priv_links.empty()) {
			std::string code = cfg.priv_links[rerolls % cfg.priv_links.size()];
			if (!roblox::launcher::launch(cfg.version, code)) {
				printf("failed to open roblox\n");
			}
		}
		else
		{
			printf("no vip links found");
			std::cin.get();
			return 1;
		}

		while (!memory->attach_to_process("RobloxPlayerBeta.exe")) {
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

		bool module_found = false;
		auto moduleStart = std::chrono::steady_clock::now();

		while (!module_found) {
			if (!process::is_running("RobloxPlayerBeta.exe")) break;

			if (memory->find_module_address("RobloxPlayerBeta.exe")) {
				module_found = true;
			}
			else {
				auto now = std::chrono::steady_clock::now();
				int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - moduleStart).count();
				int remaining = cfg.timeout - elapsed;
				if (remaining <= 0) break;
				std::fflush(stdout);
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}
		}

		if (!module_found) {
			printf("timeout or process lost waiting for module\n");
			process::terminate("RobloxPlayerBeta.exe");
			rerolls++;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			continue;
		}

		rbx::instance_t realdm = 0;
		auto dmStart = std::chrono::steady_clock::now();

		while (realdm.address == 0) {
			if (!process::is_running("RobloxPlayerBeta.exe")) break;

			std::uint64_t fakedm = memory->read<std::uint64_t>(memory->get_module_address() + Offsets::FakeDataModel::Pointer);
			if (fakedm != 0) {
				realdm = memory->read<std::uint64_t>(fakedm + Offsets::FakeDataModel::RealDataModel);
			}

			auto now = std::chrono::steady_clock::now();
			int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - dmStart).count();
			int remaining = cfg.timeout - elapsed;

			if (remaining <= 0) break;
			if (realdm.address == 0) {
				std::fflush(stdout);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (realdm.address == 0) {
			printf("timeout waiting for DataModel\n");
			process::terminate("RobloxPlayerBeta.exe");
			rerolls++;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			continue;
		}

		rbx::instance_t replicated_storage = 0;
		auto rsStart = std::chrono::steady_clock::now();

		while (replicated_storage.address == 0) {
			if (!process::is_running("RobloxPlayerBeta.exe")) break;
			std::uint64_t fakedm = memory->read<std::uint64_t>(memory->get_module_address() + Offsets::FakeDataModel::Pointer);
			if (fakedm != 0) {
				realdm = memory->read<std::uint64_t>(fakedm + Offsets::FakeDataModel::RealDataModel);
			}

			if (realdm.address != 0) {
				replicated_storage = resolve(realdm, { "ReplicatedStorage" });
			}

			auto now = std::chrono::steady_clock::now();
			int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - rsStart).count();
			int remaining = cfg.timeout - elapsed;

			if (remaining <= 0) break;
			if (replicated_storage.address == 0) {
				std::fflush(stdout);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

		if (replicated_storage.address == 0) {
			printf("timeout waiting for ReplicatedStorage\n");
			process::terminate("RobloxPlayerBeta.exe");
			rerolls++;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			continue;
		}

		std::unordered_map<std::string, locations>* active_map = &Twisted::Main::locationz;
		if (cfg.version == "1.20") active_map = &Twisted::Legacy20::locationz;
		else if (cfg.version == "1.19") active_map = &Twisted::Legacy19::locationz;
		else if (cfg.version == "1.18") active_map = &Twisted::Legacy18::locationz;

		bool found_everything = false;
		auto kinStart = std::chrono::steady_clock::now();

		while (true) {
			if (!process::is_running("RobloxPlayerBeta.exe")) break;

			realdm = 0;
			bool all_valid = true;

			std::uint64_t fakedm = memory->read<std::uint64_t>(memory->get_module_address() + Offsets::FakeDataModel::Pointer);
			if (fakedm != 0) {
				realdm = memory->read<std::uint64_t>(fakedm + Offsets::FakeDataModel::RealDataModel);
			}

			if (realdm.address != 0) {
				bool all_valid = true;
				for (auto const& [key, loc] : *active_map) {
					if (resolve(realdm, loc.path).address == 0) {
						all_valid = false;
						break;
					}
				}

				if (all_valid) {
					int valid_count = 0;

					for (auto const& [key, loc] : *active_map) {
						rbx::instance_t target = resolve(realdm, loc.path);
						if (target.address == 0) continue;

						if (loc.type == SourceType::Value) {
							if (key.find("risk") != std::string::npos) {
								std::string s = target.get_string_value();
								if (!s.empty() && s != "NONE" && s != "N/A")
									valid_count++;
							}
							else {
								double val = target.get_value_as<double>();
								if (val != 0.0 && std::isfinite(val) && std::abs(val) < 1e10)
									valid_count++;
							}
						}
						else if (loc.type == SourceType::AttributeString) {
							std::string s = target.get_attribute<std::string>(loc.attrName);
							if (!s.empty() && s != "NONE" && s != "N/A" && s != "Unknown")
								valid_count++;
						}
						else if (loc.type == SourceType::AttributeDouble) {
							double val = target.get_attribute<double>(loc.attrName);
							if (val != 0.0 && std::isfinite(val) && std::abs(val) < 1e10)
								valid_count++;
						}
					}

					if (valid_count >= (int)(active_map->size() / 2)) {
						found_everything = true;
						break;
					}
				}
			}
			else {
				all_valid = false;
			}

			auto now = std::chrono::steady_clock::now();
			int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - kinStart).count();
			int remaining = cfg.timeout - elapsed;

			if (remaining <= 0) break;
			std::fflush(stdout);

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		if (!found_everything) {
			printf("timed out waiting for kinematics tree\n");
			process::terminate("RobloxPlayerBeta.exe");
			rerolls++;
			continue;
		}

		while (process::is_running("RobloxPlayerBeta.exe")) {
			results weather_data;
			bool frame_valid = true;

			for (auto const& [key, loc] : *active_map) {
				rbx::instance_t target = resolve(realdm, loc.path);
				if (target.address == 0) { frame_valid = false; break; }

				if (loc.type == SourceType::Value) {
					if (key.find("risk") != std::string::npos) {
						weather_data[key] = target.get_string_value();
					}
					else {
						double val = target.get_value_as<double>();
						if (cfg.version == "1.18" && key.find("cape") != std::string::npos) {
							val /= 2.0;
						}
						weather_data[key] = val;
					}
				}
				else if (loc.type == SourceType::AttributeDouble) weather_data[key] = target.get_attribute<double>(loc.attrName);
				else if (loc.type == SourceType::AttributeString) weather_data[key] = target.get_attribute<std::string>(loc.attrName);
			}

			if (frame_valid && !weather_data.empty()) {
				if (cfg.version == "1.18")      Twisted::Legacy18::read(weather_data);
				else if (cfg.version == "1.19") Twisted::Legacy19::read(weather_data);
				else                            Twisted::Main::read(weather_data);

				bool met = false;

				auto get_double = [&](const std::string& key) -> double {
					if (weather_data.find(key) == weather_data.end()) return -1.0;

					if (std::holds_alternative<double>(weather_data.at(key))) {
						return std::get<double>(weather_data.at(key));
					}
					return -1.0;
					};

				double s = get_double("stp");
				double v = get_double("vtp");
				double r = get_double("srh");
				double c = get_double("cape");

				if (cfg.version == "1.18" || cfg.version == "1.19") {
					if (get_double("fcape") >= cfg.cape) met = true;
				}
				else {
				if ((cfg.stp > -1 && s >= cfg.stp) || (cfg.vtp > -1 && v >= cfg.vtp) || (cfg.srh > -1 && r >= cfg.srh) || (cfg.cape > -1 && c >= cfg.cape))
					{
						met = true;
					}
				}

				if (met) { // this is a dupe check and not the actual thing
					std::string currentlink = cfg.priv_links[rerolls % cfg.priv_links.size()];
					double cur_s = get_double("stp");
					double cur_v = get_double("vtp");
					double cur_r = get_double("srh");
					double cur_c = (cfg.version == "1.18" || cfg.version == "1.19") ? get_double("fcape") : get_double("cape");

					if (currentlink == prev.link) {
						if (cur_s == prev.stp && cur_v == prev.vtp && cur_r == prev.srh && cur_c == prev.cape) {
							printf("\ndupe detected\n");
							met = false;
						}
					}
				}

				if (met) { // this is the thing and is not a dupe check
					std::string current_link = cfg.priv_links[rerolls % cfg.priv_links.size()];

					prev.link = current_link;
					prev.stp = get_double("stp");
					prev.vtp = get_double("vtp");
					prev.srh = get_double("srh");
					prev.cape = (cfg.version == "1.18" || cfg.version == "1.19") ? get_double("fcape") : get_double("cape");

					std::uint64_t players_service = resolve(realdm, { "Players" }).address;
					std::uint64_t local_player = memory->read<std::uint64_t>(players_service + Offsets::Player::LocalPlayer);
					std::uint64_t name_ptr = memory->read<std::uint64_t>(local_player + Offsets::Instance::Name);
					std::string roblox_username = memory->read_string(name_ptr);
					std::string full_link = cfg.raw_links[rerolls % cfg.raw_links.size()];

					Discord::post_roll(cfg, weather_data, rerolls, roblox_username, full_link);

					auto success_start = std::chrono::steady_clock::now();
					while (process::is_running("RobloxPlayerBeta.exe")) {
						auto now = std::chrono::steady_clock::now();
						int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - success_start).count();
						int remaining = cfg.success_wait - elapsed;
						if (remaining <= 0) break;
						int mins = remaining / 60;
						int secs = remaining % 60;
						printf("\rfound match, rerolling in: %d:%02d   ", mins, secs);
						std::fflush(stdout);
						std::this_thread::sleep_for(std::chrono::seconds(1));
					}
				}

				process::terminate("RobloxPlayerBeta.exe");
				rerolls++;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				break;
			}
		}
	}
	return 0;
}