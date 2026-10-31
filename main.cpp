/* Simulation 3D de la rotation des électrons d'un atome autour de son noyau*/
#include "raylib.h"
#include "atoms.h"

#include <math.h>
#include <vector>

// Configuration de la traînée
const int MAX_TRAIL_POINTS = 30;

int main() {
    const int screenWidth = 1024;
    const int screenHeight = 768;
    InitWindow(screenWidth, screenHeight, "Simulation Atomique Avancee - C++ & Raylib");

    // Caméra 3D
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 12.0f, 18.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    int currentAtomIndex = 1; // Par défaut : Lithium
    std::vector<Electron> electrons = CreateElectrons(ATOMS[currentAtomIndex]);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Mode caméra orbitale automatique avec la souris
        UpdateCamera(&camera, CAMERA_ORBITAL);

        float deltaTime = GetFrameTime();

        // --- GESTION DU MENU 2D (Clics de souris) ---
        Vector2 mousePos = GetMousePosition();
        for (size_t i = 0; i < ATOMS.size(); i++) {
            // Zone cliquable pour chaque bouton du menu
            Rectangle btnRect = { 20.0f, 80.0f + i * 45.0f, 180.0f, 35.0f };
            if (CheckCollisionPointRec(mousePos, btnRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (currentAtomIndex != (int)i) {
                    currentAtomIndex = i;
                    electrons = CreateElectrons(ATOMS[currentAtomIndex]); // Recréer les électrons
                }
            }
        }

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
                int totalNucleons = ATOMS[currentAtomIndex].protons + ATOMS[currentAtomIndex].neutrons;
                // Génération visuelle pseudo-aléatoire fixe pour le noyau
                for (int i = 0; i < totalNucleons; i++) {
                    float offset = (float)i * 0.5f;
                    float nx = sinf(offset * 3.0f) * 0.4f;
                    float ny = cosf(offset * 2.0f) * 0.4f;
                    float nz = sinf(offset * 1.5f) * 0.4f;
                    Color nColor = (i % 2 == 0 && ATOMS[currentAtomIndex].protons > i/2) ? RED : GRAY;
                    DrawSphere((Vector3){ nx, ny, nz }, 0.5f, nColor);
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

            // Dessin des boutons du menu
            for (size_t i = 0; i < ATOMS.size(); i++) {
                Rectangle btnRect = { 20.0f, 80.0f + i * 45.0f, 180.0f, 35.0f };
                bool isHovered = CheckCollisionPointRec(mousePos, btnRect);
                bool isCurrent = ((int)i == currentAtomIndex);

                // Couleur de fond du bouton dynamique
                Color btnColor = isCurrent ? DARKBLUE : (isHovered ? GetColor(0x333333FF) : GetColor(0x151515FF));
                Color textColor = isCurrent ? WHITE : (isHovered ? GOLD : LIGHTGRAY);

                DrawRectangleRec(btnRect, btnColor);
                DrawRectangleLinesEx(btnRect, 1, isCurrent ? BLUE : DARKGRAY);
                DrawText(ATOMS[i].name, 35, 88 + i * 45, 15, textColor);
            }

            // Panneau d'informations sur l'atome sélectionné (En bas à gauche)
            DrawRectangle(20, 600, 260, 140, GetColor(0x111111CC));
            DrawRectangleLines(20, 600, 260, 140, DARKGRAY);
            
            DrawText(TextFormat("Configuration: %s", ATOMS[currentAtomIndex].name), 30, 610, 16, WHITE);
            DrawText(TextFormat("- Protons (Rouge) : %d", ATOMS[currentAtomIndex].protons), 30, 640, 15, RED);
            DrawText(TextFormat("- Neutrons (Gris) : %d", ATOMS[currentAtomIndex].neutrons), 30, 665, 15, GRAY);
            DrawText(TextFormat("- Electrons (Bleu) : %d", ATOMS[currentAtomIndex].electronsK + ATOMS[currentAtomIndex].electronsL), 30, 690, 15, SKYBLUE);
            DrawText(TextFormat("  (Couche K: %d | Couche L: %d)", ATOMS[currentAtomIndex].electronsK, ATOMS[currentAtomIndex].electronsL), 30, 712, 13, BLUE);

            // Aide navigation
            DrawText("Utilisez la souris pour tourner la camera autour de l'atome", 320, 735, 14, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
