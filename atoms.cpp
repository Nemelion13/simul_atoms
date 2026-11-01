#include "atoms.h"
#include <algorithm>
#include <array>
#include <stdexcept>

const std::vector<AtomConfig> ATOMS = {
    { "Hydrogène (H)", 1, 0 },
    { "Hélium (He)", 2, 2 },
    { "Lithium (Li)", 3, 4 },
    { "Béryllium (Be)", 4, 5 },
    { "Bore (B)", 5, 6 },
    { "Carbone (C)", 6, 6 },
    { "Azote (N)", 7, 7 },
    { "Oxygène (O)", 8, 8 },
    { "Fluor (F)", 9, 10 },
    { "Néon (Ne)", 10, 10 },
    { "Sodium (Na)", 11, 12 },
    { "Magnésium (Mg)", 12, 12 },
    { "Aluminium (Al)", 13, 14 },
    { "Silicium (Si)", 14, 14 },
    { "Phosphore (P)", 15, 16 },
    { "Soufre (S)", 16, 16 },
    { "Chlore (Cl)", 17, 18 },
    { "Argon (Ar)", 18, 22 },
    { "Potassium (K)", 19, 20 },
    { "Calcium (Ca)", 20, 20 },
    { "Scandium (Sc)", 21, 24 },
    { "Titane (Ti)", 22, 26 },
    { "Vanadium (V)", 23, 28 },
    { "Chrome (Cr)", 24, 28 },
    { "Manganèse (Mn)", 25, 30 },
    { "Fer (Fe)", 26, 30 },
    { "Cobalt (Co)", 27, 32 },
    { "Nickel (Ni)", 28, 30 },
    { "Cuivre (Cu)", 29, 34 },
    { "Zinc (Zn)", 30, 34 },
    { "Gallium (Ga)", 31, 38 },
    { "Germanium (Ge)", 32, 42 },
    { "Arsenic (As)", 33, 42 },
    { "Sélénium (Se)", 34, 46 },
    { "Brome (Br)", 35, 44 },
    { "Krypton (Kr)", 36, 48 },
    { "Rubidium (Rb)", 37, 48 },
    { "Strontium (Sr)", 38, 50 },
    { "Yttrium (Y)", 39, 50 },
    { "Zirconium (Zr)", 40, 50 },
    { "Niobium (Nb)", 41, 52 },
    { "Molybdène (Mo)", 42, 56 },
    { "Technétium (Tc)", 43, 54 },
    { "Ruthénium (Ru)", 44, 58 },
    { "Rhodium (Rh)", 45, 58 },
    { "Palladium (Pd)", 46, 60 },
    { "Argent (Ag)", 47, 60 },
    { "Cadmium (Cd)", 48, 66 },
    { "Indium (In)", 49, 66 },
    { "Étain (Sn)", 50, 70 },
    { "Antimoine (Sb)", 51, 70 },
    { "Tellure (Te)", 52, 78 },
    { "Iode (I)", 53, 74 },
    { "Xénon (Xe)", 54, 78 },
    { "Césium (Cs)", 55, 78 },
    { "Baryum (Ba)", 56, 82 },
    { "Lanthane (La)", 57, 82 },
    { "Cérium (Ce)", 58, 82 },
    { "Praséodyme (Pr)", 59, 82 },
    { "Néodyme (Nd)", 60, 82 },
    { "Prométhium (Pm)", 61, 84 },
    { "Samarium (Sm)", 62, 90 },
    { "Europium (Eu)", 63, 90 },
    { "Gadolinium (Gd)", 64, 94 },
    { "Terbium (Tb)", 65, 94 },
    { "Dysprosium (Dy)", 66, 98 },
    { "Holmium (Ho)", 67, 98 },
    { "Erbium (Er)", 68, 98 },
    { "Thulium (Tm)", 69, 100 },
    { "Ytterbium (Yb)", 70, 104 },
    { "Lutécium (Lu)", 71, 104 },
    { "Hafnium (Hf)", 72, 108 },
    { "Tantale (Ta)", 73, 108 },
    { "Tungstène (W)", 74, 110 },
    { "Rhénium (Re)", 75, 112 },
    { "Osmium (Os)", 76, 116 },
    { "Iridium (Ir)", 77, 116 },
    { "Platine (Pt)", 78, 117 },
    { "Or (Au)", 79, 118 },
    { "Mercure (Hg)", 80, 122 },
    { "Thallium (Tl)", 81, 124 },
    { "Plomb (Pb)", 82, 126 },
    { "Bismuth (Bi)", 83, 126 },
    { "Polonium (Po)", 84, 125 },
    { "Astate (At)", 85, 125 },
    { "Radon (Rn)", 86, 136 },
    { "Francium (Fr)", 87, 136 },
    { "Radium (Ra)", 88, 138 },
    { "Actinium (Ac)", 89, 138 },
    { "Thorium (Th)", 90, 142 },
    { "Protactinium (Pa)", 91, 140 },
    { "Uranium (U)", 92, 146 },
    { "Neptunium (Np)", 93, 144 },
    { "Plutonium (Pu)", 94, 150 },
    { "Américium (Am)", 95, 148 },
    { "Curium (Cm)", 96, 151 },
    { "Berkélium (Bk)", 97, 150 },
    { "Californium (Cf)", 98, 153 },
    { "Einsteinium (Es)", 99, 153 },
    { "Fermium (Fm)", 100, 157 },
    { "Mendélévium (Md)", 101, 157 },
    { "Nobélium (No)", 102, 157 },
    { "Lawrencium (Lr)", 103, 163 },
    { "Rutherfordium (Rf)", 104, 163 },
    { "Dubnium (Db)", 105, 163 },
    { "Seaborgium (Sg)", 106, 163 },
    { "Bohrium (Bh)", 107, 163 },
    { "Hassium (Hs)", 108, 161 },
    { "Meitnérium (Mt)", 109, 169 },
    { "Darmstadtium (Ds)", 110, 171 },
    { "Roentgenium (Rg)", 111, 171 },
    { "Copernicium (Cn)", 112, 173 },
    { "Nihonium (Nh)", 113, 173 },
    { "Flérovium (Fl)", 114, 175 },
    { "Moscovium (Mc)", 115, 175 },
    { "Livermorium (Lv)", 116, 177 },
    { "Tennessine (Ts)", 117, 177 },
    { "Oganesson (Og)", 118, 176 }
};

