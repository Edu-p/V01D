#include "utils.h"

extern const int screenWidth;
extern const int screenHeight;
extern Player player;
extern Camera2D camera;
extern Font font;
extern float deltaTime;
extern Texture2D player_textures[];
extern Animation player_animations[];

static Texture2D *bg;
static Collider2D *colliders;
static size_t *colliders_length;
static char *file_name;
static bool transition = false;
static bool sign_colliding = false;
static bool ladder_colliding = false;
static float duration = 3.0f;
static float alpha = 1.0f;

static float invert_factor = -1.0f;
static int frame_counter = 0;


static Rectangle flippedRectangle(Rectangle rect) {
    Rectangle result;
    result.x = rect.x;
    result.y = rect.y;
    result.width = -rect.width;
    result.height = rect.height;
    return  result;
}



static void setupPhase1(void) {
    //Creating File
    file_name = "load_textures.asm";
    char txt[100] = "\tglobal _game\n\textern _swap_textures\n_game:\n\tcall _swap_textures\nmessage:\t\ndb 'Textures Loaded'\n";
    SaveFileText(file_name, txt);

    //Dynamic Allocation - Phase 1
    bg = (Texture2D*) malloc(sizeof(Texture2D));
    colliders = (Collider2D*) malloc(3*sizeof(Collider2D));
    colliders_length = (size_t*) malloc(sizeof(size_t));
    *colliders_length = 3;
    *bg = LoadTexture(MAPS_DIR"/level2/phase1/final.png");

    //Ground
    colliders[0].colliderType = GROUND;
    colliders[0].collider.x = 0.0f;
    colliders[0].collider.height = (float) BLOCK_SIZE;
    colliders[0].collider.y = (float)screenHeight - colliders[0].collider.height;
    colliders[0].collider.width = (float)BLOCK_SIZE * 10.40f;

    //Wall
    colliders[1].colliderType = WALL;
    colliders[1].collider.x = colliders[0].collider.x + colliders[0].collider.width;
    colliders[1].collider.height = (float)BLOCK_SIZE * 8.0f;
    colliders[1].collider.y = (float)screenHeight - colliders[1].collider.height;
    colliders[1].collider.width = (float)BLOCK_SIZE * 4.5f;

    //Sign
    colliders[2].colliderType = TRIGGER_SIGN;
    colliders[2].collider.x = (float)BLOCK_SIZE * 5.15f;
    colliders[2].collider.height = (float)BLOCK_SIZE * 1.15f;
    colliders[2].collider.y = (float)screenHeight - (colliders[0].collider.height + colliders[2].collider.height);
    colliders[2].collider.width = (float)BLOCK_SIZE * 0.65f;

    //Set Player Position and Shoot
    setPlayerPosition(&player, (Vector2){0, (float) (screenHeight - (BLOCK_SIZE + player.texture->height))});
    setShoot(&player);

}


