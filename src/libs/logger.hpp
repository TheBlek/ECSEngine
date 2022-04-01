#pragma once
#include "gnuplot.h"
#include <array>

namespace Logger {

    static Gnuplot *plot = new Gnuplot();
    
    void ResetFile(const char* filename);

    void LogDeal(float price);

    void LogFileAdvanced(const char* filename, const char* format, ...);

    void LogAdvanced(const char* format, ...);

    template<typename T>
    void Log(T value) {
        std::cout << value << std::endl;
    }

    template<typename T>
    void PlotData(T container, const char* plot_name) {
        *plot << "plot '-' with lines title '" << plot_name << "' \n";
        plot->send1d(container); 
    }

    template<typename ...Params>
   	void PlotMultipleData(const std::array<const char*, sizeof...(Params)> &names, Params && ... containers) {
        *plot << "plot ";
        for (size_t i = 0; i < names.size() - 1; i++)
            *plot << "'-' with lines title '" << names[i] << "', ";
        *plot << "'-' with lines title '" << names[names.size() - 1] << "' \n";

        (plot->send1d(containers), ...);
    }
};
