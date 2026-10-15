#!/usr/bin/env bash
set -euo pipefail
HERE="$(cd "$(dirname "$0")" && pwd)"
if command -v python3 >/dev/null 2>&1; then
  exec python3 "$HERE/build.py" "$@"
fi
exec python "$HERE/build.py" "$@"
