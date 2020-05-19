#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//function for Euclidean distance

//Global constants
const int SOCRATES_HP = 100;

const int FOREGROUND = 0;
const int BACKGROUND = 1;

const int SPRAY_MAX_DIST = 112;
const int FLAME_MAX_DIST = 32;
const int SPRAY_DMG = 2;
const int FLAME_DMG = 5;

//const int DIRT = 300;

const int SALMONELLA = 302;
const int AGG_SALMONELLA = 303;
const int ECOLI = 304;

//NOTE: these constants for bacteria ARE NOT USED TO IDENTIFY DERIVED CLASSES
//they are used by the Pit as identification on which bacteria to generate (so not breaking any of the code smell rules)

const int LIFE_PTS = 500;
const int HEALTH_PTS = 250;
const int FLAME_PTS = 300;
const int FUNGUS_PTS = -50;
const int BACpoints = 100;

const int SALMONELLA_HP = 4;
const int SALMONELLA_DMG = 1;
const int AGG_SALMONELLA_HP = 10;
const int AGG_SALMONELLA_DMG = 2;
const int ECOLI_HP = 5;
const int ECOLI_DMG = 4;
const int ECOLI_TURN = 10;
const int AGG_SALMONELLA_DIST = 72;
const int ECOLI_DIST = 256;
const int SALMONELLA_MOVE = 3;
const int ECOLI_MOVE = 2;
const int MOVE_PLAN_DIST = 10;

const int FUNGUS_DMG = 20;

const double PI = 4 * atan(1);


class Base : public GraphObject {
public:
	Base(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* sw);
	virtual void doSomething() = 0; //makes an ADT except for most derived classes
	virtual void hit(int dmg) = 0;
	//virtual bool isBacteria() const;
	//virtual bool isGoodie() const;

	virtual bool isBlocking() const;
	virtual bool isConsumable() const;
	virtual bool notOverlappable() const;
	virtual bool isDamageableOBJ() const;

	virtual void set_dead();
	virtual bool get_aliveState() const;
	StudentWorld* get_SW() const; //returning a pointer cannot be const
private:
	bool aliveState;
	StudentWorld* world;
};

class InitObjects : public Base { //for food and PITS (overlap identification)
public:
	InitObjects(int imageID, double startX, double startY, Direction dir, StudentWorld* sw);
	virtual bool notOverlappable() const;
	virtual void hit(int dmg); //does nothing because these objects have no health, but will play sound
};


class Pit : public InitObjects {
public:
	Pit(double rand_x, double rand_y, StudentWorld* sw);
	void doSomething();
private:
	std::vector<int> pit_storage;
};


class Living : public Base {
public:
	Living(int health_input, int imageID, double startX, double startY, Direction dir, StudentWorld* sw);
	int get_HP() const;
	void reset_HP(); //for healing Socrates
	//virtual void hurtSound() = 0;
	//virtual void deathSound() = 0;
	//virtual void hit(int dmg);
	//virtual bool get_aliveState() const; //redefined for living creatures as health
	void decreaseHP(int dmg);
private:
	int m_health;
	int m_startingHP;
};


class Bacteria : public Living {
public:
	Bacteria(int health_input, int imageID, double startX, double startY, StudentWorld* sw, int HURTsound, int DEATHsound);
	//virtual bool isBacteria() const;
	bool isDamageableOBJ() const;
	void hit(int dmg);
	void regularMOVE();

	bool isFoodFull(double& x, double& y); //increases food, then checks if food is full, resets if 3 (TRUE if full)
	//virtual void turn_m_dir();
	bool inBounds(double x, double y) const; //auxiliary function for checking x and y of a new location

	//virtual int getDamage() const;
private:
	bool isMoveable(); //checks if movement is 0, then decreases movement plan, (TRUE is > 0)
	void incFood();
	virtual void randomMove();
	int m_food;
	Direction m_dir;
	int m_movement_plan;
	int m_sound_hurt;
	int m_sound_death;
};

class Salmonella : public Bacteria {
public:
	Salmonella(double startX, double startY, StudentWorld* sw);
	void doSomething();
};

class Agg_Salmonella : public Bacteria {
public:
	Agg_Salmonella(double startX, double startY, StudentWorld* sw);
	void doSomething();
};

class Ecoli : public Bacteria {
public:
	Ecoli(double startX, double startY, StudentWorld* sw);
	void doSomething();
private:
	int m_attempts;
};

class Socrates : public Living {
public:
	Socrates(double startX, double startY, StudentWorld* sw);
	int getflame() const;
	int getspray() const;
	void incflame();
	void doSomething();
	void hit(int dmg);
	//virtual void hurtSound();
	//virtual void deathSound();
private:
	int m_spray_charges;
	int m_flame_charges;
	int m_pos_angle;
	int m_tick_cooldown;
};


class Dirt : public InitObjects {
public:
	Dirt(double rand_x, double rand_y, StudentWorld* sw);
	void doSomething();
	bool notOverlappable() const;
	bool isDamageableOBJ() const;
	void hit(int dmg);
	bool isBlocking() const;
};

class Food : public InitObjects {
public:
	Food(double rand_x, double rand_y, StudentWorld* sw);
	void doSomething();
	bool isConsumable() const;
};

class Goodie : public Base {
public:
	Goodie(int imageID, double rand_x, double rand_y, StudentWorld* sw, int pts);
	//virtual int getSound() const;
	bool GoodieAction(); //function of common code to all goodies, will return false if fails (dead)
	void hit(int dmg); //will die to flame or spray
	bool isDamageableOBJ() const;
	//virtual int getPoints() const; INSTEAD DIRECTLY TELL SW
private:
	virtual bool hasSound() const;
	int m_lifetime;
	int m_lifetime_max;
	int m_points;
};

class HealthGood : public Goodie {
public:
	HealthGood(double rand_x, double rand_y, StudentWorld* sw);
	void doSomething();
};

class FlameGood : public Goodie {
public:
	FlameGood(double rand_x, double rand_y, StudentWorld* sw);
	void doSomething();
};

class LifeGood : public Goodie {
public:
	LifeGood(double rand_x, double rand_y, StudentWorld* sw);
	void doSomething();
};

class FungusGood : public Goodie {
public:
	FungusGood(double rand_x, double rand_y, StudentWorld* sw);
	void doSomething();
private:
	virtual bool hasSound() const;
};



class Projectile : public Base {
public:
	Projectile(int imageID, double startX, double startY, Direction dir, StudentWorld* sw, int maxdist, int dmg);
	void hit(int dmg); //does nothing due to no "health", disappears if touching something or at maxdist
	void doSomething();
private:
	virtual void moveProjectile();
	int m_max_dist;
	int m_pixels_moved;
	int m_projecDMG;
	int m_projecDIR;
};

class Spray : public Projectile {
public:
	Spray(double startX, double startY, Direction dir, StudentWorld* sw);
};

class Flame : public Projectile {
public:
	Flame(double startX, double startY, Direction dir, StudentWorld* sw);
};

#endif // ACTOR_H_