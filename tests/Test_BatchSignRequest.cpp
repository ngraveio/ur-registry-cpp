#include "Test_BatchSignRequest.h"
#include "Test_UrHelpers.h"
#include "Test_SignRequest.h"



void TestBatchSignRequest::testBatchSignRequestDecoding()
{
    const std::string requestsUr = "ur:batch-sign-request/oyadlstaoysronadtpdagdfrghbbemhyftfebdmyvydacerfdnfhreaotaoyr"
                                   "hoeadayaoaeaxtantjooeadlncsghykaeykaeykaocyemrewytyaahdvajojkidjyzmadaeldaoaeaeaead"
                                   "fyongawdtbftsoswmkkgmhiymoaywpvwwlfhdeuegrctmsreveaxjznlheflqdtladaeaeaeaezmzmzmzma"
                                   "ogdsraeaeaeaeaeaecpgycxpdbsvwdilkgmsgprvtnbsojodibwynctvswzwzfdeeimjnkisfbgfeeolkbb"
                                   "wpcmasutamaeaeaeaeaecpgycxjypkbkgewyztahdpbaiawetnhthenstyptolprfezstefmoemwaysedyv"
                                   "omehnfpaeaeaeaeaeadaddnehonataeaeaeaeaecpgycxjypkbkgewyztahdpbaiawetnhthenstyptolpr"
                                   "fezstefmoemwaysedyvomehnfpadchcxrydmprlodecynnknbzwplyroremuwzfxrhjlrftdpaimcygdnsc"
                                   "wlturmdztgansaeaeaeahiyglflgmfphffetaoysronadtpdagdchkewlsoeymnlpjevsoxrokifzkkfmcm"
                                   "aotaoyrhoeadayaoaeaxtantjooeadlncsghykaeykaeykaocyemrewytyaahkaonljojkidjyzmadaekia"
                                   "oaeaeaeadkgetmswzntuyfpolskwfaapsleatluihkpzojnahlronbslurhspyawtfgparsgmaeaeaeaeae"
                                   "zmzmzmzmaossasaeaeaeaeaeaecmaebbhpvydsdktilednfwfzmdtsrtjpclsreegdttgrwnaaolaeaeaea"
                                   "eaeaecpgycxlggdpltlrfntdmidtbhydipednstoxdkpscfamutdpstoendfslorostrygddsvtaeaeaeae"
                                   "aeadaddngdsraeaeaeaeaeaecpgycxpdbsvwdilkgmsgprvtnbsojodibwynctvswzwzfdeeimjnkisfbgf"
                                   "eeolkbbwpcmfwbzrtgdmondjysenbgaghrllugrhnecwlknhyatlehtbsdewpmttlflrswynytolaftrtay"
                                   "nyhefywllkayuycprkbsatvthfkstibzqzkknladhshknnahsrehenfnnsiozezckkadcxrydmprlodecyn"
                                   "nknbzwplyroremuwzfxrhjlrftdpaimcygdnscwlturmdztganspmcxtdfndwdavyztyazcceclrhoxaose"
                                   "nndmdynnguckfewldlpawllahphnhfpfsfkopmcxbkwyahaspajnrlcenlmoetoxlfkirhfegmishkpafnm"
                                   "dfdknqziodaeckenynedapscxbyfnfteypttecxrlclmhnbgeaobkbtqdmsjtwfinjpioeyhdwlotleengw"
                                   "fssrpfrdcxchmocewnhfsfqzvdfsfwmynljtttcwdkgubwvlkbdisokspsgtdwsacksgfgjpverdcxfrrhf"
                                   "sztluhslokiktctendywlolfmmssbpetksfksgoimflgtyaftehnbwsldnsrdcxfzpepeflsszmonjnvsie"
                                   "betpvekgpkdnrpwtgrhngwgloefxcnjkkiuofhvtmourrdcxkkosctzcjsskaxwsdmdlmerftkspzttnkkf"
                                   "gwkihfnwsbtnefsuecxkkhywfrhwtrdcxtdctpeksswkpcybtetvaryladerhatzmatwlpdinoxfhspemtb"
                                   "qdyaurynbynyenrdcxykcfnnzsvlwzlurodkkocmftkbfelkkntyfetarszoamlfttbtfruydwqzctmnmnr"
                                   "dcxzsntlodpfewkambduylafwcsetdesnltghgwckptmsetbahdjzpykttlzcinltemrdhfnsrtadchcxgd"
                                   "mondjysenbgaghrllugrhnecwlknhyatlehtbsdewpmttlflrswynytolaftrtaeaeaeahiyglflgmfphff"
                                   "etaoysronadtpdagdmeesdwwydelplnzotodavdpsfshfrelaaotaoyrhoeadayaoaeaxtantjooeadlncs"
                                   "ghykaeykaeykaocyemrewytyaahkaonljojkidjyzmadaekiaoaeaeaeadkgkgdijlgyjtdykedslrrhred"
                                   "mvdwsfljeseykcljopfdloyurbttnesrpbtgyonaeaeaeaeaezmzmzmzmaohnasaeaeaeaeaeaecmaebbhp"
                                   "vydsdktilednfwfzmdtsrtjpclsreegdttgrwnmknnaeaeaeaeaeaecpgycxlggdpltlrfntdmidtbhydip"
                                   "ednstoxdkpscfamutdpstoendfslorostrygddsvtaeaeaeaeaeadaddnlarkaeaeaeaeaeaecpgycxmohe"
                                   "jthldlghmolgrkylstrhldvotivsrpaofrpltojeknvawswebngwghldcfnlfwbzrtgdmondjysenbgaghr"
                                   "llugrhnecwlknhyatlehtbsdewpmttlflrswynytolaftrtlrcsdsbksgaxlbkspklrgmfxwtvovesaadwf"
                                   "enaoiyylcebwbkclguhyioaxgyytzckkadcxrydmprlodecynnknbzwplyroremuwzfxrhjlrftdpaimcyg"
                                   "dnscwlturmdztganspmcxtdfndwdavyztyazcceclrhoxaosenndmdynnguckfewldlpawllahphnhfpfsf"
                                   "kopmcxbkwyahaspajnrlcenlmoetoxlfkirhfegmishkpafnmdfdknqziodaeckenynedapscxbyfnfteyp"
                                   "ttecxrlclmhnbgeaobkbtqdmsjtwfinjpioeyhdwlotleengwfssrpfrdcxchmocewnhfsfqzvdfsfwmynl"
                                   "jtttcwdkgubwvlkbdisokspsgtdwsacksgfgjpverdcxfrrhfsztluhslokiktctendywlolfmmssbpetks"
                                   "fksgoimflgtyaftehnbwsldnsrdcxfzpepeflsszmonjnvsiebetpvekgpkdnrpwtgrhngwgloefxcnjkki"
                                   "uofhvtmourrdcxkkosctzcjsskaxwsdmdlmerftkspzttnkkfgwkihfnwsbtnefsuecxkkhywfrhwtrdcxt"
                                   "dctpeksswkpcybtetvaryladerhatzmatwlpdinoxfhspemtbqdyaurynbynyenrdcxykcfnnzsvlwzluro"
                                   "dkkocmftkbfelkkntyfetarszoamlfttbtfruydwqzctmnmnrdcxzsntlodpfewkambduylafwcsetdesnl"
                                   "tghgwckptmsetbahdjzpykttlzcinltemrdhfnsrtadchcxgdmondjysenbgaghrllugrhnecwlknhyatle"
                                   "htbsdewpmttlflrswynytolaftrtaeaeaeahiyglflgmfphffedmuretlf";
    
    BatchSignRequest testRequests;
    testRequests.fromUr(requestsUr);
    
    SignRequest expectedSignRequest0;
    expectedSignRequest0.setRequestID(Uuid("3B5414375E3A450B8FE1251CBC2B3FB5"));
    expectedSignRequest0.setCoinIdentity(CoinIdentity(EllipticCurve::Secp256k1, 0));
    expectedSignRequest0.setDerivationPath(Keypath("m/84'/0'/0'", 934670036));
    expectedSignRequest0.setSignData(convertQByteArrayToVector(QByteArray::fromHex("70736274ff010089020000000144a549ead63ac9c6987b90669208ece5e93f28de4b1f97b5e4036c995f47b3d50100000000ffffffff0250c3000000000000225120a80fe5278c52cab2e0a0c9702713f61fe8f2f248346a6d7dcc1245338c14ec1609dd06000000000022512074aa0a4aeefc052d0e63edda5a5f9cd4a9a6b245fad33ea29408c130e2916041000000000001012b31a507000000000022512074aa0a4aeefc052d0e63edda5a5f9cd4a9a6b245fad33ea29408c130e2916041011720bd2eb288281a9e7a15ec81b8b593f243b96fbcd2b16a1a509c1b87df95fc499c000000")));
    expectedSignRequest0.setOrigin("NGRAVE");

    SignRequest expectedSignRequest1;
    expectedSignRequest1.setRequestID(Uuid("177ce9c9328e856be8a4b87d40793e16"));
    expectedSignRequest1.setCoinIdentity(CoinIdentity(EllipticCurve::Secp256k1, 0));
    expectedSignRequest1.setDerivationPath(Keypath("m/84'/0'/0'", 934670036));
    expectedSignRequest1.setSignData(convertQByteArrayToVector(QByteArray::fromHex("70736274ff01007d02000000017b3897f29ddb41a6c5f304ac8a078b6575fb6d0584a50f8bb9c8f8f046b1bf520000000000ffffffff02c4090000000000001600145be12624d08a2b424095d7c07221c33450d14bf104a60000000000002251208d50aed5bc9d2e62d65e27af2bc7a424ac1906dd2dc7a29b3d88b8c7bd5026e0000000000001012b50c3000000000000225120a80fe5278c52cab2e0a0c9702713f61fe8f2f248346a6d7dcc1245338c14ec164215c050929b74c1a04954b78b4b6035e97a5e078a5a0f28ec96d547bfee9ace803ac0089a5f44e98c08db22bb0f07e05678d015b479990161599e05c331363c9c67fefd790120bd2eb288281a9e7a15ec81b8b593f243b96fbcd2b16a1a509c1b87df95fc499cad20d23c2c25e1fcf8fd1c21b9a402c19e2e309e531e45e92fb1e9805b6056b0cc76ad200aee0509b16db71c999238a4827db945526859b13c95487ab46725357c9a9f25ac20113c3a32a9d320b72190a04a020a0db3976ef36972673258e9a38a364f3dc3b0ba2017921cf156ccb4e73d428f996ed11b245313e37e27c978ac4d2cc21eca4672e4ba203bb93dfc8b61887d771f3630e9a63e97cbafcfcc78556a474df83a31a0ef899cba2040afaf47c4ffa56de86410d8e47baa2bb6f04b604f4ea24323737ddc3fe092dfba2079a71ffd71c503ef2e2f91bccfc8fcda7946f4653cef0d9f3dde20795ef3b9f0ba20d21faf78c6751a0d38e6bd8028b907ff07e9a869a43fc837d6b3f8dff6119a36ba20f5199efae3f28bb82476163a7e458c7ad445d9bffb0682d10d3bdb2cb41f8e8eba20fa9d882d45f4060bdb8042183828cd87544f1ea997380e586cab77d5fd698737ba569cc001172050929b74c1a04954b78b4b6035e97a5e078a5a0f28ec96d547bfee9ace803ac0000000")));
    expectedSignRequest1.setOrigin("NGRAVE");
    
    SignRequest expectedSignRequest2;
    expectedSignRequest2.setRequestID(Uuid("91392cee288586fbce25e7ac3d56b580"));
    expectedSignRequest2.setCoinIdentity(CoinIdentity(EllipticCurve::Secp256k1, 0));
    expectedSignRequest2.setDerivationPath(Keypath("m/84'/0'/0'", 934670036));
    expectedSignRequest2.setSignData(convertQByteArrayToVector(QByteArray::fromHex("70736274ff01007d02000000017b7b276f516e307c2684b9b52ee7ef476bc1f52170b02fa1df0dda39b60d51a50000000000ffffffff0260090000000000001600145be12624d08a2b424095d7c07221c33450d14bf1989e0000000000002251208d50aed5bc9d2e62d65e27af2bc7a424ac1906dd2dc7a29b3d88b8c7bd5026e0000000000001012b80bb000000000000225120925f6e5d2f54928dbbf7c7b989e2d0e8b6023baece6b7ae6efed0c4f548919994215c050929b74c1a04954b78b4b6035e97a5e078a5a0f28ec96d547bfee9ace803ac08418260aca037f78aa845243f0e2e4c201f3360266f71c130a21535e670351f9fd790120bd2eb288281a9e7a15ec81b8b593f243b96fbcd2b16a1a509c1b87df95fc499cad20d23c2c25e1fcf8fd1c21b9a402c19e2e309e531e45e92fb1e9805b6056b0cc76ad200aee0509b16db71c999238a4827db945526859b13c95487ab46725357c9a9f25ac20113c3a32a9d320b72190a04a020a0db3976ef36972673258e9a38a364f3dc3b0ba2017921cf156ccb4e73d428f996ed11b245313e37e27c978ac4d2cc21eca4672e4ba203bb93dfc8b61887d771f3630e9a63e97cbafcfcc78556a474df83a31a0ef899cba2040afaf47c4ffa56de86410d8e47baa2bb6f04b604f4ea24323737ddc3fe092dfba2079a71ffd71c503ef2e2f91bccfc8fcda7946f4653cef0d9f3dde20795ef3b9f0ba20d21faf78c6751a0d38e6bd8028b907ff07e9a869a43fc837d6b3f8dff6119a36ba20f5199efae3f28bb82476163a7e458c7ad445d9bffb0682d10d3bdb2cb41f8e8eba20fa9d882d45f4060bdb8042183828cd87544f1ea997380e586cab77d5fd698737ba569cc001172050929b74c1a04954b78b4b6035e97a5e078a5a0f28ec96d547bfee9ace803ac0000000")));
    expectedSignRequest2.setOrigin("NGRAVE");
    
    BatchSignRequest expectedRequests({expectedSignRequest0, expectedSignRequest1, expectedSignRequest2});
    
    Compare(testRequests, expectedRequests);
}

void TestBatchSignRequest::Compare(const BatchSignRequest& decodedBatchSignRequest, const BatchSignRequest& expectedBatchSignRequest)
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Decoded and expected sign-request array length are not the same",
                                 decodedBatchSignRequest.getSignRequests().size(),
                                 expectedBatchSignRequest.getSignRequests().size()
                                 );
    
    for(size_t i = 0; i < expectedBatchSignRequest.getSignRequests().size(); i++)
        TestSignRequest::Compare(decodedBatchSignRequest.getSignRequests()[0], expectedBatchSignRequest.getSignRequests()[0]);    
}
