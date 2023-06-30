#pragma once
#include "GameApp.h"

class GameObject; 
class RenderObject;
class Material;



class PickingDemo : public GameApp
{
public:
    PickingDemo(HINSTANCE hInstance);
    virtual ~PickingDemo();

    virtual bool Init() override;

protected:
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

    void LoadTextures();
    void BuildRootSignature();
    void BuildDescriptorHeaps();
    void BuildShadersAndInputLayout();
    void BuildCarGeometry();
    void BuildMaterials();
    void BuildRenderItems();
    void BuildFrameResources();
    void BuildPSOs();
    void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<shared_ptr<RenderObject>>& ritems);
    void Pick(int posX, int posY);

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

    shared_ptr<RenderObject> _pickedItem = nullptr;

    PassConstants _mainConstant;

    Camera _camera;

    POINT _lastMousepos;
};

