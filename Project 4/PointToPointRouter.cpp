#include "provided.h"
#include "ExpandableHashMap.h"
#include <list>
#include <set>
#include <unordered_map>
#include <queue>

using namespace std;

class PointToPointRouterImpl
{
public:
	PointToPointRouterImpl(const StreetMap* sm);
	~PointToPointRouterImpl();
	DeliveryResult generatePointToPointRoute(
		const GeoCoord& start,
		const GeoCoord& end,
		list<StreetSegment>& route,
		double& totalDistanceTravelled) const;
private:
	const StreetMap* str_ptr;

	struct Node {
		/*Node(double in_gCost, double in_hCost, GeoCoord in_parent, const GeoCoord in_coord) :
			gCost(in_gCost), hCost(in_hCost), parent_coord(in_parent), coord(in_coord)
		{};*/

		double gCost;
		double hCost;
		GeoCoord parent_coord;
		GeoCoord coord;
		bool inOpenSet = true;

		const double fCost() {
			return gCost + hCost;
		}
	};

	struct myComparator { //used for the priority queue to rank cost function of F costs
		bool operator() (Node& p1, Node& p2)
		{
			//return true / false to order
			if (p1.fCost() == p2.fCost()) {
				return p1.hCost >= p2.hCost;
			}

			return p1.fCost() > p2.fCost();
		}
	};
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm) : str_ptr(sm)
{
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
	const GeoCoord& start,
	const GeoCoord& end,
	list<StreetSegment>& route,
	double& totalDistanceTravelled) const

{
	vector<StreetSegment> segments;
	route.clear();
	totalDistanceTravelled = 0;

	if (!str_ptr->getSegmentsThatStartWith(end, segments) || !str_ptr->getSegmentsThatStartWith(start, segments))
		return BAD_COORD;
	//if not a bad coord, segments will start at the starting coord

	if (start == end) {
		return DELIVERY_SUCCESS;
	}

	/////////////////////////////////////////////////////////////////////////////////

	priority_queue <Node, vector<Node>, myComparator> open_queue; //sorted based on lowest f cost
	ExpandableHashMap<GeoCoord, Node> open_hash;

	////unordered_map<GeoCoord, Node, customHash> open_hash;

	////ExpandableHashMap<GeoCoord, Node> closed_hash; //maps GeoCoords that are closed to Nodes, used for finding
	////priority_queue <Node, vector<Node>, myComparator> closed_queue;



	Node startNode{ 0, distanceEarthMiles(start, end), start, start }; //starting node
	open_hash.associate(start, startNode);
	open_queue.push(startNode);
	Node current;

	for (;;) {
		if (open_queue.empty())
			return NO_ROUTE;

		//GeoCoord cur_parent = open_queue.top().parent_coord;
		//GeoCoord cur_coordinate = open_queue.top().coord;
		//double cur_h = open_queue.top().hCost;
		//double cur_g = open_queue.top().gCost;
		//Node current{ cur_g, cur_h, cur_parent, cur_coordinate };

		current = open_queue.top();

		open_queue.pop();
		current.inOpenSet = false;
		open_hash.associate(current.coord, current);

		if (current.coord == end) //current equal to target node
			break;

		str_ptr->getSegmentsThatStartWith(current.coord, segments); //getting neighbors

		for (int i = 0; i < segments.size(); i++) { //for each neighbor of current node
			Node* open_ptr = open_hash.find(segments[i].end);
		
			if (open_ptr == nullptr) { //not in open_hash
				GeoCoord parentCOORD = current.coord;
				GeoCoord newCOORD = segments[i].end;
				double newHCOST = distanceEarthMiles(newCOORD, end);
				double newGCOST = current.gCost + distanceEarthMiles(newCOORD, current.coord);
				Node newNode{ newGCOST, newHCOST, parentCOORD,  newCOORD };

				open_queue.push(newNode);
				open_hash.associate(newCOORD, newNode);
				continue;
			}

			double temp_gScore; //for case where already in open_hash
			temp_gScore = current.gCost + distanceEarthMiles(open_ptr->coord, current.coord);
			if (temp_gScore < open_ptr->gCost) {
				open_ptr->parent_coord = current.coord;
				open_ptr->gCost = temp_gScore;
				if (open_ptr->inOpenSet == false) { //if cheaper, put it back in the open_queue to be processed again
					open_ptr->inOpenSet = true;
					open_queue.push(*open_ptr);
				}
			}
		}
	}


	while (current.coord != start) {

		StreetSegment toAdd;
		toAdd.end = current.coord;
		toAdd.start = current.parent_coord;

		string name;
		str_ptr->getSegmentsThatStartWith(current.coord, segments); //backtracing
		for (int i = 0; i < segments.size(); i++) {
			if (segments[i].end == current.parent_coord) {
				name = segments[i].name;
				totalDistanceTravelled += distanceEarthMiles(segments[i].start, segments[i].end);
				break;
			}
		}

		toAdd.name = name;
		route.push_front(toAdd);
		
		current = *(open_hash.find(current.parent_coord)); //getting node of parent to repeat process
	}

	return DELIVERY_SUCCESS;
}

	//******************** PointToPointRouter functions ***************************

	// These functions simply delegate to PointToPointRouterImpl's functions.
	// You probably don't want to change any of this code.

	PointToPointRouter::PointToPointRouter(const StreetMap * sm)
	{
		m_impl = new PointToPointRouterImpl(sm);
	}

	PointToPointRouter::~PointToPointRouter()
	{
		delete m_impl;
	}

	DeliveryResult PointToPointRouter::generatePointToPointRoute(
		const GeoCoord & start,
		const GeoCoord & end,
		list<StreetSegment> & route,
		double& totalDistanceTravelled) const
	{
		return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
	}
