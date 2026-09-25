#include "constants.h"
#include "visualrestraint.h"

static void blendTowardLuma(s32 *r, s32 *g, s32 *b, s32 blend)
{
	const s32 luma = (54 * *r + 183 * *g + 19 * *b) >> 8;

	*r = (*r * (256 - blend) + luma * blend) >> 8;
	*g = (*g * (256 - blend) + luma * blend) >> 8;
	*b = (*b * (256 - blend) + luma * blend) >> 8;
}

enum visualrestraintstageprofile visualRestraintGetStageProfile(s32 stage)
{
	switch (stage) {
	case STAGE_DEFECTION:
	case STAGE_INVESTIGATION:
	case STAGE_EXTRACTION:
	case STAGE_MBR:
		return VISUAL_RESTRAINT_STAGE_DATADYNE;

	case STAGE_G5BUILDING:
	case STAGE_MP_G5BUILDING:
		return VISUAL_RESTRAINT_STAGE_G5;

	case STAGE_PELAGIC:
	case STAGE_DEEPSEA:
		return VISUAL_RESTRAINT_STAGE_MARITIME;

	case STAGE_INFILTRATION:
	case STAGE_RESCUE:
	case STAGE_ESCAPE:
	case STAGE_MAIANSOS:
	case STAGE_MP_AREA52:
		return VISUAL_RESTRAINT_STAGE_AREA51;

	case STAGE_AIRBASE:
	case STAGE_AIRFORCEONE:
		return VISUAL_RESTRAINT_STAGE_AVIATION;

	case STAGE_VILLA:
	case STAGE_MP_VILLA:
	case STAGE_CHICAGO:
	case STAGE_CRASHSITE:
	case STAGE_RETAKING:
	case STAGE_DEFENSE:
	case STAGE_CITRAINING:
	case STAGE_DUEL:
	case STAGE_MP_CARPARK:
	case STAGE_MP_WAREHOUSE:
	case STAGE_MP_RAVINE:
	case STAGE_MP_PIPES:
	case STAGE_MP_TEMPLE:
	case STAGE_MP_COMPLEX:
	case STAGE_MP_BASE:
	case STAGE_MP_RUINS:
	case STAGE_MP_SEWERS:
	case STAGE_MP_FELICITY:
	case STAGE_MP_FORTRESS:
	case STAGE_MP_GRID:
		return VISUAL_RESTRAINT_STAGE_GROUNDED;
	}

	return VISUAL_RESTRAINT_STAGE_NONE;
}

void visualRestraintApplyEnvironmentPixel(enum visualrestraintstageprofile profile, u8 *rptr, u8 *gptr, u8 *bptr)
{
	s32 r = *rptr;
	s32 g = *gptr;
	s32 b = *bptr;
	s32 max = r > g ? (r > b ? r : b) : (g > b ? g : b);
	s32 min = r < g ? (r < b ? r : b) : (g < b ? g : b);
	s32 chroma = max - min;
	const s32 luma = (54 * r + 183 * g + 19 * b) >> 8;

	if (profile == VISUAL_RESTRAINT_STAGE_NONE) {
		return;
	}

	// Broad material restraint. Preserve deep shadow and bright displays/lights.
	if (max < 228 && luma > 24 && chroma > 34) {
		blendTowardLuma(&r, &g, &b, 24);
	}

	if (profile == VISUAL_RESTRAINT_STAGE_DATADYNE) {
		if (max < 232 && r > g + 12 && b > g + 16 && r > 48 && b > 48) {
			blendTowardLuma(&r, &g, &b, 78);
			r = r * 92 / 100;
			g = g * 92 / 100;
			b = b * 94 / 100;
		}
	} else if (profile == VISUAL_RESTRAINT_STAGE_G5) {
		if (max < 232 && b > r + 18 && g > r + 8) {
			blendTowardLuma(&r, &g, &b, 64);
			r = r * 92 / 100;
			g = g * 92 / 100;
			b = b * 88 / 100;
		}
	} else if (profile == VISUAL_RESTRAINT_STAGE_MARITIME) {
		if (max < 232 && r > g + 28 && r > b + 24) {
			blendTowardLuma(&r, &g, &b, 52);
			r = r * 90 / 100;
		} else if (max < 232 && b > r + 16 && g > r + 10) {
			blendTowardLuma(&r, &g, &b, 48);
			b = b * 92 / 100;
		}
	} else if (profile == VISUAL_RESTRAINT_STAGE_AREA51) {
		if (max < 232 && b > r + 18 && g > r + 6) {
			blendTowardLuma(&r, &g, &b, 48);
			b = b * 92 / 100;
		} else if (max < 232 && g > r + 18 && g > b + 8) {
			r = (15 * r + g) / 16;
			g = g * 91 / 100;
			b = b * 94 / 100;
		}
	} else if (profile == VISUAL_RESTRAINT_STAGE_AVIATION) {
		if (max < 232 && b > r + 20 && b > g + 8) {
			blendTowardLuma(&r, &g, &b, 44);
			b = b * 90 / 100;
		} else if (max < 232 && r > g + 26 && r > b + 22) {
			blendTowardLuma(&r, &g, &b, 40);
			r = r * 91 / 100;
		}
	}

	*rptr = (u8)r;
	*gptr = (u8)g;
	*bptr = (u8)b;
}

