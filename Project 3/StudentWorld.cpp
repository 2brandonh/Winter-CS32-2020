#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <algorithm>
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw

#include <list>
#include <queue>
using namespace std;
class Base;


GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath), lvl(getLevel())
{
	increaseScore(0); //made so score will be initialized
}

///////////////// MAKING A NEW OBJECT WITH QUEUE LIST ////////////////

void StudentWorld::createActor(Base* actor) {
	toAdd.push(actor);
}

///////////////////////////// COLLISION DETECTION ///////////////////

bool StudentWorld::initOverlap(double x, double y) {
	for (list<Base*>::iterator ptr = actors.begin(); ptr != actors.end(); ptr++) {
		if ((*ptr)->notOverlappable() == true) {
			if (dist(x, y, (*ptr)->getX(), (*ptr)->getY()) <= 2 * double(SPRITE_RADIUS))
				return true;
		}
	}
	return false;
}

bool StudentWorld::isMovementOverlap(double x, double y) {
	for (list<Base*>::iterator ptr = actors.begin(); ptr != actors.end(); ptr++) {
		if ((*ptr)->isBlocking() == true) {
			if ((*ptr)->get_aliveState()) {
				if (dist(x, y, (*ptr)->getX(), (*ptr)->getY()) <= double(SPRITE_RADIUS))
					return true;
			}
		}
	}
	return false;
}

bool StudentWorld::isFoodOverlap(double x, double y) {
	for (list<Base*>::iterator ptr = actors.begin(); ptr != actors.end(); ptr++) {
		if ((*ptr)->isConsumable() == true) {
			if (dist(x, y, (*ptr)->getX(), (*ptr)->getY()) <= 2 * double(SPRITE_RADIUS))
				if ((*ptr)->get_aliveState()) {
					(*ptr)->set_dead();
					return true;
				}
		}
	}
	return false;
}


bool StudentWorld::isSocratesOverlap(double x, double y) {
	if (dist(x, y, m_socrates->getX(), m_socrates->getY()) <= 2 * double(SPRITE_RADIUS)) {
		if (m_socrates->get_aliveState())
			return true;
	}
	return false;
}

bool StudentWorld::damageableOverlap(double x, double y, int projectile_dmg) {
	for (list<Base*>::iterator ptr = actors.begin(); ptr != actors.end(); ptr++) {
		if ((*ptr)->isDamageableOBJ() == true) {
			if (dist(x, y, (*ptr)->getX(), (*ptr)->getY()) <= 2 * double(SPRITE_RADIUS)) {
				if ((*ptr)->get_aliveState()) { //IMPORTANT AS TO NOT HAVE MULTIPLE COLLISIONS ON ONE OBJECT
					(*ptr)->hit(projectile_dmg);
					return true;
				}

			}
		}
	}
	return false;
}
///////////// FUNCTIONS ACTORS USE ///////////

void StudentWorld::restoreSocrates() {
	m_socrates->reset_HP();
}

void StudentWorld::flameINC() {
	m_socrates->incflame();
}

void StudentWorld::decreaseBacCount() {
	tot_bacteria--; //used when a bacteria dies
}

void StudentWorld::increaseBacCount() {
	tot_bacteria++; //used when a bacteria duplicates
}

void StudentWorld::hurtSocrates(int dmg) {
	m_socrates->hit(dmg);
}

bool StudentWorld::isSocratesNear(double x, double y, int proximity, Direction& dir) {
	double distance = dist(x, y, m_socrates->getX(), m_socrates->getY());
	if (distance <= double(proximity)) {
		if (m_socrates->get_aliveState()) {
			dir = computeAngle(x, y, m_socrates->getX(), m_socrates->getY());
			return true;
		}
	}
	return false;
}

bool StudentWorld::isFoodNear(double x, double y, Direction& dir) {
	double distance = VIEW_WIDTH; //need some high number;
	double temp_dist;
	bool temp = false;

	for (list<Base*>::iterator ptr = actors.begin(); ptr != actors.end(); ptr++) {
		if ((*ptr)->isConsumable() == true) {
			if ((*ptr)->get_aliveState()) {
				temp_dist = dist(x, y, (*ptr)->getX(), (*ptr)->getY());

				if (temp_dist < distance && temp_dist <= VIEW_RADIUS) {
					distance = temp_dist;
					dir = computeAngle(x, y, (*ptr)->getX(), (*ptr)->getY());
					temp = true;
				}
			}
		}
	}

	return temp;
}

//////////////////    UTILITY       ////////////////

void StudentWorld::GoodieChance(int L) {
	double x, y;

	int i = randInt(0, max(510 - L * 10, 200) - 1);
	if (i == 0) {
		randCircumferencePos(x, y);
		createActor(new FungusGood(x, y, this));
	}

	i = randInt(0, max(510 - L * 10, 250) - 1);
	if (i == 0) {
		i = randInt(1, 10);
		randCircumferencePos(x, y);

		if (i <= 1)
			createActor(new LifeGood(x, y, this));
		else if (i <= 4)
			createActor(new FlameGood(x, y, this));
		else
			createActor(new HealthGood(x, y, this));
	}
}

