#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath>
#include <cstdlib>
using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid
const int icpsize=12;
//const int sizeofblock = 32; // Size of each centipede segment
//const int sizeofhead = 32;
int cp[icpsize][3] = {};
const int playerarea=3;
const int temp1=playerarea * (boxPixelsY);
const int temp2=playerarea * (boxPixelsX);
const int maxBullets = 100;
int mushroomxpos=-1;
int mushroomypos=-1;
 

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
void moveplayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void moveBullet(float bullet[], sf::Clock& bulletClock, float mushroom[][4], int numofmushroom);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite, float mushroom[][4], int numofmushroom);
void createcp(int cp[][3], float mushroom[][4], int numofmushroom);
int centipedespawn(int x1, int y1, float mushroom[][4], int numofmushroom);
void cpmovements(sf::RenderWindow& window, int cp[][3], sf::Sprite& cpbSprite, sf::Sprite& cphSprite, float mushroom[][4], int numofmushroom, bool left, bool right);

void drawcp(sf::RenderWindow& window, int cp[][3], sf::Sprite& cpbSprite, sf::Sprite& cphSprite);
void fireBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite, sf::Clock& bulletClock, float player[], sf::Sprite& playerSprite, float mushroom[][4],int numofmushroom);
void drawmushroom(sf::RenderWindow& window, float mushroom[][4], sf::Sprite& mushroomSprite, int numofmushroom);
void hit(sf::RenderWindow& window, float mushroom[][4], float bullet[], sf::Sprite& mushroomSprite, int numofmushroom, int cp[][3], int icpsize, sf::Sprite& centipedeSprite,int score, float player[]);
void destroymushroom(float mushroom[][4], int destroyposition,sf::RenderWindow& window, sf::Sprite& mushroomSprite, int numofmushroom,int score);
bool isMushroomCollision(int x, int y, float mushroom[][4], int numofmushroom);
bool isColliding(float x1, float y1, float x3, float y3,float x2, float y2, float x4, float y4);
   




