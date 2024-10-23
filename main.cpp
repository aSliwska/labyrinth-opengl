#include "pch.h"
#include "main.h"

typedef sf::Event sfe;
typedef sf::Keyboard sfk;

Spherical CAMERA(12.0f, 0.785398f, 0.7f);
Spherical PLAYER_ROTATION(1.0f, 0.9f, 0.0f);
sf::Vector3f PLAYER_POSITION(0.0f, 0.0f, 0.0f);

float TILE_SIZE = 1.6;

void initOpenGL()
{
    float bgColor[] = { 22, 25, 51 };
    glClearColor(bgColor[0] / 256.0, bgColor[1] / 256.0, bgColor[2] / 256.0, 0.0f);

    glLightfv(GL_LIGHT1, GL_AMBIENT, new GLfloat[]{ 1.5f, 1.7f, 5.0f, 1.0f });
    glLightfv(GL_LIGHT1, GL_DIFFUSE, new GLfloat[]{ 1.0f, 1.3f, 5.0f, 1.0f });
    glLightfv(GL_LIGHT1, GL_SPECULAR, new GLfloat[]{ 0.0f, 0.0f, 0.0f, 1.0f });

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, new GLfloat[]{ 1.0f, 1.3f, 2.0f, 1.0f });

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glMaterialfv(GL_FRONT, GL_AMBIENT, new GLfloat[]{ 1.5f, 1.7f, 5.0f, 1.0f });
    glMaterialfv(GL_FRONT, GL_DIFFUSE, new GLfloat[]{ 1.0f, 1.3f, 5.0f, 1.0f });
    glMaterialfv(GL_FRONT, GL_SPECULAR, new GLfloat[]{ 0.0f, 0.0f, 0.0f, 1.0f });
    glMaterialfv(GL_FRONT, GL_EMISSION, new GLfloat[]{ 0, 0, 0, 1.0f });
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);

    glCullFace(GL_BACK);

    //glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    
}

void reshapeScreen(sf::Vector2u size)
{
    float windowProportions = (GLdouble)size.x / (GLdouble)size.y;

    glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-CAMERA.distance * windowProportions /2, CAMERA.distance * windowProportions /2, -CAMERA.distance /2, CAMERA.distance /2, -10.0f, 1000.0f);
    //gluPerspective(45.0, (GLdouble)size.x / (GLdouble)size.y, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawWorldAxes() {
    glBegin(GL_LINES);
    glColor3d(1.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(1.0, 0.0, 0.0);

    glColor3d(0.0, 1.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 1.0, 0.0);

    glColor3d(0.0, 0.0, 1.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 1.0);
    glEnd();
}

void drawModel(Model* model, float x, float y, float z)
{
    glPushMatrix();
    glTranslatef(x + model->xOffset, y + model->yOffset, z + model->zOffset);

	glBegin(GL_TRIANGLES);

    // Loop over shapes
    for (size_t s = 0; s < model->shapes.size(); s++) 
    {
        // Loop over faces (polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < model->shapes[s].mesh.num_face_vertices.size(); f++)
        {
            size_t fv = size_t(model->shapes[s].mesh.num_face_vertices[f]);

            // TODO most likely not needed
            tinyobj::index_t idx0 = model->shapes[s].mesh.indices[index_offset];
            tinyobj::real_t nx = model->attrib.normals[3 * size_t(idx0.normal_index) + 0];
            tinyobj::real_t ny = model->attrib.normals[3 * size_t(idx0.normal_index) + 1];
            tinyobj::real_t nz = model->attrib.normals[3 * size_t(idx0.normal_index) + 2];
            glNormal3f(nx, ny, nz);
            
            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) 
            {
                // access to vertex
                tinyobj::index_t idx = model->shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = model->attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = model->attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = model->attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                
                tinyobj::real_t tx = model->attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];

                sf::Vector3f color = model->colorMap[tx];
                glColor3f(color.x, color.y, color.z);
                glVertex3f(vx, vy, vz);
            }
            index_offset += fv;
        }
    }
    glEnd();
    glPopMatrix();
}

