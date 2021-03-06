/* Chemfiles, an efficient IO library for chemistry file formats
* Copyright (C) 2015 Guillaume Fraux
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/
*/

/*! @file Logger.hpp
* Log utilities and configuration
*/

#ifndef CHEMFILES_LOGGING_H
#define CHEMFILES_LOGGING_H

#include <string>
#include <ostream>

#include "chemfiles/exports.hpp"

namespace chemfiles {

/*!
 * @class Logger Logger.hpp Logger.cpp
 * @brief The Logger class is a singleton class providing logging facilities.
 */
class CHFL_EXPORT Logger {
public:
    //! The \c LogLevel control what will be logged and what will be dismissed
    enum LogLevel{
        //! No logging at all
        NONE = 0,
        //! Logging only errors
        ERROR = 1,
        //! Logging errors and warnings
        WARNING = 2,
        //! Logging errors, warnings and informations
        INFO = 3,
        //! Logging everything and debug informations
        DEBUG = 4
    };

    ~Logger();

    //! Set the logging level
    static void level(LogLevel);
    //! Get the current logging level
    static LogLevel level() {return instance.current_level;}
    //! Set the file for logging
    static void log_to_file(const std::string &filename);
    //! Make the logger output to stdout
    static void log_to_stdout();
    //! Make the logger output to stderr
    static void log_to_stderr();
    //! Make the logger output to stdlog
    static void log_to_stdlog();

    //! Get the singleton out stream
    static std::ostream& out(LogLevel level);

private:
    //! Close the log file if it exists.
    void close();
    //! Return a stream to write the log
    std::ostream& get_stream(LogLevel level);
    //! Constructor
    Logger();

    //! Copy/move is not permited
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    //! Afectation is not permited
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    //! Singleton instance
    static Logger instance;

    //! Logging level
    LogLevel current_level;
    //! Current log stream
    std::ostream* os; // A raw pointer is needed to hold reference to the standard streams
    //! Is the current stream a file ?
    bool is_file;
};

} // namespace chemfiles

#ifndef CHEMFILES_PUBLIC
    //! The LOG macro should be used to get a stream with the good logging level
    #define LOG(level) chemfiles::Logger::out(chemfiles::Logger::level)
#endif // CHEMFILES_PUBLIC

#endif