int main()
{
	srand(time(0));
	int num=-1;
         bool left=true;
         bool right=false;


	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	// window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(50);

	// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/background.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20)); // Reduces Opacity to 25%

	// Initializing Player and Player Sprites.
	float player[2] = {};
	player[x] = (gameColumns / 2) * boxPixelsX;
	player[y] = resolutionY - boxPixelsY;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Bullet and Bullet Sprites.
	float bullet[3] = {};
	bullet[x] = player[x];
	bullet[y] = player[y] - boxPixelsY;
	bullet[exists] = true;
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	// initializing centipede body
	sf::Texture cpbTexture;
        sf::Sprite cpbSprite;
        cpbTexture.loadFromFile("Textures/c_body_left_walk.png");
        cpbSprite.setTexture(cpbTexture);
        cpbSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
        // initializing centipede head
        sf::Texture cphTexture;
        sf::Sprite cphSprite;
        cphTexture.loadFromFile("Textures/c_head_left_walk.png");
        cphSprite.setTexture(cphTexture);
        cphSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
        sf::Clock cpClock;
        //initializing mushroom sprite and texture
        
        int numofmushroom=std::rand() % 30 + 20;
        float mushroom[numofmushroom][4] = {};                //array for each index or num of mushroom and its positions and its heath
        for (int i=0;i<numofmushroom;i++){
         for(int j=0;j<4;j++){
        if(j==0){                               //initialising and setting up array
        mushroom[i][0]=i;
        }
        if(j==1){
        int randxpos=std::rand() % (resolutionX - (2 * boxPixelsX)) + (boxPixelsX);
        mushroom[i][1]=randxpos;
        }
        if(j==2){
        int randypos=std::rand() % (resolutionY - (2 * temp1)) + (boxPixelsY); //mushroom doesnt spawn in player area
        mushroom[i][2]=randypos;
        }
        if(j==3){
        int mushroomhealth=2;
        mushroom[i][3]=mushroomhealth;      // health
        }
        }
        }
	sf::Texture mushroomTexture;
	sf::Sprite mushroomSprite;
	mushroomTexture.loadFromFile("Textures/mushroom.png");              // initializing its sprites and pngs
	mushroomSprite.setTexture(mushroomTexture);
	mushroomSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
        createcp(cp, mushroom, numofmushroom);
      
      //  sf::Text scoretxt;
       // scoretxt.setCharacterSize(24);
      //  scoretxt.setFillColor(sf::Color::White);
     //   scoretxt.setString("scoretxt: 0");
     //   scoretxt.setPosition(500, 500);

	while(window.isOpen()) {
	         int score = 0;; // Replace this with your actual score variable
               //  scoretxt.setString("Score: " + std::to_string(score));

		///////////////////////////////////////////////////////////////
		//                                                           //
		// Call Your Functions Here. Some have been written for you. //
		// Be vary of the order you call them, SFML draws in order.  //
		//                                                           //
		///////////////////////////////////////////////////////////////

		window.draw(backgroundSprite);
		moveplayer(window, player, playerSprite);
		drawPlayer(window, player, playerSprite);
		drawmushroom(window, mushroom, mushroomSprite, numofmushroom);
		
		drawcp( window, cp, cpbSprite, cphSprite);
		cpmovements(window, cp, cpbSprite, cphSprite, mushroom, numofmushroom, left, right);

                fireBullet(window, bullet, bulletSprite, bulletClock, player, playerSprite, mushroom, numofmushroom);
		if (bullet[exists] == true) {
			moveBullet(bullet, bulletClock, mushroom, numofmushroom);
			drawBullet(window, bullet, bulletSprite, mushroom, numofmushroom);
			hit(window, mushroom, bullet, mushroomSprite, numofmushroom, cp, icpsize, cpbSprite, score, player); // using here so whenever bullet exists it checks for a hit
		}
		
		
                
           //     window.draw(scoretxt);
         //        window.draw(scoretxt);
                
               
               
                
                
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				return 0;
			}
		}		
		window.display();
		window.clear(); //was an error with the bullet fired
		
	}
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}
void moveBullet(float bullet[], sf::Clock& bulletClock, float mushroom[][4],int numofmushroom) {
        for (int i = 0; i < numofmushroom; i++) {
           if((bullet[x] >= mushroom[i][1] - (boxPixelsX / 2)) && (bullet[x] <= mushroom[i][1] + (boxPixelsX / 2)) && (bullet[y] >= mushroom[i][2] - (boxPixelsY / 2)) && (bullet[y] <= mushroom[i][2] + (boxPixelsY / 2)) && (mushroom[i][3]>0)) { //mushroom[i][3] to ignore if it was previously terminated otherwise terminate the bullet as soon as it hits it
           bullet[exists] = false;
           }
        else {
	if (bulletClock.getElapsedTime().asMilliseconds() < 20)
		return;

	bulletClock.restart();
	bullet[y] -= 10;	
	if (bullet[y] < -32)
		bullet[exists] = false;
		}
}
}
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite, float mushroom[][4],int numofmushroom) {
        for (int i = 0; i < numofmushroom; i++) {
           if((bullet[x] >= mushroom[i][1] - (boxPixelsX / 2)) && (bullet[x] <= mushroom[i][1] + (boxPixelsX / 2)) && (bullet[y] >= mushroom[i][2] - (boxPixelsY / 2)) && (bullet[y] <= mushroom[i][2] + (boxPixelsY / 2)) && (mushroom[i][3]>0) ) {
           bullet[exists] = false;
           }
           else{
	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
	}
	}
}
void createcp(int cp[][3], float mushroom[][4], int numofmushroom) {
    int i = 0;
    
    // Randomly choose a position for the centipede head
    do {
        cp[i][x] = (rand() % (resolutionX - (boxPixelsX)) + boxPixelsX);
        cp[i][y] = (rand() % (resolutionY - (4*temp1)) + boxPixelsY) ;
    } while (centipedespawn(cp[i][x], cp[i][y], mushroom, numofmushroom)!=1);  //find a position where the mushroom doesnt collide with the centipede
    for (int j = 0; j < icpsize; j++) {
        cp[i + j][x] = cp[i][x] + j * 32;    // set the rest of the body
        cp[i + j][y] = cp[i][y];
        cp[i + j][exists] = true;
    }
}