namespace {

struct Orbital {
    int principalNumber;
    char type;
    int capacity;
};

constexpr std::array<Orbital, 19> ORBITALS = {{
    { 1, 's', 2 }, { 2, 's', 2 }, { 2, 'p', 6 }, { 3, 's', 2 },
    { 3, 'p', 6 }, { 4, 's', 2 }, { 3, 'd', 10 }, { 4, 'p', 6 },
    { 5, 's', 2 }, { 4, 'd', 10 }, { 5, 'p', 6 }, { 6, 's', 2 },
    { 4, 'f', 14 }, { 5, 'd', 10 }, { 6, 'p', 6 }, { 7, 's', 2 },
    { 5, 'f', 14 }, { 6, 'd', 10 }, { 7, 'p', 6 }
}};

struct ConfigurationException {
    int atomicNumber;
    int donorPrincipalNumber;
    char donorOrbital;
    int acceptorPrincipalNumber;
    char acceptorOrbital;
    int electronCount;
};

// Quelques exceptions connues a la regle de remplissage des sous-couches.
constexpr std::array<ConfigurationException, 20> CONFIGURATION_EXCEPTIONS = {{
    { 24, 4, 's', 3, 'd', 1 },
    { 29, 4, 's', 3, 'd', 1 },
    { 41, 5, 's', 4, 'd', 1 },
    { 42, 5, 's', 4, 'd', 1 },
    { 44, 5, 's', 4, 'd', 1 },
    { 45, 5, 's', 4, 'd', 1 },
    { 46, 5, 's', 4, 'd', 2 },
    { 47, 5, 's', 4, 'd', 1 },
    { 57, 4, 'f', 5, 'd', 1 },
    { 58, 4, 'f', 5, 'd', 1 },
    { 64, 4, 'f', 5, 'd', 1 },
    { 78, 6, 's', 5, 'd', 1 },
    { 79, 6, 's', 5, 'd', 1 },
    { 89, 5, 'f', 6, 'd', 1 },
    { 90, 5, 'f', 6, 'd', 2 },
    { 91, 5, 'f', 6, 'd', 1 },
    { 92, 5, 'f', 6, 'd', 1 },
    { 93, 5, 'f', 6, 'd', 1 },
    { 96, 5, 'f', 6, 'd', 1 },
    { 103, 6, 'd', 7, 'p', 1 }
}};

void TransferElectrons(
    std::vector<ElectronicSubshell>& configuration,
    const ConfigurationException& exception
) {
    for (ElectronicSubshell& subshell : configuration) {
        if (subshell.principalNumber == exception.donorPrincipalNumber &&
            subshell.orbital == exception.donorOrbital) {
            subshell.electronCount -= exception.electronCount;
        }

        if (subshell.principalNumber == exception.acceptorPrincipalNumber &&
            subshell.orbital == exception.acceptorOrbital) {
            subshell.electronCount += exception.electronCount;
        }
    }

    
}

}

