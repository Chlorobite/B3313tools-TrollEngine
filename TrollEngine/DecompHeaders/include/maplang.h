/********************************************************************************
	maplang.h: Ultra 64 MARIO Brothers include file

	Copyright (c) 1995 Nintendo co., ltd.  All rights reserved

	October 24, 1995
 ********************************************************************************/

#ifndef MAPLANG_H
#define	MAPLANG_H

#define	MAP_SYSTEM_STORAGES		2
#define	MAP_STORAGE_CAMERA		0
#define	MAP_STORAGE_LIGHT		1


#ifndef RGBA16
#define	RGBA16(r,g,b,a)		(((r)<<11) | ((g)<<6) | ((b)<<1) | (a))
#endif

/********************************************************************************/
/*	Map language command ID.													*/
/********************************************************************************/

#define	_hmsExec_id			 0
#define	_hmsExit_id			 1
#define	_hmsJump_id			 2
#define	_hmsReturn_id		 3

#define	_hmsBegin_id		 4
#define	_hmsEnd_id			 5
#define	_hmsStorage_id		 6
#define	_hmsPutFlag_id		 7

#define	_hmsScene_id		 8
#define	_hmsOrtho_id		 9
#define	_hmsPersp_id		10
#define	_hmsGroup_id		11
#define	_hmsLayer_id		12
#define	_hmsLod_id			13
#define	_hmsSelect_id		14
#define	_hmsCamera_id		15
#define	_hmsCoord_id		16
#define	_hmsTrans_id		17
#define	_hmsRotate_id		18
#define	_hmsJoint_id		19
#define	_hmsBboard_id		20

#define	_hmsGfx_id			21
#define	_hmsShadow_id		22
#define	_hmsObject_id		23
#define	_hmsCProg_id		24
#define	_hmsClear_id		25
#define	_hmsPrint_id		26
#define	_hmsBranch_id		27
#define	_hmsSucker_id		28

#define	_hmsScale_id		29
#define	_hmsMatrix_id		30
#define	_hmsArea_id			31
#define	_hmsHeader_id		32


/********************************************************************************/
/*	Map language basic macro.													*/
/********************************************************************************/

#define	_msCmd(cmd,var,dat)						((ulong)(cmd)<<24 | (ulong)(var)<<16 | ((ulong)(dat)&0xffff))
#define	_msPac(da1,da2)							(((ulong)(da1)&0xffff)<<16 | ((ulong)(da2)&0xffff))
#define	_msAdr(adr)								((ulong)(adr))
#define	_msU32(dat)								((ulong)(dat*65536.0f))

/********************************************************************************/
/*	Map hierarcky command macro.												*/
/********************************************************************************/

#define	hmsExec(addr)							_msCmd(_hmsExec_id	,0,0),_msAdr(addr),
#define	hmsExit()								_msCmd(_hmsExit_id	,0,0),
#define	hmsJump(addr)							_msCmd(_hmsJump_id	,0,0),_msAdr(addr),
#define	hmsCall(addr)							_msCmd(_hmsJump_id	,1,0),_msAdr(addr),
#define	hmsReturn()								_msCmd(_hmsReturn_id	,0,0),

#define	hmsBegin()								_msCmd(_hmsBegin_id	,0,0),
#define	hmsEnd()								_msCmd(_hmsEnd_id	,0,0),
#define	hmsStorage(n)							_msCmd(_hmsStorage_id,0,n),



/* Scene node	*/

#define	hmsScene(centh, centv, ofsh, ofsv, nstrages)  \
			_msCmd(_hmsScene_id, 0, nstrages), \
			_msPac(centh, centv), \
			_msPac(ofsh , ofsv ),


#define	hmsOrtho(scale)  \
			_msCmd(_hmsOrtho_id, 0, scale), \

#define	hmsPersp(angle, near, far)  \
			_msCmd(_hmsPersp_id, 0, angle), \
			_msPac(near, far),

#define	hmsPerspective(angle, near, far, proc)  \
			_msCmd(_hmsPersp_id, 1, angle), \
			_msPac(near, far), \
 			_msAdr(proc),


/* Group node	*/

#define	hmsGroup()  \
			_msCmd(_hmsGroup_id, 0, 0),



/* Group node	*/

#define	hmsArea(x1, y1, z1, x2, y2, z2)  \
			_msCmd(_hmsArea_id, 0, 0), \
			_msPac(x1, y1), \
			_msPac(z1, x2), \
			_msPac(y2, z2),


/* Layer node	*/

#define	hmsLayer(zbuf)  \
			_msCmd(_hmsLayer_id, zbuf, 0),


/* LOD node		*/

#define	hmsLOD(near, far)  \
			_msCmd(_hmsLod_id, 0, 0), \
			_msPac(near, far),


/* Select node		*/

#define	hmsSelect(code, proc)  \
			_msCmd(_hmsSelect_id, 0, code), \
			_msAdr(proc),

/* Camera node	*/

#define	hmsCamera(mode, posx, posy, posz, lookx, looky, lookz, proc)  \
			_msCmd(_hmsCamera_id, 0, mode), \
			_msPac(posx , posy ), \
			_msPac(posz , lookx), \
			_msPac(looky, lookz), \
			_msAdr(proc),

/* Coord node	*/

#define	hmsCoord(px,py,pz,ax,ay,az)  \
			_msCmd(_hmsCoord_id, 0x00, 0), \
			_msPac(px,py), \
			_msPac(pz,ax), \
			_msPac(ay,az),

