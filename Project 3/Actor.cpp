#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// BASE CLASS IMPLEMENTATIONS //

Base::Base(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* sw) : GraphObject(imageID, startX, startY, dir, depth),
aliveState(true), world(sw) {}

void Base::set_dead() {
	aliveState = false;
}
bool Base::get_aliveState() const {
	return aliveState; //returns if alive or not
}
StudentWorld* Base::get_SW() const {
	return world;
}

bool Base::isConsumable() const {
	return false;
}

bool Base::isDamageableOBJ() const {
	return false;
}
bool Base::isBlocking() const {
	return false;
}
bool Base::notOverlappable() const {
	return false;
}

// INIT OBJECT CLASS IMPLEMENTATIONS //

InitObjects::InitObjects(int imageID, double startX, double startY, Direction dir, StudentWorld* sw) :
	Base(imageID, startX, startY, dir, BACKGROUND, sw) {}

bool InitObjects::notOverlappable() const {
	return true;
}

void InitObjects::hit(int dmg) { }

// PIT CLASS IMPLEMENTATIONS // 

Pit::Pit(double rand_x, double rand_y, StudentWorld* sw) : InitObjects(IID_PIT, rand_x, rand_y, 0, sw)
{
	for (int i = 0; i < 5; i++)
		pit_storage.push_back(SALMONELLA);
	for (int i = 0; i < 3; i++)
		pit_storage.push_back(AGG_SALMONELLA);
	for (int i = 0; i < 2; i++)
		pit_storage.push_back(ECOLI);
}

void Pit::doSomething() {
	if (pit_storage.empty()) {
		set_dead();
		return;
	}

	int i = randInt(1, 50);
	if (i == 1) {
		i = randInt(0, pit_storage.size() - 1);

		switch (pit_storage[i]) {
		case SALMONELLA:
			get_SW()->createActor(new Salmonella(getX(), getY(), get_SW()));
			break;
		case AGG_SALMONELLA:
			get_SW()->createActor(new Agg_Salmonella(getX(), getY(), get_SW()));
			break;
		case ECOLI:
			get_SW()->createActor(new Ecoli(getX(), getY(), get_SW()));
			break;
		}
		get_SW()->playSound(SOUND_BACTERIUM_BORN);
		pit_storage.erase(pit_storage.begin() + i);
	}

	//pick random bac
	//update bac count via sw
}


// DIRT CLASS IMPLEMENTATIONS //

Dirt::Dirt(double rand_x, double rand_y, StudentWorld* sw) : InitObjects(IID_DIRT, rand_x, rand_y, 0, sw) {}
bool Dirt::notOverlappable() const {
	return false;
}
void Dirt::doSomething() {}
void Dirt::hit(int dmg) {
	set_dead();
}
bool Dirt::isDamageableOBJ() const {
	return true;
}
bool Dirt::isBlocking() const {
	return true;
}


// FOOD CLASS IMPLEMENTATIONS //

Food::Food(double rand_x, double rand_y, StudentWorld* sw) : InitObjects(IID_FOOD, rand_x, rand_y, 90, sw) {}
void Food::doSomething() {}
bool Food::isConsumable() const {
	return true;
}

// GOODIE CLASS IMPLEMENTATIONS //

Goodie::Goodie(int imageID, double rand_x, double rand_y, StudentWorld* sw, int pts) : Base(imageID, rand_x, rand_y, 0, BACKGROUND, sw),
m_lifetime(0), m_lifetime_max(max(rand() % (300 - 10 * get_SW()->getLevel()), 50)), m_points(pts) {}

bool Goodie::GoodieAction() {
	if (!get_aliveState())
		return false;

	if (get_SW()->isSocratesOverlap(getX(), getY())) {
		set_dead();
		get_SW()->increaseScore(m_points);
		if (hasSound())
			get_SW()->playSound(SOUND_GOT_GOODIE);
		return true;
	}

	m_lifetime++;
	//int test = randInt(0, 300 - 10 * get_SW()->getLevel());
	if (m_lifetime > m_lifetime_max) {
		set_dead();
	}
	return false;
}

bool Goodie::hasSound() const {
	return true;
}

bool Goodie::isDamageableOBJ() const {
	return true;
}

void Goodie::hit(int dmg) {
	set_dead();
}

//HEALTH GOODIE IMPLEMENTATION //

HealthGood::HealthGood(double rand_x, double rand_y, StudentWorld* sw) :
	Goodie(IID_RESTORE_HEALTH_GOODIE, rand_x, rand_y, sw, HEALTH_PTS) {}

