#pragma once

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../src/hdkey.h"

class TestHDKey : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestHDKey);
    CPPUNIT_TEST(EncodeMasterKey);
    CPPUNIT_TEST(EncodeBtcTesnetPubKey);
    CPPUNIT_TEST(EncodeEthXpub);
    CPPUNIT_TEST(EncodeEthXprvWithHardenedChildren);
    CPPUNIT_TEST(EncodeRandomXpubWithChildren);
    CPPUNIT_TEST(EncodeDerivedKeyWithBtcBIP44DefinedInUseInfo);
    CPPUNIT_TEST(EncodeSingleKeyData);
    CPPUNIT_TEST(EncodePrivateKeyFullyDefined);
    CPPUNIT_TEST(EncodeIncorrectMasterKeyWithoutChainCode);
    CPPUNIT_TEST(EncodeIncorrectMasterKeyWithExcludedFields);
    CPPUNIT_TEST(EncodeIncorrectParentFingerprintForSingleDerivationPath);
    CPPUNIT_TEST(EncodeIncorrectBIP44InUseInfo);
    CPPUNIT_TEST(EncodeIncorrectBIP44WithTooShortDerivation);
    CPPUNIT_TEST(EncodeIncorrectBIP44WithInvalidChildType);
    CPPUNIT_TEST(EncodeIncorrectHardenedChildren);
    CPPUNIT_TEST(ExtractXPubParentFingerprints);
    CPPUNIT_TEST(ExtractXPrvParentFingerprints);
    CPPUNIT_TEST(ExtractParentFingerprintFromInvalidKeys);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    void EncodeMasterKey(void);
    void EncodeBtcTesnetPubKey(void);
    void EncodeEthXpub(void);
    void EncodeEthXprvWithHardenedChildren(void);
    void EncodeRandomXpubWithChildren(void);
    void EncodeDerivedKeyWithBtcBIP44DefinedInUseInfo(void);
    void EncodeSingleKeyData(void);
    void EncodePrivateKeyFullyDefined(void);
    void EncodeIncorrectMasterKeyWithoutChainCode(void);
    void EncodeIncorrectMasterKeyWithExcludedFields(void);
    void EncodeIncorrectParentFingerprintForSingleDerivationPath(void);
    void EncodeIncorrectBIP44InUseInfo(void);
    void EncodeIncorrectBIP44WithTooShortDerivation(void);
    void EncodeIncorrectBIP44WithInvalidChildType(void);
    void EncodeIncorrectHardenedChildren(void);
    void ExtractXPubParentFingerprints(void);
    void ExtractXPrvParentFingerprints(void);
    void ExtractParentFingerprintFromInvalidKeys(void);

private:

};

