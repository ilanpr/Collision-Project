#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

struct Ball {
    sf::Vector2f pos;
    sf::Vector2f prevPos;
    float r;
    sf::Color color;

    Ball(float x, float y, float radius)
        : pos(x, y), prevPos(x, y), r(radius) 
    {
        color = sf::Color(200, 200, 255);
    }

    void applyVelocity(float dt) {
        sf::Vector2f vel = pos - prevPos;
        prevPos = pos;
        pos += vel;   
    }

    void move(float ax, float ay) {
        pos.x += ax;
        pos.y += ay;
    }

    void draw(sf::RenderWindow &win) {
        sf::CircleShape c(r);
        c.setOrigin(r, r);
        c.setPosition(pos);
        c.setFillColor(color);
        win.draw(c);
    }
};

float length(sf::Vector2f v) {
    return std::sqrt(v.x*v.x + v.y*v.y);
}


void resolveCollision(Ball &a, Ball &b) {
    sf::Vector2f diff = b.pos - a.pos;
    float dist = length(diff);
    float minDist = a.r + b.r;

    if (dist == 0 || dist >= minDist) return;

    float overlap = minDist - dist;
    sf::Vector2f n = diff / dist;

    
    a.pos -= n * (overlap * 0.5f);
    b.pos += n * (overlap * 0.5f);

    
    sf::Vector2f va = a.pos - a.prevPos;
    sf::Vector2f vb = b.pos - b.prevPos;

    float relVel = (vb.x - va.x)*n.x + (vb.y - va.y)*n.y;

    if (relVel < 0) {
        
        float impulse = relVel;
        va += n * impulse;
        vb -= n * impulse;
    }


    a.prevPos = a.pos - va;
    b.prevPos = b.pos - vb;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ball Collision - Reducible Style");
    window.setFramerateLimit(60);

    std::vector<Ball> balls;
    int N = 10;

    // spawn random bola
    for (int i = 0; i < N; i++) {
        float x = rand() % 700 + 50;
        float y = rand() % 500 + 50;
        float r = 20;
        balls.emplace_back(x, y, r);

        //kecepatan random 
        balls[i].prevPos = balls[i].pos - sf::Vector2f(rand()%5-2, rand()%5-2);
    }

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        for (auto &b : balls)
            b.applyVelocity(dt);

        for (int i = 0; i < N; i++)
            for (int j = i+1; j < N; j++)
                resolveCollision(balls[i], balls[j]);

   
        for (auto &b : balls) {
            if (b.pos.x - b.r < 0) {
                b.pos.x = b.r;
                b.prevPos.x = b.pos.x + (b.prevPos.x - b.pos.x) * -0.9f;
            }
            if (b.pos.x + b.r > 800) {
                b.pos.x = 800 - b.r;
                b.prevPos.x = b.pos.x + (b.prevPos.x - b.pos.x) * -0.9f;
            }
            if (b.pos.y - b.r < 0) {
                b.pos.y = b.r;
                b.prevPos.y = b.pos.y + (b.prevPos.y - b.pos.y) * -0.9f;
            }
            if (b.pos.y + b.r > 600) {
                b.pos.y = 600 - b.r;
                b.prevPos.y = b.pos.y + (b.prevPos.y - b.pos.y) * -0.9f;
            }
        }

        window.clear(sf::Color(20, 20, 25));

        for (auto &b : balls)
            b.draw(window);

        window.display();
    }

    return 0;
}
