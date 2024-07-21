#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"



#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 4

#define BIG_SHOT true
#define SMALL_SHOT false
#define FORCE_MELEE_DAMAGE 0.5

#define CAMERA_SPEED 0.92f
#define PICKUP_SIZE 24

#define TRIPLE_SHOT 1
#define WAVER 2
#define WALL_CRAWLER 3
#define SHIELD 4

#define SpaceStageSizeX 2500
#define RuinsStageSizeX 2300
#define SpaceExteriorSizeX SCREEN_WIDTH

#define STRAIGHT 0
#define BOUNCING 1
#define SEEKING 2


Scene::Scene()
{
	map = NULL;
	player = NULL;
}


Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i] != NULL)
			delete enemies[i];
	}
	playerShots.clear();
}


void Scene::init(int lives)
{
	cameraSpeed = CAMERA_SPEED;
	cameraX = 0; cameraY = 0;
	gameOver = false;
	this->lives = lives;
	mousePressed = false;
	cooldown = false;
	invulnerable = false;
	timeNotPressed = 0;
	cooldownTime = 200;
	chargeTime = 550;
	timePressed = 0;
	killCounter = 0;
	inBoss = false;
	bossDead = false;
	victory = false;
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram());
	smallShotTex.loadFromFile("images/playerSmallShot.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bigShotTex.loadFromFile("images/playerBigShot.png", TEXTURE_PIXEL_FORMAT_RGBA);

	forcePickUpTexture.loadFromFile("images/forceUpgrade.png", TEXTURE_PIXEL_FORMAT_RGBA);
	forceShot1.loadFromFile("images/forceShot1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	forceShot2.loadFromFile("images/forceShot2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	forceShot3.loadFromFile("images/forceShot3.png", TEXTURE_PIXEL_FORMAT_RGBA);

	enemy1Tex.loadFromFile("images/enemy1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	enemy2Tex.loadFromFile("images/enemy2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	enemy3FloorTex.loadFromFile("images/enemy3Floor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	enemy3CeilingTex.loadFromFile("images/enemy3Ceiling.png", TEXTURE_PIXEL_FORMAT_RGBA);
	enemy4Tex.loadFromFile("images/enemy4.png", TEXTURE_PIXEL_FORMAT_RGBA);

	enemy1Shot.loadFromFile("images/enemy1Shot.png", TEXTURE_PIXEL_FORMAT_RGBA);
	seekingBallTex.loadFromFile("images/seekingBall.png", TEXTURE_PIXEL_FORMAT_RGBA);

	heartTexture.loadFromFile("images/heart.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textureSetUp(glm::ivec2(25, 25), 5);
	godModeTex.loadFromFile("images/godMode.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textureSetUp(glm::ivec2(100, 14), 6);

	spaceBgTexture.loadFromFile("images/spaceTexture.png", TEXTURE_PIXEL_FORMAT_RGBA);
	ruinsBgTexture.loadFromFile("images/ruinsTexture.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textureSetUp(glm::ivec2(SpaceStageSizeX + SpaceExteriorSizeX + RuinsStageSizeX, SCREEN_HEIGHT), 2);
	textureSetUp(glm::ivec2(RuinsStageSizeX, SCREEN_HEIGHT), 3);
	
	backGroundPlanets.loadFromFile("images/backGroundPlanets.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textureSetUp(glm::ivec2(10000, SCREEN_HEIGHT), 7);

	backGroundStars.loadFromFile("images/backGroundStars.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textureSetUp(glm::ivec2(10000, SCREEN_HEIGHT), 4);

	victoryTex.loadFromFile("images/theEnd.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textureSetUp(glm::ivec2(250, 60), 0);

	explosion.loadFromFile("images/explosions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	textureSetUp(glm::ivec2(25, 25), 8);

	if (lives == 1)
	{
		sceneEnd.loadFromFile("images/gameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);
		textureSetUp(glm::ivec2(500, 100), 1);
	}
	else
	{
		sceneEnd.loadFromFile("images/RtoRestart.png", TEXTURE_PIXEL_FORMAT_RGBA);
		textureSetUp(glm::ivec2(300, 40), 1);
	}
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram());
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), SCREEN_HEIGHT / 2));
	player->setTileMap(map);

	force = new Force();
	force->init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram());

	boss = new Boss();
	boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::vec2(SpaceStageSizeX + RuinsStageSizeX + SCREEN_WIDTH - 200, 0), glm::ivec2(200, SCREEN_HEIGHT));

	initializeEnemies();
	initializePickUps();
	currentTime = 0.0f;

	SoundManager::instance().getSoundEngine()->removeAllSoundSources();

	irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/playSong.mp3", false, false, true);
	sound->setVolume(0.3f);
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	spreadUpdate(deltaTime);
	shotSystem(deltaTime);
	updateEnemyShots();
	force->update(deltaTime, player->getPosition(), player->getSize());
	detectColisions();
	if (player->getCrushed()) gameOver = true;
	hotkeySystem();
	prepareBoss();
	updateExplosions(deltaTime);

	cameraX += cameraSpeed;
	ShaderProgramSetter::getInstance().setProjection(cameraX, SCREEN_WIDTH + cameraX, SCREEN_HEIGHT, cameraY);
}

void Scene::render()
{
	ShaderProgramSetter::getInstance().use();
	glm::mat4 backgroundTexture;

	backgroundTexture = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0.f));
	ShaderProgramSetter::getInstance().setModelView(backgroundTexture);
	texQuad[2]->render(spaceBgTexture);

	backgroundTexture = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX/20, 0, 0.f));
	ShaderProgramSetter::getInstance().setModelView(backgroundTexture);
	texQuad[4]->render(backGroundStars);

	backgroundTexture = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX / 7, 0, 0.f));
	ShaderProgramSetter::getInstance().setModelView(backgroundTexture);
	texQuad[7]->render(backGroundPlanets);

	backgroundTexture = glm::translate(glm::mat4(1.0f), glm::vec3(SpaceStageSizeX, 0, 0.f));
	ShaderProgramSetter::getInstance().setModelView(backgroundTexture);
	texQuad[3]->render(ruinsBgTexture);

	glm::mat4 modelview;
	ShaderProgramSetter::getInstance().setModelView(modelview);
	map->render();

	if (gameOver)
	{
		if (lives == 1) modelview = glm::translate(glm::mat4(1.0f), glm::vec3(SCREEN_WIDTH / 2 - 250 + cameraX, SCREEN_HEIGHT / 2 - 50, 0.f));
		else modelview = glm::translate(glm::mat4(1.0f), glm::vec3(SCREEN_WIDTH/2 - 300/2 + cameraX, SCREEN_HEIGHT / 2 - 50, 0.f));
		ShaderProgramSetter::getInstance().setModelView(modelview);
		texQuad[1]->render(sceneEnd);
	}	
	else if (victory)
	{
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(SCREEN_WIDTH / 2 - 250 + cameraX, SCREEN_HEIGHT / 2 - 30, 0.f));
		ShaderProgramSetter::getInstance().setModelView(modelview);
		texQuad[0]->render(victoryTex);

		boss->render();
	}
	else
	{
		int heartSeparation = 40;
		
		for (int i = 0; i < lives; i++)
		{
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(20 + heartSeparation*i + cameraX, 20, 0.f));
			ShaderProgramSetter::getInstance().setModelView(modelview);
			texQuad[5]->render(heartTexture);
		}

		if (invulnerable)
		{
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(20 + cameraX, 60, 0.f));
			ShaderProgramSetter::getInstance().setModelView(modelview);
			texQuad[6]->render(godModeTex);
		}

		for (int i = 0; i < pickUps.size(); i++)
		{
			pickUps[i]->render();
		}

		for (int i = 0; i < enemyExplosions.size(); i++)
		{
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(enemyExplosions[i].explosionLocation.x, enemyExplosions[i].explosionLocation.y, 0.f));
			ShaderProgramSetter::getInstance().setModelView(modelview);
			texQuad[8]->render(explosion);
		}

		for (int i = 0; i < playerShots.size(); i++)
		{
			playerShots[i].render();
		}

		for (int i = 0; i < enemyShots.size(); i++)
		{
			enemyShots[i].render();
		}

		if(force->getForceLevel() > 0) force->render(player->getPosition(), player->getSize());

		boss->render();

		player->render();

		for (int i = 0; i < enemies.size(); i++)
		{
			if (!inBoss && enemies[i]->getEnemyPos().x + enemies[i]->getEnemySize().x > cameraX && enemies[i]->getEnemyPos().x < cameraX + SCREEN_WIDTH)
			{
				enemies[i]->render();
			}
		}
	}
}


