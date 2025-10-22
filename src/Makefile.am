AM_CXXFLAGS  = -std=c++17 -Wall -Wextra -Wpedantic -fvisibility=hidden
AM_CPPFLAGS  = -I$(top_srcdir)/src

lib_LTLIBRARIES = libur-registry.la
libur_registry_la_LDFLAGS = -version-info 1:0:0 -static -no-undefined

libur_registry_la_SOURCES = \
  batch_sign_request.cpp batch_sign_response.cpp \
  coin_identity.cpp coininfo.cpp cosmos_signature.cpp \
  detailed_account.cpp eth_signature.cpp eth_sign_request.cpp \
  hdkey_bip32.cpp hdkey.cpp intent.cpp \
  keypath_component.cpp keypath.cpp \
  output_descriptor.cpp output_descriptor_function.cpp \
  portfolio_coin.cpp portfolio.cpp portfolio_metadata.cpp \
  registry_item.cpp request_base.cpp signature_base.cpp \
  sign_request.cpp sign_response.cpp \
  uai.cpp uuid.cpp

pkgincludedir = $(includedir)/ur-registry
pkginclude_HEADERS = \
  batch_sign_request.h batch_sign_response.h cbor_exception.h \
  coin_identity.h coininfo.h cosmos_signature.h \
  detailed_account.h eth_signature.h eth_sign_request.h \
  hdkey_bip32.h hdkey.h intent.h \
  keypath_component.h keypath.h \
  output_descriptor.h output_descriptor_function.h \
  portfolio_coin.h portfolio.h portfolio_metadata.h \
  registry_item.h registry_type.h request_base.h signature_base.h \
  sign_request.h sign_response.h \
  uai.h uuid.h
