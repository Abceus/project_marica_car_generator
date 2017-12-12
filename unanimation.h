#ifndef UNANIMATION_H
#define UNANIMATION_H

#include <iostream>
#include <fstream>

using namespace std;

//The chunk header structure. Length: 32 bytes.
struct VChunkHeader
{
    char 	ChunkID[20]; 	//The chunk name.The Unreal Engine does not really care about this value.
    int 	TypeFlags; 		//The Unreal Engine does not really care about this value either, even though it supposedly specifies a file version identifier.
    int 	DataSize; 		//The size of each data record potentially following this header.
    int		DataCount;		//The number of data records following this header.
};

//An Euclidian vector denoting a position in 3D space, corresponding to the UnrealScript struct type Vector. Length: 12 bytes.

struct VPoint
{
    float X;
    float Y;
    float Z;

    VPoint operator+(VPoint op2);
    VPoint operator-(VPoint op2);
    VPoint operator+=(VPoint op2);
    friend ostream &operator<<(ostream &stream, VPoint o);
    VPoint(){}
    VPoint(float vX, float vY, float vZ)
    {
        X = vX;
        Y = vY;
        Z = vZ;
    }
};


//Associates skeletal mesh points with a UV coordinate of a material.Length: 12 bytes.


struct VVertex
{
    unsigned short int 	PointIndex; 	//Index of the point in the points list.
    float				U; 				//U coordinate in the material.
    float 				V; 				//V coordinate in the material.
    unsigned char		MatIndex; 		//Index of the material in the materials list. (seems unused, probably overridden by face's own MatIndex)
    unsigned char		Reserved; 		//Padding byte to get to a multiple of four bytes for the structure.
};



//Combined three vertices to a triangle face.Length: 12 bytes.
struct VTriangle
{
    unsigned short int 	WedgeIndex[3]; 		//Indices of the three vertices in the wedges list.
    unsigned char		MatIndex; 			//Index of the face's material in the materials list.
    unsigned char		AuxMatIndex; 		//Index of an additional material in the materials list. (seems unused)
    int					SmoothingGroups; 	//Bit field of smoothing groups this face belongs to. (Does the engine actually care about this for skeletal meshes ? )
};


//Specifies a surface material.Length: 88 bytes.
struct VMaterial
{
    char	MaterialName[64]; 		//Name of the material.The Unreal Engine will use this to find a texture or other material with this name in any package.
    int		TextureIndex; 			//Should be the same as this material's index in the materials list. (But it's probably a good idea to not rely on the value being correct.)
    int		PolyFlags; 				//Poly flags for all faces with this material. (seems unused)
    int 	AuxMaterial;			//? ? ? (seems unused)
    int		AuxFlags;				//? ? ? (seems unused)
    int		LodBias;				//? ? ? (seems unused)
    int		LodStyle;				//? ? ? (seems unused)
};


//A unit quaternion denoting an orientation or rotation in 3D space, corresponding to the UnrealScript struct type Quat.Length : 16 bytes.
struct VQuat
{
    float 	X;
    float	Y;
    float	Z;
    float	W;

    VQuat operator+(VQuat op2);
    VQuat operator-(VQuat op2);
    VQuat operator+=(VQuat op2);
    friend ostream &operator<<(ostream &stream, VQuat o);
    VQuat(){}
    VQuat(float vX, float vY, float vZ, float vW)
    {
        X = vX;
        Y = vY;
        Z = vZ;
        W = vW;
    }
};


//A helper struct for storing the bone position.Length : 44 bytes.
struct VJointPos
{
    VQuat 	Orientation; 	//Rotation of the bone, relative to its parent.
    VPoint 	Position; 		//Location of the bone, relative to its parent.
    float	Length;			//? ? ? (seems unused)
    float	XSize;			//? ? ? (seems unused)
    float	YSize;			//? ? ? (seems unused)
    float	ZSize;			//? ? ? (seems unused)

    VJointPos operator+(VJointPos op2);
    VJointPos operator-(VJointPos op2);
    VJointPos operator+=(VJointPos op2);
    VJointPos operator=(VJointPos op2);
    friend ostream &operator<<(ostream &stream, VJointPos o);
};


//Specifies a node in the skeletal bone tree for both the skeletal mesh and its animations.Length: 120 bytes.
struct VBone
{
    char		Name[64]; 		//Name of the bone.
    int			Flags;			//? ? ? (seems unused)
    int			NumChildren; 	//Number of child nodes in the skeletal bone tree.
    int			ParentIndex; 	//Index of this bone's parent in the bone list. The root bone must be the first in the bone list and have ParentIndex zero.
    VJointPos 	BonePos; 		//Bone position information.
};


//Specifies the influence of a bone's movement (through animation or otherwise) on a skeletal mesh point's position.Length: 12 bytes.
struct VRawBoneInfluence
{
    float	Weight; 		//How strongly the bone's movement affects the point's location.Values are in the range(0.0; 1.0], i.e.zero weights should not appear in the influences list, because they are meaningless.
    int		PointIndex; 	//Index of the affected point in the points list.
    int		BoneIndex; 		//Index of the affecting bone in the bones list.
};

#endif // UNANIMATION_H