void Scene::initializePickUps()
{
	for (int i = pickUps.size(); i >= 1; --i)
	{
		delete pickUps[i - 1];
		pickUps.pop_back();
	}

	PickUpItem *forceUpgrade = new PickUpItem();
	forceUpgrade->init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::ivec2(3155, SCREEN_HEIGHT / 2 - 12), glm::ivec2(PICKUP_SIZE, PICKUP_SIZE), forcePickUpTexture);
	pickUps.push_back(forceUpgrade);

	forceUpgrade = new PickUpItem();
	forceUpgrade->init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::ivec2(1890, 45), glm::ivec2(PICKUP_SIZE, PICKUP_SIZE), forcePickUpTexture);
	pickUps.push_back(forceUpgrade);
}


void Scene::initializeEnemies()
{
	for (int i = enemies.size(); i >= 1; --i)
	{
		delete enemies[i-1];
		enemies.pop_back();
	}

	// init(enemyPos, enemyType, move?, shootCooldown, texture)
	
	//TRIPLE SHOOTERS --------------------------------
	createEnemy(glm::ivec2(1250, 165), TRIPLE_SHOT, true, 1700, enemy1Tex);

	createEnemy(glm::ivec2(1550, 300), TRIPLE_SHOT, true, 1700, enemy1Tex);

	createEnemy(glm::ivec2(2575, 130), TRIPLE_SHOT, false, 1700, enemy1Tex);

	createEnemy(glm::ivec2(2575, 240), TRIPLE_SHOT, false, 1700, enemy1Tex);

	createEnemy(glm::ivec2(2350, 180), TRIPLE_SHOT, true, 1700, enemy1Tex);


	createEnemy(glm::ivec2(3750, SCREEN_HEIGHT / 2 - 50), TRIPLE_SHOT, false, 1700, enemy1Tex);

	createEnemy(glm::ivec2(3750, SCREEN_HEIGHT / 2 + 50), TRIPLE_SHOT, false, 1700, enemy1Tex);

	createEnemy(glm::ivec2(4075, SCREEN_HEIGHT / 2 - 21), TRIPLE_SHOT, false, 1700, enemy1Tex);

	createEnemy(glm::ivec2(4200, SCREEN_HEIGHT / 2 - 21), TRIPLE_SHOT, true, 1700, enemy1Tex);

	createEnemy(glm::ivec2(4575, SCREEN_HEIGHT / 2 - 113), TRIPLE_SHOT, false, 1700, enemy1Tex);

	createEnemy(glm::ivec2(4575, SCREEN_HEIGHT / 2 + 80), TRIPLE_SHOT, false, 1700, enemy1Tex);


	// WAVERS ----------------------------------------

	//pack1
	glm::ivec2 posPack = glm::ivec2 (700, 300);

	createEnemy(glm::ivec2(posPack.x, posPack.y), WAVER, true, 2200, enemy2Tex);

	createEnemy(glm::ivec2(posPack.x+10, posPack.y +10), WAVER, true, 2200, enemy2Tex);

	createEnemy(glm::ivec2(posPack.x+20, posPack.y +20), WAVER, true, 2200, enemy2Tex);

	createEnemy(glm::ivec2(posPack.x+30, posPack.y +30), WAVER, true, 2200, enemy2Tex);

	createEnemy(glm::ivec2(posPack.x+40, posPack.y +40), WAVER, true, 2200, enemy2Tex);


	//pack2
	posPack = glm::ivec2(1150, 200);

	createEnemy(glm::ivec2(posPack.x, posPack.y), WAVER, true, 2200, enemy2Tex);

	createEnemy(glm::ivec2(posPack.x + 10, posPack.y + 10), WAVER, true, 2200, enemy2Tex);

	createEnemy(glm::ivec2(posPack.x + 20, posPack.y + 20), WAVER, true, 2200, enemy2Tex);

	//pack3
	posPack = glm::ivec2(2200, 330);

	createEnemy(glm::ivec2(posPack.x, posPack.y), WAVER, true, 2200, enemy2Tex);

	createEnemy(glm::ivec2(posPack.x + 10, posPack.y + 10), WAVER, true, 2200, enemy2Tex);

	createEnemy(glm::ivec2(posPack.x + 20, posPack.y + 20), WAVER, true, 2200, enemy2Tex);


	// doblePack
	createEnemy(glm::ivec2(posPack.x + 100, posPack.y-57), WAVER, true, 2200, enemy2Tex);

	createEnemy(glm::ivec2(posPack.x + 120, posPack.y - 37), WAVER, true, 2200, enemy2Tex);

	createEnemy(glm::ivec2(posPack.x + 140, posPack.y -17), WAVER, true, 2200, enemy2Tex);

	createEnemy(glm::ivec2(posPack.x + 160, posPack.y + 7), WAVER, true, 2200, enemy2Tex);

	createEnemy(glm::ivec2(posPack.x + 180, posPack.y + 27), WAVER, true, 2200, enemy2Tex);


	//WALL CRAWLERS ------------------------------------
	createEnemy(glm::ivec2(817, 384), WALL_CRAWLER, true, 1000, enemy3FloorTex);

	createEnemy(glm::ivec2(1380, 370), WALL_CRAWLER, true, 1000, enemy3FloorTex);

	createEnemy(glm::ivec2(3300, 333), WALL_CRAWLER, true, 1000, enemy3FloorTex);

	createEnemy(glm::ivec2(3300, 113), WALL_CRAWLER, true, 1000, enemy3CeilingTex);

	createEnemy(glm::ivec2(4075, 64), WALL_CRAWLER, true, 1000, enemy3CeilingTex);

	createEnemy(glm::ivec2(4075, 384), WALL_CRAWLER, true, 1000, enemy3FloorTex);


	//SHIELDS ------------------------------------------


	createEnemy(glm::ivec2(1825, 40), SHIELD, false, 0, enemy4Tex);

	createEnemy(glm::ivec2(2500, 111), SHIELD, true, 0, enemy4Tex);

	createEnemy(glm::ivec2(2500, 265), SHIELD, true, 0, enemy4Tex);

	createEnemy(glm::ivec2(3030, SCREEN_HEIGHT / 2 - 30), SHIELD, false, 0, enemy4Tex);

	createEnemy(glm::ivec2(3060, SCREEN_HEIGHT / 2 - 30), SHIELD, false, 0, enemy4Tex);

	createEnemy(glm::ivec2(3090, SCREEN_HEIGHT / 2 - 30), SHIELD, false, 0, enemy4Tex);

	createEnemy(glm::ivec2(3120, SCREEN_HEIGHT / 2 - 30), SHIELD, false, 0, enemy4Tex);

	createEnemy(glm::ivec2(3060, SCREEN_HEIGHT / 2 - 30), SHIELD, false, 0, enemy4Tex);

	createEnemy(glm::ivec2(3090, SCREEN_HEIGHT / 2 - 30), SHIELD, false, 0, enemy4Tex);

	createEnemy(glm::ivec2(3650, SCREEN_HEIGHT / 2 - 75), SHIELD, true, 0, enemy4Tex);

	createEnemy(glm::ivec2(3650, SCREEN_HEIGHT / 2 + 50), SHIELD, true, 0, enemy4Tex);

	createEnemy(glm::ivec2(4500, SCREEN_HEIGHT / 2 - 105), SHIELD, true, 0, enemy4Tex);

	createEnemy(glm::ivec2(4500, SCREEN_HEIGHT / 2 + 65), SHIELD, true, 0, enemy4Tex);
}


