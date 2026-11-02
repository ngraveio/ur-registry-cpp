#include "Test_Intent.h"
#include "Test_UrHelpers.h"

void TestIntent::testValidIntent()
{
  Intent expectedIntent{"$BATCH.",
                        {{"TX(1)", 5},
                         {"TX(2)", 5},
                         {"TX(3)", 5},
                         {"SIG(2)", 5},
                         {"SIG(3)", 5},
                         {"SIG(4)", 5}}};

  const std::string TestIntentUr = "ur:intent/oyadoeadiodkfwfpghfxfddmaolnlfihghhddeehdtahlfihghhddeeydtahlfihghhddeeodtahlfiygugafldeeydtahlfiygugafldeeodtahlfiygugafldeeedtahinknoeps";
  Intent TestIntent;
  TestIntent.fromUr(TestIntentUr);

  Compare(TestIntent, expectedIntent);
}

void TestIntent::testIntentWrongParamTupleLength()
{
  /*
  {
    1: {
      1: "$BATCH.",
      2: [
        ["TX(0)", 5],
        ["TX(1)", 5, 5],
        ["TX(2)", 5],
        ["SIG(1)", 5],
        ["SIG(2)", 5],
        ["SIG(3)", 5]
      ]
    }
  }
  }*/
  const std::string TestIntentUr = "ur:intent/oyadoeadiodkfwfpghfxfddmaolnlfihghhddedydtahlsihghhddeehdtahahlfihghhddeeydtahlfiygugafldeehdtahlfiygugafldeeydtahlfiygugafldeeodtahonhngmwd";
  Intent TestIntent;

  CPPUNIT_ASSERT_THROW_MESSAGE(
      "Throws on parameter tuple length not equal to 2",
      TestIntent.fromUr(TestIntentUr),
      CborException);
}

void TestIntent::testIntentEncodingMaxUint16()
{
  // The valid intent above, with the encoding of "TX(1)" set to 65535
  Intent expectedIntent{"$BATCH.",
                        {{"TX(1)", 65535},
                         {"TX(2)", 5},
                         {"TX(3)", 5},
                         {"SIG(2)", 5},
                         {"SIG(3)", 5},
                         {"SIG(4)", 5}}};

  const std::string TestIntentUr = "ur:intent/oyadoeadiodkfwfpghfxfddmaolnlfihghhddeehdtcfzmzmlfihghhddeeydtahlfihghhddeeodtahlfiygugafldeeydtahlfiygugafldeeodtahlfiygugafldeeedtahlnaaldnb";
  Intent TestIntent;
  TestIntent.fromUr(TestIntentUr);

  Compare(TestIntent, expectedIntent);
}

void TestIntent::testIntentEncodingOutOfRange()
{
  // The valid intent above, with the encoding of "TX(1)" outside uint .size 2
  Intent TestIntent;

  ValidateUrDecodingException(TestIntent, "ur:intent/oyadoeadiodkfwfpghfxfddmaolnlfihghhddeehdtcxlfihghhddeeydtahlfihghhddeeodtahlfiygugafldeeydtahlfiygugafldeeodtahlfiygugafldeeedtahploeiese",
                              CborErrorIllegalType, "1. Encoding -1 failed");
  ValidateUrDecodingException(TestIntent, "ur:intent/oyadoeadiodkfwfpghfxfddmaolnlfihghhddeehdtcyaeadaeaelfihghhddeeydtahlfihghhddeeodtahlfiygugafldeeydtahlfiygugafldeeodtahlfiygugafldeeedtahhdmweysf",
                              CborErrorImproperValue, "2. Encoding 65536 failed");
  // 2^32 + 5 used to be truncated to 5
  ValidateUrDecodingException(TestIntent, "ur:intent/oyadoeadiodkfwfpghfxfddmaolnlfihghhddeehdtcwaeaeaeadaeaeaeahlfihghhddeeydtahlfihghhddeeodtahlfiygugafldeeydtahlfiygugafldeeodtahlfiygugafldeeedtahsomktkso",
                              CborErrorImproperValue, "3. Encoding 2^32 + 5 failed");
}

void TestIntent::Compare(const Intent &decodedIntent, const Intent &expectedIntent)
{

  CPPUNIT_ASSERT_EQUAL_MESSAGE(
      "Prefixes placeholders don't match",
      decodedIntent.getSubtituteFields().prefix_str,
      expectedIntent.getSubtituteFields().prefix_str);

  CPPUNIT_ASSERT_MESSAGE(
      "Parameter fields arrays don't match",
      decodedIntent.getSubtituteFields().fields_array == expectedIntent.getSubtituteFields().fields_array);
}
