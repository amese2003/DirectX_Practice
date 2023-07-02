#pragma once
#include "GameApp.h"
#include "CubeRenderTarget.h"

class GameObject; 
class RenderObject;
class Material;

const UINT CubeMapSize = 512;

class DynamicCubeDemo : public GameApp
{
public:
    DynamicCubeDemo(HINSTANCE hInstance);
    virtual ~DynamicCubeDemo();

    virtual bool Init() override;

protected:
    virtual void CreateRtvAndDsvDescriptorHeaps() override;
    virtual void OnResize() override;
    virtual void Update() override;
    virtual void Draw() override;


    virtual void OnMouseDown(WPARAM btnState, int x, int y) override;
    virtual void OnMouseUp(WPARAM btnState, int x, int y)   override;
    virtual void OnMouseMove(WPARAM btnState, int x, int y) override;

    void OnKeyboardInput();
    void AnimateMaterials();
    void UpdateObjectConstant();
    void UpdateMaterialBuffer();
    void UpdateMainPassCB();
    void UpdateCubeMapConstant();

    void LoadTextures();
    void BuildRootSignature();
    void BuildDescriptorHeaps();
    void BuildCubeDepthStencil();
    void BuildShadersAndInputLayout();
    void BuildMeshGeometry();
    void BuildCarGeometry();
    void BuildSkullGeometry();
    void BuildMaterials();
    void BuildRenderItems();
    void BuildFrameResources();
    void BuildPSOs();
    void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<shared_ptr<RenderObject>>& ritems);
    void DrawSceneToCubeMap();
    //void Pick(int posX, int posY);

    void BuildCubeFaceCamera(float x, float y, float z);

    std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

private:

    std::vector<std::shared_ptr<FrameResource>> _frameResources;
    shared_ptr<FrameResource> _currFrameResource = nullptr;
    int _currFrameResourceIndex = 0;

    UINT _cbvSrvDescriptorSize = 0;

    ComPtr<ID3D12RootSignature> _rootSignature = nullptr;
    ComPtr<ID3D12DescriptorHeap> _srvHeap = nullptr;

    std::unordered_map<std::string, std::shared_ptr<MeshGeometry>> _geometry;
    std::unordered_map<std::string, std::shared_ptr<Material>> _material;
    std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;


    std::unordered_map<std::string, ComPtr<ID3DBlob>> _shaders;
    std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> _pipelineStates;

    std::vector<D3D12_INPUT_ELEMENT_DESC> _inputLayout;

    // List of all the render items.
    std::vector<std::shared_ptr<RenderObject>> _gameObjects;

    // Render items divided by PSO.
    std::vector<shared_ptr<RenderObject>> _renderItems[static_cast<int>(RenderLayer::Count)];

    shared_ptr<RenderObject> _skullItem = nullptr;

    UINT _skyTexHeapIndex = 0;
    UINT _dynamicTexHeapIndex = 0;

    PassConstants _mainConstant;

    Camera _camera;
    Camera _cubeMapCamera[6];

    POINT _lastMousepos;

    shared_ptr<CubeRenderTarget> _dynamicCubeMap = nullptr;
    CD3DX12_CPU_DESCRIPTOR_HANDLE _cubedsvhandle;

    ComPtr<ID3D12Resource> _cubeDepthStencilBuffer = nullptr;
};

