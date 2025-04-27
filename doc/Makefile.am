MANPAGE_DOCS = ui-active.md \
	ui-armable.md \
	ui-button.md \
	ui-composite.md \
	ui-context.md \
	ui-font.md \
	ui-label.md \
	ui-manager.md \
	ui-password.md \
	ui-pie_menu.md \
	ui-radio_box.md \
	ui-rect.md \
	ui-row_column.md \
	ui-text_field.md \
	ui-toggle.md \
	ui-utf8tou32str.md \
	ui-vertex_buffer.md \
	ui-widget.md

dist_doc_DATA = ui.md template.md $(MANPAGE_DOCS)

# Make is not properly able to handle filenames with colons, so we
# need to resort to this madness to get the manpages installed with
# the filenames we want.
if GENERATE_MANPAGES
SUFFIXES = .md .3

MANPAGES = $(MANPAGE_DOCS:.md=.3)

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