static void setupPhase2(void) {
    //Change invert_factor
    invert_factor = 1.0f;

    //Free previous allocated memory and Unload Texture

    sign_colliding = false;
    clearLevel2();


    //Dynamic Allocation - Phase 2
    bg = (Texture2D*) malloc(sizeof(Texture2D));
    colliders = (Collider2D*) malloc(12 * sizeof(Collider2D));
    colliders_length = (size_t*) malloc(sizeof(size_t));
    *colliders_length = 12; //4 Ground 3 Triggers 9 Platform 1 Wall = 17
    *bg = LoadTexture(MAPS_DIR"/level2/phase2/final.png");

    //Ground Left
    colliders[0].colliderType = GROUND;
    colliders[0].collider.x = 0.0f;
    colliders[0].collider.height = (float) BLOCK_SIZE;
    colliders[0].collider.y = (float)screenHeight - colliders[0].collider.height;
    colliders[0].collider.width = (float)BLOCK_SIZE * 18.35f;

    //Ground Right 1
    colliders[1].colliderType = GROUND;
    colliders[1].collider.x = (float)BLOCK_SIZE * 23.67f;
    colliders[1].collider.height = (float) BLOCK_SIZE;
    colliders[1].collider.y = (float)screenHeight - colliders[1].collider.height;
    colliders[1].collider.width = (float)BLOCK_SIZE * 8.00f;

    //Ground Right 2
    colliders[2].colliderType = GROUND;
    colliders[2].collider.x = (float)BLOCK_SIZE * 32.38f;
    colliders[2].collider.height = (float) BLOCK_SIZE;
    colliders[2].collider.y = (float)screenHeight - colliders[0].collider.height;
    colliders[2].collider.width = (float)BLOCK_SIZE * 1.90f;

    //Ground Right 3
    colliders[3].colliderType = GROUND;
    colliders[3].collider.x = (float)BLOCK_SIZE * 35.00f;
    colliders[3].collider.height = (float) BLOCK_SIZE;
    colliders[3].collider.y = (float)screenHeight - colliders[3].collider.height;
    colliders[3].collider.width = (float)BLOCK_SIZE * 5.00f;


    //Platform 1 (Small Rect)
    colliders[4].colliderType = PLATFORM;
    colliders[4].collider.x = (float)BLOCK_SIZE * 18.90f;
    colliders[4].collider.height = (float)BLOCK_SIZE;
    colliders[4].collider.y = (float)(screenHeight - (2.32 * colliders[4].collider.height));
    colliders[4].collider.width = (float)BLOCK_SIZE * 2.00f;


    //Platform 2 (Square)

    colliders[5].colliderType = PLATFORM;
    colliders[5].collider.x = (float)BLOCK_SIZE * 21.90f;
    colliders[5].collider.height = (float)BLOCK_SIZE;
    colliders[5].collider.y = (float)(screenHeight - (3.23 * colliders[5].collider.height));
    colliders[5].collider.width = (float)BLOCK_SIZE;


    //Platform 3 (Mid Rect)

    colliders[6].colliderType = PLATFORM;
    colliders[6].collider.x = (float)BLOCK_SIZE * 27.60f;
    colliders[6].collider.height = (float)BLOCK_SIZE;
    colliders[6].collider.y = (float)(screenHeight - (4.32 * colliders[6].collider.height));
    colliders[6].collider.width = (float)BLOCK_SIZE * 3.00f;


    //Platform 4 (Square)

    colliders[7].colliderType = PLATFORM;
    colliders[7].collider.x = (float)BLOCK_SIZE * 30.65f;
    colliders[7].collider.height = (float)BLOCK_SIZE;
    colliders[7].collider.y = (float)(screenHeight - (5.28 * colliders[7].collider.height));
    colliders[7].collider.width = (float)BLOCK_SIZE;


    //Platform 5 (Large Rect)

    colliders[8].colliderType = PLATFORM;
    colliders[8].collider.x = (float)BLOCK_SIZE * 31.73f;
    colliders[8].collider.height = (float)BLOCK_SIZE;
    colliders[8].collider.y = (float)(screenHeight - (6.25 * colliders[8].collider.height));
    colliders[8].collider.width = (float)BLOCK_SIZE * 4.00f;


    //Platform 6 (Square)

    colliders[9].colliderType = PLATFORM;
    colliders[9].collider.x = (float)BLOCK_SIZE * 35.83f;
    colliders[9].collider.height = (float)BLOCK_SIZE;
    colliders[9].collider.y = (float)(screenHeight - (5.28 * colliders[9].collider.height));
    colliders[9].collider.width = (float)BLOCK_SIZE;


    //Platform 7

    colliders[10].colliderType = PLATFORM;
    colliders[10].collider.x = (float)BLOCK_SIZE * 36.85f;
    colliders[10].collider.height = (float)BLOCK_SIZE;
    colliders[10].collider.y = (float)(screenHeight - (4.32 * colliders[10].collider.height));
    colliders[10].collider.width = (float)BLOCK_SIZE * 3.00f;

    //Stairs
    colliders[11].colliderType = TRIGGER_LADDER;
    colliders[11].collider.x = (float)BLOCK_SIZE * 26.84f;
    colliders[11].collider.height = (float)BLOCK_SIZE * 2.85f;
    colliders[11].collider.y = (float)(screenHeight - (1.55 * colliders[11].collider.height));
    colliders[11].collider.width = (float)BLOCK_SIZE * 0.6f;


    //Set Player Position and Shoot
    player.onGround = false;
    player.jumping = false;
    player.walking = false;
    player.idle = true;
    player.dir = 1.0f;
    setPlayerPosition(&player, (Vector2){0, (float) (screenHeight - (BLOCK_SIZE + player.texture->height))});
    changeAnimationTo(&player, &player_animations[IDLE]);
    setShoot(&player);

    //Set Enemy Position and Animation
}


