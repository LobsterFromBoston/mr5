#pragma once
#include "base.hpp"
#include <iostream>
#include <iomanip>

#define ll long long

namespace Twisted {
    struct LastRoll {
        std::string link = "";
        double stp = -1;
        double vtp = -1;
        double srh = -1;
        double cape = -1;
    };

    namespace Main {
        inline ll PlaceID = 14170731342;

        inline std::unordered_map<std::string, locations> locationz{
            {"stp",        {{"ReplicatedStorage", "kinematics", "stp"}, SourceType::Value}},
            {"vtp",        {{"ReplicatedStorage", "kinematics", "vtp"}, SourceType::Value}},
            {"temp",       {{"ReplicatedStorage", "kinematics", "temp"}, SourceType::Value}},
            {"cape",       {{"ReplicatedStorage", "kinematics", "cape"}, SourceType::Value}},
            {"lapse",      {{"ReplicatedStorage", "kinematics", "lapse"}, SourceType::Value}},
            {"pwat",       {{"ReplicatedStorage", "kinematics", "pwat"}, SourceType::Value}},
            {"rh",         {{"ReplicatedStorage", "kinematics", "rh"}, SourceType::Value}},
            {"dewpoint",   {{"ReplicatedStorage", "kinematics", "dewpoint"}, SourceType::Value}},
            {"3cape",      {{"ReplicatedStorage", "kinematics", "3cape"}, SourceType::Value}},
            {"mid_lapse",  {{"ReplicatedStorage", "kinematics", "mid_lapse"}, SourceType::Value}},
            {"srh",        {{"ReplicatedStorage", "kinematics", "srh"}, SourceType::Value}},
            {"mb_rh",      {{"ReplicatedStorage", "kinematics", "mb_rh"}, SourceType::Value}},
            {"composite",  {{"ReplicatedStorage", "kinematics", "composite"}, SourceType::Value}},
            {"risk_d1",    {{"ReplicatedStorage", "kinematics", "risk"}, SourceType::Value}},

            {"speed",      {{"ReplicatedStorage", "kinematics", "motion"}, SourceType::AttributeDouble, "speed"}},
            {"direction",  {{"ReplicatedStorage", "kinematics", "motion"}, SourceType::AttributeString, "direction"}},
            {"risk_d2",    {{"ReplicatedStorage", "kinematics", "risk"}, SourceType::AttributeString, "Day_2"}},
            {"risk_d3",    {{"ReplicatedStorage", "kinematics", "risk"}, SourceType::AttributeString, "Day_3"}}
        };

        inline void read(results& data) {
            auto get_dbl = [&](const std::string& key) { return (data.count(key)) ? std::get<double>(data[key]) : 0.0; };
            auto get_str = [&](const std::string& key) { return (data.count(key)) ? std::get<std::string>(data[key]) : "N/A"; };
;
            std::cout << "D1: " << get_str("risk_d1") << " D2: " << get_str("risk_d2") << " D3: " << get_str("risk_d3") << std::endl << std::endl;
            std::cout << "STP/VTP: " << get_dbl("stp") << "/" << get_dbl("vtp") << std::endl;
            std::cout << "CAPE: " << get_dbl("cape") << " J/KG" << std::endl;
            std::cout << "0-3KM LAPSE: " << get_dbl("lapse") << " C/KM" << std::endl;
            std::cout << "PWAT: " << get_dbl("pwat") << " IN" << std::endl;
            std::cout << "SURFACE RH: " << get_dbl("rh") << "%" << std::endl << std::endl;
            std::cout << "DEWPOINT: " << get_dbl("dewpoint") << " F" << std::endl;
            std::cout << "3CAPE: " << get_dbl("3cape") << " J/KG" << std::endl;
            std::cout << "3-6KM LAPSE: " << get_dbl("mid_lapse") << " C/KM" << std::endl;
            std::cout << "SRH: " << get_dbl("srh") << " M2S2" << std::endl;
            std::cout << "700-500MB RH " << get_dbl("mb_rh") << "%" << std::endl << std::endl;
            std::cout << "WIND SPEEDS: " << get_dbl("speed") << " MPH" << std::endl;
            std::cout << "STORM MOTION: " << get_str("direction") << std::endl;
            std::cout << "COMPOSITE: " << get_dbl("composite") << std::endl;
        }
    }

