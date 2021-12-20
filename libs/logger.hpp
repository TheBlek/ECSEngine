#pragma once
#include "gnuplot.h"
#include <vector>

class Logger {
public:

    inline static Gnuplot* plot;
    
    static void ResetFile(const char* filename);

    static void LogDeal(float price);

    static void LogFileAdvanced(const char* filename, const char* format, ...);

    static void LogAdvanced(const char* format, ...);

    template<typename T>
    static void PlotData(T container, const char* plot_name) {
        if (plot == nullptr) {
            plot = new Gnuplot();
        }

        *plot << "plot '-' with lines title '" << plot_name << "'";
        plot->send1d(container);     
    }

    template<typename ...Params>
    static void PlotMultipleData(std::vector<const char*> names, Params&& ... containers) {
        if (plot == nullptr) {
            plot = new Gnuplot();
        }
        *plot << "plot ";
        for (size_t i = 0; i < names.size() - 1; i++)
            *plot << "'-' with lines title '" << names[i] << "', ";
        *plot << "'-' with lines title '" << names[names.size() - 1] << "' \n";

        (plot->send1d(containers), ...);
    }
};