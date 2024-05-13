#pragma once

#include <chrono>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <optional>
#include <mutex>
#include <thread>
#include <functional>

using namespace std::literals;

#define LOG(lgLev, a_msg) C_Log::getInstance().log(lgLev, a_msg)

class C_Log {

public: // types

    enum E_LogLevel {
      lgInfo,
      lgWarning,
      lgError
    };

    enum E_Loggers {
        lgrConsole = 1,
        lgrFile,
        lgrBoth = lgrConsole | lgrFile
    };

public:
    static C_Log& getInstance() {
        static C_Log obj;
        return obj;
    }
    // Формирование даты для имени файла "%Y_%m_%d"
    std::string getFileTimeStamp() const;

    bool setup(E_Loggers a_loggers, const std::string &a_fileName = "");

    // Логирование данных
    void log(E_LogLevel a_lgLev, const std::string &a_msg);

private:
    std::optional<std::chrono::system_clock::time_point> manual_ts_;

    std::ofstream m_logFile;

    std::mutex m_logMutex;

    std::string m_fileName;
    std::string m_fileExt{".log"};
    std::string date;

    std::vector<std::function<void(const std::string &)>> m_logFuncs;

private:

    //template<typename... Ts>
    void logConsole(const std::string & a_msg);

    //template<typename... Ts>
    void logToFile(const std::string & a_msg);

    auto getTime() const {

        return std::chrono::system_clock::now();
    }

    auto getTimeStamp() const {
        const auto now = getTime();
        const auto t_c = std::chrono::system_clock::to_time_t(now);
        return std::put_time(std::gmtime(&t_c), "%F %T");
    }

    C_Log() = default;
    C_Log(const C_Log&) = delete;
};





