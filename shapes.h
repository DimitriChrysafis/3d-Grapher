#ifndef SHAPES_H
#define SHAPES_H

#include <SFML/Graphics.hpp>
#include "linear_algebra.h"
#include "constants.h"
#include <vector>
#include <utility>
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;


/**
 * @brief Generates points on the surface of a sphere.
 *
 * USES SPHERICAL COORDINATES:
 * Points are distributed across the surface based on the GIVEN resolution.
 * you need basic mvc
 * i did the same thing in my model viewer repo where it's just spherical coordinates
 * it is very cool
 *
 * @param radius The radius of the sphere
 * @param resolution the number of points to generate for each angle increment
 * @return a vector of vec3 points representing the sphere's surface
 *
 */
vector<Vec3> generateSpherePoints(float radius, int resolution) {
    vector<Vec3> points;
    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            float theta = (float)i / (resolution - 1) * M_PI;
            float phi = (float)j / (resolution - 1) * 2 * M_PI;
            float x = radius * sin(theta) * cos(phi);
            float y = radius * sin(theta) * sin(phi);
            float z = radius * cos(theta);
            points.push_back(Vec3(x, y, z));
        }
    }
    return points;
}

/***
 * VERTICES ARE CACULATED GIVEN THE CENTER AND SIZE (kinda cool innit)
 * size of the cube is just the lengths of the edges
 * origin can be computed given that just read the code
 */
void createCube(vector<Vec3> &cubeVertices, vector<pair<int, int>> &cubeEdges, Vec3 center = Vec3(0, 0, 0)) {
    cubeVertices = {
            center + Vec3(-CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE),
            center + Vec3(CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE),
            center + Vec3(CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE),
            center + Vec3(-CUBE_SIZE, CUBE_SIZE, -CUBE_SIZE),
            center + Vec3(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE),
            center + Vec3(CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE),
            center + Vec3(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE),
            center + Vec3(-CUBE_SIZE, CUBE_SIZE, CUBE_SIZE)
    };

    cubeEdges = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };
}


/*
 * pramid just read it it's a 3/4 base pyramid
 * :3
 */
void createPyramid(vector<Vec3> &pyramidVertices, vector<pair<int, int>> &pyramidEdges, Vec3 center = Vec3(0, 0, 0)) {
    pyramidVertices = {
            center + Vec3(-PyramidSize, -PyramidSize, -PyramidSize),
            center + Vec3(PyramidSize, -PyramidSize, -PyramidSize),
            center + Vec3(0, PyramidSize, 0),
            center + Vec3(0, -PyramidSize, PyramidSize)
    };

    pyramidEdges = {
            {0, 1}, {1, 2}, {2, 0},
            {0, 3}, {1, 3}, {2, 3}
    };
}

/*
 *
 */

void createEllipse(vector<Vec3> &ellipsePoints, float a, float b, int resolution, Vec3 center = Vec3(0, 0, 0), float angle = 0.0f) {
    ellipsePoints.clear();
    for (int i = 0; i < resolution; ++i) {
        float theta = (float)i / (resolution - 1) * 2 * M_PI;
        float x = a * cos(theta);
        float y = b * sin(theta);
        float xRot = x * cos(angle) - y * sin(angle);
        float yRot = x * sin(angle) + y * cos(angle);
        ellipsePoints.push_back(center + Vec3(xRot, yRot, 0));
    }
}

void createEllipsoid(vector<Vec3> &ellipsoidPoints, float a, float b, float c, int resolution) {
    ellipsoidPoints.clear();
    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            float theta = (float)i / (resolution - 1) * M_PI;
            float phi = (float)j / (resolution - 1) * 2 * M_PI;
            float x = a * sin(theta) * cos(phi);
            float y = b * sin(theta) * sin(phi);
            float z = c * cos(theta);
            ellipsoidPoints.push_back(Vec3(x, y, z));
        }
    }
}

