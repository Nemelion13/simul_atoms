#ifndef ATOMS_H
#define ATOMS_H

#include "raylib.h"

#include <deque>
#include <string>
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
    int atomicNumber;
    int neutrons;
};

struct ElectronicSubshell {
    int principalNumber;
    char orbital;
    int electronCount;
};

extern const std::vector<AtomConfig> ATOMS;

std::vector<ElectronicSubshell> GetElectronicConfiguration(int atomicNumber);
std::string FormatElectronicConfiguration(const std::vector<ElectronicSubshell>& configuration);
std::vector<Electron> CreateElectrons(AtomConfig config);

#endif
