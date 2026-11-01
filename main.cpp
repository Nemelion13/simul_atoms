/* Simulation 3D de la rotation des électrons d'un atome autour de son noyau
    Nemelion13
    Octobre 2026
*/
#include "raylib.h"
#include "atoms.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <math.h>
#include <string>
#include <vector>

// Configuration de la traînée
const int MAX_TRAIL_POINTS = 30;

std::string GetToolAssetPath(const char* fileName) {
    const std::string executablePath =
        std::string(GetApplicationDirectory()) + "Tools/" + fileName;
    if (FileExists(executablePath.c_str())) return executablePath;

    const std::string workingDirectoryPath = std::string("Tools/") + fileName;
    if (FileExists(workingDirectoryPath.c_str())) return workingDirectoryPath;

    return std::string("../Tools/") + fileName;
}

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
    const int screenHeight = 700;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Simulation Atomique Avancee - C++ & Raylib");
    SetWindowMinSize(800, 600);

    const char* iconPath = FileExists("Tools/logo.png")
        ? "Tools/logo.png"
        : "../Tools/logo.png";
    if (FileExists(iconPath)) {
        Image windowIcon = LoadImage(iconPath);
        if (windowIcon.data != nullptr) {
            SetWindowIcon(windowIcon);
            UnloadImage(windowIcon);
        } else {
            TraceLog(LOG_WARNING, "Unable to load window icon: %s", iconPath);
        }
    } else {
        TraceLog(LOG_WARNING, "Window icon not found: Tools/logo.png");
    }

    Music backgroundMusic = {};
    Sound buttonSound = {};
    bool hasBackgroundMusic = false;
    bool hasButtonSound = false;
    InitAudioDevice();
    if (IsAudioDeviceReady()) {
        const std::string backgroundMusicPath = GetToolAssetPath("generic.mp3");
        if (FileExists(backgroundMusicPath.c_str())) {
            backgroundMusic = LoadMusicStream(backgroundMusicPath.c_str());
            if (IsMusicValid(backgroundMusic)) {
                backgroundMusic.looping = true;
                PlayMusicStream(backgroundMusic);
                hasBackgroundMusic = true;
            } else {
                TraceLog(LOG_WARNING, "Unable to load background music: %s",
                    backgroundMusicPath.c_str());
            }
        } else {
            TraceLog(LOG_WARNING, "Background music not found: Tools/generic.mp3");
        }

        const std::string buttonSoundPath = GetToolAssetPath("btn_beat.mp3");
        if (FileExists(buttonSoundPath.c_str())) {
            buttonSound = LoadSound(buttonSoundPath.c_str());
            if (IsSoundValid(buttonSound)) {
                hasButtonSound = true;
            } else {
                TraceLog(LOG_WARNING, "Unable to load button sound: %s",
                    buttonSoundPath.c_str());
            }
        } else {
            TraceLog(LOG_WARNING, "Button sound not found: Tools/btn_beat.mp3");
        }
    } else {
        TraceLog(LOG_WARNING, "Audio device is not available.");
    }

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
    float cameraYaw = 0.0f;
    float cameraPitch = atan2f(12.0f, 18.0f);
    float cameraDistance = sqrtf(12.0f * 12.0f + 18.0f * 18.0f);
    bool automaticCameraOrbit = false;
    constexpr float automaticCameraOrbitSpeed = 0.35f;

    // nombre d'images par seconde
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        const float deltaTime = GetFrameTime();
        if (hasBackgroundMusic) UpdateMusicStream(backgroundMusic);
        const Vector2 mousePosition = GetMousePosition();
        const Rectangle autoOrbitButtonBounds = {
            static_cast<float>(GetScreenWidth()) - 210.0f, 20.0f, 190.0f, 30.0f
        };
        const bool mouseOverAutoOrbitButton =
            CheckCollisionPointRec(mousePosition, autoOrbitButtonBounds);
        if (automaticCameraOrbit) {
            cameraYaw += automaticCameraOrbitSpeed * deltaTime;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
            mousePosition.x > 240.0f &&
            !mouseOverAutoOrbitButton) {
            const Vector2 mouseDelta = GetMouseDelta();
            cameraYaw -= mouseDelta.x * 0.005f;
            cameraPitch += mouseDelta.y * 0.005f;
            cameraPitch = fmaxf(-PI / 2.0f + 0.05f,
                fminf(cameraPitch, PI / 2.0f - 0.05f));
        }

        if (mousePosition.x > 240.0f && !mouseOverAutoOrbitButton) {
            cameraDistance -= GetMouseWheelMove() * cameraDistance * 0.1f;
            cameraDistance = fmaxf(5.0f, fminf(cameraDistance, 60.0f));
        }

        camera.position = (Vector3){
            camera.target.x + cameraDistance * cosf(cameraPitch) * sinf(cameraYaw),
            camera.target.y + cameraDistance * sinf(cameraPitch),
            camera.target.z + cameraDistance * cosf(cameraPitch) * cosf(cameraYaw)
        };

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
            const int currentScreenWidth = GetScreenWidth();
            const int currentScreenHeight = GetScreenHeight();

            if (GuiButton(autoOrbitButtonBounds,
                automaticCameraOrbit ? "Arreter la rotation" : "Rotation automatique")) {
                automaticCameraOrbit = !automaticCameraOrbit;
                if (hasButtonSound) PlaySound(buttonSound);
            }

            // 1. RENDU DE LA SCÈNE 3D
            BeginMode3D(camera);

                // Dessin du Noyau Simplifié (Représentation globale compacte)
                int totalNucleons = ATOMS[currentAtomIndex].atomicNumber + ATOMS[currentAtomIndex].neutrons;
                // Génération visuelle pseudo-aléatoire fixe pour le noyau
                for (int i = 0; i < totalNucleons; i++) {
                    float offset = (float)i * 0.5f;
                    float nx = sinf(offset * 3.0f) * 0.4f;
                    float ny = cosf(offset * 2.0f) * 0.3f;
                    float nz = sinf(offset * 1.5f) * 0.35f;
                    Color nColor = (i % 2 == 0 && ATOMS[currentAtomIndex].atomicNumber > i/2) ? RED : RAYWHITE;
                    DrawSphere((Vector3){ nx, ny, nz }, 0.3f, nColor);
                }

                // Dessin des Orbites, Traînées et Électrons
                for (const auto& e : electrons) {
    
                    DrawCircle3D((Vector3){0, 0, 0}, e.radius, {1,0,0}, 90.0f - e.inclination * RAD2DEG, Fade(e.color, 0.08f));

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

            const float atomListHeight = static_cast<float>(currentScreenHeight - 310);
            Rectangle atomListBounds = { 20.0f, 80.0f, 200.0f, atomListHeight };
            int selectedAtomIndex = currentAtomIndex;
            GuiListView(atomListBounds, atomNames.c_str(), &atomListScrollIndex, &selectedAtomIndex);
            if (selectedAtomIndex >= 0 && selectedAtomIndex != currentAtomIndex) {
                currentAtomIndex = selectedAtomIndex;
                electrons = CreateElectrons(ATOMS[currentAtomIndex]);
                if (hasButtonSound) PlaySound(buttonSound);
            }

            const AtomConfig& selectedAtom = ATOMS[currentAtomIndex];
            const std::vector<ElectronicSubshell> electronicConfiguration =
                GetElectronicConfiguration(selectedAtom.atomicNumber);

            // Panneau d'informations sur l'atome sélectionné (En bas à gauche)
            const int infoPanelY = currentScreenHeight - 160;
            DrawRectangle(20, infoPanelY, 260, 140, GetColor(0x111111CC));
            DrawRectangleLines(20, infoPanelY, 260, 140, DARKGRAY);
            
            DrawText(TextFormat("Configuration: %s", selectedAtom.name), 30, infoPanelY + 10, 16, WHITE);
            DrawText(TextFormat("- Protons (Rouge) : %d", selectedAtom.atomicNumber), 30, infoPanelY + 40, 15, RED);
            DrawText(TextFormat("- Neutrons (Blanc) : %d", selectedAtom.neutrons), 30, infoPanelY + 65, 15, GRAY);
            DrawText(TextFormat("- Electrons : %d", selectedAtom.atomicNumber), 30, infoPanelY + 90, 15, SKYBLUE);

            DrawText("Configuration electronique :", 320, infoPanelY + 55, 16, LIGHTGRAY);
            DrawElectronicConfiguration(electronicConfiguration, 320, infoPanelY + 80, currentScreenWidth);
            // Aide navigation
            DrawText("Clic gauche + deplacement : tourner | Molette : zoom",
                320, currentScreenHeight - 25, 14, GRAY);

        EndDrawing();
    }

    if (hasBackgroundMusic) {
        StopMusicStream(backgroundMusic);
        UnloadMusicStream(backgroundMusic);
    }
    if (hasButtonSound) {
        StopSound(buttonSound);
        UnloadSound(buttonSound);
    }
    if (IsAudioDeviceReady()) CloseAudioDevice();
    CloseWindow();
    return 0;
}
