#include <cstdio>
#include <climits>

using namespace std;
const int INF_TIME = INT_MAX;

struct State {
    int time;
    int idx;
};

struct Lift {
    int dest_idx;
    int travel_time;
    int interval;
    int next;
};

struct Heap {
    State* data;
    int size;
    int capacity;
};

void heap_swap(State& a, State& b) {
    State tmp = a; a = b; b = tmp;
}

void sift_up(Heap& h, int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h.data[p].time <= h.data[i].time) break;
        heap_swap(h.data[p], h.data[i]);
        i = p;
    }
}

void sift_down(Heap& h, int i) {
    int n = h.size;
    while (true) {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int smallest = i;
        if (l < n && h.data[l].time < h.data[smallest].time) smallest = l;
        if (r < n && h.data[r].time < h.data[smallest].time) smallest = r;
        if (smallest == i) break;
        heap_swap(h.data[i], h.data[smallest]);
        i = smallest;
    }
}

void heap_init(Heap& h, int capacity) {
    h.data = new State[capacity];
    h.size = 0;
    h.capacity = capacity;
}

bool heap_empty(const Heap& h) {
    return h.size == 0;
}

State heap_top(const Heap& h) {
    return h.data[0];
}

void heap_push(Heap& h, const State& s) {
    if (h.size >= h.capacity) return;
    h.data[h.size] = s;
    sift_up(h, h.size);
    h.size++;
}

void heap_pop(Heap& h) {
    if (h.size == 0) return;
    h.data[0] = h.data[--h.size];
    sift_down(h, 0);
}

void relax_neighbors(int W, int H, const int* heights, int* dist, Heap& heap, int curr_time, int u)
{
    const int dx4[4] = { 1,-1,0,0 };
    const int dy4[4] = { 0,0,1,-1 };
    int ux = u % W, uy = u / W;

    for (int k = 0; k < 4; ++k) {
        int nx = ux + dx4[k];
        int ny = uy + dy4[k];
        if (nx < 0 || nx >= W || ny < 0 || ny >= H) continue;
        int v = ny * W + nx;
        int cost = (heights[v] > heights[u]
            ? (int)(heights[v] - heights[u]) + 1
                : 1);
        int nt = curr_time + cost;
        if (nt < dist[v]) {
            dist[v] = nt;
            heap_push(heap, { nt, v });
        }
    }
}

void relax_lifts(const int* head, const Lift* lifts, int* dist, Heap& heap, int curr_time, int u)
{
    for (int li = head[u]; li != -1; li = lifts[li].next) {
        const Lift& L = lifts[li];
        if (L.interval <= 0) continue;
        int wait = (L.interval - curr_time % L.interval) % L.interval;
        int nt = curr_time + wait + L.travel_time;
        int v = L.dest_idx;
        if (nt < dist[v]) {
            dist[v] = nt;
            heap_push(heap, { nt, v });
        }
    }
}

void run_dijkstra(int W, int H, const int* head, const Lift* lifts, const int* heights, int* dist, Heap& heap, int start, int goal)
{
    while (!heap_empty(heap)) {
        State cur = heap_top(heap);
        heap_pop(heap);
        if (cur.time > dist[cur.idx]) continue;
        if (cur.idx == goal) break;
        relax_neighbors(W, H, heights, dist, heap, cur.time, cur.idx);
        relax_lifts(head, lifts, dist, heap, cur.time, cur.idx);
    }
}

int main() {
    int W, H, sx, sy, dx, dy, num_lifts;
    if (scanf("%d %d %d %d %d %d %d",
        &W, &H, &sx, &sy, &dx, &dy, &num_lifts) != 7) {
        return 0;
    }

    int N = W * H;
    int start = sy * W + sx;
    int goal = dy * W + dx;

    int* head = new int[N];
    int* heights = new int[N];
    int* dist = new int[N];
    Lift* lifts = new Lift[num_lifts];

    for (int i = 0; i < N; ++i) {
        head[i] = -1;
        dist[i] = INF_TIME;
    }

    for (int i = 0; i < num_lifts; ++i) {
        int x1, y1, x2, y2, trav, inter;
        (void)scanf("%d %d %d %d %d %d",
            &x1, &y1, &x2, &y2, &trav, &inter);
        int u = y1 * W + x1;
        lifts[i] = { y2 * W + x2, trav, inter, head[u] };
        head[u] = i;
    }

    for (int i = 0; i < N; ++i) {
        (void)scanf("%d", &heights[i]);
    }

    dist[start] = 0;
    Heap heap;
    heap_init(heap, N);
    heap_push(heap, { 0, start });

    run_dijkstra(W, H, head, lifts, heights, dist, heap, start, goal);

    int ans = dist[goal];
    if (ans == INF_TIME) ans = -1;
    printf("%d\n", ans);

    delete[] head;
    delete[] heights;
    delete[] dist;
    delete[] lifts;
    delete[] heap.data;
    return 0;
}