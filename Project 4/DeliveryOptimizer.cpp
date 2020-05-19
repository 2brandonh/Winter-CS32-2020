#include "provided.h"
#include "support.h"
#include <cmath>
#include <ctime>
#include <math.h>
#include <vector>
using namespace std;

class DeliveryOptimizerImpl
{
public:
	DeliveryOptimizerImpl(const StreetMap* sm);
	~DeliveryOptimizerImpl();
	void optimizeDeliveryOrder(
		const GeoCoord& depot,
		vector<DeliveryRequest>& deliveries,
		double& oldCrowDistance,
		double& newCrowDistance) const;
private:
	const StreetMap* strt_ptr;
};


DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm) : strt_ptr(sm)
{
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
	const GeoCoord& depot,
	vector<DeliveryRequest>& deliveries,
	double& oldCrowDistance,
	double& newCrowDistance) const
{
	srand((unsigned)time(NULL));
	oldCrowDistance = 0;
	newCrowDistance = 0;
	vector<DeliveryRequest> best;

	DeliveryRequest temp_depot("temp_depot", depot);
	deliveries.insert(deliveries.begin(), temp_depot);

	for (int i = 0; i < deliveries.size(); i++) { //computing old crow distance
		if (i == deliveries.size() - 1) {
			oldCrowDistance += distanceEarthMiles(deliveries[0].location, deliveries[i].location);
			break;
		}
		oldCrowDistance += distanceEarthMiles(deliveries[i].location, deliveries[i + 1].location);
	}

	//Now the fun part, SIMULATED ANNEALING! /////////////////////////////////

	Annealing anneal(deliveries);

	while (anneal.getTemp() > anneal.getMin()) {
		int node1 = rand() % deliveries.size(); 
		if (node1 == 0) node1++; //so as to never reach 0, the temp depot
		int node2 = rand() % deliveries.size();
		if (node2 == 0) node2++;

		if (node1 == node2 && deliveries.size() != 2) //need to not enter infinite loop with only one delivery item
			continue;

		double old_dist = 0;
		double new_dist = 0;

		anneal.compute_dist(node1, node2, old_dist, new_dist, deliveries);

		anneal.delta_T();

		if (new_dist > old_dist) { //if new distance is worse, deterministic probability to continue on and update deliveries
			if (anneal.chance(old_dist, new_dist) == false)
				continue;
		}

		DeliveryRequest temp = deliveries[node1];
		deliveries[node1] = deliveries[node2];
		deliveries[node2] = temp;

		if (new_dist <= old_dist)
			best = deliveries; //keep track of the optimal one
	}

	deliveries = best;

	for (int i = 0; i < deliveries.size(); i++) { //computing new crow distance
		if (i == deliveries.size() - 1) {
			newCrowDistance += distanceEarthMiles(deliveries[0].location, deliveries[i].location);
			break;
		}
		newCrowDistance += distanceEarthMiles(deliveries[i].location, deliveries[i + 1].location);
	}

	deliveries.erase(deliveries.begin()); //remove the temp depot
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
	m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
	delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
	const GeoCoord& depot,
	vector<DeliveryRequest>& deliveries,
	double& oldCrowDistance,
	double& newCrowDistance) const
{
	return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