void Scene::spreadUpdate(int deltaTime) 
{
	player->update(deltaTime, cameraSpeed, cameraX);
	for (int i = 0; i < playerShots.size(); i++)
	{
		playerShots[i].update(deltaTime, player->getPosition(), cameraX);
	}

	for (int i = 0; i < enemyShots.size(); i++)
	{
		enemyShots[i].update(deltaTime, player->getPosition(), cameraX);
	}

	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->getEnemyPos().x + enemies[i]->getEnemySize().x > cameraX && enemies[i]->getEnemyPos().x < cameraX + SCREEN_WIDTH)
		{
			enemies[i]->update(deltaTime);
		}
	}

	boss->update(deltaTime);
}


void Scene::shotSystem(int deltaTime)
{
	if (!gameOver && !victory)
	{
		if (Game::instance().getMousePressed())
			mousePressed = true;


		else if (mousePressed && !cooldown)
		{
			bool charged = false;
			glm::ivec2 playerShotStartPos = glm::ivec2(player->getPosition().x + player->getSize().x, player->getPosition().y + player->getSize().y / 2);
			//small shot
			if (timePressed < chargeTime)
			{
				Shot playerShot;
				playerShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(),
					glm::fvec2(1, 0), playerShotStartPos, glm::ivec2(25, 15), SMALL_SHOT, smallShotTex, 1.f, 10, false, STRAIGHT);
				playerShots.push_back(playerShot);
				mousePressed = false;
				cooldown = true;
				timeNotPressed = 0;
				SoundManager::instance().getSoundEngine()->play2D("sounds/lightShot.mp3");
			}
			//charged shot
			else
			{
				Shot playerShot;
				playerShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::fvec2(1, 0), playerShotStartPos, glm::ivec2(62, 30), BIG_SHOT, bigShotTex, 0.18f, 10, false, STRAIGHT);
				playerShots.push_back(playerShot);
				mousePressed = false;
				cooldown = true;
				timeNotPressed = 0;
				//usado para que el tiro del force sepa si has cargado el tiro o no para hacer mas daño tambien
				charged = true;
				SoundManager::instance().getSoundEngine()->play2D("sounds/chargedShot.mp3");
			}

			//force shots
			Shot forceShot;
			glm::ivec2 forceShotStartPos = glm::ivec2(force->getPos().x + force->getSize().x, force->getPos().y + force->getSize().y / 2);
			if (force->getForceLevel() == 1)
			{
				glm::ivec2 shotSize;
				if (charged) shotSize = glm::ivec2(35, 27);
				else shotSize = glm::ivec2(20, 15);
				forceShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::fvec2(1, 0), forceShotStartPos, shotSize, SMALL_SHOT, forceShot1, 0.25f, 6, true, STRAIGHT);
				playerShots.push_back(forceShot);
			}
			else if (force->getForceLevel() == 2)
			{
				glm::ivec2 shotSize;
				if (charged) shotSize = glm::ivec2(35, 27);
				else shotSize = glm::ivec2(20, 15);
				forceShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::fvec2(1, 0), forceShotStartPos, shotSize, SMALL_SHOT, forceShot2, 0.5f, 7, true, STRAIGHT);
				playerShots.push_back(forceShot);
				forceShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::fvec2(1, 0.75f), forceShotStartPos, shotSize, SMALL_SHOT, forceShot2, 0.5f, 7, true, STRAIGHT);
				playerShots.push_back(forceShot);
				forceShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::fvec2(1, -0.75f), forceShotStartPos, shotSize, SMALL_SHOT, forceShot2, 0.5f, 7, true, STRAIGHT);
				playerShots.push_back(forceShot);
			}
			else if (force->getForceLevel() >= 3)
			{
				glm::ivec2 shotSize;
				if (charged) shotSize = glm::ivec2(50, 35);
				else shotSize = glm::ivec2(35, 27);
				forceShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::fvec2(1, 0), forceShotStartPos, shotSize, BIG_SHOT, forceShot3, 0.06f, 8, true, STRAIGHT);
				playerShots.push_back(forceShot);
				forceShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::fvec2(1, 0.75f), forceShotStartPos, shotSize, BIG_SHOT, forceShot3, 0.06f, 8, true, STRAIGHT);
				playerShots.push_back(forceShot);
				forceShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::fvec2(1, -0.75f), forceShotStartPos, shotSize, BIG_SHOT, forceShot3, 0.06f, 8, true, STRAIGHT);
				playerShots.push_back(forceShot);
			}

			timePressed = 0;
		}

		if (mousePressed)
		{
			timePressed += deltaTime;
		}
		if (cooldown) timeNotPressed += deltaTime;
		if (timeNotPressed > cooldownTime) cooldown = false;
	}
}


