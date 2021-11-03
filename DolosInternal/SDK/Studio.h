#pragma once

#ifndef STUDIO_H
#define STUDIO_H

#include "Math/Vector.h"
#include "Math/VMatrix.h"


#define MAX_NUM_LODS 8
#define MODEL_VERTEX_FILE_ID		(('V'<<24)+('S'<<16)+('D'<<8)+'I')
typedef float Quaternion[4];
typedef float RadianEuler[3];



class studiohdr_t;
struct mstudiomodel_t;
struct studiohwdata_t;
struct mstudiomodel_t;

struct vertexFileHeader_t
{

	int     id;                             // MODEL_VERTEX_FILE_ID
	int     version;                        // MODEL_VERTEX_FILE_VERSION
	int     checksum;                       // same as studiohdr_t, ensures sync
	int     numLODs;                        // num of valid lods
	int     numLODVertexes[MAX_NUM_LODS];   // num verts for desired root lod
	int     numFixups;                      // num of vertexFileFixup_t
	int     fixupTableStart;                // offset from base to fixup table
	int     vertexDataStart;                // offset from base to vertex block
	int     tangentDataStart;               // offset from base to tangent block

};

struct studiohwdata_t
{
    int					m_RootLOD;	// calced and clamped, nonzero for lod culling
    int					m_NumLODs;
    int*				m_pLODs;
    int					m_NumStudioMeshes;

    inline float	LODMetric(float unitSphereSize) const { return (unitSphereSize != 0.0f) ? (100.0f / unitSphereSize) : 0.0f; }
    inline int		GetLODForMetric(float lodMetric) const
    {
        return 0;
    }


    // Each model counts how many rendered faces it accounts for each frame:
    inline void		UpdateFacesRenderedCount(studiohdr_t* pStudioHdr, int& hwDataHash, int nLOD, int nInstances, int nFacesOverride = -1)
    {

    }
    int					m_NumFacesRenderedThisFrame;
    int					m_NumTimesRenderedThisFrame;
    studiohdr_t*		m_pStudioHdr; // There is no way to map between these inside CStudioRender, so we have to store it.
 // !_CERT
};
struct mstudioflex_t
{

	int					flexdesc;	// input value

	float				target0;	// zero
	float				target1;	// one
	float				target2;	// one
	float				target3;	// zero

	int					numverts;
	int					vertindex;
};
struct mstudio_meshvertexdata_t
{

	Vector*				Position(int i) const;
	Vector*				Normal(int i) const;
	int*				TangentS(int i) const;
	int*				Texcoord(int i) const;
	int*				BoneWeights(int i) const;
	int*				Vertex(int i) const;
	bool				HasTangentData(void) const;
	int					GetModelVertexIndex(int i) const;
	int					GetGlobalVertexIndex(int i) const;

	// indirection to this mesh's model's vertex data
	const int* modelvertexdata;

	// used for fixup calcs when culling top level lods
	// expected number of mesh verts at desired lod
	int					numLODVertexes[MAX_NUM_LODS];
};
struct mstudiomesh_t
{

	int					material;

	int					modelindex;
	mstudiomodel_t* pModel() const;

	int					numvertices;		// number of unique vertices/normals/texcoords
	int					vertexoffset;		// vertex mstudiovertex_t

	// Access thin/fat mesh vertex data (only one will return a non-NULL result)
	const int* GetVertexData(void* pModelData = NULL);
	const int* GetThinVertexData(void* pModelData = NULL);

	int					numflexes;			// vertex animation
	int					flexindex;
	inline mstudioflex_t* pFlex(int i) const { return (mstudioflex_t*)(((byte*)this) + flexindex) + i; };

	// special codes for material operations
	int					materialtype;
	int					materialparam;

	// a unique ordinal for this mesh
	int					meshid;

	Vector				center;

	mstudio_meshvertexdata_t vertexdata;

	int					unused[8]; // remove as appropriate

	mstudiomesh_t() {}
private:
	// No copy constructors allowed
	mstudiomesh_t(const mstudiomesh_t& vOther);
};
struct mstudiomodel_t
{

	inline const char* pszName(void) const { return name; }
	char				name[64];

	int					type;

	float				boundingradius;

	int					nummeshes;
	int					meshindex;
	inline mstudiomesh_t* pMesh(int i) const { return (mstudiomesh_t*)(((byte*)this) + meshindex) + i; };

	// cache purposes
	int					numvertices;		// number of unique vertices/normals/texcoords
	int					vertexindex;		// vertex Vector
	int					tangentsindex;		// tangents Vector

	// These functions are defined in application-specific code:
	const vertexFileHeader_t* CacheVertexData(void* pModelData);

	// Access thin/fat mesh vertex data (only one will return a non-NULL result)
	const int* GetVertexData(void* pModelData = NULL);
	const int* GetThinVertexData(void* pModelData = NULL);

