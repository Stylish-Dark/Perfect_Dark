#ifndef PORT_VISUAL_RESTRAINT_H
#define PORT_VISUAL_RESTRAINT_H

#include <PR/ultratypes.h>

enum visualrestraintstageprofile {
	VISUAL_RESTRAINT_STAGE_NONE,
	VISUAL_RESTRAINT_STAGE_GROUNDED,
	VISUAL_RESTRAINT_STAGE_DATADYNE,
	VISUAL_RESTRAINT_STAGE_G5,
	VISUAL_RESTRAINT_STAGE_MARITIME,
	VISUAL_RESTRAINT_STAGE_AREA51,
	VISUAL_RESTRAINT_STAGE_AVIATION,
};

enum visualrestraintstageprofile visualRestraintGetStageProfile(s32 stage);
void visualRestraintApplyEnvironmentPixel(enum visualrestraintstageprofile profile, u8 *r, u8 *g, u8 *b);
void visualRestraintApplyAtmospherePixel(s32 stage, u8 *r, u8 *g, u8 *b);
void visualRestraintApplyAtmosphereFloat(s32 stage, f32 *r, f32 *g, f32 *b);

bool visualRestraintIsCharacterFile(s32 fileNum);
void visualRestraintApplyCharacterPixel(s32 fileNum, u8 *r, u8 *g, u8 *b);
void visualRestraintRegisterCharacterCiTexture(const u8 *addr, u32 size, s32 fileNum);
void visualRestraintResetCharacterCiRegistry(void);
s32 visualRestraintFindCharacterCiTextureFile(const u8 *addr, u32 size);

#endif
