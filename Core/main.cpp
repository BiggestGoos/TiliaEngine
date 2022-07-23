#include "vendor/glad/include/glad/glad.h"
#include "vendor/glfw/include/GLFW/glfw3.h"

#include "vendor/glm/include/glm/glm.hpp"
#include "vendor/glm/include/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/include/glm/gtc/type_ptr.hpp"
#include "vendor/glm/include/glm/gtx/string_cast.hpp"

#include "vendor/stb_image/include/stb_image/stb_image.h"

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "Core/Values/OpenGL/3.3/Utils.h"
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Shader.h"
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Texture_2D.h"
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Cube_Map.h"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.h"
#include "Core/Modules/Console/Logging.h"
#include "Core/Modules/Rendering/OpenGL/3.3/Batch.h"
#include "Core/Modules/Rendering/OpenGL/3.3/Renderer.h"
#include "Core/Modules/Rendering/OpenGL/3.3/Mesh.h"
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Shader_Data.h"
#include "Core/Modules/Exceptions/Tilia_Exception.h"
#include "Core/Values/OpenGL/3.3/Enums.h"
#include "Core/Temp/Camera.h"
#include "Core/Temp/Input.h"
#include "Core/Temp/Limit_Fps.h"
#include "Core/Temp/Stopwatch.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput();

// settings
unsigned int SCR_WIDTH = 1600;
unsigned int SCR_HEIGHT = 1200;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

using namespace tilia;
using namespace tilia::utils;
using namespace tilia::render;
using namespace tilia::enums;

Polymode polymode{ Polymode::Fill };

bool pause{ true };

float light_z{ 0.0f };

float angle{};
float add_angle{ 60.0f };

glm::bvec3 axis{};

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

uint32_t passed_frames{};
uint32_t fps{};

Input_Manager input;

template<size_t size>
void create_square(Mesh<size>& mesh, glm::mat4 model, std::array<glm::vec2, 4> uv, uint32_t tex_slot);

template<size_t size>
void create_square(Mesh<size>& mesh, glm::mat4 model);

template<size_t size>
void create_circle(Mesh<size>& mesh, glm::mat4 model, size_t segment_count);

template<size_t size>
void create_cube(Mesh<size>& mesh, glm::mat4 model, bool complex = false);

template<size_t size>
void create_sphere(Mesh<size>& mesh, glm::mat4 model, uint32_t tex_index = 0);

