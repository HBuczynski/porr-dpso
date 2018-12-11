//
// Created by mariusz on 12.11.18.
//

#ifndef PORR_DPSO_DPSOCONFIG_H
#define PORR_DPSO_DPSOCONFIG_H

#include <string>

struct DPSOConfig {
    int population_cnt = 50;
    int iterations = 20;

    float social_coefficient = 5.0f;
    float cognitive_coefficient = 5.0f;
    float swarm_inertion = 5.0f;
    float previous_pos_impact_coefficient = 5.0f;

    std::string dataset_name = "OL";
    unsigned int number_of_nodes = 30;
};


#endif //PORR_DPSO_DPSOCONFIG_H