void StudentWorld::randPos(double& x_pos, double& y_pos, int low, int high) {
	double deg = double(rand_Degree()) * 180 / PI;
	int u = randInt(low, high) + randInt(low, high);
	int radius = 0;
	if (u > high)
		radius = 2 * high - u;
	else
		radius = u;

	x_pos = VIEW_WIDTH / 2 + radius * cos(deg);
	y_pos = VIEW_HEIGHT / 2 + radius * sin(deg);
}

int StudentWorld::rand_Degree() {
	return randInt(0, 360);
}

void StudentWorld::randCircumferencePos(double& x, double& y) {
	double deg = double(rand_Degree()) * 180 / PI;
	x = VIEW_WIDTH / 2 + VIEW_RADIUS * cos(deg);
	y = VIEW_HEIGHT / 2 + VIEW_RADIUS * sin(deg);
}

double StudentWorld::dist(double x1, double y1, double x2, double y2) const
{
	double x = x1 - x2;
	double y = y1 - y2;
	double distance;

	distance = pow(x, 2) + pow(y, 2);
	distance = sqrt(distance);

	return distance;
}

int StudentWorld::computeAngle(double x1, double y1, double x2, double y2) const
{
	double x = x2 - x1;
	double y = y2 - y1;
	if (x == 0)
		return 0; //preventing divide by 0

	double angle = atan(y / x) * 180 / PI;

	if (y >= 0 && x >= 0)
		return int(angle);
	else if (y >= 0 && x <= 0)
		return int(angle + 180);
	else if (y <= 0 && x <= 0)
		return int(angle + 180);
	else
		return int(angle + 360);

}

/////////////////////////////////////

int StudentWorld::init()
{
	m_socrates = new Socrates(0, VIEW_HEIGHT / 2, this);

	lvl = getLevel();
	num_pit = lvl;
	tot_bacteria = 10 * num_pit; //have a get function in each pit to know how many bacteria are left
	num_pizza = min(5 * lvl, 25);
	num_dirt = max(180 - 20 * lvl, 20);
	//note updateback function changes tot_bacteria

	for (int i = 0; i < num_pit + num_pizza + num_dirt; i++) {

		//PIT INIT
		double x_pos, y_pos;
		randPos(x_pos, y_pos, 0, VIEW_RADIUS - SPRITE_WIDTH);

		if (initOverlap(x_pos, y_pos)) //IF TOUCHING ANOTHER food OR PIT, RETRY RANDOM
			continue;

		if (i < num_pit)
			actors.push_back(new Pit(x_pos, y_pos, this));
		else if (i < num_pizza + num_pit)
			actors.push_back(new Food(x_pos, y_pos, this));
		else
			actors.push_back(new Dirt(x_pos, y_pos, this));

	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{

	//responsible for disposing of actors and changing their state

	if (m_socrates->get_aliveState())
		m_socrates->doSomething();

	for (list<Base*>::iterator ptr = actors.begin(); ptr != actors.end(); ptr++) {
		if ((*ptr)->get_aliveState()) { //if alive, then do something
			(*ptr)->doSomething();
		}
		if (m_socrates->get_aliveState() == false) {
			return GWSTATUS_PLAYER_DIED;
		}
		if (tot_bacteria == 0) {
			playSound(SOUND_FINISHED_LEVEL);
			return GWSTATUS_FINISHED_LEVEL;
		}
	}

	for (list<Base*>::iterator ptr = actors.begin(); ptr != actors.end();) {
		if ((*ptr)->get_aliveState() == false) { //erasing dead actors
			delete* ptr;
			ptr = actors.erase(ptr);
		}
		else
			ptr++;
	}

	GoodieChance(lvl);

	while (!toAdd.empty()) {
		actors.push_back(toAdd.front());
		toAdd.pop();
	}

	// Potentially add new actors to the game (e.g., goodies or fungi)

	// Update the Game Status Line
	ostringstream oss;
	oss << "Score: ";
	oss.fill('0');
	if (getScore() >= 0)
		oss << setw(6) << getScore() << "  ";
	else
		oss << "-" << setw(5) << -1 * getScore() << "  ";
	oss << "Level: " << getLevel() << "  ";
	oss << "Lives: " << getLives() << "  ";
	oss << "Health: " << m_socrates->get_HP() << "  ";
	oss << "Sprays: " << m_socrates->getspray() << "  ";
	oss << "Flames: " << m_socrates->getflame();  //<< "  " << "COUNT: " << tot_bacteria; for debugging abcteria count
	string s = oss.str();
	setGameStatText(s); // update the score/lives/level text at screen top

	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//decLives();



	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	if (m_socrates != nullptr) {
		delete m_socrates;
		m_socrates = nullptr;
	}

	for (list<Base*>::iterator ptr = actors.begin(); ptr != actors.end();) {
		delete* ptr;
		ptr = actors.erase(ptr);
	}
}

StudentWorld::~StudentWorld() {
	cleanUp();
}