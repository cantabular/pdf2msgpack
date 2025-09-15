package main

import (
	"io"
	"log"
	"os"

	"github.com/sensiblecodeio/pdf2msgpack/go/pkg/p2m"
)

func main() {
	r, err := p2m.NewReader(os.Stdin)
	if err != nil {
		log.Fatal(err)
	}

	log.Printf("FileName: %q", r.Meta.FileName)
	log.Printf("Pages: %d", r.Meta.Pages)
	log.Printf("Creator: %q", r.Meta.Creator)
	log.Printf("Producer: %q", r.Meta.Producer)

	if r.Meta.FontInfo != nil {
		for _, fi := range *r.Meta.FontInfo {
			log.Printf("FontInfo: %+v\n", fi)
		}
	}

	var p p2m.Page

	for {
		err := r.Next(&p)
		if err == io.EOF {
			break
		}
		if err != nil {
			log.Fatal(err)
		}

		log.Printf("Page i=%v s=%v len(paths)=%d len(glyphs)=%d", p.Number, p.Size, len(p.Paths), len(p.Glyphs))
	}
}
