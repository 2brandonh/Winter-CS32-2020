#include "support.h"
#include <vector>

using namespace std;

Annealing::Annealing(std::vector<DeliveryRequest>& deliveries) {
	size = deliveries.size();
}

double Annealing::getTemp() {
	return temperature;
}

int Annealing::getMin() {
	return min_temp;
}

void Annealing::delta_T() {
	temperature -= (1 / ((alpha2 * .1 * size * log(alpha * cur_step + 1)))); //as number of data points increases, temp should decrease slower
	cur_step++;
	//cerr << temperature << endl;
	//temperature *= beta;
}

bool Annealing::chance(double old_cost, double new_cost) {
	double prob = exp(k * (old_cost - new_cost) / temperature); //probability for accepting worse swap
	double x = rand() / (RAND_MAX + 1.);
	if (x <= prob)
		return true;
	else
		return false;
}

void Annealing::compute_dist(int node1, int node2, double& old_dist, double& new_dist, vector<DeliveryRequest>& deliveries) {

	//for (int i = 0; i < deliveries.size(); i++) { //computing old crow distance
	//	if (i == deliveries.size() - 1) {
	//		old_dist += distanceEarthMiles(deliveries[0].location, deliveries[i].location);
	//		break;
	//	}
	//	old_dist += distanceEarthMiles(deliveries[i].location, deliveries[i + 1].location);
	//}

	//vector<DeliveryRequest> temp;
	//temp = deliveries;

	//DeliveryRequest tempNode = temp[node1];
	//temp[node1] = temp[node2];
	//temp[node2] = tempNode;

	//for (int i = 0; i < temp.size(); i++) { //computing new crow distance
	//	if (i == temp.size() - 1) {
	//		new_dist += distanceEarthMiles(temp[0].location, temp[i].location);
	//		break;
	//	}
	//	new_dist += distanceEarthMiles(temp[i].location, temp[i + 1].location);
	//}

	//cerr << "STOP";
	//if (abs(node1 - node2) == 1) {
	//	cerr << "BY 1 ERROR ";
	//}

	// TESTING 2 DIFFERENT METHODS
	//double TESTDELTA1 = new_dist - old_dist;
	//new_dist = 0;
	//old_dist = 0;

	// ^ Above code is inefficient, calculates total distance


	// IMPORTANT NOTE

	//while this seems complicated, it is here to make runtime faster
	//instead of checking total distance it only checks adjacent distances to the swapped nodes after each swap

	if (abs(node1 - node2) != 1) {
		if (node1 == deliveries.size() - 1) {
			old_dist += distanceEarthMiles(deliveries[node1].location, deliveries[node1 - 1].location);
			old_dist += distanceEarthMiles(deliveries[node1].location, deliveries[0].location);
			new_dist += distanceEarthMiles(deliveries[node2].location, deliveries[node1 - 1].location);
			new_dist += distanceEarthMiles(deliveries[node2].location, deliveries[0].location);
		}
		else {
			old_dist += distanceEarthMiles(deliveries[node1].location, deliveries[node1 + 1].location);
			old_dist += distanceEarthMiles(deliveries[node1].location, deliveries[node1 - 1].location);
			new_dist += distanceEarthMiles(deliveries[node2].location, deliveries[node1 + 1].location);
			new_dist += distanceEarthMiles(deliveries[node2].location, deliveries[node1 - 1].location);
		}

		if (node2 == deliveries.size() - 1) {
			old_dist += distanceEarthMiles(deliveries[node2].location, deliveries[node2 - 1].location);
			old_dist += distanceEarthMiles(deliveries[node2].location, deliveries[0].location);
			new_dist += distanceEarthMiles(deliveries[node1].location, deliveries[node2 - 1].location);
			new_dist += distanceEarthMiles(deliveries[node1].location, deliveries[0].location);
		}
		else {
			old_dist += distanceEarthMiles(deliveries[node2].location, deliveries[node2 + 1].location);
			old_dist += distanceEarthMiles(deliveries[node2].location, deliveries[node2 - 1].location);
			new_dist += distanceEarthMiles(deliveries[node1].location, deliveries[node2 + 1].location);
			new_dist += distanceEarthMiles(deliveries[node1].location, deliveries[node2 - 1].location);
		}
	}
	else {
		if (node1 < node2) { //special case to consider for two adjacent nodes swapping
			int tempInt = node1;
			node1 = node2;
			node2 = tempInt;
		}
		if (node1 == deliveries.size() - 1) {
			old_dist += distanceEarthMiles(deliveries[node1].location, deliveries[0].location);
			new_dist += distanceEarthMiles(deliveries[node2].location, deliveries[0].location);
		}
		else {
			old_dist += distanceEarthMiles(deliveries[node1].location, deliveries[node1 + 1].location);
			new_dist += distanceEarthMiles(deliveries[node2].location, deliveries[node1 + 1].location);
		}

		old_dist += distanceEarthMiles(deliveries[node2].location, deliveries[node2 - 1].location);
		new_dist += distanceEarthMiles(deliveries[node1].location, deliveries[node2 - 1].location);

	}

	/*double TESTDELTA2 = new_dist - old_dist;
	if (int(TESTDELTA1) != int(TESTDELTA2))
		cerr << TESTDELTA1 << " " << TESTDELTA2 << "         " << node1 << ", " << node2 << endl;*/
	/*if (int(TESTDELTA1) == int(TESTDELTA2) && abs(node1 - node2) == 1)
		cerr << "         STILL SAME " << node1 << ", " << node2 << endl;*/
}