#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <string>

#include "Test_OutputDescriptor.h"
#include "../src/output_descriptor.h"
#include "Test_UrHelpers.h"

void TestOutputDescriptor::setUp(void)
{

}

void TestOutputDescriptor::tearDown(void)
{
    
}

void TestOutputDescriptor::testValidOutDescWithPlaceholders()
{
    /* we only set the key data since it's the only mandatory field in the HDKey URType */

    HDKey key0;
    key0.setKeyData(convertToKeyData(fromHex("021c0b479ecf6e67713ddf0c43b634592f51c037b6f951fb1dc6361a98b1e5735e")));
    
    HDKey key1;
    key1.setKeyData(convertToKeyData(fromHex("0397fcf2274abd243d42d42d3c248608c6d1935efca46138afef43af08e9712896")));
    
    const OutputDescriptor output{"wsh(sortedmulti(2,@0,@1))", {key0, key1}, "a name", "a note"};

    const auto expectedBytes = fromHex("A401781977736828736F727465646D756C746928322C40302C403129290282D99D6FA1035821021C0B479ECF6E67713DDF0C43B634592F51C037B6F951FB1DC6361A98B1E5735ED99D6FA10358210397FCF2274ABD243D42D42D3C248608C6D1935EFCA46138AFEF43AF08E9712896036661206E616D65046661206E6F7465");
    const auto expectedUR = std::string("ur:output-descriptor/oxadkscfktjkisdejkjljpjyihiejnkpjzjyindeeydwfzdydwfzehdtdtaolftantjloyaxhdclaocebdflnntkjtiojsfsurbnfxrpeehkdlgyrtemrpytgyzocaswencymkpavwjkhytantjloyaxhdclaxmsztwzdigerydkfsfwtydpfndklnayswttmuhyztoxhsetpewsfxpeaywljsdemtaxiyhscxjthsjnihaaiyhscxjtjljyihvsdrtkec");

    ValidateCborResults(output, expectedBytes);
    ValidateUrEncoding(output, expectedUR);

}

void TestOutputDescriptor::testValidTextualOutputDesc()
{
    const OutputDescriptor output{"pk(03e220e776d811c44075a4a260734445c8967865f5357ba98ead3bc6a6552c36f2)"};

    const auto expectedBytes = fromHex("A1017846706B2830336532323065373736643831316334343037356134613236303733343434356338393637383635663533353762613938656164336263366136353532633336663229");
    const auto expectedUR = std::string("ur:output-descriptor/oyadksfgjojededyeoiheyeydyihememenieetehehiaeeeedyemechseehseyendyemeoeeeeeeeciaetesenemeteneciyeceoecemidhsesetihhsieeoidiaenhsenececeyiaeoeniyeydtlyfepacf");

    ValidateCborResults(output, expectedBytes);
    ValidateUrEncoding(output, expectedUR);
}

void TestOutputDescriptor::testInvalidOutputDescriptors()
{
    const HDKey hdkey{};

    /* empty source */
    OutputDescriptor output {""};    
    ValidateUrEncodingException(output, CborError::CborErrorImproperValue);
    
    /* source with placeholders but keys array is empty */
    output.setSource("(@000,@01)");
    ValidateUrEncodingException(output, CborError::CborErrorImproperValue);
    
    /* length of keys array is shorted than than the length of the placeholders in the source */
    output.setKeys({hdkey});
    ValidateUrEncodingException(output, CborError::CborErrorImproperValue);
    
    /* placeholder is not a number */
    output.setSource("pk(@z4,@1)");
    output.setKeys({hdkey, hdkey});
    ValidateUrEncodingException(output, CborError::CborErrorImproperValue);
    
    /* source is textual but keys is none empty */
    output.setSource("pk(03e220e776d811c44075a4a260734445c8967865f5357ba98ead3bc6a6552c36f2)");
    ValidateUrEncodingException(output, CborError::CborErrorImproperValue);
    
    /* unsupported legacy type */
    output.setKeys({});
    output.setUseLegacyType(true);    
    ValidateUrEncodingException(output, CborError::CborErrorIllegalType);
}