void visualRestraintApplyAtmospherePixel(s32 stage, u8 *rptr, u8 *gptr, u8 *bptr)
{
	if (stage >= 900) {
		stage -= 900;
	}

	const enum visualrestraintstageprofile profile = visualRestraintGetStageProfile(stage);
	s32 r = *rptr;
	s32 g = *gptr;
	s32 b = *bptr;
	s32 max = r > g ? (r > b ? r : b) : (g > b ? g : b);
	s32 min = r < g ? (r < b ? r : b) : (g < b ? g : b);
	s32 chroma = max - min;

	if (profile == VISUAL_RESTRAINT_STAGE_NONE || chroma < 28) {
		return;
	}

	// Atmosphere may carry more colour than physical materials.
	blendTowardLuma(&r, &g, &b, 22);

	if (stage == STAGE_CRASHSITE) {
		// Preserve the hot crash-site mood, but make the red/yellow atmosphere dusty.
		blendTowardLuma(&r, &g, &b, max > 220 ? 76 : 46);
		r = r * 96 / 100;
		g = g * 96 / 100;
		b = b * 94 / 100;
	} else if (stage == STAGE_VILLA || stage == STAGE_MP_VILLA) {
		// Keep Mediterranean blue/orange, remove the postcard-like cyan punch.
		blendTowardLuma(&r, &g, &b, 42);
	} else if (stage == STAGE_DEFENSE || stage == STAGE_CITRAINING
			|| stage == STAGE_DUEL || stage == STAGE_RETAKING) {
		// Carrington Institute daylight becomes paler and less cyan.
		blendTowardLuma(&r, &g, &b, 38);
	} else if (profile == VISUAL_RESTRAINT_STAGE_G5
			|| profile == VISUAL_RESTRAINT_STAGE_AREA51
			|| profile == VISUAL_RESTRAINT_STAGE_AVIATION
			|| profile == VISUAL_RESTRAINT_STAGE_MARITIME
			|| profile == VISUAL_RESTRAINT_STAGE_DATADYNE) {
		blendTowardLuma(&r, &g, &b, 18);
	}

	*rptr = (u8)r;
	*gptr = (u8)g;
	*bptr = (u8)b;
}

void visualRestraintApplyAtmosphereFloat(s32 stage, f32 *rptr, f32 *gptr, f32 *bptr)
{
	u8 r = (u8)(*rptr < 0.0f ? 0 : (*rptr > 255.0f ? 255 : (s32)(*rptr + 0.5f)));
	u8 g = (u8)(*gptr < 0.0f ? 0 : (*gptr > 255.0f ? 255 : (s32)(*gptr + 0.5f)));
	u8 b = (u8)(*bptr < 0.0f ? 0 : (*bptr > 255.0f ? 255 : (s32)(*bptr + 0.5f)));

	visualRestraintApplyAtmospherePixel(stage, &r, &g, &b);

	*rptr = (f32)r;
	*gptr = (f32)g;
	*bptr = (f32)b;
}

#define VISUAL_RESTRAINT_CI_REGISTRY_SIZE 512

struct visualrestraintcitexture {
	const u8 *addr;
	u32 size;
	u32 fingerprint;
	s32 fileNum;
};

static struct visualrestraintcitexture g_VisualRestraintCiTextures[VISUAL_RESTRAINT_CI_REGISTRY_SIZE];
static s32 g_VisualRestraintCiTextureCount = 0;
static s32 g_VisualRestraintCiTextureNext = 0;

static u32 visualRestraintTextureFingerprint(const u8 *addr, u32 size)
{
	u32 hash = 2166136261u;
	const u32 count = size < 16 ? size : 16;

	for (u32 i = 0; i < count; i++) {
		hash ^= addr[i];
		hash *= 16777619u;
	}

	hash ^= size;
	hash *= 16777619u;
	return hash;
}

