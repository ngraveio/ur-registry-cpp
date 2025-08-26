pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ur-registry.pc
SUBDIRS = src tests
EXTRA_DIST = autogen.sh
ACLOCAL_AMFLAGS = -I m4

TESTS =
check-local: lint-cppcheck lint-flawfinder

.PHONY: lint-cppcheck lint-flawfinder

lint-cppcheck:
	@echo "Running cppcheck..."
	cppcheck --std=c++17 --language=c++ \
	         --inline-suppr --suppress=missingIncludeSystem \
	         --suppress=unusedFunction \
	         --enable=all --force -q --error-exitcode=1 ./

lint-flawfinder:
	@echo "Running flawfinder..."
	flawfinder --minlevel=4 --error-level=4 ./