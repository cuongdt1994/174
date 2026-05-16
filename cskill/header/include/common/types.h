#ifndef __ONLINEGAME_COMMON_TYPE_H__
#define __ONLINEGAME_COMMON_TYPE_H__

#include <sys/types.h>
#include <algorithm>
#include <math.h>
#include <stdint.h>

#pragma pack(1)

struct A3DVECTOR
{
	float x;
	float y;
	float z;

public:
	A3DVECTOR(){}
	A3DVECTOR(float x,float y,float z):x(x),y(y),z(z){}
	A3DVECTOR & operator +=(const A3DVECTOR & rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	inline A3DVECTOR & operator *=(float scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}

	inline const A3DVECTOR & operator-=(const A3DVECTOR & rhs)
	{
	 	x -= rhs.x; 
	 	y -= rhs.y; 
	 	z -= rhs.z; 
		return *this;
	}

	/*
	inline float pseudo_distance(const A3DVECTOR & pos) const
	{
		float disx = fabs(x - pos.x);
		float disz = fabs(z - pos.z);
		float disy = fabs(y - pos.y);
		if(disx < disy)  disx = disy;
		if(disx < disz)  disx = disz;
		return disx;
	}
	*/
	inline float squared_distance(const A3DVECTOR & pos) const
	{
		float disx = x-pos.x;
		float disy = y-pos.y;
		float disz = z-pos.z;
		return disx*disx + disz*disz + disy*disy;
	}

	inline float horizontal_distance(const A3DVECTOR & pos) const
	{
		float disx = x-pos.x;
		float disz = z-pos.z;
		return disx*disx + disz*disz;
	}

	inline float dot_product(const A3DVECTOR & pos) const
	{
		return x*pos.x + y*pos.y + z*pos.z;
	}
	
	inline float squared_magnitude() const 
	{
		return x*x+y*y+z*z;
	}

	inline void normalize()
	{
		float mag = 1.f/sqrt(x*x+y*y+z*z);
		x *= mag;
		y *= mag;
		z *= mag;
	}
	
	friend struct rect;
};

inline bool check_speed( const A3DVECTOR & cur, const A3DVECTOR &next,float maxdis_square)
{
	A3DVECTOR offset(next);
	offset -= cur;
	float tmp = offset.squared_magnitude();
	return tmp <= maxdis_square;
}

inline unsigned char a3dvector_to_dir(const A3DVECTOR & cur)
{
	return ((unsigned int)(atan2(cur.z, cur.x)*(128./3.1415926535))) & 0xFF;
}

inline unsigned char a3dvector_to_dir(const float cur[3])
{
	return ((unsigned int)(atan2(cur[2],cur[0])*(128./3.1415926535))) & 0xFF;
}

#pragma pack()

struct rect
{
	float left;		// x min
	float top;		// z min
	float right;		// x max
	float bottom;		// z max
	inline rect(float l,float t,float r,float b):left(l),top(t),right(r),bottom(b) {} 
	inline rect(const A3DVECTOR &pos1, const A3DVECTOR &pos2)
	{
		if(pos1.x < pos2.x)
		{
			left = pos1.x;
			right = pos2.x;
		}
		else
		{
			left = pos2.x;
			right = pos1.x;
		}

		if(pos1.z < pos2.z)
		{
			top = pos1.z;
			bottom = pos2.z;
		}
		else
		{
			top = pos2.z;
			bottom = pos1.z;
		}
	}
	inline rect(){}
	inline bool IsIn(float x, float z) const { return x>=left && x < right && z >= top && z <bottom; } 
	inline bool IsIn(const rect &rt) const { return rt.left >=left && rt.right <=right && rt.top >=top && rt.bottom <= bottom; }
	inline bool IsOut(float x, float z) const { return x<left || x >= right || z < top || z >= bottom; }
	inline float Width() const { return right - left;}
	inline float Height() const { return bottom - top;}
	inline bool IsOverlap( const rect & rt) const 
	{
		if(rt.left >= right || rt.top >= bottom || rt.bottom <= top || rt.right <=left) return false;
		return true;
	}
	inline void Union(const rect & rt)
	{
		left = std::min(left,rt.left);
		top = std::min(top,rt.top);
		right = std::max(right,rt.right);
		bottom = std::max(bottom,rt.bottom);
	}
	inline void Cut(const rect & rt)
	{
		if(!IsOverlap(rt)) return;
		if(left < rt.left) right = rt.left;
		if(right > rt.right) left = rt.right;
		if(top < rt.top) bottom = rt.top;
		if(bottom > rt.bottom) top = rt.bottom;
		if(left == rt.left && right == rt.right && top == rt.top && bottom == rt.bottom
		 || IsIn(rt) || rt.IsIn(*this)) 
		{
			left = right;
			top = bottom;
		}
	}

