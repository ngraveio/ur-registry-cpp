AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = test_ur_registry
TESTS = $(check_PROGRAMS)

test_ur_registry_SOURCES = main.cpp \
    Test_EthSignRequest.cpp Test_HDKey.cpp Test_Keypath.cpp \
    Test_CoinInfo.cpp Test_OutputDescriptor.cpp Test_DetailedAccount.cpp \
    Test_PortfolioMetadata.cpp Test_PortfolioCoin.cpp Test_Portfolio.cpp \
    Test_UrSignatures.cpp Test_CoinIdentity.cpp Test_SignRequest.cpp \
    Test_BatchSignRequest.cpp Test_Intent.cpp

test_ur_registry_LDADD = $(top_builddir)/src/libur-registry.la -lcppunit -ldl