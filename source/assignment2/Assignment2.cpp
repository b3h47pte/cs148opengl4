#include "assignment2/Assignment2.h"
#include "common/core.h" // <-- haha.
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include <cmath>

Assignment2::Assignment2(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera)), elapsedTime(0.f)
{
}

std::unique_ptr<Application> Assignment2::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment2>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment2::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(75.f, 1280.f / 720.f);
}

glm::vec2 Assignment2::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment2::SetupScene()
{
    SetupExample1();
}

void Assignment2::SetupCamera()
{
    camera->Translate(glm::vec3(0.f, 0.f, 10.f));
}

void Assignment2::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp)
{   
    if (state != SDL_KEYDOWN) {
        return;
    }

    switch (key.sym) {
    case SDLK_1:
        if (!repeat && state == SDL_KEYDOWN) {
            SetupExample1();
        }
        break;
    case SDLK_2:
        if (!repeat && state == SDL_KEYDOWN) {
            SetupExample2();
        }
        break;
    case SDLK_3:
        if (!repeat && state == SDL_KEYDOWN) {
            SetupExample3();
        }
        break;
    case SDLK_UP:
        sceneObject->Rotate(glm::vec3(SceneObject::GetWorldRight()), -0.1f);
        break;
    case SDLK_DOWN:
        sceneObject->Rotate(glm::vec3(SceneObject::GetWorldRight()), 0.1f);
        break;
    case SDLK_RIGHT:
        sceneObject->Rotate(glm::vec3(SceneObject::GetWorldUp()), 0.1f);
        break;
    case SDLK_LEFT:
        sceneObject->Rotate(glm::vec3(SceneObject::GetWorldUp()), -0.1f);
        break;
    case SDLK_w:
        camera->Translate(glm::vec3(SceneObject::GetWorldForward() * -0.3f));
        break;
    case SDLK_a:
        camera->Translate(glm::vec3(SceneObject::GetWorldRight() * 0.3f));
        break;
    case SDLK_s:
        camera->Translate(glm::vec3(SceneObject::GetWorldForward() * 0.3f));
        break;
    case SDLK_d:
        camera->Translate(glm::vec3(SceneObject::GetWorldRight() * -0.3f));
        break;
    case SDLK_SPACE:
        camera->Translate(glm::vec3(SceneObject::GetWorldUp() * -0.3f));
        break;
    case SDLK_LCTRL:
    case SDLK_RCTRL:
        camera->Translate(glm::vec3(SceneObject::GetWorldUp() * 0.3f));
        break;
    case SDLK_EQUALS:
        sceneObject->AddScale(0.1f);
        break;
    case SDLK_MINUS:
        sceneObject->AddScale(-0.1f);
        break;
    default:
        Application::HandleInput(key, state, repeat, timestamp);
        break;
    }
}

void Assignment2::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
    std::static_pointer_cast<PerspectiveCamera>(camera)->SetAspectRatio(x / y);
}

void Assignment2::SetupExample1()
{
    scene->ClearScene();
#ifndef DISABLE_OPENGL_SUBROUTINES
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/vert/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/vert/blinnphong.frag"}
    };
#else
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/vert/noSubroutine/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/vert/noSubroutine/blinnphong.frag"}
    };
#endif
    std::shared_ptr<BlinnPhongShader> shader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_VERTEX_SHADER);
    shader->SetDiffuse(glm::vec4(0.3f, 0.3f, 0.3f, 1.f));
    shader->SetSpecular(glm::vec4(0.9f, 0.9f, 0.9f, 1.f), 40.f);
    shader->SetAmbient(glm::vec4(0.1f));

    std::shared_ptr<RenderingObject> sphereTemplate = PrimitiveCreator::CreateIcoSphere(shader, 5.f, 4);

    // Give a R/G/B color to each vertex to visualize the sphere.
    const auto totalVertices = sphereTemplate->GetTotalVertices();

    std::unique_ptr<RenderingObject::ColorArray> vertexColors = make_unique<RenderingObject::ColorArray>();
    vertexColors->reserve(totalVertices);

    for (auto i = 0; i < totalVertices; ++i) {
        vertexColors->emplace_back(0.5f, 0.5f, 0.5f, 1.f);
    }
    
    sphereTemplate->SetVertexColors(std::move(vertexColors));

    sceneObject = std::make_shared<SceneObject>(sphereTemplate);
    scene->AddSceneObject(sceneObject);

    std::unique_ptr<BlinnPhongLightProperties> lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

    pointLight = std::make_shared<Light>(std::move(lightProperties));

    lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(1.f, 0.f, 0.f, 1.f);
    lightProperties->specularColor = glm::vec4(0.f, 1.f, 0.f, 1.f);
    pointLight2 = std::make_shared<Light>(std::move(lightProperties));

    lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(0.f, 0.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 0.f, 0.f, 1.f);
    pointLight3 = std::make_shared<Light>(std::move(lightProperties));

    scene->AddLight(pointLight);
    scene->AddLight(pointLight2);
    scene->AddLight(pointLight3);
}

