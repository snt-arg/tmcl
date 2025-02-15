/**
# ##############################################################################
#  Copyright (c) 2021- University of Bonn                                      #
#  All rights reserved.                                                        #
#                                                                              #
#  Author: Nicky Zimmerman                                                     #
#                                                                              #
#  File: LowVarianceResampling.cpp                                             #
# ##############################################################################
**/


#include "LowVarianceResampling.h"

#include <numeric>
#include <functional> 

void LowVarianceResampling::Resample(std::vector<Particle>& particles)
{
	std::vector<Particle> new_particles;
	int n_particles = particles.size();
	std::vector<double> transWeights(n_particles);

	std::transform(particles.begin(), particles.end(), transWeights.begin(), [](Particle &p){ return p.weight * p.weight; });
	double sumWeights = std::accumulate(transWeights.begin(), transWeights.end(), 0.0);
	
	double effN = 1.0 / sumWeights;

	if (effN < 0.5 * n_particles)
	{
		double unitW = 1.0 / n_particles;
		//std::cout << "resample" << std::endl;
		double r = drand48() * 1.0 / n_particles;
		double acc = particles[0].weight;
		int i = 0;

		for(int j = 0; j < n_particles; ++j)
		{
			double U = r + j * 1.0 / n_particles;
			while((U > acc) && (i < n_particles))
			{
				++i;
				acc += particles[i].weight;
			}
			particles[i].weight = unitW;
			new_particles.push_back(particles[i]);
		}
		particles = new_particles;
	}
}