	inline bool GetIntersection(const rect & rt)
	{
		if(rt.IsIn(*this)) return true;
		if(!IsOverlap(rt)) 
		{
			left = right = top = bottom = 0; 
			return false;
		}
		left = std::max(left,rt.left);
		top = std::max(top,rt.top);
		right = std::min(right,rt.right);
		bottom = std::min(bottom,rt.bottom);
		return true;
	}
	inline float GetArea() const
	{
		return (right - left) * (bottom - top);
	}
};

/**
 *	Types of XIDs
 */
enum
{
	GM_TYPE_INVALID = -1,
	GM_TYPE_SERVER,				//id is the server
	GM_TYPE_BROADCAST,			//id is useless, this is a broadcast message, and the receiver is the received server object
	GM_TYPE_PLAYER,				//id is the user id
	GM_TYPE_TEAM,				//id is the team target
	GM_TYPE_NPC,				//id is an npc
	GM_TYPE_ACTIVE,				//id Useless, representing active objects, namely NPC and player
	GM_TYPE_MATTER,				//id is an item
	GM_TYPE_PET,				//id It is a pet or an attached NPC, and its ID is calculated from the player's account ID
	GM_TYPE_OBJECT,				//id is a special object, such as a transaction object
	GM_TYPE_SELF,				//id It is yourself, you can ignore the following ID
	GM_TYPE_MINE_SPAWNER,			//id is the spawner in the MATTER generator
	GM_TYPE_MAFIA,				//id is the gang target
};
#define 	PET_MASK 	0x20000000	//bit 29 indicates whether it is a pet

struct XID
{
	int type;
	int id;
	XID(){}
	XID(int __type, int __id):type(__type),id(__id){}
	inline bool operator==(const XID & rhs) const
	{
		return type == rhs.type && id == rhs.id;
	}

	inline bool operator!=(const XID & rhs) const
	{
		return id != rhs.id || type != rhs.type;
	}

	inline bool operator<(const XID & rhs) const
	{
		return id < rhs.id;
	}

	inline bool IsActive() const
	{
		return type == GM_TYPE_NPC || type == GM_TYPE_PLAYER;
	}

	inline bool IsPlayerClass() const
	{
		return type == GM_TYPE_PLAYER;
	}

	inline bool IsPlayer() const
	{
		return type == GM_TYPE_PLAYER;
	}

	inline bool IsPet() const
	{
		return type == GM_TYPE_NPC && (id & PET_MASK);
	}
	
	inline bool IsValid() const
	{
		return type != -1;
	}

	inline bool IsErrorType() const
	{
		return !(type >= -1 && type <= GM_TYPE_MAFIA);
	}
	
};

struct XID_HashFunc
{
	unsigned long operator()(const XID & id) const
	{
		return id.type * 5 + id.id*7;
	}
};


	template <typename WRAPPER>
WRAPPER & operator<<(WRAPPER & wrapper,const XID &id)
{
	return wrapper<<id.type<<id.id;
}

	template <typename WRAPPER>
WRAPPER & operator>>(WRAPPER & wrapper,XID & id)
{
	return wrapper>>id.type>>id.id;
}

/*
 *	NPC and item ID conversion function
 */
inline int ID2WIDX(int id) { return (int) ( ( ((unsigned int)id) & (0x0FF00000)) >> 20); }
inline int ID2IDX(int id) { return id & 0x000FFFFF; }
inline int MKOBJID(int worldid,int idx)
{
	//ASSERT(worldid <= 0x7f && worldid >=0);
	int tmp;
	tmp = ((worldid & 0xFF)<<20) | ( idx & 0x000FFFFF);
	return tmp;
}

inline void MAKE_ID(XID & id, int u_id)
{
	id.id = u_id;
	switch(u_id & 0xC0000000)
	{
	case 0:
	case 0x40000000:
		id.type = GM_TYPE_PLAYER;
	break;
	case 0x80000000:
		id.type = GM_TYPE_NPC;
	break;
	case 0xC0000000:
		id.type = GM_TYPE_MATTER;
	break;
	}
}

inline bool IS_HUMANSIDE(const XID & id) { return id.type == GM_TYPE_PLAYER;}

class gnpc;
class gmatter;
class gplayer;
template<typename T> inline int MERGE_ID(int oldid);
template<> inline int MERGE_ID<gnpc>(int oldid) { return oldid | 0x80000000;}
template<> inline int MERGE_ID<gmatter>(int oldid) { return oldid | 0xC0000000;}
template<> inline int MERGE_ID<gplayer>(int oldid) { return oldid;}
inline int MERGE_PET_ID(int id){return (MERGE_ID<gnpc>(id)) | PET_MASK;}


/*
 *	Represents the structure of a session on the link server
 */
struct link_sid
{
	int cs_id;
	int cs_sid;
	int user_id;
};

template <typename WRAPPER>
WRAPPER & operator <<(WRAPPER & wrapper, const link_sid & rhs)
{
	wrapper.push_back(&rhs,sizeof(rhs));
	return wrapper;
}

template <typename WRAPPER>
WRAPPER & operator >>(WRAPPER & wrapper, link_sid & rhs)
{
	wrapper.pop_back(&rhs,sizeof(rhs));
	return wrapper;
}

/*
   Some common data structures required by the protocol
 */
#pragma pack(1)
namespace C2S
{
	struct cmd_header
	{
		unsigned short cmd;
	};
}

namespace S2C
{
	struct multi_data_header
	{
		unsigned short cmd;
		unsigned short count;
	};

	struct single_data_header
	{
		unsigned short cmd;
	};
}
#pragma pack()

#endif

