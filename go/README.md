p2m-reader
==========

Pure Go PDF2Msgpack decoder.


Installation
------------

To install the `p2m-reader` run:

    go install ./cmd/p2m-reader


Usage
-----

```sh
$ pdf2msgpack --font-info 0-example.pdf | p2m-reader
2025/09/15 16:54:06 FileName: "example.pdf"
2025/09/15 16:54:06 Pages: 4
2025/09/15 16:54:06 Creator: "Microsoft® Office Word 2007"
2025/09/15 16:54:06 Producer: "Microsoft® Office Word 2007"
2025/09/15 16:54:06 FontInfo: {Name:ABCDEE+Cambria,Bold Type:TrueType Encoding:WinAnsi Embedded:true Subset:true ToUnicode:false}
2025/09/15 16:54:06 FontInfo: {Name:ABCDEE+Cambria Type:TrueType Encoding:WinAnsi Embedded:true Subset:true ToUnicode:false}
2025/09/15 16:54:06 FontInfo: {Name:ABCDEE+Calibri Type:TrueType Encoding:WinAnsi Embedded:true Subset:true ToUnicode:false}
2025/09/15 16:54:06 Page i=1 s={595.4 841.8} len(paths)=4 len(glyphs)=34
2025/09/15 16:54:06 Page i=2 s={595.4 841.8} len(paths)=238 len(glyphs)=300
2025/09/15 16:54:06 Page i=3 s={595.4 841.8} len(paths)=237 len(glyphs)=301
2025/09/15 16:54:06 Page i=4 s={595.4 841.8} len(paths)=237 len(glyphs)=306
```
