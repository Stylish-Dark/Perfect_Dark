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
