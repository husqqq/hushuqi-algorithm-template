"""Shared PDF renderer with retained TeX diagnostics and atomic output."""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile


def render_pdf(command, out, cwd):
    work = Path(tempfile.mkdtemp(prefix='template-tex-'))
    tex = work/'document.tex'
    subprocess.run(command+['-s', '-o', str(tex)], cwd=cwd, check=True)
    for run in range(3):
        result = subprocess.run(['xelatex', '-halt-on-error', '-interaction=nonstopmode',
                                 '-output-directory='+str(work), str(tex)], cwd=cwd,
                                capture_output=True)
        (work/f'pass-{run+1}.txt').write_bytes(result.stdout+result.stderr)
        if result.returncode:
            raise RuntimeError(f'XeLaTeX failed; diagnostics: {work}')
    log = (work/'document.log').read_text(encoding='utf-8', errors='replace')
    bad = ('Missing character:', 'There were undefined references', 'There were undefined citations')
    if any(item in log for item in bad):
        raise RuntimeError(f'Missing glyphs or unresolved references; diagnostics: {work}')
    boxes = re.findall(r'Overfull \\[hv]box \(([^\n]+)', log)
    print(f'TEX_DIAGNOSTICS overfull={len(boxes)} logs={work}', flush=True)
    if boxes:
        print('\n'.join(boxes[:20]), flush=True)
        raise RuntimeError(f'Overfull boxes; diagnostics: {work}')
    from pypdf import PdfReader
    pdf = work/'document.pdf'
    pages = len(PdfReader(pdf).pages)
    if pages < 2:
        raise ValueError(f'Empty PDF; diagnostics: {work}')
    out.parent.mkdir(parents=True, exist_ok=True)
    pending = out.with_suffix('.pdf.tmp')
    shutil.copyfile(pdf, pending)
    pending.replace(out)
    print(f'PDF_BUILT {out.name} pages={pages}', flush=True)
    return pages
