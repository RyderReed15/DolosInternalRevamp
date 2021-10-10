#pragma once


#ifndef MATERIAL_SYSTEM_H
#define MATERIAL_SYSTEM_H

#include "../Math/Vector.h"
#include "IAppSystem.h"


class IAppSystem;

class KeyValues;
struct MaterialSystem_Config_t;
struct MaterialSystemHWID_t;
class IShader;
class IMatRenderContext;
class ICallQueue;
struct MorphWeight_t;
class IFileList;
struct VertexStreamSpec_t;
struct ShaderStencilState_t;
struct MeshInstanceData_t;
class IClientMaterialSystem;
class CPaintMaterial;
class IPaintMapDataManager;
class IPaintMapTextureManager;
class GPUMemoryStats;
struct AspectRatioInfo_t;
struct CascadedShadowMappingState_t;

class IMaterialProxyFactory;
class ITexture;
class IMaterialSystemHardwareConfig;
class CShadowMgr;

class CreateInterfaceFn;
class MaterialThreadMode_t;
class MaterialVideoMode_t;
class MaterialBufferReleaseFunc_t;
class ModeChangeCallbackFunc_t;
class MaterialAdapterInfo_t;
class ImageFormat;
class MaterialBufferRestoreFunc_t;
class HDRType_t;
class EndFrameCleanupFunc_t;
class OnLevelShutdownFunc_t;
class MaterialHandle_t;
class RenderTargetSizeMode_t;
class MaterialRenderTargetDepth_t;
class image_format;
class IMaterialVar;


enum class MaterialVarFlags_t {
    NO_DRAW = 1 << 2,
    IGNOREZ = 1 << 15,
    WIREFRAME = 1 << 28
};
class IMaterial
{
public:
    // Get the name of the material.  This is a full path to
    // the vmt file starting from "hl2/materials" (or equivalent) without
    // a file extension.
    virtual const char* GetName() const = 0;
    virtual const char* GetTextureGroupName() const = 0;


    virtual int /*PreviewImageRetVal_t*/ get_preview_image_properties(int* width, int* height,
        image_format* imageFormat,
        bool* isTranslucent) const = 0;


    virtual int  get_preview_image(unsigned char* data,
        int width, int height,
        image_format imageFormat) const = 0;
    //
    virtual int get_mapping_width() = 0;
    virtual int get_mapping_height() = 0;

    virtual int get_num_animation_frames() = 0;

    // For material subrects (material pages).  Offset(u,v) and scale(u,v) are normalized to texture.
    virtual bool in_material_page(void) = 0;
    virtual void get_material_offset(float* pOffset) = 0;
    virtual void get_material_scale(float* pScale) = 0;
    virtual IMaterial* get_material_page(void) = 0;

    // find a vmt variable.
    // This is how game code affects how a material is rendered.
    // The game code must know about the params that are used by
    // the shader for the material that it is trying to affect.
    virtual IMaterialVar* find_var(const char* varName, bool* found, bool complain = true) = 0;

    // The user never allocates or deallocates materials.  Reference counting is
    // used instead.  Garbage collection is done upon a call to
    // i_material_system::UncacheUnusedMaterials.
    virtual void increment_reference_count(void) = 0;
    virtual void decrement_reference_count(void) = 0;

    inline void add_ref()
    {
        increment_reference_count();
    }

    inline void release()
    {
        decrement_reference_count();
    }
    // Each material is assigned a number that groups it with like materials
    // for sorting in the application.
    virtual int get_enumeration_id(void) const = 0;

    virtual void get_low_res_color_sample(float s, float t, float* color) const = 0;

    // This computes the state snapshots for this material
    virtual void recompute_state_snapshots() = 0;

    // Are we translucent?
    virtual bool is_translucent() = 0;

    // Are we alphatested?
    virtual bool is_alpha_tested() = 0;

    // Are we vertex lit?
    virtual bool is_vertex_lit() = 0;