void createTorus(vector<Vec3> &torusPoints, float majorRadius, float minorRadius, int majorResolution, int minorResolution, Vec3 center = Vec3(0, 0, 0)) {
    torusPoints.clear();
    for (int i = 0; i < majorResolution; ++i) {
        float theta = (float)i / (majorResolution - 1) * 2 * M_PI;
        for (int j = 0; j < minorResolution; ++j) {
            float phi = (float)j / (minorResolution - 1) * 2 * M_PI;
            float x = (majorRadius + minorRadius * cos(phi)) * cos(theta);
            float y = (majorRadius + minorRadius * cos(phi)) * sin(theta);
            float z = minorRadius * sin(phi);
            torusPoints.push_back(center + Vec3(x, y, z));
        }
    }
}

Vector2f project(const Vec3& point, float zoom) {
    float scale = zoom / (point.z + 300.0f);
    return Vector2f(point.x * scale + WINDOW_WIDTH / 2, -point.y * scale + WINDOW_HEIGHT / 2);
}

void drawCoordinateAxes(RenderWindow &window, const vector<Vector2f> &projectedAxes) {
    Vertex xAxis[] = {
            Vertex(Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), Color::Red),
            Vertex(projectedAxes[0], Color::Red)
    };
    Vertex yAxis[] = {
            Vertex(Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), Color::Green),
            Vertex(projectedAxes[1], Color::Green)
    };
    Vertex zAxis[] = {
            Vertex(Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), Color::Blue),
            Vertex(projectedAxes[2], Color::Blue)
    };
    window.draw(xAxis, 2, Lines);
    window.draw(yAxis, 2, Lines);
    window.draw(zAxis, 2, Lines);
}

void drawCube(RenderWindow &window, const vector<Vector2f> &projectedCubeVertices, const vector<pair<int, int>> &cubeEdges) {
    for (const auto& edge : cubeEdges) {
        Vertex line[] = {
                Vertex(projectedCubeVertices[edge.first]),
                Vertex(projectedCubeVertices[edge.second])
        };
        window.draw(line, 2, Lines);
    }
}

void drawPyramid(RenderWindow &window, const vector<Vector2f> &projectedPyramidVertices, const vector<pair<int, int>> &pyramidEdges) {
    for (const auto& edge : pyramidEdges) {
        Vertex line[] = {
                Vertex(projectedPyramidVertices[edge.first]),
                Vertex(projectedPyramidVertices[edge.second])
        };
        window.draw(line, 2, Lines);
    }
}

void drawEllipse(RenderWindow &window, const vector<Vector2f> &projectedEllipsePoints) {
    for (size_t i = 0; i < projectedEllipsePoints.size(); ++i) {
        Vertex line[] = {
                Vertex(projectedEllipsePoints[i]),
                Vertex(projectedEllipsePoints[(i + 1) % projectedEllipsePoints.size()])
        };
        window.draw(line, 2, Lines);
    }
}

void drawEllipsoid(RenderWindow &window, const vector<Vector2f> &projectedEllipsoidPoints, int resolution) {
    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            size_t index = i * resolution + j;
            Vertex line[] = {
                    Vertex(projectedEllipsoidPoints[index]),
                    Vertex(projectedEllipsoidPoints[(index + 1) % (resolution * resolution)])
            };
            window.draw(line, 2, Lines);
        }
    }
}

void drawTorus(RenderWindow &window, const vector<Vector2f> &projectedTorusPoints, int majorResolution, int minorResolution) {
    for (int i = 0; i < majorResolution; ++i) {
        for (int j = 0; j < minorResolution; ++j) {
            size_t index = i * minorResolution + j;
            Vertex line[] = {
                    Vertex(projectedTorusPoints[index]),
                    Vertex(projectedTorusPoints[(index + 1) % (majorResolution * minorResolution)])
            };
            window.draw(line, 2, Lines);
        }
    }
}

void drawSphere(RenderWindow &window, const vector<Vector2f> &projectedSpherePoints) {
    for (size_t i = 0; i < projectedSpherePoints.size(); ++i) {
        Vertex line[] = {
                Vertex(projectedSpherePoints[i]),
                Vertex(projectedSpherePoints[(i + 1) % projectedSpherePoints.size()])
        };
        window.draw(line, 2, Lines);
    }
}

#endif

