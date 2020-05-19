#include "provided.h"
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
	DeliveryPlannerImpl(const StreetMap* sm);
	~DeliveryPlannerImpl();
	DeliveryResult generateDeliveryPlan(
		const GeoCoord& depot,
		const vector<DeliveryRequest>& deliveries,
		vector<DeliveryCommand>& commands,
		double& totalDistanceTravelled) const;
private:
	const StreetMap* sm_ptr;
};

string computeDirection(double a) {
	if (a < 22.5)
		return "east";
	else if (a < 67.5)
		return "northeast";
	else if (a < 112.5)
		return "north";
	else if (a < 157.5)
		return "northwest";
	else if (a < 202.5)
		return "west";
	else if (a < 247.5)
		return "southwest";
	else if (a < 292.5)
		return "south";
	else if (a < 337.5)
		return "southeast";
	else
		return "east";
}

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm) :sm_ptr(sm)
{
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
	const GeoCoord& depot,
	const vector<DeliveryRequest>& deliveries,
	vector<DeliveryCommand>& commands,
	double& totalDistanceTravelled) const
{
	DeliveryOptimizer optimizer(sm_ptr);
	double old_dist, new_dist;
	vector<DeliveryRequest> efficient_deliveries = deliveries;

	commands.clear();
	totalDistanceTravelled = 0;

	optimizer.optimizeDeliveryOrder(depot, efficient_deliveries, old_dist, new_dist); //reorders deliveries

	/*
	Proceed on a street: Proceed forward on a street of a given name for a given distance
	(in miles)
	• Turn on a street: Turn left or right on a street of a given name
	• Deliver an item: Deliver a given item at the current spot
	*/

	efficient_deliveries.insert(efficient_deliveries.begin(), DeliveryRequest("temp_depot", depot)); //pushing to front a temporary depot to make directions from
	PointToPointRouter router(sm_ptr);
	//double tot_dist = 0;
	string cur_street = "";
	double tot_dist = 0;
	double angle = 0;
	DeliveryCommand CMD; //command

	for (int i = 0; i < efficient_deliveries.size(); i++) { //starting at depot
		double cur_dist = 0;

		GeoCoord start = efficient_deliveries[i].location; //start is current
		GeoCoord end;
		if (i != efficient_deliveries.size() - 1)
			end = efficient_deliveries[i + 1].location;
		else
			end = efficient_deliveries[0].location; //if at end, find distance back to depot

		list<StreetSegment> segments;
		double dist;
		if (router.generatePointToPointRoute(start, end, segments, dist) != DELIVERY_SUCCESS)
			return router.generatePointToPointRoute(start, end, segments, dist);
		tot_dist += dist;

		for (list<StreetSegment>::iterator j = segments.begin(); j != segments.end(); j++) {

			if (j == segments.begin()) {
				cur_dist = distanceEarthMiles(j->start, j->end); //reset cur_dist as starting segment
				cur_street = j->name;
				angle = angleOfLine(*j);
			}

			else if (cur_street != j->name) { //means that a turn has been made
				//PUSH BACK
				CMD.initAsProceedCommand(computeDirection(angle), cur_street, cur_dist);
				commands.push_back(CMD);

				list<StreetSegment>::iterator k = --j;
				j++;
				cur_dist = distanceEarthMiles(j->start, j->end); //reset new street
				cur_street = j->name;
				angle = angleBetween2Lines(*k, *j);

				string turn = "no turn";
				if (angle >= 1 && angle < 180)
					turn = "left";
				if (angle >= 180 && angle <= 359)
					turn = "right";
				if (turn != "no turn") {
					CMD.initAsTurnCommand(turn, cur_street);
					commands.push_back(CMD);
				}
				angle = angleOfLine(*j);
			}

			else {
				cur_dist += distanceEarthMiles(j->start, j->end);
			}
		}

		if (cur_dist != 0) { // Should not print if delivery on same node
			CMD.initAsProceedCommand(computeDirection(angle), cur_street, cur_dist);
			commands.push_back(CMD);
		}

		if (i + 1 != efficient_deliveries.size()) { //acounting for temp depot
			CMD.initAsDeliverCommand(efficient_deliveries[i + 1].item);
			commands.push_back(CMD);
		}

	}
	totalDistanceTravelled = tot_dist;
	return DELIVERY_SUCCESS;
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
	m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
	delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
	const GeoCoord& depot,
	const vector<DeliveryRequest>& deliveries,
	vector<DeliveryCommand>& commands,
	double& totalDistanceTravelled) const
{
	return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