    namespace Legacy20 {
        inline int ll PlaceID = 123866795811080;

        inline std::unordered_map<std::string, locations> locationz{
            {"stp",        {{"ReplicatedStorage", "kinematics", "stp"}, SourceType::Value}},
            {"vtp",        {{"ReplicatedStorage", "kinematics", "vtp"}, SourceType::Value}},
            {"temp",       {{"ReplicatedStorage", "kinematics", "temp"}, SourceType::Value}},
            {"cape",       {{"ReplicatedStorage", "kinematics", "cape"}, SourceType::Value}},
            {"lapse",      {{"ReplicatedStorage", "kinematics", "lapse"}, SourceType::Value}},
            {"pwat",       {{"ReplicatedStorage", "kinematics", "pwat"}, SourceType::Value}},
            {"rh",         {{"ReplicatedStorage", "kinematics", "rh"}, SourceType::Value}},
            {"dewpoint",   {{"ReplicatedStorage", "kinematics", "dewpoint"}, SourceType::Value}},
            {"3cape",      {{"ReplicatedStorage", "kinematics", "3cape"}, SourceType::Value}},
            {"mid_lapse",  {{"ReplicatedStorage", "kinematics", "mid_lapse"}, SourceType::Value}},
            {"srh",        {{"ReplicatedStorage", "kinematics", "srh"}, SourceType::Value}},
            {"mb_rh",      {{"ReplicatedStorage", "kinematics", "mb_rh"}, SourceType::Value}},
            {"composite",  {{"ReplicatedStorage", "kinematics", "composite"}, SourceType::Value}},
            {"risk_d1",    {{"ReplicatedStorage", "kinematics", "risk"}, SourceType::Value}},
            {"speed",      {{"ReplicatedStorage", "kinematics", "motion"}, SourceType::AttributeDouble, "speed"}},
            {"direction",  {{"ReplicatedStorage", "kinematics", "motion"}, SourceType::AttributeString, "direction"}},
            {"risk_d2",    {{"ReplicatedStorage", "kinematics", "risk"}, SourceType::AttributeString, "Day_2"}},
            {"risk_d3",    {{"ReplicatedStorage", "kinematics", "risk"}, SourceType::AttributeString, "Day_3"}}
        };

        inline void read(results& data) {
            Twisted::Main::read(data);
        }
    }

    namespace Legacy19 {
        inline int ll PlaceID = 78568440332100;

        inline std::unordered_map<std::string, locations> locationz{
            {"risk_d1", {{"ReplicatedStorage", "kinematics", "risk"}, SourceType::Value}},
            {"risk_d2", {{"ReplicatedStorage", "kinematics", "risk", "day_2"}, SourceType::Value}},
            {"risk_d3", {{"ReplicatedStorage", "kinematics", "risk", "day_3"}, SourceType::Value}},

            {"flapse", {{"ReplicatedStorage", "kinematics", "forecasted", "lapse"}, SourceType::Value}},
            {"fdew", {{"ReplicatedStorage", "kinematics", "forecasted", "dewpoint"}, SourceType::Value}},
            {"ftemp", {{"ReplicatedStorage", "kinematics", "forecasted", "temperature"}, SourceType::Value}},
            {"fcape", {{"ReplicatedStorage", "kinematics", "forecasted", "cape"}, SourceType::Value}},
            {"fhumid", {{"ReplicatedStorage", "kinematics", "forecasted", "humidity"}, SourceType::Value}},

            {"clapse", {{"ReplicatedStorage", "kinematics", "current", "lapse"}, SourceType::Value}},
            {"cdew", {{"ReplicatedStorage", "kinematics", "current", "dewpoint"}, SourceType::Value}},
            {"ctemp", {{"ReplicatedStorage", "kinematics", "current", "temperature"}, SourceType::Value}},
            {"ccape", {{"ReplicatedStorage", "kinematics", "current", "cape"}, SourceType::Value}},
            {"chumid", {{"ReplicatedStorage", "kinematics", "current", "humidity"}, SourceType::Value}},
        };

