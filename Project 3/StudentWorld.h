#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <list>
#include <queue>

//classes not includes due to declarations
class Socrates;
class Base;

using Direction = int;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	//ALLOWS TO CHECK ALL INTERACTABLES *****

	// X Y of nearest food int (&a, &b, c, d)
	// Check if touching Socrates THEN SETS SOCRATES HURT IF TRUE, BASED ON DAMAGE AMOUNT bool 
	//Socrates can be easily managed since the ptr to Socrates is most derived

	//BacteriaOverlap(Bacteria *) DMG VALUES NEEDED SINCE POLYMORPHISM CAN'T SEE IT
	//ProjectileOverlap(Projectile *)

	//nearest food bool with &x and &y
	//

	void restoreSocrates();
	void flameINC();
	void hurtSocrates(int dmg);

	void decreaseBacCount();
	void increaseBacCount();
	bool isMovementOverlap(double x, double y); //used by a bacteria to check if movement is obstructed BY DIRT
	bool isFoodOverlap(double x, double y);
	bool isSocratesOverlap(double x, double y);
	bool isSocratesNear(double x, double y, int proximity, Direction& dir); //returns false if not close, direction updates otherwise
	bool isFoodNear(double x, double y, Direction& dir); //returns false if not close, direction updates otherwise
	bool damageableOverlap(double x, double y, int projectile_dmg); //used by flame and spray to check if overlap by a damageable object
	void createActor(Base* actor);
	virtual ~StudentWorld();

private:
	Socrates* m_socrates;
	std::list<Base*> actors; //container of pointers to actors for dynamic memory

	void GoodieChance(int L); //Goodiemaker
	std::queue <Base*> toAdd;

	int computeAngle(double x1, double y1, double x2, double y2) const;
	int rand_Degree();
	void randPos(double& x_pos, double& y_pos, int low, int high);
	void randCircumferencePos(double& x, double& y);

	int lvl;
	int num_dirt;
	int num_pit;
	int num_pizza;
	int tot_bacteria;
	bool initOverlap(double x, double y); //used for creating pits and food to see if overlap occurs
	double dist(double x1, double y1, double x2, double y2) const; //checking proximity

};

#endif // STUDENTWORLD_H_
