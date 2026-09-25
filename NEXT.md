# NEXT

1. Validate the latest `visual-restraint` branch on Windows and Linux CI after the vertex-colour/dataDyne pass.
2. Test in-game with an NTSC-final ROM, prioritising:
   - G5 Building guards;
   - Pelagic II guards;
   - Deep Sea guards;
   - dataDyne Defection / Investigation / Extraction guards.
3. Compare `Visual Restraint = 0`, `0.3`, and `0.6` and choose the baseline only after screenshots.
4. Inspect whether any remaining bright uniform regions are CI-paletted; do not alter CI indices blindly.
5. Trace background room colour arrays for the dataDyne stages and implement a selective architectural purple treatment.
6. Preserve bright colour for screens, alarms, effects and small technological accents rather than globally flattening them.
