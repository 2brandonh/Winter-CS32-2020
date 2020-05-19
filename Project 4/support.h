#ifndef SUPPORT_H
#define SUPPORT_H
#include "provided.h"
#include <vector>

class Annealing {
public:
	Annealing(std::vector<DeliveryRequest>& deliveries);
	double getTemp();
	int getMin();
	void delta_T();
	bool chance(double old_cost, double new_cost);
	void compute_dist(int node1, int node2, double& old_dist, double& new_dist, std::vector<DeliveryRequest>& deliveries);
private:
	const double beta = 0.8; //if using const for temp change
	const double alpha = 1; 
	const double alpha2 = 1;
	const double k = 557.9; //k is -T0 * ln(0.8) //55.7 //111.4, constant for cooling
	int cur_step = 1;
	double temperature = 2500;
	const int min_temp = 0;
	int size;
};

#endif