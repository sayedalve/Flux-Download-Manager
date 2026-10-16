const fs = require('fs');

function inspect() {
  const bg = fs.readFileSync('idm audit/extensions/IDMGCExt/background.js', 'utf8');
  console.log('--- background.js downloads occurrences ---');
  let idx = 0;
  while ((idx = bg.indexOf('downloads', idx)) !== -1) {
    console.log(bg.substring(Math.max(0, idx - 40), Math.min(bg.length, idx + 80)));
    console.log('---');
    idx += 9;
  }

  const cs = fs.readFileSync('idm audit/extensions/IDMGCExt/content.js', 'utf8');
  console.log('--- content.js click/link occurrences ---');
  idx = 0;
  while ((idx = cs.indexOf('click', idx)) !== -1) {
    console.log(cs.substring(Math.max(0, idx - 40), Math.min(cs.length, idx + 80)));
    console.log('---');
    idx += 5;
  }
}

inspect();
