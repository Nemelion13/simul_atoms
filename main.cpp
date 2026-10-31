/* Simulation 3D de la rotation des électrons d'un atome autour de son noyau*/
#include "raylib.h"
#include "atoms.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <math.h>
#include <string>
#include <vector>

// Configuration de la traînée
const int MAX_TRAIL_POINTS = 30;

void DrawElectronicConfiguration(
    const std::vector<ElectronicSubshell>& configuration,
    int x,
    int y,
    int screenWidth
) {
    constexpr int fontSize = 16;
    constexpr int exponentFontSize = 11;
    constexpr int termSpacing = 10;

    for (const ElectronicSubshell& subshell : configuration) {
        const std::string term =
            std::to_string(subshell.principalNumber) + subshell.orbital;
        const std::string exponent = std::to_string(subshell.electronCount);
        const int termWidth = MeasureText(term.c_str(), fontSize);
        const int exponentWidth = MeasureText(exponent.c_str(), exponentFontSize);
        const int fullWidth = termWidth + exponentWidth + termSpacing;

        if (x + fullWidth > screenWidth - 20) {
            x = 320;
            y += 24;
        }

        DrawText(term.c_str(), x, y, fontSize, WHITE);
        DrawText(exponent.c_str(), x + termWidth, y - 4, exponentFontSize, WHITE);
        x += fullWidth;
    }
}

int main() {
    // Initialisation de la fenêtre
    const int screenWidth = 1024;
    const int screenHeight = 768;
    InitWindow(screenWidth, screenHeight, "Simulation Atomique Avancee - C++ & Raylib");

    // Liste Raygui : chaque nom est séparé du suivant par un point-virgule.
    std::string atomNames;
    for (const AtomConfig& atom : ATOMS) {
        if (!atomNames.empty()) atomNames += ';';
        atomNames += atom.name;
    }

    int currentAtomIndex = 1; // Par défaut : Lithium
    std::vector<Electron> electrons = CreateElectrons(ATOMS[currentAtomIndex]);
    int atomListScrollIndex = 0;

    // Caméra 3D
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 12.0f, 18.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // nombre d'images par seconde
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Mode caméra orbitale automatique avec la souris
        UpdateCamera(&camera, CAMERA_ORBITAL);

        float deltaTime = GetFrameTime();

        // --- MISE A JOUR DES ELECTRONS & TRAINÉES ---
        for (auto& e : electrons) {
            // Avancement de l'angle
            e.angle += e.speed * deltaTime;
            if (e.angle > 2 * PI) e.angle -= 2 * PI;

            // Calcul de la nouvelle position 3D
            float x = cosf(e.angle) * e.radius;
            float y = sinf(e.angle) * e.radius * sinf(e.inclination);
            float z = sinf(e.angle) * e.radius * cosf(e.inclination);
            Vector3 currentPos = { x, y, z };

            // Ajouter la position courante à l'historique de la traînée
            e.trail.push_back(currentPos);
            if (e.trail.size() > MAX_TRAIL_POINTS) {
                e.trail.pop_front(); // Supprimer le point le plus ancien
            }
        }

        // --- RENDU GRAPHIQUE ---
        BeginDrawing();
            ClearBackground(BLACK);

            // 1. RENDU DE LA SCÈNE 3D
            BeginMode3D(camera);

                // Dessin du Noyau Simplifié (Représentation globale compacte)
                int totalNucleons = ATOMS[currentAtomIndex].atomicNumber + ATOMS[currentAtomIndex].neutrons;
                // Génération visuelle pseudo-aléatoire fixe pour le noyau
                for (int i = 0; i < totalNucleons; i++) {
                    float offset = (float)i * 0.5f;
                    float nx = sinf(offset * 3.0f) * 0.4f;
                    float ny = cosf(offset * 2.0f) * 0.4f;
                    float nz = sinf(offset * 1.5f) * 0.4f;
                    Color nColor = (i % 2 == 0 && ATOMS[currentAtomIndex].atomicNumber > i/2) ? RED : RAYWHITE;
                    DrawSphere((Vector3){ nx, ny, nz }, 0.1f, nColor);
                }

                // Dessin des Orbites, Traînées et Électrons
                for (const auto& e : electrons) {
                    
                    // A. Dessin de l'orbite théorique complète (ligne fine atténuée)
                    DrawCircle3D((Vector3){0,0,0}, e.radius, (Vector3){0, sinf(e.inclination), cosf(e.inclination)}, 90.0f, Fade(e.color, 0.08f));

                    // B. Dessin de la traînée (liaison des points historiques)
                    if (e.trail.size() > 1) {
                        for (size_t i = 0; i < e.trail.size() - 1; i++) {
                            // Calcul du facteur de fondu : plus le point est vieux, plus il est transparent
                            float alpha = (float)i / e.trail.size(); 
                            // Ligne légèrement plus épaisse ou doublée pour l'effet visuel
                            DrawLine3D(e.trail[i], e.trail[i+1], Fade(e.color, alpha * 0.6f));
                        }
                    }

                    // C. Dessin de la sphère de l'électron à sa position actuelle (le dernier point du trail)
                    if (!e.trail.empty()) {
                        Vector3 currentPos = e.trail.back();
                        DrawSphere(currentPos, 0.18f, e.color);
                        DrawSphereWires(currentPos, 0.20f, 6, 6, WHITE); // Effet de surbrillance
                    }
                }

            EndMode3D();

            // 2. RENDU DE L'INTERFACE 2D (Menu & Textes)
            DrawText("SIMULATION ATOMIQUE 3D", 20, 20, 22, GOLD);
            DrawText("Choisissez un element :", 20, 55, 15, LIGHTGRAY);

            Rectangle atomListBounds = { 20.0f, 80.0f, 230.0f, 480.0f };
            int selectedAtomIndex = currentAtomIndex;
            GuiListView(atomListBounds, atomNames.c_str(), &atomListScrollIndex, &selectedAtomIndex);
            if (selectedAtomIndex >= 0 && selectedAtomIndex != currentAtomIndex) {
                currentAtomIndex = selectedAtomIndex;
                electrons = CreateElectrons(ATOMS[currentAtomIndex]);
            }

            const AtomConfig& selectedAtom = ATOMS[currentAtomIndex];
            const std::vector<ElectronicSubshell> electronicConfiguration =
                GetElectronicConfiguration(selectedAtom.atomicNumber);

            // Panneau d'informations sur l'atome sélectionné (En bas à gauche)
            DrawRectangle(20, 600, 260, 140, GetColor(0x111111CC));
            DrawRectangleLines(20, 600, 260, 140, DARKGRAY);
            
            DrawText(TextFormat("Configuration: %s", selectedAtom.name), 30, 610, 16, WHITE);
            DrawText(TextFormat("- Protons (Rouge) : %d", selectedAtom.atomicNumber), 30, 640, 15, RED);
            DrawText(TextFormat("- Neutrons (Gris) : %d", selectedAtom.neutrons), 30, 665, 15, GRAY);
            DrawText(TextFormat("- Electrons : %d", selectedAtom.atomicNumber), 30, 690, 15, SKYBLUE);

            DrawText("Configuration electronique :", 320, 655, 16, LIGHTGRAY);
            DrawElectronicConfiguration(electronicConfiguration, 320, 680, screenWidth);
            // Aide navigation
            DrawText("Utilisez la souris pour tourner la camera autour de l'atome", 320, 735, 14, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
