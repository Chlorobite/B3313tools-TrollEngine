/********************************************************************************
	gex.h: Ultra 64 MARIO Brothers include file

	Copyright (c) 1995 Nintendo co., ltd.  All rights reserved

	Sept 25  1995
 ********************************************************************************/

#ifndef GEX_H
#define	GEX_H


/********************************************************************************/
/*	gsDPLoadTextureBlock() without gsDPSetTextureImage()						*/
/********************************************************************************/

#define	gsxDPLoadTextureImage(fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt)		\
									\
	gsDPSetTile(fmt, siz, 0, 0, G_TX_LOADTILE, 0 , cmt, maskt,	\
		shiftt, cms, masks, shifts),				\
	gsDPLoadSync(),							\
	gsDPLoadBlock(G_TX_LOADTILE, 0, 0, (width)*(height)-1,		\
				 	CALC_DXT(width, siz##_BYTES)),					\
	gsDPSetTile(fmt, siz, ((((width) * siz##_LINE_BYTES)+7)>>3), 0,	\
		G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks,	\
		shifts),						\
	gsDPSetTileSize(G_TX_RENDERTILE, 0, 0,				\
		((width)-1) << G_TEXTURE_IMAGE_FRAC,			\
		((height)-1) << G_TEXTURE_IMAGE_FRAC)


/********************************************************************************/
/*	Static display-list to load texture image made by Tanimoto.					*/
/********************************************************************************/

#define	g_Tani_LoadTextureImage(gfxptr, timg, fmt, siz, width, height,	tmem, ttdn)		\
																						\
	gDPSetTextureImage(gfxptr, fmt, siz, 1, timg);										\
	gDPTileSync(gfxptr);																\
	gDPSetTile(gfxptr, fmt, siz, 0, tmem, ttdn, 0,  0,0,0, 0,0,0);						\
	gDPLoadSync(gfxptr);																\
	gDPLoadBlock(gfxptr, ttdn, 0, 0, ((width)*(height)-1),								\
				  CALC_DXT(width, siz##_BYTES) )


#define	gs_Tani_LoadTextureImage(timg, fmt, siz, width, height,	tmem, ttdn)			\
																					\
	gsDPSetTextureImage(fmt, siz, 1, timg),											\
	gsDPTileSync(),																	\
	gsDPSetTile(fmt, siz, 0, tmem, ttdn, 0,  0,0,0, 0,0,0),							\
	gsDPLoadSync(),																	\
	gsDPLoadBlock(ttdn, 0, 0, ((width)*(height)-1),									\
				  CALC_DXT(width, siz##_BYTES) )

/********************************************************************************/
/*	Static display-list to load texture image Ver.2 made by Tanimoto.			*/
/********************************************************************************/

#define	g_Tani_LoadTextureImage2(gfxptr, timg, fmt, siz, width, height,	tmem, ttdn)		\
																						\
	gDPSetTextureImage(gfxptr, fmt, siz, 1, timg);										\
	gDPLoadSync(gfxptr);																\
	gDPLoadBlock(gfxptr, ttdn, 0, 0, ((width)*(height)-1),								\
				  CALC_DXT(width, siz##_BYTES) )


#define	gs_Tani_LoadTextureImage2(timg, fmt, siz, width, height, tmem, ttdn)		\
																					\
	gsDPSetTextureImage(fmt, siz, 1, timg),											\
	gsDPLoadSync(),																	\
	gsDPLoadBlock(ttdn, 0, 0, ((width)*(height)-1),									\
				  CALC_DXT(width, siz##_BYTES) )


/********************************************************************************/
/*	Static display-list to set up Texture Tile Descriptor made by Tanimoto.		*/
/********************************************************************************/

#define	g_Tani_SetUpTileDescrip(gfxptr, fmt, siz, width, height, tmem, ttdn, cms, masks, shifts, cmt, maskt, shiftt)				\
																					\
	gDPTileSync(gfxptr);															\
	gDPSetTile(gfxptr, fmt, siz, ((((width)*siz##_LINE_BYTES)+7)>>3), tmem,			\
				ttdn, 0, (cmt), (maskt), (shiftt), (cms), (masks), (shifts));		\
	gDPSetTileSize(gfxptr, ttdn, 0, 0,												\
					(((width) -1) << G_TEXTURE_IMAGE_FRAC),							\
					(((height)-1) << G_TEXTURE_IMAGE_FRAC) )


#define	gs_Tani_SetUpTileDescrip(fmt, siz, width, height, tmem, ttdn, cms, masks, shifts, cmt, maskt, shiftt)			\
																					\
	gsDPTileSync(),																	\
	gsDPSetTile(fmt, siz, ((((width)*siz##_LINE_BYTES)+7)>>3), tmem,				\
				ttdn, 0, (cmt), (maskt), (shiftt), (cms), (masks), (shifts)),		\
	gsDPSetTileSize(ttdn, 0, 0,														\
					(((width) -1) << G_TEXTURE_IMAGE_FRAC),							\
					(((height)-1) << G_TEXTURE_IMAGE_FRAC) )


#endif