void drawScene(Map* map, Model* bushModel, Model* characterModel, Model* groundModel, Model* flagModel)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE); // TODO most likely not needed
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(
        CAMERA.getX(), CAMERA.getY(), CAMERA.getZ(),
        0.0, 0.0, 0.0,
        CAMERA.getX(), CAMERA.getY() + 1, CAMERA.getZ()
    );

    glLightfv(GL_LIGHT1, GL_POSITION, new GLfloat[]{ 5.0f, 10.0f, -5.0f, 0.0f });

    glPushMatrix();
    glRotatef(PLAYER_ROTATION.theta, 0, 1, 0);
    drawModel(characterModel, 0, 0, 0);
    glPopMatrix();
    
    // move the world
    glTranslatef(-PLAYER_POSITION.x, -PLAYER_POSITION.y, -PLAYER_POSITION.z);

    float z = 0;
    for (std::vector<Map::Tile> row : map->getTiles())
    {
        float x = 0;
        for (Map::Tile tile : row)
        {
            if (tile != Map::Tile::air)
                drawModel(groundModel, x, 0, z);

            if (tile == Map::Tile::bush) 
                drawModel(bushModel, x, 0, z);
            if (tile == Map::Tile::flag)
                drawModel(flagModel, x, 0, z);

            x += TILE_SIZE;
        }
        z += TILE_SIZE;
    }

    glFlush();
}

int main()
{
    bool running = true;
    float walkSpeed = 1.5f;
    float cameraSpeed = 0.6f;

    Model* bushModel = new Model("resources/models", "bush");
    bushModel->setOffset(-bushModel->xMin, -bushModel->yMin, -bushModel->zMin);
    Model* characterModel = new Model("resources/models", "character_alt");
    characterModel->setOffset(-characterModel->xMin - (characterModel->xMax - characterModel->xMiddle), -characterModel->yMin, -characterModel->zMin - (characterModel->zMax - characterModel->zMiddle));
    Model* groundModel = new Model("resources/models", "flat_ground");
    groundModel->setOffset(-groundModel->xMin, -groundModel->yMin - groundModel->yMax, -groundModel->zMin);
    Model* flagModel = new Model("resources/models", "flag");
    flagModel->setOffset(-flagModel->xMin + TILE_SIZE / 2 - (flagModel->xMiddle - flagModel->xMin), -flagModel->yMin, -flagModel->zMin + TILE_SIZE / 2 - (flagModel->zMiddle - flagModel->zMin));

    float characterRadius = std::min(characterModel->xMax - characterModel->xMin, characterModel->zMax - characterModel->zMin) / 2;

    Map* map = Map::loadMap("resources/test1.txt");
    InputHandler inputHandler = InputHandler(&CAMERA, &PLAYER_POSITION, &PLAYER_ROTATION, walkSpeed, cameraSpeed, TILE_SIZE, map, characterRadius);
    PLAYER_POSITION.x = ((int)map->getStartX() + 0.5) * TILE_SIZE ;
    PLAYER_POSITION.z = ((int)map->getStartZ() - 0.5) * TILE_SIZE;

    sf::ContextSettings context(24, 0, 8, 4, 5);
    sf::RenderWindow window(sf::VideoMode(1280, 1024), "Labyrinth", 7U, context);
    sf::Clock deltaClock;

    ImGui::SFML::Init(window);

    window.setVerticalSyncEnabled(true);

    reshapeScreen(window.getSize());
    initOpenGL();

    while (running)
    {
        sfe event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sfe::Closed) 
            {
                running = false;
            }

            if (event.type == sfe::KeyPressed)
            {
                if (event.key.code == sfk::Escape) {
                    running = false;
                    break;
                }
                if (event.key.code == sfk::A) {
                    inputHandler.setAPressed(true);
                }
                if (event.key.code == sfk::W) {
                    inputHandler.setWPressed(true);
                }
                if (event.key.code == sfk::S) {
                    inputHandler.setSPressed(true);
                }
                if (event.key.code == sfk::D) {
                    inputHandler.setDPressed(true);
                }
            }

            if (event.type == sfe::KeyReleased)
            {
                if (event.key.code == sfk::A) {
                    inputHandler.setAPressed(false);
                }
                if (event.key.code == sfk::W) {
                    inputHandler.setWPressed(false);
                }
                if (event.key.code == sfk::S) {
                    inputHandler.setSPressed(false);
                }
                if (event.key.code == sfk::D) {
                    inputHandler.setDPressed(false);
                }
            }

            if (event.type == sfe::Resized) 
            {
                reshapeScreen(window.getSize());
            }
        }

        reshapeScreen(window.getSize());

        drawScene(map, bushModel, characterModel, groundModel, flagModel);
        inputHandler.handleUserInput(deltaClock.getElapsedTime());
        //std::cout << deltaClock.getElapsedTime().asMilliseconds() << std::endl;
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    delete map;
    return 0;
}