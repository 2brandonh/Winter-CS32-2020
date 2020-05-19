#ifndef HASHMAP_H
#define HASHMAP_H

#include <string>
#include <list>
#include <functional>

// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
	int buckets;
	double items;
	const double MAXLOAD;
	unsigned int mapFunction(const KeyType& hash) const;
	void newMap();

	struct Associations {
		KeyType m_key;
		ValueType m_value;
	};

	std::list<Associations>* map; //ARRAY of Association lists
};

template<typename KeyType, typename ValueType>
unsigned int ExpandableHashMap<KeyType, ValueType>::mapFunction(const KeyType& hashItem) const {

	//std::hash<std::string> hash;
//	return str_hash(hashItem);

	unsigned int hasher(const KeyType & k); // prototype

	unsigned int hashValue = hasher(hashItem);

	unsigned int bucketNum = hashValue % buckets;
	return bucketNum;
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor) : buckets(8), items(0), MAXLOAD(maximumLoadFactor)
{
	map = new std::list<Associations>[buckets];
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
	delete[] map;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
	delete[] map; //fast way to reset
	buckets = 8;
	items = 0;
	map = new std::list<Associations>[buckets];
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::newMap() {
	std::list<Associations>* newMap = new std::list<Associations>[buckets];

	for (int i = 0; i < buckets / 2; i++) { //rehashing over to a new ExpandableHashMap
		if (!map[i].empty()) {
			typename std::list<Associations>::iterator ptr = map[i].begin();
			while (!map[i].empty()) {
				unsigned int new_index = mapFunction(ptr->m_key);
				//newMap[new_index].push_back(*ptr);   
				newMap[new_index].splice(newMap[new_index].end(), map[i], ptr);
				ptr = map[i].begin();
			}
		}
	}

	std::list<Associations>* tempDelete = map; //reassigning pointers
	map = newMap;
	delete[]tempDelete;

}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
	return int(items);
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	//case 1, same key so update the value (size does not change)
	ValueType* value_ptr = find(key);
	if (value_ptr != nullptr) {
		*value_ptr = value;
		return;
	}

	//case 2, new association
	items++;
	if (items / buckets > MAXLOAD) {
		buckets *= 2;
		newMap();
	}

	unsigned int i = mapFunction(key);

	Associations a;
	a.m_key = key;
	a.m_value = value;
	map[i].push_back(a);
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
	unsigned int i = mapFunction(key); //going to bucket with the key
	if (map[i].empty())
		return nullptr;
	else {
		typename std::list<Associations>::iterator ptr;
		ptr = map[i].begin();
		while (ptr != map[i].end()) {
			if (ptr->m_key == key)
				return &(ptr->m_value);
			ptr++;
		}
	}
	return nullptr;
}

#endif