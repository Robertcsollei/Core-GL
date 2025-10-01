#pragma once
#include <string>
#include <terrakit/core/config/ResourceHandle.h>
#include <terrakit/core/libraries/MaterialLibrary.h>
#include <terrakit/core/libraries/MeshLibrary.h>
#include <terrakit/core/libraries/TextureLibrary.h>

namespace terrakit::core::config {

class ResourceManager {
public:
    ResourceManager(const std::string& assetsPath = "assets/",
                   const std::string& texturesPath = "assets/textures/");

    MeshHandle GetMesh(const std::string& id);
    MeshHandle CreateMesh(const std::string& id, MeshHandle mesh);

    MaterialHandle CreateMaterial(MaterialHandle material);
    MaterialHandle GetMaterial(const std::string& id);

    TextureHandle LoadTexture(const std::string& path);
    TextureHandle GetTexture(const std::string& id);

    std::string GetAssetsPath() const { return m_AssetsPath; }
    std::string GetTexturesPath() const { return m_TexturesPath; }

    // Direct library access (temporary - for gradual migration)
    MeshLibrary& meshes() { return m_Meshes; }
    MaterialLibrary& materials() { return m_Materials; }
    TextureLibrary& textures() { return m_Textures; }

private:
    std::string m_AssetsPath;
    std::string m_TexturesPath;

    MeshLibrary m_Meshes;
    MaterialLibrary m_Materials;
    TextureLibrary m_Textures;
};

} // namespace terrakit::core::config