#ifndef TEST_CONTAINER_NAMES_H
#define TEST_CONTAINER_NAMES_H

#include "tests_preparing.h"

using namespace std;
using namespace s11n;


//=======================================================================================
TEST_F( Test_s11n, name_of_containers )
{
    static_assert ( is_same<std::string::value_type,char>::value, "" );
    EXPECT_EQ( signature< string >(), "std::string<char>" );

    EXPECT_EQ( signature< vector<bool>              >(), "std::vector<bool>"        );
    EXPECT_EQ( signature< deque<bool>               >(), "std::deque<bool>"         );
    EXPECT_EQ( signature< valarray<bool>            >(), "std::valarray<bool>"      );
    EXPECT_EQ( signature< unordered_set<bool>       >(), "std::unordered_set<bool>" );
    EXPECT_EQ( signature< list<char>                >(), "std::list<char>"          );
    EXPECT_EQ( signature< set<bool>                 >(), "std::set<bool>"           );
    EXPECT_EQ( signature< multiset<char>            >(), "std::multiset<char>"      );
    EXPECT_EQ( signature< unordered_set<char>       >(), "std::unordered_set<char>" );

    EXPECT_EQ( signature< unordered_multiset<char>  >(),
                    "std::unordered_multiset<char>" );

    // Надо делать using, иначе ломается макрос сравнения.
    using AT   = array<bool,13>;

    using MT   = map<int8_t,bool>;
    using MMT  = multimap<int8_t,bool>;

    using UMT  = unordered_map<int8_t,bool>;
    using UMMT = unordered_multimap<int8_t,bool>;


    EXPECT_EQ( signature< AT   >(), "std::array<bool>"                      );
    EXPECT_EQ( signature< MT   >(), "std::map<{int8,bool}>"                 );
    EXPECT_EQ( signature< MMT  >(), "std::multimap<{int8,bool}>"            );
    EXPECT_EQ( signature< UMT  >(), "std::unordered_map<{int8,bool}>"       );
    EXPECT_EQ( signature< UMMT >(), "std::unordered_multimap<{int8,bool}>"  );

    // "Не является" контейнером, т.к. не имеет метода size(). Пишите письма...
    // vdeb << signature< forward_list<bool> >();
    // EXPECT_EQ( signature< forward_list<bool> >(), "std::forward_list<bool>"    );
}
//=======================================================================================
namespace test_ns
{
    struct OwnType2{};
}

TEST_F( Test_s11n, crcs_of_containers )
{
    EXPECT_EQ( signature_crc< string >(), own_crc<string>() );

    // Надо делать using, иначе ломается макрос сравнения.
    using V  = vector<test_ns::OwnType2>;
    using D  = deque<test_ns::OwnType2>;
    using VA = valarray<test_ns::OwnType2>;
    using S  = set<int64_t>;
    using US = unordered_set<uint32_t>;
    using A  = array<test_ns::OwnType2,13>;
    using M  = map<int8_t,test_ns::OwnType2>;
    using UM = unordered_map<int8_t,test_ns::OwnType2>;

    EXPECT_EQ( signature_crc< V  >(), own_crc< V  >() );
    EXPECT_EQ( signature_crc< D  >(), own_crc< D  >() );
    EXPECT_EQ( signature_crc< VA >(), own_crc< VA >() );
    EXPECT_EQ( signature_crc< S  >(), own_crc< S  >() );
    EXPECT_EQ( signature_crc< US >(), own_crc< US >() );
    EXPECT_EQ( signature_crc< A  >(), own_crc< A  >() );
    EXPECT_EQ( signature_crc< M  >(), own_crc< M  >() );
    EXPECT_EQ( signature_crc< UM >(), own_crc< UM >() );
}
//=======================================================================================


#endif // TEST_CONTAINER_NAMES_H