void HealthGood::doSomething() {
	if (GoodieAction()) {
		get_SW()->restoreSocrates();
	}
}

//FLAME GOODIE IMPLEMENTATION //

FlameGood::FlameGood(double rand_x, double rand_y, StudentWorld* sw) :
	Goodie(IID_FLAME_THROWER_GOODIE, rand_x, rand_y, sw, FLAME_PTS) {}

void FlameGood::doSomething() {
	if (GoodieAction()) {
		get_SW()->flameINC();
	}
}

//LIFE GOODIE IMPLEMENTATION//

LifeGood::LifeGood(double rand_x, double rand_y, StudentWorld* sw) :
	Goodie(IID_EXTRA_LIFE_GOODIE, rand_x, rand_y, sw, LIFE_PTS) {}

void LifeGood::doSomething() {
	if (GoodieAction()) {
		get_SW()->incLives();
	}
}

//FUNGUS GOODIE IMPLEMENTATION//

FungusGood::FungusGood(double rand_x, double rand_y, StudentWorld* sw) :
	Goodie(IID_FUNGUS, rand_x, rand_y, sw, FUNGUS_PTS) {}

void FungusGood::doSomething() {
	if (GoodieAction())
		get_SW()->hurtSocrates(FUNGUS_DMG);
}

bool FungusGood::hasSound() const {
	return false;
}

// LIVING CLASS IMPLEMENTATIONS // 

Living::Living(int health_input, int imageID, double startX, double startY, Direction dir, StudentWorld* sw) :
	Base(imageID, startX, startY, dir, FOREGROUND, sw),
	m_health(health_input), m_startingHP(health_input) {} //note foreground as all living are in front

int Living::get_HP() const {
	return m_health;
}

//bool Living::get_aliveState() const {
//	return 
//}

void Living::decreaseHP(int dmg) {
	m_health -= dmg;
	if (m_health <= 0) {
		set_dead();
	}
}

void Living::reset_HP() {
	m_health = m_startingHP;
}

// BACTERIA CLASS IMPLEMENTATIONS // 

Bacteria::Bacteria(int health_input, int imageID, double startX, double startY, StudentWorld* sw, int HURTsound, int DEATHsound) :
	Living(health_input, imageID, startX, startY, 90, sw),
	m_food(0), m_dir(90), m_movement_plan(0), m_sound_hurt(HURTsound), m_sound_death(DEATHsound) {}

//bool Bacteria::isBacteria() const {
//	return true;
//}

bool Bacteria::isDamageableOBJ() const {
	return true;
}

void Bacteria::randomMove() {
	m_movement_plan = MOVE_PLAN_DIST;
	m_dir = randInt(0, 359);
	setDirection(m_dir);
}

bool Bacteria::isFoodFull(double& x, double& y) { //called after Overlapping food returns true

	if (m_food >= 3) {
		m_food = 0;
		x = getX();
		y = getY();

		if (getX() < VIEW_WIDTH / 2)
			x += SPRITE_RADIUS;
		else if (getX() > VIEW_WIDTH / 2)
			x -= SPRITE_RADIUS;

		if (getY() < VIEW_HEIGHT / 2)
			y += SPRITE_RADIUS;
		else if (getY() > VIEW_HEIGHT / 2)
			y -= SPRITE_RADIUS;

		get_SW()->increaseBacCount();
		return true;
	}
	// otherwise if food is not full, check if overlapping CANNOT DUPLICATE AND EAT AT THE SAME TICK
	if (get_SW()->isFoodOverlap(getX(), getY()))
		incFood();
	return false;
}

bool Bacteria::isMoveable() {
	if (m_movement_plan <= 0) {
		m_movement_plan = MOVE_PLAN_DIST;
		return false;
	}
	return true;
}

bool Bacteria::inBounds(double newX, double newY) const {
	double x = newX - double(VIEW_WIDTH / 2);
	double y = newY - double(VIEW_HEIGHT / 2);

	double distance;

	distance = pow(x, 2) + pow(y, 2);
	distance = sqrt(distance);

	if (distance >= VIEW_RADIUS)
		return false;

	return true;
}

void Bacteria::hit(int dmg) {

	decreaseHP(dmg);

	if (get_aliveState())
		get_SW()->playSound(m_sound_hurt);
	else {
		get_SW()->playSound(m_sound_death);
		get_SW()->increaseScore(BACpoints);
		get_SW()->decreaseBacCount();

		int i = randInt(0, 1);
		if (i == 0)
			get_SW()->createActor(new Food(getX(), getY(), get_SW()));
	}
}

