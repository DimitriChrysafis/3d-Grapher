#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include "linear_algebra.h"
#include "controls.h"
#include "shapes.h"
#include "constants.h"
#include <iomanip>

using namespace std;
using namespace sf;


void addExtremePointLabels(RenderWindow &window, Font &font, const vector<Vec3> &cubeVertices, const vector<Vector2f> &projectedVertices) {
    for (int index : {0, 1, 2, 3, 4, 5, 6, 7}) {
        Vector2f point = projectedVertices[index];
        stringstream label;
        label << "P" << index << " (" << cubeVertices[index].x << ", " << cubeVertices[index].y << ", " << cubeVertices[index].z << ")";

        Text text(label.str(), font, 15);
        text.setFillColor(Color::White);
        text.setPosition(point.x + LABEL_OFFSET, point.y - LABEL_OFFSET);

        window.draw(text);
    }
}

void handleEvents(RenderWindow &window, float &zoom, bool &isDragging, float &angleX, float &angleY, Vector2i &previousMousePosition, bool &showPyramid, bool &showSphere, bool &showEllipse, bool &showTorus, bool &showEllipsoid) {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed)
            window.close();

        if (event.type == Event::MouseWheelScrolled)
            handleMouseWheelScrolled(event, zoom);

        if (event.type == Event::MouseButtonPressed)
            handleMouseButtonPressed(event, isDragging, previousMousePosition, window);

        if (event.type == Event::MouseButtonReleased)
            handleMouseButtonReleased(event, isDragging);

        if (event.type == Event::MouseMoved)
            handleMouseMoved(event, isDragging, angleX, angleY, previousMousePosition);

        if (event.type == Event::KeyPressed) {
            if (event.key.code == Keyboard::E) showEllipse = !showEllipse;
            if (event.key.code == Keyboard::P) showPyramid = !showPyramid;
            if (event.key.code == Keyboard::S) showSphere = !showSphere;
            if (event.key.code == Keyboard::T) showTorus = !showTorus;
            if (event.key.code == Keyboard::O) showEllipsoid = !showEllipsoid;
        }
    }
}

vector<Vector2f> projectPoints(const vector<Vec3>& points, const Mat3& rotationMatrix, float zoom) {
    vector<Vector2f> projectedPoints;
    for (const Vec3& point : points) {
        Vec3 rotatedPoint = rotationMatrix * point;
        projectedPoints.push_back(project(rotatedPoint, zoom));
    }
    return projectedPoints;
}

int main() {
    ContextSettings settings;
    settings.antialiasingLevel = 8;
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "3D Shapes", Style::Default, settings);
    window.setFramerateLimit(60);

    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }

    vector<Vec3> axes = {
            Vec3(COORD_AXIS_SIZE, 0, 0),
            Vec3(0, COORD_AXIS_SIZE, 0),
            Vec3(0, 0, COORD_AXIS_SIZE)
    };

    vector<Vec3> cubeVertices;
    vector<pair<int, int>> cubeEdges;
    createCube(cubeVertices, cubeEdges);

    vector<Vec3> pyramidVertices;
    vector<pair<int, int>> pyramidEdges;
    createPyramid(pyramidVertices, pyramidEdges);

    vector<Vec3> ellipsePoints3D;
    vector<Vec3> torusPoints3D;
    vector<Vec3> ellipsoidPoints3D;
    vector<pair<int, int>> torusEdges;

    float angleX = 0.0f, angleY = 0.0f;
    float zoom = 200.0f;
    float sphereRadius = 100.0f;
    float ellipseA = 80.0f, ellipseB = 50.0f;
    float torusMajorRadius = 100.0f, torusMinorRadius = 30.0f;
    float ellipsoidA = 100.0f, ellipsoidB = 60.0f, ellipsoidC = 40.0f;
    bool isDragging = false;
    Vector2i previousMousePosition;

    bool showPyramid = true;
    bool showSphere = true;
    bool showEllipse = true;
    bool showTorus = true;
    bool showEllipsoid = true;

    createTorus(torusPoints3D, torusMajorRadius, torusMinorRadius, 30, 30);
    createEllipsoid(ellipsoidPoints3D, ellipsoidA, ellipsoidB, ellipsoidC, 30);
    createEllipse(ellipsePoints3D, ellipseA, ellipseB, 100);

    while (window.isOpen()) {
        handleEvents(window, zoom, isDragging, angleX, angleY, previousMousePosition, showPyramid, showSphere, showEllipse, showTorus, showEllipsoid);

        Mat3 rotationMatrixX = Mat3::rotationMatrix(angleX, Vec3(1, 0, 0));
        Mat3 rotationMatrixY = Mat3::rotationMatrix(angleY, Vec3(0, 1, 0));
        Mat3 rotationMatrix = rotationMatrixY * rotationMatrixX;

        vector<Vec3> spherePoints = generateSpherePoints(sphereRadius, SPHERE_RESOLUTION);

        vector<Vector2f> projectedSpherePoints = showSphere ? projectPoints(spherePoints, rotationMatrix, zoom) : vector<Vector2f>();
        vector<Vector2f> projectedEllipsePoints = showEllipse ? projectPoints(ellipsePoints3D, rotationMatrix, zoom) : vector<Vector2f>();
        vector<Vector2f> projectedTorusPoints = showTorus ? projectPoints(torusPoints3D, rotationMatrix, zoom) : vector<Vector2f>();
        vector<Vector2f> projectedEllipsoidPoints = showEllipsoid ? projectPoints(ellipsoidPoints3D, rotationMatrix, zoom) : vector<Vector2f>();

        vector<Vector2f> projectedAxes = projectPoints(axes, rotationMatrix, zoom);
        vector<Vector2f> projectedCubeVertices = projectPoints(cubeVertices, rotationMatrix, zoom);
        vector<Vector2f> projectedPyramidVertices = showPyramid ? projectPoints(pyramidVertices, rotationMatrix, zoom) : vector<Vector2f>();

        window.clear(Color::Black);

        drawCoordinateAxes(window, projectedAxes);
        drawCube(window, projectedCubeVertices, cubeEdges);
        if (showPyramid) drawPyramid(window, projectedPyramidVertices, pyramidEdges);
        if (showEllipse) drawEllipse(window, projectedEllipsePoints);
        if (showTorus) drawTorus(window, projectedTorusPoints, 30, 30);
        if (showEllipsoid) drawEllipsoid(window, projectedEllipsoidPoints, 30);
        addExtremePointLabels(window, font, cubeVertices, projectedCubeVertices);
        if (showSphere) drawSphere(window, projectedSpherePoints);

        window.display();
    }

    return 0;
}