void Scene::updateEnemyShots()
{
	if (inBoss)
	{
		if (boss->getDropBall())
		{
			Shot bossShot;
			bossShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::ivec2(-1,-1),
				glm::ivec2(boss->getPos().x, boss->getPos().y + boss->getSize().y / 2),
				boss->getSeekingShotSize(), SMALL_SHOT, seekingBallTex, -1, boss->getSeekingShotSpeed(), false, SEEKING);
			enemyShots.push_back(bossShot);
		}

		if (boss->getShoot())
		{
			Shot bossShot;
			bossShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::normalize(glm::fvec2(-rand() % 101, rand() % 101 - 50)),
				glm::ivec2(boss->getPos().x + boss->getSize().x / 5, boss->getPos().y + boss->getSize().y / 3),
				boss->getBouncingShotSize(), SMALL_SHOT, enemy1Shot, -1, boss->getBouncingShotSpeed(), false, BOUNCING);
			enemyShots.push_back(bossShot);
		}
	}
	else
	{

		for (int i = 0; i < enemies.size(); i++) // enemy3Shot
		{
			Enemy *enemy = enemies[i];
			if (enemy->getShoot())
			{
				if (enemy->getEnemyType() == 1)
				{
					glm::fvec2 shotDirection =
						glm::normalize(glm::fvec2(player->getPosition().x + player->getSize().x - enemy->getEnemyPos().x, player->getPosition().y - enemy->getEnemyPos().y + player->getSize().y / 2));

					Shot enemyShot;
					enemyShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), shotDirection,
						glm::ivec2(enemy->getEnemyPos().x, enemy->getEnemyPos().y + enemy->getEnemySize().y / 2),
						enemy->getShotSize(), SMALL_SHOT, enemy1Shot, -1, enemy->getShotSpeed(), false, STRAIGHT);
					enemyShots.push_back(enemyShot);

					shotDirection = glm::normalize(glm::fvec2(player->getPosition().x - enemy->getEnemyPos().x, player->getPosition().y + 150 - enemy->getEnemyPos().y + player->getSize().y / 2));
					enemyShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), shotDirection,
						glm::ivec2(enemy->getEnemyPos().x, enemy->getEnemyPos().y + enemy->getEnemySize().y / 2),
						enemy->getShotSize(), SMALL_SHOT, enemy1Shot, -1, enemy->getShotSpeed(), false, STRAIGHT);
					enemyShots.push_back(enemyShot);

					shotDirection = glm::normalize(glm::fvec2(player->getPosition().x - enemy->getEnemyPos().x, player->getPosition().y - 150 - enemy->getEnemyPos().y + player->getSize().y / 2));
					enemyShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), shotDirection,
						glm::ivec2(enemy->getEnemyPos().x, enemy->getEnemyPos().y + enemy->getEnemySize().y / 2),
						enemy->getShotSize(), SMALL_SHOT, enemy1Shot, -1, enemy->getShotSpeed(), false, STRAIGHT);
					enemyShots.push_back(enemyShot);

				}
				if (enemy->getEnemyType() == 3)
				{
					glm::fvec2 shotDirection =
						glm::normalize(glm::fvec2(player->getPosition().x + player->getSize().x - enemy->getEnemyPos().x, player->getPosition().y - enemy->getEnemyPos().y + player->getSize().y / 2));

					Shot enemyShot;
					enemyShot.init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), shotDirection,
						glm::ivec2(enemy->getEnemyPos().x, enemy->getEnemyPos().y + enemy->getEnemySize().y / 2),
						enemy->getShotSize(), SMALL_SHOT, enemy1Shot, -1, enemy->getShotSpeed(), false, STRAIGHT);
					enemyShots.push_back(enemyShot);
				}
			}
		}
	}
}


