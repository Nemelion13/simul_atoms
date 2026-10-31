#ifndef ATOMS_H
#define ATOMS_H

#include "raylib.h"

#include <deque>
#include <vector>

struct Electron {
    float radius;
    float speed;
    float angle;
    float inclination;
    Color color;
    std::deque<Vector3> trail;
};

struct AtomConfig {
    const char* name;
    int protons;
    int neutrons;
    int electronsK;
    int electronsL;
};

extern const std::vector<AtomConfig> ATOMS;

std::vector<Electron> CreateElectrons(AtomConfig config);

#endif