static bool visualRestraintIsDataDyneCharacterFile(s32 fileNum)
{
	switch (fileNum) {
	case FILE_CDDSHOCK:
	case FILE_CFEM_GUARD:
	case FILE_CDD_SECGUARD:
	case FILE_CDD_LABTECH:
	case FILE_CDD_GUARD:
	case FILE_CDD_SHOCK:
	case FILE_CDD_SHOCK_INF:
	case FILE_CDDSNIPER:
		return true;
	}

	return false;
}

static bool visualRestraintIsMilitarySecurityCharacterFile(s32 fileNum)
{
	switch (fileNum) {
	case FILE_CA51GUARD:
	case FILE_CAREA51GUARD:
	case FILE_CA51TROOPER:
	case FILE_CA51AIRMAN:
	case FILE_CNSA_LACKEY:
	case FILE_CPRES_SECURITY:
	case FILE_CALASKAN_GUARD:
	case FILE_CCISOLDIER:
		return true;
	}

	return false;
}

static bool visualRestraintIsFlightCrewCharacterFile(s32 fileNum)
{
	switch (fileNum) {
	case FILE_CSTEWARD:
	case FILE_CSTEWARDESS:
	case FILE_CSTEWARDESS_COAT:
	case FILE_CPILOTAF1:
		return true;
	}

	return false;
}

static bool visualRestraintIsTechnicalCharacterFile(s32 fileNum)
{
	switch (fileNum) {
	case FILE_COVERALL:
	case FILE_CLABTECH:
	case FILE_CBIOTECH:
	case FILE_CFEMLABTECH:
	case FILE_CCILABTECH:
	case FILE_CCIFEMTECH:
		return true;
	}

	return false;
}

static bool visualRestraintIsUrbanSecurityCharacterFile(s32 fileNum)
{
	switch (fileNum) {
	case FILE_CCIAGUY:
	case FILE_CFBIGUY:
	case FILE_CCHICROB:
		return true;
	}

	return false;
}

static bool visualRestraintIsCivilianSupportCharacterFile(s32 fileNum)
{
	switch (fileNum) {
	case FILE_COFFICEWORKER:
	case FILE_COFFICEWORKER2:
	case FILE_CSECRETARY:
	case FILE_CSTRIPES:
	case FILE_CNEGOTIATOR:
		return true;
	}

	return false;
}

bool visualRestraintIsCharacterFile(s32 fileNum)
{
	return fileNum == FILE_CG5_GUARD
		|| fileNum == FILE_CG5_SWAT_GUARD
		|| fileNum == FILE_CPELAGIC_GUARD
		|| visualRestraintIsDataDyneCharacterFile(fileNum)
		|| visualRestraintIsMilitarySecurityCharacterFile(fileNum)
		|| visualRestraintIsFlightCrewCharacterFile(fileNum)
		|| visualRestraintIsTechnicalCharacterFile(fileNum)
		|| visualRestraintIsUrbanSecurityCharacterFile(fileNum)
		|| visualRestraintIsCivilianSupportCharacterFile(fileNum);
}

