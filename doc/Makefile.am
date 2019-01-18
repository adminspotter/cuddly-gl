SUFFIXES = .md .3

MANPAGES =

if GENERATE_MANPAGES
MANPAGES += ui-rect.3 \
	ui-active.3 \
	ui-vertex_buffer.3
endif

man3_MANS = $(MANPAGES)

.md.3: $<
	$(RONN) --manual='CuddlyGL' < $< > $@

clean-local:
	rm -rf $(MANPAGES)