void Scene::detectColisions()
{
	//disparo aliado-pared (disminuimos la hitbox por calidad de gameplay) y disparo borde de pantalla
	for (int j = 0; j < playerShots.size(); j++)
	{
		Shot *playerShot = &playerShots[j];
		playerShot->setShotSpeed(playerShot->getShotSpeed());
		if (playerShot->getPos().x > cameraX + SCREEN_WIDTH + 50 || playerShot->getPos().y > +SCREEN_HEIGHT + 50 || playerShot->getPos().y < -50 ||
			map->collisionMoveRight(glm::ivec2(playerShot->getPos().x, playerShot->getPos().y + playerShot->getSize().y / 4), 3 * glm::ivec2(playerShot->getSize().x / 5, playerShot->getSize().y / 6)))
		{
			playerShots.erase(playerShots.begin() + j);
			--j;
		}
	}
	// Fuera de la pelea de boss
	if (!inBoss)
	{
		//colision player-pickUp
		for (int i = 0; i < pickUps.size(); i++)
		{
			if (detectColision(player->getSize(), glm::ivec2(player->getPosition().x + SCREEN_X, player->getPosition().y + +SCREEN_Y), pickUps[i]->getSize(), pickUps[i]->getPos()))
			{
				delete pickUps[i];
				pickUps.erase(pickUps.begin() + i);
				force->upgradeForce();
				--i;
				irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/pickUpSound.mp3", false, false, true);
				sound->setVolume(0.5f);
			}
		}

		for (int j = 0; j < enemyShots.size(); j++)
		{
			Shot *enemyShot = &enemyShots[j];
			//disparo enemigo-pared
			enemyShot->setShotSpeed(enemyShot->getShotSpeed());
			if (map->collisionMoveRight(glm::ivec2(enemyShot->getPos().x, enemyShot->getPos().y + enemyShot->getSize().y / 4), 3 * glm::ivec2(enemyShot->getSize().x / 5, enemyShot->getSize().y / 6)))
			{
				enemyShots.erase(enemyShots.begin() + j);
				--j;
			}
		}

		//colisiones con enemigos
		for (int i = 0; i < enemies.size(); i++)
		{
			Enemy *enemy = enemies[i];
			if (enemy->getEnemyPos().x + enemy->getEnemySize().x > cameraX && enemy->getEnemyPos().x < cameraX + SCREEN_WIDTH)
			{
				bool enemyDead = false;

				//colision enemies-force
				if (force->getForceLevel() > 0 && detectColision(force->getSize(), force->getPos(), enemy->getEnemySize(), enemy->getEnemyPos()))
				{
					if (enemy->gotHit(FORCE_MELEE_DAMAGE)) enemyDead = true;
				}

				for (int j = 0; j < playerShots.size(); j++)
				{
					Shot *playerShot = &playerShots[j];
					//colision playerShots-enemies
					if (detectColision(playerShot -> getSize(), playerShot->getPos(), enemy->getEnemySize(), enemy->getEnemyPos()))
					{
						if (enemy->gotHit(playerShot->getDamage())) enemyDead = true;

						if (playerShot->getBigShot()) playerShot->setShotSpeed(2 * playerShot->getShotSpeed() / 3);

						else
						{
							playerShots.erase(playerShots.begin() + j);
							j--;
						}
					}
				}
				//colision player-enemies
				if (!invulnerable && detectColision(player->getSize(), glm::ivec2(player->getPosition().x + SCREEN_X, player->getPosition().y + SCREEN_Y),
					enemy->getEnemySize(), enemy->getEnemyPos()))
				{
					enemyDead = true;
					gameOver = true;
				}

				if (enemyDead)
				{
					//el septimo  y el 43 enemigo que mates te dropea una mejora de force
					killCounter++;
					if (killCounter == 7 || killCounter == 43)
					{
						PickUpItem *forceUpgrade = new PickUpItem();
						forceUpgrade->init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), enemy->getEnemyPos(),
							glm::ivec2(PICKUP_SIZE, PICKUP_SIZE), forcePickUpTexture);
						pickUps.push_back(forceUpgrade);
					}
					// cada 30 enemigos te sube una vida
					if (killCounter % 30 == 0 && lives < 3)
					{
						lives++;
					}

					enemyExplosions.push_back(EnemyExplosion{ enemies[i]->getEnemyPos(), 300 });

					delete enemies[i];
					enemies.erase(enemies.begin() + i);
					i--;

					irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/explosion.mp3", false, false, true);
					sound->setVolume(0.3f);
				}
			}
		}
	}

	//FINAL BOSS
	else
	{
		for (int j = 0; j < playerShots.size(); j++)
		{
			Shot *playerShot = &playerShots[j];
			if (detectColision(playerShot->getSize(), playerShot->getPos(), glm::ivec2(boss->getSize().x - 120, boss->getSize().y), glm::ivec2(boss->getPos().x + 120, boss->getPos().y)))
			{
				if (boss->gotHit(playerShot->getDamage()))
				{
					bossDead = true;
					inBoss = false;
					victory = true;
					invulnerable = true;
				}
				if (playerShot->getBigShot()) playerShot->setShotSpeed(2 * playerShot->getShotSpeed() / 3);

				else
				{
					playerShots.erase(playerShots.begin() + j);
					j--;
				}
			}
		}

		//colision player-boss
		if (!invulnerable && detectColision(player->getSize(), glm::ivec2(player->getPosition().x + SCREEN_X, player->getPosition().y + SCREEN_Y),
			boss->getSize(), boss->getPos())) gameOver = true;
	}

	//colision enemyShots - force

	for (int i = 0; i < enemyShots.size(); i++)
	{
		Shot *enemyShot = &enemyShots[i];
		if (force->getForceLevel() > 0 && enemyShot->getShotType() != SEEKING &&
			detectColision(glm::ivec2(force->getSize().x / 3, force->getSize().y / 3),
				glm::ivec2(force->getPos().x + force->getSize().x / 3 + SCREEN_X, force->getPos().y + force->getSize().x / 3 + SCREEN_Y),
				enemyShot->getSize(), enemyShot->getPos()))
		{
			enemyShots.erase(enemyShots.begin() + i);
			--i;
		}
	}

	//colision player - enemyShots (hitbox mas pequeña que la textura para que quede mas representativo a lo visual)
	for (int i = 0; i < enemyShots.size(); i++)
	{
		if (!invulnerable && detectColision(glm::ivec2(player->getSize().x / 3, player->getSize().y / 3), 
			glm::ivec2(player->getPosition().x + SCREEN_X + player->getSize().x /4, player->getPosition().y + SCREEN_Y + player->getSize().y / 6),
			enemyShots[i].getSize(), enemyShots[i].getPos()))
		{
			gameOver = true;
		}
			
	}
}


