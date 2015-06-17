#include <iostream>
#include <vector>
#include <map>
#include "../reflection++/visitor_chain.hpp"
#include "../reflection++/visitor/json.hpp"
#include "../reflection++/accessor_infer.hpp"
#include "../reflection++/meta.hpp"

namespace rpp_another_namespace {

RPP_ACCESSOR_INFER_INIT()

struct TestStruct3 {
    int a{1};
    double b{2};

    TestStruct3() = default;
};

struct TestStruct4 {
    TestStruct3 c;
    std::string d{"string\t字符串\n"};
    std::vector<float> e{5, 5.1, 5.2};
    std::map<std::string, unsigned> f{{"item1", 3}, {"item2", 4}};
    int g[3]{7, 8, 9};
    bool h{true};

    TestStruct4() = default;
};

RPP_ACCESSOR_BIND_OBJECT(
    TestStruct3,
    __(a)
    __(b)
)

RPP_ACCESSOR_BIND_OBJECT(
    TestStruct4,
    __(c)
    __(d)
    __(e)
    __(f)
    __(g)
    __(h)
)

static const int test2 = []() {
    rpp::MetaImpl<
        rpp::TypeList<rpp::VisitorJSON<>>,
        RPP_ACCESSOR_GET(
            RPP_HOLDER_STR("value"),
            RPP_HOLDER_LOCAL(TestStruct4)
        )
    > meta{TestStruct4{}};

    rpp::VisitorJSON<> v{std::cerr};

    meta.doVisit(v);

    return 0;
}();

}
