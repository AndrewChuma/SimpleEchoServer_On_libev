#include <ctime>

#include "C_Log.h"

std::string C_Log::getFileTimeStamp() const
{
    auto time = getTime();
    auto t_c = std::chrono::system_clock::to_time_t(time);
    char date[] = "YYYY_MM_DD";
    std::strftime(date, std::size(date), "%Y_%m_%d", std::gmtime(&t_c));

    std::string res_date{date};

    return res_date;

}

bool C_Log::setup(E_Loggers a_loggers, const std::string &a_fileName)
{
    if ( a_loggers & lgrConsole ) {
       std::function<void(const std::string &)> func = std::bind(&C_Log::logConsole, this, std::placeholders::_1);
       m_logFuncs.push_back(func);
    }

    if ( a_loggers & lgrFile ) {
        if (a_fileName.empty() ) {
            return false;
        }
        m_fileName = a_fileName;

        std::function<void(const std::string &)> func = std::bind(&C_Log::logToFile, this, std::placeholders::_1);
        m_logFuncs.push_back(func);
    }
    return true;
}

void C_Log::log(E_LogLevel a_lgLev, const std::string & a_msg )
{
    std::lock_guard lock(m_logMutex);

    std::string lgLevel;
    switch(a_lgLev) {
    case lgInfo:
         lgLevel = "Info: ";
         break;
    case lgError:
         lgLevel = "Error: ";
         break;
    case lgWarning:
         lgLevel = "Warning: ";
    default: lgLevel = "";
    };

    for( const auto &func : m_logFuncs ) {
        func(lgLevel + a_msg);
    }
}

void C_Log::logConsole(const std::string & a_msg)
{
    std::cout << getTimeStamp() << ": " << a_msg << std::endl;
}


void C_Log::logToFile(const std::string & a_msg)
{
    std::string cur_date = getFileTimeStamp();
    if ( date != cur_date ) {
        date = cur_date;

        auto fn = m_fileName+ cur_date + m_fileExt;
        m_logFile.close();
        m_logFile.open(fn, std::ios_base::app);
    }

    m_logFile << getTimeStamp() << ": " << a_msg << '\n';
}
