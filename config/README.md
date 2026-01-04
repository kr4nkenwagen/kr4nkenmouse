# kr4nkenmouse configurator

This tool lets you configure kr4nkenmouse easily over serial.

## Prerequisites

You must have **uv** installed. If you don't have it yet:
- **Linux/macOS:** `curl -LsSf https://astral-sh.uv.io/install.sh | sh`
- **Windows:** `powershell -c "irm https://astral-sh.uv.io/install.ps1 | iex"`

## How to Run

The simplest way to run this without managing virtual environments is using `uv run`.

### Single Command
```bash
uv run --with pyserial configurator.py set_sensitivity=100

### Installation
```bash
uv sync
uv run configurator.py
