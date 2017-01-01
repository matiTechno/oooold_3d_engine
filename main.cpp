#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include "camera.h"
#include "transform.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "lights.h"
#include "model.h"
#include "sky_box.h"

// depth Texture(0), diffuse Texture(1), specular Texture(2)

void setLVP(Shader& shader, Transform& transform)
{
    shader.bind();
    glm::mat4 lightProjection = glm::ortho(-30.0, 30.0, -30.0, 30.0, 1.0, 100.0);
    glm::mat4 lightView = glm::lookAt(transform.getPos_ref(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 LVP = lightProjection * lightView;
    glUniformMatrix4fv(glGetUniformLocation(shader.getProgram_ref(), "LVP"), 1, GL_FALSE, &LVP[0][0]);
}

int main()
{
    const float Pi = 3.14159;
    sf::Window window(sf::VideoMode(1000, 800), "opengl", sf::Style::Close, sf::ContextSettings(24, 8, 8, 3, 1));
    window.setVerticalSyncEnabled(true);
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err)
        std::cout << "could not initialize glew\n";
    glEnable(GL_MULTISAMPLE);
    //#########################################################

    // mesh data
    std::vector<Vertex> vQube = {
        Vertex(glm::vec3(1, -1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0)), // right
        Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0)),
        Vertex(glm::vec3(1, 1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0)),
        Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec2(0 , 0), glm::vec3(-1, 0, 0)), // left
        Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0)),
        Vertex(glm::vec3(-1, 1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0)),
        Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0)),
        Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 1, 0)), // up
        Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0)),
        Vertex(glm::vec3(1, 1, -1), glm::vec2(1, 1), glm::vec3(0, 1, 0)),
        Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec2(0, 0), glm::vec3(0, -1, 0)), // down
        Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0)),
        Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, -1, 0)),
        Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)), // back
        Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)),
        Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)),
        Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1)),
        Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1)), //front
        Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1)),
        Vertex(glm::vec3(1, 1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1)),
        Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1))};
    std::vector<Vertex> vSurf = {
        Vertex(glm::vec3(-25, 0, 25), glm::vec2(0, 0), glm::vec3(0, 1, 0)), // up
        Vertex(glm::vec3(-25, 0, -25), glm::vec2(0, 6), glm::vec3(0, 1, 0)),
        Vertex(glm::vec3(25, 0, -25), glm::vec2(6, 6), glm::vec3(0, 1, 0)),
        Vertex(glm::vec3(25, 0, 25), glm::vec2(6, 0), glm::vec3(0, 1, 0))};
    std::vector<GLuint> iQube = {
        0, 1, 2, // 1
        2, 3, 0,
        4, 5, 6, // 2
        6, 7, 4,
        8, 9, 10, // 3
        10, 11, 8,
        12, 13, 14, // 4
        14, 15, 12,
        16, 17, 18, // 5
        18, 19, 16,
        20, 21, 22, // 6
        22, 23, 20};
    std::vector<GLuint> iSurf = {
        0, 1, 2,
        2, 3, 0};
    //    std::vector<GLuint> iScreen = {
    //        16, 17, 18,
    //        18, 19, 16};
    // meshes
    Mesh qube(vQube, iQube); // Q U B E
    Mesh surf(vSurf, iSurf); // S U R F A C E
    //Mesh screen(vQube, iScreen); // S C R E E N for postprocessing

    //models
    Model monkey("res/models/monkey.obj");
    Model nanoSuit("res/models/nanosuit/nanosuit.obj");
    Model dragon("res/models/dragon.obj");

    // transforms
    Transform tSunLight0(glm::vec3(0, 28, 0));
    Transform tPointLight0(glm::vec3(20, 12, 4), glm::vec3(), glm::vec3(0.2, 0.2, 0.2));
    Transform tPointLight1(glm::vec3(0, 6, 0), glm::vec3(), glm::vec3(0.2, 0.2, 0.2));
    Transform tPointLight2(glm::vec3(5, 3, 2), glm::vec3(), glm::vec3(0.2, 0.2, 0.2));
    Transform tSpotLight0(glm::vec3(-10, 13, 0), glm::vec3(), glm::vec3(0.2, 0.2, 0.2));

    Transform tQube0(glm::vec3(0, 10, -5), glm::vec3(Pi/6, Pi/6, 0), glm::vec3(2, 5, 2));
    Transform tMonkey(glm::vec3(0, 8, -3));
    Transform tDragon(glm::vec3(-15, 3, 2));

    Transform tQube1(glm::vec3(2, 5, 3), glm::vec3(Pi/4, Pi/4, Pi/4));
    Transform tSurf(glm::vec3(0, 0, 0));
    Transform tNanoSuit(glm::vec3(20, 0, 0));
    // textures
    Texture qube_diffuse("res/textures/qube_diffuse.png");
    Texture qube_specular("res/textures/qube_specular.png");
    Texture surf_diffuse("res/textures/surf_diffuse.png");
    Texture surf_specular("res/textures/surf_specular.png");

    // skyBox
    std::vector<std::string> skyBoxTextures ={
        std::string("res/textures/SkyboxSet1/DarkStormy/DarkStormyLeft2048.png"),
        std::string("res/textures/SkyboxSet1/DarkStormy/DarkStormyRight2048.png"),
        std::string("res/textures/SkyboxSet1/DarkStormy/DarkStormyUp2048.png"),
        std::string("res/textures/SkyboxSet1/DarkStormy/DarkStormyDown2048.png"),
        std::string("res/textures/SkyboxSet1/DarkStormy/DarkStormyFront2048.png"),
        std::string("res/textures/SkyboxSet1/DarkStormy/DarkStormyBack2048.png")};
    SkyBox skyBox(skyBoxTextures);

    // lights
    std::vector<SunLight> sunLights = {
        SunLight(&tSunLight0, glm::vec3(0.025, 0.025, 0.025), glm::vec3(0.03, 0.03, 0.03), glm::vec3(0.2, 0.2, 0.2))}; // zalozenie: swiatlo skierowane
    // jest zawsze do punktu (0, 0, 0)

    std::vector<PointLight> pointLights = {
        PointLight(&tPointLight0, glm::vec3(0.05, 0.05, 0.05), glm::vec3(0.4, 0.4, 0.4), glm::vec3(0.5, 0.5, 0.5), 0.1, 0.1, 1),
        PointLight(&tPointLight1, glm::vec3(0.05, 0.05, 0.05), glm::vec3(0.1, 0.05, 0.4), glm::vec3(0.1, 0.1, 0.5), 0.2, 0.2, 1),
        PointLight(&tPointLight2, glm::vec3(0.05, 0.05, 0.05), glm::vec3(0.4, 0.2, 0), glm::vec3(0.4, 0.2, 0), 0.2, 0.2, 1)};

    std::vector<SpotLight> spotLights = {
        SpotLight(glm::vec3(0, -1, 0), &tSpotLight0, glm::vec3(0.08, 0, 0.1), glm::vec3(0.24, 0, 0.3), glm::vec3(0.3, 0, 0.5), 0.1, 0.1, 1,
        Pi/6, Pi/5)};

    // shaders
    Shader shader_test("res/shaders/shader_test");
    Shader shader_mat("res/shaders/shader_mat");
    Shader shader_mat_tex("res/shaders/shader_mat_tex");
    Shader shader_depthMap("res/shaders/shader_depthMap");
    Shader shader_skyBox("res/shaders/shader_skyBox");
    //Shader shader_postProcessDepthMap("res/shaders/shader_postProcessDepthMap");

    //********************************************* depth map -> directional light
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    const GLuint depthMap_WIDTH = 2000, depthMap_HEIGHT = 2000;
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthMap_WIDTH, depthMap_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = {1, 1, 1, 1};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //********************************************* depth map -> point light
    //    GLuint depthCubeMap;
    //    glGenTextures(1, &depthCubeMap);
    //    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

    //    for(GLuint i = 0; i < 6; i++)
    //        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, depthMap_WIDTH, depthMap_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //    GLuint depthCubeMapFBO;
    //    glGenFramebuffers(1, &depthCubeMapFBO);
    //    glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFBO);
    //    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
    //    glDrawBuffer(GL_NONE);
    //    glReadBuffer(GL_NONE);

    //    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //        std::cout << "Framebuffer not complete!" << std::endl;

    //    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //#########################################################
    Camera camera(glm::vec3(0, 10, 10), Pi/2, static_cast<float>(window.getSize().x)/window.getSize().y, 0.1, 100);
    sf::Time time;
    sf::Clock clock;
    bool running = true;
    while(running)
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                running = false;
            else if(event.type == sf::Event::KeyPressed)
            {
                switch(event.key.code)
                {
                case sf::Keyboard::Escape:
                    running = false;
                    break;
                default:
                    break;
                }
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            camera.moveForward(0.3);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            camera.moveForward(-0.3);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            camera.moveRight(-0.3);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            camera.moveRight(0.3);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))
            camera.rotateX(0.02);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
            camera.rotateX(-0.02);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
            camera.rotateY(0.02);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
            camera.rotateY(-0.02);
        time += clock.restart();
        //#########################################################

        tSunLight0.getPos_ref().z = 60*cos(time.asSeconds()/20);
        tSunLight0.getPos_ref().x = 60*sin(time.asSeconds()/20);

        tPointLight2.getPos_ref().z = 3*cos(time.asSeconds()/6) + 2;
        tPointLight2.getPos_ref().x = 3*sin(time.asSeconds()/6) + 5;

        // -------------------------------- ^^^^ transformation updates (up)

        glViewport(0, 0, depthMap_WIDTH, depthMap_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glDisable(GL_FRAMEBUFFER_SRGB);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        setLVP(shader_depthMap, tSunLight0);

        shader_depthMap.update(0, &tPointLight0); // pointLight0
        qube.draw();

        shader_depthMap.update(0, &tPointLight1); // pointLight1
        qube.draw();

        shader_depthMap.update(0, &tPointLight2); // pointLight2
        qube.draw();

        shader_depthMap.update(0, &tSpotLight0); // spotLight0
        qube.draw();

        shader_depthMap.update(0, &tQube0); // qube0
        qube.draw();

        shader_depthMap.update(0, &tMonkey); // monkey
        monkey.draw();

        shader_depthMap.update(0, &tDragon); // dragon
        dragon.draw();

        shader_depthMap.update(0, &tQube1); // qube 1
        qube.draw();

        shader_depthMap.update(0, &tSurf); // surface
        surf.draw();

        shader_depthMap.update(0, &tNanoSuit); // nanoSuit
        nanoSuit.draw();

        // -------------------------------- ^^^^ rendering depth map (up)

        //        glViewport(0, 0, window.getSize().x, window.getSize().y); // postProcessing texture image
        //        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //        glEnable(GL_FRAMEBUFFER_SRGB);
        //        glClear(GL_COLOR_BUFFER_BIT);
        //        glDisable(GL_DEPTH_TEST);
        //        glActiveTexture(GL_TEXTURE0);
        //        glBindTexture(GL_TEXTURE_2D, depthMap);
        //        shader_postProcessDepthMap.bind();
        //        screen.draw();


        // ************************************************************ final image

        glViewport(0, 0, window.getSize().x, window.getSize().y);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        // shader updates

        setLVP(shader_mat, tSunLight0);
        setLVP(shader_mat_tex, tSunLight0);

        shader_mat.setLightScene(&sunLights, &pointLights, &spotLights);
        shader_mat_tex.setLightScene(&sunLights, &pointLights, &spotLights);

        // '''''''''''''''''''''''''''''''''''''''''''''''''''''''''' lights
        shader_test.update(&camera, &tSunLight0); // sunLight0
        shader_test.setSimpleColor(glm::vec3(1, 1, 1));
        qube.draw();

        shader_test.update(&camera, &tPointLight0); // pointLight0
        shader_test.setSimpleColor(glm::vec3(1, 1, 1));
        qube.draw();
        shader_test.update(&camera, &tPointLight1); // pointLight1
        shader_test.setSimpleColor(glm::vec3(0.1, 0.05, 1));
        qube.draw();
        shader_test.update(&camera, &tPointLight2); // pointLight2
        shader_test.setSimpleColor(glm::vec3(1, 0.7, 0));
        qube.draw();

        shader_test.update(&camera, &tSpotLight0); // spotLight0
        shader_test.setSimpleColor(glm::vec3(0.8, 0, 1));
        qube.draw();

        // '''''''''''''''''''''''''''''''''''''''''''''''''''''''''' objects without texture
        shader_mat.update(&camera, &tQube0); // qube0
        shader_mat.setMat(11.264, glm::vec3(0.25, 0.20725, 0.20725), glm::vec3(1.0, 0.829, 0.829), glm::vec3(0.296648, 0.296648, 0.296648));
        qube.draw();

        shader_mat.update(&camera, &tMonkey); // monkey
        shader_mat.setMat(12.8, glm::vec3(0.19125, 0.0735, 0.0225), glm::vec3(0.7038, 0.27048, 0.0828), glm::vec3(0.256777, 0.137622, 0.086014));
        monkey.draw();

        shader_mat.update(&camera, &tDragon); // dragon
        shader_mat.setMat(83.2, glm::vec3(0.24725, 0.2245, 0.0645), glm::vec3(0.34615, 0.3143, 0.0903), glm::vec3(0.797357, 0.723991, 0.208006));
        dragon.draw();

        // '''''''''''''''''''''''''''''''''''''''''''''''''''''''''' objects with texture
        shader_mat_tex.update(&camera, &tQube1); // qube1
        shader_mat_tex.setMat(20);
        qube_diffuse.bind(1);
        qube_specular.bind(2);
        qube.draw();

        shader_mat_tex.update(&camera, &tSurf); // surface
        shader_mat_tex.setMat(12.5);
        surf_diffuse.bind(1);
        surf_specular.bind(2);
        surf.draw();

        shader_mat_tex.update(&camera, &tNanoSuit); // nanoSuit
        nanoSuit.draw(&shader_mat_tex, 100);

        // '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''' S.K.Y...B.O.X.
        glDepthFunc(GL_LEQUAL);
        shader_skyBox.update(&camera);
        skyBox.bind();
        qube.draw();

        //#########################################################
        window.display();
    }

    // cleaning
    glDeleteFramebuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthMap);
    //glDeleteFramebuffers(1, &depthCubeMapFBO);
    //glDeleteTextures(1, &depthCubeMap);
    window.close();
    return 0;
}
