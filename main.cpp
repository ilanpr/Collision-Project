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
    initwindow(width, height,);

    int n;
    cout << "Jumlah Partikel: ";
    cin >> n;

    vector<Particle> P;
    for (int i = 0; i < n; i++)
        P.emplace_back(rand()%width, rand()%height, 8);

    bool useQuad = true;
    bool showTree = true;

    int page = 0;

    while (true) {
        setactivepage(page);
        setvisualpage(1 - page);
        cleardevice();

        // update posisi
        for (auto &p : P) {
            p.x += p.vx;
            p.y += p.vy;

            if (p.x < p.r || p.x > width - p.r) p.vx *= -1;
            if (p.y < p.r || p.y > height - p.r) p.vy *= -1;
        }

        // collision
        if (!useQuad) {
            for (int i=0;i<n;i++)
                for (int j=i+1;j<n;j++)
                    resolveCollision(P[i], P[j]);
        }
        else {
            QuadTree qt({width/2, height/2, width/2, height/2});

            for (auto &p : P)
                qt.insert(&p);

            for (auto &p : P) {
                AABB range = {p.x, p.y, 40, 40};
                vector<Particle*> cand;
                qt.query(range, cand);

                for (auto *o : cand)
                    if (&p != o)
                        resolveCollision(p, *o);
            }

            if (showTree) qt.draw();
        }

        // gambar particle
        for (auto &p : P) {
            setcolor(WHITE);
            circle(p.x, p.y, p.r);

            setfillstyle(SOLID_FILL, WHITE);
            floodfill(p.x, p.y, WHITE);
        }

        setcolor(YELLOW);
        outtextxy(10, 10, useQuad ? (char*)"Mode: QUADTREE" : (char*)"Mode: BRUTEFORCE");

        // input
        if (kbhit()) {
            char c = getch();
            if (c=='q') useQuad = true;
            if (c=='b') useQuad = false;
            if (c=='t') showTree = !showTree;
        }

        page = 1 - page;
        delay(10);
    }

    closegraph();
    return 0;
}
