#include "pch.h"
#include "15. VecAddDemo.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Waves.h"
#include "ComputeTest.h"

void VecAddDemo::Init()
{
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		{
			ShaderInfo info;
			info.shaderType = SHADER_TYPE::COMPUTE;

			ShaderArg args;
			args.cs = "CS";

			shader->Init(L"..\\Shaders\\13. VecAdd.fx", info, args);
		}

		shared_ptr<ComputeTest> test = make_shared<ComputeTest>();
		test->SetShader(shader);

		
		

		shared_ptr<GameObject> grid = make_shared<GameObject>();
		grid->GetOrAddTransform();
		grid->AddComponent(test);
		CUR_SCENE->Add(grid);
	}


	shared_ptr<GameObject> cameraObject = make_shared<GameObject>();
	cameraObject->GetOrAddTransform();
	cameraObject->AddComponent(make_shared<Camera>());
	cameraObject->AddComponent(make_shared<CameraScript>());
	CUR_SCENE->Add(cameraObject);

	GRAPHICS->GetCommandQueue()->WaitSync();
	GRAPHICS->GetComputeQueue()->WaitSync();
}

void VecAddDemo::Update()
{


}

void VecAddDemo::Render()
{
}