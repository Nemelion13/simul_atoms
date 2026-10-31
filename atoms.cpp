#include "atoms.h"

const std::vector<AtomConfig> ATOMS = {
    { "Hydrogene (H)", 1, 0, 1, 0 },
    { "Lithium (Li)", 3, 4, 2, 1 },
    { "Carbone (C)", 6, 6, 2, 4 },
    { "Oxygene (O)", 8, 8, 2, 6 }
};

std::vector<Electron> CreateElectrons(AtomConfig config) {
    std::vector<Electron> electrons;

    for (int i = 0; i < config.electronsK; i++) {
        float startAngle = (2.0f * PI / config.electronsK) * i;
        float inclination = 0.5f + (i * 0.4f);
        electrons.push_back({ 3.5f, 2.5f, startAngle, inclination, BLUE, {} });
    }

    for (int i = 0; i < config.electronsL; i++) {
        float startAngle = (2.0f * PI / config.electronsL) * i;
        float inclination = -0.7f + (i * 0.3f);
        electrons.push_back({ 6.5f, 1.5f, startAngle, inclination, SKYBLUE, {} });
    }

    return electrons;
}