void Bacteria::regularMOVE() {
	double x, y;
	m_dir = getDirection();

	if (isMoveable()) {
		m_movement_plan--;
		getPositionInThisDirection(m_dir, SALMONELLA_MOVE, x, y);
		if (get_SW()->isMovementOverlap(x, y) || !inBounds(x, y)) {
			randomMove();
		}
		else {
			//setDirection(m_dir); don't need this as continues moving in same direction
			moveTo(x, y);
		}
	}
	else if (get_SW()->isFoodNear(getX(), getY(), m_dir)) { //if food can be found
		getPositionInThisDirection(m_dir, SALMONELLA_MOVE, x, y);
		setDirection(m_dir);

		if (get_SW()->isMovementOverlap(x, y)) {
			randomMove();
		}
	}
	else //no food can be found
		randomMove();

}

//void Bacteria::turn_m_dir() {
//	m_dir += 10;
//}

void Bacteria::incFood() {
	m_food++;
}

// SALMONELLA /////

Salmonella::Salmonella(double startX, double startY, StudentWorld* sw) :
	Bacteria(SALMONELLA_HP, IID_SALMONELLA, startX, startY, sw, SOUND_SALMONELLA_HURT, SOUND_SALMONELLA_DIE) {}

void Salmonella::doSomething() {
	if (!get_aliveState())
		return;
	double x, y;

	if (get_SW()->isSocratesOverlap(getX(), getY()))
		get_SW()->hurtSocrates(SALMONELLA_DMG);
	else if (isFoodFull(x, y)) //steps 3-5 in spec
		get_SW()->createActor(new Salmonella(x, y, get_SW()));

	regularMOVE();
}

// AGGRESSIVE SALMONELLA //

Agg_Salmonella::Agg_Salmonella(double startX, double startY, StudentWorld* sw) :
	Bacteria(AGG_SALMONELLA_HP, IID_SALMONELLA, startX, startY, sw, SOUND_SALMONELLA_HURT, SOUND_SALMONELLA_DIE) {}

void Agg_Salmonella::doSomething() {
	if (!get_aliveState())
		return;

	bool step_skip = false;
	double x, y;
	Direction dir = getDirection();

	if (get_SW()->isSocratesNear(getX(), getY(), AGG_SALMONELLA_DIST, dir)) { //step 2
		getPositionInThisDirection(dir, SALMONELLA_MOVE, x, y);
		if (!get_SW()->isMovementOverlap(x, y) && inBounds(x, y)) {
			moveTo(x, y);
			setDirection(dir);
		}
		step_skip = true; //step 6+ skipper
	}

	if (get_SW()->isSocratesOverlap(getX(), getY())) { //step 3
		get_SW()->hurtSocrates(AGG_SALMONELLA_DMG);
		if (step_skip) return;
	}
	else if (isFoodFull(x, y))  //step 4-5
		get_SW()->createActor(new Agg_Salmonella(x, y, get_SW()));

	if (step_skip) return;
	regularMOVE();

}

// ECOLI /////////////

Ecoli::Ecoli(double startX, double startY, StudentWorld* sw) :
	Bacteria(ECOLI_HP, IID_ECOLI, startX, startY, sw, SOUND_ECOLI_HURT, SOUND_ECOLI_DIE), m_attempts(0) {}

void Ecoli::doSomething() {
	double x, y;
	Direction dir = getDirection();

	if (!get_aliveState())
		return;

	if (get_SW()->isSocratesOverlap(getX(), getY())) //step 2
		get_SW()->hurtSocrates(ECOLI_DMG);
	else if (isFoodFull(x, y)) //step 3-4
		get_SW()->createActor(new Ecoli(x, y, get_SW()));

	if (get_SW()->isSocratesNear(getX(), getY(), ECOLI_DIST, dir)) { //step 5

		m_attempts = 0;
		while (m_attempts < 10) {
			setDirection(dir);
			getPositionInThisDirection(dir, ECOLI_MOVE, x, y);
			if (get_SW()->isMovementOverlap(x, y)) {
				m_attempts++;
				setDirection(getDirection() + ECOLI_TURN);
				dir += ECOLI_TURN;
			}
			else {
				if (!inBounds(x, y)) {
					m_attempts++;
					continue;
				}

				moveTo(x, y);
				return;
			}
		}

	}
}

// SOCRATES CLASS IMPLEMENTATIONS //

