#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include <wininet.h>
#include <vector>
#include <variant>
#include <map>
#include "../roblox/twisted/base.hpp"
#include "config.hpp"

#pragma comment(lib, "wininet.lib")

class Discord {
public:
    static void send_webhook(const std::string& full_url, const std::string& content) {
        HINTERNET hSession = InternetOpenA("WinINet", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hSession) return;

        char host[256], path[1024];
        URL_COMPONENTSA urlComp = { sizeof(urlComp) };
        urlComp.lpszHostName = host;
        urlComp.dwHostNameLength = sizeof(host);
        urlComp.lpszUrlPath = path;
        urlComp.dwUrlPathLength = sizeof(path);

        if (InternetCrackUrlA(full_url.c_str(), 0, 0, &urlComp)) {
            HINTERNET hConnect = InternetConnectA(hSession, host, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
            if (hConnect) {
                const char* szHeaders = "Content-Type: application/json";
                HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", path, NULL, NULL, NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD, 0);

                if (hRequest) {
                    std::string json_payload = "{\"content\": \"" + content + "\"}";

                    HttpSendRequestA(hRequest, szHeaders, (DWORD)strlen(szHeaders), (LPVOID)json_payload.c_str(), (DWORD)json_payload.length());

                    InternetCloseHandle(hRequest);
                }
                InternetCloseHandle(hConnect);
            }
        }
        InternetCloseHandle(hSession);
    }

    static void post_roll(const Config& cfg, results& data, int rerolls, const std::string& username, const std::string& full_link) {
        if (cfg.webhook.empty()) return;

        auto get_str = [&](const std::string& key) -> std::string {
            if (data.find(key) == data.end()) return "N/A";

            if (std::holds_alternative<std::string>(data.at(key))) {
                return std::get<std::string>(data.at(key));
            }

            if (std::holds_alternative<double>(data.at(key))) {
                double val = std::get<double>(data.at(key));

                if (key == "pwat" || key == "composite") {
                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(2) << val;
                    return ss.str();
                }

                return std::to_string((int)val);
            }

            return "N/A";
        };

        std::string body = "(failed to ping)";
        if (cfg.roleid.empty()) {
            body = "@everyone\\n";
        }
        else {
            body = "<@&" + cfg.roleid + ">\\n";
        }
        body += "found server\\n";
        body += "attempt: " + std::to_string(rerolls + 1) + " (" + std::to_string(rerolls) + " rerolls)" "\\n";
        body += "rolled by: " + username + "\\n";

        if (cfg.version == "1.19") {
            body += "```yaml\\n";
            body += "VERSION: TWISTED-1-19-1\\n";
            body += "D1: " + get_str("risk_d1") + ", D2: " + get_str("risk_d2") + ", D3: " + get_str("risk_d3") + "\\n\\n";
            body += "FORECASTED:\\n";
            body += "TEMP: " + get_str("ftemp") + "\\n";
            body += "DEWPOINT: " + get_str("fdew") + "\\n";
            body += "LAPSE RATES: " + get_str("flapse") + "\\n";
            body += "HUMIDITY: " + get_str("fhumid") + "%\\n";
            body += "CAPE: " + get_str("fcape") + "\\n\\n";
            body += "CURRENT:\\n";
            body += "TEMP: " + get_str("ctemp") + "\\n";
            body += "DEWPOINT: " + get_str("cdew") + "\\n";
            body += "LAPSE RATES: " + get_str("clapse") + "\\n";
            body += "HUMIDITY: " + get_str("chumid") + "%\\n";
            body += "CAPE: " + get_str("ccape") + "\\n";
            body += "```\\n";
        }
        else if (cfg.version == "1.18")
        {
            body += "```yaml\\n";
            body += "VERSION: TWISTED-1-18-1\\n";
            body += "CURRENT RISK: " + get_str("risk") + "\\n\\n";
            body += "FORECASTED:\\n";
            body += "TEMP: " + get_str("ftemp") + "\\n";
            body += "DEWPOINT: " + get_str("fdew") + "\\n";
            body += "LAPSE RATES: " + get_str("flapse") + "\\n";
            body += "HUMIDITY: " + get_str("fhumid") + "%\\n";
            body += "CAPE: " + get_str("fcape") + "\\n\\n";
            body += "CURRENT:\\n";
            body += "TEMP: " + get_str("ctemp") + "\\n";
            body += "DEWPOINT: " + get_str("cdew") + "\\n";
            body += "LAPSE RATES: " + get_str("clapse") + "\\n";
            body += "HUMIDITY: " + get_str("chumid") + "%\\n";
            body += "CAPE: " + get_str("ccape") + "\\n";
            body += "```\\n";
        }
        else {
            body += "```yaml\\n";
            body += "VERSION: " + (cfg.version == "1.20" ? std::string("TWISTED-1-20-1") : std::string("TWISTED-MAIN")) + "\\n";
            body += "D1: " + get_str("risk_d1") + " D2: " + get_str("risk_d2") + " D3: " + get_str("risk_d3") + "\\n\\n";
            body += "COMPOSITES:\\n";
            body += "STP/VTP: " + get_str("stp") + "/" + get_str("vtp") + "\\n\\n";
            body += "TEMPERATURES: " + get_str("temp") + " F\\n";
            body += "CAPE: " + get_str("cape") + " J/KG\\n";
            body += "0-3KM LAPSE: " + get_str("lapse") + " C/KM\\n";
            body += "PWAT: " + get_str("pwat") + " IN\\n";
            body += "SURFACE RH: " + get_str("rh") + "%\\n\\n";
            body += "DEWPOINT: " + get_str("dewpoint") + " F\\n";
            body += "3CAPE: " + get_str("3cape") + " J/KG\\n";
            body += "3-6KM LAPSE: " + get_str("mid_lapse") + " C/KM\\n";
            body += "SRH: " + get_str("srh") + " M2S2\\n";
            body += "700-500MB RH " + get_str("mb_rh") + "%\\n\\n";
            body += "WIND SPEEDS: " + get_str("speed") + " MPH\\n";
            body += "STORM MOTION: " + get_str("direction") + "\\n";
            body += "COMPOSITE: " + get_str("composite") + "\\n";
            body += "```\\n";
        }

        body += "join server: " + full_link;

        send_webhook(cfg.webhook, body);
    }
};