#include "includes/camera.h"

extern const int screenWidth;
extern const int screenHeight;

//Atualiza a posicao da camera pra seguir o jogador
void UpdatePlayerCamera(Camera2D *camera, Player *player, float screen_edge) {
    camera->target.x = player->position.x;
    camera->offset = (Vector2) {0.5f*(float)screenWidth, 0};
    clampCameraToLimits(camera, screen_edge);
}

//Impede que a camera saia dos limites do cenario
void clampCameraToLimits(Camera2D *camera, float screen_edge) {
    if (camera->target.x - camera->offset.x < 0) {
        camera->target.x = camera->offset.x;
    }

    if ((camera->target.x - camera->offset.x) + (float) screenWidth > screen_edge) {
        camera->target.x = screen_edge - (float) screenWidth + camera->offset.x;
    }
}