void Scene::hotkeySystem()
{
	if (Game::instance().getKeyPress('1'))
	{
		bossDead = false;
		inBoss = false;
		initializeEnemies();
		enemyShots.clear();
		playerShots.clear();
		boss = new Boss();
		boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::vec2(SpaceStageSizeX + RuinsStageSizeX + SCREEN_WIDTH - 200, 0), glm::ivec2(200, SCREEN_HEIGHT));
		cameraX = 0;
		cameraSpeed = CAMERA_SPEED;
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + cameraX, SCREEN_HEIGHT/2));

		SoundManager::instance().getSoundEngine()->removeAllSoundSources();
		irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/playSong.mp3", true, false, true);
		sound->setVolume(0.3f);
	}
	else if (Game::instance().getKeyPress('2'))
	{
		bossDead = false;
		inBoss = false;
		initializeEnemies();
		enemyShots.clear();
		playerShots.clear();
		boss = new Boss();
		boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::vec2(SpaceStageSizeX + RuinsStageSizeX + SCREEN_WIDTH - 200, 0), glm::ivec2(200, SCREEN_HEIGHT));
		cameraX = SpaceStageSizeX+75;
		cameraSpeed = CAMERA_SPEED;
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + cameraX, SCREEN_HEIGHT /2));

		SoundManager::instance().getSoundEngine()->removeAllSoundSources();
		irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/playSong.mp3", true, false, true);
		sound->setVolume(0.3f);
	}
	else if (Game::instance().getKeyPress('3'))
	{
		bossDead = false;
		inBoss = true;
		initializeEnemies();
		enemyShots.clear();
		playerShots.clear();
		boss = new Boss();
		boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), glm::vec2(SpaceStageSizeX + RuinsStageSizeX + SCREEN_WIDTH - 200, 0), glm::ivec2(200, SCREEN_HEIGHT));
		cameraX = SpaceStageSizeX + RuinsStageSizeX + 1;
		cameraSpeed = CAMERA_SPEED;
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + cameraX, SCREEN_HEIGHT/2));

		SoundManager::instance().getSoundEngine()->removeAllSoundSources();
		irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/bossTheme.mp3", true, false, true);
		sound->setVolume(0.3f);
	}

	if (Game::instance().getKeyPress('f'))
	{
		force -> upgradeForce();
	}
}


