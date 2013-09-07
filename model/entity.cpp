#include <qmath.h>
#include <iostream>
#include <iomanip>

#include "entity.h"

int Entity::sampleStep_ = 10000;

Entity::Entity()
{
    this->position_ = Vector("0", "0", "0");
    this->velocity_ = Vector("0", "0", "0");
    this->acceleration_ = Vector("0", "0", "0");
    this->mass_ = "5.97219e24";
    this->moveable_ = true;
    this->timeStep_ = 1;
    this->curStep_ = 0;
}

Entity::Entity(Vector position, Vector velocity, QString mass, bool moveable)
{
    this->position_ = position;
    this->velocity_ = velocity;
    this->acceleration_ = Vector("0", "0", "0");
    this->mass_ = mass.toStdString();
    this->moveable_ = moveable;
    this->timeStep_ = 1;
    this->curStep_ = 0;
}

Vector Entity::position()
{
    return this->position_;
}

void Entity::setPosition(Vector position)
{
    this->position_ = position;
}

Vector Entity::velocity()
{
    return this->velocity_;
}

void Entity::setVelocity(Vector velocity)
{
    this->velocity_ = velocity;
}

Vector Entity::accleration()
{
    return this->acceleration_;
}

void Entity::setAccleration(Vector acceleration)
{
    this->acceleration_ = acceleration;
}

mpf_class Entity::mass()
{
    return this->mass_;
}

void Entity::setMass(double mass)
{
    this->mass_ = mass;
}

bool Entity::moveable()
{
    return this->moveable_;
}

void Entity::setMoveable(bool moveable)
{
    this->moveable_ = moveable;
}

mpf_class Entity::timeStep()
{
    return this->timeStep_;
}

void Entity::setTimeStep(QString timeStamp)
{
    this->timeStep_ = timeStamp.toStdString();
}

void Entity::tick(QList<Entity*> entities)
{
    if (this->moveable_)
    {
        calcAccleration(entities);
        move();
    }
}

void Entity::calcAccleration(QList<Entity*> entities)
{
    Vector accTotal;
    foreach (Entity* e, entities)
    {
        if (e != this) {
            Vector delta = e->position().sub(this->position());
            //std::cout << "Delta: " << delta.x() << " " << delta.y() << std::endl;

            mpf_class r = delta.length();
            //std::cout << "Length: " << r << std::endl;

            mpf_class r2 = r * r;
            //std::cout << "Length^2: " << r2 << std::endl;

            mpf_class f = G * (this->mass_ * e->mass()) / r2;
            //std::cout << "Force: " << f << std::endl;

            mpf_class a = f / this->mass_;
            //std::cout << "Acceleration: " << a << std::endl;

            Vector accDir = delta.normalized();
            //std::cout << "Acceleration Dir: " << accDir.x() << " " << accDir.y() << std::endl;

            accTotal = accTotal.add(accDir.scaleByFactor(a));
        }
    }

    this->acceleration_ = accTotal;
    //std::cout << "Total Acceleration: " << this->accleration().x() << " " << this->accleration().y() << std::endl;
}

void Entity::move()
{
    if (moveable_) {
        this->velocity_ = this->velocity_.add(this->acceleration_.scaleByFactor(this->timeStep_));
        this->position_ = this->position_.add(this->velocity_.scaleByFactor(this->timeStep_));

        if (this->curStep_ >= this->sampleStep_) {
            std::cout << std::setprecision(12) << this->position_.x() << " " << this->position().y() << std::endl;
            this->curStep_ = 0;
        }

        this->curStep_++;
    }
}