std::vector<ElectronicSubshell> GetElectronicConfiguration(int atomicNumber) {
    if (atomicNumber < 1 || atomicNumber > 118) {
        throw std::out_of_range("Le numero atomique doit etre compris entre 1 et 118.");
    }

    int remainingElectrons = atomicNumber;
    std::vector<ElectronicSubshell> configuration;

    for (const Orbital& orbital : ORBITALS) {
        const int electronsInOrbital = std::min(remainingElectrons, orbital.capacity);
        configuration.push_back({
            orbital.principalNumber,
            orbital.type,
            electronsInOrbital
        });
        remainingElectrons -= electronsInOrbital;
    }

    for (const ConfigurationException& exception : CONFIGURATION_EXCEPTIONS) {
        if (exception.atomicNumber == atomicNumber) {
            TransferElectrons(configuration, exception);
            break;
        }
    }

    configuration.erase(
        std::remove_if(configuration.begin(), configuration.end(),
            [](const ElectronicSubshell& subshell) {
                return subshell.electronCount == 0;
            }),
        configuration.end()
    );

    return configuration;
}

std::string FormatElectronicConfiguration(
    const std::vector<ElectronicSubshell>& configuration
) {
    std::string result;

    for (const ElectronicSubshell& subshell : configuration) {
        if (!result.empty()) result += ' ';
        result += std::to_string(subshell.principalNumber);
        result += subshell.orbital;
        result += '^';
        result += std::to_string(subshell.electronCount);
    }

    return result;
}

std::vector<Electron> CreateElectrons(AtomConfig config) {
    std::vector<Electron> electrons;
    const std::vector<ElectronicSubshell> configuration =
        GetElectronicConfiguration(config.atomicNumber);
    std::array<int, 8> electronsPerShell = {};

    for (const ElectronicSubshell& subshell : configuration) {
        electronsPerShell[subshell.principalNumber] += subshell.electronCount;
    }

    constexpr std::array<Color, 8> shellColors = {{
        LIME, BLUE, SKYBLUE, GREEN, YELLOW, ORANGE, RED, WHITE
    }};

    for (int shell = 1; shell < static_cast<int>(electronsPerShell.size()); shell++) {
        const int shellElectronCount = electronsPerShell[shell];
        const float radius = 3.5f + 1.5f * (shell - 1);
        const Color color = shellColors[shell];

        for (int i = 0; i < shellElectronCount; i++) {
            const float startAngle = (2.0f * PI / shellElectronCount) * i;
            const float inclination = (i % 2 == 0 ? 1.0f : -1.0f) *
                (0.25f + 0.2f * (i % 4));
            electrons.push_back({
                radius,
                2.5f / shell,
                startAngle,
                inclination,
                color,
                {}
            });
        }
    }

    return electrons;
}
