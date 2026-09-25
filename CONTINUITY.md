# CONTINUITY

## Canonical working branch

- Branch: `visual-restraint`
- Draft PR: #1 into `port`
- Purpose: make Perfect Dark's presentation more materially plausible and restrained, taking cues from GoldenEye's muted naturalism without erasing Perfect Dark's identity.

## Visual rule

Colour should read as a property of a physical material, not as a videogame faction label.

Keep strong colour where it makes physical/semantic sense: screens, alarms, illuminated controls, effects and genuinely unusual technology. Large cloth, armour and architectural surfaces should be darker, dirtier and less poster-like.

## Implemented

### 1. Global renderer restraint

A configurable OpenGL shader pass:
- preserves perceptual luminance;
- selectively compresses stronger chroma rather than applying flat desaturation;
- adds a small contrast lift to retain contour;
- defaults to `0.6`;
- is exposed as **Extended Video Options -> Visual Restraint**.

Setting the slider to `0` disables the global shader grade only. Model/stage-specific treatments remain part of this fork.

Slider changes now update the active shader uniform directly; they no longer clear the texture/shader cache.

### 2. G5 guard treatment

Target body files:
- `FILE_CG5_GUARD`
- `FILE_CG5_SWAT_GUARD`

Treatment:
- bright cyan/sky-blue material colour is pulled toward darker steel/slate blue;
- embedded RGBA16/RGBA32 textures are adjusted;
- model vertex/material colour arrays are adjusted too;
- heads, weapons and unrelated models are not touched.

### 3. Pelagic / Deep Sea guard treatment

Target body:
- `FILE_CPELAGIC_GUARD`

Confirmed from stage setup code: both Pelagic II and Deep Sea use this body.

Treatment:
- bright clean red -> darker worn maritime red/burgundy;
- stark near-white -> warmer off-white;
- embedded RGBA16/RGBA32 textures and model colour arrays are both covered.

### 4. dataDyne character treatment

Target bodies:
- dataDyne security guard
- standard dataDyne guard
- shock guard / shock infantry
- dataDyne lab tech
- dataDyne sniper

Purple-dominant material colours are selectively pulled toward darker plum/charcoal. Neutral regions are left alone.

### 5. dataDyne architectural treatment

The PC background preprocessor now knows the active stage.

For:
- Defection
- Investigation
- Extraction
- Mr Blonde's Revenge

room geometry colour arrays selectively compress purple-dominant non-emissive material tints. Very bright values are deliberately excluded to preserve screens/lights/tech accents.

### 6. CI / test packaging

`.github/workflows/visual-restraint-ci.yml` builds:
- Windows x86_64 NTSC-final
- Linux x86_64 NTSC-final

The Windows job uploads a ready-to-test ZIP containing the executable and required DLLs.

Concurrency is enabled so newer branch pushes cancel superseded visual-restraint CI runs.

## Validation state

Known successful Windows + Linux CI:
- base renderer/texture implementation;
- G5/Pelagic vertex-colour pass;
- dataDyne character-body pass.

The final architectural/background + live-uniform head must still receive its own green CI run before being treated as build-validated.

## Deliberate limits

- Paletted CI character textures are not modified yet. Do not blindly recolour palette indices.
- Do not broaden hue transforms until in-game screenshots show what remains wrong.
- Do not globally flatten bright displays/effects merely to reduce saturation.
- The current numbers are first-pass thresholds, not final art-direction values.

## Immediate next step

Build and run the current branch with an NTSC-final ROM, then capture matched screenshots from:
1. G5 Building;
2. Pelagic II;
3. Deep Sea;
4. dataDyne Defection;
5. dataDyne Investigation;
6. dataDyne Extraction.

For each useful scene compare `Visual Restraint = 0`, `0.3`, and `0.6`.

Use those screenshots to tune thresholds before adding any broader treatment.
