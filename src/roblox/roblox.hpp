#pragma once
#include <iostream>
#include <filesystem>
#include <string>
#include <windows.h>
#include <shellapi.h>

#include "offsets.hpp"
#include "twisted/hierarchy.h"

namespace fs = std::filesystem;

namespace roblox {
    class launcher {
    public:
        static bool launch(const std::string& version, const std::string& linkCode) {
            std::string hash = Offsets::ClientVersion;
            fs::path exe_path = fs::current_path() / "roblox" / ("version-" + hash) / "RobloxPlayerBeta.exe";

            if (!fs::exists(exe_path)) {
                return false;
            }

            std::string place_id;
            if (version == "1.22")      place_id = std::to_string(Twisted::Main::PlaceID);
            else if (version == "1.20") place_id = std::to_string(Twisted::Legacy20::PlaceID);
            else if (version == "1.19") place_id = std::to_string(Twisted::Legacy19::PlaceID);
            else if (version == "1.18") place_id = std::to_string(Twisted::Legacy18::PlaceID);
            else return false;

            std::string bobox = "roblox://experiences/start?placeId=" + place_id + "&linkCode=" + linkCode;
            std::string args = "--app --deeplink \"" + bobox + "\"";

            return (INT_PTR)ShellExecuteA(NULL, "open", exe_path.string().c_str(), args.c_str(), NULL, SW_SHOWNORMAL) > 32;
        }
    };

    namespace downloader {
        inline void download(const std::string& hash) {
            fs::path bat_path = fs::current_path() / "bootstrapper.bat";
            if (!fs::exists(bat_path)) {
                std::cerr << "bootstrapper.bat is missing from directory!" << std::endl;
                return;
            }
            ShellExecuteA(NULL, "open", "bootstrapper.bat", hash.c_str(), NULL, SW_SHOW);
        }
    }
}