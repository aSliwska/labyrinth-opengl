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
}

void Game::loadShaders()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    GLuint attribPosition = 0;
    GLuint attribColor = 1;
    GLuint attribNormal = 2;
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);

    GLuint vboOffset = 0;
    modelRepo->character->setVBOOffset(vboOffset);

    vboOffset += modelRepo->character->getVerticesSize();
    modelRepo->bush->setVBOOffset(vboOffset);

    vboOffset += modelRepo->bush->getVerticesSize();
    modelRepo->ground->setVBOOffset(vboOffset);

    vboOffset += modelRepo->ground->getVerticesSize();
    modelRepo->flag->setVBOOffset(vboOffset);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    mergedVertices.insert(mergedVertices.end(), modelRepo->character->getVertices()->begin(), modelRepo->character->getVertices()->end());
    mergedVertices.insert(mergedVertices.end(), modelRepo->bush->getVertices()->begin(), modelRepo->bush->getVertices()->end());
    mergedVertices.insert(mergedVertices.end(), modelRepo->ground->getVertices()->begin(), modelRepo->ground->getVertices()->end());
    mergedVertices.insert(mergedVertices.end(), modelRepo->flag->getVertices()->begin(), modelRepo->flag->getVertices()->end());

    glBufferData(GL_ARRAY_BUFFER, mergedVertices.size() * sizeof(float), &mergedVertices[0], GL_STATIC_DRAW);

    vaoVertexSize = 9;
    glEnableVertexAttribArray(attribPosition);
    glEnableVertexAttribArray(attribColor);
    glEnableVertexAttribArray(attribNormal);
    glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, vaoVertexSize * sizeof(float), (void*)0);
    glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, vaoVertexSize * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(attribNormal, 3, GL_FLOAT, GL_FALSE, vaoVertexSize * sizeof(float), (void*)(6 * sizeof(float)));
    
    if (!shader.loadFromFile("resources/shaders/instancer.vert", "resources/shaders/light.frag"))
    {
        std::cout << "Failed to load shaders." << std::endl;
    }

    shader.setUniform("lightCoords", sf::Vector3f(3.07107f, 3.64842f, 2.44218f));
    shader.setUniform("ambientColor", sf::Vector3f(1.5f, 1.7f, 5.0f));
    shader.setUniform("diffuseColor", sf::Vector3f(5.0f, 5.3f, 25.0f));
    shader.setUniform("bgColor", sf::Vector3f(bgColor[0] / 256.0, bgColor[1] / 256.0, bgColor[2] / 256.0));
}

void Game::run() 
{
    bool isRunning = true;

    sf::RenderWindow window(sf::VideoMode(1180, 820), "Labyrinth", 7U, sf::ContextSettings(24, 0, 8, 4, 5));
    sf::Clock deltaClock, renderClock;

    ImGui::SFML::Init(window);

    window.setVerticalSyncEnabled(true);

    reshapeScreen(window.getSize());
    initOpenGL();
    loadShaders();    

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

        drawScene(renderClock);

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

    sf::Shader::bind(NULL);
    ImGui::SFML::Shutdown();
}

void Game::initOpenGL()
{
    bgColor.insert(bgColor.end(), { 22, 25, 51 });
    glClearColor(bgColor[0] / 256.0, bgColor[1] / 256.0, bgColor[2] / 256.0, 0.0f);

    glCullFace(GL_BACK);
}

void Game::reshapeScreen(const sf::Vector2u& size)
{
    float windowProportions = (GLdouble)size.x / (GLdouble)size.y;
    glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);

    float halfCameraDistance = camera->getDistance() / 2;
    orthogonalProjection = glm::ortho(-halfCameraDistance * windowProportions, halfCameraDistance * windowProportions, -halfCameraDistance, halfCameraDistance, -10.0f, 1000.0f);
}

void Game::drawModel(const std::unique_ptr<Model>& model, float x, float y, float z, glm::mat4 modelTransform, const glm::mat4& orthogonalViewTransform)
{
    modelTransform = glm::translate(modelTransform, glm::vec3(x + model->xOffset, y + model->yOffset, z + model->zOffset));
    shader.setUniform("model", sf::Glsl::Mat4(glm::value_ptr(modelTransform)));

    glDrawArrays(GL_TRIANGLES, model->VBOOffset / vaoVertexSize, model->getVerticesSize() / vaoVertexSize);
}

void Game::drawScene(sf::Clock& renderClock)
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sf::Shader::bind(&shader); // this is an equivalent of glUseProgram and yes, it has to be bound every loop

    // calc elapsed time
    elapsedRenderTime = (elapsedRenderTime + renderClock.getElapsedTime().asSeconds());
    renderClock.restart();
    while (elapsedRenderTime > (43.99315f)) {
        elapsedRenderTime -= (43.99315f);
    }
    shader.setUniform("time", elapsedRenderTime);

    // calculate new camera position
    glm::mat4 viewProjection = glm::lookAt(
        glm::vec3(camera->getX(), camera->getY(), camera->getZ()),
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(camera->getX(), camera->getY() + 1, camera->getZ())
    );
    glm::mat4 orthogonalViewTransform = orthogonalProjection * viewProjection;
    shader.setUniform("viewProjection", sf::Glsl::Mat4(glm::value_ptr(orthogonalViewTransform)));

    // draw the player
    glm::mat4 modelTransform = glm::rotate(glm::mat4(1.0), glm::radians(playerRotation->getTheta()), glm::vec3(0, 1, 0));
    shader.setUniform("rotation", sf::Glsl::Mat4(glm::value_ptr(modelTransform)));
    drawModel(modelRepo->character, 0, 0, 0, modelTransform, orthogonalViewTransform);

    // move the world
    orthogonalViewTransform = glm::translate(orthogonalViewTransform, glm::vec3(-playerPosition->x, -playerPosition->y, -playerPosition->z));
    shader.setUniform("viewProjection", sf::Glsl::Mat4(glm::value_ptr(orthogonalViewTransform)));
    shader.setUniform("rotation", sf::Glsl::Mat4(glm::value_ptr(glm::mat4(1.0))));

    // draw the world
    float z = 0;
    for (std::vector<Map::Tile> row : map->getTiles())
    {
        float x = 0;
        for (Map::Tile tile : row)
        {
            if (tile != Map::Tile::air)
                drawModel(modelRepo->ground, x, 0, z, glm::mat4(1.0), orthogonalViewTransform);

            if (tile == Map::Tile::bush)
                drawModel(modelRepo->bush, x, 0, z, glm::mat4(1.0), orthogonalViewTransform);

            if (tile == Map::Tile::flag)
                drawModel(modelRepo->flag, x, 0, z, glm::mat4(1.0), orthogonalViewTransform);

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