#define	hmsCoordT(px,py,pz)  \
			_msCmd(_hmsCoord_id, 0x10,px), \
			_msPac(py,pz),

#define	hmsCoordR(ax,ay,az)  \
			_msCmd(_hmsCoord_id, 0x20,ax), \
			_msPac(ay,az),

#define	hmsCoordY(ay)  \
			_msCmd(_hmsCoord_id, 0x30,ay),

#define	hmsGfxCoord(mode,gfx,px,py,pz,ax,ay,az)  \
			_msCmd(_hmsCoord_id, 0x80+mode, 0), \
			_msPac(px,py), \
			_msPac(pz,ax), \
			_msPac(ay,az), \
			_msAdr(gfx),

#define	hmsGfxCoordT(mode,gfx,px,py,pz)  \
			_msCmd(_hmsCoord_id, 0x90+mode,px), \
			_msPac(py,pz), \
			_msAdr(gfx),

#define	hmsGfxCoordR(mode,gfx,ax,ay,az)  \
			_msCmd(_hmsCoord_id, 0xA0+mode,ax), \
			_msPac(ay,az), \
			_msAdr(gfx),

#define	hmsGfxCoordY(mode,gfx,ay)  \
			_msCmd(_hmsCoord_id, 0xB0+mode,ay), \
			_msAdr(gfx),


/* Trans node	*/

#define	hmsTrans(px,py,pz)  \
			_msCmd(_hmsTrans_id, 0x00, px), \
			_msPac(py,pz),

#define	hmsGfxTrans(mode,gfx,px,py,pz)  \
			_msCmd(_hmsTrans_id, 0x80+mode, px), \
			_msPac(py,pz), \
			_msAdr(gfx),


/* Rotate node	*/

#define	hmsRotate(ax,ay,az)  \
			_msCmd(_hmsRotate_id, 0x00, ax), \
			_msPac(ay,az),

#define	hmsGfxRotate(mode,gfx,ax,ay,az)  \
			_msCmd(_hmsRotate_id, 0x80+mode, ax), \
			_msPac(ay,az), \
			_msAdr(gfx),



/* Scale node	*/

#define	hmsScale(scaling)  \
			_msCmd(_hmsScale_id, 0x00, 0), \
			_msU32(scaling),

#define	hmsGfxScale(mode,gfx,scaling)  \
			_msCmd(_hmsScale_id, 0x80+mode, 0), \
			_msU32(scaling), \
			_msAdr(gfx),



/* Matrix node	*/

#define	hmsMatrix(matrix)  \
			_msCmd(_hmsMatrix_id, 0x00, 0), \
			_msAdr(matrix),

#define	hmsGfxMatrix(mode,gfx,matrix)  \
			_msCmd(_hmsMatrix_id, 0x80+mode, 0), \
			_msAdr(matrix), \
			_msAdr(gfx),



/* Joint node	*/

#define	hmsJoint(mode,gfx,px,py,pz)  \
			_msCmd(_hmsJoint_id, mode, px), \
			_msPac(py,pz), \
			_msAdr(gfx),


/* Bboard node	*/

#define	hmsBboard(px,py,pz)  \
			_msCmd(_hmsBboard_id, 0x00, px), \
			_msPac(py,pz),

#define	hmsGfxBboard(mode,gfx,px,py,pz)  \
			_msCmd(_hmsBboard_id, 0x80+mode, px), \
			_msPac(py,pz), \
			_msAdr(gfx),


/* Gfx node		*/

#define	hmsGfx(mode,gfx)  \
			_msCmd(_hmsGfx_id, mode, 0), \
			_msAdr(gfx),



/* Shape header node	*/

#define	hmsHeader(radius)  \
			_msCmd(_hmsHeader_id, 0, radius),


/* Shadow node	*/

#define	hmsShadowOn(size)  \
			_msCmd(_hmsShadow_id, 0, 1), \
			_msPac(128, size),


#define	hmsShadow(size, level, flag)  \
			_msCmd(_hmsShadow_id, 0, flag), \
			_msPac(level, size),

/* Object node	*/

#define	hmsObject()  \
			_msCmd(_hmsObject_id, 0, 0),


/* CProg node	*/

#define	hmsCProg(code, proc)  \
			_msCmd(_hmsCProg_id, 0, code), \
			_msAdr(proc),

/* Clear node	*/

#define	hmsClear(color, proc)  \
			_msCmd(_hmsClear_id, 0, color), \
			_msAdr(proc),


/* Print node	*/

#define	hmsPrint(string, width, height, length)  \
			_msCmd(_hmsPrint_id, length, (width)*256+(height)), \
			_msAdr(string),


/* sucker node	*/

#define	hmsSucker(posx, posy, posz, code, proc)  \
			_msCmd(_hmsSucker_id, code, posx), \
			_msPac(posy,posz), \
			_msAdr(proc),



#define	hmsPutFlags(flag)						_msCmd(_hmsPutFlag_id, 0, flag),
#define	hmsSetFlags(flag)						_msCmd(_hmsPutFlag_id, 1, flag),
#define	hmsClrFlags(flag)						_msCmd(_hmsPutFlag_id, 2, flag),


#define	hmsBranch(storage)  \
			_msCmd(_hmsBranch_id, 0, storage),


/********************************************************************************/
/*	Map Language command macro.													*/
/********************************************************************************/

typedef unsigned long	Hierarchy, *HierarchyPtr;

#endif
