SUFFIXES = .md .3

MANPAGES =

if GENERATE_MANPAGES
MANPAGES += ui-rect.3 \
	ui-active.3 \
	ui-vertex_buffer.3
endif

man3_MANS = $(MANPAGES)

.md.3: $<
	$(RONN) --roff --pipe --name="$(basename $<)" --section=3 --date='2019-08-07' --manual="CuddlyGL Programmer's Manual" --organization="CuddlyGL $(VERSION)" < $< > $@

clean-local:
	rm -rf $(MANPAGES)
