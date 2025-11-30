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
void query(const AABB &range, vector<Particle*> &found) {
        if (!boundary.intersects(range)) return;

        for (auto *p : particles)
            if (range.contains(p))
                found.push_back(p);

        if (divided) {
            nw->query(range, found);
            ne->query(range, found);
            sw->query(range, found);
            se->query(range, found);
        }
    }

    void draw() {
        rectangle(boundary.x-boundary.w, boundary.y-boundary.h,
                  boundary.x+boundary.w, boundary.y+boundary.h);

        if (divided) {
            nw->draw(); ne->draw();
            sw->draw(); se->draw();
        }
    }
};

void resolveCollision(Particle &a, Particle &b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dist = sqrt(dx*dx + dy*dy);

    if (dist == 0) return;

    if (dist < a.r + b.r) {
        // Normalisasi normal vector
        float nx = dx / dist;
        float ny = dy / dist;

        // Relative velocity
        float tx = -ny;
        float ty = nx;

        float v1n = a.vx * nx + a.vy * ny;
        float v1t = a.vx * tx + a.vy * ty;
        float v2n = b.vx * nx + b.vy * ny;
        float v2t = b.vx * tx + b.vy * ty;

        // Swap normal velocities (massa sama)
        float temp = v1n;
        v1n = v2n;
        v2n = temp;

        // balik ke koordinat XY
        a.vx = v1n * nx + v1t * tx;
        a.vy = v1n * ny + v1t * ty;
        b.vx = v2n * nx + v2t * tx;
        b.vy = v2n * ny + v2t * ty;

        //partikel supaya ngga lengket satu sama lain
        float overlap = (a.r + b.r - dist) / 2;
        a.x -= overlap * nx;
        a.y -= overlap * ny;
        b.x += overlap * nx;
        b.y += overlap * ny;
    }
}

int main() {
    int width = 900, height = 700;
