#pragma once

struct Camera {
    double n, f; // near and far planes distance
    double l, r; // near plane left and right edges coordinates
    double b, t; // near plane bottom and top edges coordinates
};
