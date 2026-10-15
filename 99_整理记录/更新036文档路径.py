"""Rewrite 036 source links in maintained note templates after folder moves."""
from pathlib import Path
import json

root = Path(__file__).resolve().parents[1]
plan = json.loads((root / '99_整理记录/036_路径迁移.json').read_text(encoding='utf-8'))
moves = sorted(plan, key=lambda item: len(item['from']), reverse=True)
changed = []
for path in (root / '99_整理记录/知识库模板').glob('*.md'):
    old = path.read_text(encoding='utf-8')
    new = old
    for item in moves:
        new = new.replace(item['from'], item['to'])
    if new != old:
        path.write_text(new, encoding='utf-8')
        changed.append(path.name)
print(json.dumps({'updated_templates': changed}, ensure_ascii=False))
