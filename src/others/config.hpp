#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

static std::string strip_link(const std::string& link) {
    std::string key = "privateServerLinkCode=";
    size_t pos = link.find(key);
    if (pos != std::string::npos) {
        return link.substr(pos + key.length());
    }
    return link;
}

std::string deobfuscate(const std::vector<uint8_t>& data, const std::vector<uint8_t>& key) {
    std::string out;
    for (size_t i = 0; i < data.size(); i++)
        out += (char)(data[i] ^ key[i % key.size()]);
    return out;
}

std::vector<uint8_t> key = { 0x5A, 0x13, 0xC7, 0x9F };
std::vector<uint8_t> enc = {
    0x32, 0x67, 0xB3, 0xEF, 0x29, 0x29, 0xE8, 0xB0, 0x3E, 0x7A, 0xB4, 0xFC,
    0x35, 0x61, 0xA3, 0xB1, 0x39, 0x7C, 0xAA, 0xB0, 0x3B, 0x63, 0xAE, 0xB0,
    0x2D, 0x76, 0xA5, 0xF7, 0x35, 0x7C, 0xAC, 0xEC, 0x75, 0x22, 0xF2, 0xAD,
    0x69, 0x25, 0xFF, 0xAD, 0x6A, 0x25, 0xFE, 0xA7, 0x63, 0x2A, 0xF1, 0xAB,
    0x6B, 0x23, 0xF5, 0xAB, 0x75, 0x7C, 0x9D, 0xF5, 0x0B, 0x54, 0xA3, 0xCC,
    0x10, 0x75, 0x8E, 0xF1, 0x30, 0x61, 0xA1, 0xA7, 0x0A, 0x7B, 0x8F, 0xF7,
    0x32, 0x6A, 0xA1, 0xE5, 0x2E, 0x5B, 0xBE, 0xAB, 0x3E, 0x59, 0xEA, 0xA8,
    0x6E, 0x2B, 0xEA, 0xD0, 0x20, 0x74, 0x80, 0xCC, 0x20, 0x44, 0x91, 0xAD,
    0x3C, 0x5A, 0x9E, 0xF8, 0x05, 0x74, 0x95, 0xD5, 0x22, 0x58, 0xA3, 0xDC,
    0x13, 0x72, 0xA4, 0xD4, 0x29, 0x5E, 0x9D, 0xE7, 0x1B, 0x45, 0x95, 0xF6, 0x37
}; // in case u dont know this is just the discord webhook encrypted because idk

class Config {
public:
    std::string version;
    std::string webhook;
    std::string roleid;

    std::vector<std::string> priv_links;
    std::vector<std::string> raw_links;

    int stp;
    int vtp;
    int srh;
    int cape;
    int timeout;
    int success_wait;

    bool shared;

    bool load(const std::string& filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            ordered_json default_cfg;
            default_cfg["version"] = "1.22";
            default_cfg["priv_link_1"] = "";
            default_cfg["webhook"] = "";
            default_cfg["roleid"] = "";
            default_cfg["shared"] = false;
            default_cfg["stp"] = 1;
            default_cfg["vtp"] = 1;
            default_cfg["srh"] = 1;
            default_cfg["cape"] = 1;
            default_cfg["timeout"] = 30;
            default_cfg["success_wait"] = 300;

            std::ofstream out(filename);
            if (out.is_open()) {
                out << default_cfg.dump(4);
                out.close();
                printf("created config file, please fill up: %s\n", filename.c_str());
            }
            else {
                printf("failed to write config file, please try running as administrator\n");
                std::cin.get();
                return false;
            }

            return false;
        }

        try {
            json j;
            file >> j;

            version = j.value("version", "1.22");
            roleid = j.value("roleid", "");
            stp = j.value("stp", 0);
            vtp = j.value("vtp", 0);
            srh = j.value("srh", 0);
            cape = j.value("cape", 0);
            timeout = j.value("timeout", 30);
            success_wait = j.value("success_wait", 300);
            shared = j.value("shared", false);

            if (shared) {
                
                roleid = "1464539393858474056";
                webhook = "https://discord.com/api/webhooks/676767/hi"; //fake webhook that will appear in notepad okbye
                webhook = deobfuscate(enc, key);
            }
            else {
                webhook = j.value("webhook", "");
            }

            priv_links.clear();
            raw_links.clear();

            for (auto& [key, value] : j.items()) {
                if (key.find("priv_link_") == 0 && value.is_string()) {
                    std::string raw = value.get<std::string>();
                    if (!raw.empty()) {
                        raw_links.push_back(raw);
                        priv_links.push_back(strip_link(raw));
                    }
                }
            }

            std::string error_msg = "";
            if (webhook.empty()) error_msg += "- webhook url is missing\n";
            if (priv_links.empty()) error_msg += "- vip links not found (priv_link_1, etc.)\n";

            if (!error_msg.empty()) {
                std::string full_msg = "config.json is incomplete:\n\n" + error_msg + "\nplease fix these issues and restart.";
                printf("%s\n", full_msg.c_str());
                std::cin.get();
                return false;
            }
        }
        catch (const std::exception& e) {
            printf("json parse error: %s\n", e.what());
            return false;
        }

        return true;
    }
};
