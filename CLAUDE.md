# CLAUDE.md - trigger-seq (Hook's Gator)

## Purpose
Hook's Gator is an 8/16-step gate/trigger sequencer module for Eurorack/modular synthesizers. The project is being revisited ("renewing with the idea of maybe solving its problems with other tools"). It has two major components: PIC microcontroller firmware and KiCad PCB/schematic hardware design.

## Project Structure
- **firmware/gator.X/**: MPLAB X project for PIC16F15345 firmware (C, XC8 compiler)
- **hardware/**: KiCad schematics, PCB layouts, gerber files, and backup revisions
- **README.md**: Minimal — one line noting the project revival intent

## Key Concepts
- Sequencer outputs 8 or 16 gate/trigger steps (2x8 or 1x16 mode)
- Controls: Rate (tempo), Width (duty cycle), Steps (sequence length), Swing (timing offset)
- Digital inputs: Run/Stop, Up/Down (direction), Reset, Mode (2x8 vs 1x16)
- Two CD4051 analog multiplexers select which step output fires; transistor buffers invert signals
- Swing offsets every other beat by a fraction of the period

## Subdirectory Documentation
- See [firmware/gator.X/CLAUDE.md](firmware/gator.X/CLAUDE.md) for firmware details
- See [hardware/CLAUDE.md](hardware/CLAUDE.md) for PCB/schematic details
