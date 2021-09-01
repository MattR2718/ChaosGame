//Chaos Game Steps:
//1: Set x seed points
//2: Pick random point in space
//3: Pick random number, y, between 0 and x -1
//4: Move random point half way towards seed point num y
//5: Repeat 3 and 4, z times

#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>
#include <time.h>

//Structure to store rgb values of different colours to convert between HSV values
struct rgb {
    int r = 255;
    int g = 255;
    int b = 255;
};

//Comverts HSV values to RGB values
void HSVtoRGB(float H, float S, float V, rgb& colour) {
    if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
        std::cout << "The givem HSV values are not in valid range \n";
        return;
    }
    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;
    if (H >= 0 && H < 60) {
        r = C, g = X, b = 0;
    }
    else if (H >= 60 && H < 120) {
        r = X, g = C, b = 0;
    }
    else if (H >= 120 && H < 180) {
        r = 0, g = C, b = X;
    }
    else if (H >= 180 && H < 240) {
        r = 0, g = X, b = C;
    }
    else if (H >= 240 && H < 300) {
        r = X, g = 0, b = C;
    }
    else {
        r = C, g = 0, b = X;
    }
    int R = (r + m) * 255;
    int G = (g + m) * 255;
    int B = (b + m) * 255;

    colour.r = R;
    colour.g = G;
    colour.b = B;
}

//Clears out array of points
//Either fully clears or clears all non seedpoints based on full
void reset(std::vector<std::pair<int, int>>& points, int& numSeeds, bool full) {
    if (full) {
        points.erase(points.begin(), points.end());
    }
    else {
        points.erase(points.begin() + numSeeds, points.end());
    }
}

//Prints number of points added each frame and maximum points
void pointNumsShow(int& ppf, int& mp) {
    std::cout << "Points Per Frame: " << ppf << "\nMax Points: " << mp << "\n\n";
}

void showRule(int& cr) {
    std::cout << "Rules: ";
    switch (cr) {
    case 0:
        std::cout << "A point inside repeatedly jumps half of the distance towards a randomly chosen vertex\n";
        break;
    case 1:
        std::cout << "A point inside repeatedly jumps half of the distance towards a randomly chosen vertex, current vertex cannot be chosen in the next iteration\n";
        break;
    case 2:
        std::cout << "A point inside repeatedly jumps half of the distance towards a randomly chosen vertex, current vertex cannot be one place away (anti-clockwise) from the previously chosen vertex\n";
        break;
    case 3:
        std::cout << "A point inside repeatedly jumps half of the distance towards a randomly chosen vertex, current vertex cannot be 2 places away from the previously chosen vertex.\n";
        break;
    }
    std::cout << '\n';
}

//Creates Sierpinski triangle
//Function for a three seed point shape
//Sets what rule is used for choosing vertecies
//Sets number of seed points
//Sets the seed points
void threeSeed(std::vector<std::pair<int, int>>& points, int& numSeeds, int& rule, const int& WIDTH, const int& HEIGHT, int chosenRule) {
    std::cout << "Three Seed Points\n";
    showRule(chosenRule);
    rule = chosenRule;
    numSeeds = 3;
    points.push_back(std::make_pair(int(WIDTH / 2), 0));
    points.push_back(std::make_pair(WIDTH, HEIGHT));
    points.push_back(std::make_pair(0, HEIGHT));
}

//Function for a four seed point shape
void fourSeed(std::vector<std::pair<int, int>>& points, int& numSeeds, int& rule, const int& WIDTH, const int& HEIGHT, int chosenRule) {
    std::cout << "Four Seed Points\n";
    showRule(chosenRule);
    rule = chosenRule;
    numSeeds = 4;
    points.push_back(std::make_pair(0, 0));
    points.push_back(std::make_pair(WIDTH, 0));
    points.push_back(std::make_pair(WIDTH, HEIGHT));
    points.push_back(std::make_pair(0, HEIGHT));
}

//Function for a five seed point shape
void fiveSeed(std::vector<std::pair<int, int>>& points, int& numSeeds, int& rule, const int& WIDTH, const int& HEIGHT, int chosenRule) {
    std::cout << "Five Seed Points\n";
    showRule(chosenRule);
    rule = chosenRule;
    numSeeds = 5;
    points.push_back(std::make_pair(int(WIDTH / 2), 0));
    points.push_back(std::make_pair(WIDTH, int(0.375 * HEIGHT)));
    points.push_back(std::make_pair(int(0.8 * WIDTH), HEIGHT));
    points.push_back(std::make_pair(int(0.2 * WIDTH), HEIGHT));
    points.push_back(std::make_pair(0, int(0.375 * HEIGHT)));
}

