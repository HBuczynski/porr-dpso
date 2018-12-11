//
// Created by mariusz on 12.11.18.
//

#ifndef PORR_DPSO_DPSOCONFIG_H
#define PORR_DPSO_DPSOCONFIG_H

#include <string>

struct DPSOConfig {
    int population_cnt = 5;
    int iterations = 10;

    float social_coefficient = 0.5f;
    float cognitive_coefficient = 0.5f;
    float swarm_inertion = 0.5f;
    float previous_pos_impact_coefficient = 0.5f;

    std::string dataset_name = "OL";
    unsigned int number_of_nodes = 30;

};


#endif //PORR_DPSO_DPSOCONFIG_H
