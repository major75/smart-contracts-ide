#include <boost/test/unit_test.hpp>
#include <ion/chain/abi_serializer.hpp>
#include <ion/chain/permission_object.hpp>
#include <ion/chain/resource_limits.hpp>
#include <ion/testing/tester.hpp>

using namespace ion;
using namespace ion::chain;
using namespace ion::testing;
using namespace fc;

BOOST_AUTO_TEST_SUITE(talk_tests)

BOOST_AUTO_TEST_CASE(post) try {
    tester t; // {setup_policy::none};

    // Load contract
    t.create_account(N(talk));
    t.set_code(N(talk), read_wasm("talk.wasm"));
    t.set_abi(N(talk), read_abi("talk.abi").data());
    t.produce_block();

    // Create users
    t.create_account(N(john));
    t.create_account(N(jane));

    // Test "post" action
    t.push_action(
        N(talk), N(post), N(john),
        mutable_variant_object //
        ("id", 1)              //
        ("reply_to", 0)        //
        ("user", "john")       //
        ("content", "post 1")  //
    );
    t.push_action(
        N(talk), N(post), N(jane),
        mutable_variant_object //
        ("id", 2)              //
        ("reply_to", 0)        //
        ("user", "jane")       //
        ("content", "post 2")  //
    );
    t.push_action(
        N(talk), N(post), N(john),
        mutable_variant_object       //
        ("id", 3)                    //
        ("reply_to", 2)              //
        ("user", "john")             //
        ("content", "post 3: reply") //
    );

    // Can't reply to non-existing message
    BOOST_CHECK_THROW(
        [&] {
            t.push_action(
                N(talk), N(post), N(john),
                mutable_variant_object       //
                ("id", 4)                    //
                ("reply_to", 99)             //
                ("user", "john")             //
                ("content", "post 3: reply") //
            );
        }(),
        fc::exception);
}
FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
