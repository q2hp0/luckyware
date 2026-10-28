# thanks you Zenobia for creating this script which btw I can made it in less than 20 seconds with using AI. https://github.com/zenobiatranoss
#!/usr/bin/env bash
set -e
d="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
b="$d/build"
mkdir -p "$b"
cd "$b"
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
echo "done"
ls -la ./anti-luckyware 2>/dev/null || ls -la ./Release/anti-luckyware.exe 2>/dev/null
