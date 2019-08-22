SUFFIXES = .md .3

MANPAGES =

if GENERATE_MANPAGES
MANPAGES += ui-rect.3 \
	ui-active.3 \
	ui-vertex_buffer.3 \
	ui-widget.3 \
	ui-label.3
endif

# Make is not properly able to handle filenames with colons, so we
# need to resort to this madness to get the manpages installed with
# the filenames we want.
noinst_DATA = $(MANPAGES)

# ronn may add an extra NAME section that we don't need, so we strip
# it out with the sed command.
.md.3:
	$(RONN) --roff --pipe \
		--name="$(subst -,::,$(basename $<))" \
		--section=3 \
		--manual="$(PACKAGE_NAME) Programmer's Manual" \
		--organization="$(PACKAGE_STRING)" \
		--date='2019-08-10' \
		< $< > $@
	if [ "$$(grep -c '^\.SH "NAME"$$' $@)" != "1" ] ;\
	then \
	  sed -i -e '/\.SH "NAME"/{N;/- $(subst -,::,$(basename $<))$$/{s/\n//;d;}}' $@ ;\
	fi

if GENERATE_MANPAGES
install-data-local:
	$(mkdir_p) $(mandir)/man3
	for man in $(MANPAGES) ;\
	do \
	  $(INSTALL_DATA) $$man $(mandir)/man3/$$(echo $$man | sed -e 's/-/::/g') ;\
	done

uninstall-local:
	for man in $(MANPAGES) ;\
	do \
	  rm -f $(mandir)/man3/$$(echo $$man | sed -e 's/-/::/g') ;\
	done

clean-local:
	rm -f $(MANPAGES)
endif
