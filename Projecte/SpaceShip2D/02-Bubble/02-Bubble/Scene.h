#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Shot.h"
#include "Enemy.h"
#include "Boss.h"
#include "Texture.h"
#include "PickUpItem.h"
#include "Force.h"



// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(int lives);
	void update(int deltaTime);
	void render();
	bool getGameOver();
	bool getVictory();
	void invulnerability();
	bool detectColision(glm::ivec2 objectSize, glm::ivec2 objectPos, glm::ivec2 targetSize, glm::ivec2 targetPos);

private:
	void initializeEnemies();
	void initializePickUps();
	void shotSystem(int deltaTime);
	void updateEnemyShots();
	void spreadUpdate(int deltaTime);
	void detectColisions();
	void hotkeySystem();
	void textureSetUp(glm::ivec2 size, int pos);
	void createEnemy(glm::ivec2 enemyPos, int enemyType, bool movement, int shootCooldown, Texture &texture);
	void prepareBoss();
	void updateExplosions(int deltaTime);

private:
	bool mousePressed, cooldown, gameOver, victory, invulnerable, inBoss, bossDead;
	int timePressed, timeNotPressed, chargeTime, cooldownTime, lives, killCounter;
	float cameraX, cameraY, cameraSpeed, currentTime;
	TileMap *map;
	Player *player;
	Force *force;
	vector<Shot> playerShots;
	vector<Shot> enemyShots;
	vector<Enemy *> allEnemies;
	vector<Enemy *> enemies;
	vector<PickUpItem *> pickUps;
	Boss *boss;
	Texture smallShotTex, bigShotTex, forceShot1, forceShot2, forceShot3, forcePickUpTexture,
			enemy1Tex, enemy2Tex, enemy3FloorTex, enemy3CeilingTex, enemy4Tex,
			sceneEnd, victoryTex, spaceBgTexture, ruinsBgTexture, enemy1Shot, bossTex, 
			seekingBallTex, heartTexture, godModeTex, backGroundStars, backGroundPlanets, explosion;
	TexturedQuad *texQuad[9];

	//para guardar los enemigos derrotados y renderizar una explosion
	struct EnemyExplosion {
		glm::ivec2 explosionLocation;
		int explosionTime;
	};
	vector<EnemyExplosion> enemyExplosions;




};


#endif // _SCENE_INCLUDE