int centipedespawn(int x1, int y1, float mushroom[][4], int numofmushroom) {  //check if it doesnt spawn anywhere near where mushroom spawn
    for (int i = 0; i < numofmushroom; i++) {
        if (mushroom[i][3] > 0) { // Mushroom is not destroyed
            if (x1 >= mushroom[i][1] - boxPixelsX && x1 <= mushroom[i][1] + boxPixelsX &&
                y1 >= mushroom[i][2] - boxPixelsY && y1 <= mushroom[i][2] + boxPixelsY) {
                return 0; 
            }
        }
    }
    return 1;
}

void cpmovements(sf::RenderWindow& window, int cp[][3], sf::Sprite& cpbSprite, sf::Sprite& cphSprite, float mushroom[][4], int numofmushroom, bool left, bool right) {
    static sf::Clock movementClock;
    static int level = 1; // Initialize level
    static bool descending = false; // To keep track of whether the centipede is descending

    if (movementClock.getElapsedTime().asMilliseconds() < 500) {
        return;
    }

    movementClock.restart();

    static bool rc = false;
    

    if (!rc) {
        for (int i = icpsize-1; i > 0; i--) {
            cp[i][x] = cp[i - 1][x];
            cp[i][y] = cp[i - 1][y];
        }

        if (cp[0][x] < (resolutionX - boxPixelsX) && (centipedespawn(cp[0][x], cp[0][y], mushroom, numofmushroom)==1)) {
            cp[0][x] += 32;
        } else {
            rc = true;
            cp[0][y] += 32;
        }
    } else {
        for (int i = icpsize - 1; i > 0; --i) {
            cp[i][x] = cp[i - 1][x];
            cp[i][y] = cp[i - 1][y];
        }

        if (cp[0][x] > 0 && (centipedespawn(cp[0][x], cp[0][y], mushroom, numofmushroom)==1)) {
            cp[0][x] -= 32;
        } else {
            rc = false;
            cp[0][y] += 32;
        }
    }
      if(cp[0][y]>=(resolutionY-(2*boxPixelsY))){
       if (cp[0][x] > 0 && (cp[0][x] <= (resolutionX - boxPixelsX)/2)){
         cp[0][x] -= 32;
       }
       else{
        cp[0][x] +=32;
       }
      }
    window.display();
    drawcp(window, cp, cpbSprite, cphSprite);
}







void drawcp(sf::RenderWindow& window, int cp[][3], sf::Sprite& cpbSprite, sf::Sprite& cphSprite) {
    for (int i = 0; i < icpsize; ++i) {
        if (cp[i][exists]) {
            if (i == 0) {
                cphSprite.setPosition(cp[i][x], cp[i][y]);
                window.draw(cphSprite);
            } else {
                cpbSprite.setPosition(cp[i][x], cp[i][y]);
                window.draw(cpbSprite);
            }
        }
    }
}



void moveplayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
    // Handle player input for movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player[x] > 0) {
        // Move left within the player area
        player[x]=player[x]-1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player[x] < resolutionX - boxPixelsX){
        // Move right within the player area
        player[x]=player[x]+1; 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        // Move up
        if(player[y]>resolutionY - temp1){
        player[y]=player[y]-1;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player[y] < resolutionY - boxPixelsY) {
        // Move down
        player[y]=player[y]+1; 
    }
    drawPlayer(window, player, playerSprite);
}
void fireBullet(sf::RenderWindow& window, float bullet[],sf::Sprite& bulletSprite, sf::Clock& bulletClock, float player[], sf::Sprite& playerSprite, float mushroom[][4],int numofmushroom) {
    sf::Time t = bulletClock.getElapsedTime();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && t.asMilliseconds() > 60) {  //chose 60 cuz with 30 there was a glitch where down and left keys made it go faster.
        bulletClock.restart();
        bullet[x] = player[x];
        bullet[y] = player[y] - boxPixelsY;
        bullet[exists] = true;
        moveBullet(bullet, bulletClock, mushroom, numofmushroom);
	drawBullet(window, bullet, bulletSprite, mushroom, numofmushroom);
	}
    }