        inline void read(results& data) {
            auto get_dbl = [&](const std::string& key) { return (data.count(key)) ? std::get<double>(data[key]) : 0.0; };
            auto get_str = [&](const std::string& key) { return (data.count(key)) ? std::get<std::string>(data[key]) : "N/A"; };

            std::cout << "DAY 1: " << get_str("risk_d1") << ", DAY 2: " << get_str("risk_d2") << ", DAY 3: " << get_str("risk_d3") << std::endl << std::endl;

            std::cout << "TEMPERATURE: " << get_dbl("ftemp") << " F" << std::endl;
            std::cout << "DEWPOINT: " << get_dbl("fdew") << " F" << std::endl;
            std::cout << "CAPE: " << get_dbl("fcape") << " J/KG" << std::endl;
            std::cout << "LAPSE: " << get_dbl("flapse") << " C/KM" << std::endl;
            std::cout << "HUMIDITY: " << get_dbl("fhumid") << "%" << std::endl;
        }
    }
    namespace Legacy18 {
        inline ll PlaceID = 127382198965278;

        inline std::unordered_map<std::string, locations> locationz{
            {"risk", {{"ReplicatedStorage", "kinematics", "risk"}, SourceType::Value}},

            {"flapse", {{"ReplicatedStorage", "kinematics", "forecasted", "lapse"}, SourceType::Value}},
            {"fdew", {{"ReplicatedStorage", "kinematics", "forecasted", "dewpoint"}, SourceType::Value}},
            {"ftemp", {{"ReplicatedStorage", "kinematics", "forecasted", "temperature"}, SourceType::Value}},
            {"fcape", {{"ReplicatedStorage", "kinematics", "forecasted", "cape"}, SourceType::Value}},
            {"fhumid", {{"ReplicatedStorage", "kinematics", "forecasted", "humidity"}, SourceType::Value}},

            {"clapse", {{"ReplicatedStorage", "kinematics", "current", "lapse"}, SourceType::Value}},
            {"cdew", {{"ReplicatedStorage", "kinematics", "current", "dewpoint"}, SourceType::Value}},
            {"ctemp", {{"ReplicatedStorage", "kinematics", "current", "temperature"}, SourceType::Value}},
            {"ccape", {{"ReplicatedStorage", "kinematics", "current", "cape"}, SourceType::Value}},
            {"chumid", {{"ReplicatedStorage", "kinematics", "current", "humidity"}, SourceType::Value}},
        };

        inline void read(results& data) {
            auto get_dbl = [&](const std::string& key) { return (data.count(key)) ? std::get<double>(data[key]) : 0.0; };
            auto get_str = [&](const std::string& key) { return (data.count(key)) ? std::get<std::string>(data[key]) : "N/A"; };

            std::cout << "CURRENT RISK: " << get_str("risk") << std::endl << std::endl;
            std::cout << "TEMPERATURES: " << get_dbl("ftemp") << " F" << std::endl;
            std::cout << "DEWPOINT: " << get_dbl("fdew") << " F" << std::endl;
            std::cout << "CAPE: " << get_dbl("fcape") << " J/KG" << std::endl;
            std::cout << "LAPSE: " << get_dbl("flapse") << " C/KM" << std::endl;
            std::cout << "HUMIDITY: " << get_dbl("fhumid") << "%" << std::endl << std::endl;
        }
    }
}