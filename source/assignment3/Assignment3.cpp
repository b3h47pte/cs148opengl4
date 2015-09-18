#include "assignment3/Assignment3.h"
#include "common/core.h" // <-- haha.
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include "common/Utility/Texture/TextureLoader.h"
#include <cmath>

Assignment3::Assignment3(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera)), elapsedTime(0.f)
{
}

std::unique_ptr<Application> Assignment3::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return std::make_unique<Assignment3>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment3::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(75.f, 1280.f / 720.f);
}

glm::vec2 Assignment3::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment3::SetupScene()
{
    SetupExample1();
}

void Assignment3::SetupCamera()
{
    camera->Translate(glm::vec3(0.f, 0.f, 10.f));
}

void Assignment3::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp)
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

void Assignment3::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
    std::static_pointer_cast<PerspectiveCamera>(camera)->SetAspectRatio(x / y);
}

void Assignment3::SetupExample1()
{
    scene->ClearScene();
#ifndef DISABLE_OPENGL_SUBROUTINES
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/fragTexture/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/fragTexture/blinnphong.frag"}
    };
#else
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.frag"}
    };
#endif
    std::vector<std::shared_ptr<RenderingObject>> meshTemplate = MeshLoader::LoadMesh(nullptr, "outlander/Model/Outlander_Model.obj");
    if (meshTemplate.empty()) {
        std::cerr << "ERROR: Failed to load the model. Check your paths." << std::endl;
        return;
    }

    for (auto i = 0; i < meshTemplate.size(); ++i) {
        std::shared_ptr<BlinnPhongShader> shader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
        shader->SetDiffuse(glm::vec4(0.3f, 0.3f, 0.3f, 1.f));
        shader->SetSpecular(glm::vec4(0.9f, 0.9f, 0.9f, 1.f), 40.f);
        shader->SetAmbient(glm::vec4(0.7f));

        switch (i) {
        case 0:
            shader->SetTexture(BlinnPhongShader::TextureSlots::DIFFUSE, TextureLoader::LoadTexture("outlander/TGA/Outlander_Backpack_Diffuse.tga"));
            shader->SetTexture(BlinnPhongShader::TextureSlots::SPECULAR, TextureLoader::LoadTexture("outlander/TGA/Outlander_Backpack_Spec.tga"));
            break;
        case 1:
            shader->SetTexture(BlinnPhongShader::TextureSlots::DIFFUSE, TextureLoader::LoadTexture("outlander/TGA/Outlander_Body_Diffuse.tga"));
            shader->SetTexture(BlinnPhongShader::TextureSlots::SPECULAR, TextureLoader::LoadTexture("outlander/TGA/Outlander_Body_Spec.tga"));
            break;
        case 2:
            shader->SetTexture(BlinnPhongShader::TextureSlots::DIFFUSE, TextureLoader::LoadTexture("outlander/TGA/Outlander_HeadArmsPacks_Diffuse.tga"));
            shader->SetTexture(BlinnPhongShader::TextureSlots::SPECULAR, TextureLoader::LoadTexture("outlander/TGA/Outlander_HeadArmsPacks_Spec.tga"));
            break;
        default:
            std::cerr << "WARNING: Unhandled mesh group when assigning a shader." << std::endl;
            break;
        }

        meshTemplate[i]->SetShader(std::move(shader));
    }

    sceneObject = std::make_shared<SceneObject>(meshTemplate);
    scene->AddSceneObject(sceneObject);

    std::unique_ptr<BlinnPhongLightProperties> lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

    pointLight = std::make_shared<Light>(std::move(lightProperties));

    lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    pointLight2 = std::make_shared<Light>(std::move(lightProperties));

    lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    pointLight3 = std::make_shared<Light>(std::move(lightProperties));

    lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    pointLight4 = std::make_shared<Light>(std::move(lightProperties));

    scene->AddLight(pointLight);
    scene->AddLight(pointLight2);
    scene->AddLight(pointLight3);
    scene->AddLight(pointLight4);
}

void Assignment3::Tick(double deltaTime)
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

    pointLight4->SetPosition(glm::vec3(camera->GetPosition()));
    elapsedTime += deltaTime;
}
