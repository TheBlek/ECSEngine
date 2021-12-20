#include "logger.hpp"

#include <stdio.h>
#include <stdarg.h>
#include <fstream>

void Logger::ResetFile(const char* filename) {
    std::ofstream file(filename);
    file << std::endl;
    file.close();
}

void Logger::LogDeal(float price) {
    Logger::LogFileAdvanced("output/data", "%f\n", price);
    //Logger::LogAdvanced("A deal was made on price %f\n", price );
}

void Logger::LogFileAdvanced(const char* filename, const char* format ...) {

    FILE * file = std::fopen(filename, "a");

    va_list va_args;
    va_start(va_args, format);

    vfprintf(file, format, va_args);

    va_end(va_args);

    fclose(file);
}

void Logger::LogAdvanced(const char* format, ...)  {

    va_list va_args;
    va_start(va_args, format);

    vprintf(format, va_args);

    va_end(va_args);

}