int main()
{

    try
    {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // glfw window creation
        // --------------------
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // tell GLFW to capture our mouse
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSwapInterval(0);

        //glad: load all OpenGL function pointers
        //---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        input.Init(window);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // configure global opengl state
        // -----------------------------
        glLineWidth(2);
        glPointSize(10);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);

        camera.Reset();

        Renderer renderer{};

        std::shared_ptr<Shader> light_shader{ std::make_shared<Shader>() };
        light_shader->Init("res/shaders/light_shader.vert", "res/shaders/light_shader.frag");

        std::shared_ptr<Shader> cube_shader{ std::make_shared<Shader>() };
        cube_shader->Init("res/shaders/cube_shader.vert", "res/shaders/cube_shader.frag");

        Cube_Map_Data def{};

        def.sides[0].file_path = "res/textures/container2.png";
        def.sides[1].file_path = "res/textures/container2.png";
        def.sides[2].file_path = "res/textures/container2.png";
        def.sides[3].file_path = "res/textures/container2.png";
        def.sides[4].file_path = "res/textures/container2.png";
        def.sides[5].file_path = "res/textures/container2.png";

        std::shared_ptr<Cube_Map> box_texture{ std::make_shared<Cube_Map>() };
        box_texture->Set(def);

        std::shared_ptr<Cube_Map> box_specular_texture{ std::make_shared<Cube_Map>() };
        box_specular_texture->Set({
            "res/textures/container2_specular.png",
            "res/textures/container2_specular.png",
            "res/textures/container2_specular.png",
            "res/textures/container2_specular.png",
            "res/textures/container2_specular.png",
            "res/textures/container2_specular.png"
        });

        std::shared_ptr<Texture_2D> tex_2d{ std::make_shared<Texture_2D>() };
        tex_2d->Set_Texture("res/teures/container2.png");

        light_shader->Uniform("material.diffuse", 0);
        light_shader->Uniform("material.specular", 1);
        
        constexpr size_t cube_count{ 10 };
        constexpr size_t light_count{ 1 };
        
        // positions all containers
        glm::vec3 cubePositions[] = {
            glm::vec3(0.0f,  0.0f,  0.0f),
            glm::vec3(2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f,  2.0f, -2.5f),
            glm::vec3(1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };
        // positions of the point lights
        glm::vec3 pointLightPositions[] = {
            glm::vec3(1.2f, 1.0f, 2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3(0.0f,  0.0f, -3.0f)
        };

        std::vector<std::shared_ptr<Mesh<9>>> meshes{};
        
        glm::mat4 model{ 1.0f };

        for (size_t i = 0; i < cube_count; i++)
        {

            std::shared_ptr<Mesh<9>> new_mesh{ std::make_shared<Mesh<9>>() };

            new_mesh->Set_Var<Mesh_Var::Shader>()(light_shader);

            new_mesh->Add_Texture(box_texture);
            //new_mesh->Add_Texture(box_specular_texture);

            new_mesh->Set_Var<Mesh_Var::Cull_Face>()(Face::Back);

            new_mesh->Set_Var<Mesh_Var::Primitive>()(Primitive::Triangles);

            model = glm::translate(glm::mat4{ 1.0f }, cubePositions[i]);

            create_cube(*new_mesh, model, true);

            renderer.Add_Mesh(new_mesh->Get_Mesh_Data());

            meshes.push_back(new_mesh);

        }

        for (size_t i = 0; i < light_count; i++)
        {

            std::shared_ptr<Mesh<9>> new_mesh{ std::make_shared<Mesh<9>>() };

            new_mesh->Set_Var<Mesh_Var::Shader>()(cube_shader);

            new_mesh->Set_Var<Mesh_Var::Cull_Face>()(Face::Back);

            new_mesh->Set_Var<Mesh_Var::Primitive>()(Primitive::Triangles);

            model = glm::translate(glm::mat4{ 1.0f }, pointLightPositions[i]);
            model = glm::scale(model, { 0.2f, 0.2f, 0.2f });

            create_cube(*new_mesh, model);

            renderer.Add_Mesh(new_mesh->Get_Mesh_Data());

            meshes.push_back(new_mesh);

        }

        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {

            S2D::utils::Sleep_Program(60);

            if (!pause)
                light_z += 1.0f * deltaTime;

            pointLightPositions[0].z = -sinf(static_cast<float>(light_z)) * 5.0f;

            for (size_t i = 0; i < cube_count + light_count; i++)
            {

                meshes[i]->Set_Var<Mesh_Var::Polymode>()(polymode);

                if (i < cube_count) {

                    model = glm::translate(glm::mat4{ 1.0f }, cubePositions[i]);

                    if (axis != glm::zero<glm::bvec3>())
                        model = glm::rotate(model, glm::radians(angle), static_cast<glm::vec3>(axis));

                    model = glm::scale(model, glm::vec3{ 2.0f, 1.0f, 1.0f });

                    create_cube(*meshes[i], model, true);

                }
                else if (i >= cube_count)
                {

                    model = glm::translate(glm::mat4{ 1.0f }, pointLightPositions[i - cube_count]);

                    model = glm::scale(model, glm::vec3{ 0.2f });

                    create_cube(*meshes[i], model);

                }

            }
            
            if (!pause)
                angle += add_angle * deltaTime;

            fps = static_cast<uint32_t>(static_cast<double>(passed_frames) / glfwGetTime());

            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            std::cout << fps << " : " << deltaTime << " : " << add_angle << " : "<< angle << " : " << axis.x << " , " << axis.y << " , " << axis.z << '\n';

            processInput();
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            // pass projection matrix to shader (note that in this case it could change every frame)
            glm::mat4 projection{ glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 100.0f) };
            light_shader->Uniform("projection", projection);
            cube_shader->Uniform("projection", projection);

            // camera/view transformation
            glm::mat4 view{ camera.GetViewMatrix() };
            light_shader->Uniform("view", view);
            cube_shader->Uniform("view", view);

            renderer.m_camera_pos = camera.Position;

            light_shader->Uniform("lightColor", 1.0f, 1.0f, 1.0f);
            light_shader->Uniform("lightPos", pointLightPositions[0]);

            try
            {
                renderer.Render();
            }
            catch (const std::exception& e)
            {
                std::cout << e.what() << '\n';
            }

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
            input.Update();

            ++passed_frames;

        }

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
    }
    catch (const utils::Tilia_Exception& e) {
        std::cout << "\n<<<Tilia_Exception>>>\n";
        std::cout << "Line: " << e.Get_Origin_Line() << '\n' <<
                     "File: " << e.Get_Origin_File() << '\n';
        std::cout << e.what() << '\n';
    }
    catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }

    glfwTerminate();

    while (true)
    {

    }

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput()
{

    if (input.Get_Key_Pressed(KEY_ENTER))
    {
        if (polymode == Polymode::Fill)
            polymode = Polymode::Line;
        else if (polymode == Polymode::Line)
            polymode = Polymode::Fill;
    }

    if (input.Get_Key_Pressed(KEY_BACKSPACE))
        pause = !pause;

    if (input.Get_Key_Down(KEY_UP))
        add_angle += 10.0f * deltaTime;
    if (input.Get_Key_Down(KEY_DOWN))
        add_angle -= 10.0f * deltaTime;

    if (input.Get_Key_Pressed(KEY_1))
        axis.x = !axis.x;
    if (input.Get_Key_Pressed(KEY_2))
        axis.y = !axis.y;
    if (input.Get_Key_Pressed(KEY_3))
        axis.z = !axis.z;

    if (input.Get_Key_Down(KEY_W))
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (input.Get_Key_Down(KEY_S))
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (input.Get_Key_Down(KEY_A))
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (input.Get_Key_Down(KEY_D))
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    if (input.Get_Key_Down(KEY_SPACE))
        camera.ProcessKeyboard(Camera_Movement::UP, deltaTime);
    if (input.Get_Key_Down(KEY_LEFT_CONTROL))
        camera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);

    if (input.Get_Key_Pressed(KEY_TAB))
        camera.Reset(true);
    if (input.Get_Key_Pressed(KEY_0))
        camera.Position = { 0.0f, 0.0f, 0.0f };

    camera.ProcessMouseScroll(static_cast<float>(input.Get_Mouse_Scroll()));

    float xpos = static_cast<float>(input.Get_Mouse_Pos().x);
    float ypos = static_cast<float>(input.Get_Mouse_Pos().y);

    if (firstMouse && (input.Get_Mouse_Pos().x + input.Get_Mouse_Pos().y))
    {
        camera.Reset();
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    else if (firstMouse)
    {
        camera.Reset();
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    
    if (input.Get_Mouse_Button_Down(0))
        camera.ProcessMouseMovement(xoffset, yoffset);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

template<size_t size>
void create_square(Mesh<size>& mesh, glm::mat4 model, std::array<glm::vec2, 4> uv, uint32_t tex_slot) {

    mesh.vertices.clear();
    mesh.indices.clear();

    glm::vec3 pos_1{ -0.5f, -0.5f, 0.0f };
    glm::vec3 pos_2{ 0.5f, -0.5f, 0.0f };
    glm::vec3 pos_3{ 0.5f,  0.5f, 0.0f };
    glm::vec3 pos_4{ -0.5f,  0.5f, 0.0f };

    pos_1 = glm::vec3{ model * glm::vec4{ pos_1, 1.0f } };
    pos_2 = glm::vec3{ model * glm::vec4{ pos_2, 1.0f } };
    pos_3 = glm::vec3{ model * glm::vec4{ pos_3, 1.0f } };
    pos_4 = glm::vec3{ model * glm::vec4{ pos_4, 1.0f } };

    mesh.vertices.push_back(Vertex<size>{ pos_1.x, pos_1.y, pos_1.z, uv[0].x, uv[0].y, static_cast<float>(tex_slot) });
    mesh.vertices.push_back(Vertex<size>{ pos_2.x, pos_2.y, pos_2.z, uv[1].x, uv[1].y, static_cast<float>(tex_slot) });
    mesh.vertices.push_back(Vertex<size>{ pos_3.x, pos_3.y, pos_3.z, uv[2].x, uv[2].y, static_cast<float>(tex_slot) });
    mesh.vertices.push_back(Vertex<size>{ pos_4.x, pos_4.y, pos_4.z, uv[3].x, uv[3].y, static_cast<float>(tex_slot) });

    mesh.indices.push_back(0);
    mesh.indices.push_back(1);
    mesh.indices.push_back(2);
    mesh.indices.push_back(2);
    mesh.indices.push_back(3);
    mesh.indices.push_back(0);

    Vertex_Info v_info{};

    v_info.sizes = { 3, 2, 1 };
    v_info.strides = { static_cast<int32_t>(size) };
    v_info.offsets = { 0, 3, 5 };

    mesh.Set_Vertex_Info(v_info);

}

template<size_t size>
void create_square(Mesh<size>& mesh, glm::mat4 model)
{

    mesh.vertices.clear();
    mesh.indices.clear();

    glm::vec3 pos_1{ -0.5f, -0.5f, 0.0f };
    glm::vec3 pos_2{ 0.5f, -0.5f, 0.0f };
    glm::vec3 pos_3{ 0.5f,  0.5f, 0.0f };
    glm::vec3 pos_4{ -0.5f,  0.5f, 0.0f };

    pos_1 = glm::vec3{ model * glm::vec4{ pos_1, 1.0f } };
    pos_2 = glm::vec3{ model * glm::vec4{ pos_2, 1.0f } };
    pos_3 = glm::vec3{ model * glm::vec4{ pos_3, 1.0f } };
    pos_4 = glm::vec3{ model * glm::vec4{ pos_4, 1.0f } };

    mesh.vertices.push_back(Vertex<size>{ pos_1.x, pos_1.y, pos_1.z, 0.0f, 0.0f });
    mesh.vertices.push_back(Vertex<size>{ pos_2.x, pos_2.y, pos_2.z, 1.0f, 0.0f });
    mesh.vertices.push_back(Vertex<size>{ pos_3.x, pos_3.y, pos_3.z, 1.0f, 1.0f });
    mesh.vertices.push_back(Vertex<size>{ pos_4.x, pos_4.y, pos_4.z, 0.0f, 1.0f });

    mesh.indices.push_back(0);
    mesh.indices.push_back(1);
    mesh.indices.push_back(2);
    mesh.indices.push_back(2);
    mesh.indices.push_back(3);
    mesh.indices.push_back(0);

    Vertex_Info v_info{};

    v_info.sizes = { 3, 2 };
    v_info.strides = { static_cast<int32_t>(size) };
    v_info.offsets = { 0, 3 };

    mesh.Set_Var<Mesh_Var::Vertex_Info>()(v_info);

}

template<size_t size>
void create_circle(Mesh<size>& mesh, glm::mat4 model, size_t segment_count) {

    mesh.vertices.clear();
    mesh.indices.clear();

    glm::vec3 mid_point{ glm::vec3{ model * glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f } } };

    mesh.vertices.push_back(Vertex<size>{ mid_point.x, mid_point.y, mid_point.z });

    for (float i = 360.0f; i >= 0.0f; i -= 360.0f / segment_count)
    {

        glm::vec2 vec{ model * glm::vec4{ sinf(glm::radians(i)), cosf(glm::radians(i)), 1.0f, 1.0f } };

        mesh.vertices.push_back(Vertex<size>{ vec.x, vec.y, 0.0f });

    }

    for (size_t i = 1; i <= segment_count; i++)
    {
        mesh.indices.push_back(0);
        mesh.indices.push_back(static_cast<uint32_t>(i));
        if (i != segment_count)
            mesh.indices.push_back(static_cast<uint32_t>(i + 1));
        else
            mesh.indices.push_back(1);
    }

    Vertex_Info v_info{};

    v_info.sizes = { 3 };
    v_info.strides = { static_cast<int32_t>(size) };
    v_info.offsets = { 0 };

    mesh.Set_Vertex_Info(v_info);

}

template<size_t size>
void create_cube(Mesh<size>& mesh, glm::mat4 model, bool complex)
{

    mesh.vertices.clear();
    mesh.indices.clear();
    
    if (complex)
    {
        std::vector<glm::vec3> positions{};
        std::vector<glm::vec3> normals{};
        std::vector<glm::vec3> tex_coords{};

        constexpr size_t vertex_count{ 24 };

        positions.push_back({ -0.5f, -0.5f,  0.5f });
        positions.push_back({ 0.5f, -0.5f,  0.5f });
        positions.push_back({ 0.5f,  0.5f,  0.5f });
        positions.push_back({ -0.5f,  0.5f,  0.5f });

        positions.push_back({ -0.5f, -0.5f, -0.5f });
        positions.push_back({ 0.5f, -0.5f, -0.5f });
        positions.push_back({ 0.5f,  0.5f, -0.5f });
        positions.push_back({ -0.5f,  0.5f, -0.5f });

        positions.push_back({ 0.5f, -0.5f,  0.5f });
        positions.push_back({ 0.5f, -0.5f,  -0.5f });
        positions.push_back({ 0.5f,  0.5f,  -0.5f });
        positions.push_back({ 0.5f,  0.5f,  0.5f });

        positions.push_back({ -0.5f, -0.5f, -0.5f });
        positions.push_back({ -0.5f, -0.5f, 0.5f });
        positions.push_back({ -0.5f,  0.5f, 0.5f });
        positions.push_back({ -0.5f,  0.5f, -0.5f });

        positions.push_back({ -0.5f, 0.5f,  0.5f });
        positions.push_back({ 0.5f, 0.5f,  0.5f });
        positions.push_back({ 0.5f,  0.5f,  -0.5f });
        positions.push_back({ -0.5f,  0.5f,  -0.5f });

        positions.push_back({ 0.5f, -0.5f, -0.5f });
        positions.push_back({ -0.5f, -0.5f, -0.5f });
        positions.push_back({ -0.5f,  -0.5f, 0.5f });
        positions.push_back({ 0.5f,  -0.5f, 0.5f });

        for (size_t i = 0; i < vertex_count; i++)
        {
            tex_coords.push_back(positions[i]);
            positions[i] = glm::vec3{ model * glm::vec4{ positions[i], 1.0f } };
        }

        normals.push_back({ 0.0f, 0.0f, 1.0f });
        normals.push_back({ 0.0f, 0.0f, 1.0f });
        normals.push_back({ 0.0f, 0.0f, 1.0f });
        normals.push_back({ 0.0f, 0.0f, 1.0f });

        normals.push_back({ 0.0f, 0.0f, -1.0f });
        normals.push_back({ 0.0f, 0.0f, -1.0f });
        normals.push_back({ 0.0f, 0.0f, -1.0f });
        normals.push_back({ 0.0f, 0.0f, -1.0f });

        normals.push_back({ 1.0f, 0.0f, 0.0f });
        normals.push_back({ 1.0f, 0.0f, 0.0f });
        normals.push_back({ 1.0f, 0.0f, 0.0f });
        normals.push_back({ 1.0f, 0.0f, 0.0f });

        normals.push_back({ -1.0f, 0.0f, 0.0f });
        normals.push_back({ -1.0f, 0.0f, 0.0f });
        normals.push_back({ -1.0f, 0.0f, 0.0f });
        normals.push_back({ -1.0f, 0.0f, 0.0f });

        normals.push_back({ 0.0f, 1.0f, 0.0f });
        normals.push_back({ 0.0f, 1.0f, 0.0f });
        normals.push_back({ 0.0f, 1.0f, 0.0f });
        normals.push_back({ 0.0f, 1.0f, 0.0f });

        normals.push_back({ 0.0f, -1.0f, 0.0f });
        normals.push_back({ 0.0f, -1.0f, 0.0f });
        normals.push_back({ 0.0f, -1.0f, 0.0f });
        normals.push_back({ 0.0f, -1.0f, 0.0f });

        for (size_t i = 0; i < vertex_count; i++)
        {
            normals[i] = { normals[i] * glm::mat3{glm::transpose(model)} };
        }

        for (size_t i = 0; i < vertex_count; i++)
        {
            mesh.vertices.push_back(Vertex<size>{
                positions[i].x, positions[i].y, positions[i].z,
                    normals[i].x, normals[i].y, normals[i].z,
                    tex_coords[i].x, tex_coords[i].y, tex_coords[i].z
            });
        }

        mesh.indices.push_back(0);
        mesh.indices.push_back(1);
        mesh.indices.push_back(2);
        mesh.indices.push_back(2);
        mesh.indices.push_back(3);
        mesh.indices.push_back(0);

        mesh.indices.push_back(5);
        mesh.indices.push_back(4);
        mesh.indices.push_back(7);
        mesh.indices.push_back(7);
        mesh.indices.push_back(6);
        mesh.indices.push_back(5);

        mesh.indices.push_back(8);
        mesh.indices.push_back(9);
        mesh.indices.push_back(10);
        mesh.indices.push_back(10);
        mesh.indices.push_back(11);
        mesh.indices.push_back(8);

        mesh.indices.push_back(12);
        mesh.indices.push_back(13);
        mesh.indices.push_back(14);
        mesh.indices.push_back(14);
        mesh.indices.push_back(15);
        mesh.indices.push_back(12);

        mesh.indices.push_back(16);
        mesh.indices.push_back(17);
        mesh.indices.push_back(18);
        mesh.indices.push_back(18);
        mesh.indices.push_back(19);
        mesh.indices.push_back(16);

        mesh.indices.push_back(22);
        mesh.indices.push_back(21);
        mesh.indices.push_back(20);
        mesh.indices.push_back(20);
        mesh.indices.push_back(23);
        mesh.indices.push_back(22);

        Vertex_Info v_info{};

        v_info.sizes = { 3, 3, 3 };
        v_info.strides = { static_cast<int32_t>(size) };
        v_info.offsets = { 0, 3, 6 };

        mesh.Set_Var<Mesh_Var::Vertex_Info>()(v_info);

    }
    else
    {
        
        std::vector<glm::vec3> positions{};

        constexpr size_t vertex_count{ 8 };

        positions.push_back({ -0.5f, -0.5f,  0.5f });
        positions.push_back({ 0.5f, -0.5f,  0.5f });
        positions.push_back({ 0.5f,  0.5f,  0.5f });
        positions.push_back({ -0.5f,  0.5f,  0.5f });

        positions.push_back({ -0.5f, -0.5f, -0.5f });
        positions.push_back({ 0.5f, -0.5f, -0.5f });
        positions.push_back({ 0.5f,  0.5f, -0.5f });
        positions.push_back({ -0.5f,  0.5f, -0.5f });

        for (size_t i = 0; i < vertex_count; i++)
        {
            positions[i] = glm::vec3{ model * glm::vec4{ positions[i], 1.0f } };
            mesh.vertices.push_back(Vertex<size>{ positions[i].x, positions[i].y, positions[i].z });
        }

        mesh.indices.push_back(0);
        mesh.indices.push_back(1);
        mesh.indices.push_back(2);
        mesh.indices.push_back(2);
        mesh.indices.push_back(3);
        mesh.indices.push_back(0);

        mesh.indices.push_back(5);
        mesh.indices.push_back(4);
        mesh.indices.push_back(7);
        mesh.indices.push_back(7);
        mesh.indices.push_back(6);
        mesh.indices.push_back(5);

        mesh.indices.push_back(1);
        mesh.indices.push_back(5);
        mesh.indices.push_back(6);
        mesh.indices.push_back(6);
        mesh.indices.push_back(2);
        mesh.indices.push_back(1);

        mesh.indices.push_back(4);
        mesh.indices.push_back(0);
        mesh.indices.push_back(3);
        mesh.indices.push_back(3);
        mesh.indices.push_back(7);
        mesh.indices.push_back(4);

        mesh.indices.push_back(3);
        mesh.indices.push_back(2);
        mesh.indices.push_back(6);
        mesh.indices.push_back(6);
        mesh.indices.push_back(7);
        mesh.indices.push_back(3);

        mesh.indices.push_back(5);
        mesh.indices.push_back(1);
        mesh.indices.push_back(0);
        mesh.indices.push_back(0);
        mesh.indices.push_back(4);
        mesh.indices.push_back(5);

        Vertex_Info v_info{};

        v_info.sizes = { 3 };
        v_info.strides = { static_cast<int32_t>(size) };
        v_info.offsets = { 0 };

        mesh.Set_Var<Mesh_Var::Vertex_Info>()(v_info);

    }

}

//template<size_t size>
//void create_cube(Mesh<size>& mesh, glm::mat4 model, uint32_t tex_slot) {
//
//    mesh.vertices.clear();
//    mesh.indices.clear();
//
//    glm::vec3 pos_1{ -0.5f, -0.5f,  0.5f };
//    glm::vec3 pos_2{  0.5f, -0.5f,  0.5f };
//    glm::vec3 pos_3{  0.5f,  0.5f,  0.5f };
//    glm::vec3 pos_4{ -0.5f,  0.5f,  0.5f };
//
//    glm::vec3 pos_5{ -0.5f, -0.5f, -0.5f };
//    glm::vec3 pos_6{  0.5f, -0.5f, -0.5f };
//    glm::vec3 pos_7{  0.5f,  0.5f, -0.5f };
//    glm::vec3 pos_8{ -0.5f,  0.5f, -0.5f };
//
//    pos_1 = glm::vec3{ model * glm::vec4{ pos_1, 1.0f } };
//    pos_2 = glm::vec3{ model * glm::vec4{ pos_2, 1.0f } };
//    pos_3 = glm::vec3{ model * glm::vec4{ pos_3, 1.0f } };
//    pos_4 = glm::vec3{ model * glm::vec4{ pos_4, 1.0f } };
//    pos_5 = glm::vec3{ model * glm::vec4{ pos_5, 1.0f } };
//    pos_6 = glm::vec3{ model * glm::vec4{ pos_6, 1.0f } };
//    pos_7 = glm::vec3{ model * glm::vec4{ pos_7, 1.0f } };
//    pos_8 = glm::vec3{ model * glm::vec4{ pos_8, 1.0f } };
//
//    mesh.vertices.push_back(Vertex<size>{ pos_1.x, pos_1.y, pos_1.z, -1.0f, -1.0f, 1.0f, static_cast<float>(tex_slot)});
//    mesh.vertices.push_back(Vertex<size>{ pos_2.x, pos_2.y, pos_2.z,  1.0f, -1.0f, 1.0f, static_cast<float>(tex_slot)});
//    mesh.vertices.push_back(Vertex<size>{ pos_3.x, pos_3.y, pos_3.z,  1.0f,  1.0f, 1.0f, static_cast<float>(tex_slot)});
//    mesh.vertices.push_back(Vertex<size>{ pos_4.x, pos_4.y, pos_4.z, -1.0f,  1.0f, 1.0f, static_cast<float>(tex_slot)});
//
//    mesh.vertices.push_back(Vertex<size>{ pos_5.x, pos_5.y, pos_5.z, -1.0f, -1.0f, -1.0f, static_cast<float>(tex_slot)});
//    mesh.vertices.push_back(Vertex<size>{ pos_6.x, pos_6.y, pos_6.z,  1.0f, -1.0f, -1.0f, static_cast<float>(tex_slot)});
//    mesh.vertices.push_back(Vertex<size>{ pos_7.x, pos_7.y, pos_7.z,  1.0f,  1.0f, -1.0f, static_cast<float>(tex_slot)});
//    mesh.vertices.push_back(Vertex<size>{ pos_8.x, pos_8.y, pos_8.z, -1.0f,  1.0f, -1.0f, static_cast<float>(tex_slot)});
//
//    mesh.indices.push_back(0);
//    mesh.indices.push_back(1);
//    mesh.indices.push_back(2);
//    mesh.indices.push_back(2);
//    mesh.indices.push_back(3);
//    mesh.indices.push_back(0);
//
//    mesh.indices.push_back(5);
//    mesh.indices.push_back(4);
//    mesh.indices.push_back(7);
//    mesh.indices.push_back(7);
//    mesh.indices.push_back(6);
//    mesh.indices.push_back(5);
//
//    mesh.indices.push_back(1);
//    mesh.indices.push_back(5);
//    mesh.indices.push_back(6);
//    mesh.indices.push_back(6);
//    mesh.indices.push_back(2);
//    mesh.indices.push_back(1);
//
//    mesh.indices.push_back(4);
//    mesh.indices.push_back(0);
//    mesh.indices.push_back(3);
//    mesh.indices.push_back(3);
//    mesh.indices.push_back(7);
//    mesh.indices.push_back(4);
//
//    mesh.indices.push_back(3);
//    mesh.indices.push_back(2);
//    mesh.indices.push_back(6);
//    mesh.indices.push_back(6);
//    mesh.indices.push_back(7);
//    mesh.indices.push_back(3);
//
//    mesh.indices.push_back(5);
//    mesh.indices.push_back(1);
//    mesh.indices.push_back(0);
//    mesh.indices.push_back(0);
//    mesh.indices.push_back(4);
//    mesh.indices.push_back(5);
//
//    Vertex_Info v_info{};
//
//    v_info.sizes = { 3, 3, 1 };
//    v_info.strides = { static_cast<int32_t>(size) };
//    v_info.offsets = { 0, 3, 6 };
//
//    mesh.Set_Var<Mesh_Var::Vertex_Info>()(v_info);
//
//}

template<size_t size>
void generate_icosadehdron(Mesh<size>& mesh) {

    float t = (1.0f + sqrt(5.0f)) * 0.5f;
    
    // add vertices
    //
    mesh.vertices.push_back(Vertex<size>{-1.0f,  t,     0.0f });
    mesh.vertices.push_back(Vertex<size>{ 1.0f,  t,     0.0f });
    mesh.vertices.push_back(Vertex<size>{-1.0f, -t,     0.0f });
    ////
    mesh.vertices.push_back(Vertex<size>{ 1.0f, -t,     0.0f });
    mesh.vertices.push_back(Vertex<size>{ 0.0f, -1.0f,  t    });
    mesh.vertices.push_back(Vertex<size>{ 0.0f,  1.0f,  t    });
    
    mesh.vertices.push_back(Vertex<size>{ 0.0f, -1.0f, -t    });
    mesh.vertices.push_back(Vertex<size>{ 0.0f,  1.0f, -t    });
    mesh.vertices.push_back(Vertex<size>{ t,     0.0f, -1.0f });
    //
    mesh.vertices.push_back(Vertex<size>{ t,     0.0f,  1.0f });
    mesh.vertices.push_back(Vertex<size>{-t,     0.0f, -1.0f });
    mesh.vertices.push_back(Vertex<size>{-t,     0.0f,  1.0f });
    
    mesh.indices.push_back(0);
    mesh.indices.push_back(11);
    mesh.indices.push_back(5);

    mesh.indices.push_back(0);
    mesh.indices.push_back(5);
    mesh.indices.push_back(1);

    mesh.indices.push_back(0);
    mesh.indices.push_back(1);
    mesh.indices.push_back(7);

    mesh.indices.push_back(0);
    mesh.indices.push_back(7);
    mesh.indices.push_back(10);

    mesh.indices.push_back(0);
    mesh.indices.push_back(10);
    mesh.indices.push_back(11);

    mesh.indices.push_back(1);
    mesh.indices.push_back(5);
    mesh.indices.push_back(9);

    mesh.indices.push_back(5);
    mesh.indices.push_back(11);
    mesh.indices.push_back(4);

    mesh.indices.push_back(11);
    mesh.indices.push_back(10);
    mesh.indices.push_back(2);

    mesh.indices.push_back(10);
    mesh.indices.push_back(7);
    mesh.indices.push_back(6);

    mesh.indices.push_back(7);
    mesh.indices.push_back(1);
    mesh.indices.push_back(8);

    mesh.indices.push_back(3);
    mesh.indices.push_back(9);
    mesh.indices.push_back(4);

    mesh.indices.push_back(3);
    mesh.indices.push_back(4);
    mesh.indices.push_back(2);

    mesh.indices.push_back(3);
    mesh.indices.push_back(2);
    mesh.indices.push_back(6);

    mesh.indices.push_back(3);
    mesh.indices.push_back(6);
    mesh.indices.push_back(8);

    mesh.indices.push_back(3);
    mesh.indices.push_back(8);
    mesh.indices.push_back(9);

    mesh.indices.push_back(4);
    mesh.indices.push_back(9);
    mesh.indices.push_back(5);

    mesh.indices.push_back(2);
    mesh.indices.push_back(4);
    mesh.indices.push_back(11);

    mesh.indices.push_back(6);
    mesh.indices.push_back(2);
    mesh.indices.push_back(10);

    mesh.indices.push_back(8);
    mesh.indices.push_back(6);
    mesh.indices.push_back(7);

    mesh.indices.push_back(9);
    mesh.indices.push_back(8);
    mesh.indices.push_back(1);

}

template<size_t size>
void create_sphere(Mesh<size>& mesh, glm::mat4 model, uint32_t tex_index)
{

    mesh.vertices.clear();
    mesh.indices.clear();

    generate_icosadehdron(mesh);

    size_t vertex_count{ mesh.vertices.size() };
    for (size_t i = 0; i < vertex_count; i++)
    {
        glm::vec4 pos{ mesh.vertices[i].vertices[0], mesh.vertices[i].vertices[1], mesh.vertices[i].vertices[2], 1.0f };
        pos = model * glm::normalize(pos);
        mesh.vertices[i].vertices[3] = mesh.vertices[i].vertices[0];
        mesh.vertices[i].vertices[4] = mesh.vertices[i].vertices[1];
        mesh.vertices[i].vertices[5] = mesh.vertices[i].vertices[2];
        mesh.vertices[i].vertices[0] = pos.x;
        mesh.vertices[i].vertices[1] = pos.y;
        mesh.vertices[i].vertices[2] = pos.z;
        mesh.vertices[i].vertices[6] = static_cast<float>(tex_index);
    }
    
    Vertex_Info v_info{};

    v_info.sizes = { 3, 3, 1 };
    v_info.strides = { static_cast<int32_t>(size) };
    v_info.offsets = { 0, 3, 6 };

    mesh.Set_Vertex_Info(v_info);

}
