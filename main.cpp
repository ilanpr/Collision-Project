#include <graphics.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
using namespace std;

struct Particle {
    float x, y;
    float vx, vy;
    float r;
    float mass;

    Particle(float X, float Y, float R) {
        x = X; y = Y; r = R;
        vx = (rand() % 5 - 2);
        vy = (rand() % 5 - 2);
        mass = 1.0f;
    }
};


struct AABB {
    float x, y, w, h;

    bool contains(Particle* p) const {
        return (p->x >= x - w && p->x <= x + w &&
                p->y >= y - h && p->y <= y + h);
    }

    bool intersects(const AABB &o) const {
        return !(o.x - o.w > x + w || o.x + o.w < x - w ||
                 o.y - o.h > y + h || o.y + o.h < y - h);
    }
};


struct QuadTree {
    static const int CAP = 4;
    AABB boundary;
    vector<Particle*> particles;
    bool divided = false;

    QuadTree *nw=NULL, *ne=NULL, *sw=NULL, *se=NULL;

    QuadTree(AABB b) : boundary(b) {}

    void subdivide() {
        float x = boundary.x, y = boundary.y;
        float w = boundary.w/2, h = boundary.h/2;

        nw = new QuadTree({x-w, y-h, w, h});
        ne = new QuadTree({x+w, y-h, w, h});
        sw = new QuadTree({x-w, y+h, w, h});
        se = new QuadTree({x+w, y+h, w, h});

        divided = true;
    }

    bool insert(Particle *p) {
        if (!boundary.contains(p)) return false;

        if (particles.size() < CAP) {
            particles.push_back(p);
            return true;
        }

        if (!divided) subdivide();

        if (nw->insert(p)) return true;
        if (ne->insert(p)) return true;
        if (sw->insert(p)) return true;
        if (se->insert(p)) return true;

        return false;
    }
