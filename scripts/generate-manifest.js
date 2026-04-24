#!/usr/bin/env node
const fs = require('fs')
const path = require('path')
const matter = require('gray-matter')

const docsDir = path.join(__dirname, '..', 'docs')
const manifestPath = path.join(docsDir, 'manifest.json')

const entries = fs.readdirSync(docsDir)
    .filter(f => f.endsWith('.mdx'))
    .sort()
    .map(filename => {
        const { data } = matter(fs.readFileSync(path.join(docsDir, filename), 'utf8'))
        return {
            filename,
            slug: data.slug || filename.replace(/\.mdx$/, ''),
            title: data.title || '',
            type: data.type || 'Doc',
            status: data.status || 'Draft',
            category: data.category || '',
            heading: data.heading || '',
            published_date: data.published_date || '',
            updated_date: data.updated_date || '',
            image: data.image || '',
            image_attribution: data.image_attribution || '',
        }
    })

fs.writeFileSync(manifestPath, JSON.stringify(entries, null, 2) + '\n')
console.log(`Wrote ${entries.length} entries to ${path.relative(process.cwd(), manifestPath)}`)