int main() {

    //Constants for Screen Width, Height and point radius
    const int WIDTH = 1000;
    const int HEIGHT = 1000;
    const int r = 1;

    //Boolean used to pause points being added
    bool play = true;
    
    //Keeps track of points added each frame and maximum number in shape
    int pointsPerFrame = 100;
    int maxPoints = 10000;
    pointNumsShow(pointsPerFrame, maxPoints);

    //Colours
    rgb white;
    rgb red;
    red.g = 0;
    red.b = 0;
    rgb green;
    green.r = 0;
    green.b = 0;
    rgb blue;
    blue.r = 0;
    blue.g = 0;
    rgb yellow;
    yellow.b = 0;
    rgb pink;
    pink.g = 0;
    rgb cyan;
    cyan.r = 0;
    rgb colourx;
    rgb coloury;
    int colourCount = 0;
    std::vector<rgb> colours = { white, red, green, blue, yellow, pink, cyan, colourx, coloury };

    //Rules:
    //0: A point inside repeatedly jumps half of the distance towards a randomly chosen vertex
    //1: A point inside repeatedly jumps half of the distance towards a randomly chosen vertex, current vertex cannot be chosen in the next iteration
    //2: A point inside repeatedly jumps half of the distance towards a randomly chosen vertex, current vertex cannot be one place away (anti-clockwise) from the previously chosen vertex
    //3: A point inside repeatedly jumps half of the distance towards a randomly chosen vertex, current vertex cannot be 2 places away from the previously chosen vertex.
    int rule = -1;

    //Jump is 1/percentage to jump towards vertex
    float jump = 1 / 0.5;

    //Variable for number of seed points and a vector containing all points in shape
    int numSeeds;
    std::vector<std::pair<int, int>> points;

    //Keeps track of previous vertex choice for rules 1, 2 and 3
    int prevChoice = -1;
    
    //Functions to choose initial shape shown, uncomment to change
    threeSeed(points, numSeeds, rule, WIDTH, HEIGHT, 0);
    //fourSeed(points, numSeeds, rule, WIDTH, HEIGHT, 1);
    //fourSeed(points, numSeeds, rule, WIDTH, HEIGHT, 2);
    //fourSeed(points, numSeeds, rule, WIDTH, HEIGHT, 3);
    //fiveSeed(points, numSeeds, rule, WIDTH, HEIGHT, 1);
    //fiveSeed(points, numSeeds, rule, WIDTH, HEIGHT, 2);
    //fiveSeed(points, numSeeds, rule, WIDTH, HEIGHT, 3);

    //Seeds random numbers
    srand(time(NULL));

    //Ceate the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Chaos Game");
    window.setFramerateLimit(30);

    //Create a shape for each point
    //sf::CircleShape point(r);
    sf::RectangleShape point(sf::Vector2f(1.0, 1.0));

    //Get random point and add to points list
    std::pair<int, int> rPoint = { int(rand() % WIDTH), int(rand() % HEIGHT) };
    points.push_back(rPoint);

    //Run the program as long as the window is open
    while (window.isOpen())
    {
        //Check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            //"close requested" event: close the window
            if (event.type == sf::Event::Closed)
                window.close();
            //Cycles through colours when mouse clicked
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                colourCount++;
                colourCount %= colours.size();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                colourCount = (colourCount == 0) ? colours.size() - 1 : colourCount - 1;
            }
            //Detects keys pressed
            if (event.type == sf::Event::KeyPressed) {
                switch(event.key.code) {
                //Number keys change which shape is displayed on screen
                case(sf::Keyboard::Num1): {
                    reset(points, numSeeds, true);
                    threeSeed(points, numSeeds, rule, WIDTH, HEIGHT, 0);
                }
                break;
                case(sf::Keyboard::Num2): {
                    reset(points, numSeeds, true);
                    fourSeed(points, numSeeds, rule, WIDTH, HEIGHT, 1);
                }
                break;
                case(sf::Keyboard::Num3): {
                    reset(points, numSeeds, true);
                    fourSeed(points, numSeeds, rule, WIDTH, HEIGHT, 2);
                }
                break;
                case(sf::Keyboard::Num4): {
                    reset(points, numSeeds, true);
                    fourSeed(points, numSeeds, rule, WIDTH, HEIGHT, 3);
                }
                break;
                case(sf::Keyboard::Num5): {
                    reset(points, numSeeds, true);
                    fiveSeed(points, numSeeds, rule, WIDTH, HEIGHT, 1);
                }
                break;
                case(sf::Keyboard::Num6): {
                    reset(points, numSeeds, true);
                    fiveSeed(points, numSeeds, rule, WIDTH, HEIGHT, 3);
                }
                break;
                case(sf::Keyboard::Num7): {
                    reset(points, numSeeds, true);
                    fiveSeed(points, numSeeds, rule, WIDTH, HEIGHT, 2);
                }
                break;
                //Up and Down arrow keys change the number of points added each frame
                case(sf::Keyboard::Up): {
                    pointsPerFrame += 50;
                    pointNumsShow(pointsPerFrame, maxPoints);
                }
                break;
                case(sf::Keyboard::Down): {
                    pointsPerFrame -= 50;
                    pointNumsShow(pointsPerFrame, maxPoints);
                }
                break;
                //Left and Right change the maximum points for each shape
                case(sf::Keyboard::Right): {
                    maxPoints += 500;
                    pointNumsShow(pointsPerFrame, maxPoints);
                }
                break;
                case(sf::Keyboard::Left): {
                    maxPoints -= 500;
                    pointNumsShow(pointsPerFrame, maxPoints);
                }
                break;
                //Space pauses points being added
                case(sf::Keyboard::Space): {
                    play = !play;
                }
                break;
                }
            }
        }

        if (play) {
            //Reset when maxPoints are on screen
            if (points.size() > maxPoints) {
                reset(points, numSeeds, false);
            }

            //Place multiple point per frame
            //Place points based on rule chosen for shape
            for (int i = 0; i < pointsPerFrame; i++) {
                switch (rule) {
                case 0: {
                    int seedChoice = rand() % numSeeds;
                    points.push_back(std::make_pair(int((points[seedChoice].first + points[points.size() - 1].first) / jump), int((points[seedChoice].second + points[points.size() - 1].second) / jump)));
                }
                      break;
                case 1: {
                    int seedChoice = rand() % numSeeds;
                    while (seedChoice == prevChoice) {
                        seedChoice = rand() % numSeeds;
                    }
                    prevChoice = seedChoice;
                    points.push_back(std::make_pair(int((points[seedChoice].first + points[points.size() - 1].first) / jump), int((points[seedChoice].second + points[points.size() - 1].second) / jump)));
                }
                      break;
                case 2: {
                    int seedChoice = rand() % numSeeds;
                    while (seedChoice == prevChoice) {
                        seedChoice = rand() % numSeeds;
                    }
                    prevChoice = (seedChoice == 0) ? 3 : seedChoice - 1;
                    points.push_back(std::make_pair(int((points[seedChoice].first + points[points.size() - 1].first) / jump), int((points[seedChoice].second + points[points.size() - 1].second) / jump)));
                }
                      break;
                case 3: {
                    int seedChoice = rand() % numSeeds;
                    while (seedChoice == prevChoice) {
                        seedChoice = rand() % numSeeds;
                    }
                    prevChoice = (seedChoice + 2) % numSeeds;
                    points.push_back(std::make_pair(int((points[seedChoice].first + points[points.size() - 1].first) / jump), int((points[seedChoice].second + points[points.size() - 1].second) / jump)));
                }
                      break;
                default: {
                    int seedChoice = rand() % numSeeds;
                    points.push_back(std::make_pair(int((points[seedChoice].first + points[points.size() - 1].first) / 2), int((points[seedChoice].second + points[points.size() - 1].second) / 2)));
                }
                       break;
                }

            }
        }
        
        //Clear the window with black color
        window.clear(sf::Color::Black);

        //Draw
        //Draw seed points
        for (auto& p : points) {
            if (colourCount == 7) {
                HSVtoRGB(p.first * 360 / WIDTH, 100, 100, colourx);
                point.setFillColor(sf::Color(colourx.r, colourx.g, colourx.b));
            }else if (colourCount == colours.size() - 1) {
                HSVtoRGB(p.second * 360 / HEIGHT, 100, 100, colours[colourCount]);
                point.setFillColor(sf::Color(colours[colourCount].r, colours[colourCount].g, colours[colourCount].b));
            }
            else {
                point.setFillColor(sf::Color(colours[colourCount].r, colours[colourCount].g, colours[colourCount].b));
            }
            point.setPosition(sf::Vector2f(p.first - r, p.second - r));
            //po.setPosition(sf::Vector2f(p.first - r, p.second - r));
            window.draw(point);
            //window.draw(po);
        }

        //End frame
        window.display();
    }
}