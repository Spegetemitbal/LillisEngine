//
// Created by Somed on 6/9/2025.
//

#ifndef PHYSICSMATERIAL_H
#define PHYSICSMATERIAL_H

struct PhysicsMaterial
{
    //Bounciness, between 0 and 1
    float restitution = 0;
    float staticFriction = 0.6f;
    float dynamicFriction = 0.4f;
};

#endif //PHYSICSMATERIAL_H
