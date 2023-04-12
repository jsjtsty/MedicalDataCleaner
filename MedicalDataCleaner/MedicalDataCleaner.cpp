#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include "MedicalDataParser.h"
#include "MedicalParsedData.h"
#include "util.h"

int main() {
    std::cout << "MedicalDataCleaner - Clean data for dialog diagnosis." << std::endl;
    std::cout << "Version 1.0.3. Copyright (C) NulStudio 2014-2023. All rights reserved." << std::endl;
    std::cout << std::endl;

    std::vector<MedicalDataEntry> parsedData;

    std::cout << "\033[1m* Read File: \033[0m" << std::endl;

    while (true) {
        std::cout << "Input [File Path]: ";
        std::string path;
        std::getline(std::cin, path);
        trim(path);
        if (path[0] == '\"') {
            path.erase(0, 1);
        }
        if (path[path.size() - 1] == '\"') {
            path.erase(path.size() - 1, 1);
        }

        try {
            if (!std::filesystem::exists(path)) {
                throw std::runtime_error("File not exist.");
            }
            parsedData = parseMedicalDataFile(path);
            break;
        }
        catch (...) {
            std::cout << "Error: Cannot parse file \'" << path << "\', please try again." << std::endl << std::endl;
        }
    }

    std::cout << std::endl << "\033[1m* Data Review: \033[0m" << std::endl;

    int i = 1;
    for (const auto& entry : parsedData) {
        auto duration = (int)time(NULL);
        auto parsed = MedicalParsedData::parse(entry);
        std::cout << "Entry [#" << i << "]: " << std::endl;
        std::cout << "\033[1;34m主诉: \033[0m" << parsed.getComplaint() << std::endl;
        std::cout << "\033[1;34m现病史: \033[0m" << parsed.getHistory() << std::endl;
        std::cout << "\033[1;34m既往史: \033[0m" << parsed.getFormer() << std::endl;
        std::cout << "Press enter to continue...";
        getchar();
        duration = (int)time(NULL) - duration;
        std::cout << "Time spent for Entry [#" << i << "]: \033[1;34m";
        int minute = duration / 60, second = duration % 60;
        if (minute > 0) {
            std::cout << minute << "m";
        }
        std::cout << second << "s\033[0m" << std::endl << std::endl;
        ++i;
    }


    return 0;
}

int SelectMenu() {
    std::cout << "* Select Mode: " << std::endl;
    std::cout << "";
    return 0;
}