void clearLevel2() {
    UnloadTexture(*bg);
    free(bg);
    free(colliders);
    free(colliders_length);
    bg = NULL;
    colliders = NULL;
    colliders_length = NULL;
}


void startLevel2() {
    //Checking whether has already started
    static bool done = false;
    if(done) return;
    setupPhase1();
    done = true;
}


void inputHandlerLevel2() {
    //If it is transitioning, return
    if(transition) return;

    //Store current player velocity
    float vel_x = player.velocity.x;
    float vel_y = player.velocity.y;


    //Store current key states (Array?)
    bool left_down = IsKeyDown(KEY_A);
    bool right_down = IsKeyDown(KEY_D);
    bool jump_pressed = IsKeyPressed(KEY_SPACE);
    bool shoot_pressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool upper_down = IsKeyDown(KEY_W);
    bool upper_pressed = IsKeyPressed(KEY_W);
    bool lower_down = IsKeyDown(KEY_S);
    bool lower_pressed = IsKeyPressed(KEY_S);


    //Set velocity of player
    if(right_down) {
        vel_x = 100.0f*deltaTime*invert_factor;
        player.dir = 1.0f * invert_factor;
    }
    if(left_down) {
        vel_x = -100.0f*deltaTime*invert_factor;
        player.dir = -1.0f * invert_factor;
    }
    if((!left_down && !right_down) ||(left_down && right_down)) {
        vel_x = 0;
        player.walking = false;
        if(!player.idle && !player.jumping) {
            changeAnimationTo(&player, &player_animations[IDLE]);
            player.idle = true;
        }
    } else {
        player.idle = false;
        if(!player.walking && !player.jumping) {
            changeAnimationTo(&player, &player_animations[WALK]);
            player.walking = true;
        }
    }

    //Player is climbing the ladder
    if(ladder_colliding)
    {
        if(upper_pressed && player.onGround) //on the floor, beginning to climb
        {
            player.onGround = false;
            if(upper_down) vel_y = -100*deltaTime;
        }
        if(!player.onGround) //on the ladder
        {
            if (upper_down) vel_y = -100*deltaTime;
            if (lower_down) vel_y = 100*deltaTime;
        }

    }

    //Player has jumped
    if(jump_pressed && player.onGround) {
        if(!player.jumping) {
            changeAnimationTo(&player, &player_animations[JUMP]);
            player.jumping = true;
            player.walking = false;
            player.idle = false;
        }
        player.onGround = false;
        vel_y = -300.0f*deltaTime;
    }

    //Bullet has been shot
    if(shoot_pressed && !player.bullet.active) {
        setShoot(&player);
        player.bullet.active = true;

    }


    //Bullet has been shot
    if(shoot_pressed && !player.bullet.active) { player.bullet.active = true; }

    //After check input, updates player velocity
    setPlayerVelocity(&player, (Vector2){vel_x, vel_y});

}

void updateLevel2() {
    //If it is transitioning, return
    if(transition) return;

    static bool phase_done = false;

    //Move player to next position := (position + velocity)
    movePlayer(&player);

    //Move Animation to next step
    frame_counter++;
    moveAnimation(&player, &frame_counter);

    //If bullet has been shot, update its positions (i. e., shoot)
    if(player.bullet.active) {
        shoot(&player.bullet);
    }

    //If player is not on ground, apply velocity downwards (gravity)
    if(!player.onGround && !ladder_colliding) player.velocity.y += 10.0f*deltaTime;

    //Check if file has been deleted and change phase
    if(!phase_done && !FileExists(file_name)) {
        phase_done = true;
        transition = true;
        setupPhase2();
    }

    UpdatePlayerCamera(&camera, &player, (float)bg->width);
}

