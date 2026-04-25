# CLAUDE.md - hardware

## Purpose
KiCad hardware design files for the Hook's Gator trigger sequencer PCB and front panel. Includes schematics, PCB layouts, component library, netlist, BOM, and manufactured gerbers. Multiple backup revisions are present.

## Key Files
- **trigger-seq.sch**: Top-level schematic (root sheet)
- **main.sch / clock.sch / counter.sch / power.sch / switching.sch**: Hierarchical sub-sheets
- **clock-processing.sch / reset-processing.sch**: Additional processing sub-sheets
- **trigger-seq.kicad_pcb**: Main PCB layout (2-layer)
- **trigger-seq-front-panel.kicad_pcb**: Front panel PCB
- **trigger-seq-panel.kicad_pcb**: Panel/carrier PCB
- **trigger-seq-cache.lib / trigger-seq-rescue.lib**: Local component symbol libraries
- **trigger-seq.net**: KiCad netlist export
- **trigger-seq.csv**: BOM export
- **trigger-seq.pro**: KiCad project file

## Gerbers
- **gerbers/**: Production gerbers for the main PCB (F.Cu, B.Cu, F.Mask, B.Mask, F.SilkS, B.SilkS, Edge.Cuts, drill)
- **gerbers/front-panel/**: Gerbers for the front panel PCB
- **gerbers/panel/**: (directory present, contents may vary)
- Gerbers at the hardware root level are older exports; prefer **gerbers/** subdirectory for fabrication

## Backup Revisions
- **backup/, backup2/, backup3/**: Incremental design snapshots; `backup3/` appears most complete with full schematic set
- `.bak` files alongside `.sch`/`.kicad_pcb` files are KiCad auto-backups — safe to ignore

## Design Notes
- Uses two CD4051 8-channel analog multiplexers to select which step is active
- NPN transistor buffers invert address/inhibit signals between the PIC and 4051s (logic must be pre-inverted in firmware)
- Power supply schematic on `power.sch`; likely ±12V Eurorack rails with 5V regulation for PIC
- Front panel is a separate PCB (common Eurorack approach for laser-cut or PCB panels)

## Tools Required
- KiCad 5.x (files use legacy `.sch` format, not KiCad 6+ `.kicad_sch`)
- Gerber viewer (e.g., gerbv) to inspect fabrication files

## Critical Knowledge
- The `.sch` format is KiCad 4/5 legacy; opening in KiCad 6+ will prompt for migration — make a backup first
- `trigger-seq-rescue.lib` exists because KiCad couldn't find some symbols at rescue time — keep it alongside the project
- `electro-music-help/` subdirectory likely contains reference material from the electro-music.com forum