Socrates::Socrates(double startX, double startY, StudentWorld* sw) : Living(SOCRATES_HP, IID_PLAYER, startX, startY, 0, sw),
m_spray_charges(20), m_flame_charges(5), m_pos_angle(180), m_tick_cooldown(0) {}

void Socrates::hit(int dmg) {
	decreaseHP(dmg);

	if (get_aliveState())
		get_SW()->playSound(SOUND_PLAYER_HURT);
	else {
		get_SW()->playSound(SOUND_PLAYER_DIE);
		get_SW()->decLives();
	}
}

int Socrates::getflame() const {
	return m_flame_charges;
}
int Socrates::getspray() const {
	return m_spray_charges;
}
void Socrates::incflame() {
	m_flame_charges += 5;
}

void Socrates::doSomething() {
	double x;
	double y;

	if (get_aliveState() == false)
		return;
	int ch;
	if (get_SW()->getKey(ch)) {
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			m_pos_angle -= 5;
			moveTo(VIEW_WIDTH / 2 + cos(m_pos_angle * PI / 180) * VIEW_RADIUS, VIEW_HEIGHT / 2 + sin(m_pos_angle * PI / 180) * VIEW_RADIUS);
			setDirection(m_pos_angle + 180);
			m_tick_cooldown = 0;
			break;
		case KEY_PRESS_RIGHT:
			m_pos_angle += 5;
			moveTo(VIEW_WIDTH / 2 + cos(m_pos_angle * PI / 180) * VIEW_RADIUS, VIEW_HEIGHT / 2 + sin(m_pos_angle * PI / 180) * VIEW_RADIUS);
			setDirection(m_pos_angle + 180);
			m_tick_cooldown = 0;
			break;
		case KEY_PRESS_SPACE:
			m_tick_cooldown = 0;
			if (m_spray_charges > 0) {
				get_SW()->playSound(SOUND_PLAYER_SPRAY);
				m_spray_charges--;

				getPositionInThisDirection(getDirection(), 2 * SPRITE_RADIUS, x, y);
				//get_SW()->actors.push_back(new Flame(x, y, getDirection(), get_SW()));
				get_SW()->createActor(new Spray(x, y, getDirection(), get_SW()));
			}
			break;
		case KEY_PRESS_ENTER:
			if (m_flame_charges > 0) {
				get_SW()->playSound(SOUND_PLAYER_FIRE);
				m_flame_charges--;

				for (int i = 0; i < 16; i++) {
					int dir = getDirection() + i * 22;

					getPositionInThisDirection(dir, 2 * SPRITE_RADIUS, x, y);
					get_SW()->createActor(new Flame(x, y, dir, get_SW()));
				}
			}
			break;
		}
	}
	else {
		if (m_spray_charges < 20 && m_tick_cooldown >= 1) //Socrates must wait one or more ticks without spraying to replenish
			m_spray_charges++;
	}

	if (m_tick_cooldown <= 2) //to prevent infinite m_tick_cooldown ++
		m_tick_cooldown++; //number of ticks since not spraying
}


// PROJECTILE CLASS IMPLEMENATIONS //

Projectile::Projectile(int imageID, double startX, double startY, Direction dir, StudentWorld* sw, int maxdist, int dmg) :
	Base(imageID, startX, startY, dir, BACKGROUND, sw),
	m_max_dist(maxdist), m_pixels_moved(0), m_projecDMG(dmg), m_projecDIR(dir) {}

void Projectile::hit(int dmg) {}

void Projectile::moveProjectile() {

	m_pixels_moved += 2 * SPRITE_RADIUS;
	moveAngle(m_projecDIR, 2 * SPRITE_RADIUS);

	if (m_pixels_moved > m_max_dist)
		set_dead();
}

void Projectile::doSomething() {
	if (!get_aliveState())
		return;
	if (get_SW()->damageableOverlap(getX(), getY(), m_projecDMG)) {
		set_dead();
		return;
	}
	moveProjectile();
}

// SPRAY CLASS IMPLEMENTATION //

Spray::Spray(double startX, double startY, Direction dir, StudentWorld* sw) :
	Projectile(IID_SPRAY, startX, startY, dir, sw, SPRAY_MAX_DIST, SPRAY_DMG) {}


// FLAME CLASS IMPLEMENTATION //

Flame::Flame(double startX, double startY, Direction dir, StudentWorld* sw) :
	Projectile(IID_FLAME, startX, startY, dir, sw, FLAME_MAX_DIST, FLAME_DMG) {}
