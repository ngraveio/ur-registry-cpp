bin_PROGRAMS = decode_ur_interactive encode_ur_interactive

decode_ur_interactive_SOURCES = \
    display_ur_fields.cpp \
    decode_ur_interactive.cpp

encode_ur_interactive_SOURCES = \
    encode_ur_types.cpp \
    encode_ur_interactive.cpp

decode_ur_interactive_LDADD = $(top_builddir)/src/libur-registry.la -lbc-ur -lwallylite -ltinycbor
encode_ur_interactive_LDADD = $(top_builddir)/src/libur-registry.la -lbc-ur -lwallylite -ltinycbor

pkgincludedir = $(includedir)/ur-registry/examples
pkginclude_HEADERS = \
  display_ur_fields.h \
  encode_ur_types.h