    // Gets the vertex format
    virtual void get_vertex_format() const = 0;

    // returns true if this material uses a material proxy
    virtual bool has_proxy(void) const = 0;

    virtual bool uses_env_cubemap(void) = 0;

    virtual bool needs_tangent_space(void) = 0;

    virtual bool needs_power_of_two_frame_buffer_texture(bool bCheckSpecificToThisFrame = true) = 0;
    virtual bool needs_full_frame_buffer_texture(bool bCheckSpecificToThisFrame = true) = 0;

    // returns true if the shader doesn't do skinning itself and requires
    // the data that is sent to it to be preskinned.
    virtual bool needs_software_skinning(void) = 0;

    // Apply constant color or alpha modulation
    virtual void AlphaModulate(float alpha) = 0;
    virtual void ColorModulate(float r, float g, float b) = 0;

    // Material Var flags...
    virtual void SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0; // Not used, index outdated, see below
    virtual bool GetMaterialVarFlag(MaterialVarFlags_t flag) const = 0;

    // Gets material reflectivity
    virtual void get_reflectivity(Vector& reflect) = 0;

    // Gets material property flags
    virtual bool get_property_flag(int /*MaterialPropertyTypes_t*/ type) = 0;

    // Is the material visible from both sides?
    virtual bool is_two_sided() = 0;

    // Sets the shader associated with the material
    virtual void set_shader(const char* pShaderName) = 0;

    // Can't be const because the material might have to precache itself.
    virtual int get_num_passes(void) = 0;

    // Can't be const because the material might have to precache itself.
    virtual int get_texture_memory_bytes(void) = 0;

    // Meant to be used with materials created using CreateMaterial
    // It updates the materials to reflect the current values stored in the material vars
    virtual void refresh() = 0;

    // GR - returns true is material uses lightmap alpha for blending
    virtual bool needs_lightmap_blend_alpha(void) = 0;

    // returns true if the shader doesn't do lighting itself and requires
    // the data that is sent to it to be prelighted
    virtual bool needs_software_lighting(void) = 0;

    // Gets at the shader parameters
    virtual int shader_param_count() const = 0;
    virtual IMaterialVar** get_shader_params(void) = 0;

    // Returns true if this is the error material you get back from i_material_system::FindMaterial if
    // the material can't be found.
    virtual bool IsErrorMaterial() const = 0;

    virtual void Unused() = 0;

    // Gets the current alpha modulation
    virtual float GetAlphaModulation() = 0;
    virtual void GetColorModulation(float* r, float* g, float* b) = 0;

    // Is this translucent given a particular alpha modulation?
    virtual bool is_translucent_under_modulation(float fAlphaModulation = 1.0f) const = 0;

    // fast find that stores the index of the found var in the string table in local cache
    virtual IMaterialVar* find_var_fast(char const* pVarName, unsigned int* pToken) = 0;

    // Sets new VMT shader parameters for the material
    virtual void set_shader_and_params(void* pKeyValues) = 0;
    virtual const char* get_shader_name() const = 0;

    virtual void delete_if_unreferenced() = 0;

    virtual bool is_sprite_card() = 0;

    virtual void call_bind_proxy(void* proxyData) = 0;

    virtual void refresh_preserving_material_vars() = 0;

    virtual bool was_reloaded_from_whitelist() = 0;

    virtual bool set_temp_excluded(bool bSet, int nExcludedDimensionLimit) = 0;

    virtual int get_reference_count() const = 0;


};


class IMaterialSystem : IAppSystem
{
public:

    virtual CreateInterfaceFn               Init(char const* pShaderAPIDLL, IMaterialProxyFactory* pMaterialProxyFactory, CreateInterfaceFn fileSystemFactory, CreateInterfaceFn cvarFactory) = 0;
    virtual void                            SetShaderAPI(char const* pShaderAPIDLL) = 0;
    virtual void                            SetAdapter(int nAdapter, int nFlags) = 0;
    virtual void                            ModInit() = 0;
    virtual void                            ModShutdown() = 0;
    virtual void                            SetThreadMode(MaterialThreadMode_t mode, int nServiceThread = -1) = 0;
    virtual MaterialThreadMode_t            GetThreadMode() = 0;
    virtual void                            ExecuteQueued() = 0;
    virtual void                            OnDebugEvent(const char* pEvent) = 0;
    virtual IMaterialSystemHardwareConfig* GetHardwareConfig(const char* pVersion, int* returnCode) = 0;
    virtual void                            __unknown() = 0;
    virtual bool                            UpdateConfig(bool bForceUpdate) = 0; //20
    virtual bool                            OverrideConfig(const MaterialSystem_Config_t& config, bool bForceUpdate) = 0;
    virtual const MaterialSystem_Config_t& GetCurrentConfigForVideoCard() const = 0;
    virtual bool                            GetRecommendedConfigurationInfo(int nDXLevel, KeyValues* pKeyValues) = 0;
    virtual int                             GetDisplayAdapterCount() const = 0;
    virtual int                             GetCurrentAdapter() const = 0;
    virtual void                            GetDisplayAdapterInfo(int adapter, MaterialAdapterInfo_t& info) const = 0;
    virtual int                             GetModeCount(int adapter) const = 0;
    virtual void                            GetModeInfo(int adapter, int mode, MaterialVideoMode_t& info) const = 0;
    virtual void                            AddModeChangeCallBack(ModeChangeCallbackFunc_t func) = 0;
    virtual void                            GetDisplayMode(MaterialVideoMode_t& mode) const = 0; //30
    virtual bool                            SetMode(void* hwnd, const MaterialSystem_Config_t& config) = 0;
    virtual bool                            SupportsMSAAMode(int nMSAAMode) = 0;
    virtual const MaterialSystemHWID_t& GetVideoCardIdentifier(void) const = 0;
    virtual void                            SpewDriverInfo() const = 0;
    virtual void                            GetBackBufferDimensions(int& width, int& height) const = 0;
    virtual ImageFormat                     GetBackBufferFormat() const = 0;
    virtual const AspectRatioInfo_t& GetAspectRatioInfo() const = 0;
    virtual bool                            SupportsHDRMode(HDRType_t nHDRModede) = 0;
    virtual bool                            AddView(void* hwnd) = 0;
    virtual void                            RemoveView(void* hwnd) = 0; //40
    virtual void                            SetView(void* hwnd) = 0;
    virtual void                            BeginFrame(float frameTime) = 0;
    virtual void                            EndFrame() = 0;
    virtual void                            Flush(bool flushHardware = false) = 0;
    virtual unsigned int                    GetCurrentFrameCount() = 0;
    virtual void                            SwapBuffers() = 0;
    virtual void                            EvictManagedResources() = 0;
    virtual void                            ReleaseResources(void) = 0;
    virtual void                            ReacquireResources(void) = 0;
    virtual void                            AddReleaseFunc(MaterialBufferReleaseFunc_t func) = 0; //50
    virtual void                            RemoveReleaseFunc(MaterialBufferReleaseFunc_t func) = 0;
    virtual void                            AddRestoreFunc(MaterialBufferRestoreFunc_t func) = 0;
    virtual void                            RemoveRestoreFunc(MaterialBufferRestoreFunc_t func) = 0;
    virtual void                            AddEndFrameCleanupFunc(EndFrameCleanupFunc_t func) = 0;
    virtual void                            RemoveEndFrameCleanupFunc(EndFrameCleanupFunc_t func) = 0;
    virtual void                            OnLevelShutdown() = 0;
    virtual bool                            AddOnLevelShutdownFunc(OnLevelShutdownFunc_t func, void* pUserData) = 0;
    virtual bool                            RemoveOnLevelShutdownFunc(OnLevelShutdownFunc_t func, void* pUserData) = 0;
    virtual void                            OnLevelLoadingComplete() = 0;
    virtual void                            ResetTempHWMemory(bool bExitingLevel = false) = 0; //60
    virtual void                            HandleDeviceLost() = 0;
    virtual int                             ShaderCount() const = 0;
    virtual int                             GetShaders(int nFirstShader, int nMaxCount, IShader** ppShaderList) const = 0;
    virtual int                             ShaderFlagCount() const = 0;
    virtual const char* ShaderFlagName(int nIndex) const = 0;
    virtual void                            GetShaderFallback(const char* pShaderName, char* pFallbackShader, int nFallbackLength) = 0;
    virtual IMaterialProxyFactory* GetMaterialProxyFactory() = 0;
    virtual void                            SetMaterialProxyFactory(IMaterialProxyFactory* pFactory) = 0;
    virtual void                            EnableEditorMaterials() = 0;
    virtual void                            EnableGBuffers() = 0; //70
    virtual void                            SetInStubMode(bool bInStubMode) = 0;
    virtual void                            DebugPrintUsedMaterials(const char* pSearchSubString, bool bVerbose) = 0;
    virtual void                            DebugPrintUsedTextures(void) = 0;
    virtual void                            ToggleSuppressMaterial(char const* pMaterialName) = 0;
    virtual void                            ToggleDebugMaterial(char const* pMaterialName) = 0;
    virtual bool                            UsingFastClipping(void) = 0;
    virtual int                             StencilBufferBits(void) = 0; //number of bits per pixel in the stencil buffer
    virtual void                            UncacheAllMaterials() = 0;
    virtual void                            UncacheUnusedMaterials(bool bRecomputeStateSnapshots = false) = 0;
    virtual void                            CacheUsedMaterials() = 0; //80
    virtual void                            ReloadTextures() = 0;
    virtual void                            ReloadMaterials(const char* pSubString = NULL) = 0;
    virtual IMaterial* CreateMaterial(const char* pMaterialName, KeyValues* pVMTKeyValues) = 0;
    virtual IMaterial* FindMaterial(char const* pMaterialName, const char* pTextureGroupName = nullptr, bool complain = true, const char* pComplainPrefix = NULL) = 0;
    virtual void							unk0() = 0;
    virtual MaterialHandle_t                FirstMaterial() const = 0;
    virtual MaterialHandle_t                NextMaterial(MaterialHandle_t h) const = 0;
    virtual MaterialHandle_t                InvalidMaterial() const = 0;
    virtual IMaterial* GetMaterial(MaterialHandle_t h) const = 0;
    virtual int                             GetNumMaterials() const = 0;
    virtual ITexture* FindTexture(char const* pTextureName, const char* pTextureGroupName, bool complain = true) = 0;
    virtual bool                            IsTextureLoaded(char const* pTextureName) const = 0;
    virtual ITexture* CreateProceduralTexture(const char* pTextureName, const char* pTextureGroupName, int w, int h, ImageFormat fmt, int nFlags) = 0;
    virtual void                            BeginRenderTargetAllocation() = 0;
    virtual void                            EndRenderTargetAllocation() = 0; // Simulate an Alt-Tab in here, which causes a release/restore of all resources
    virtual ITexture* CreateRenderTargetTexture(int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat	format, MaterialRenderTargetDepth_t depth) = 0;
    virtual ITexture* CreateNamedRenderTargetTextureEx(const char* pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth, unsigned int textureFlags, unsigned int renderTargetFlags = 0) = 0;
    virtual ITexture* CreateNamedRenderTargetTexture(const char* pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth, bool bClampTexCoords = true, bool bAutoMipMap = false) = 0;
    virtual ITexture* CreateNamedRenderTargetTextureEx2(const char* pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth, unsigned int textureFlags, unsigned int renderTargetFlags = 0) = 0;
   
};
#endif // !MATERIAL_SYSTEM_H
