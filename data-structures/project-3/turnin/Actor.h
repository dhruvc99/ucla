#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED

#include "GraphObject.h"

class StudentWorld;
class Goodie;
class Penelope;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* w, int imageID, double x, double y, int dir, int depth);

      // Action to perform for each tick.
    virtual void doSomething() = 0;

      // Get this actor's world
    StudentWorld* world() const;
    
      // Does this object block agent movement?
    virtual bool blocksMovement() const;

      // Is this actor dead?
    bool isDead() const;
 
      // Mark this actor as dead.
    void setDead();
 
      // If this is an activated object, perform its effect on a (e.g., for an
      // Exit have a use the exit).
    virtual void activateIfAppropriate(Actor* a);

      // If this object uses exits, use the exit.
    virtual void useExitIfAppropriate();

      // If this object can die by falling into a pit or burning, die.
    virtual void dieByFallOrBurnIfAppropriate();

      // If this object can be infected by vomit, get infected.
    virtual void beVomitedOnIfAppropriate();

      // If this object can pick up goodies, pick up g
    virtual void pickUpGoodieIfAppropriate(Goodie* g);

      // Does this object block flames?
    virtual bool blocksFlame() const;

      // Does this object trigger landmines only when they're active?
    virtual bool triggersOnlyActiveLandmines() const;

      // Can this object cause a zombie to vomit?
    virtual bool triggersZombieVomit() const;

      // Is this object a threat to citizens?
    virtual bool threatensCitizens() const;

      // Does this object trigger citizens to follow it or flee it?
    virtual bool triggersCitizens() const;
    
      // Constant step for Agent movement:
    const int STEP = 4;
    
private:
    StudentWorld* m_world;
    bool m_dead;
};

class Wall : public Actor
{
public:
    Wall(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual bool blocksMovement() const;
    virtual bool blocksFlame() const;
};

class ActivatingObject : public Actor
{
public:
    ActivatingObject(StudentWorld* w, int imageID, double x, double y, int depth, int dir);
    
    // moved doSomething() to ActivatingObject since common to all ActivatingObjects
    virtual void doSomething();
};

class Exit : public ActivatingObject
{
public:
    Exit(StudentWorld* w, double x, double y);
    // virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    virtual bool blocksFlame() const;
};

class Pit : public ActivatingObject
{
public:
    Pit(StudentWorld* w, double x, double y);
    // virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
};

class Projectile : public ActivatingObject
{
public:
    Projectile(StudentWorld* w, int imageID, double x, double y, int dir);
    virtual void doSomething();
    virtual void activateifAppropriate(Actor* a);
    
private:
    int m_tickCount;
};

class Flame : public Projectile
{
public:
    Flame(StudentWorld* w, double x, double y, int dir);
    // virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
};

class Vomit : public Projectile
{
public:
    Vomit(StudentWorld* w, double x, double y, int dir); // changed provided interface by adding dir to constructor since we must specify vomit's direction
    // virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
};

class Landmine : public ActivatingObject
{
public:
    Landmine(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    virtual void dieByFallOrBurnIfAppropriate();
    
private:
    int m_safetyTicks;
    bool m_active;
};

class Goodie : public ActivatingObject
{
public:
    Goodie(StudentWorld* w, int imageID, double x, double y);
    virtual void doSomething(); // since all Goodie's have same doSomething implementation
    virtual void activateIfAppropriate(Actor* a);
    virtual void dieByFallOrBurnIfAppropriate();

      // Have p pick up this goodie.
    virtual void pickUp(Penelope* p) = 0;
};

class VaccineGoodie : public Goodie
{
public:
    VaccineGoodie(StudentWorld* w, double x, double y);
    // virtual void doSomething();
    virtual void pickUp(Penelope* p);
};

class GasCanGoodie : public Goodie
{
public:
    GasCanGoodie(StudentWorld* w, double x, double y);
    // virtual void doSomething();
    virtual void pickUp(Penelope* p);
};

class LandmineGoodie : public Goodie
{
public:
    LandmineGoodie(StudentWorld* w, double x, double y);
    // virtual void doSomething();
    virtual void pickUp(Penelope* p);
};

class Agent : public Actor
{
public:
    Agent(StudentWorld* w, int imageID, double x, double y, int dir);
    virtual bool blocksMovement() const;
    virtual bool triggersOnlyActiveLandmines() const;
    
    bool getParalyzed() const;
    void setParalyzed(bool paralyzed);
    
private:
    bool m_paralyzed;
};

class Human : public Agent
{
public:
    Human(StudentWorld* w, int imageID, double x, double y);
    virtual void beVomitedOnIfAppropriate();
    virtual bool triggersZombieVomit() const;

      // Make this human uninfected by vomit i.e. when Penelope uses vaccine
    void clearInfection();

      // How many ticks since this human was infected by vomit? -- returns bool instead of int
      // return true if infected for 500 ticks i.e. thing human is dead
    bool getInfectionDuration();
    
    bool isInfected() const;
    virtual void dieByInfection() = 0;
    
private:
    bool m_infected;
    int m_infectionCount;
};

class Penelope : public Human
{
public:
    Penelope(StudentWorld* w, double x, double y);
    virtual void doSomething();
    
    virtual void dieByInfection();
    
    virtual bool triggersCitizens() const;
    virtual void useExitIfAppropriate();
    virtual void dieByFallOrBurnIfAppropriate();
    virtual void pickUpGoodieIfAppropriate(Goodie* g);

      // Increase the number of vaccines the object has.
    void increaseVaccines();

      // Increase the number of flame charges the object has.
    void increaseFlameCharges();

      // Increase the number of landmines the object has.
    void increaseLandmines();

      // How many vaccines does the object have?
    int getNumVaccines() const;

      // How many flame charges does the object have?
    int getNumFlameCharges() const;

      // How many landmines does the object have?
    int getNumLandmines() const;
    
private:
    int m_numVaccines;
    int m_numFlameCharges;
    int m_numLandmines;
};

class Citizen : public Human
{
public:
    Citizen(StudentWorld* w,  double x, double y);
    virtual void doSomething();
    virtual void useExitIfAppropriate();
    virtual void dieByFallOrBurnIfAppropriate();
    virtual void dieByInfection();
    virtual void beVomitedOnIfAppropriate();
};

class Zombie : public Agent
{
public:
    Zombie(StudentWorld* w,  double x, double y);
    virtual bool threatensCitizens() const;
    virtual bool triggersCitizens() const;
};

class DumbZombie : public Zombie
{
public:
    DumbZombie(StudentWorld* w,  double x, double y);
    virtual void doSomething();
    virtual void dieByFallOrBurnIfAppropriate();
};

class SmartZombie : public Zombie
{
public:
    SmartZombie(StudentWorld* w,  double x, double y);
    virtual void doSomething();
    virtual void dieByFallOrBurnIfAppropriate();
};

#endif // ACTOR_INCLUDED