	int					numattachments;
	int					attachmentindex;

	int					numeyeballs;
	int					eyeballindex;
	inline  int* pEyeball(int i) { return (int*)(((byte*)this) + eyeballindex) + i; };

	int vertexdata;

	int					unused[8];		// remove as appropriate
};
struct model_t
{
	void* fnHandle;               //0x0000 
	char    szName[260];            //0x0004 
	__int32 nLoadFlags;             //0x0108 
	__int32 nServerCount;           //0x010C 
	__int32 type;                   //0x0110 
	__int32 flags;                  //0x0114 
	Vector  vecMins;                //0x0118 
	Vector  vecMaxs;                //0x0124 
	float   radius;                 //0x0130 
	char    pad[0x1C];              //0x0134
};
struct mstudiobone_t
{
	int                    sznameindex;
	inline char* const    pszName(void) const { return ((char*)this) + sznameindex; }
	int                    parent;
	int                    bonecontroller[6];    // bone controller index, -1 == none
	Vector                 pos;
	Quaternion             quat;
	RadianEuler            rot;
	// compression scale
	Vector                 posscale;
	Vector                 rotscale;

	matrix3x4_t            poseToBone;
	Quaternion             qAlignment;
	int                    flags;
	int                    proctype;
	int                    procindex;
	mutable int            physicsbone;
	inline void* pProcedure() const { if (procindex == 0) return NULL; else return  (void*)(((byte*)this) + procindex); };
	int                    surfacepropidx;
	inline char* const    pszSurfaceProp(void) const { return ((char*)this) + surfacepropidx; }
	inline int             GetSurfaceProp(void) const { return surfacepropLookup; }

	int                    contents;
	int                    surfacepropLookup;
	int                    m_iPad01[7];

	mstudiobone_t() {}
private:
	// No copy constructors allowed
	mstudiobone_t(const mstudiobone_t& vOther);
};


struct mstudiobbox_t
{
	int         bone;
	int         group;
	Vector      bbmin;
	Vector      bbmax;
	int         szhitboxnameindex;
	int32_t     m_iPad01[3];
	float       m_flRadius;
	int32_t     m_iPad02[4];

	const char* GetName()
	{
		if (!szhitboxnameindex) return nullptr;
		return (const char*)((uint8_t*)this + szhitboxnameindex);
	}
};


struct mstudiohitboxset_t
{
	int    sznameindex;
	int    numhitboxes;
	int    hitboxindex;

	const char* GetName()
	{
		if (!sznameindex) return nullptr;
		return (const char*)((uint8_t*)this + sznameindex);
	}

	mstudiobbox_t* GetHitbox(int i)
	{
		if (i > numhitboxes) return nullptr;
		return (mstudiobbox_t*)((uint8_t*)this + hitboxindex) + i;
	}
};


struct mstudiobodyparts_t
{

	int					sznameindex;
	inline char* const pszName(void) const { return ((char*)this) + sznameindex; }
	int					nummodels;
	int					base;
	int					modelindex; // index into models array
	inline mstudiomodel_t* pModel(int i) const { return (mstudiomodel_t*)(((byte*)this) + modelindex) + i; };
};


class studiohdr_t
{
public:
	int					id;
	int					version;

	long				checksum;		// this has to be the same in the phy and vtx files to load!

	char				name[64];
	int					length;


	Vector				eyeposition;	// ideal eye position

	Vector				illumposition;	// illumination center

	Vector				hull_min;		// ideal movement hull size
	Vector				hull_max;

	Vector				view_bbmin;		// clipping bounding box
	Vector				view_bbmax;

	int					flags;

	int					numbones;			// bones
	int					boneindex;



	int					numbonecontrollers;		// bone controllers
	int					bonecontrollerindex;

	int					numhitboxsets;
	int					hitboxsetindex;


	int					numlocalanim;
	int					localanimindex;

	int					numlocalseq;
	int					localseqindex;
	int					activitylistversion;
	int					eventsindexed;
	int					numtextures;
	int					textureindex;
	int					numcdtextures;
	int					cdtextureindex;

	// replaceable textures tables
	int					numskinref;
	int					numskinfamilies;
	int					skinindex;

	int					numbodyparts;
	int					bodypartindex;
	// Look up hitbox set by index
	mstudiohitboxset_t* GetHitboxSet(int i) const
	{
		if (i > numhitboxsets) return nullptr;
		return (mstudiohitboxset_t*)(((byte*)this) + hitboxsetindex) + i;
	};

	mstudiobone_t* GetBone(int i) const {
		if (i > numbones) return nullptr;
		return (mstudiobone_t*)(((byte*)this) + boneindex) + i;
	};
	inline mstudiobodyparts_t* pBodypart(int i) const { return (mstudiobodyparts_t*)(((byte*)this) + bodypartindex) + i; };
};




#endif // !STUDIO_H