bool Scene::getGameOver()
{
	return gameOver;
}


bool Scene::getVictory()
{
	return victory;
}


void Scene::invulnerability()
{
	invulnerable = !invulnerable;
}


bool Scene::detectColision(glm::ivec2 objectSize, glm::ivec2 objectPos, glm::ivec2 targetSize, glm::ivec2 targetPos)
{
	if ((objectPos.x <= targetPos.x + targetSize.x) && (objectPos.x + objectSize.x >= targetPos.x) &&
		(objectPos.y <= targetPos.y + targetSize.y) && (objectPos.y + objectSize.y >= targetPos.y))
	{
		return true;
	}
	return false;
}


void Scene::textureSetUp(glm::ivec2 size, int pos)
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), size };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	texQuad[pos] = TexturedQuad::createTexturedQuad(geom, texCoords, ShaderProgramSetter::getInstance().getShaderProgram());
}


void Scene::createEnemy(glm::ivec2 enemyPos, int enemyType, bool movement, int shootCooldown, Texture &texture)
{
	Enemy *enemy = new Enemy();
	enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderProgramSetter::getInstance().getShaderProgram(), enemyPos, enemyType, movement, shootCooldown, texture);
	enemy->setTileMap(map);
	enemies.push_back(enemy);
}


void Scene::prepareBoss()
{
	if (cameraX <= 4801 && cameraX >= 4800)
	{
		SoundManager::instance().getSoundEngine()->removeAllSoundSources();
		irrklang::ISound* sound = SoundManager::instance().getSoundEngine()->play2D("sounds/bossTheme.mp3", true, false, true);
		sound->setVolume(0.3f);
	}
	else if (cameraX >= 4801)
	{
		cameraSpeed = 0;
		inBoss = true;
	}
}


void Scene::updateExplosions(int deltaTime)
{
	for (int i = 0; i < enemyExplosions.size(); i++)
	{
		enemyExplosions[i].explosionTime -= deltaTime;
		if (enemyExplosions[i].explosionTime <= 0)
		{
			enemyExplosions.erase(enemyExplosions.begin() + i);
			--i;
		}
	}
}