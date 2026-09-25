# Visual Direction: Restrained Perfect Dark

This fork retains Perfect Dark's geometry, level identity, sci-fi world and recognisable faction language, but removes the conspicuous videogame-colour treatment that makes parts of it feel more cartoonish than GoldenEye.

## Principle

**Colour should look caused by a material, light source or technology—not chosen merely to label an asset.**

Human-made cloth, armour, painted metal, concrete and architecture should therefore be more restrained. Strong colour remains available to screens, alarms, lighting, effects and genuinely alien technology.

## Hierarchy

The intended visual hierarchy is:

**silhouette -> value/shadow -> material -> colour**

rather than letting hue dominate recognition.

## Current treatment

The renderer supplies a selective high-chroma baseline while preserving luminance and contour. Stage-specific preprocessing then applies more meaningful material treatment to character bodies, room colour arrays, paletted room textures, atmosphere and architectural props.

Examples:
- G5 cyan armour -> darker steel/slate blue;
- Pelagic red -> worn maritime red/burgundy, with warmer off-white cloth;
- dataDyne purple -> darker plum/charcoal on broad human-made surfaces;
- Area 51 security colour -> navy/steel/olive rather than clean faction blue/green;
- aviation uniforms -> formal navy/muted red rather than toy-like primary colour.

The objective is **not** to make Perfect Dark grey. Colour becomes more effective because it is no longer equally loud everywhere.

## Preserve deliberately

Do not automatically suppress:
- screens and illuminated controls;
- alarms and effect colours;
- strong light sources;
- Skedar/Cetan/alien technology;
- colour whose unusual intensity is itself part of the fiction.

## Remaining art problem

Direct-colour character textures and model tints are covered broadly, but CI-paletted character/model textures still require palette-aware handling. After that, the project can move beyond palette restraint into material detail: grime, wear, mottling and texture breakup on conspicuously flat human-made surfaces.
