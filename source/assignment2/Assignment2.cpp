#include "assignment2/Assignment2.h"
#include "common/core.h" // <-- haha.
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"

Assignment2::Assignment2(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera))
{
}

std::unique_ptr<Application> Assignment2::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return std::make_unique<Assignment2>(std::move(scene), std::move(camera));
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
    case SDLK_UP:
        sphereObject->Rotate(glm::vec3(SceneObject::GetWorldRight()), -0.1f);
        break;
    case SDLK_DOWN:
        sphereObject->Rotate(glm::vec3(SceneObject::GetWorldRight()), 0.1f);
        break;
    case SDLK_RIGHT:
        sphereObject->Rotate(glm::vec3(SceneObject::GetWorldUp()), 0.1f);
        break;
    case SDLK_LEFT:
        sphereObject->Rotate(glm::vec3(SceneObject::GetWorldUp()), -0.1f);
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
        sphereObject->AddScale(0.1f);
        break;
    case SDLK_MINUS:
        sphereObject->AddScale(-0.1f);
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
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/vert/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/vert/blinnphong.frag"}
    };
    std::shared_ptr<BlinnPhongShader> shader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_VERTEX_SHADER);
    shader->SetDiffuse(glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
    shader->SetSpecular(glm::vec4(0.9f, 0.9f, 0.9f, 1.f), 40.f);
    shader->SetAmbient(glm::vec4(0.1f));

    std::shared_ptr<RenderingObject> sphereTemplate = PrimitiveCreator::CreateIcoSphere(shader, 5.f, 3);

    // Give a R/G/B color to each vertex to visualize the sphere.
    const auto totalVertices = sphereTemplate->GetTotalVertices();

    std::unique_ptr<RenderingObject::ColorArray> vertexColors = std::make_unique<RenderingObject::ColorArray>();
    vertexColors->reserve(totalVertices);

    for (auto i = 0; i < totalVertices; ++i) {
        if (i % 3 == 0) {
            vertexColors->emplace_back(1.f, 0.f, 0.f, 1.f);
        } else if (i % 3 == 1) { 
            vertexColors->emplace_back(0.f, 1.f, 0.f, 1.f);
        } else if (i % 3 == 2) {
            vertexColors->emplace_back(0.f, 0.f, 1.f, 1.f);
        }
    }
    sphereTemplate->SetVertexColors(std::move(vertexColors));

    sphereObject = std::make_shared<SceneObject>(sphereTemplate);
    scene->AddSceneObject(sphereObject);

    std::unique_ptr<BlinnPhongLightProperties> lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

    std::shared_ptr<Light> pointLight = std::make_shared<Light>(std::move(lightProperties));
    pointLight->Translate(glm::vec3(0.f, 0.f, 10.f));

    scene->AddLight(pointLight);
}

void Assignment2::SetupExample2()
{
    scene->ClearScene();
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/frag/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/frag/blinnphong.frag"}
    };
    std::shared_ptr<BlinnPhongShader> shader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
    shader->SetDiffuse(glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
    shader->SetSpecular(glm::vec4(0.9f, 0.9f, 0.9f, 1.f), 40.f);
    shader->SetAmbient(glm::vec4(0.1f));

    std::shared_ptr<RenderingObject> sphereTemplate = PrimitiveCreator::CreateIcoSphere(shader, 5.f, 3);

    // Give a R/G/B color to each vertex to visualize the sphere.
    const auto totalVertices = sphereTemplate->GetTotalVertices();

    std::unique_ptr<RenderingObject::ColorArray> vertexColors = std::make_unique<RenderingObject::ColorArray>();
    vertexColors->reserve(totalVertices);

    for (auto i = 0; i < totalVertices; ++i) {
        if (i % 3 == 0) {
            vertexColors->emplace_back(1.f, 0.f, 0.f, 1.f);
        } else if (i % 3 == 1) { 
            vertexColors->emplace_back(0.f, 1.f, 0.f, 1.f);
        } else if (i % 3 == 2) {
            vertexColors->emplace_back(0.f, 0.f, 1.f, 1.f);
        }
    }
    sphereTemplate->SetVertexColors(std::move(vertexColors));

    sphereObject = std::make_shared<SceneObject>(sphereTemplate);
    scene->AddSceneObject(sphereObject);

    std::unique_ptr<BlinnPhongLightProperties> lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

    std::shared_ptr<Light> pointLight = std::make_shared<Light>(std::move(lightProperties));
    pointLight->Translate(glm::vec3(0.f, 0.f, 10.f));

    scene->AddLight(pointLight);
}