void drawmushroom(sf::RenderWindow& window, float mushroom[][4], sf::Sprite& mushroomSprite, int numofmushroom) {
    for (int i = 0; i < numofmushroom; i++) {
        if (mushroom[i][3] > 0) {  // Draw mushrooms with health greater than 0
            mushroomSprite.setPosition(mushroom[i][1], mushroom[i][2]);
            window.draw(mushroomSprite);
        }
    }
}

// for what happens when a bullet hits smthn

void hit(sf::RenderWindow& window, float mushroom[][4], float bullet[], sf::Sprite& mushroomSprite, int numofmushroom, int cp[][3], int icpsize, sf::Sprite& centipedeSprite,int score, float player[]) {
    //for the mushroom being hit 
    int destroyposition=-1;
    for (int i = 0; i < numofmushroom; i++) {
        if (mushroom[i][3] > 0){ // Mushroom is not destroyed
           if((bullet[x] >= mushroom[i][1] - (boxPixelsX / 2)) && (bullet[x] <= mushroom[i][1] + (boxPixelsX / 2)) && (bullet[y] >= mushroom[i][2] - (boxPixelsY / 2)) && (bullet[y] <= mushroom[i][2] + (boxPixelsY / 2))) {
            mushroom[i][3]=mushroom[i][3] - 1; // Decrease mushroom health
            if (mushroom[i][3] == 0) {
                destroyposition=i;
            }
            
        }
        }
    }
     for (int i = 1; i < icpsize; ++i) {
     bool collision = isColliding(bullet[x], bullet[y], boxPixelsX, boxPixelsY, cp[i][x], cp[i][y], boxPixelsX, boxPixelsY);
        if (cp[i][exists] && collision) {
            cp[i][exists] = false; // Destroy the  segment
            bullet[exists] = false; //  bullet destroyed
            score +=10;
            //created a new segment after the hit segment
            if (i + 1 < icpsize) {
                cp[i + 1][exists] = true;
               
            }

            // Update the centipede size
            icpsize++;

            break; // Exit the loop after hitting the first segment
        }
    }
    bool collision2=isColliding(bullet[x], bullet[y], boxPixelsX, boxPixelsY, cp[0][x], cp[0][y], boxPixelsX, boxPixelsY);
    if(cp[0][exists] && collision2){
     score +=20;
     for(int i=0;i<icpsize;i++){
     cp[i][exists]=false;
    }
    }
    
  //  for(int i=0;i<icpsize;i++){
  //   if((cp[i][x]>player[x]-32 && cp[i][x]<player[x]+32) || (cp[i][y]>player[y]-32 && cp[i][y]<player[y]+32)){
       
 //     player[exists]=false;
      
 //    }
 //   }
    
    destroymushroom(mushroom, destroyposition, window, mushroomSprite, numofmushroom, score);  

}



void destroymushroom(float mushroom[][4], int destroyposition,sf::RenderWindow& window, sf::Sprite& mushroomSprite, int numofmushroom,int score) {
    mushroom[destroyposition][3] = -1;
     drawmushroom(window, mushroom, mushroomSprite, numofmushroom);
     score +=1;
}


bool isColliding(float x1, float y1, float x3, float y3,
                 float x2, float y2, float x4, float y4) {
    // Check if one rectangle is to the left of the other
    if ((x1 + x3) < x2 || x1 > (x2 + x4)) {
        return false;
    }

    // Check if one rectangle is above the other
    if ((y1 + y3) < y2 || y1 > (y2 + y4)) {
        return false;
    }

    // If neither of the above conditions is true, the rectangles overlap
    return true;
}

