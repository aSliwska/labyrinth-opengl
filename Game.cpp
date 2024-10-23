#include "pch.h"
#include "Game.h"

Game::Game()
{
    float walkSpeed = 1.5f;
    camera = std::shared_ptr<LockedSpherical>(new LockedSpherical(12.0f, 0.785398f, 0.7f));
    playerRotation = std::shared_ptr<UnlockedSpherical>(new UnlockedSpherical(1.0f, 0.9f, 0.0f));
    tileSize = 1.6;
    modelRepo = std::unique_ptr<ModelRepository>(new ModelRepository("resources/models", tileSize));
    map = Map::loadMap("resources/test1.txt");

    float characterRadius = std::min(modelRepo->character->xMax - modelRepo->character->xMin, modelRepo->character->zMax - modelRepo->character->zMin) / 2;
    playerPosition = std::shared_ptr<sf::Vector3f>(new sf::Vector3f(((float)map->getStartX() + 0.5) * tileSize, 0.0f, ((float)map->getStartZ() - 0.5) * tileSize));

    inputHandler = std::unique_ptr<InputHandler>(new InputHandler(camera, playerPosition, playerRotation, walkSpeed, tileSize, map, characterRadius));

    /*sf::Shader shader;
    if (!shader.loadFromFile("resources/shaders/instancer.vert", sf::Shader::Vertex)) {
        std::cout << "Failed to load vertex shader." << std::endl;
        delete map;
        return 0;
    }*/
}

void Game::run() 
{
    bool isRunning = true;

    sf::RenderWindow window(sf::VideoMode(1280, 920), "Labyrinth", 7U, sf::ContextSettings(24, 0, 8, 4, 5));
    sf::Clock deltaClock;

    ImGui::SFML::Init(window);

    window.setVerticalSyncEnabled(true);

    reshapeScreen(window.getSize());
    initOpenGL();

    while (isRunning)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                isRunning = false;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) {
                    isRunning = false;
                    break;
                }
                if (event.key.code == sf::Keyboard::A) {
                    inputHandler->setAPressed(true);
                }
                if (event.key.code == sf::Keyboard::W) {
                    inputHandler->setWPressed(true);
                }
                if (event.key.code == sf::Keyboard::S) {
                    inputHandler->setSPressed(true);
                }
                if (event.key.code == sf::Keyboard::D) {
                    inputHandler->setDPressed(true);
                }
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::A) {
                    inputHandler->setAPressed(false);
                }
                if (event.key.code == sf::Keyboard::W) {
                    inputHandler->setWPressed(false);
                }
                if (event.key.code == sf::Keyboard::S) {
                    inputHandler->setSPressed(false);
                }
                if (event.key.code == sf::Keyboard::D) {
                    inputHandler->setDPressed(false);
                }
            }

            if (event.type == sf::Event::Resized)
            {
                reshapeScreen(window.getSize());
            }
        }

        reshapeScreen(window.getSize());
        drawScene();
        inputHandler->handleUserInput(deltaClock.getElapsedTime());

        if (checkWinCondition()) {
            std::cout << "\nYou won!\n" << std::endl;
            isRunning = false;
        }

        //std::cout << deltaClock.getElapsedTime().asMilliseconds() << std::endl;

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}

void Game::initOpenGL()
{
    float bgColor[] = { 22, 25, 51 };
    glClearColor(bgColor[0] / 256.0, bgColor[1] / 256.0, bgColor[2] / 256.0, 0.0f);

    glLightfv(GL_LIGHT1, GL_AMBIENT, new GLfloat[]{ 1.5f, 1.7f, 5.0f, 1.0f });
    glLightfv(GL_LIGHT1, GL_DIFFUSE, new GLfloat[]{ 1.0f, 1.3f, 5.0f, 1.0f });
    glLightfv(GL_LIGHT1, GL_SPECULAR, new GLfloat[]{ 0.0f, 0.0f, 0.0f, 1.0f });
    glLightfv(GL_LIGHT1, GL_POSITION, new GLfloat[]{ 7.07107f, 7.64842f, 6.44218f, 0.f });

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, new GLfloat[]{ 1.0f, 1.3f, 2.0f, 1.0f });

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glMaterialfv(GL_FRONT, GL_AMBIENT, new GLfloat[]{ 1.5f, 1.7f, 5.0f, 1.0f });
    glMaterialfv(GL_FRONT, GL_DIFFUSE, new GLfloat[]{ 1.0f, 1.3f, 5.0f, 1.0f });
    glMaterialfv(GL_FRONT, GL_SPECULAR, new GLfloat[]{ 0.0f, 0.0f, 0.0f, 1.0f });
    glMaterialfv(GL_FRONT, GL_EMISSION, new GLfloat[]{ 0, 0, 0, 1.0f });
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);

    glCullFace(GL_BACK);

    glShadeModel(GL_FLAT);
}

void Game::reshapeScreen(const sf::Vector2u& size)
{
    float windowProportions = (GLdouble)size.x / (GLdouble)size.y;

    glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float halfCameraDistance = camera->getDistance() / 2;
    glOrtho(-halfCameraDistance * windowProportions, halfCameraDistance * windowProportions, -halfCameraDistance, halfCameraDistance, -10.0f, 1000.0f);
    //gluPerspective(45.0, (GLdouble)size.x / (GLdouble)size.y, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Game::drawModel(const std::unique_ptr<Model>& model, float x, float y, float z)
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

void Game::drawScene()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(
        camera->getX(), camera->getY(), camera->getZ(),
        0.0, 0.0, 0.0,
        camera->getX(), camera->getY() + 1, camera->getZ()
    ); // TODO cut xyz camera calculations

    glPushMatrix();
    glRotatef(playerRotation->getTheta(), 0, 1, 0);
    drawModel(modelRepo->character, 0, 0, 0);
    glPopMatrix();

    // move the world
    glTranslatef(-playerPosition->x, -playerPosition->y, -playerPosition->z);

    float z = 0;
    for (std::vector<Map::Tile> row : map->getTiles())
    {
        float x = 0;
        for (Map::Tile tile : row)
        {
            if (tile != Map::Tile::air)
                drawModel(modelRepo->ground, x, 0, z);

            if (tile == Map::Tile::bush)
                drawModel(modelRepo->bush, x, 0, z);

            if (tile == Map::Tile::flag)
                drawModel(modelRepo->flag, x, 0, z);

            x += tileSize;
        }
        z += tileSize;
    }

    glFlush();
}

bool Game::checkWinCondition()
{
    return (*map).getTiles()[std::floor(playerPosition->z / tileSize)][std::floor(playerPosition->x / tileSize)] == Map::Tile::flag;
}