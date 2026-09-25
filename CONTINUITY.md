# CONTINUITY

## Canonical working branch

- Branch: `visual-restraint`
- Draft PR: #1 into `port`
- Current code head before this checkpoint: `1dc65feb26751e86caeab77ecb61835c0cbb3410`
- Purpose: make Perfect Dark's presentation more materially plausible and visually restrained, using GoldenEye's naturalistic discipline as a reference without erasing Perfect Dark's identity.

## Core visual rule

Colour should read as a property of a physical material, not as a videogame faction label.

Strong colour remains appropriate for displays, alarms, illuminated controls, effects and genuinely alien technology. Large human-made cloth, armour, walls, fixtures and props should carry more restrained, physically plausible colour.

## Implemented coverage

### Global renderer

- Selective high-chroma restraint rather than flat desaturation.
- Perceptual luminance is preserved before hue is compressed.
- Small contrast lift preserves contour/value structure.
- Bright-emissive preservation is built into the shader.
- `Extended Video Options -> Visual Restraint` controls the global shader grade.
- Default restraint is `0.6`.
- Slider updates are live and do not flush texture/shader caches.
- Three-point filtering is the restrained fork's default texture filtering mode.

### Stage material profiles

Shared stage profiles now distinguish:
- dataDyne;
- G5;
- maritime / Pelagic;
- Area 51;
- aviation / Air Base / Air Force One;
- grounded human environments.

Skedar/Cetan/alien visual language is intentionally not forced into the human material grammar.

### Human character bodies

Targeted or restrained families include:
- G5 guards and G5 SWAT;
- Pelagic / Deep Sea guards;
- dataDyne security, guards, shock troops, lab staff and sniper;
- Area 51 guards, troopers and airmen;
- NSA / presidential security;
- Alaskan guards;
- CI soldiers;
- Air Force One flight crew;
- technical / lab / biotech staff;
- Chicago urban-security/robber bodies;
- office workers, secretary, stripes and negotiator support bodies.

Direct-colour RGBA16/RGBA32 embedded textures and model colour arrays are both covered for these targeted models.

### Human environments

The restraint system now reaches beyond vertex colour:
- room colour arrays;
- stage-scoped paletted room textures;
- atmosphere colours (sky/cloud/water);
- architectural props;
- doors, lifts, crates, tables, chairs, desks, cabinets, lockers, gates, barriers, mainframes, pillars and walls;
- human fixture panels and consoles;
- grounded human multiplayer arenas.

Prop classification is cached per file to avoid repeated filename scanning.

### Colour preservation

- Bright displays/lights are deliberately protected by material thresholds.
- The global shader has an emissive-preservation term.
- Alien fixtures are excluded from the human architectural-prop treatment.

## Validation

- The broad restraint system has repeatedly compiled successfully on Linux.
- An interrupted-turn Windows failure exposed a real cross-platform type bug: cloud/water environment colours are `f32`, while the original atmosphere helper accepted `u8*`.
- This is fixed by a dedicated float atmosphere wrapper that converts/clamps through the same 8-bit colour treatment and writes the result back as `f32`.
- The current head must be green on both Windows x64 and Linux x64 before further broad art changes are stacked on it.

## Deliberate remaining limits

- Paletted **character/model** textures are not yet treated; only direct-colour embedded character textures are. Do not modify CI indices blindly.
- Heads, weapons and pickups are not broadly colour-rewritten.
- Alien/Skedar/Cetan material colour remains intentionally freer and more saturated.
- Current thresholds are engineering/art-direction first passes; screenshot review will eventually tune exact values.

## Next technical unit

1. Keep Windows + Linux CI green.
2. Trace paletted character/model texture palettes safely and add palette-aware treatment for already-targeted human bodies.
3. Audit remaining principal human body models for conspicuous videogame colour, adding only restrained general treatment where justified.
4. Then move from palette correction into material breakup: wear, dirt, mottling and variation on the worst flat human-made surfaces.
5. Preserve the distinction between restrained human materials and legitimately vivid alien/technological colour.