void visualRestraintApplyCharacterPixel(s32 fileNum, u8 *rptr, u8 *gptr, u8 *bptr)
{
	s32 r = *rptr;
	s32 g = *gptr;
	s32 b = *bptr;
	s32 max = r > g ? (r > b ? r : b) : (g > b ? g : b);
	s32 min = r < g ? (r < b ? r : b) : (g < b ? g : b);
	s32 chroma = max - min;
	s32 luma = (54 * r + 183 * g + 19 * b) >> 8;

	if (!visualRestraintIsCharacterFile(fileNum)) {
		return;
	}

	if (chroma > 24) {
		s32 blend = 22 + (chroma - 24) / 3;

		if (blend > 72) {
			blend = 72;
		}

		r = (r * (256 - blend) + luma * blend) >> 8;
		g = (g * (256 - blend) + luma * blend) >> 8;
		b = (b * (256 - blend) + luma * blend) >> 8;
	}

	if (fileNum == FILE_CG5_GUARD || fileNum == FILE_CG5_SWAT_GUARD) {
		if (b > r + 20 && g > r + 8) {
			luma = (54 * r + 183 * g + 19 * b) >> 8;
			r = (3 * r + luma) / 4;
			g = (3 * g + luma) / 4;
			b = (3 * b + luma) / 4;
			r = r * 84 / 100;
			g = g * 84 / 100;
			b = b * 84 / 100;
		}
	} else if (fileNum == FILE_CPELAGIC_GUARD) {
		if (r > g + 28 && r > b + 28) {
			r = r * 82 / 100;
			g = (g * 94 + r * 6) / 100;
			b = (b * 92 + r * 8) / 100;
		}

		max = r > g ? (r > b ? r : b) : (g > b ? g : b);
		min = r < g ? (r < b ? r : b) : (g < b ? g : b);

		if (min > 185 && max - min < 40) {
			r = r * 92 / 100;
			g = g * 90 / 100;
			b = b * 86 / 100;
		}
	} else if (visualRestraintIsDataDyneCharacterFile(fileNum)) {
		if (r > g + 10 && b > g + 14 && r > 55 && b > 55) {
			luma = (54 * r + 183 * g + 19 * b) >> 8;
			r = (3 * r + luma) / 4;
			g = (2 * g + luma) / 3;
			b = (3 * b + luma) / 4;
			r = r * 90 / 100;
			g = g * 90 / 100;
			b = b * 92 / 100;
		}
	} else if (visualRestraintIsMilitarySecurityCharacterFile(fileNum)) {
		if (b > r + 18 && b > g + 4) {
			luma = (54 * r + 183 * g + 19 * b) >> 8;
			r = (3 * r + luma) / 4;
			g = (3 * g + luma) / 4;
			b = (2 * b + luma) / 3;
			r = r * 90 / 100;
			g = g * 90 / 100;
			b = b * 86 / 100;
		} else if (g > r + 18 && g > b + 8) {
			r = (9 * r + g) / 10;
			g = g * 86 / 100;
			b = b * 88 / 100;
		}
	} else if (visualRestraintIsFlightCrewCharacterFile(fileNum)) {
		if (b > r + 20 && b > g + 8) {
			r = r * 90 / 100;
			g = g * 90 / 100;
			b = b * 82 / 100;
		} else if (r > g + 24 && r > b + 20) {
			r = r * 84 / 100;
			g = (19 * g + r) / 20;
			b = (18 * b + 2 * r) / 20;
		}
	} else if (visualRestraintIsTechnicalCharacterFile(fileNum)) {
		if (g > r + 12 && b > r + 14) {
			luma = (54 * r + 183 * g + 19 * b) >> 8;
			r = (3 * r + luma) / 4;
			g = (3 * g + luma) / 4;
			b = (3 * b + luma) / 4;
		}

		max = r > g ? (r > b ? r : b) : (g > b ? g : b);
		min = r < g ? (r < b ? r : b) : (g < b ? g : b);

		if (min > 205 && max - min < 28) {
			r = r * 96 / 100;
			g = g * 95 / 100;
			b = b * 92 / 100;
		}
	}

	*rptr = (u8)r;
	*gptr = (u8)g;
	*bptr = (u8)b;
}

void visualRestraintResetCharacterCiRegistry(void)
{
	g_VisualRestraintCiTextureCount = 0;
	g_VisualRestraintCiTextureNext = 0;
}

void visualRestraintRegisterCharacterCiTexture(const u8 *addr, u32 size, s32 fileNum)
{
	if (!addr || !size) {
		return;
	}

	const bool target = visualRestraintIsCharacterFile(fileNum);
	const u32 fingerprint = visualRestraintTextureFingerprint(addr, size);

	for (s32 i = 0; i < g_VisualRestraintCiTextureCount; i++) {
		if (g_VisualRestraintCiTextures[i].addr == addr) {
			g_VisualRestraintCiTextures[i].size = size;
			g_VisualRestraintCiTextures[i].fingerprint = fingerprint;
			g_VisualRestraintCiTextures[i].fileNum = target ? fileNum : -1;
			return;
		}
	}

	// Non-target model loads only need to invalidate an address that was
	// previously registered by a targeted body. Do not fill the registry with
	// unrelated model textures.
	if (!target) {
		return;
	}

	s32 index;

	if (g_VisualRestraintCiTextureCount < VISUAL_RESTRAINT_CI_REGISTRY_SIZE) {
		index = g_VisualRestraintCiTextureCount++;
	} else {
		index = g_VisualRestraintCiTextureNext;
		g_VisualRestraintCiTextureNext = (g_VisualRestraintCiTextureNext + 1)
			% VISUAL_RESTRAINT_CI_REGISTRY_SIZE;
	}

	g_VisualRestraintCiTextures[index].addr = addr;
	g_VisualRestraintCiTextures[index].size = size;
	g_VisualRestraintCiTextures[index].fingerprint = fingerprint;
	g_VisualRestraintCiTextures[index].fileNum = fileNum;
}

s32 visualRestraintFindCharacterCiTextureFile(const u8 *addr, u32 size)
{
	if (!addr || !size) {
		return -1;
	}

	for (s32 i = 0; i < g_VisualRestraintCiTextureCount; i++) {
		struct visualrestraintcitexture *entry = &g_VisualRestraintCiTextures[i];

		if (entry->addr == addr
				&& entry->size == size
				&& entry->fileNum >= 0
				&& entry->fingerprint == visualRestraintTextureFingerprint(addr, size)) {
			return entry->fileNum;
		}
	}

	return -1;
}
