#pragma once
#include "util.h"
#include "Daq.h"

namespace mel {

    class Encoder {

    public:        

        // parameters
        const double transmission_ = 0;
        const uint32 counts_per_rev_ = 0;
        //const uint quadrature_factor_ = 0;

        // constructor
        Encoder() {}
        Encoder(double transmission, uint32 counts_per_rev);

        // state variables
        double count_;
        double count_rate_;

        // getters and setters of state variables
        double get_count();
        double get_count_rate();

    };
}