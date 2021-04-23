#pragma once

struct Camera {
    float n, f; // near and far planes distance
    float l, r; // near plane left and right edges coordinates
    float b, t; // near plane bottom and top edges coordinates
};
