#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
using namespace std;

//It may use the STL vector, list, stack, queue, and priority_queue classes if you wish.

unsigned int hasher(const GeoCoord& g)
{
	return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
	StreetMapImpl();
	~StreetMapImpl();
	bool load(string mapFile);
	bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
	ExpandableHashMap<GeoCoord, vector<StreetSegment>> streetSegs;
	//struct Data {
	//	GeoCoord start;
	//	GeoCoord end;
	//	string name;
	//};
};

StreetMapImpl::StreetMapImpl()
{

}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
	ifstream infile(mapFile);
	if (!infile)		        // Did opening the file fail?
		return false;

	string street_name;
	int num_streets;

	while (getline(infile, street_name)) { //getting the street name
		infile >> num_streets;
		infile.ignore(10000, '\n'); //ignoring everything else on that line

		for (int i = 0; i < num_streets; i++) {
			string coord1, coord2, coord3, coord4;
			infile >> coord1;
			infile >> coord2;
			infile >> coord3;
			infile >> coord4;

			GeoCoord a(coord1, coord2);
			GeoCoord b(coord3, coord4);

			StreetSegment forward{ a, b, street_name }; //keeping track of forward and reverse endings / names in a struct
			StreetSegment reverse{ b, a, street_name };
			//Data reverse{ b, a, street_name };

			//forward_vec.push_back(forward);
			//reverse_vec.push_back(reverse);

		vector<StreetSegment>* ptr = streetSegs.find(forward.start);
		if (ptr == nullptr) {
			vector<StreetSegment> tempAdd;
			tempAdd.push_back(forward);
			streetSegs.associate(forward.start, tempAdd);
		}
		else
			ptr->push_back(forward);

		ptr = streetSegs.find(reverse.start);
		if (ptr == nullptr) {
			vector<StreetSegment> tempAdd;
			tempAdd.push_back(reverse);
			streetSegs.associate(reverse.start, tempAdd);
		}
		else
			ptr->push_back(reverse);
			infile.ignore(10000, '\n'); //ignoring everything else on that line
		}
	}
	return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
	const vector<StreetSegment>* ptr = streetSegs.find(gc);
	if (ptr == nullptr)
		return false;

	segs = *ptr;
	return true;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
	m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
	delete m_impl;
}

bool StreetMap::load(string mapFile)
{
	return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
	return m_impl->getSegmentsThatStartWith(gc, segs);
}