void physicsUpdateLevel2() {
    //If it is transitioning, return
    if(transition) return;

    //Clamp map limits - Player
    if(player.position.x < 0) setPlayerPosition(&player, (Vector2){0, player.position.y});

    if(player.position.x + player.collider_rect.width > (float)bg->width) setPlayerPosition(&player, (Vector2){(float)bg->width - player.collider_rect.width, player.position.y});

    //Clamp map limits - Bullet
    if(player.bullet.collider.collider.x < 0) {
        player.bullet.active = false;
    }
    if(player.bullet.collider.collider.x + player.bullet.collider.collider.width > (float)bg->width){
        player.bullet.active = false;
    }

    for(int i = 0; i < *colliders_length; i++) {
        //Player Collisions
        if(CheckCollisionRecs(player.collider_rect, colliders[i].collider)) {
            Rectangle collision_rect = GetCollisionRec(player.collider_rect, colliders[i].collider);

            if(colliders[i].colliderType == GROUND) {
                playerOnCollisionGround(&player, colliders[i].collider, collision_rect);
            }
            else if(colliders[i].colliderType == WALL) {
                playerOnCollisionWall(&player, colliders[i].collider,collision_rect);
            }
            else if(colliders[i].colliderType == PLATFORM) {
                playerOnCollisionPlatform(&player, colliders[i].collider, collision_rect);


            }
            else if(colliders[i].colliderType == TRIGGER_SIGN) {
                sign_colliding = true;
            }
            else if(colliders[i].colliderType == TRIGGER_LADDER) {
                ladder_colliding = true;

            }
        } else {
            if(colliders[i].colliderType == TRIGGER_LADDER) { ladder_colliding = false; }
            if(colliders[i].colliderType == TRIGGER_SIGN) { sign_colliding = false; }
        }
        //Bullet Collisions
        if(colliders[i].colliderType != TRIGGER_SIGN && colliders[i].colliderType != TRIGGER_LADDER) {
            if (CheckCollisionRecs(player.bullet.collider.collider, colliders[i].collider)) {
                player.bullet.active = false;
            }
            //Check Collision with Enemy
        }
    }
}

static void drawColliders() {
    for(int i = 0; i < *colliders_length; i++) {
        DrawRectangleLinesEx(colliders[i].collider, 2, RED);
    }
}


static void showMessage() {
    DrawTexture(*bg, 0, 0, Fade(BLACK, 0.6f));
    Rectangle signMessage = {120, 120, 300, 190};
    DrawRectangleLines((int) signMessage.x, (int) signMessage.y, (int) signMessage.width, (int) signMessage.height,BLACK);
    DrawTextRec(font, "Maybe, just maybe, there is a suspicious file out there...", signMessage, 33.0f, 1.0f, 1, BLACK);
}


void renderLevel2() {
    ClearBackground(WHITE);

    //Draw background
    DrawTexture(*bg, 0, 0, WHITE);

    //Draw player

    if(player.dir < 0) { DrawTextureRec(*player.texture, flippedRectangle(player.src_rect), player.position, WHITE); }
    else { DrawTextureRec(*player.texture, player.src_rect, player.position, WHITE); }


    //If bullet has been shot, draw
    if(player.bullet.active) {
        DrawTexture(*player.bullet.texture, (int)player.bullet.collider.collider.x, (int)player.bullet.collider.collider.y, WHITE);
    }


    //DrawText(TextFormat("(Vx, Vy): %.2f %.2f", player.velocity.x, player.velocity.y), (int)player.position.x, (int)player.position.y - 20, 12, BLUE);

    DrawRectangleLinesEx(player.collider_rect, 2, RED);
    drawColliders();
    DrawRectangleLinesEx(player.bullet.collider.collider, 2, GREEN);
    DrawFPS(0, 0);

    //Sign
    if(sign_colliding) { showMessage(); }

    //Transition (Maybe global variables instead?)
    if(transition) {
        DrawRectangle(0,0, screenWidth, screenHeight, Fade(WHITE, alpha));
        duration -= 0.1f;
        if(duration < 0.01f) {
            alpha -= 0.01f;
            if(alpha < 0.01f) {
                transition = false;
            }
        }
    }
}