# Visual Direction: restrained Perfect Dark

This fork keeps Perfect Dark's models, level design and sci-fi identity, but pushes the presentation toward GoldenEye's more disciplined visual naturalism.

## Core rule

Colour should read as a property of a physical material, not as a label painted onto a videogame asset.

That means:
- preserve luminance and silhouette before hue;
- compress conspicuously strong chroma rather than indiscriminately desaturating everything;
- keep already-muted concrete, cloth, metal, stone and shadow largely unchanged;
- reserve strong colour for displays, alarms, effects and genuinely unusual technology;
- prefer darker, dirtier, material-plausible variants of faction colours rather than deleting faction colour coding.

## First implementation

The renderer now has a configurable **Visual Restraint** pass. It is deliberately not a simple saturation slider.

For each final fragment it:
1. calculates perceptual luminance;
2. separates chroma from luminance;
3. detects only stronger chroma;
4. compresses that chroma progressively;
5. adds a small contrast lift to retain contour.

The fork defaults to 0.6. Setting **Extended Video Options -> Visual Restraint** to 0 restores the original colour response.

## Targeted asset pass: next

The global pass is a baseline, not the final treatment. The next work should identify and selectively revise the worst offenders while preserving their identity:

- **G5 guards** — cyan/sky-blue armour -> slate/steel blue, darker value structure, less uniform colour.
- **Pelagic II / Deep Sea guards** — bright red + stark white -> faded maritime red/burgundy + dirty off-white, stronger fabric/shadow variation.
- **dataDyne interiors and uniforms** — keep violet branding but pull large flat purples toward charcoal/plum; preserve small illuminated accents.
- **environmental materials** — identify large flat-colour surfaces that would benefit from dirt, wear, mottling or restrained hue variation.

The goal is not to make Perfect Dark grey or to imitate GoldenEye asset-for-asset. The goal is to make Perfect Dark's world look less visibly colour-designed and more materially plausible.