void Assignment2::SetupExample2()
{
    scene->ClearScene();
#ifndef DISABLE_OPENGL_SUBROUTINES
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/frag/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/frag/blinnphong.frag"}
    };
#else
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/frag/noSubroutine/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/frag/noSubroutine/blinnphong.frag"}
    };
#endif
    std::shared_ptr<BlinnPhongShader> shader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
    shader->SetDiffuse(glm::vec4(0.3f, 0.3f, 0.3f, 1.f));
    shader->SetSpecular(glm::vec4(0.9f, 0.9f, 0.9f, 1.f), 40.f);
    shader->SetAmbient(glm::vec4(0.1f));

    std::shared_ptr<RenderingObject> sphereTemplate = PrimitiveCreator::CreateIcoSphere(shader, 5.f, 4);

    // Give a R/G/B color to each vertex to visualize the sphere.
    const auto totalVertices = sphereTemplate->GetTotalVertices();

    std::unique_ptr<RenderingObject::ColorArray> vertexColors = make_unique<RenderingObject::ColorArray>();
    vertexColors->reserve(totalVertices);

    for (auto i = 0; i < totalVertices; ++i) {
        vertexColors->emplace_back(0.5f, 0.5f, 0.5f, 1.f);
    }
    sphereTemplate->SetVertexColors(std::move(vertexColors));

    sceneObject = std::make_shared<SceneObject>(sphereTemplate);
    scene->AddSceneObject(sceneObject);

    std::unique_ptr<BlinnPhongLightProperties> lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

    pointLight = std::make_shared<Light>(std::move(lightProperties));

    lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(1.f, 0.f, 0.f, 1.f);
    lightProperties->specularColor = glm::vec4(0.f, 1.f, 0.f, 1.f);
    pointLight2 = std::make_shared<Light>(std::move(lightProperties));

    lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(0.f, 0.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 0.f, 0.f, 1.f);
    pointLight3 = std::make_shared<Light>(std::move(lightProperties));

    scene->AddLight(pointLight);
    scene->AddLight(pointLight2);
    scene->AddLight(pointLight3);
}

void Assignment2::SetupExample3()
{
    scene->ClearScene();
#ifndef DISABLE_OPENGL_SUBROUTINES
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/frag/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/frag/blinnphong.frag"}
    };
#else
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/frag/noSubroutine/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/frag/noSubroutine/blinnphong.frag"}
    };
#endif
    std::shared_ptr<BlinnPhongShader> shader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
    shader->SetDiffuse(glm::vec4(0.3f, 0.3f, 0.3f, 1.f));
    shader->SetSpecular(glm::vec4(0.9f, 0.9f, 0.9f, 1.f), 40.f);
    shader->SetAmbient(glm::vec4(0.1f));

    std::vector<std::shared_ptr<RenderingObject>> meshTemplate = MeshLoader::LoadMesh(shader, "outlander/Model/Outlander_Model.obj");
    if (meshTemplate.empty()) {
        std::cerr << "ERROR: Failed to load the model. Check your paths." << std::endl;
        return;
    }

    sceneObject = std::make_shared<SceneObject>(meshTemplate);
    scene->AddSceneObject(sceneObject);

    std::unique_ptr<BlinnPhongLightProperties> lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

    pointLight = std::make_shared<Light>(std::move(lightProperties));

    lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(1.f, 0.f, 0.f, 1.f);
    lightProperties->specularColor = glm::vec4(0.f, 1.f, 0.f, 1.f);
    pointLight2 = std::make_shared<Light>(std::move(lightProperties));

    lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(0.f, 0.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 0.f, 0.f, 1.f);
    pointLight3 = std::make_shared<Light>(std::move(lightProperties));

    scene->AddLight(pointLight);
    scene->AddLight(pointLight2);
    scene->AddLight(pointLight3);
}

void Assignment2::Tick(double deltaTime)
{
    glm::vec3 position(std::sin(elapsedTime), 0.f, std::cos(elapsedTime));
    glm::vec3 position2(std::sin(elapsedTime), std::cos(elapsedTime), 0.f);
    glm::vec3 position3(0.f, std::sin(elapsedTime), std::cos(elapsedTime));
    position *= 10.f;
    position2 *= 10.f;
    position3 *= 10.f;
    if (pointLight) {
        pointLight->SetPosition(position);
    }

    if (pointLight2) {
        pointLight2->SetPosition(position2);
    }

    if (pointLight3) {
        pointLight3->SetPosition(position3);
    }
    elapsedTime += deltaTime;
}
